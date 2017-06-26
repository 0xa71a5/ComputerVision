#include <opencv2/video/video.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>

#include <iostream>
#include <cstdio>

using namespace std;
using namespace cv;

//利用光流法 计算前后两张图之间的角点位移 筛选之后计算出背景位移向量
//第一个参数是前一帧  第二个参数是第二帧 第三个参数是位移向量（用一个点坐标代替）
void getMoveVector(Mat pre,Mat current,Point2f & pt_)
{
	Mat img=pre.clone();
	Mat img2=current.clone();//img是前一帧画面  img2是第二帧画面
	
	vector<Point2f> features;
	vector<Point2f> features2;//分别用来存储两个图片的角点
	
	int d_row=300,d_col=700;
	//double time1=getTickCount();
	cvtColor(img,img,6);
	cvtColor(img2,img2,6); // cvtColor(img3,img3,6);
	
	/*
	resize(img,img,Size(700,400));
	resize(img2,img2,Size(700,400));
	resize(img3,img3,Size(700,400));
	*/
	
	goodFeaturesToTrack(img,features,500,0.01,10);//获取前一帧的角点

	vector<Point2f> points2;
	
	Mat status,err;
	
	calcOpticalFlowPyrLK(img,img2,features,points2,status,err);//利用光流法求出对应的角点位移后的在第二帧中的角点
	vector<Point2f> vecmove;

	
	for(int i=0;i<features.size();i++)
	{
		vecmove.push_back(3*points2[i]-3*features[i]);//将所有在第二帧中找到的角点减去第一帧中的角点获得所有的唯一向量  数量与角点数相同 并存储起来
	}
	waitKey(1);//不知道为什么  如果这里不暂停就会报错  可能是电脑原因
	//cout<<vecmove;
	Mat t(d_row,d_col,CV_8UC1(0));
	t=Scalar(0);
	int d_row_half=d_row/2,d_col_half=d_col/2;
	for(int i=0;i<vecmove.size();i++)
	{
		
		int rowt=d_row_half+vecmove[i].y,colt=d_col_half+vecmove[i].x;//在一个全黑的图片矩阵中画所有的唯一向量（点集)
	//	cout<<i<<" "<<wid<<" "<<hei<<endl;
		if(rowt>d_row-10||rowt<0||colt>d_col||colt<0)//防止越界
			continue;
		t.at<uchar>(rowt,colt)=255;
	}
	
	
	blur(t,t,Size(5,5));//滤波处理 将离散的去掉
	threshold(t,t,160,255,0);//二值化

	
	vector<vector<Point>> countours;
		vector<Vec4i> hierarchy;
	findContours(t,countours,hierarchy,CV_RETR_TREE,CV_CHAIN_APPROX_SIMPLE,Point(0,0));//找出最大轮廓
	

	int countoursize=0,maxcountour=0;
	for(int i=0;i<countours.size();i++)
		{
			if(fabs(contourArea(countours[i]))>countoursize)
			{
				countoursize=fabs(contourArea(countours[i]));
				maxcountour=i;
			}
		}
	double xmount=0,ymount=0;
	if(countours.size()!=0)
{
		
		for(int i=0;i<countours[maxcountour].size();i++)
		{
			xmount+=countours[maxcountour][i].x;
			ymount+=countours[maxcountour][i].y;
		}
		xmount/=countours[maxcountour].size();
		ymount/=countours[maxcountour].size();
}
	
	xmount=(xmount-d_col_half)/3;
	ymount=(ymount-d_row_half)/3;//之前进行了一个3倍放大  现在返回回去
	//cout<<xmount<<" "<<ymount<<endl;
	pt_.x=xmount;
	pt_.y=ymount;

}


int main()
{
	Mat img1;//=imread("x:\\img1.jpg");
	Mat img2;//=imread("x:\\img2.jpg");
	Point2f point;
	double t=getTickCount();
	VideoCapture cap("x:\\robot.mp4");
	while(cap.get(CV_CAP_PROP_POS_FRAMES)<cap.get(CV_CAP_PROP_FRAME_COUNT)-100)
{
	cap>>img1>>img2>>img2>>img2;
	getMoveVector(img1,img2,point);
	double t2=(getTickCount()-t)/getTickFrequency();
	imshow("1",img1);
	cout<<point<<endl;
	char c=waitKey(30);
	if(c==27)
		break;
}


	return 0;
}