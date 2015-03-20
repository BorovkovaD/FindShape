#include "hough.h"

Hough::Hough()
{
}

Mat Hough::start()
{
    imshow("Canny",img);
    // Чтобы не перелопачивать все изображение запомним точки границ
    height = img.rows;
    width = img.cols;
    for(i=0;i<height;i++)
    {
        for(j=0;j<width;j++)
        {
            if(img.at<unsigned char>(i,j)>0)// использовать Getpixel и TRGB
            {
                pts.push_back(Point(j,i));//почему точки наоборот???
            }
        }
    }
    // Преобразование Хафа переведем точки в параметрическое пространство прямых
    HoughAccumulator=HoughTransform(img,pts);

    cv::normalize(HoughAccumulator,HoughAccumulator,0,1,CV_MINMAX);
    namedWindow("HoughAccumulator");
    imshow("HoughAccumulator",HoughAccumulator);
    Mat mean1,stdDev1;
    //Mat mean(1,1,CV_32FC1,Scalar(0));
    //Mat stdDev(1,1,CV_32FC1,Scalar(0));
    double mean = 0;
    double stdDev = 0;
    meanstdDev(HoughAccumulator,mean,stdDev);
    cv::meanStdDev(HoughAccumulator,mean1,stdDev1);
    //qDebug()<<mean<<stdDev;
    double Mean=mean1.at<double>(0,0);
    qDebug()<<Mean;

    HoughAccumulator-=Mean; // Выровняем относительно нуля
    // сигма применяется к целочисленному изображению, поэтоу домножим
    double StdDev=stdDev1.at<double>(0,0);//*255;
    qDebug()<<StdDev;
    // Порог по аккумулятору (разбиваем картинку на участки, в которых будем искать локальные максимумы)
    Mat thrAcc;
    HoughAccumulator.convertTo(thrAcc,CV_8UC1,255);
    // зададим порог отсечения (меньше порог - больше прямых)
    // Погрог 3 сигмы (отсекаем почти все)
    Mat thrAcc1 (thrAcc.rows, thrAcc.cols, CV_8UC1, Scalar(0));
    //qDebug()<<3*StdDev;
    Threshold(thrAcc, thrAcc1, 3*StdDev);
    //cv::threshold(thrAcc,thrAcc,3*StdDev,255,3);
    namedWindow("thrAcc");
    imshow("thrAcc",thrAcc1);
    // Найдем параметры прямых, за которые саккумулировалось наибольшее кол-во голосов
    /*vector<Point> LinesParams;

    FindLines(thrAcc,LinesParams);

    qDebug() << LinesParams.size()  << endl;
    // Нарисуем точки в пространстве параметров (каждая точка - это прямая)
    for(int i=0;i<LinesParams.size();i++)
    {
        circle(thrAcc,Point(LinesParams[i]),3,CV_RGB(255,255,255),-1);
    }
    //namedWindow("thrAcc");
    //imshow("thrAcc",thrAcc);
    // Нарисуем прямые по заданным параметрам
    // Здесь точки границ, подставляются в уравнения прямых (чтобы получались отрезки, а не бесконечные прямые)
    drawLines(img,LinesParams,pts,20);

    namedWindow("Image");
    imshow("Image",img);*/
    cvWaitKey(0);
    cvDestroyAllWindows();
}

//-----------------------------------------------------------------------------------------------------
// Переводим точки границ в пространство параметров прямой линии (в полярных координатах)
//-----------------------------------------------------------------------------------------------------
Mat Hough::HoughTransform(Mat &input,vector<Point> pts)
{
    // максимальное расстояние от начала координат до точки изобажения
    int Rmax=sqrt(powl(input.rows,2)+powl(input.cols,2));
    Mat acc (Rmax, thetaMax, CV_32FC1, Scalar(0));
    for(i=0;i<pts.size();i++)
    {
        // x cosθ + y sinθ = R
        for(theta=0;theta<thetaMax;theta++)
        {
            // переводим в float угол и в радианы, почему double thetaRadians???
            double thetaRadians=static_cast<float>(theta)*CV_PI/180.0;
            //вычисляем R зная координаты и угол
            double Rd=pts[i].x*cos(thetaRadians)+pts[i].y*sin(thetaRadians);
            //если ???
            if(Rd>0 && Rd<Rmax)
            {
                // увеличиваем значение, то есть считаем кол-во точек, лежащих на прямой с заданными параметрами???
                acc.at<float>((int)Rd,theta)+=1; // использовать SetRGB
            }
        }
    }
    // размываем чтобы ???
    cv::GaussianBlur(acc,acc, Size(3,3),1); // использовать Гауса моего
    return acc;
}

void Hough::meanstdDev(Mat &input, double mean, double stdDev)
{
    double p =0, k =0;
    int N=0;
    uint8_t* pixelPtrIN = (uint8_t*)input.data;
    //double* pixelPtrMean = (double*)mean.data;
    //double* pixelPtrStdDev = (double*)stdDev.data;
    for (i = 0; i<input.rows; i++)
        for(j = 0; j<input.cols; j++)
        {
            //p += pixelPtrIN[i*input.cols +j];
            if(pixelPtrIN[i*input.cols +j] > 0)
            {
                p += pixelPtrIN[i*input.cols +j];
                N++;
            }
        }
    p /= N;
    qDebug()<<p;
    mean = p;
    for (i = 0; i<input.rows; i++)
        for(j = 0; j<input.cols; j++)
        {
            if(pixelPtrIN[i*input.cols +j] > 0)
            {
                k += (pixelPtrIN[i*input.cols +j]-p)*(pixelPtrIN[i*input.cols +j]-p);
            }
        }
    k = sqrt(k/N);
    qDebug()<<k;
    stdDev= k;
    //pixelPtrMean[0+0] = p;
    //pixelPtrStdDev[0+0] = k;
}

void Hough::Threshold(Mat &input, Mat &output, double threshold_val)
{
    qDebug()<<threshold_val;
    uint8_t* pixelPtrIN = (uint8_t*)input.data;
    uint8_t* pixelPtrOUT = (uint8_t*)output.data;
    for (i = 0; i < input.rows; i++)
        for(j = 0; j< input.cols; j++)
        {
            if(pixelPtrIN[i*input.cols +j]>(threshold_val))
            {
                pixelPtrOUT[i*output.cols +j] = pixelPtrIN[i*input.cols +j];
            }
            else
            {
                pixelPtrOUT[i*output.cols +j] = 0;
            }
        }
}

void Hough::normal (Mat& input)
{

}

//----------------------------------------------------------------------
// поиск локальных максимумов
//----------------------------------------------------------------------
void Hough::FindLines(Mat& img,vector<Point>& pts)
{
    vector<Rect> Rects;
    Mat img8U( img.size(), CV_8UC3 );
    cv::normalize(img,img,0,255,CV_MINMAX);
    img.convertTo(img8U,CV_8UC1);

    Mat rect_12 = getStructuringElement(CV_SHAPE_RECT, Size(3,3));
    erode(img8U, img8U, rect_12,Point(),1);
    Mat rect_6 = getStructuringElement(CV_SHAPE_RECT, Size(3,3));
    dilate(img8U,img8U,rect_6,Point(),2);
    vector<vector<Point> > contours;

    vector<Vec4i> hierarchy;

    findContours(img8U,contours, hierarchy, CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE, Point());

    if(contours.size()>0)
    {
        for( int i = 0; i < contours.size(); i++ )
        {
            //if(contourArea(contours[i])>5)
            //{
                Rect r=cv::boundingRect(contours[i]);
                Rects.push_back(r);
            //}
        }
    }
    if(Rects.size()>1)
    {
        for(i=0;i<Rects.size();i++)
        {
            Mat subImg=img(Rects[i]);
            double M,m;
            Point p,P;
            minMaxLoc(subImg,&m,&M,&p,&P);
            P.x+=Rects[i].x;
            P.y+=Rects[i].y;
            pts.push_back(P);
        }
    }
    Rects.clear();
}

void Hough::drawLines(Mat& img,vector<Point>& par,vector<Point>& pts,int gap)
{
    for(i=0;i<par.size();i++)
    {

    double R=par[i].y;
    double th=par[i].x/180.0*CV_PI;

        if(fabs(sin(th))>fabs(cos(th))) // Чтобы не делить на ноль и малые числа (для вертикальных и горизонтальных линий)
        {
            for(int m=0;m<pts.size();m++)
            {
                int x=pts[m].x;
                int y=(R-(double)x*cosl(th))/sinl(th);
                if(fabsl(y-pts[m].y)<gap && x>0 && x<img.cols && y>0 && y<img.rows) // Если точка не слишком далеко от прямой, нарисуем её
                {
                img.at<unsigned char>(y,x)=128; // Отмечаем точку
                }
            }
        }
        else
        {
            for(int m=0;m<pts.size();m++)
            {
                int y=pts[m].y;
                int x=(R-(double)y*sin(th))/cos(th);
                if(fabsl(x-pts[m].x)<gap && x>0 && x<img.cols && y>0 && y<img.rows) // Если точка не слишком далеко от прямой, нарисуем её
                {
                img.at<unsigned char>(y,x)=128; // Отмечаем точку
                }
            }
        }

    }
}
