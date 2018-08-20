#ifndef GRABCUT_TOOL_H
#define GRABCUT_TOOL_H

#include "tools.h"

QT_BEGIN_NAMESPACE


class GrabcutTool : public Tool
{
    Q_OBJECT
public:
    GrabcutTool(QObject *parent) : Tool(parent) {};
    void init(QPixmap pm, float scale, QColor fg, QColor bg);
    void finish() {};
    void onMousePress(QPoint);
    void onMouseRelease(QPoint);
    void onMouseMove(QPoint);
private:
    QPoint old_pos;
    bool mouse_pressed=false;

};

QT_END_NAMESPACE

#endif
