#include "function.h"

Function::Function()
{
}

Function::~Function()
{
}

void Function::SetPix(int row, int col, TRGB& setPix, Mat& output)
{
    uint8_t* ptr = (uint8_t*)output.data;
    assert (row < output.rows);
    assert (col < output.cols);
    ptr[row * output.cols + col ] = setPix.r;
}

void Function::GetPix (int row, int col, TRGB& retPix, Mat& input)
{
    uint8_t* ptr = (uint8_t*)input.data;
    assert (row < input.rows);
    assert (col < input.cols);
    retPix.r =  ptr[row * input.cols + col];
    retPix.g = 0;
    retPix.b = 0;
}

void Function::GaussianFilterOperator (float sigma, Mat input, Mat& output)
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
            pix.r = 0;
            pix.g = 0;
            pix.b = 0;
            /* проходимся окном вокруг этого пикселя */
            for ( k = maxWin-N; k <= maxWin+N; k++){
                /* l - индекс другого пикселя ,
                который вносит вклад */
                l = j + k-maxWin-N;
                if (( l >= 0 ) && ( l < width )) {
                    GetPix( i , l , p , input);
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
            tmp[j]= pix;
        }
        /* строка готова - сохраняем временный массив
        в само изображение */
        for ( j = 0; j < width; j++)
            SetPix ( i , j , tmp[j], output );
    }
    /* второй проход - вертикальное размытие */
    for ( j = 0; j < width; j++)
    {
        for ( i = 0; i < height; i++)
        {
            double sum = 0;
            pix.r = 0;
            pix.g = 0;
            pix.b = 0;
            for ( k = maxWin-N; k <= maxWin+N; k++){
                /* отличие в том, что итерация теперь будет
                не по строке , а по столбцу */
                l = i + k-maxWin-N;
                if (( l >= 0 ) && ( l < height )) {
                    GetPix( l , j , p, output);
                    pix.r += p.r * window[k];
                    pix.g += p.g * window[k];
                    pix.b += p.b * window[k];
                    sum += window[k];
                }
            }
            pix.r /= sum;
            pix.g /= sum;
            pix.b /= sum;
            tmp[i]= pix;
        }
        for ( i = 0; i < height; i++)
            SetPix ( i , j , tmp[i], output );
    }
}
