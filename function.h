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

#define maxWin  100 /**< максимальный размер окна*/
#define maxDim  4096 /**< максимальный линейный размер рисунка*/

class Function
{
public:
    /**< структура для хранения пикселя*/
    struct TRGB {
        float r; /**< значение красного канала*/
        float g; /**< значение зелёного канала*/
        float b; /**< значение синего канала*/
    };
    int N; /**< ???*/
    float window[2*maxWin]; /**< окно */
    int i , j , k , l, height, width; /**< переменные циклов и вспомогательные переменные */
    float sum; /**< сумма элементов окна , для нормализации */
    float s2; /**< 2 * sigma * sigma */
    TRGB tmp[maxDim]; /**< временный массив для формирования строки/столбца */
    TRGB pix; /**< текущий пиксель */
    TRGB p; /**< ещё один пиксель для итерации */
    /**
      * @brief Метод, устанавливающий значение пикселя
      * @param col – столбец
      * @param row – строка
      * @param setPix – ссылка на устанавливаемый пиксель
      * @param output - ссылка на изображение, в которое устанавливаем пиксель
      */
    void SetPix(int row, int col, TRGB& setPix, Mat& output);
    /**
      * @brief Метод, возвращающий значение пикселя
      * @param col – столбец
      * @param row – строка
      * @param retPix – ссылка на пиксель, в котором устанавливается значение
      * @param output - ссылка на изображение, из которого получаем пиксель
      */
    void GetPix (int row, int col, TRGB& retPix, Mat& input);
    /**
      * @brief Метод, размывающий изображение
      * @param sigma - параметр размытия
      * @param input – ссылка на исходное изображение
      * @param output - ссылка на обработанное изображение
      */
    void GaussianFilterOperator (float sigma, Mat input, Mat& output);
    Function();
    ~Function();
};

#endif // FUNCTION_H
