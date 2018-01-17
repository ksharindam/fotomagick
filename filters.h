#ifndef FILTERS_H
#define FILTERS_H
#include <QPixmap>
#include <Magick++.h>

QT_BEGIN_NAMESPACE

class Filters : public QObject
{
    Q_OBJECT
public:
    Filters(QObject *parent);
    void setPixmap(QPixmap pm);
private:
    QPixmap pixmap;
public slots:
    void applyAutoEqualize();
signals:
    void imageChanged(QPixmap);
};

Magick::Image  QImage2Magick(const QImage &image);
QImage         Magick2QImage(Magick::Image &image);

QT_END_NAMESPACE

#endif
