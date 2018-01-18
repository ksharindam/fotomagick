#include "tools.h"
#include <QLabel>
#include <QVBoxLayout>
//#include <QBrush>
//#include <QLinearGradient>
//#include <cmath>

Tool:: Tool(QObject *parent) : QObject(parent)
{
}

void
Tool:: setPixmap(QPixmap& pm)
{
    pixmap = pm;
}

// ****************** Pencil Tool ********************* //
PencilTool:: PencilTool(QObject *parent) : Tool(parent)
{
    mouse_pressed = false;
}

void
PencilTool:: init(QPixmap pm, QColor fg, QColor )
{
    pixmap = pm;
    fg_color = fg;
}

void
PencilTool:: finish()
{
}

void
PencilTool:: setColors(QColor fg, QColor)
{
    fg_color = fg;
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
    Q_UNUSED(pos);              // Used to supress unused variable warning;
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
    pen.setWidth(5);
    pen.setCapStyle(Qt::RoundCap);
    //pen.setJoinStyle(Qt::RoundJoin);
}

void
BrushTool:: init(QPixmap pm, QColor fg, QColor )
{
    pixmap = pm;
    fg_color = fg;
    pen.setColor(fg);
    brushManager = new BrushManager( qobject_cast<QWidget*>(this->parent()) );
    connect(brushManager, SIGNAL(settingsChanged()), this, SLOT(onSettingsChange()));
}

void
BrushTool:: finish()
{
    brushManager->deleteLater();
}

void
BrushTool:: setColors(QColor fg, QColor)
{
    fg_color = fg;
}

void
BrushTool:: onSettingsChange()
{
    pen.setWidth(brushManager->thicknessSlider->value());
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
    painter.begin(&pixmap);
    painter.setPen(pen);
    painter.drawLine(start, pos);
    painter.end();
    emit canvasUpdated(pixmap);
    start = pos;
}

BrushManager:: BrushManager(QWidget *parent) : QWidget(parent)
{
    parent->layout()->addWidget(this);
    QVBoxLayout *layout = new QVBoxLayout(this);
    QLabel *label = new QLabel("Thickness:", this);
    thicknessSlider = new QSlider(Qt::Horizontal,this);
    thicknessSlider->setRange(2, 64);
    layout->addWidget(label);
    layout->addWidget(thicknessSlider);
    connect(thicknessSlider, SIGNAL(valueChanged(int)), this, SLOT(onValueChange(int)));
}

void
BrushManager:: onValueChange(int)
{
    emit settingsChanged();
}
