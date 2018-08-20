#ifndef CANVAS_H
#define CANVAS_H

#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsPixmapItem>
#include <QGraphicsSceneMouseEvent>

QT_BEGIN_NAMESPACE

// TODO : Add undo manager
class Canvas : public QGraphicsScene
{
    Q_OBJECT
public:
    Canvas(QGraphicsView *parent);
    float scaleFactor = 1.0;

    QPixmap topLayer();
    void setScale(float scale);
    void setPixmap(QPixmap pm);
    void setImage(QImage image);
    void mousePressEvent(QGraphicsSceneMouseEvent *ev);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *ev);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *ev);
private:
    //QGraphicsView *view;
    QGraphicsPixmapItem *pixmapItem;
    QList<QPixmap> layers;
    QList<QPoint> layersPosition;
    void compositeLayers();
    void rotate(float degree, Qt::Axis axis = Qt::ZAxis);
public slots:
    void onCanvasUpdate(QPixmap);
    void onImageChange(QPixmap);
    // Layer Management
    void addLayer(QPixmap pm);
    void newBlankLayer();
    void duplicateLayer();
    void deleteLayer();
    void mergeLayers();
    // Image Transform
    void rotateLeft();
    void rotateRight();
    void flip();
    void flop();
signals:
    void mousePressed(QPoint);
    void mouseReleased(QPoint);
    void mouseMoved(QPoint);
    void imageChanged(QPixmap);
};

QT_END_NAMESPACE

#endif //CANVAS_H