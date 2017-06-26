
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
END_MESSAGE_MAP()

// CMFCApplication2View ����/����

CMFCApplication2View::CMFCApplication2View()
{
	// TODO: �ڴ˴���ӹ������
	ifstream input("D:\\feipian.raw",std::ios::binary);
	width=2048;
	height=2099;
	length=2048*2099;
	buffer=new char [length];
	input.read(buffer,length);
	input.close();
	posX=0;
	posY=0;
	div=5;
	pressed=false;
	lastImgX=lastImgY=0;
	testimg.Create(int(width/div),int(height/div),24);
	

	
	for(int i=0;i<int(width/div);i++)
		for(int j=0;j<int(height/div);j++)
			{
				unsigned int num=buffer[int(j*div*width+i*div)];
				testimg.SetPixel(i,j,RGB(num,num,num));
			}
			

		//new

	
		
		


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
	div--;

	testimg=CImage();
	testimg.Create(width/div,height/div,24);
	
	for(int i=0;i<width/div;i++)
		for(int j=0;j<height/div;j++)
			{
				unsigned int num=buffer[int(j*div*width+i*div)];
				testimg.SetPixel(i,j,RGB(num,num,num));
			}


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
*/

void CMFCApplication2View::OnDraw(CDC* /*pDC*/)
{
	//CImage testimg;
	 testimg.Draw(this->GetDC()->m_hDC,posX,posY);
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

	//
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
	CRect rect_button(40+600,300,100+600,330); 
	//btn1.Create(_T("change"),WS_CHILD|WS_VISIBLE|WS_BORDER,rect_button,this,IDC_BTN1);
	//btn1.ShowWindow(SW_SHOWNORMAL);
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
									//����ֵ�ͳ�ʼ�������ֵ
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
		div-=0.3;
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
		int i0=height/4;
		float width=2048,height=2099;//ԭʼ��Ⱥ͸߶�
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
