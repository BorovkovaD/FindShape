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
    vector<Point> pts;
    int i , j, theta, height, width; /**< переменные циклов и вспомогательные переменные */
    Mat HoughAccumulator;
    Mat HoughTransform(Mat &input,vector<Point> pts);
    void FindLines(Mat& img,vector<Point>& pts);
    void drawLines(Mat& img,vector<Point>& par,vector<Point>& pts,int gap);
    void Threshold (Mat& input, Mat& output, double threshold_val);
    void normal (Mat& input);
    void meanstdDev (Mat& input, double mean, double stdDev);
public:
    cv::Mat img; /**< изображение*/
    Mat start();
    Hough();
};

#endif // HOUGH_H
