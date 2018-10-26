#include "stdafx.h"
#include "UserList.h"
#include "resource.h"


CUserList::CUserList(CFont *font1,CFont *font2,HWND hwnd)
{
	m_font1 = font1;
	m_font2 = font2;
	m_IsMoveOn = FALSE;//移动
	m_hwnd = hwnd;
}


CUserList::~CUserList(void)
{
}
BEGIN_MESSAGE_MAP(CUserList, CStatic)
	ON_WM_PAINT()
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSELEAVE()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDBLCLK()
END_MESSAGE_MAP()


void CUserList::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	m_cdc->SelectObject(m_bmp);
	m_cdc->PatBlt(0,0,rect.Width(),rect.Height(),WHITENESS);
	int count = m_manage.size();
	for(int i=0; i<count; i++)
	{
		if((m_manage[i]->Top>0 && m_manage[i]->Top<rect.Width()) ||
			(m_manage[i]->Top+50>0 && m_manage[i]->Top+50<rect.Width()) )
		{
			m_dc->SelectObject(&(m_manage[i]->bmp));
			if(m_manage[i]->IsSelect == TRUE)
				m_cdc->BitBlt(0,m_manage[i]->Top,240,50,m_dc,480,0,SRCCOPY);
			else if(m_manage[i]->Hover == TRUE)
				m_cdc->BitBlt(0,m_manage[i]->Top,240,50,m_dc,240,0,SRCCOPY);
			else
				m_cdc->BitBlt(0,m_manage[i]->Top,240,50,m_dc,0,0,SRCCOPY);
		}
	}
	dc.BitBlt(0,0,rect.Width(),rect.Height(),m_cdc,0,0,SRCCOPY);
}

void CUserList::AddUserItem(CString account,CString _head,CString username,CString usersign)
{
	MyStruct *user = Cheek(account);
	if(user != NULL)
	{
		if(_head == "")
		{
			DeleteUserItem(account);
			Invalidate();
			return;
		}
		user->account = account;
		user->str_head = _head;
		user->username = username;
		user->usersign = usersign;
		DrawUserItem(user);
		Invalidate();
		return;
	}
	user = new MyStruct;
	user->account = account;
	user->str_head = _head;
	user->username = username;
	user->usersign = usersign;
	user->IsSelect = FALSE;
	user->Hover = FALSE;
	DrawUserItem(user);
	user->Top = m_manage.size()*50;
	m_manage.push_back(user);
	Invalidate();
}

MyStruct* CUserList::Cheek(CString account)
{
	int count = m_manage.size();
	for(int i=0; i<count; i++)
	{
		if(m_manage[i]->account == account)
			return m_manage[i];
	}
	return NULL;
}

void CUserList::DeleteUserItem(CString account)
{
	vector<MyStruct*> ::iterator it;
	for(it=m_manage.begin(); it!=m_manage.end(); it++)
	{
		if((*it)->account == account)
		{
			delete (*it);
			m_manage.erase(it);
			return;
		}
	}
}

void CUserList::DrawUserItem(MyStruct *user)
{
	CDC *dc = this->GetDC();
	if(user->bmp.m_hObject!=NULL)
		user->bmp.DeleteObject();
	user->bmp.CreateCompatibleBitmap(dc,720,50);
	CBitmap bmp;
	CString str;
	str.Format("res\\Head40\\%s.bmp",user->str_head);
	bmp.Attach((HBITMAP)LoadImageA(NULL,str,IMAGE_BITMAP,40,40,LR_LOADFROMFILE));
	m_dc->SelectObject(m_itembg);
	m_cdc->SelectObject(user->bmp);

	m_cdc->BitBlt(0,0,720,50,m_dc,0,0,SRCCOPY);
	m_dc->SelectObject(bmp);
	CFont *font = m_dc->SelectObject(m_font1);
	for(int i=0; i<3; i++)
	{
		m_cdc->Rectangle((i*240)+9,4,(i*240)+11+40,46);
		m_cdc->BitBlt((i*240)+10,5,40,40,m_dc,0,0,SRCCOPY);
		m_cdc->SelectObject(m_font1);
		m_cdc->DrawText(user->username,CRect((i*240)+60,10,(i*240)+60+100,10+18),DT_LEFT|DT_SINGLELINE);
		m_cdc->SelectObject(m_font2);
		m_cdc->DrawText(user->usersign,CRect((i*240)+63,30,(i*240)+63+180,30+18),DT_LEFT|DT_SINGLELINE);
	}
	m_cdc->SelectObject(font);

	this->ReleaseDC(dc);
}

void CUserList::PreSubclassWindow()
{
	// TODO: 在此添加专用代码和/或调用基类
	this->GetClientRect(&rect);
	rect.right = 250;
	rect.bottom = 462;
	CDC *dc = this->GetDC();
	m_pen.CreatePen(PS_SOLID,1,RGB(105,105,105));
	m_dc = new CDC();
	m_dc->CreateCompatibleDC(dc);
	m_cdc = new CDC();
	m_cdc->CreateCompatibleDC(dc);
	m_cdc->SetBkMode(TRANSPARENT);
	m_cdc->SelectObject(&m_pen);
	m_itembg = new CBitmap();
	m_itembg->LoadBitmapA(IDB_USERITEM);
	
	m_bmp = new CBitmap();
	m_bmp->CreateCompatibleBitmap(dc,rect.Width(),rect.Height());

	OldSelect = NULL;
	OldHover = NULL;
	this->ReleaseDC(dc);
	CStatic::PreSubclassWindow();
}

void CUserList::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (!m_IsMoveOn)
	{
		Invalidate(FALSE);
		TRACKMOUSEEVENT tme;
        tme.cbSize = sizeof(TRACKMOUSEEVENT);
        tme.dwFlags = TME_LEAVE;
        tme.hwndTrack = this->m_hWnd;
		
        if (::_TrackMouseEvent(&tme)) //!!!!!MOUSELEAVE消息必须由此函数触发
        {
			m_IsMoveOn = true;
        }
	}
	MyStruct *item = GetItemForPoint(point);
	if(item == NULL)
	{
		if(OldHover != NULL)
		{
			OldHover->Hover = FALSE;
			OldHover =  NULL;
			Invalidate();
		}
	}
	else
	{
		if(OldHover == item) return;
		item->Hover = TRUE;
		if(OldHover != NULL)
			OldHover->Hover = FALSE;
		OldHover =  item;
		Invalidate();
	}
	CStatic::OnMouseMove(nFlags, point);
}

void CUserList::OnMouseLeave()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	m_IsMoveOn = FALSE;
	if(OldHover != NULL)
	{
		OldHover->Hover = FALSE;
		OldHover =  NULL;
		Invalidate();
	}
	CStatic::OnMouseLeave();
}

void CUserList::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	MyStruct *item = GetItemForPoint(point);
	if(item == NULL) return;
	if(OldSelect != NULL)
		OldSelect->IsSelect = FALSE;
	item->IsSelect = TRUE;
	OldSelect = item;
	Invalidate();
	CStatic::OnLButtonUp(nFlags, point);
}

MyStruct* CUserList::GetItemForPoint(CPoint p)
{
	int i,count = m_manage.size();
	for(i=0; i<count; i++)
	{
		CRect rect(0,m_manage[i]->Top,250,m_manage[i]->Top+50);
		if(rect.PtInRect(p))
		{
			return m_manage[i];
		}
	}
	return NULL;
}

void CUserList::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	/*CSessionDlg *dlg = new CSessionDlg();
	dlg->Create(IDD_DIALOG2,this);
	dlg->ShowWindow(SW_SHOW);*/
	m_select = GetItemForPoint(point);
	::PostMessageA(m_hwnd,WM_USER+202,0,0);
	CStatic::OnLButtonDblClk(nFlags, point);
}

void CUserList::GetSelectInfo(CString &account,CString &head,CString &username,CString &usersign)
{
	account = m_select->account;
	head = m_select->str_head;
	username = m_select->username;
	usersign = m_select->usersign;
}

void CUserList::GetInfoByAccount(CString account,CString &head,CString &username,CString &usersign)
{
	int count = m_manage.size();
	for(int i=0; i<count; i++)
	{
		if(m_manage[i]->account == account)
		{
			head = m_manage[i]->str_head;
			username = m_manage[i]->username;
			usersign = m_manage[i]->usersign;//用户签名
			return;
		}
	}
}

