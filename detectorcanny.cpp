#include "detectorcanny.h"

DetectorCanny::DetectorCanny(string filename)
{
    // получаем картинку
    img = cv::imread(filename);
    if( img.data == NULL )
    {
        printf( "file cannot be loaded\n");
        return ;
    }
}

DetectorCanny::~DetectorCanny()
{
}

Mat DetectorCanny::start(int sigma)
{
    Mat img_gray (img.rows, img.cols, CV_8UC1, Scalar(0));
    RGB2GRAY(img, img_gray);
    Mat img_gaus (img_gray.rows, img_gray.cols, CV_8UC1, Scalar(0));
    double sig = sigma/10;
    GaussianFilterOperator (sig, img_gray, img_gaus);
    Mat img_sobel (img_gaus.rows, img_gaus.cols, CV_8UC1, Scalar(0));
    SobelOperator(img_gaus, img_sobel);
    Mat img_double (img_sobel.rows, img_sobel.cols, CV_8UC1, Scalar(0));
    DoubleThresholding(0.3,0.8,img_sobel, img_double);
    Mat img_canny (img_double.rows, img_double.cols, CV_8UC1, Scalar(0));
    BlobAnalysis(img_double, img_canny);
    return img_canny;
}

void DetectorCanny::GaussianFilterOperator (double& sigma, Mat& input, Mat& output)
{
    assert( input.channels() == output.channels() );
    assert (sigma >= 0);
    height = input.rows;
    width = input.cols;
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
    for ( i = 0; i < height; i++)
    {
        for ( j = 0; j < width; j++)
        {
            /* будем вычислять сумму использованных коэффициентов
            для нормализации; это нужно делать каждый раз
            потому что для размытия крайних пикселей
            используется только часть окна */
            double sum = 0;
            /* это[i , j] пиксель который будем формировать */
            pix = 0;
            /* проходимся окном вокруг этого пикселя */
            for ( k = maxWin-N; k <= maxWin+N; k++){
                /* l - индекс другого пикселя ,
                который вносит вклад */
                l = j + k-maxWin-N;
                if (( l >= 0 ) && ( l < width )) {
                    f.GetPix( i , l , p , input);
                    pix += ( p * window[k] );
                    sum += window[k];
                }
            }
            /* нормализация - сумма использованных коэффициентов
            окна должна быть равна 1 */
            pix /= sum;
            /* пиксель готов - сохраняем во временный массив */
            tmp[j]= pix;
        }
        /* строка готова - сохраняем временный массив
        в само изображение */
        for ( j = 0; j < width; j++)
            f.SetPix ( i , j , tmp[j], output );
    }
    /* второй проход - вертикальное размытие */
    for ( j = 0; j < width; j++)
    {
        for ( i = 0; i < height; i++)
        {
            double sum = 0;
            pix = 0;
            for ( k = maxWin-N; k <= maxWin+N; k++){
                /* отличие в том, что итерация теперь будет
                не по строке , а по столбцу */
                l = i + k-maxWin-N;
                if (( l >= 0 ) && ( l < height )) {
                    f.GetPix( l , j , p, output);
                    pix += p * window[k];
                    sum += window[k];
                }
            }
            pix /= sum;
            tmp[i]= pix;
        }
        for ( i = 0; i < height; i++)
            f.SetPix ( i , j , tmp[i], output );
    }
}

void DetectorCanny::RGB2GRAY(Mat& input, Mat &output)
{
    assert(output.channels() == 1);

    uint8_t* pixelPtrIN = (uint8_t*)input.data;
    uint8_t* pixelPtrOUT = (uint8_t*)output.data;
    int cn = input.channels();
    int coeffR = 77;  //0.299;
    int coeffG = 151; //0.587;
    int coeffB = 28;  //0.114;

    for(i = 0; i < input.rows; i++)
    {
        for(j = 0; j < output.cols; j++)
        {
            uint8_t red     = pixelPtrIN[j*cn + i*output.cols*cn + 2];
            uint8_t green   = pixelPtrIN[j*cn + i*output.cols*cn + 1];
            uint8_t blue    = pixelPtrIN[j*cn + i*output.cols*cn + 0];
            pixelPtrOUT[j + i*output.cols] = ((red*coeffR)+(green*coeffG)+(blue*coeffB))>>8;
        }
    }
}

void DetectorCanny::SobelOperator(Mat& input, Mat& output)
{
    uint8_t* pixelPtrIN = (uint8_t*)input.data;
    height = input.rows;
    width = input.cols;
    /**< массив для хранения значения градиента*/
    uint8_t **Gradient = new uint8_t*[height];
    for(i = 0; i < height; i++)
        Gradient[i] = new uint8_t[width];
    /**< массив для хранения направления градиента*/
    double **Angle = new double*[height];
    for(i = 0; i < height; i++)
        Angle[i] = new double[width];
    /**< значение градиента для точки по оси Х*/
    double Gx;
    /**< значение градиента для точки по оси У*/
    double Gy;
    for(i = 1; i<height-1; i++)
        for(j = 1; j < width-1; j++)
        {
                /* ядра свёрток, вычисляющие первую производную по направлениям (изменения интенсивности в направлениях)*/
                Gx = (pixelPtrIN[(i+1)*width + (j-1)]+2*pixelPtrIN[(i+1)*width + j]+pixelPtrIN[(i+1)*width + (j+1)])-(pixelPtrIN[(i-1)*width + (j-1)]+2*pixelPtrIN[(i-1)*width + j]+pixelPtrIN[(i-1)*width + (j+1)]);
                Gy = (pixelPtrIN[(i-1)*width + (j+1)]+2*pixelPtrIN[i*width + (j+1)]+pixelPtrIN[(i+1)*width + (j+1)])-(pixelPtrIN[(i-1)*width + (j-1)]+2*pixelPtrIN[i*width + (j-1)]+pixelPtrIN[(i+1)*width + (j-1)]);
                /*размер этого изменения*/
                Gradient[i][j] = sqrt(Gx*Gx+Gy*Gy);
                /*направление градиента */
                double angle = atan2(Gy,Gx);
                /* перевод угла в пространство [0, PI] */
                Angle[i][j] = angle < 0 ? angle + CV_PI : angle;
        }
    for(i = 1; i < height-1; i++)
        for(j = 1; j < width-1; j++)
        {
            /* округляем угол к  3*PI/4*/
            if(abs(3*CV_PI/4 - Angle[i][j])<=CV_PI/8)
                Angle[i][j]=3;
            else
            {
                /* округляем угол к  PI/4*/
                if(abs(CV_PI/4 - Angle[i][j])<=CV_PI/8)
                //if(Angle[i][j]<=CV_PI/4+CV_PI/8 && Angle[i][j]>=CV_PI/4-CV_PI/8)
                    Angle[i][j]=1;
                else
                {
                    /* округляем угол к  PI/2*/
                    if(abs(CV_PI/2 - Angle[i][j])<CV_PI/8)
                    //if (Angle[i][j]<=CV_PI/2+CV_PI/8 && Angle[i][j]>=CV_PI/2-CV_PI/8)
                        Angle[i][j]=2;
                    else
                    {
                        /* округляем угол к  0*/
                        Angle[i][j]=0;
                    }
                }
            }
        }
    /* подавление не-максимумов*/
    for(i = 1; i < height-1; i++)
    {
        for(j = 1; j < width-1; j++)
        {
            pix = 0;
            /* если точка лежит на горизонтальной границе и является максимумом*/
            if((Angle[i][j]==0) && (Gradient[i][j]>Gradient[i-1][j]) && (Gradient[i][j]>Gradient[i+1][j]))
            {
                pix = Gradient[i][j];
                f.SetPix(i,j,pix,output);
            }
            /* если точка лежит на границе PI/4 и является максимумом*/
            else if ((Angle[i][j]==1) && (Gradient[i][j]>Gradient[i-1][j-1]) && (Gradient[i][j]>Gradient[i+1][j+1]))
            {
                pix = Gradient[i][j];
                f.SetPix(i,j,pix,output);
            }
            /* если точка лежит на вертикальной границе и является максимумом*/
            else if ((Angle[i][j]==2) && (Gradient[i][j]>Gradient[i][j-1]) && (Gradient[i][j]>Gradient[i][j+1]))
            {
                pix = Gradient[i][j];
                f.SetPix(i,j,pix,output);
            }
            /* если точка лежит на границе 3*PI/4 и является максимумом*/
            else if ((Angle[i][j]==3) && (Gradient[i][j]>Gradient[i-1][j+1]) && (Gradient[i][j]>Gradient[i+1][j-1]))
            {
                pix = Gradient[i][j];
                f.SetPix(i,j,pix,output);
            }
            else
            {
                pix = 0;
                f.SetPix(i,j,pix,output);
            }
        }
    }
}

void DetectorCanny::DoubleThresholding(double low_pr, double high_pr, Mat& input, Mat& output)
{
    uint8_t* pixelPtrIN = (uint8_t*)input.data;
    /**< нижний порог значения пикселя*/
    double down = low_pr*255;
    /**< верхний порог значения пикселя*/
    double up = high_pr*255;
    height = input.rows;
    width = input.cols;
    pix = 0;
    for(i = 0; i < height; i++)
        for(j = 0; j < width; j++)
        {
            /* если значение пикселя выше верхней границы,
             * то он принимает максимальное значение*/
            if(pixelPtrIN[i*width + j] >= up)
            {
                pix = 255;
                f.SetPix(i,j,pix,output);
            }
            /* если значение пикселя ниже нижней границы,
             * то он подавляется*/
            else if (pixelPtrIN[i*width + j] <= down)
                {
                    pix = 0;
                    f.SetPix(i,j,pix,output);
                }
            /* если значение пикселя между границ,
             * то он принимает среднее значение*/
            else
                {
                    pix = 127;
                    f.SetPix(i,j,pix,output);
                }
        }
}

void DetectorCanny::BlobAnalysis(Mat& input, Mat& output)
{
    height = input.rows;
    width = input.cols;
    pix = 0;
    p = 0;
    for(i = 1; i < height-1; i++)
        for(j = 1; j < width-1; j++)
        {
            f.GetPix( i , j , p , input);
            /* если пиксель лежит на границе*/
            if(p>0)
            {
                /* проверяем соседние пиксели,
                 * если пиксель принадлежит группе пикселей, то сохраняем его
                 * иначе подавляем его*/
                f.GetPix( i-1 , j-1 , p , input);
                if(p>0)
                {
                    f.GetPix( i , j , pix , input);
                    f.SetPix(i,j,pix,output);
                    continue;
                }
                f.GetPix( i-1 , j , p , input);
                if(p>0)
                {
                    f.GetPix( i , j , pix , input);
                    f.SetPix(i,j,pix,output);
                    continue;
                }
                f.GetPix( i-1 , j+1 , p , input);
                if(p>0)
                {
                    f.GetPix( i , j , pix , input);
                    f.SetPix(i,j,pix,output);
                    continue;
                }
                f.GetPix( i , j+1 , p , input);
                if(p>0)
                {
                    f.GetPix( i , j , pix , input);
                    f.SetPix(i,j,pix,output);
                    continue;
                }
                f.GetPix( i+1 , j+1 , p , input);
                if(p>0)
                {
                    f.GetPix( i , j , pix , input);
                    f.SetPix(i,j,pix,output);
                    continue;
                }
                f.GetPix( i+1 , j , p , input);
                if(p>0)
                {
                    f.GetPix( i , j , pix , input);
                    f.SetPix(i,j,pix,output);
                    continue;
                }
                f.GetPix( i+1 , j-1 , p , input);
                if(p>0)
                {
                    f.GetPix( i , j , pix , input);
                    f.SetPix(i,j,pix,output);
                    continue;
                }
                f.GetPix( i , j-1 , p , input);
                if(p>0)
                {
                    f.GetPix( i , j , pix , input);
                    f.SetPix(i,j,pix,output);
                    continue;
                }
            }
        }
}


