#ifndef MAIN_H
#define MAIN_H
#include "ui_mainwindow.h"
#include "canvas.h"
#include "tools.h"
#include <QButtonGroup>


QT_BEGIN_NAMESPACE


class Window : public QMainWindow, Ui_MainWindow
{
    Q_OBJECT
public:
    Window();
    void openFile(QString filename);
private slots:
    void openFile();
    void saveFile();
    void onToolClick(int);
private:
    QPixmap pixmap;
    Canvas *canvas;

    int prev_btn=0;
    QButtonGroup *toolBtnGr;
    QList<Tool*> toolList;
    PencilTool *pencilTool;
    BrushTool *brushTool;
    void connectSignals();
};


QT_END_NAMESPACE
#endif // MAIN_H