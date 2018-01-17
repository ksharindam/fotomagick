#ifndef MAIN_H
#define MAIN_H
#include "ui_mainwindow.h"
#include "canvas.h"
#include "tools.h"
#include "filters.h"
#include <QButtonGroup>


QT_BEGIN_NAMESPACE

// TODO : multi layer image handling

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
    void onImageChange(QPixmap);
    void onImageChange(QImage image) { onImageChange(QPixmap::fromImage(image)); };
private:
    QPixmap pixmap;
    Canvas *canvas;
    Filters *filters;

    int prev_btn=0;
    QButtonGroup *toolBtnGr;
    QList<Tool*> toolList;
    PencilTool *pencilTool;
    BrushTool *brushTool;
    void connectSignals();
};


QT_END_NAMESPACE
#endif // MAIN_H