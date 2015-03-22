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
    int sigma = ui->spinBox->value();
    detector = new DetectorCanny(fileName.toStdString());
    Mat Canny =  detector->start(sigma);
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

void MainWindow::on_pushButton_2_clicked()
{
    int sigma = ui->spinBox->value();
    detector = new DetectorCanny(fileName.toStdString());
    Mat Canny =  detector->start(sigma);
    Mat tmp = img.clone();
    lines = new Hough();
    lines->img = Canny;
    lines->img1 = tmp;
    Mat triangles = lines->FindTriangle();
    cv::cvtColor(triangles, triangles, CV_BGR2RGB);
    QImage image((uchar*)triangles.data, triangles.cols, triangles.rows, triangles.step1(),QImage::Format_RGB888);
    QPixmap pix = QPixmap::fromImage(image);
    QGraphicsScene *scene = new QGraphicsScene;
    scene->clear();
    scene->addPixmap(pix);
    ui->graphicsView->setScene(scene);

}

void MainWindow::on_actionOpenFile_triggered()
{
    fileName = (QFileDialog::getOpenFileName(this,
        tr("Open Image"), "/home", tr("Image Files (*.png)")));//.toLocal8Bit().constData();
    img = cv::imread(fileName.toStdString());
    //detector = new DetectorCanny(fileName.toStdString());
    //Mat Canny =  detector->start(1);
    //imshow("Canny", Canny);
    qDebug()<<img.rows;
    qDebug()<<img.cols;
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
    }
    /*QBrush br(Qt::TexturePattern);
        br.setTextureImage(image);
        QPalette plt = ui->widget->palette();
        plt.setBrush(QPalette::Background, br);
        ui->widget->setPalette(plt);
        ui->widget->show();/*
    /*Mat img, gray;
    img=imread(fileName.toStdString());
      cvtColor(img, gray, CV_BGR2GRAY);
    // smooth it, otherwise a lot of false circles may be detected
    GaussianBlur( gray, gray, Size(9, 9), 2, 2 );*/
    /*img = cv::imread(fileName.toStdString());
    imshow("img", img);
    lines = new Hough();
    lines->img = Canny;
    lines->img1 = img;
    lines->start();*/

    cvWaitKey(0);
    cvDestroyAllWindows();

}

void MainWindow::on_actionSaveFile_triggered()
{

}
