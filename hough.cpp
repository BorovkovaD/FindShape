#include "hough.h"

Hough::Hough()
{
}

Hough::~Hough()
{
}

Mat Hough::FindCircle()
{
    vector<Vec3f> circles;
    HoughCircles(img, circles, CV_HOUGH_GRADIENT,
                 2, img.rows/4, 100, 100 );
    for( size_t i = 0; i < circles.size(); i++ )
    {
         Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
         int radius = cvRound(circles[i][2]);
         // draw the circle center
         circle( img1, center, 3, Scalar(0,255,0), -1, 2, 0 );
         // draw the circle outline
         circle( img1, center, radius, Scalar(0,0,255), 3, 2, 0 );
    }
    return img1;
}

Mat Hough::FindLines()
{
    vector<Vec4i> lines;
    HoughLinesP(img, lines, 1, CV_PI/180, 50, 50, 10 );
    for( size_t i = 0; i < lines.size(); i++ )
    {
        Vec4i l = lines[i];
        line( img1, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(0,0,255), 2, LINE_AA);
    }
    return img1;
}
