#include "main.h"
#include <QApplication>
#include <QFileDialog>
#include <QDebug>

Window:: Window()
{
    setupUi(this);
    QHBoxLayout *layout = new QHBoxLayout(scrollAreaWidgetContents);
    layout->setContentsMargins(0, 0, 0, 0);
    canvas = new Canvas(this);
    layout->addWidget(canvas);
    filters = new Filters(this);
    toolBtnGr = new QButtonGroup(this);
    toolBtnGr->addButton(pencilBtn);    // Button ids are added in this order... -2,-3,-4...
    toolBtnGr->addButton(brushBtn);
    pencilTool = new PencilTool(frameColor);
    brushTool = new BrushTool(frameColor);
    toolList << pencilTool << brushTool;
    connectSignals();
    pencilBtn->setChecked(true);
    onToolClick(-2);
}

void
Window:: connectSignals()
{
    connect(actionOpen, SIGNAL(triggered()), this, SLOT(openFile()));
    connect(actionSave, SIGNAL(triggered()), this, SLOT(saveFile()));
    connect(actionQuit, SIGNAL(triggered()), this, SLOT(close()));
    connect(actionZoom_In, SIGNAL(triggered()), this, SLOT(zoomIn()));
    connect(actionZoom_Out, SIGNAL(triggered()), this, SLOT(zoomOut()));
    // connect filters slots
    connect(actionAuto_Equalize, SIGNAL(triggered()), filters, SLOT(applyAutoEqualize()));
    connect(actionAuto_Contrast, SIGNAL(triggered()), filters, SLOT(applyAutoContrast()));
    // signals from tool buttons
    connect(toolBtnGr, SIGNAL(buttonClicked(int)), this, SLOT(onToolClick(int)));
    foreach (Tool *tool, toolList) {
        connect(tool, SIGNAL(canvasUpdated(QPixmap)), canvas, SLOT(onCanvasUpdate(QPixmap)));
        connect(tool, SIGNAL(imageChanged(QPixmap)),  this,         SLOT(onImageChange(QPixmap)));
    }
    // signals from tools and layer_manager
    connect(filters, SIGNAL(imageChanged(QPixmap)), this, SLOT(onImageChange(QPixmap))); // TODO : use qimage
    connect(canvas, SIGNAL(imageChanged(QPixmap)), this, SLOT(onImageChange(QPixmap)));
}

void
Window:: onToolClick(int btn_id)
{
    if (prev_btn!=0) {
        prev_btn = abs(prev_btn)-2;
        toolList[prev_btn]->finish();
        disconnect(canvas, SIGNAL(mousePressed(QPoint)), toolList[prev_btn], SLOT(onMousePress(QPoint)));
        disconnect(canvas, SIGNAL(mouseReleased(QPoint)), toolList[prev_btn], SLOT(onMouseRelease(QPoint)));
        disconnect(canvas, SIGNAL(mouseMoved(QPoint)), toolList[prev_btn], SLOT(onMouseMove(QPoint)));
    }
    toolList[abs(btn_id)-2]->init(canvas->topLayer(), scaleFactor,Qt::black, Qt::white);
    connect(canvas, SIGNAL(mousePressed(QPoint)), toolList[abs(btn_id)-2], SLOT(onMousePress(QPoint)));
    connect(canvas, SIGNAL(mouseReleased(QPoint)), toolList[abs(btn_id)-2], SLOT(onMouseRelease(QPoint)));
    connect(canvas, SIGNAL(mouseMoved(QPoint)), toolList[abs(btn_id)-2], SLOT(onMouseMove(QPoint)));
    prev_btn = btn_id;
}

void
Window:: setScale(float scale)
{
    scaleFactor = scale;
    toolList[abs(prev_btn)-2]->setScale(scaleFactor);
    canvas->setScale(scaleFactor);
}

void
Window:: zoomIn()
{
    setScale(scaleFactor*6/5);
    qDebug() << "zoom in" << scaleFactor;
}

void
Window:: zoomOut()
{
    setScale(scaleFactor*5/6);
    qDebug() << "zoom out" << scaleFactor;
}

void
Window:: onImageChange(QPixmap pm)
{
    canvas->onImageChange(pm);
    filters->setPixmap(pm);
    toolList[abs(prev_btn)-2]->setPixmap(pm);
}

void
Window:: openFile()
{
    QString filename = QFileDialog::getOpenFileName(this, "Open Image", "",
                "Image files (*.jpg *.png );;JPG Images (*.jpg *.jpeg);;PNG Images (*.png);;All files (*.*)" );
    if (!filename.isEmpty())
        openFile(filename);
}

void
Window:: openFile(QString filename)
{
    QImage new_image(filename);
    if (new_image.isNull()) return;

    onImageChange(new_image);
}

void
Window:: saveFile()
{
    QString filename = QFileDialog::getSaveFileName(this, "Save Image", "",
                "Image files (*.jpg *.png );;JPG Images (*.jpg *.jpeg);;PNG Images (*.png);;All files (*.*)" );
    if (!filename.isEmpty())
        canvas->topLayer().save(filename);
}


int main(int argc, char *argv[])
{
    Magick::InitializeMagick(*argv);
    QApplication app(argc, argv);
    Window *win = new Window();
    win->resize(640, 600);
    win->openFile("/home/subha/Arindam.jpg");
    win->show();
    return app.exec();
}
