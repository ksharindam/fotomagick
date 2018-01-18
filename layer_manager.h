#ifndef LAYER_MANAGER_H
#define LAYER_MANAGER_H

#include <QPixmap>

QT_BEGIN_NAMESPACE

class LayerManager : public QObject
{
    Q_OBJECT
public:
    LayerManager(QObject *parent);
    QList<QPixmap> layers;
    QList<QPoint> layersPosition;
    void compositeLayers();
    QPixmap topLayer();
public slots:
    void onCanvasUpdate(QPixmap);
    void onImageChange(QPixmap);
signals:
    void imageChanged(QPixmap);
    void showImage(QPixmap);
};

QT_END_NAMESPACE

#endif