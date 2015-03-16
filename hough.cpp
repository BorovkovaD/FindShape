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
void hough_line(Mat& inputImg, Mat& inputImg1)
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
                                //++m.second;
                                flag = 1;
                                //if (m.first.point2.y<=y)
                                //{
                                lineStruct tempLine;
                                tempLine.radius = radius;
                                tempLine.angle = angle;
                                cv::Point point(x,y);
                                tempLine.point2 = m.first.point2;
                                tempLine.point1 = m.first.point1;
                                auto result = mymap.find(tempLine);
                                mymap.erase(result);
                                tempLine.point2 = point;
                                mymap.insert ( pair<lineStruct,int>(tempLine, 1) );
                                    //m.first.point2 = point;
                                    //m.first.point2.x = x;
                                    //m.first.point2.y = y;
                                //}
                                break;
                            }
                        }
                            //5) в теле цикла 1 добавляй новые записи в map, если линии с заданным R и углом не существует
                        if (flag == 0)
                        {
                                lineStruct tempLine;
                                tempLine.radius = radius;
                                tempLine.angle = angle;
                                cv::Point point(x,y);
                                tempLine.point1 = point;
                                mymap.insert ( pair<lineStruct,int>(tempLine, 1) );
                        }

                        //6) Конец цикла (1)
                        }
                    }
                }
            }

    int minPoints = 15;
    IplImage tmp=inputImg1;
    //7) Цикл (2) по всем элементам map
    for (auto& x: mymap) {

        //8) в теле цикла (2) проверяй, что значение counter больше определенного minPoints (minPoints - целое число, параметр функции) и сохраняй/рисуй линию
        if (x.second > minPoints)
        {
                //int y = (x.first.radius/sin(x.first.angle));
                //CvPoint pointa(0,y);
                //y = (x.first.radius - inputImg.rows*cos(x.first.angle))/sin(x.first.angle);
                //CvPoint pointb(inputImg.rows,y);

                cvLine(&tmp,x.first.point1,x.first.point2,127,2,cv::LINE_8,0);
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
