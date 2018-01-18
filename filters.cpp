#include "filters.h"


Magick::Image QImage2Magick(const QImage &image)
{
    Magick::Image mgk(image.width(), image.height(), "ARGB", Magick::CharPixel, image.bits());
    return mgk;
}

QImage Magick2QImage(Magick::Image &image)
{
    uchar data[image.columns() * image.rows() * 4];
    image.write(0, 0, image.columns(), image.rows(), "ARGB", Magick::CharPixel, &data);
    QImage img(data, image.columns(), image.rows(), image.columns()*4, QImage::Format_ARGB32);
    return img;
}

Filters:: Filters(QObject *parent) : QObject(parent)
{
}

void
Filters:: setPixmap(QPixmap pm)
{
    pixmap = pm;
}

void
Filters:: applyAutoEqualize()
{
    Magick::Image magickImage = QImage2Magick(pixmap.toImage());
    magickImage.equalize();
    QImage modifiedImage = Magick2QImage(magickImage);

    emit imageChanged(QPixmap::fromImage(modifiedImage));
}

