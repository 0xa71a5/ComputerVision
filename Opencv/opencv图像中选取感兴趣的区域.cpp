//#include "stdafx.h"
#include <cv.h>
#include <cxcore.h>
#include <highgui.h>
#include <opencv2/opencv.hpp> 
#include <iostream>

using namespace std;
int threshold=20;
IplImage *pic1;
IplImage *pic2;
IplImage *dst;
CvPoint up;
CvPoint down;
CvRect rect;

void mouseEventHandler(int eventinput,int x,int y,int flags,void *param)
{
	IplImage *picpass=(IplImage*)param;
	if(eventinput==CV_EVENT_LBUTTONDOWN)
	{
		up.x=x;
		up.y=y;
		//cvDestroyWindow("drawrec");
	}
	else if (eventinput==CV_EVENT_LBUTTONUP)
	{
		down.x=x;
		down.y=y;
		if(down.x>up.x&&down.y>up.y)
		{
			rect.x=up.x;
			rect.y=up.y;
			rect.width=down.x-up.x;
			rect.height=down.y-up.y;
			cvSetImageROI(picpass,rect);
			dst=cvCloneImage(picpass);
			cvNamedWindow("drawrec",1);
			cvShowImage("drawrec",dst);
			
		}

	}


}



void on_trackbar(int num1)
{
	cvThreshold(pic1,pic2,num1,254,0);
	cvShowImage("transfered",pic2);

}

int main( int argc, char** argv )
{
	pic1=cvLoadImage("lena.jpg");
	cvNamedWindow("image",CV_WINDOW_AUTOSIZE);
	cvShowImage("image",pic1);
	cvSetMouseCallback("image",mouseEventHandler,(void *)pic1);

	cvWaitKey(0);



	/*

	IplImage* tmppic;
	tmppic=cvLoadImage("lena.jpg");
	pic1=cvCreateImage(cvGetSize(tmppic),8,1);
	pic2=cvCreateImage(cvGetSize(tmppic),8,1);
	
	cvNamedWindow("transfered",1);
	cvNamedWindow("draw",1);
	cvRectangle(tmppic,cvPoint(10,10),cvPoint(30,40),CV_RGB(255,255,0));
	cvShowImage("draw",tmppic);

	cvWaitKey(0);

	*/

	


  return 0;
}
