#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QDebug>
#include <QString>
#include <string.h>
#include <QMessageBox>
using namespace std;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    if (flag == 1 && fileNamenew != fileName)
    {
        fileName = fileNamenew;
        sigmac = ui->spinBox->value();
        detector = new DetectorCanny(fileNamenew.toStdString());
        Mat Canny =  detector->start(sigmac);
        Mat tmp = img.clone();
        lines = new Hough();
        lines->img = Canny;
        lines->img1 = tmp;
        Mat circles = lines->FindCircle();
        cv::cvtColor(circles, circles, CV_BGR2RGB);
        QImage image((uchar*)circles.data, circles.cols, circles.rows, circles.step1(),QImage::Format_RGB888);
        QPixmap pix = QPixmap::fromImage(image);
        QGraphicsScene *scene = new QGraphicsScene;
        scene->clear();
        scene->addPixmap(pix);
        ui->graphicsView->setScene(scene);
    }
    if (flag == 1 && fileNamenew == fileName && (ui->spinBox->value()) != sigmac)
    {
        fileName = fileNamenew;
        sigmanewc = ui->spinBox->value();
        sigmac = sigmanewc;
        detector = new DetectorCanny(fileNamenew.toStdString());
        Mat Canny =  detector->start(sigmanewc);
        Mat tmp = img.clone();
        lines = new Hough();
        lines->img = Canny;
        lines->img1 = tmp;
        Mat circles = lines->FindCircle();
        cv::cvtColor(circles, circles, CV_BGR2RGB);
        QImage image((uchar*)circles.data, circles.cols, circles.rows, circles.step1(),QImage::Format_RGB888);
        QPixmap pix = QPixmap::fromImage(image);
        QGraphicsScene *scene = new QGraphicsScene;
        scene->clear();
        scene->addPixmap(pix);
        ui->graphicsView->setScene(scene);
    }
    if(flag == 0)
    {
        QMessageBox msgBox;
        msgBox.setText("Open the image");
        msgBox.exec();
    }
}

void MainWindow::on_pushButton_2_clicked()
{
    if (flag == 1 && fileNamenew != fileName)
    {
        fileName = fileNamenew;
        sigmal = ui->spinBox->value();
        detector = new DetectorCanny(fileNamenew.toStdString());
        Mat Canny =  detector->start(sigmal);
        Mat tmp = img.clone();
        lines = new Hough();
        lines->img = Canny;
        lines->img1 = tmp;
        Mat line = lines->FindLines();
        cv::cvtColor(line, line, CV_BGR2RGB);
        QImage image((uchar*)line.data, line.cols, line.rows, line.step1(),QImage::Format_RGB888);
        QPixmap pix = QPixmap::fromImage(image);
        QGraphicsScene *scene = new QGraphicsScene;
        scene->clear();
        scene->addPixmap(pix);
        ui->graphicsView->setScene(scene);
    }
    if (flag == 1 && fileNamenew == fileName && (ui->spinBox->value()) != sigmal)
    {
        fileName = fileNamenew;
        sigmanewl = ui->spinBox->value();
        sigmal = sigmanewl;
        detector = new DetectorCanny(fileNamenew.toStdString());
        Mat Canny =  detector->start(sigmanewl);
        Mat tmp = img.clone();
        lines = new Hough();
        lines->img = Canny;
        lines->img1 = tmp;
        Mat line = lines->FindLines();
        cv::cvtColor(line, line, CV_BGR2RGB);
        QImage image((uchar*)line.data, line.cols, line.rows, line.step1(),QImage::Format_RGB888);
        QPixmap pix = QPixmap::fromImage(image);
        QGraphicsScene *scene = new QGraphicsScene;
        scene->clear();
        scene->addPixmap(pix);
        ui->graphicsView->setScene(scene);
    }
    if(flag == 0)
    {
        QMessageBox msgBox;
        msgBox.setText("Open the image");
        msgBox.exec();
    }
}

void MainWindow::on_pushButton_3_clicked()
{
    flag = 0;
    fileNamenew = (QFileDialog::getOpenFileName(this,
        tr("Open Image"), "/home", tr("Image Files (*.png)")));
    img = cv::imread(fileNamenew.toStdString());
    if((img.rows<50) || (img.cols<50))
    {
        QMessageBox msgBox;
        msgBox.setText("The image is very small");
        msgBox.exec();
    }
    else if((img.rows>1000) || (img.cols>1000))
    {
        QMessageBox msgBox;
        msgBox.setText("The image is very large");
        msgBox.exec();
    }
    else if(img.channels()!=3)
    {
        QMessageBox msgBox;
        msgBox.setText("The image should be triple-channel");
        msgBox.exec();
    }
    else
    {
        Mat img1;
        cv::cvtColor(img, img1, CV_BGR2RGB);
        QImage image((uchar*)img1.data, img1.cols, img1.rows, img1.step1(), QImage::Format_RGB888);
        QPixmap pix = QPixmap::fromImage(image);
        QGraphicsScene *scene = new QGraphicsScene;
        scene->addPixmap(pix);
        ui->graphicsView->setScene(scene);
        flag = 1;
    }
}
