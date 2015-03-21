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
    f.pix.r = 0;
    f.pix.g = 0;
    f.pix.b = 0;
    //сохраняем только те точки,которые не ноль
    for(i=0;i<height;i++)
    {
        for(j=0;j<width;j++)
        {
            f.GetPix(i,j,f.pix,img);
            if(f.pix.r>0)
            {
                pts.push_back(Point(j,i));//почему точки наоборот???
            }
        }
    }
    // Преобразование Хафа переведем точки в параметрическое пространство прямых
    HoughAccumulator=HoughTransform(img,pts);
    //??
    cv::normalize(HoughAccumulator,HoughAccumulator,0,1,CV_MINMAX);
    namedWindow("HoughAccumulator");
    imshow("HoughAccumulator",HoughAccumulator);
    Mat mean,stdDev;
    //??
    cv::meanStdDev(HoughAccumulator,mean,stdDev);
    //f.pix.r = 0;
    //f.pix.g = 0;
    //f.pix.b = 0;
    //f.GetPix(0,0,f.pix,mean);
    double Mean=mean.at<double>(0,0);
    HoughAccumulator-=Mean; // Выровняем относительно нуля
    // сигма применяется к целочисленному изображению, поэтоу домножим
    //f.GetPix(0,0,f.pix,stdDev);
    double StdDev=stdDev.at<double>(0,0)*255;
    // Порог по аккумулятору (разбиваем картинку на участки, в которых будем искать локальные максимумы)
    Mat thrAcc;
    HoughAccumulator.convertTo(thrAcc,CV_8UC1,255);
    // зададим порог отсечения (меньше порог - больше прямых)
    // Погрог 3 сигмы (отсекаем почти все)
    Threshold(thrAcc, thrAcc, 3*StdDev);
    //namedWindow("thrAcc");
    //imshow("thrAcc",thrAcc);
    // Найдем параметры прямых, за которые саккумулировалось наибольшее кол-во голосов
    vector<Point> LinesParams;

    FindLines(thrAcc,LinesParams);

    qDebug() << LinesParams.size()  << endl;
    // Нарисуем точки в пространстве параметров (каждая точка - это прямая)
    for(int i=0;i<LinesParams.size();i++)
    {
        circle(thrAcc,Point(LinesParams[i]),3,CV_RGB(255,255,255),-1);
    }
    // Нарисуем прямые по заданным параметрам
    // Здесь точки границ, подставляются в уравнения прямых (чтобы получались отрезки, а не бесконечные прямые)
    drawLines(img,LinesParams,pts,20);

    namedWindow("Image");
    imshow("Image",img);
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
            //если радиус в пределах допустимых значений???
            if(Rd>0 && Rd<Rmax)
            {
                // увеличиваем значение, то есть считаем кол-во точек, лежащих на прямой с заданными параметрами???
                acc.at<float>((int)Rd,theta)+=1; // использовать SetRGB
            }
        }
    }
    // размываем чтобы ???
    //f.GaussianFilterOperator(1,acc,acc1);
    cv::GaussianBlur(acc,acc, Size(3,3),1); // использовать Гауса моего
    return acc;
}

void Hough::Threshold(Mat &input, Mat &output, double threshold_val)
{
    uint8_t* pixelPtrIN = (uint8_t*)input.data;
    uint8_t* pixelPtrOUT = (uint8_t*)output.data;
    f.pix.r = 0;
    f.pix.g = 0;
    f.pix.b = 0;
    for (i = 0; i < input.rows; i++)
        for(j = 0; j< input.cols; j++)
        {
            f.GetPix(i,j,f.pix,input);
            if(f.pix.r>(threshold_val))
            {
                f.SetPix(i,j,f.pix,output);
                //pixelPtrOUT[i*output.cols +j] = pixelPtrIN[i*input.cols +j];
            }
            else
            {
                f.pix.r = 0;
                f.SetPix(i,j,f.pix,output);
            }
        }
}


//----------------------------------------------------------------------
// поиск локальных максимумов
//----------------------------------------------------------------------
void Hough::FindLines(Mat& img,vector<Point>& pts)
{
    vector<Rect> Rects;
    Mat img8U( img.size(), CV_8UC3 );
    //???
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
                Rect r=cv::boundingRect(contours[i]);
                Rects.push_back(r);
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
