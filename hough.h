#ifndef HOUGH_H
#define HOUGH_H

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/opencv.hpp"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <QDebug>

using namespace cv;
using namespace std;

#define thetaMax 360
/**
@class Hough
@brief Данный класс реализует поиск отрезков по Хафу
*/
class Hough
{
    void FindRectangle();
public:
    Mat FindCircle();
    Mat FindTriangle();
    cv::Mat img; /**< изображение*/
    cv::Mat img1; /**< изображение*/
    void start();
    Hough();
};

#endif // HOUGH_H
