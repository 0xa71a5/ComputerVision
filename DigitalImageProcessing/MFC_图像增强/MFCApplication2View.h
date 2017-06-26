
// MFCApplication2View.h : CMFCApplication2View 类的接口
//

#pragma once


class CMFCApplication2View : public CView
{
protected: // 仅从序列化创建
	CMFCApplication2View();
	DECLARE_DYNCREATE(CMFCApplication2View)

// 特性
public:
	CMFCApplication2Doc* GetDocument() const;

// 操作
public:
	CButton btn;
	CSliderCtrl slideX;
	CSliderCtrl slideY;
	CSliderCtrl slideC;
	int aaa;
	CImage img; 
	CImage img2;
	CButton btn1;
	CButton btnSharp;
	CButton btnSmooth;
	CButton btnInvert;
	CButton btnGamma;
	CButton btnReset;
	float width;
	float height;
	int length;
	int currentHeight;
	int currentWidth;
	int sliderXVal;
	int sliderYVal;
	int sliderCVal;
	unsigned int maxVal,minVal;
	unsigned short *buffer;
	unsigned short **buffer2;
	unsigned short **currentImg;
	float **imgData;
	CImage testimg;
	bool slided;
	CImage img1;
	float div;
	int posX,posY;
	int lastMx,lastMy;
	int lastImgX,lastImgY;
	int lastPosX,lastPosY;
	bool pressed;
// 重写
public:
	void grayWindowImage(int maxV,int minVal,int left,int right,const CImage &src,CImage &dst);
	virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	void clone(unsigned short  **src,unsigned short  **dst);
	void mallocArray(float **&a,int rows,int cols);
	void freeArray(float **a);
	void mallocArrayWord(unsigned short **&a,int rows,int cols);
	void freeArrayWord(unsigned short **a);
	void sharping(unsigned short **src,unsigned short **dst,float k,int method);
	void contrast(unsigned short **src,unsigned short **dst,float a,float b);
	void negativeImage(unsigned short **src,unsigned short **dst);
	void smooth(unsigned short **src,unsigned short **dst,int flag);
	void gamma(unsigned short **src,unsigned short **dst,float g,float p);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 实现
public:
	virtual ~CMFCApplication2View();

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnClickBtn1();  
	afx_msg void OnClickBtnSharp();  
	afx_msg void OnClickBtnSmooth();
	afx_msg void OnClickBtnGamma();
	afx_msg void OnClickBtnInvert();
	afx_msg void OnClickBtnReset();


	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnFileOpen();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
};

#ifndef _DEBUG  // MFCApplication2View.cpp 中的调试版本
inline CMFCApplication2Doc* CMFCApplication2View::GetDocument() const
   { return reinterpret_cast<CMFCApplication2Doc*>(m_pDocument); }
#endif

