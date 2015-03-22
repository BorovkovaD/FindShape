#include "function.h"


Function::Function()
{
    /*detect(filename);
    Mat Canny =  detect.start();
    Mat img;
    img = cv::imread(filename);
    lines.img = Canny;
    lines.img1 = img;
    lines.start();*/

}

Function::~Function()
{
}

void Function::SetPix(int row, int col, float &setPix, Mat& output)
{
    uint8_t* ptr = (uint8_t*)output.data;
    assert (row < output.rows);
    assert (col < output.cols);
    ptr[row * output.cols + col ] = setPix;
}

void Function::GetPix (int row, int col, float &retPix, Mat& input)
{
    uint8_t* ptr = (uint8_t*)input.data;
    assert (row < input.rows);
    assert (col < input.cols);
    retPix =  ptr[row * input.cols + col];
}
