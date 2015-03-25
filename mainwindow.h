#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include "detectorcanny.h"
#include "hough.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    QBrush *brush;
    QPalette *palette;
    int flag = 0;
    int sigmac;
    int sigmanewc;
    int sigmal;
    int sigmanewl;
    QString fileNamenew;
public:
    Mat img;
    DetectorCanny *detector;
    Hough *lines;
    QString fileName;
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
