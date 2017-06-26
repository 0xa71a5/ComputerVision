#include <opencv2/video/video.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>

#include <iostream>
#include <cstdio>

using namespace std;
using namespace cv;
//VideoCapture cap("x:\\CAR.avi");
	VideoCapture cap("x:\\robot.mp4");
	int framescount=cap.get(CV_CAP_PROP_FRAME_COUNT);
	int currentFrame=0;
	Mat imgFrame;

//利用光流法 计算前后两张图之间的角点位移 筛选之后计算出背景位移向量
//第一个参数是前一帧  第二个参数是第二帧 第三个参数是位移向量（用一个点坐标代替）
void getMoveVector(Mat pre,Mat current,Point2f & pt_)
{
	Mat img=pre.clone();
	Mat img2=current.clone();//img是前一帧画面  img2是第二帧画面
	
	vector<Point2f> features;
	vector<Point2f> features2;//分别用来存储两个图片的角点
	
	int d_row=300,d_col=600;
	//double time1=getTickCount();
	cvtColor(img,img,6);
	cvtColor(img2,img2,6); // cvtColor(img3,img3,6);
	
	/*
	resize(img,img,Size(700,400));
	resize(img2,img2,Size(700,400));
	resize(img3,img3,Size(700,400));
	*/
	
	goodFeaturesToTrack(img,features,600,0.01,5);//获取前一帧的角点

	vector<Point2f> points2;
	
	Mat status,err;
	
	calcOpticalFlowPyrLK(img,img2,features,points2,status,err);//利用光流法求出对应的角点位移后的在第二帧中的角点
	vector<Point2f> vecmove;

	for(int i=0;i<features.size();i++)
	{
		circle(imgFrame,points2[i],1,Scalar(255,0,0));
		line(imgFrame,features[i],points2[i],Scalar(0,0,255));
	}



	
	for(int i=0;i<features.size();i++)
	{
		vecmove.push_back(3*points2[i]-3*features[i]);//将所有在第二帧中找到的角点减去第一帧中的角点获得所有的唯一向量  数量与角点数相同 并存储起来
	}
	//waitKey(1);//不知道为什么  如果这里不暂停就会报错  可能是电脑原因
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
	Mat t1=t.clone();
	line(t1,Point(0,t.rows/2),Point(t.cols,t.rows/2),Scalar(255),1);
	line(t1,Point(t.cols/2,0),Point(t.cols/2,t.rows),Scalar(255),1);

	imshow("No blur",t1);
	Mat element=getStructuringElement(MORPH_RECT,Size(2,2));
	//erode(t,t,element);
	dilate(t,t,element);
	blur(t,t,Size(9,9));//滤波处理 将离散的去掉
	threshold(t,t,160,255,0);//二值化
	Mat t2=t.clone();
	line(t2,Point(t.cols/2,0),Point(t.cols/2,t.rows),Scalar(255),1);
	line(t2,Point(0,t.rows/2),Point(t.cols,t.rows/2),Scalar(255),1);

	imshow("test",t2);
	
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
	pt_.x=xmount;
	pt_.y=ymount;
	if(countours.size()!=0)
{
		
		for(int i=0;i<countours[maxcountour].size();i++)
		{
			xmount+=countours[maxcountour][i].x;
			ymount+=countours[maxcountour][i].y;
		}
		xmount/=countours[maxcountour].size();
		ymount/=countours[maxcountour].size();
		

		//Try to find out the opposite direction vectors
		vector<Point2f> interestPoints;
		for(int i=0;i<vecmove.size();i++)
		{
			if(xmount>0)
			{
				if(vecmove[i].x<0&&vecmove[i].x<-2*xmount)
				{
					interestPoints.push_back(points2[i]);
				}
			}
			if(xmount<0)
			{
				if(vecmove[i].x>0&&vecmove[i].x>-2*xmount)
				{
					interestPoints.push_back(points2[i]);
				}
			}

		}
		for(int i=0;i<interestPoints.size();i++)
		{
			circle(imgFrame,interestPoints[i],3,Scalar(0,255,0));
		}
		//Above... Try to find out the opposite direction vectors

	xmount=(xmount-d_col_half)/3;
	ymount=(ymount-d_row_half)/3;//之前进行了一个3倍放大  现在返回回去
	//cout<<xmount<<" "<<ymount<<endl;
	pt_.x=xmount;
	pt_.y=ymount;

}
	
}

void handler(int num)
{
	
	cap.set(CV_CAP_PROP_POS_FRAMES,num);

}
int main2()
{
	cvNamedWindow("No blur",1);
	cvMoveWindow("No blur",0,0);
	cvNamedWindow("test",1);
	cvMoveWindow("test",0,300);
	cap.set(CV_CAP_PROP_POS_FRAMES,30);
	Mat img1;//=imread("x:\\img1.jpg");
	Mat img2;//=imread("x:\\img2.jpg");
	Mat img1_cur;
	Mat img2_cur;
	Point2f point;
	
	double t=getTickCount();
	int frames=0;
	cvNamedWindow("1",1);
	cvCreateTrackbar("Frame","1",&frames,framescount,handler);
	cap>>img1;
	int wid_img1=img1.cols;
	int hei_img1=img1.rows;

	while(1)
{
	cap>>img1>>img2>>img2>>img2;
	img1_cur=img1(Rect(0,0,wid_img1,hei_img1*0.7)).clone();
	img2_cur=img2(Rect(0,0,wid_img1,hei_img1*0.7)).clone();
	img1=img1_cur.clone();
	img2=img2_cur.clone();
	imgFrame=img2.clone();
	
	getMoveVector(img1,img2,point);
	//double t2=(getTickCount()-t)/getTickFrequency();
	imshow("1",imgFrame);
	if(point!=Point2f(0,0))
	cout<<point;
	else
		cout<<"Background not move";
	char c=waitKey(0);
	
	if(c==27)
		break;

	
	currentFrame=cap.get(CV_CAP_PROP_POS_FRAMES);
	cout<<" "<<currentFrame<<endl;
	if(c!=8)
	currentFrame++;
	else
		currentFrame-=3;
	cap.set(CV_CAP_PROP_POS_FRAMES,currentFrame-1);
}


	return 0;
}