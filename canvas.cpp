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

// **************** Layer Management ***************** //

QPixmap
Canvas:: topLayer()
{
    if (layers.isEmpty()) return QPixmap();
    return layers.last();
}

void
Canvas:: compositeLayers()
{
}

void
Canvas:: onCanvasUpdate(QPixmap pm)
{
    if (scaleFactor > 1.0)
        pm = pm.scaledToHeight(scaleFactor*pm.height());
    else if (scaleFactor < 1.0)
        pm = pm.scaledToHeight(scaleFactor*pm.height(), Qt::SmoothTransformation);
    setPixmap(pm);
    //qDebug("canvas update");
}

void
Canvas:: onImageChange(QPixmap pm)
{
    if (!layers.isEmpty())
        layers.removeLast();
    layers << pm;
    onCanvasUpdate(pm);
}

void
Canvas:: addLayer(QPixmap pm)
{
    layers.append(pm);
    emit imageChanged(pm);
}


void
Canvas:: newBlankLayer()
{
}

void
Canvas:: duplicateLayer()
{
    QPixmap pm = topLayer().copy();
    addLayer(pm);
}


void
Canvas:: deleteLayer()
{
    if (layers.count()>1)
        layers.removeLast();
    emit imageChanged(topLayer());
}

void
Canvas:: mergeLayers()
{
}

void
Canvas:: setScale(float scale)
{
    scaleFactor = scale;
    onCanvasUpdate(topLayer());
}

// ************************* Image Transform ***************************
void
Canvas:: rotate(float degree, Qt::Axis axis)
{
    QTransform transform;
    transform.rotate(degree, axis);
    emit imageChanged( topLayer().transformed(transform) );
}

void
Canvas:: rotateLeft()
{
    rotate(270);
}

void
Canvas:: rotateRight()
{
    rotate(90);
}

void
Canvas:: flip()
{
    rotate(180, Qt::XAxis);
}

void
Canvas:: flop()
{
    rotate(180, Qt::YAxis);
}

