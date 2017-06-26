//#include "stdafx.h"
#include <cv.h>
#include <cxcore.h>
#include <highgui.h>
#include <opencv2/opencv.hpp> 
#include <iostream>
using namespace std;
int threshold=20;
void on_trackbar(int num1)
{
	threshold=num1;

}

int main( int argc, char** argv )
{
  //声明IplImage指针
  IplImage* pFrame = NULL;

 //获取摄像头
  CvCapture* pCapture = cvCreateCameraCapture(-1);
 
  //创建窗口
  cvNamedWindow("video", 1);
   pFrame=cvQueryFrame( pCapture );
 
   IplImage* g_pCannyImg = cvCreateImage(cvGetSize(pFrame), IPL_DEPTH_8U, 1);  
//   cvNamedWindow("video_conver",1);
   g_pCannyImg=pFrame;
 //  cvShowImage("video_conver",g_pCannyImg);
   IplImage *img2;
   img2=cvCreateImage(cvGetSize(pFrame),pFrame->depth,1);
   cvCvtColor(pFrame,img2,CV_BGR2GRAY);
  // cvNamedWindow("test",1);
  // cvShowImage("test",img2);

   g_pCannyImg = cvCreateImage(cvGetSize(pFrame),IPL_DEPTH_8U, 1); // IPL_DEPTH_8U
   cout<<pFrame->depth;
  int nThresholdEdge = 1;  
  cvCreateTrackbar("Threshold","video", &nThresholdEdge, 100, on_trackbar);  
  //显示视屏
  while(1)
  {
      pFrame=cvQueryFrame( pCapture );
      if(!pFrame)break;
	  cvCvtColor(pFrame,img2,CV_BGR2GRAY);
	  cvCanny(img2, g_pCannyImg, threshold,threshold*3, 3);  
	  cvShowImage("video",g_pCannyImg);
      char c=cvWaitKey(33);
      if(c==27)break;

  }
  cvReleaseCapture(&pCapture);
  cvDestroyWindow("video");

  return 0;
}