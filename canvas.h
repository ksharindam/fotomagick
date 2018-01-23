#ifndef CANVAS_H
#define CANVAS_H

#include <QLabel>
#include <QMouseEvent>

QT_BEGIN_NAMESPACE

// TODO : Add undo manager
class Canvas : public QLabel
{
    Q_OBJECT
public:
    Canvas(QWidget *parent);
    float scaleFactor = 1.0;

    QPixmap topLayer();
    void setScale(float scale);
    void setImage(QImage image);
    void mousePressEvent(QMouseEvent *ev);
    void mouseReleaseEvent(QMouseEvent *ev);
    void mouseMoveEvent(QMouseEvent *ev);
private:
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