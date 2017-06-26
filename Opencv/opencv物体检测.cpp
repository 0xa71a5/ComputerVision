#include <iostream>
#include <string>

#include <opencv2/opencv.hpp>
using namespace cv;
using namespace std;
void detectObject(Mat img,int Hmin, int Hmax, Scalar color,string name)
{
	Mat imgHSV;
	Mat tmp = img.clone();
	cvtColor(img,imgHSV,CV_RGB2HSV);
	MatIterator_<Vec3b> it, end;  
	int H,S,V;
    for( it = imgHSV.begin<Vec3b>(), end = imgHSV.end<Vec3b>(); it != end; ++it)  
    {  
        H = (*it)[0];S = (*it)[1];V = (*it)[2];
		if ( (H<Hmax && H>Hmin) && S>43 && V>46  )
		{
				(*it)[0] = 0;
				(*it)[1] = 0;
				(*it)[2] = 0;
		}
		else
		{
			(*it)[0] = 255;
			(*it)[1] = 255;
			(*it)[2] = 255;
		}
    } 
	
	Mat element = getStructuringElement(MORPH_RECT, Size(3, 3));   
    morphologyEx(imgHSV,imgHSV, MORPH_DILATE, element);
	imshow("after processing",imgHSV);
	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;	
	Canny(imgHSV, imgHSV, 80, 255, 3);  
	 findContours(imgHSV, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));  
	double maxArea = 0;double maxIndex = 0;
	if (contours.size()>=1)
	{
		Point curPoint;
		for (int i = 0; i<contours.size(); i++)
		{
			Moments mom = moments(contours[i]);  
			curPoint = Point(mom.m10/mom.m00,mom.m01/mom.m00);
			if (contourArea(contours[i])>5)
				circle(tmp,curPoint,50,color,3);
			//drawContours(tmp,contours,-1,color,3);
		}
	}
	imshow(name,tmp);
}
int main()
{
	Mat img;
	img = imread("1.png");
	imshow("previous",img);
	GaussianBlur(img,img,Size(5,5),1.5);

	detectObject(img,121,126,Scalar(0,0,255),"red");
	detectObject(img,0,25,Scalar(255,0,0),"blue");
	detectObject(img,78,99,Scalar(0,255,255),"Yellow");
	

	waitKey(0);
}