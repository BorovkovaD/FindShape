#include <QCoreApplication>
#include "detectorcanny.h"
#include "hough.h"

int main(int argc, char *argv[])
{
    // имя картинки задаётся первым параметром
    string filename = "C:/Qt/4.png\0";
    DetectorCanny image(filename);
    Mat Canny = image.start();
    imshow("Canny", Canny);
    Hough lines;
    lines.img = Canny;
    lines.start();

    cvWaitKey(0);
    cvDestroyAllWindows();
    return 0;
}
