#include "main.h"
#include "tools/iscissor_tool.h"
#include "tools/grabcut_tool.h"
#include <QApplication>
#include <QFileDialog>
#include <QColorDialog>
#include <QDebug>

Window:: Window()
{
    setupUi(this);
    colorChooser = new ColorChooser(frameColor);
    frameColor->layout()->addWidget(colorChooser);
    canvas = new Canvas(graphicsView);
    filters = new Filters(this);
    // group all tool buttons 
    toolBtnGr = new QButtonGroup(this);
    toolBtnGr->addButton(grabcutBtn);
    toolBtnGr->addButton(iscissorBtn);
    toolBtnGr->addButton(pencilBtn);    // Button ids are added in this order... -2,-3,-4...
    toolBtnGr->addButton(brushBtn);
    toolBtnGr->addButton(eraserBtn);
    toolBtnGr->addButton(floodfillBtn);
    /*toolBtnGr->addButton(sprayBtn);
    toolBtnGr->addButton(textBtn);*/
    // create tools
    PencilTool *pencilTool = new PencilTool(frameColor);
    BrushTool *brushTool = new BrushTool(frameColor);
    EraserTool *eraserTool = new EraserTool(frameColor);
    FloodfillTool *floodfillTool = new FloodfillTool(frameColor);
    IScissorTool *iscissorTool = new IScissorTool(frameColor);
    GrabcutTool *grabcutTool = new GrabcutTool(frameColor);
    toolList << grabcutTool << iscissorTool << pencilTool << brushTool << eraserTool << floodfillTool;

    connectSignals();
    pencilBtn->setChecked(true);
    onToolClick(-4);
}

void
Window:: connectSignals()
{
    connect(actionOpen, SIGNAL(triggered()), this, SLOT(openFile()));
    connect(actionSave, SIGNAL(triggered()), this, SLOT(saveFile()));
    connect(actionQuit, SIGNAL(triggered()), this, SLOT(close()));
    connect(actionZoom_In, SIGNAL(triggered()), this, SLOT(zoomIn()));
    connect(actionZoom_Out, SIGNAL(triggered()), this, SLOT(zoomOut()));
    connect(actionRotate_Left, SIGNAL(triggered()), canvas, SLOT(rotateLeft()));
    connect(actionRotate_Right, SIGNAL(triggered()), canvas, SLOT(rotateRight()));
    connect(actionFlip_Image, SIGNAL(triggered()), canvas, SLOT(flip()));
    connect(actionFlop_Image, SIGNAL(triggered()), canvas, SLOT(flop()));
    // connect layer slots
    connect(actionDuplicate_Layer, SIGNAL(triggered()), canvas, SLOT(duplicateLayer()));
    connect(actionDelete_Layer, SIGNAL(triggered()), canvas, SLOT(deleteLayer()));
    // connect filters slots
    connect(actionAuto_Equalize, SIGNAL(triggered()), filters, SLOT(applyAutoEqualize()));
    connect(actionAuto_Contrast, SIGNAL(triggered()), filters, SLOT(applyAutoContrast()));
    connect(actionGray_Scale, SIGNAL(triggered()), filters, SLOT(applyGrayScale()));
    // signals from tool buttons
    connect(toolBtnGr, SIGNAL(buttonClicked(int)), this, SLOT(onToolClick(int)));
    foreach (Tool *tool, toolList) {
        connect(tool, SIGNAL(canvasUpdated(QPixmap)), canvas, SLOT(onCanvasUpdate(QPixmap)));
        connect(tool, SIGNAL(imageChanged(QPixmap)),  this,         SLOT(onImageChange(QPixmap)));
    }
    // signals from tools and layer_manager
    connect(filters, SIGNAL(imageChanged(QPixmap)), this, SLOT(onImageChange(QPixmap))); // TODO : use qimage
    connect(canvas, SIGNAL(imageChanged(QPixmap)), this, SLOT(onImageChange(QPixmap)));
    connect(colorChooser, SIGNAL(colorChanged()), this, SLOT(onColorChange()));
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
    toolList[abs(btn_id)-2]->init(canvas->topLayer(), canvas->scaleFactor,colorChooser->fg_color, colorChooser->bg_color);
    connect(canvas, SIGNAL(mousePressed(QPoint)), toolList[abs(btn_id)-2], SLOT(onMousePress(QPoint)));
    connect(canvas, SIGNAL(mouseReleased(QPoint)), toolList[abs(btn_id)-2], SLOT(onMouseRelease(QPoint)));
    connect(canvas, SIGNAL(mouseMoved(QPoint)), toolList[abs(btn_id)-2], SLOT(onMouseMove(QPoint)));
    prev_btn = btn_id;
}

void
Window:: onColorChange()
{
    toolList[abs(prev_btn)-2]->fg_color = colorChooser->fg_color;
    toolList[abs(prev_btn)-2]->bg_color = colorChooser->bg_color;
}

void
Window:: setScale(float scale)
{
    canvas->setScale(scale);
    toolList[abs(prev_btn)-2]->scaleFactor = scale;
}

void
Window:: zoomIn()
{
    setScale(canvas->scaleFactor*6/5);
}

void
Window:: zoomOut()
{
    setScale(canvas->scaleFactor*5/6);
}

void
Window:: onImageChange(QPixmap pm)
{
    canvas->onImageChange(pm);
    filters->pixmap = pm;
    toolList[abs(prev_btn)-2]->pixmap = pm;
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

// *********************** Color Chooser Widget **********************
ColorChooser:: ColorChooser(QWidget *parent) : QLabel(parent)
{
    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    setMouseTracking(true);
    fg_color = Qt::black;
    bg_color = Qt::white;
    setColors();
}

void
ColorChooser:: mousePressEvent(QMouseEvent *ev)
{
    QColor color;
    if ( QRect(0,0,25,24).contains(ev->pos()) ) {
        color = QColorDialog::getColor(fg_color, this);
        if (color.isValid()) {
            fg_color = color;
            emit colorChanged();
        }
    }
    else {
        color = QColorDialog::getColor(bg_color, this);
        if (color.isValid()) {
            bg_color = color;
            emit colorChanged();
        }
    }
    setColors();
}

void
ColorChooser:: setColors()
{
    QPixmap pm(50, 24);
    pm.fill(bg_color);
    QPainter painter(&pm);
    painter.setPen(fg_color);
    painter.setBrush(fg_color);
    painter.drawRect(0,0,24,23);
    painter.end();
    setPixmap(pm);
}

// ********************* Main Function *******************
int main(int argc, char *argv[])
{
    Magick::InitializeMagick(*argv);
    QApplication app(argc, argv);
    Window *win = new Window();
    win->resize(640, 600);
    win->openFile("/home/me/Arindam.jpg");
    win->show();
    return app.exec();
}
