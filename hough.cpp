#include "hough.h"
#include <QDebug>

using cv::Mat;
using namespace std;

//1) верни структуру для линии
struct lineStruct
{
    int radius;
    int angle;
    cv::Point point1;
    cv::Point point2;
    bool operator<(const lineStruct& other) const
    {
        return radius < other.radius;
    }
    bool operator==(const lineStruct& other) const
    {
        return radius == other.radius;
    }
};

//2) создай map пустой вне функции

map<lineStruct,int> mymap;

int getPix(int x, int y, Mat& inputImg)
{
    /// @todo написать
    uint8_t* ptr = (uint8_t*)inputImg.data;
    assert (x < inputImg.size().width);
    assert (y < inputImg.size().height);
    return ptr[y * inputImg.cols + x ];
}

//....
void hough_line(Mat& inputImg)
{
    /// 0) размеры ячейки
    int h;
    int w;
    int q = 5;

    /// 1) обнулить счетчики всех ячеек;
    lineStruct temp;
    temp.radius = 0;
    temp.angle = 0;
    mymap[temp]=0;
    /// 2) для каждой точки интереса:
    for (int y = 0; y < inputImg.size().height; y++){
        for (int x = 0; x < inputImg.size().width; x++){
            //qDebug()<<x<<y;
            if (getPix(x, y, inputImg) > 0){
                /// 3) для каждой прямой (x*cos a + y*sin a = r), проходящей через данную точку:
                //for (int radius = 0; radius < 1000; radius++){
                    for (int angle = 0; angle < 180; angle++){
                        int radius = x*cos(angle) + y*sin(angle);
                        int flag = 0;
                        //3) Цикл (1) по всем элементам map
                        for (auto& m: mymap) {
                        //4) в теле цикла 1 увеличивай счетчик, если линии с заданным радиусом и углом уже существует
                            if (m.first.radius == radius && m.first.angle == angle)
                            {
                                ++m.second;
                                flag = 1;
                                break;
                            }
                        }
                            //5) в теле цикла 1 добавляй новые записи в map, если линии с заданным R и углом не существует
                        if (flag == 0)
                        {
                                lineStruct tempLine;
                                tempLine.radius = radius;
                                tempLine.angle = angle;
                                mymap.insert ( pair<lineStruct,int>(tempLine, 1) );
                        }

                        //6) Конец цикла (1)
                        }
                    }
                }
            }

    int minPoints = 5;
    IplImage tmp=inputImg;
    //7) Цикл (2) по всем элементам map
    for (auto& x: mymap) {

        //8) в теле цикла (2) проверяй, что значение counter больше определенного minPoints (minPoints - целое число, параметр функции) и сохраняй/рисуй линию
        if (x.second > minPoints)
        {
            for(int t = 0; t<inputImg.rows;t++)
            {
                qDebug()<<t;
                int y = (x.first.radius - t*cos(x.first.angle))/sin(x.first.angle);
                CvPoint pointa(t,y);
                t++;
                y = (x.first.radius - t*cos(x.first.angle))/sin(x.first.angle);
                CvPoint pointb(t,y);
                cvLine(&tmp,pointa,pointb,255,4,cv::LINE_8,0);
            }
        }
    }
    cvShowImage("image", &tmp);
}


/*struct lineStruct
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
map <lineStruct, int> lineCounter;
void increase_counter(int radius, int angle)
{
    /// @todo написать
    map::iterator i = lineCounter.find((lineStruct.radius, lineStruct.angle));
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
}*/
