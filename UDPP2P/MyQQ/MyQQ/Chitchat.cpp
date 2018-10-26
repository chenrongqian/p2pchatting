#include "stdafx.h"
#include "Chitchat.h"
#include "resource.h"

CChitchat::CChitchat(void)
{
	m_cdc = NULL;
}


CChitchat::~CChitchat(void)
{

}


BEGIN_MESSAGE_MAP(CChitchat, CStatic)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()


void CChitchat::OnPaint()
{
	CClientDC dc(this); // device context for painting
	if(m_cdc == NULL) Init(&dc);
	m_dc->SelectObject(&m_bg);
	m_cdc->SelectObject(&m_bmp);
	m_cdc->BitBlt(0,0,rect.Width(),rect.Height(),m_dc,2,72,SRCCOPY);
	int count = m_mange.size();
	for(int i=0; i<count; i++)
	{
		DrawMsg(m_mange[i]);
	}
	dc.BitBlt(0,0,rect.Width(),rect.Height(),m_cdc,0,0,SRCCOPY);
	
}

void CChitchat::AddUserMsg(CString msg)
{
	Chat *chat = new Chat;
	int width = 40,height = 40;
	Calculation(msg,width,height);
	height -= 10;

	int count = m_mange.size();
	if(count > 0)
		chat->Top = 15 + m_mange[count-1]->Top + m_mange[count-1]->Height;
	else
		chat->Top = 5;

	chat->Head = CRect(359,4,391,36);

	chat->TopRight = CRect(345,5,358,16);
	chat->TopCentre = CRect(345-width,5,345,16);
	chat->TopLeft = CRect(345-width-8,0,345-width,16);
	
	chat->CentreRight = CRect(345,16,352,16+height);
	chat->CentreCentre = CRect(345-width,16,345,16+height);
	chat->CentreLeft = CRect(345-width-8,16,345-width,16+height);
	
	chat->BottonRight = CRect(345,16+height,352,16+height+8);
	chat->BottonCentre = CRect(345-width,16+height,345,16+height+8);
	chat->BottonLeft = CRect(345-width-8,16+height,345-width,16+height+8);
	
	chat->flag = MyEnum::USER;
	chat->Msg = msg;
	chat->Height = height + 30;
	m_mange.push_back(chat);

	AutoRoll();
}

void CChitchat::AddOtherMsg(CString msg)
{
	Chat *chat = new Chat;
	int width = 40,height = 40;
	Calculation(msg,width,height);
	height -= 10;
	int count = m_mange.size();
	if(count > 0)
		chat->Top = 15 + m_mange[count-1]->Top + m_mange[count-1]->Height;
	else
		chat->Top = 5;
	chat->Head = CRect(4,4,36,36);
	chat->TopLeft = CRect(37,0,37+13,10+16);
	chat->TopCentre = CRect(37+13,5,37+13+width,16);
	chat->TopRight = CRect(37+13+width,5,37+13+width+7,16);
	chat->CentreLeft = CRect(37+6,16,37+6+7,16+height);
	chat->CentreCentre = CRect(37+6+7,16,37+6+7+width,16+height);
	chat->CentreRight = CRect(37+6+7+width,16,37+6+7+width+7,16+height);
	chat->BottonLeft = CRect(37+6,16+height,37+6+7,16+height+8);
	chat->BottonCentre = CRect(37+6+7,16+height,37+6+7+width,16+height+8);
	chat->BottonRight = CRect(37+6+7+width,16+height,37+6+7+width+7,16+height+8);
	chat->flag = MyEnum::OTHER;
	chat->Msg = msg;
	chat->Height = height + 30;
	m_mange.push_back(chat);
	AutoRoll();
}

void CChitchat::Init(CDC *dc)
{
	// TODO: 在此添加专用代码和/或调用基类
	this->GetClientRect(&rect);
	rect.right = 395;
	rect.bottom = 294;
	m_pen.CreatePen(PS_SOLID,1,RGB(105,105,105));

	m_cdc = new CDC();
	m_dc = new CDC();
	m_cdc->CreateCompatibleDC(dc);
	m_dc->CreateCompatibleDC(dc);
	m_bmp.CreateCompatibleBitmap(dc,rect.Width(),rect.Height());
	m_bg.LoadBitmapA(IDB_BMPBG1);
	m_other.LoadBitmapA(IDB_OTHER);
	m_user.LoadBitmapA(IDB_USER);
	m_headuser.Attach((HBITMAP)LoadImageA(NULL,"res\\Head30\\exam.bmp",IMAGE_BITMAP,30,30,LR_LOADFROMFILE));
	m_headother.Attach((HBITMAP)LoadImageA(NULL,"res\\Head30\\exam.bmp",IMAGE_BITMAP,30,30,LR_LOADFROMFILE));
	m_font.CreatePointFont(100,"微软雅黑");

	m_cdc->SelectObject(&m_font);
	m_cdc->SelectObject(&m_pen);
}

void CChitchat::DrawMsg(Chat *chat)
{
	m_cdc->SelectObject(m_bmp);
	//画头像框
	m_cdc->Rectangle(chat->Head.left,chat->Head.top+chat->Top,chat->Head.right,
		chat->Top+chat->Head.bottom);

	if(chat->flag == MyEnum::OTHER)
		m_dc->SelectObject(&m_headother);
	else
		m_dc->SelectObject(&m_headuser);
	//画头像
	m_cdc->BitBlt(chat->Head.left+1,chat->Head.top+chat->Top+1,30,30,m_dc,0,0,SRCCOPY);

	if(chat->flag == MyEnum::OTHER)
	{
		COLORREF color = RGB(205,215,226);
		m_dc->SelectObject(&m_other);
		//上左
		m_cdc->BitBlt(chat->TopLeft.left,chat->Top+chat->TopLeft.top,
			chat->TopLeft.Width(),chat->TopLeft.Height(),m_dc,0,0,SRCAND);
		//上中
		m_cdc->FillSolidRect(chat->TopCentre.left,
			chat->TopCentre.top+chat->Top,chat->TopCentre.Width(),chat->TopCentre.Height(),color);
		//上右
		m_cdc->BitBlt(chat->TopRight.left,chat->TopRight.top+chat->Top,
			chat->TopRight.Width(),chat->TopRight.Height(),m_dc,28,5,SRCAND);
		//中左
		m_cdc->FillSolidRect(chat->CentreLeft.left,
			chat->CentreLeft.top+chat->Top,chat->CentreLeft.Width(),chat->CentreLeft.Height(),color);
		//中中
		m_cdc->FillSolidRect(chat->CentreCentre.left,
			chat->CentreCentre.top+chat->Top,chat->CentreCentre.Width(),chat->CentreCentre.Height(),color);
		//中右
		m_cdc->FillSolidRect(chat->CentreRight.left,
			chat->CentreRight.top+chat->Top,chat->CentreRight.Width(),chat->CentreRight.Height(),color);
		//下左
		m_cdc->BitBlt(chat->BottonLeft.left,chat->BottonLeft.top+chat->Top,
			chat->BottonLeft.Width(),chat->BottonLeft.Height(),m_dc,6,27,SRCAND);
		//下中
		m_cdc->FillSolidRect(chat->BottonCentre.left,
			chat->BottonCentre.top+chat->Top,chat->BottonCentre.Width(),chat->BottonCentre.Height(),color);
		//下右
		m_cdc->BitBlt(chat->BottonRight.left,chat->BottonRight.top+chat->Top,
			chat->BottonRight.Width(),chat->BottonRight.Height(),m_dc,28,27,SRCAND);
		CRect rect(chat->CentreCentre.left,chat->CentreCentre.top+chat->Top-7,chat->CentreCentre.right,chat->CentreCentre.bottom+chat->Top+5);
		m_cdc->DrawText(chat->Msg,rect,DT_EDITCONTROL|DT_WORDBREAK);
	}
	else
	{
		COLORREF color = RGB(80,205,232);
		m_dc->SelectObject(&m_user);
		//上右
		m_cdc->BitBlt(chat->TopRight.left,chat->TopRight.top+chat->Top,
			chat->TopRight.Width(),chat->TopRight.Height(),m_dc,27,5,SRCAND);
		//上中
		m_cdc->FillSolidRect(chat->TopCentre.left,
			chat->TopCentre.top+chat->Top,chat->TopCentre.Width(),chat->TopCentre.Height(),color);
		//上左
		m_cdc->BitBlt(chat->TopLeft.left,chat->Top+chat->TopLeft.top,
			chat->TopLeft.Width(),chat->TopLeft.Height(),m_dc,5,0,SRCAND);
		
		//中右
		m_cdc->FillSolidRect(chat->CentreRight.left,
			chat->CentreRight.top+chat->Top,chat->CentreRight.Width(),chat->CentreRight.Height(),color);
		//中中
		m_cdc->FillSolidRect(chat->CentreCentre.left,
			chat->CentreCentre.top+chat->Top,chat->CentreCentre.Width(),chat->CentreCentre.Height(),color);
		//中左
		m_cdc->FillSolidRect(chat->CentreLeft.left,
			chat->CentreLeft.top+chat->Top,chat->CentreLeft.Width(),chat->CentreLeft.Height(),color);
		
		//下右
		m_cdc->BitBlt(chat->BottonRight.left,chat->BottonRight.top+chat->Top,
			chat->BottonRight.Width(),chat->BottonRight.Height(),m_dc,26,27,SRCAND);
		//下中
		m_cdc->FillSolidRect(chat->BottonCentre.left,
			chat->BottonCentre.top+chat->Top,chat->BottonCentre.Width(),chat->BottonCentre.Height(),color);
		//下左
		m_cdc->BitBlt(chat->BottonLeft.left,chat->BottonLeft.top+chat->Top,
			chat->BottonLeft.Width(),chat->BottonLeft.Height(),m_dc,5,27,SRCAND);
		
		CRect rect(chat->CentreCentre.left,chat->CentreCentre.top+chat->Top-7,chat->CentreCentre.right,chat->CentreCentre.bottom+chat->Top+5);
		m_cdc->DrawText(chat->Msg,rect,DT_EDITCONTROL|DT_WORDBREAK);
	}
}

void CChitchat::Calculation(CString str,int &w,int &h)
{
	Split(str);
	int count = m_arry.size();
	int width=0,height = 18,temp=0;
	int nub=0;
	if(count == 0) nub = 1;
	for(int i=0; i<count; i++)
	{
		if(temp == 0) nub++;
		temp += GetTextWidth(m_arry[i]);
		if(temp > 280)
		{
			width = 280;
			temp-=280;
			nub++;
		}
		else if(temp > width)
		{
			width = temp;
			temp = 0;
		}
		else
		{
			temp = 0;
		}
	}
	while(temp>280)
	{
		temp-=280;
		nub++;
	}
	w = width;
	h = nub*TextHeight;
}

void CChitchat::Split(CString str)
{
	m_arry.clear();
	int start=0,pos=0;
	CString value;
	while(pos!=-1)
	{
		pos = str.Find("\r\n",pos);
		if(pos == -1) break;
		value = str.Mid(start,pos-start);
		m_arry.push_back(value);
		pos += 4;
		start = pos;
	}
	if(m_arry.size() == 0)
		m_arry.push_back(str);
	else
	{
		value = str.Right(str.GetLength()-start+2);
		m_arry.push_back(value);
	}
}

BOOL CChitchat::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	return TRUE;
	return CStatic::OnEraseBkgnd(pDC);
}

int CChitchat::GetTextWidth(CString str)
{
	CSize size = m_cdc->GetTextExtent(str);
	int cx = size.cx;
	size = m_cdc->GetTextExtent("我");
	TextHeight = size.cy;
	return cx;
}

void CChitchat::AutoRoll()
{
	int count = m_mange.size();
	if(count < 1) 
	{
		this->OnPaint();
		return;
	}
	Chat *item = m_mange[count-1];
	if(item->Top+item->Height <= 294)
	{
		this->OnPaint();
		return;
	}
	int offset = item->Top+item->Height - 294;
	for(int i=0; i<count; i++)
	{
		m_mange[i]->Top -= offset;
	}
	this->OnPaint();
}
