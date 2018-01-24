#include "tools.h"
#include <QLabel>
#include <QVBoxLayout>
#include <vector>
//#include <QBrush>
//#include <QLinearGradient>
//#include <cmath>

// ****************** Pencil Tool ********************* //

void
PencilTool:: init(QPixmap pm, float scale, QColor fg, QColor )
{
    pixmap = pm;
    scaleFactor = scale;
    fg_color = fg;
}

void
PencilTool:: onMousePress(QPoint pos)
{
    mouse_pressed = true;
    start = pos;
}

void
PencilTool:: onMouseRelease(QPoint)
{
    mouse_pressed = false;
    emit imageChanged(pixmap);
}

void
PencilTool:: onMouseMove(QPoint pos)
{
    if (!mouse_pressed) return;
    painter.begin(&pixmap);
    painter.drawLine(start/scaleFactor, pos/scaleFactor);
    painter.end();
    emit canvasUpdated(pixmap);
    start = pos;
}

// ****************** Brush Tool ********************* //

BrushTool:: BrushTool(QObject *parent) : Tool(parent)
{
    pen.setWidth(5);
    pen.setCapStyle(Qt::RoundCap);
}

void
BrushTool:: init(QPixmap pm, float scale, QColor fg, QColor )
{
    pixmap = pm;
    scaleFactor = scale;
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
BrushTool:: onMouseRelease(QPoint)
{
    mouse_pressed = false;
    emit imageChanged(pixmap);
}

void
BrushTool:: onMouseMove(QPoint pos)
{
    if (!mouse_pressed) return;
    painter.begin(&pixmap);
    painter.setPen(pen);
    painter.drawLine(start/scaleFactor, pos/scaleFactor);
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


// ************************ Eraser Tool ****************************
EraserTool:: EraserTool(QObject *parent) : Tool(parent)
{
    pen.setColor(Qt::white);
    pen.setWidth(5);
    pen.setCapStyle(Qt::RoundCap);
}

void
EraserTool:: init(QPixmap pm, float scale, QColor, QColor )
{
    pixmap = pm;
    scaleFactor = scale;
    brushManager = new BrushManager( qobject_cast<QWidget*>(this->parent()) );
    connect(brushManager, SIGNAL(settingsChanged()), this, SLOT(onSettingsChange()));
}

void
EraserTool:: finish()
{
    brushManager->deleteLater();
}

void
EraserTool:: onSettingsChange()
{
    pen.setWidth(brushManager->thicknessSlider->value());
}

void
EraserTool:: onMousePress(QPoint pos)
{
    mouse_pressed = true;
    start = pos;
}

void
EraserTool:: onMouseRelease(QPoint)
{
    mouse_pressed = false;
    emit imageChanged(pixmap);
}

void
EraserTool:: onMouseMove(QPoint pos)
{
    if (!mouse_pressed) return;
    painter.begin(&pixmap);
    painter.setPen(pen);
    painter.drawLine(start/scaleFactor, pos/scaleFactor);
    painter.end();
    emit canvasUpdated(pixmap);
    start = pos;
}


// ************************ Flood fill tool *************************
/* Stack Based Scanline Floodfill 
   Source : http://lodev.org/cgtutor/floodfill.html#Scanline_Floodfill_Algorithm_With_Stack
*/
void
floodfill(QImage &img, int x, int y, QRgb oldColor, QRgb newColor)
{
  if (oldColor == newColor) return;
  int w = img.width();
  int h = img.height();
  std::vector<QPoint> q;
  bool spanAbove, spanBelow;

  q.push_back(QPoint(x, y));

  while(!q.empty())
  {
    QPoint pt = q.back();
    q.pop_back();
    x = pt.x();
    y = pt.y();
    while (x >= 0 && img.pixel(x,y) == oldColor) x--;
    x++;
    spanAbove = spanBelow = 0;
    while (x < w && img.pixel(x,y) == oldColor )
    {
      img.setPixel(x,y, newColor);
      if(!spanAbove && y > 0 && img.pixel(x,y-1) == oldColor)
      {
        q.push_back(QPoint(x, y - 1));
        spanAbove = 1;
      }
      else if (spanAbove && y > 0 && img.pixel(x,y-1) != oldColor)
      {
        spanAbove = 0;
      }
      if(!spanBelow && y < h - 1 && img.pixel(x,y+1) == oldColor)
      {
        q.push_back(QPoint(x, y + 1));
        spanBelow = 1;
      }
      else if(spanBelow && y < h - 1 && img.pixel(x,y+1) != oldColor)
      {
        spanBelow = 0;
      }
      x++;
    }
  }

}


void
FloodfillTool:: init(QPixmap pm, float scale, QColor fg, QColor )
{
    pixmap = pm;
    scaleFactor = scale;
    fg_color = fg;
}


void
FloodfillTool:: onMousePress(QPoint pos)
{
    QImage img = pixmap.toImage();
    floodfill(img, pos.x()/scaleFactor, pos.y()/scaleFactor, img.pixel(pos), fg_color.rgb());
    pixmap = QPixmap::fromImage(img);
    emit imageChanged(pixmap);
}
