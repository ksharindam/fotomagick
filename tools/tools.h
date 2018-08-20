#ifndef TOOLS_H
#define TOOLS_H

#include <core/core.hpp>
#include <imgproc/imgproc.hpp>

#include <QPixmap>
#include <QMouseEvent>
#include <QPainter>
#include <QSlider>

QT_BEGIN_NAMESPACE

cv::Mat qImage2Mat(QImage img);
QImage  mat2QImage(cv::Mat const& mat);

/* This is the base class for all tools
TODO : manage image scaling
*/
class Tool : public QObject
{
    Q_OBJECT
public:
    QPixmap pixmap;
    float scaleFactor;
    QColor fg_color;
    QColor bg_color;

    Tool(QObject *parent) : QObject(parent) {};
    virtual void init(QPixmap pixmap, float scale, QColor fg, QColor bg) = 0;
    virtual void finish() = 0;
protected:
    QPainter painter;
public slots:
    virtual void onMousePress(QPoint) = 0;
    virtual void onMouseRelease(QPoint) = 0;
    virtual void onMouseMove(QPoint) = 0;
signals:
    void imageChanged(QPixmap);
    void canvasUpdated(QPixmap);
    void maskCreated(QImage);       // for selection tools only
};

//************************* Pencil Tool ************************
class PencilTool : public Tool
{
    Q_OBJECT
public:
    PencilTool(QObject *parent) : Tool(parent) { };
    void init(QPixmap pixmap, float scale, QColor fg, QColor bg);
    void finish() {};
    void onMousePress(QPoint);
    void onMouseRelease(QPoint);
    void onMouseMove(QPoint);
private:
    QPoint start;
    bool mouse_pressed = false;

};

//************************ Brush Tool *************************
class BrushManager : public QWidget
{
    Q_OBJECT
public:
    BrushManager(QWidget *parent);
    QSlider *thicknessSlider;
private slots:
    void onValueChange(int);
signals:
    void settingsChanged();
};

class BrushTool : public Tool
{
    Q_OBJECT
public:
    BrushTool(QObject *parent);
    void init(QPixmap pixmap, float scale, QColor fg, QColor bg);
    void finish();
    void onMousePress(QPoint);
    void onMouseRelease(QPoint);
    void onMouseMove(QPoint);
private:
    QPoint start;
    QPen pen;
    bool mouse_pressed = false;
    BrushManager *brushManager;
private slots:
    void onSettingsChange();
};

// *************************** Eraser Tool ******************************


class EraserTool : public Tool
{
    Q_OBJECT
public:
    EraserTool(QObject *parent);
    void init(QPixmap pixmap, float scale, QColor fg, QColor bg);
    void finish();
    void onMousePress(QPoint);
    void onMouseRelease(QPoint);
    void onMouseMove(QPoint);
private:
    QPoint start;
    QPen pen;
    bool mouse_pressed = false;
    BrushManager *brushManager;
private slots:
    void onSettingsChange();
};



// *************************** Floodfill Tool ***************************
class FloodfillTool : public Tool
{
    Q_OBJECT
public:
    FloodfillTool(QObject *parent) : Tool(parent) {};
    void init(QPixmap pixmap, float scale, QColor fg, QColor bg);
    void finish() {};
    void onMousePress(QPoint);
    void onMouseRelease(QPoint) {};
    void onMouseMove(QPoint) {};
};

void floodfill(QImage &img, int x, int y, QRgb newColor);

QT_END_NAMESPACE

#endif