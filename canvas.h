#ifndef CANVAS_H
#define CANVAS_H

#include <QLabel>
#include <QMouseEvent>

QT_BEGIN_NAMESPACE

class Canvas : public QLabel
{
    Q_OBJECT
public:
    Canvas(QWidget *parent);
    void mousePressEvent(QMouseEvent *ev);
    void mouseReleaseEvent(QMouseEvent *ev);
    void mouseMoveEvent(QMouseEvent *ev);
    void setImage(QImage image);
signals:
    void mousePressed(QPoint);
    void mouseReleased(QPoint);
    void mouseMoved(QPoint);
};

QT_END_NAMESPACE

#endif //CANVAS_H