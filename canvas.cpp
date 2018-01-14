#include "canvas.h"

Canvas:: Canvas(QWidget *parent) : QLabel(parent)
{
    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    setMouseTracking(true);
}

void
Canvas:: mousePressEvent(QMouseEvent *ev)
{
    emit mousePressed(ev->pos());
}

void
Canvas:: mouseReleaseEvent(QMouseEvent *ev)
{
    emit mouseReleased(ev->pos());
}

void
Canvas:: mouseMoveEvent(QMouseEvent *ev)
{
    emit mouseMoved(ev->pos());
}

void
Canvas:: setImage(QImage image)
{
    setPixmap(QPixmap::fromImage(image));
}