#ifndef DETECTORCANNY_H
#define DETECTORCANNY_H

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/opencv.hpp"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <QDebug>
#include "function.h"

using namespace cv;
using namespace std;


/**
@class DetectorCanny
@brief Данный класс реализует детектор Canny
*/
class DetectorCanny
{
    int i , j , height, width; /**< переменные циклов и вспомогательные переменные */
private:

    /**
      * @brief Метод, переводящий трёхканальное изображение в одноканальное
      * @param input – ссылка на исходное изображение
      * @param output - ссылка на обработанное изображение
      */
    void RGB2GRAY(Mat& input, Mat &output);

    /**
      * @brief Метод, описывающий оператор Собеля
      * @param input – ссылка на исходное изображение
      * @param output - ссылка на обработанное изображение
      */
    void SobelOperator(Mat& input, Mat& output);

    /**
      * @brief Метод, выделяющий границы с помощью двойного порога фильтрации
      * @param low_pr – значение нижнего порога
      * @param high_pr – значение верхнего порога
      * @param input – ссылка на исходное изображение
      * @param output - ссылка на обработанное изображение
      */
    void DoubleThresholding(double low_pr, double high_pr, Mat& input, Mat& output);

    /**
      * @brief Метод, описывающий оператор Собеля
      * @param input – ссылка на исходное изображение
      * @param output - ссылка на обработанное изображение
      */
    void BlobAnalysis(Mat& input,Mat& output);


public:
    Function f;
    Mat start();
    cv::Mat img; /**< изображение*/
    /// @brief Конструктор по умолчанию
    DetectorCanny(string);
    /// @brief Деструктор по умолчанию
    ~DetectorCanny();
};

#endif // DETECTORCANNY_H
