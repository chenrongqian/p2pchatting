#include "stdafx.h"
#include "MyEdit.h"
#include "resource.h"

CMyEdit::CMyEdit(void)
{
}


CMyEdit::~CMyEdit(void)
{
}
BEGIN_MESSAGE_MAP(CMyEdit, CEdit)
	ON_WM_ERASEBKGND()
	ON_CONTROL_REFLECT(EN_CHANGE, &CMyEdit::OnEnChange)
	ON_WM_CTLCOLOR_REFLECT()
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()

BOOL CMyEdit::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	return TRUE;
	//return CEdit::OnEraseBkgnd(pDC);
}


void CMyEdit::OnEnChange()
{
	CRect rc;
	CWnd *pParent = GetParent();
	GetWindowRect(&rc);
	pParent->ScreenToClient(&rc);
	pParent->InvalidateRect(&rc, FALSE);
}

void CMyEdit::PreSubclassWindow()
{
	// TODO: 在此添加专用代码和/或调用基类
	m_font = NULL;
	
	//CFont *font = GetFont();
	CEdit::PreSubclassWindow();
}

HBRUSH CMyEdit::CtlColor(CDC* pDC, UINT nCtlColor)
{
	// TODO:  在此更改 DC 的任何特性
	HBRUSH hNullBr = (HBRUSH)::GetStockObject(NULL_BRUSH);
	if(m_font == NULL)
	{
		m_font = new CFont();
		m_font->CreatePointFont(100,"微软雅黑");
		SetFont(m_font);
	}
	pDC->SetBkMode(TRANSPARENT);
	return hNullBr;
}

void CMyEdit::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CRect rc;
	CWnd *pParent = GetParent();
	GetWindowRect(&rc);
	pParent->ScreenToClient(&rc);
	pParent->InvalidateRect(&rc, FALSE);
	CEdit::OnLButtonDown(nFlags, point);
}

void CMyEdit::CreateEdit(CRect rect,CWnd *wnd,UINT nID)
{
	this->Create(ES_MULTILINE|WS_CHILD|WS_VISIBLE,rect,wnd,nID);
}

void CMyEdit::InvateDraw()
{
	CRect rc;
	CWnd *pParent = GetParent();
	GetWindowRect(&rc);
	pParent->ScreenToClient(&rc);
	pParent->InvalidateRect(&rc, FALSE);
}