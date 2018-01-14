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
    toolBtnGr = new QButtonGroup(this);
    toolBtnGr->addButton(pencilBtn);    // Button ids are added in this order... -2,-3,-4...
    toolBtnGr->addButton(brushBtn);    // Button ids are added in this order... -2,-3,-4...
    pencilTool = new PencilTool(this);
    brushTool = new BrushTool(this);
    toolList << pencilTool << brushTool;
    connectSignals();
}

void
Window:: connectSignals()
{
    connect(actionOpen, SIGNAL(triggered()), this, SLOT(openFile()));
    connect(actionSave, SIGNAL(triggered()), this, SLOT(saveFile()));
    connect(actionQuit, SIGNAL(triggered()), this, SLOT(close()));
    connect(toolBtnGr, SIGNAL(buttonClicked(int)), this, SLOT(onToolClick(int)));
    foreach (Tool *tool, toolList)
        connect(tool, SIGNAL(canvasUpdated(QPixmap)), canvas, SLOT(setPixmap(QPixmap)));
}

void
Window:: onToolClick(int btn_id)
{
    toolList[abs(btn_id)-2]->init(pixmap, Qt::black, Qt::white);
    connect(canvas, SIGNAL(mousePressed(QPoint)), toolList[abs(btn_id)-2], SLOT(onMousePress(QPoint)));
    connect(canvas, SIGNAL(mouseReleased(QPoint)), toolList[abs(btn_id)-2], SLOT(onMouseRelease(QPoint)));
    connect(canvas, SIGNAL(mouseMoved(QPoint)), toolList[abs(btn_id)-2], SLOT(onMouseMove(QPoint)));
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

    pixmap = QPixmap::fromImage(new_image);
    canvas->setPixmap(pixmap);
}

void
Window:: saveFile()
{
    QString filename = QFileDialog::getSaveFileName(this, "Save Image", "",
                "Image files (*.jpg *.png );;JPG Images (*.jpg *.jpeg);;PNG Images (*.png);;All files (*.*)" );
    if (!filename.isEmpty())
        pixmap.save(filename);
}


int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    Window *win = new Window();
    win->resize(640, 600);
    win->openFile("/home/subha/Arindam.jpg");
    win->show();
    return app.exec();
}
