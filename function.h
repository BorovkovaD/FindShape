#ifndef FUNCTION_H
#define FUNCTION_H

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


class Function
{
public:
    float pix; /**< текущий пиксель */
    /**
      * @brief Метод, устанавливающий значение пикселя
      * @param col – столбец
      * @param row – строка
      * @param setPix – ссылка на устанавливаемый пиксель
      * @param output - ссылка на изображение, в которое устанавливаем пиксель
      */
    void SetPix(int row, int col, float& setPix, Mat& output);
    /**
      * @brief Метод, возвращающий значение пикселя
      * @param col – столбец
      * @param row – строка
      * @param retPix – ссылка на пиксель, в котором устанавливается значение
      * @param output - ссылка на изображение, из которого получаем пиксель
      */
    void GetPix (int row, int col, float& retPix, Mat& input);
    Function();
    ~Function();
};

#endif // FUNCTION_H
