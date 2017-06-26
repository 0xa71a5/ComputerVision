#include <cv.h>
#include <cxcore.h>
#include <highgui.h>
#include <math.h>
#include <iostream>
using namespace std;
using namespace cv;
int numofthreshold=157;
int numoferosion=1;
VideoCapture cap("X:\\road.avi");
char flag='l';
//22ms  
void trackbar(int num)
{
	cap.set(CV_CAP_PROP_POS_FRAMES,num);
	
	Mat video;
	cap>>video;
	double t=getTickCount();
	GaussianBlur(video,video,Size(3,3),0,0,BORDER_DEFAULT);
	cvtColor(video,video,CV_BGR2GRAY);
	threshold(video,video,numofthreshold,255,3);
	Canny(video,video,30,90);
	vector<Vec4i> lines;
	HoughLinesP(video, lines, 1, CV_PI/180,100,100,300);

	Mat outmat(video.cols,video.rows,CV_8UC1(0));
	outmat=Scalar(0);
	cout<<int(lines.size())<<"   \n";
	double length;
	double maxX=lines[0][0],itemMax=0;
	if(lines.size()>1)
	{
	for(int i=1;i<lines.size();i++)
	{
		if(lines[i][0]>maxX)
		{maxX=lines[i][0];itemMax=i;}

	}
	}

	//for( size_t i = 0; i < max((int)lines.size(),2); i++ )

  Vec4i l = lines[itemMax];
  length=sqrt((l[2]-l[0])*(l[2]-l[0])+(l[3]-l[1])*(l[3]-l[1]));
  line( outmat, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(255), 1, CV_AA);
 // printf("<%d>(%f,%f)  (%f,%f)  ",(int)i,l[0],l[1],l[2],l[3]);
  cout<<"item"<<itemMax<<" length:"<<int(length)<<"  P1("<<(int)l[0]<<","<<(int)l[1]<<")  P2("<<(int)l[2]<<","<<(int)l[3]<<")"<<endl;
	
	cout<<(getTickCount()-t)/getTickFrequency()<<endl;
	//
	imshow("1",outmat);
	

}

int main()
{
	Mat img3;
	int start=180;
	cvNamedWindow("1",1);
	
	Mat video;
	cap.set(CV_CAP_PROP_POS_FRAMES,10);
	double rate=cap.get(CV_CAP_PROP_FPS);
	double waittime=1000/rate;
	long int currentframe=0;
	long int frametostop=cap.get(CV_CAP_PROP_FRAME_COUNT);
	Mat rot,out;
	double width=cap.get(CV_CAP_PROP_FRAME_WIDTH);
	double height=cap.get(CV_CAP_PROP_FRAME_HEIGHT);
	int startvalue=10;
	cvCreateTrackbar("frames","1",&startvalue,frametostop-50,trackbar);
	cout<<frametostop<<endl;
	while(flag!=27)
	{
		flag=cvWaitKey(0);
	}
	return 0;
}
