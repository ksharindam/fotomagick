#ifndef CANVAS_H
#define CANVAS_H

#include <QLabel>
#include <QMouseEvent>

QT_BEGIN_NAMESPACE

// TODO : Add pixmap to undo cache
class Canvas : public QLabel
{
    Q_OBJECT
public:
    Canvas(QWidget *parent);
    float scaleFactor = 1.0;
    QList<QPixmap> layers;
    QList<QPoint> layersPosition;

    QPixmap topLayer();
    void compositeLayers();
    void setScale(float scale);
    void mousePressEvent(QMouseEvent *ev);
    void mouseReleaseEvent(QMouseEvent *ev);
    void mouseMoveEvent(QMouseEvent *ev);
    void setImage(QImage image);
public slots:
    void onCanvasUpdate(QPixmap);
    void onImageChange(QPixmap);
    void addLayer(QPixmap pm);
    void newBlankLayer();
    void duplicateLayer();
    void deleteLayer();
    void mergeLayers();
signals:
    void mousePressed(QPoint);
    void mouseReleased(QPoint);
    void mouseMoved(QPoint);
    void imageChanged(QPixmap);
};

QT_END_NAMESPACE

#endif //CANVAS_H