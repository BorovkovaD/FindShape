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

using namespace cv;
using namespace std;

#define maxWin  100 /**< максимальный размер окна*/
#define maxDim  4096 /**< максимальный линейный размер рисунка*/

/**
@class DetectorCanny
@brief Данный класс реализует детектор Canny
*/
class DetectorCanny
{
    /**< структура для хранения пикселя*/
    struct TRGB {
        float r; /**< значение красного канала*/
        float g; /**< значение зелёного канала*/
        float b; /**< значение синего канала*/
    };
    float window[2*maxWin]; /**< окно */
    //float window[2*maxWin]; /**< окно */
    int N; /**< ???*/
    int i , j , k , l, height, width; /**< переменные циклов и вспомогательные переменные */
    float sum; /**< сумма элементов окна , для нормализации */
    float s2; /**< 2 * sigma * sigma */
    TRGB tmp[maxDim]; /**< временный массив для формирования строки/столбца */
    TRGB pix; /**< текущий пиксель */
    TRGB p; /**< ещё один пиксель для итерации */
private:
    /**
      * @brief Метод, устанавливающий значение пикселя
      * @param col – столбец
      * @param row – строка
      * @param setPix – ссылка на устанавливаемый пиксель
      * @param output - ссылка на изображение, в которое устанавливаем пиксель
      */
    void SetRGB(int row, int col, TRGB& setPix, Mat& output);

    /**
      * @brief Метод, возвращающий значение пикселя
      * @param col – столбец
      * @param row – строка
      * @param retPix – ссылка на пиксель, в котором устанавливается значение
      * @param output - ссылка на изображение, из которого получаем пиксель
      */
    void GetRGB (int row, int col, TRGB& retPix, Mat& input);

    /**
      * @brief Метод, переводящий трёхканальное изображение в одноканальное
      * @param input – ссылка на исходное изображение
      * @param output - ссылка на обработанное изображение
      */
    void RGB2GRAY(Mat& input, Mat &output);

    /**
      * @brief Метод, размывающий изображение
      * @param sigma - параметр размытия
      * @param input – ссылка на исходное изображение
      * @param output - ссылка на обработанное изображение
      */
    void GaussianFilterOperator (float sigma, Mat input, Mat& output);

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
    Mat start();
    cv::Mat img; /**< изображение*/
    /// @brief Конструктор по умолчанию
    DetectorCanny(string);
    /// @brief Деструктор по умолчанию
    ~DetectorCanny();
};

#endif // DETECTORCANNY_H
