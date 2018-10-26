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
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

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
	// TODO: �ڴ����ר�ô����/����û���
	m_font = NULL;
	
	//CFont *font = GetFont();
	CEdit::PreSubclassWindow();
}

HBRUSH CMyEdit::CtlColor(CDC* pDC, UINT nCtlColor)
{
	// TODO:  �ڴ˸��� DC ���κ�����
	HBRUSH hNullBr = (HBRUSH)::GetStockObject(NULL_BRUSH);
	if(m_font == NULL)
	{
		m_font = new CFont();
		m_font->CreatePointFont(100,"΢���ź�");
		SetFont(m_font);
	}
	pDC->SetBkMode(TRANSPARENT);
	return hNullBr;
}

void CMyEdit::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
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