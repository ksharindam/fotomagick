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


void
Filters:: applyAutoEqualize()
{
    Magick::Image magickImage = QImage2Magick(pixmap.toImage());
    magickImage.equalize();
    QImage modifiedImage = Magick2QImage(magickImage);

    emit imageChanged(QPixmap::fromImage(modifiedImage));
}

void
Filters:: applyAutoContrast()
{
    Magick::Image magickImage = QImage2Magick(pixmap.toImage());
    magickImage.normalize();
    QImage modifiedImage = Magick2QImage(magickImage);

    emit imageChanged(QPixmap::fromImage(modifiedImage));
}

void
Filters:: applyGrayScale()
{
    QImage img = pixmap.toImage();
    for (int y=0;y<img.height();y++) {
        QRgb* line = ((QRgb*)img.scanLine(y));
        for (int x=0;x<img.width();x++) {
            int val = qGray(line[x]);
            line[x] = qRgba(val,val,val, qAlpha(line[x]));
        }
    }
    emit imageChanged(QPixmap::fromImage(img));
}

