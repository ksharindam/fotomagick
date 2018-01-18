#include "layer_manager.h"

LayerManager:: LayerManager(QObject *parent) : QObject(parent)
{
}


QPixmap
LayerManager:: topLayer()
{
    if (layers.isEmpty()) return QPixmap();
    return layers.last();
}

void
LayerManager:: compositeLayers()
{
}

void
LayerManager:: onCanvasUpdate(QPixmap pm)
{
    emit showImage(pm);
}

void
LayerManager:: onImageChange(QPixmap pm)
{
    emit showImage(pm);
    if (!layers.isEmpty())
        layers.removeLast();
    layers << pm;
}


