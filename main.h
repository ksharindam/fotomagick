#ifndef MAIN_H
#define MAIN_H
#include "ui_mainwindow.h"
#include "canvas.h"
#include "tools.h"
#include "filters.h"
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
    void onImageChange(QPixmap);
    void onImageChange(QImage image) { onImageChange(QPixmap::fromImage(image)); };
    void zoomIn();
    void zoomOut();
private:
    Canvas *canvas;
    Filters *filters;
    QList<Tool*> toolList;

    int prev_btn=0;
    QButtonGroup *toolBtnGr;

    void connectSignals();
    void setScale(float scale);
};


QT_END_NAMESPACE
#endif // MAIN_H