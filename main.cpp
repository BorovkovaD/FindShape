
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

struct TRGB {
    float r;
    float g;
    float b;
};
///* максимальный размер окна */
const int maxWin = 100;
///* максимальный линейный размер рисунка */
const int maxDim = 4096;
///* окно */
float window[2*maxWin];
int N;
///* переменные циклов и вспомогательные переменные */
int i , j , k , l, height, width;
///* сумма элементов окна , для нормализации */
float sum;
///* 2 * sigma * sigma */
float s2;
///* временный массив для формирования строки/столбца */
TRGB tmp[maxDim];
///* текущий пиксель */
TRGB pix;
///* ещё один пиксель для итерации */
TRGB p;

const double PI = 3.14159265;
const int size = 6;
const double sigma = 10;
const double low_pr = 0.20;
const double high_pr = 0.80;
void DoubleThresholding(Mat& imgIn, Mat& imgOut);

void SetRGB(int col, int row, TRGB& setPix, Mat& output)
{
    uint8_t* ptr = (uint8_t*)output.data;
    switch (output.channels())
    {
    case 1:
        ptr[row * output.cols + col ]    = setPix.r;
        break;
    case 2:
        ptr[row * output.cols + col ]    = setPix.r;
        ptr[row * output.cols + col + 1] = setPix.g;
        break;
    case 3:
        ptr[row * output.cols + col]     = setPix.r;
        ptr[row * output.cols + col + 1] = setPix.g;
        ptr[row * output.cols + col + 2] = setPix.b;
        break;
    default:
        break;

    }
}

void houghLine(Mat& imgIn, Mat& imgOut, float accuracy=0.1)
{
    uint8_t* pixelPtrIN = (uint8_t*)imgIn.data;
    uint8_t* pixelPtrOUT = (uint8_t*)imgOut.data;
    // максимальное расстояние от начала координат - это длина диагонали
    int RMax = cvRound( sqrt( (double)(imgIn.cols*imgIn.cols + imgIn.rows*imgIn.rows) ) );
    qDebug()<<RMax;
    // картинка для хранения фазового пространства Хафа (r, f)
            // 0 < r < RMax
            // 0 < f < 2*PI
    Mat phase (RMax, 180, CV_8UC1, Scalar(0));
    uint8_t* pixelPtrPHASE = (uint8_t*)imgIn.data;
    int x=0, y=0, r=0, f=0;
    float theta=0;
    // пробегаемся по пикселям изображения контуров
            for(y=0; y<imgIn.rows; y++)
            {
                    for(x=0; x<imgIn.cols; x++)
                    {
                            if(pixelPtrIN[y*imgIn.cols+x]>0)
                            { // это пиксель контура?
                                    // рассмотрим все возможные прямые, которые могут
                                    // проходить через эту точку
                                    for(f=0; f<180; f++)
                                    { //перебираем все возможные углы наклона
                                            for(r=0; r<RMax; r++){ // перебираем все возможные расстояния от начала координат
                                                //qDebug()<<"y"<<y<<"x"<<x<<"f"<<f<<"r"<<r;
                                                theta=f*CV_PI/180.0; // переводим градусы в радианы

                                                    // Если решение уравнения достаточно хорошее (точность больше заданой)
                                                    if ( abs(( (y)*sin(theta) + (x)*cos(theta)) - r) < accuracy ){
                                                            pixelPtrPHASE[r*phase.cols + f]++; // увеличиваем счетчик для этой точки фазового пространства.
                                                        }
                                            }
                                    }
                            }
                    }
            }
            imshow( "phase", phase );
            // увеличим фазовую картинку
            /*Mat phaseImg (phase.cols*3, phase.rows*3, CV_8UC1, Scalar(0));
            uint8_t* pixelPtrPHASEIMG = (uint8_t*)imgIn.data;
            //cvResize(phase, phaseImg);
            //imshow("phaseImage", phaseImg);
            // Выбираем точку фазового пространства которая набрала наибольшее число попаданий
            unsigned int MaxPhaseValue = 0;
            float Theta=0;
            int R=0;
            for(f=0; f<180; f++){ //перебираем все возможные углы наклона
                     for(r=0; r<RMax; r++){ // перебираем все возможные расстояния от начала координат
                            if(pixelPtrPHASE[r*phase.cols + f]>MaxPhaseValue){
                                     MaxPhaseValue = pixelPtrPHASE[r*phase.cols + f];
                                     Theta = f;
                                     R = r;
                            }
                     }
            }
#if 1
            // нормировка
            float scaler = 0xFFFFFFFF/(float)MaxPhaseValue;
            for(y=0; y<phaseImg.rows; y++){
                    for(x=0; x<phaseImg.cols; x++){
                            pixelPtrPHASEIMG[y*phaseImg.cols+x]*=scaler;
                    }
            }
            imshow( "phaseImage2", phaseImg);
#endif
            // Рисуем линию по точкам для  R, Teta которые получили в результате преобразования
                    Theta=Theta*CV_PI/180.0;
                    for(y=0; y<imgOut.rows; y++){
                            for(x=0; x<imgOut.cols; x++){
                                    if ( cvRound(((y) * sin(Theta) + (x) * cos(Theta))) == R){
                                        pix.r = 255;
                                        pix.g = 0;
                                        pix.b = 0;
                                        SetRGB(x,y,pix,imgOut);
                                    }
                            }
                    }
                    imshow("line", imgOut);*/

}

void BLOB_ANALYSIS(Mat& imgIn, Mat& imgOut)
{
    uint8_t* pixelPtrIN = (uint8_t*)imgIn.data;
    uint8_t* pixelPtrOUT = (uint8_t*)imgOut.data;
    for(int i = 0; i < imgIn.rows; i++)
        for(int j = 0; j < imgIn.cols; j++)
            if((i>=1)&&(i<imgIn.rows-1)&&(j>=1)&&(j<imgIn.cols-1))
            {
                if(pixelPtrIN[i*imgIn.cols + j]>0)
                {
                    if(pixelPtrIN[(i-1)*imgIn.cols + j-1]>0)
                    {
                        pix.r = pixelPtrIN[i*imgIn.cols + j];
                        SetRGB(j,i,pix,imgOut);
                    }
                    if(pixelPtrIN[(i-1)*imgIn.cols + j]>0)
                    {
                        pix.r = pixelPtrIN[i*imgIn.cols + j];
                        SetRGB(j,i,pix,imgOut);
                    }
                    if(pixelPtrIN[(i-1)*imgIn.cols + j+1]>0)
                    {
                        pix.r = pixelPtrIN[i*imgIn.cols + j];
                        SetRGB(j,i,pix,imgOut);
                    }
                    if(pixelPtrIN[i*imgIn.cols + j+1]>0)
                    {
                        pix.r = pixelPtrIN[i*imgIn.cols + j];
                        SetRGB(j,i,pix,imgOut);
                    }
                    if(pixelPtrIN[(i+1)*imgIn.cols + j+1]>0)
                    {
                        pix.r = pixelPtrIN[i*imgIn.cols + j];
                        SetRGB(j,i,pix,imgOut);
                    }
                    if(pixelPtrIN[(i+1)*imgIn.cols + j]>0)
                    {
                        pix.r = pixelPtrIN[i*imgIn.cols + j];
                        SetRGB(j,i,pix,imgOut);
                    }
                    if(pixelPtrIN[(i+1)*imgIn.cols + j-1]>0)
                    {
                        pix.r = pixelPtrIN[i*imgIn.cols + j];
                        SetRGB(j,i,pix,imgOut);
                    }
                    if(pixelPtrIN[i*imgIn.cols + j-1]>0)
                    {
                        pix.r = pixelPtrIN[i*imgIn.cols + j];
                        SetRGB(j,i,pix,imgOut);
                    }
                }
            }
    else
            {
                if((i == 0) && (j>=1) && (j< imgIn.cols-1))
                {
                    if(pixelPtrIN[i*imgIn.cols + j]>0)
                    {
                        if(pixelPtrIN[i*imgIn.cols + j+1]>0)
                        {
                            pix.r = pixelPtrIN[i*imgIn.cols + j];
                            SetRGB(j,i,pix,imgOut);
                        }
                        if(pixelPtrIN[(i+1)*imgIn.cols + j+1]>0)
                        {
                            pix.r = pixelPtrIN[i*imgIn.cols + j];
                            SetRGB(j,i,pix,imgOut);
                        }
                        if(pixelPtrIN[(i+1)*imgIn.cols + j]>0)
                        {
                            pix.r = pixelPtrIN[i*imgIn.cols + j];
                            SetRGB(j,i,pix,imgOut);
                        }
                        if(pixelPtrIN[(i+1)*imgIn.cols + j-1]>0)
                        {
                            pix.r = pixelPtrIN[i*imgIn.cols + j];
                            SetRGB(j,i,pix,imgOut);
                        }
                        if(pixelPtrIN[i*imgIn.cols + j-1]>0)
                        {
                            pix.r = pixelPtrIN[i*imgIn.cols + j];
                            SetRGB(j,i,pix,imgOut);
                        }
                    }
                }
                else
                {
                    if((i>=1)&&(i<imgIn.rows-1)&&(j==0))
                    {
                        if(pixelPtrIN[i*imgIn.cols + j]>0)
                        {
                            if(pixelPtrIN[(i-1)*imgIn.cols + j]>0)
                            {
                                pix.r = pixelPtrIN[i*imgIn.cols + j];
                                SetRGB(j,i,pix,imgOut);
                            }
                            if(pixelPtrIN[(i-1)*imgIn.cols + j+1]>0)
                            {
                                pix.r = pixelPtrIN[i*imgIn.cols + j];
                                SetRGB(j,i,pix,imgOut);
                            }
                            if(pixelPtrIN[i*imgIn.cols + j+1]>0)
                            {
                                pix.r = pixelPtrIN[i*imgIn.cols + j];
                                SetRGB(j,i,pix,imgOut);
                            }
                            if(pixelPtrIN[(i+1)*imgIn.cols + j+1]>0)
                            {
                                pix.r = pixelPtrIN[i*imgIn.cols + j];
                                SetRGB(j,i,pix,imgOut);
                            }
                            if(pixelPtrIN[(i+1)*imgIn.cols + j]>0)
                            {
                                pix.r = pixelPtrIN[i*imgIn.cols + j];
                                SetRGB(j,i,pix,imgOut);
                            }
                        }
                    }
                    else
                    {
                        if((i ==0) &&(j == 0))
                        {
                            if(pixelPtrIN[i*imgIn.cols + j]>0)
                            {
                                if(pixelPtrIN[i*imgIn.cols + j+1]>0)
                                {
                                    pix.r = pixelPtrIN[i*imgIn.cols + j];
                                    SetRGB(j,i,pix,imgOut);
                                }
                                if(pixelPtrIN[(i+1)*imgIn.cols + j+1]>0)
                                {
                                    pix.r = pixelPtrIN[i*imgIn.cols + j];
                                    SetRGB(j,i,pix,imgOut);
                                }
                                if(pixelPtrIN[(i+1)*imgIn.cols + j]>0)
                                {
                                    pix.r = pixelPtrIN[i*imgIn.cols + j];
                                    SetRGB(j,i,pix,imgOut);
                                }
                            }
                        }
                        else
                        {
                            if((i == imgIn.rows-1)&&(j>=1)&&(j<imgIn.cols-1))
                            {
                                if(pixelPtrIN[i*imgIn.cols + j]>0)
                                {
                                    if(pixelPtrIN[(i-1)*imgIn.cols + j-1]>0)
                                    {
                                        pix.r = pixelPtrIN[i*imgIn.cols + j];
                                        SetRGB(j,i,pix,imgOut);
                                    }
                                    if(pixelPtrIN[(i-1)*imgIn.cols + j]>0)
                                    {
                                        pix.r = pixelPtrIN[i*imgIn.cols + j];
                                        SetRGB(j,i,pix,imgOut);
                                    }
                                    if(pixelPtrIN[(i-1)*imgIn.cols + j+1]>0)
                                    {
                                        pix.r = pixelPtrIN[i*imgIn.cols + j];
                                        SetRGB(j,i,pix,imgOut);
                                    }
                                    if(pixelPtrIN[i*imgIn.cols + j+1]>0)
                                    {
                                        pix.r = pixelPtrIN[i*imgIn.cols + j];
                                        SetRGB(j,i,pix,imgOut);
                                    }
                                    if(pixelPtrIN[i*imgIn.cols + j-1]>0)
                                    {
                                        pix.r = pixelPtrIN[i*imgIn.cols + j];
                                        SetRGB(j,i,pix,imgOut);
                                    }
                                }
                            }
                            else
                            {
                                if((i>=1)&&(i<imgIn.rows-1)&&(j==imgIn.cols-1))
                                {
                                    if(pixelPtrIN[i*imgIn.cols + j]>0)
                                    {
                                        if(pixelPtrIN[(i-1)*imgIn.cols + j-1]>0)
                                        {
                                            pix.r = pixelPtrIN[i*imgIn.cols + j];
                                            SetRGB(j,i,pix,imgOut);
                                        }
                                        if(pixelPtrIN[(i-1)*imgIn.cols + j]>0)
                                        {
                                            pix.r = pixelPtrIN[i*imgIn.cols + j];
                                            SetRGB(j,i,pix,imgOut);
                                        }
                                        if(pixelPtrIN[(i+1)*imgIn.cols + j]>0)
                                        {
                                            pix.r = pixelPtrIN[i*imgIn.cols + j];
                                            SetRGB(j,i,pix,imgOut);
                                        }
                                        if(pixelPtrIN[(i+1)*imgIn.cols + j-1]>0)
                                        {
                                            pix.r = pixelPtrIN[i*imgIn.cols + j];
                                            SetRGB(j,i,pix,imgOut);
                                        }
                                        if(pixelPtrIN[i*imgIn.cols + j-1]>0)
                                        {
                                            pix.r = pixelPtrIN[i*imgIn.cols + j];
                                            SetRGB(j,i,pix,imgOut);
                                        }
                                    }
                                }
                                else
                                {
                                    if((i == imgIn.rows-1)&&(j == 0))
                                    {
                                        if(pixelPtrIN[i*imgIn.cols + j]>0)
                                        {
                                            if(pixelPtrIN[(i-1)*imgIn.cols + j]>0)
                                            {
                                                pix.r = pixelPtrIN[i*imgIn.cols + j];
                                                SetRGB(j,i,pix,imgOut);
                                            }
                                            if(pixelPtrIN[(i-1)*imgIn.cols + j+1]>0)
                                            {
                                                pix.r = pixelPtrIN[i*imgIn.cols + j];
                                                SetRGB(j,i,pix,imgOut);
                                            }
                                            if(pixelPtrIN[i*imgIn.cols + j+1]>0)
                                            {
                                                pix.r = pixelPtrIN[i*imgIn.cols + j];
                                                SetRGB(j,i,pix,imgOut);
                                            }
                                        }
                                    }
                                    else
                                    {
                                        if((i == 0)&&(j == imgIn.cols-1))
                                        {
                                            if(pixelPtrIN[i*imgIn.cols + j]>0)
                                            {
                                                if(pixelPtrIN[(i+1)*imgIn.cols + j]>0)
                                                {
                                                    pix.r = pixelPtrIN[i*imgIn.cols + j];
                                                    SetRGB(j,i,pix,imgOut);
                                                }
                                                if(pixelPtrIN[(i+1)*imgIn.cols + j-1]>0)
                                                {
                                                    pix.r = pixelPtrIN[i*imgIn.cols + j];
                                                    SetRGB(j,i,pix,imgOut);
                                                }
                                                if(pixelPtrIN[i*imgIn.cols + j-1]>0)
                                                {
                                                    pix.r = pixelPtrIN[i*imgIn.cols + j];
                                                    SetRGB(j,i,pix,imgOut);
                                                }
                                            }
                                        }
                                        else
                                        {
                                            if(pixelPtrIN[i*imgIn.cols + j]>0)
                                            {
                                                if(pixelPtrIN[(i-1)*imgIn.cols + j-1]>0)
                                                {
                                                    pix.r = pixelPtrIN[i*imgIn.cols + j];
                                                    SetRGB(j,i,pix,imgOut);
                                                }
                                                if(pixelPtrIN[(i-1)*imgIn.cols + j]>0)
                                                {
                                                    pix.r = pixelPtrIN[i*imgIn.cols + j];
                                                    SetRGB(j,i,pix,imgOut);
                                                }
                                                if(pixelPtrIN[i*imgIn.cols + j-1]>0)
                                                {
                                                    pix.r = pixelPtrIN[i*imgIn.cols + j];
                                                    SetRGB(j,i,pix,imgOut);
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
}


void SobelOperator(Mat& imgIn, Mat& imgOut)
{
    uint8_t* pixelPtrIN = (uint8_t*)imgIn.data;
    uint8_t* pixelPtrOUT = (uint8_t*)imgOut.data;
    uint8_t **Q = new uint8_t*[imgIn.rows];
    for(int i = 0; i < imgIn.rows; i++)
        Q[i] = new uint8_t[imgIn.cols];
    double **Z = new double*[imgIn.rows];
    for(int i = 0; i < imgIn.rows; i++)
        Z[i] = new double[imgIn.cols];
    for(int i = 0; i<imgIn.rows; i++)
        for(int j = 0; j < imgIn.cols; j++)
        {
            if((i< imgIn.rows-2) && (j<imgIn.cols-2))
            {
                double Gx = (pixelPtrIN[(i+2)*imgIn.cols + j]+2*pixelPtrIN[(i+2)*imgIn.cols + (j+1)]+pixelPtrIN[(i+2)*imgIn.cols + (j+2)])-(pixelPtrIN[i*imgIn.cols + j]+2*pixelPtrIN[i*imgIn.cols + (j+1)]+pixelPtrIN[i*imgIn.cols + (j+2)]);
                double Gy = (pixelPtrIN[(i)*imgIn.cols + (j+2) + 0]+2*pixelPtrIN[(i+1)*imgIn.cols + (j+2) + 0]+pixelPtrIN[(i+2)*imgIn.cols + (j+2) + 0])-(pixelPtrIN[(i)*imgIn.cols + (j) + 0]+2*pixelPtrIN[(i+1)*imgIn.cols + (j) + 0]+pixelPtrIN[(i+2)*imgIn.cols + (j) + 0]);
                Q[i][j] = sqrt(abs(Gx*Gx+Gy*Gy));
                Z[i][j] = abs(atan(Gy/Gx));
            }
            else
            {
                if ((i == imgIn.rows-2) && (j<imgIn.cols-2))
                {
                    double Gx = -(pixelPtrIN[i*imgIn.cols + j]+2*pixelPtrIN[i*imgIn.cols + (j+1)]+pixelPtrIN[i*imgIn.cols + (j+2)]);
                    double Gy = (pixelPtrIN[(i)*imgIn.cols + (j+2) + 0]+2*pixelPtrIN[(i+1)*imgIn.cols + (j+2) + 0])-(pixelPtrIN[(i)*imgIn.cols + (j) + 0]+2*pixelPtrIN[(i+1)*imgIn.cols + (j) + 0]);
                    Q[i][j] = sqrt(abs(Gx*Gx+Gy*Gy));
                    Z[i][j] = abs(atan(Gy/Gx));
                }
                else
                {
                    if ((i < imgIn.rows-2) && (j == imgIn.cols-2))
                    {
                        double Gx = (pixelPtrIN[(i+2)*imgIn.cols + j]+2*pixelPtrIN[(i+2)*imgIn.cols + (j+1)])-(pixelPtrIN[i*imgIn.cols + j]+2*pixelPtrIN[i*imgIn.cols + (j+1)]);
                        double Gy = -(pixelPtrIN[(i)*imgIn.cols + (j) + 0]+2*pixelPtrIN[(i+1)*imgIn.cols + (j) + 0]+pixelPtrIN[(i+2)*imgIn.cols + (j) + 0]);
                        Q[i][j] = sqrt(abs(Gx*Gx+Gy*Gy));
                        Z[i][j] = abs(atan(Gy/Gx));
                    }
                    else
                    {
                        if((i == imgIn.rows - 2) && (j == imgIn.cols - 2))
                        {
                            double Gx = -(pixelPtrIN[i*imgIn.cols + j]+2*pixelPtrIN[i*imgIn.cols + (j+1)]);
                            double Gy = -(pixelPtrIN[(i)*imgIn.cols + (j) + 0]+2*pixelPtrIN[(i+1)*imgIn.cols + (j) + 0]);
                            Q[i][j] = sqrt(abs(Gx*Gx+Gy*Gy));
                            Z[i][j] = abs(atan(Gy/Gx));
                        }
                        else
                        {
                            if((i < imgIn.rows - 2) && (j == imgIn.cols - 1))
                            {
                                double Gx = (pixelPtrIN[(i+2)*imgIn.cols + j])-(pixelPtrIN[i*imgIn.cols + j]);
                                double Gy = -(pixelPtrIN[(i)*imgIn.cols + (j) + 0]+2*pixelPtrIN[(i+1)*imgIn.cols + (j) + 0]+pixelPtrIN[(i+2)*imgIn.cols + (j) + 0]);
                                Q[i][j] = sqrt(abs(Gx*Gx+Gy*Gy));
                                Z[i][j] = abs(atan(Gy/Gx));
                            }
                            else
                            {
                                if((i == imgIn.rows - 1) && (j < imgIn.cols - 2))
                                {
                                    double Gx = -(pixelPtrIN[i*imgIn.cols + j]+2*pixelPtrIN[i*imgIn.cols + (j+1)]+pixelPtrIN[i*imgIn.cols + (j+2)]);
                                    double Gy = (pixelPtrIN[(i)*imgIn.cols + (j+2) + 0])-(pixelPtrIN[(i)*imgIn.cols + (j) + 0]);
                                    Q[i][j] = sqrt(abs(Gx*Gx+Gy*Gy));
                                    Z[i][j] = abs(atan(Gy/Gx));
                                }
                                else
                                {
                                    if((i == imgIn.rows - 1) && (j == imgIn.cols -2))
                                    {
                                        double Gx = -(pixelPtrIN[i*imgIn.cols + j]+2*pixelPtrIN[i*imgIn.cols + (j+1)]);
                                        double Gy = -(pixelPtrIN[(i)*imgIn.cols + (j) + 0]);
                                        Q[i][j] = sqrt(abs(Gx*Gx+Gy*Gy));
                                        Z[i][j] = abs(atan(Gy/Gx));
                                    }
                                    else
                                    {
                                        if((i == imgIn.rows - 2) && (j == imgIn.cols - 1))
                                        {
                                            double Gx = -(pixelPtrIN[i*imgIn.cols + j]);
                                            double Gy = -(pixelPtrIN[(i)*imgIn.cols + (j) + 0]+2*pixelPtrIN[(i+1)*imgIn.cols + (j) + 0]);
                                            Q[i][j] = sqrt(abs(Gx*Gx+Gy*Gy));
                                            Z[i][j] = abs(atan(Gy/Gx));
                                        }
                                        else
                                        {
                                            double Gx = -(pixelPtrIN[i*imgIn.cols + j]);
                                            double Gy = -(pixelPtrIN[(i)*imgIn.cols + (j)]);
                                            Q[i][j] = sqrt(abs(Gx*Gx+Gy*Gy));
                                            Z[i][j] = abs(atan(Gy/Gx));
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    for(int i = 0; i < imgIn.rows; i++)
        for(int j = 0; j < imgIn.cols; j++)
        {
            if((abs(Z[i][j])<abs(PI/2-Z[i][j]))&&(abs(Z[i][j])<abs(PI/4-Z[i][j]))&&(abs(Z[i][j])<abs(3*PI/4-Z[i][j])))
                Z[i][j]=0;
            else
            {
                if((abs(PI/4-Z[i][j])<abs(Z[i][j]))&&(abs(PI/4-Z[i][j])<abs(PI/2-Z[i][j]))&&(abs(PI/4-Z[i][j])<abs(3*PI/4-Z[i][j])))
                    Z[i][j]=1;
                else
                {
                    if ((abs(PI/2-Z[i][j])<abs(Z[i][j]))&&(abs(PI/2-Z[i][j])<abs(PI/4-Z[i][j]))&&(abs(PI/2-Z[i][j])<abs(3*PI/4-Z[i][j])))
                        Z[i][j]=2;
                    else
                        Z[i][j]=3;
                }
            }
        }
    for(int i = 0; i < imgIn.rows; i++)
        for(int j = 0; j < imgIn.cols; j++)
        {
            pix.r = 0;
            pix.g = 0;
            pix.b = 0;
            if((i>=1)&&(i< imgIn.rows-1)&&(j>=1)&&(j<imgIn.cols-1))
            {
                if((Z[i][j]==0) && (Q[i][j]>Q[i-1][j]) && (Q[i][j]>Q[i+1][j]))
                {
                    pix.r = Q[i][j];
                    SetRGB(j,i,pix,imgOut);
                }
                else
                {
                    if ((Z[i][j]==1) && (Q[i][j]>Q[i-1][j+1]) && (Q[i][j]>Q[i+1][j-1]))
                    {
                        pix.r = Q[i][j];
                        SetRGB(j,i,pix,imgOut);
                    }
                    else
                    {
                        if ((Z[i][j]==2) && (Q[i][j]>Q[i][j-1]) && (Q[i][j]>Q[i][j+1]))
                        {
                            pix.r = Q[i][j];
                            SetRGB(j,i,pix,imgOut);
                        }
                        else
                        {
                            if ((Z[i][j]==3) && (Q[i][j]>Q[i-1][j-1]) && (Q[i][j]>Q[i+1][j+1]))
                            {
                                pix.r = Q[i][j];
                                SetRGB(j,i,pix,imgOut);
                            }
                            else
                            {
                                pix.r = 0;
                                SetRGB(j,i,pix,imgOut);
                            }
                        }
                    }
                }

            }
            else
            {
                if((i == 0) && (j>=1)&&(j<imgIn.cols-1))
                {
                    if((Z[i][j]==0) && (Q[i][j]>Q[i+1][j]))
                    {
                        pix.r = Q[i][j];
                        SetRGB(j,i,pix,imgOut);
                    }
                    else
                    {
                        if ((Z[i][j]==1) && (Q[i][j]>Q[i+1][j-1]))
                        {
                            pix.r = Q[i][j];
                            SetRGB(j,i,pix,imgOut);
                        }
                        else
                        {
                            if ((Z[i][j]==2) && (Q[i][j]>Q[i][j-1]) && (Q[i][j]>Q[i][j+1]))
                            {
                                pix.r = Q[i][j];
                                SetRGB(j,i,pix,imgOut);
                            }
                            else
                            {
                                if ((Z[i][j]==3) && (Q[i][j]>Q[i+1][j+1]))
                                {
                                    pix.r = Q[i][j];
                                    SetRGB(j,i,pix,imgOut);
                                }
                                else
                                {
                                    pix.r = 0;
                                    SetRGB(j,i,pix,imgOut);
                                }
                            }
                        }
                    }
                }
                else
                {
                    if ((i>=1)&&(i< imgIn.rows-1)&&(j==0))
                    {
                        if((Z[i][j]==0) && (Q[i][j]>Q[i-1][j]) && (Q[i][j]>Q[i+1][j]))
                        {
                            pix.r = Q[i][j];
                            SetRGB(j,i,pix,imgOut);
                        }
                        else
                        {
                            if ((Z[i][j]==1) && (Q[i][j]>Q[i-1][j+1]))
                            {
                                pix.r = Q[i][j];
                                SetRGB(j,i,pix,imgOut);
                            }
                            else
                            {
                                if ((Z[i][j]==2) && (Q[i][j]>Q[i][j+1]))
                                {
                                    pix.r = Q[i][j];
                                    SetRGB(j,i,pix,imgOut);
                                }
                                else
                                {
                                    if ((Z[i][j]==3) && (Q[i][j]>Q[i+1][j+1]))
                                    {
                                        pix.r = Q[i][j];
                                        SetRGB(j,i,pix,imgOut);
                                    }
                                    else
                                    {
                                        pix.r = 0;
                                        SetRGB(j,i,pix,imgOut);
                                    }
                                }
                            }
                        }
                    }
                    else
                    {
                        if((i == 0) && (j == 0))
                        {
                            if((Z[i][j]==0) && (Q[i][j]>Q[i+1][j]))
                            {
                                pix.r = Q[i][j];
                                SetRGB(j,i,pix,imgOut);
                            }
                            else
                            {
                                    if ((Z[i][j]==2) && (Q[i][j]>Q[i][j+1]))
                                    {
                                        pix.r = Q[i][j];
                                        SetRGB(j,i,pix,imgOut);
                                    }
                                    else
                                    {
                                        if ((Z[i][j]==3) && (Q[i][j]>Q[i+1][j+1]))
                                        {
                                            pix.r = Q[i][j];
                                            SetRGB(j,i,pix,imgOut);
                                        }
                                        else
                                        {
                                            pix.r = 0;
                                            SetRGB(j,i,pix,imgOut);
                                        }
                                    }
                                }
                        }
                        else
                        {
                            if((i == imgIn.rows - 1) && (j>=1) &&(j<imgIn.cols - 1))
                            {
                                if((Z[i][j]==0) && (Q[i][j]>Q[i-1][j]))
                                {
                                    pix.r = Q[i][j];
                                    SetRGB(j,i,pix,imgOut);
                                }
                                else
                                {
                                    if ((Z[i][j]==1) && (Q[i][j]>Q[i-1][j+1]))
                                    {
                                        pix.r = Q[i][j];
                                        SetRGB(j,i,pix,imgOut);
                                    }
                                    else
                                    {
                                        if ((Z[i][j]==2) && (Q[i][j]>Q[i][j-1]))
                                        {
                                            pix.r = Q[i][j];
                                            SetRGB(j,i,pix,imgOut);
                                        }
                                        else
                                        {
                                            if ((Z[i][j]==3) && (Q[i][j]>Q[i-1][j-1]))
                                            {
                                                pix.r = Q[i][j];
                                                SetRGB(j,i,pix,imgOut);
                                            }
                                            else
                                            {
                                                pix.r = 0;
                                                SetRGB(j,i,pix,imgOut);
                                            }
                                        }
                                    }
                                }
                            }
                            else
                            {
                                if ((i>=1)&&(i<imgIn.rows - 1)&&(j == imgIn.cols - 1))
                                {
                                    if((Z[i][j]==0) && (Q[i][j]>Q[i-1][j]) && (Q[i][j]>Q[i+1][j]))
                                    {
                                        pix.r = Q[i][j];
                                        SetRGB(j,i,pix,imgOut);
                                    }
                                    else
                                    {
                                        if ((Z[i][j]==1) && (Q[i][j]>Q[i+1][j-1]))
                                        {
                                            pix.r = Q[i][j];
                                            SetRGB(j,i,pix,imgOut);
                                        }
                                        else
                                        {
                                            if ((Z[i][j]==2) && (Q[i][j]>Q[i][j-1]))
                                            {
                                                pix.r = Q[i][j];
                                                SetRGB(j,i,pix,imgOut);
                                            }
                                            else
                                            {
                                                if ((Z[i][j]==3) && (Q[i][j]>Q[i-1][j-1]))
                                                {
                                                    pix.r = Q[i][j];
                                                    SetRGB(j,i,pix,imgOut);
                                                }
                                                else
                                                {
                                                    pix.r = 0;
                                                    SetRGB(j,i,pix,imgOut);
                                                }
                                            }
                                        }
                                    }
                                }
                                else
                                {
                                    if((i == imgIn.rows-1)&&(j == 0))
                                    {
                                        if((Z[i][j]==0) && (Q[i][j]>Q[i-1][j]))
                                        {
                                            pix.r = Q[i][j];
                                            SetRGB(j,i,pix,imgOut);
                                        }
                                        else
                                        {
                                            if ((Z[i][j]==1) && (Q[i][j]>Q[i-1][j+1]))
                                            {
                                                pix.r = Q[i][j];
                                                SetRGB(j,i,pix,imgOut);
                                            }
                                            else
                                            {
                                                if ((Z[i][j]==2) && (Q[i][j]>Q[i][j+1]))
                                                {
                                                    pix.r = Q[i][j];
                                                    SetRGB(j,i,pix,imgOut);
                                                }
                                                else
                                                {
                                                     pix.r = 0;
                                                     SetRGB(j,i,pix,imgOut);
                                                    }
                                                }
                                            }
                                    }
                                    else
                                    {
                                        if((i == 0) && (j == imgIn.cols - 1))
                                        {
                                            if((Z[i][j]==0) && (Q[i][j]>Q[i+1][j]))
                                            {
                                                pix.r = Q[i][j];
                                                SetRGB(j,i,pix,imgOut);
                                            }
                                            else
                                            {
                                                if ((Z[i][j]==1) && (Q[i][j]>Q[i+1][j-1]))
                                                {
                                                    pix.r = Q[i][j];
                                                    SetRGB(j,i,pix,imgOut);
                                                }
                                                else
                                                {
                                                    if ((Z[i][j]==2) && (Q[i][j]>Q[i][j-1]))
                                                    {
                                                        pix.r = Q[i][j];
                                                        SetRGB(j,i,pix,imgOut);
                                                    }
                                                    else
                                                    {
                                                         pix.r = 0;
                                                         SetRGB(j,i,pix,imgOut);
                                                        }
                                                    }
                                                }
                                        }
                                        else
                                        {
                                            if((Z[i][j]==0) && (Q[i][j]>Q[i-1][j]))
                                            {
                                                pix.r = Q[i][j];
                                                SetRGB(j,i,pix,imgOut);
                                            }
                                            else
                                            {

                                                    if ((Z[i][j]==2) && (Q[i][j]>Q[i][j-1]))
                                                    {
                                                        pix.r = Q[i][j];
                                                        SetRGB(j,i,pix,imgOut);
                                                    }
                                                    else
                                                    {
                                                        if ((Z[i][j]==3) && (Q[i][j]>Q[i-1][j-1]))
                                                        {
                                                            pix.r = Q[i][j];
                                                            SetRGB(j,i,pix,imgOut);
                                                        }
                                                        else
                                                        {
                                                            pix.r = 0;
                                                            SetRGB(j,i,pix,imgOut);
                                                        }
                                                    }
                                                }
                                        }
                                    }

                                }
                            }
                        }
                    }
                }
            }
        }
}

void GetRGB (int col, int row, TRGB& retPix, Mat& input)
{
    uint8_t* ptr = (uint8_t*)input.data;
    assert (row < input.size().height);
    assert (col < input.size().width);
    switch (input.channels())
    {
    case 1:
        retPix.r =  ptr[row * input.cols + col];
        retPix.g = 0;
        retPix.b = 0;
        break;
    case 2:
        retPix.r =  ptr[row * input.cols + col ];
        retPix.g =  ptr[row * input.cols + col + 1];
        retPix.b = 0;
        break;
    case 3:
        retPix.r =  ptr[row * input.cols + col ];
        retPix.g =  ptr[row * input.cols + col + 1];
        retPix.b =  ptr[row * input.cols + col + 2];
        break;
    default:
        retPix.r = 0;
        retPix.g = 0;
        retPix.b = 0;
        break;
    }
}

void GaussianFilterOperator (float sigma, Mat input, Mat& output)
{
    assert( input.channels() == output.channels() );
    assert (sigma >= 0);

    height = input.size().height - 1;
    width = input.size().width - 1;
    s2 = 2 * sigma * sigma;
    /* размер окна по " правилу трёх сигм" */
    N = ceil (3 * sigma );
    /* центральный элемент окна пока будет exp ( 0 ) = 1 */
    window[maxWin] = 1;
    /* инициализация окна */
    for ( i = 1;  i<= N; i++)
    {
        window[maxWin+i]= exp(-i*i/s2);
        window[maxWin-i]= window[maxWin-i];
    }
    /* первый проход - горизонтальное размытие */
    for ( j = 0; j <= height; j++)
    {
        for ( i = 0; i <= width; i++)
        {
            /* будем вычислять сумму использованных коэффициентов
            для нормализации; это нужно делать каждый раз
            потому что для размытия крайних пикселей
            используется только часть окна */
            double sum = 0;
            /* это[i , j] пиксель который будем формировать */
            pix.r = 0;
            pix.g = 0;
            pix.b = 0;
            /* проходимся окном вокруг этого пикселя */
            for ( k = maxWin-N; k <= maxWin+N; k++){
                /* l - индекс другого пикселя ,
                который вносит вклад */
                l = i + k-maxWin-N;
                if (( l >= 0 ) && ( l < width )) {
                    GetRGB( l , j , p , input);
                    pix.r += ( p.r * window[k] );
                    pix.g += ( p.g * window[k] );
                    pix.b += ( p.b * window[k] );
                    sum += window[k];
                }
            }
            /* нормализация - сумма использованных коэффициентов
            окна должна быть равна 1 */
            pix.r /= sum;
            pix.g /= sum;
            pix.b /= sum;
            /* пиксель готов - сохраняем во временный массив */
            tmp[i]= pix;
        }
        /* строка готова - сохраняем временный массив
        в само изображение */
        for ( i = 0; i <= width; i++)
            SetRGB ( i , j , tmp[i], output );
    };
    /* второй проход - вертикальное размытие */
    for ( i = 0; i <= width; i++)
    {
        for ( j = 0; j <= height; j++)
        {
            double sum = 0;
            pix.r = 0;
            pix.g = 0;
            pix.b = 0;
            for ( k = maxWin-N; k <= maxWin+N; k++){
                /* отличие в том, что итерация теперь будет
                не по строке , а по столбцу */
                l = j + k-maxWin-N;
                if (( l >= 0 ) && ( l < height )) {
                    GetRGB( i , l , p, output);
                    pix.r += p.r * window[k];
                    pix.g += p.g * window[k];
                    pix.b += p.b * window[k];
                    sum += window[k];
                }
            }
            pix.r /= sum;
            pix.g /= sum;
            pix.b /= sum;
            tmp[j]= pix;
        }
        for ( j = 0; j <= height; j++)
            SetRGB ( i , j , tmp[j], output );
    }
}

/**
 * RGB2GRAY
 * Author: Damir Kanafeev
 * Please, read about doxygen!
**/
void RGB2GRAY(Mat& imgIn, Mat &imgOut)
{
    assert(imgOut.channels() == 1);

    uint8_t* pixelPtrIN = (uint8_t*)imgIn.data;
    uint8_t* pixelPtrOUT = (uint8_t*)imgOut.data;
    int cn = imgIn.channels();
    int coeffR = 77;  //0.299;
    int coeffG = 151; //0.587;
    int coeffB = 28;  //0.114;

    for(int i = 0; i < imgOut.rows; i++)
    {
        for(int j = 0; j < imgOut.cols; j++)
        {
            uint8_t red     = pixelPtrIN[j*cn + i*imgOut.cols*cn + 2];
            uint8_t green   = pixelPtrIN[j*cn + i*imgOut.cols*cn + 1];
            uint8_t blue    = pixelPtrIN[j*cn + i*imgOut.cols*cn + 0];
            pixelPtrOUT[j + i*imgOut.cols] = ((red*coeffR)+(green*coeffG)+(blue*coeffB))>>8;
            //qDebug()<<pixelPtrOUT[j + i*imgOut.cols];
        }
    }
}

int main()//int argc, char* argv[]
{
        // имя картинки задаётся первым параметром
        string filename = "C:/Qt/test21.png\0";

        // получаем картинку
        Mat img_input = cv::imread(filename);
        if( img_input.data == NULL )
        {
            printf( "file cannot be loaded\n");
            return 1;
        }
        //imshow("original", img_input);
        Mat img_gray (img_input.rows, img_input.cols, CV_8UC1, Scalar(255));

        cout << "[i] image: " << filename << endl;
        printf("Size: %dx%d\n", img_input.cols, img_input.rows);

        RGB2GRAY(img_input, img_gray);
        //imshow("gray", img_gray);
        Mat img_gaus (img_gray.rows, img_gray.cols, CV_8UC1, Scalar(0));
        GaussianFilterOperator (2, img_gray, img_gaus);
        //imshow("gaus", img_gaus);
        Mat img_sobel (img_gaus.rows, img_gaus.cols, CV_8UC1, Scalar(0));
        SobelOperator(img_gaus, img_sobel);
        //imshow("sobel", img_sobel);
        Mat img_double (img_sobel.rows, img_sobel.cols, CV_8UC1, Scalar(0));
        DoubleThresholding(img_sobel, img_double);
        //imshow("double", img_double);
        Mat img_canny (img_double.rows, img_double.cols, CV_8UC1, Scalar(0));
        BLOB_ANALYSIS(img_double,img_canny);
        imshow("canny", img_canny);
        //Mat img_hough (img_canny.rows, img_canny.cols, CV_8UC1, Scalar(0));
        //houghLine(img_canny,img_hough);
        cvWaitKey(0);
        cvDestroyAllWindows();
        return 0;
}
