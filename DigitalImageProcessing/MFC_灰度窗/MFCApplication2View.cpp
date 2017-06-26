
// MFCApplication2View.cpp : CMFCApplication2View ���ʵ��
//
#include <string>

#include "stdafx.h"
// SHARED_HANDLERS ������ʵ��Ԥ��������ͼ������ɸѡ�������
// ATL ��Ŀ�н��ж��壬�����������Ŀ�����ĵ����롣
#ifndef SHARED_HANDLERS
#include "MFCApplication2.h"
#endif

#include "MFCApplication2Doc.h"
#include "MFCApplication2View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#include <fstream>
using namespace std;

// CMFCApplication2View

IMPLEMENT_DYNCREATE(CMFCApplication2View, CView)

BEGIN_MESSAGE_MAP(CMFCApplication2View, CView)
	// ��׼��ӡ����
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CMFCApplication2View::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_CREATE()
	ON_BN_CLICKED(IDC_BTN1,OnClickBtn1)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSEWHEEL()
	ON_COMMAND(ID_FILE_OPEN, &CMFCApplication2View::OnFileOpen)
	ON_WM_HSCROLL()
END_MESSAGE_MAP()

// CMFCApplication2View ����/����

void CMFCApplication2View::grayWindowImage(int minVal,int maxVal,int left,int right,const CImage &src,CImage &dst)
{
	unsigned int width=src.GetWidth();
	unsigned int height=src.GetHeight();//��ȡԭͼ��ĳ���
	unsigned int delt=0;
	dst=CImage();
	dst.Create(width,height,24);//Ŀ��ͼ�񴴽���ͬ�ߴ�
	slided=false;
	delt=maxVal-minVal;
	
	unsigned int windowWidth=right-left;//��ûҶȴ���
	
	for(int i=0;i<width;i++)
	{
		for(int j=0;j<height;j++)
		{
			unsigned int num=GetRValue(src.GetPixel(i,j));//���ԭͼ������
			unsigned int num2;
			if(num<=left)
			{
				num2=0;//�ڴ�������ȫ����λ0
			}
			else if(num>=right)
			{
				num2=255;//�ڴ����Ҳ��ȫ����λ255
			}
			else
			{
				num2=(num-left)*255/windowWidth;//�ڴ����е����ؽ���ӳ��
			}

			dst.SetPixel(i,j,RGB(num2,num2,num2));//������ͼ�������ֵ
		}
	}
}

CMFCApplication2View::CMFCApplication2View()
{
	sliderXVal=0;
	sliderYVal=255;
	ifstream input("D:\\3.dr",std::ios::binary);
	unsigned short  *width_=new unsigned short ;
	unsigned short  *height_=new unsigned short ;
	input.read((char *)width_,2);
	input.read((char *)height_,2);
	width=*width_;
	height=*height_;
	length=width*height;
	buffer=new unsigned short [length];
	input.read((char *)buffer,length*2);
	input.close();
	posX=0;
	posY=0;
	lastMx=0;
	lastMy=0;
	pressed=false;
	lastImgX=lastImgY=0;
	div=3;
	testimg.Create(int(width/div),int(height/div),24);
	img1.Create(int(width/div),int(height/div),24);

	maxVal=0x0000;
	minVal=0xFFFF;
	for(int j=2;j<length;j++)
		{
			unsigned int tmp=buffer[j];
			if(tmp>maxVal)maxVal=tmp;
			if(tmp<minVal)minVal=tmp;
		}

	unsigned int delt=maxVal-minVal;
	for(int i=0;i<int(width/div);i++)
		for(int j=0;j<int(height/div);j++)
			{
				unsigned int num=buffer[int(j*div*width+i*div)];
				num=(num-minVal)*255/delt;
				img1.SetPixel(i,j,RGB(num,num,num));
			}
	
	//	testimg=img1;
	grayWindowImage(minVal,maxVal,0,250,img1,testimg);


	
}

CMFCApplication2View::~CMFCApplication2View()
{
}


void CMFCApplication2View::OnClickBtn1()
{
	//testimg.Draw(this->GetDC()->m_hDC,500,100);
	//UpdateWindow();
	//posX+=10;
	//posY+=10;
	

	grayWindowImage(minVal,maxVal,sliderXVal,sliderYVal,img1,testimg);
	Invalidate();
}


BOOL CMFCApplication2View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ

	return CView::PreCreateWindow(cs);
}

// CMFCApplication2View ����
/*
CImage img;
img.Create(width,height,bits);//24bits means 3 byte for rgb
img.SetPixel(x,y,RGB(r,g,b));
img.Draw(this->GetDC()->m_hDC,x,y);//����Ļ����ʾͼƬ


testimg.Draw(this->GetDC()->m_hDC,posX,posY);
*/

void CMFCApplication2View::OnDraw(CDC* /*pDC*/)
{

	testimg.Draw(this->GetDC()->m_hDC,posX,posY);
	CString tmp;
	this->GetDC()->TextOutW(700,280,_T("�Ҷȴ������"));
	this->GetDC()->TextOutW(700,300+100,_T("�Ҷȴ�λ����"));

	this->GetDC()->TextOutW(750,200,_T("Min   :         "));
	this->GetDC()->TextOutW(750,215,_T("Max   :         "));
	this->GetDC()->TextOutW(750,230,_T("Width :         "));
	this->GetDC()->TextOutW(750,245,_T("Center:         "));
	tmp.Format(_T("Min   :%d"),sliderXVal);
	this->GetDC()->TextOutW(750,200,tmp);
	tmp.Format(_T("Max   :%d"),sliderYVal);
	this->GetDC()->TextOutW(750,215,tmp);
	tmp.Format(_T("Width :%d"),sliderYVal-sliderXVal);
	this->GetDC()->TextOutW(750,230,tmp);
	tmp.Format(_T("Center:%d"),(sliderYVal+sliderXVal)/2);
	this->GetDC()->TextOutW(750,245,tmp);
}


// CMFCApplication2View ��ӡ

void CMFCApplication2View::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CMFCApplication2View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// Ĭ��׼��
	return DoPreparePrinting(pInfo);
}

void CMFCApplication2View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��Ӷ���Ĵ�ӡǰ���еĳ�ʼ������
}

void CMFCApplication2View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��Ӵ�ӡ����е��������
}

void CMFCApplication2View::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	//ClientToScreen(&point);
	//OnContextMenu(this, point);
	posX=point.x;
	posY=point.y;
	Invalidate();

}




void CMFCApplication2View::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CMFCApplication2View ���

#ifdef _DEBUG
void CMFCApplication2View::AssertValid() const
{
	CView::AssertValid();
}

void CMFCApplication2View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMFCApplication2Doc* CMFCApplication2View::GetDocument() const // �ǵ��԰汾��������
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMFCApplication2Doc)));
	return (CMFCApplication2Doc*)m_pDocument;
}
#endif //_DEBUG


// CMFCApplication2View ��Ϣ�������


int CMFCApplication2View::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	

	CRect rect_sliderX(700,300,1000,330);
	slideX.Create(WS_CHILD | WS_VISIBLE,rect_sliderX,this,IDB_SLIDERX);
	slideX.ShowWindow(SW_SHOWNORMAL);
	slideX.SetRange(0,255);
	slideX.SetPos(0); 
	slideX.SetTicFreq(1);
	
	CRect rect_sliderY(700,300+30,1000,330+30);
	slideY.Create(WS_CHILD | WS_VISIBLE,rect_sliderY,this,IDB_SLIDERX);
	slideY.ShowWindow(SW_SHOWNORMAL);
	slideY.SetRange(0,255);
	slideY.SetPos(255); 
	slideY.SetTicFreq(1);

	CRect rect_sliderC(700,300+120,1000,330+120);
	slideC.Create(WS_CHILD | WS_VISIBLE,rect_sliderC,this,IDB_SLIDERX);
	slideC.ShowWindow(SW_SHOWNORMAL);
	slideC.SetRange(0,255);
	slideC.SetPos(125); 
	slideC.SetTicFreq(1);
	
	CRect rect_button(820,300+250,880,330+250); 
	btn.Create(_T("change"),WS_CHILD|WS_VISIBLE|WS_BORDER,rect_button,this,IDC_BTN1);
	btn.ShowWindow(SW_SHOWNORMAL);


	return 0;
}


void CMFCApplication2View::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	//CView::OnLButtonDown(nFlags, point);
	pressed=true;
	lastPosX=point.x;
	lastPosY=point.y;

	//lastImgX
	Invalidate();
}


void CMFCApplication2View::OnLButtonUp(UINT nFlags, CPoint point)
{
	pressed=false;
	CView::OnLButtonUp(nFlags, point);
	lastImgX=lastImgX+point.x-lastPosX;
	lastImgY=lastImgY+point.y-lastPosY;//��ఴ�����µ�ʱ���¼��ʼͼ��
	/*								//����ֵ�ͳ�ʼ�������ֵ
	MessageBox(_T("123@"));
	if(slided==true)
	{
		slided=false;
		CString tmp;
		tmp.Format(_T("%d"),sliderXVal);
		MessageBox(tmp);
	}
	*/
	
}


void CMFCApplication2View::OnMouseMove(UINT nFlags, CPoint point)
{
	if(pressed==true)//��⵽���������ס��ʱ�����ͼƬ�϶�
	{
		posX=lastImgX+(point.x-lastPosX);
		posY=lastImgY+(point.y-lastPosY);//�϶�λ�ü�Ϊ�������ڳ�ʼλ�õ�λ��
		Invalidate();
	}
}


BOOL CMFCApplication2View::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	//HANDLE a=CreateThread();
	if(zDelta>0)
		div+=0.5;
	else if(zDelta<0)
	{
		if(div>1)
		div-=0.3;
		else
			div-=0.15;
	}
	if(div<1)//�Ŵ�������������ڽ�Ԫ��ֵ
	{
	if(div==0)div=0.1;//���Ŵ�ֵ��Ϊ�����
		int t_width=width/div;//�Ŵ��Ŀ��
		int t_height=height/div;//�Ŵ��ĸ߶�
		testimg.Destroy();
		testimg=CImage();
		testimg.Create(t_width,t_height,24);
		float step=1.f/div;//�Ŵ�������ԭͼ����ÿ������֮�������µ���������
		int i0=height/2;
		
		for(int i=0;i<i0;i++)
		{
			for(int j=0;j<width;j++)
			{
				unsigned int num=buffer[int(i*width)+j];//��ȡ���ڽ�Ԫ������ֵ
				int t1=j*step-step/2,t2=j*step+step/2;//�������ڽ�Ԫ����������Χ
					for(int k=t1;k<t2;k++)
					{
						int t3=i*step-step/2,t4=i*step+step*2;//�������ڽ�Ԫ����������Χ
						for(int m=t3;m<t4;m++)
						{
							if(k<=0)k=0;
							if(k>=t_width)k=t_width-1;
							if(m<=0)m=0;
							if(m>=t_height)m=t_height-1;//�����Ե���ز����ڴ�Խ��
							testimg.SetPixel(k,m,RGB(num,num,num));//�������ڽ�Ԫ���ػҶ�ֵ��ͬ
						}
					}
			}
		}
	}
	else
	{
		testimg.Destroy();
		testimg=CImage();
		testimg.Create(int(width/div),int(height/div),24);//������С֮���ͼ��ߴ�
		for(int i=0;i<int(width/div);i++)
			for(int j=0;j<int(height/div);j++)
				{
					unsigned int num=buffer[j*(int)div*(int)width+int(float(i)*div)];//������ȡ���ػҶ�ֵ
					testimg.SetPixel(i,j,RGB(num,num,num));
				}
	}

	Invalidate();

	return CView::OnMouseWheel(nFlags, zDelta, pt);
}


void CMFCApplication2View::OnFileOpen()
{
	CString FilePathName;
    CFileDialog dlg(TRUE, //TRUEΪOPEN�Ի���FALSEΪSAVE AS�Ի���
        NULL, 
        NULL,
        OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
        (LPCTSTR)_TEXT("RAW Files (*.dr)|*.dr|All Files (*.*)|*.*||"),
        NULL);
    if(dlg.DoModal()==IDOK)
    {
        FilePathName=dlg.GetPathName(); //�ļ�����������FilePathName��

		
		sliderXVal=0;
	sliderYVal=255;
	ifstream input(FilePathName,std::ios::binary);
	unsigned short  *width_=new unsigned short ;
	unsigned short  *height_=new unsigned short ;
	input.read((char *)width_,2);
	input.read((char *)height_,2);
	width=*width_;
	height=*height_;
	length=width*height;
	buffer=new unsigned short [length];
	input.read((char *)buffer,length*2);
	input.close();
	posX=0;
	posY=0;
	lastMx=0;
	lastMy=0;
	pressed=false;
	lastImgX=lastImgY=0;
	div=3;
	testimg=CImage();
	img1=CImage();
	testimg.Create(int(width/div),int(height/div),24);
	img1.Create(int(width/div),int(height/div),24);

	maxVal=0x0000;
	minVal=0xFFFF;
	for(int j=2;j<length;j++)
		{
			unsigned int tmp=buffer[j];
			if(tmp>maxVal)maxVal=tmp;
			if(tmp<minVal)minVal=tmp;
		}


	
	unsigned int delt=maxVal-minVal;
	for(int i=0;i<int(width/div);i++)
		for(int j=0;j<int(height/div);j++)
			{
				unsigned int num=buffer[int(j*div*width+i*div)];
				num=(num-minVal)*255/delt;
				img1.SetPixel(i,j,RGB(num,num,num));
			}
	
	//	testimg=img1;
	grayWindowImage(minVal,maxVal,0,250,img1,testimg);



			Invalidate();
    }
    else
    {
         return;
    }

}


void CMFCApplication2View::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	
	if(slideY.GetPos()<slideX.GetPos()+2)
	{

		if(slideY.GetPos()==sliderYVal)
		{
			slideX.SetPos(sliderYVal-2);
		}
		else
		{
			slideY.SetPos(sliderXVal+2);
		}
		return;
	}
	if(slideC.GetPos()!=sliderCVal)
	{
		slideX.SetPos(sliderXVal+slideC.GetPos()-sliderCVal);
		slideY.SetPos(sliderYVal+slideC.GetPos()-sliderCVal);
	}
	CString tmp;
	this->GetDC()->TextOutW(750,200,_T("Min   :         "));
	this->GetDC()->TextOutW(750,215,_T("Max   :         "));
	this->GetDC()->TextOutW(750,230,_T("Width :         "));
	this->GetDC()->TextOutW(750,245,_T("Center:         "));
	tmp.Format(_T("Min   :%d"),sliderXVal);
	this->GetDC()->TextOutW(750,200,tmp);
	tmp.Format(_T("Max   :%d"),sliderYVal);
	this->GetDC()->TextOutW(750,215,tmp);
	tmp.Format(_T("Width :%d"),sliderYVal-sliderXVal);
	this->GetDC()->TextOutW(750,230,tmp);
	tmp.Format(_T("Center:%d"),(sliderYVal+sliderXVal)/2);
	this->GetDC()->TextOutW(750,245,tmp);
	sliderXVal=slideX.GetPos();//3
	sliderYVal=slideY.GetPos();//2
	sliderCVal=(sliderXVal+sliderYVal)/2;
	slideC.SetPos(sliderCVal);
	
	
	slided=true;

}
