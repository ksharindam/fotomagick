#ifndef TOOLS_H
#define TOOLS_H
#include <QPixmap>
#include <QMouseEvent>
#include <QPoint>
#include <QPainter>
#include <QSlider>

QT_BEGIN_NAMESPACE

/* This is the base class for all tools
TODO : manage image scaling
*/
class Tool : public QObject
{
    Q_OBJECT
public:
    Tool(QObject *parent);
    void setPixmap(QPixmap& pm);
    virtual void init(QPixmap& pixmap, QColor fg, QColor bg) = 0;
    virtual void finish() = 0;
    virtual void setColors(QColor fg, QColor bg) = 0;

    QPixmap pixmap;
public slots:
    virtual void onMousePress(QPoint) = 0;
    virtual void onMouseRelease(QPoint) = 0;
    virtual void onMouseMove(QPoint) = 0;
signals:
    void imageChanged(QPixmap);
    void canvasUpdated(QPixmap);
    void maskCreated(QImage);       // for selection tools only
};

class PencilTool : public Tool
{
    Q_OBJECT
public:
    PencilTool(QObject *parent);
    void init(QPixmap& pixmap, QColor fg, QColor bg);
    void finish();
    void setColors(QColor fg, QColor bg);
    void onMousePress(QPoint);
    void onMouseRelease(QPoint);
    void onMouseMove(QPoint);
private:
    QPoint start;
    QPainter painter;
    bool mouse_pressed;
    QColor fg_color;

};

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
    void init(QPixmap& pixmap, QColor fg, QColor bg);
    void finish();
    void setColors(QColor fg, QColor bg);
    void onMousePress(QPoint);
    void onMouseRelease(QPoint);
    void onMouseMove(QPoint);
private:
    QPoint start;
    QPen pen;
    QPainter painter;
    bool mouse_pressed;
    QColor fg_color;
    BrushManager *brushManager;
private slots:
    void onSettingsChange();
};

QT_END_NAMESPACE

#endif