#include "grabcut_tool.h"
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
    cv_img = qImage2Mat(pixmap.toImage());
    cv::grabCut(cv_img, mask, rect, bgdModel, fgdModel, 1, cv::GC_INIT_WITH_RECT);
    cv::compare(mask, cv::GC_PR_FGD, mask, cv::CMP_EQ);
    cv_img.copyTo(mask2, mask);
    QImage result = mat2QImage(mask2);
    //result.save("mask.png");
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

