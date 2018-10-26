#pragma once
#include "afxwin.h"
class CMyEdit :	public CEdit
{
public:
	CMyEdit(void);
	~CMyEdit(void);
	void CreateEdit(CRect rect,CWnd *wnd,UINT nID);
	void InvateDraw();
	CFont *m_font;
	DECLARE_MESSAGE_MAP()
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnEnChange();
	virtual void PreSubclassWindow();
	afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
};

