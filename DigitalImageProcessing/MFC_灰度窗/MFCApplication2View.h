
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
	float width;
	float height;
	int length;
	int sliderXVal;
	int sliderYVal;
	int sliderCVal;
	unsigned int maxVal,minVal;
	unsigned short *buffer;
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

