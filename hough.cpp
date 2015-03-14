#include <QMap>
#include <opencv2/highgui.hpp>

using cv::Mat;
struct lineStruct
{
    int radius;
    int angle;
    lineStruct (int tradius, int tangle) :
        radius(tradius), angle(tangle)
    {
    }
};

int getPix(int x, int y, Mat& inputImg)
{
    /// @todo написать
    uint8_t* ptr = (uint8_t*)inputImg.data;
    assert (x < inputImg.size().height);
    assert (y < inputImg.size().width);
    return ptr[x * inputImg.cols + y ];
}
QMap <lineStruct, int> lineCounter;
void increase_counter(int radius, int angle)
{
    /// @todo написать
    QMap::iterator i = lineCounter.find((lineStruct.radius, lineStruct.angle));
    //int counter = lineCounter((radius,angle)) ;
    &lineCounter[i] ++;

}

void hough_line(Mat& inputImg)
{
    /// 0) размеры ячейки
    int h;
    int w;
    int q = 5;

    /// 1) обнулить счетчики всех ячеек;

    /// 2) для каждой точки интереса:
    for (int y = 0; y < inputImg.size().height; y++){
        for (int x = 0; x < inputImg.size().width; x++){
            if (getPix(x, y, inputImg) > 0){
                /// 3) для каждой прямой (x*cos a + y*sin a = r), проходящей через данную точку:
                for (int radius = 0; radius < 1000; radius++){
                    for (int angle = 0; angle < 360; angle++){
                        if (radius - q < x*cos(angle) + y*sin(angle) < radius + q)
                        {
                            /// 4) увеличить соответствующий счетчик;
                            increase_counter(radius, angle);
                        }
                    }
                }
            }
        }
    }
    /// 5) выбрать ячейку с максимальным значением счетчика;
    for (;;)
    {

    }
    /// 6) параметры прямой, проходящей через максимальное число точек принять равным
    ///    координатам центра выбранной ячейки в фазовом пространстве
}
