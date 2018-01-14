#ifndef TOOLS_H
#define TOOLS_H
#include <QPixmap>
#include <QMouseEvent>
#include <QPoint>
#include <QPainter>

QT_BEGIN_NAMESPACE

/* This is the base class for all tools*/
class Tool : public QObject
{
    Q_OBJECT
public:
    Tool(QObject *parent);
    virtual void init(QPixmap& pixmap, QColor fg, QColor bg) = 0;
    virtual void finish() = 0;
    virtual void setColors(QColor fg, QColor bg) = 0;
private slots:
    virtual void onMousePress(QPoint) = 0;
    virtual void onMouseRelease(QPoint) = 0;
    virtual void onMouseMove(QPoint) = 0;
signals:
    void imageChanged(QPixmap);
    void canvasUpdated(QPixmap);
};

class PencilTool : public Tool
{
    Q_OBJECT
public:
    PencilTool(QObject *parent);
    void init(QPixmap& pixmap, QColor fg, QColor bg);
    void finish();
    void setColors(QColor fg, QColor bg);
private:
    QPoint start;
    QPixmap pixmap;
    QPainter painter;
    bool mouse_pressed;
    QColor fg_color;

    void onMousePress(QPoint);
    void onMouseRelease(QPoint);
    void onMouseMove(QPoint);
signals:
    void imageChanged(QPixmap);
    void canvasUpdated(QPixmap);
};

class BrushTool : public Tool
{
    Q_OBJECT
public:
    BrushTool(QObject *parent);
    void init(QPixmap& pixmap, QColor fg, QColor bg);
    void finish();
    void setColors(QColor fg, QColor bg);
private:
    QPoint start;
    QPixmap pixmap;
    QPainter painter;
    bool mouse_pressed;
    QColor fg_color;

    void onMousePress(QPoint);
    void onMouseRelease(QPoint);
    void onMouseMove(QPoint);
signals:
    void imageChanged(QPixmap);
    void canvasUpdated(QPixmap);
};

QT_END_NAMESPACE

#endif