#include "thresholding.h"

using namespace cv;
using namespace std;

struct TRGB {
    float r;
    float g;
    float b;
};

///* текущий пиксель */
TRGB pix1;
const double low_pr = 0.20;
const double high_pr = 0.70;
void SetRGB(int col, int row, TRGB& setPix, Mat& output);

void DoubleThresholding(Mat& imgIn, Mat& imgOut)
{
    uint8_t* pixelPtrIN = (uint8_t*)imgIn.data;
    uint8_t* pixelPtrOUT = (uint8_t*)imgOut.data;
    double down = low_pr*255;
    double up = high_pr*255;
    pix1.g = 0;
    pix1.b = 0;
    for(int i = 0; i < imgIn.rows; i++)
        for(int j = 0; j < imgIn.cols; j++)
        {
            if(pixelPtrIN[i*imgIn.cols + j] >= up)
            {
                pix1.r = 255;
                SetRGB(j,i,pix1,imgOut);
            }
            else if (pixelPtrIN[i*imgIn.cols + j] <= down)
                {
                    pix1.r = 0;
                    SetRGB(j,i,pix1,imgOut);
                }
            else
                {
                    pix1.r = 127;
                    SetRGB(j,i,pix1,imgOut);
                }
        }
}
