// MyButton.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "PicButton.h"


// CPicButton

IMPLEMENT_DYNAMIC(CPicButton, CButton)

CPicButton::CPicButton()
:m_IsMoveOn(false)
{

}

CPicButton::~CPicButton()
{

}


BEGIN_MESSAGE_MAP(CPicButton, CButton)
	ON_WM_MOUSEMOVE()
	//ON_WM_MOUSELEAVE()
	ON_MESSAGE(WM_MOUSELEAVE, OnMouseLeave)
END_MESSAGE_MAP()



// CPicButton ��Ϣ�������

void CPicButton::LoadImg(UINT ID, int nWidth, int nHeight)
{
	m_Width = nWidth;
	m_Height = nHeight;
	CRect rect;
	this->GetWindowRect(&rect);
	this->MoveWindow(rect.left,rect.top,m_Width,m_Height);
	m_bitmap.LoadBitmap(ID);
}

void CPicButton::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	// TODO:  ������Ĵ����Ի���ָ����
	CDC dc,cdc;
	dc.Attach(lpDrawItemStruct->hDC);
	CRect rect(lpDrawItemStruct->rcItem);
	cdc.CreateCompatibleDC(&dc);
	cdc.SelectObject(&m_bitmap);
	SetBkMode(lpDrawItemStruct->hDC,TRANSPARENT);
	ASSERT(lpDrawItemStruct->CtlType == ODT_BUTTON);
	dc.SetStretchBltMode(COLORONCOLOR);
		
	if (lpDrawItemStruct->itemState & ODS_DISABLED) //��ť������״̬
	{
		//dc.StretchBlt(0,0,rect.Width(),rect.Height(),&cdc,0,0,csize.cx,csize.cy,SRCCOPY);
	}
	else if (lpDrawItemStruct->itemState & ODS_SELECTED) //��ť����״̬
	{ 
		dc.BitBlt(0,0,rect.Width(),rect.Height(),&cdc,m_Width*2,0,SRCCOPY);
	}
	else if(!m_IsMoveOn) //��겻�ڰ�ť����
	{
		dc.BitBlt(0,0,rect.Width(),rect.Height(),&cdc,0,0,SRCCOPY);	
	}
	else //����ڰ�ť����
	{
		dc.BitBlt(0,0,rect.Width(),rect.Height(),&cdc,m_Width,0,SRCCOPY);
	}
	dc.Detach();
}


void CPicButton::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
//	m_IsMoveOn = true;
	if (!m_IsMoveOn)
	{
		Invalidate(FALSE);
		TRACKMOUSEEVENT tme;
        tme.cbSize = sizeof(TRACKMOUSEEVENT);
        tme.dwFlags = TME_LEAVE;
        tme.hwndTrack = this->m_hWnd;
		
        if (::_TrackMouseEvent(&tme)) //!!!!!MOUSELEAVE��Ϣ�����ɴ˺�������
        {
			m_IsMoveOn = true;
        }
	}
	CButton::OnMouseMove(nFlags, point);
}


LRESULT CPicButton::OnMouseLeave(WPARAM wParam, LPARAM lParam)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	m_IsMoveOn = false;
	Invalidate(0);
	return LRESULT(0);
}
