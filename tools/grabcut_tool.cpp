#include "grabcut_tool.h"
#include <opencv2/imgproc/imgproc.hpp>
#include <QDebug>

void
GrabcutTool:: init(QPixmap pm, float scale, QColor fg, QColor)
{
    pixmap = pm;
    scaleFactor = scale;
    fg_color = fg;
}


void
GrabcutTool:: onMousePress(QPoint pos)
{
    old_pos = pos;
    mouse_pressed = true;
}

void
GrabcutTool:: onMouseRelease(QPoint pos)
{
    mouse_pressed = false;

    cv::Rect rect(old_pos.x(), old_pos.y(), pos.x()-old_pos.x(), pos.y()-old_pos.y());
    cv::Mat cv_img, mask, mask2, bgdModel, fgdModel;
    cv_img = QImage2Mat(pixmap.toImage());
    //cv::Mat mask2(pixmap.height(), pixmap.width(), CV_8UC3);
    cv::grabCut(cv_img, mask, rect, bgdModel, fgdModel, 1, cv::GC_INIT_WITH_RECT);
    cv::compare(mask, cv::GC_PR_FGD, mask, cv::CMP_EQ);
    //cv::cvtColor(mask, mask2, CV_GRAY2BGR, 3);
    cv_img.copyTo(mask2, mask);
    QImage result = Mat2QImage(mask2);
    qDebug() << mask.cols << mask.rows << mask.step << mask2.type();
    if (result.isNull()) qDebug("no mask");
    result.save("mask.png");
    emit canvasUpdated(pixmap);
}

void
GrabcutTool:: onMouseMove(QPoint pos)
{
    if (!mouse_pressed) return;
    QPixmap pm = pixmap.copy();
    painter.begin(&pm);
    painter.drawRect(old_pos.x(), old_pos.y(), pos.x()-old_pos.x(), pos.y()-old_pos.y());
    painter.end();
    emit canvasUpdated(pm);
}

cv::Mat QImage2Mat(QImage img)
{
    if (img.format()!=QImage::Format_RGB888)
        img = img.convertToFormat(QImage::Format_RGB888);

    cv::Mat tmp(img.height(), img.width(), CV_8UC3, (uchar*)img.bits(), img.bytesPerLine());
    cv::Mat mat;
    cv::cvtColor(tmp, mat , CV_BGR2RGB);
    return mat;
}

QImage Mat2QImage(cv::Mat const& mat)
{
    QImage img((const uchar*)mat.data, mat.cols, mat.rows, mat.step, QImage::Format_RGB888);
    return img.rgbSwapped();
}
