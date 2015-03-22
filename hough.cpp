#include "hough.h"

Hough::Hough()
{
}

void Hough::start()
{

}

Mat Hough::FindCircle()
{
    vector<Vec3f> circles;
    HoughCircles(img, circles, CV_HOUGH_GRADIENT,
                 2, img.rows/4, 200, 100 );
    img1.zeros(img.rows,img.cols,0);
    for( size_t i = 0; i < circles.size(); i++ )
    {
         Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
         int radius = cvRound(circles[i][2]);
         // draw the circle center
         circle( img1, center, 3, Scalar(0,255,0), -1, 8, 0 );
         // draw the circle outline
         circle( img1, center, radius, Scalar(0,0,255), 3, 8, 0 );
    }
    return img1;
}

Mat Hough::FindTriangle()
{
    vector<Vec4i> lines;
    //cvtColor(img, cdst, COLOR_GRAY2BGR);
    HoughLinesP(img, lines, 1, CV_PI/180, 50, 50, 10 );
    for( size_t i = 0; i < lines.size(); i++ )
    {
        Vec4i l = lines[i];
        line( img1, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(0,0,255), 3, LINE_AA);
    }
        /*qDebug()<<"1";
        Vec4i l1;
        Vec4i l2;
        Vec4i l3;
        int flag1 = 0;
        int flag2 = 0;
        l1 = lines[i];
        for( size_t j = i+1; j < lines.size(); j++ )
        {
            Vec4i l = lines[j];
            if(((abs(l[0]-l1[0])<5) && (abs(l[1]-l1[1])<5)) || ((abs(l[2]-l1[0])<5) && (abs(l[3]-l1[1])<5)))
            {
                l2 = lines[j];
                flag1 = 1;
            }
        }
        if (flag1>0)
        {
            for( size_t j = 0; j < lines.size(); j++ )
            {
                Vec4i l = lines[j];
                if((abs(l[0]-l1[2])<5) && (abs(l[1]-l1[3])<5))
                {
                    if(((abs(l[2]-l2[0])<5) && (abs(l[3]-l2[1])<5)) || ((abs(l[2]-l2[2])<5) && (abs(l[3]-l2[3])<5)))
                    {
                        flag2 = 1;
                        l3 = lines[j];
                    }
                }
                else if ((abs(l[2]-l1[2])<5) && (abs(l[3]-l1[3])<5))
                {
                    if(((abs(l[0]-l2[0])<5) && (abs(l[1]-l2[1])<5)) || ((abs(l[0]-l2[2])<5) && (abs(l[1]-l2[3])<5)))
                    {
                        flag2 = 1;
                        l3 = lines[j];
                    }
                }
            }
        }
        if(flag2>0)
        {
            double angle;
            double k1 = (l1[3]-l1[1])/(l1[2]-l1[0]);
            double k2 = (l2[3]-l2[1])/(l2[2]-l2[0]);
            double k3 = (l3[3]-l3[1])/(l3[2]-l3[0]);
            if(1+k1*k2!=0 && 1+k2*k3!=0 && 1+k1*k2!=0)
               angle = atan((k2-k1)/(1+k1*k2)) + atan((k3-k2)/(1+k3*k2)) + atan((k1-k3)/(1+k1*k3));
            else if(1+k1*k2==0&& 1+k2*k3!=0 && 1+k1*k2!=0)
                angle = CV_PI/2 + atan((k3-k2)/(1+k3*k2)) + atan((k1-k3)/(1+k1*k3));
            else if(1+k2*k3==0&& 1+k1*k2!=0 && 1+k1*k2!=0)
                angle = atan((k2-k1)/(1+k1*k2)) + CV_PI/2 + atan((k1-k3)/(1+k1*k3));
            else if (1+k2*k3==0&& 1+k1*k3!=0 && 1+k1*k2!=0)
                angle = atan((k2-k1)/(1+k1*k2)) + atan((k3-k2)/(1+k3*k2)) + CV_PI/2;
            qDebug()<<angle;
            if(angle >CV_PI - CV_PI/8 && angle< CV_PI - CV_PI/8 )
            {
                line( img1, Point(l1[0], l1[1]), Point(l1[2], l1[3]), Scalar(0,0,255), 3, LINE_AA);
                line( img1, Point(l2[0], l2[1]), Point(l2[2], l2[3]), Scalar(0,0,255), 3, LINE_AA);
                line( img1, Point(l3[0], l3[1]), Point(l3[2], l3[3]), Scalar(0,0,255), 3, LINE_AA);
            }
        }
    }*/
    return img1;
}
