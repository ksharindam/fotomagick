#include "tools.h"

Tool:: Tool(QObject *parent) : QObject(parent)
{
}

// ****************** Pencil Tool ********************* //
PencilTool:: PencilTool(QObject *parent) : Tool(parent)
{
    mouse_pressed = false;
}

void
PencilTool:: init(QPixmap& pm, QColor fg, QColor )
{
    pixmap = pm;
    fg_color = fg;
    //bg_color = bg;
}

void
PencilTool:: finish()
{
}

void
PencilTool:: setColors(QColor fg, QColor)
{
    fg_color = fg;
    //bg;
}

void
PencilTool:: onMousePress(QPoint pos)
{
    mouse_pressed = true;
    start = pos;
}

void
PencilTool:: onMouseRelease(QPoint pos)
{
    Q_UNUSED(pos);
    mouse_pressed = false;
    emit imageChanged(pixmap);
}

void
PencilTool:: onMouseMove(QPoint pos)
{
    if (!mouse_pressed) return;
    painter.begin(&pixmap);
    painter.drawLine(start, pos);
    painter.end();
    emit canvasUpdated(pixmap);
    start = pos;
}

// ****************** Brush Tool ********************* //

BrushTool:: BrushTool(QObject *parent) : Tool(parent)
{
    mouse_pressed = false;
}

void
BrushTool:: init(QPixmap& pm, QColor fg, QColor )
{
    pixmap = pm;
    fg_color = fg;
    //bg_color = bg;
}

void
BrushTool:: finish()
{
}

void
BrushTool:: setColors(QColor fg, QColor)
{
    fg_color = fg;
    //Q_UNUSED(bg);
}

void
BrushTool:: onMousePress(QPoint pos)
{
    mouse_pressed = true;
    start = pos;
}

void
BrushTool:: onMouseRelease(QPoint pos)
{
    Q_UNUSED(pos);
    mouse_pressed = false;
    emit imageChanged(pixmap);
}

void
BrushTool:: onMouseMove(QPoint pos)
{
    if (!mouse_pressed) return;
    QPen pen(fg_color);
    pen.setWidth(5);
    painter.begin(&pixmap);
    painter.setPen(pen);
    painter.drawLine(start, pos);
    painter.end();
    emit canvasUpdated(pixmap);
    start = pos;
}


