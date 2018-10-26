
// MyQQDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "MyQQ.h"
#include "MyQQDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define WIDTH   250
#define HEIGHT  600

// CMyQQDlg 对话框
CString strhead,strname,strsign,account,strmsg;
HWND m_Hwnd;

CMyQQDlg::CMyQQDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMyQQDlg::IDD, pParent)
	, m_username(_T("飘渺逸云"))
	, m_usersign(_T("此人很懒，什么都没留下......"))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMyQQDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BTMIN, m_btmin);
	DDX_Control(pDX, IDC_BTCLOSE, m_btclose);
	DDX_Control(pDX, IDC_PICHEAD, m_pichead);
	DDX_Text(pDX, IDC_USERNAME, m_username);
	DDX_Text(pDX, IDC_USERSIGN, m_usersign);
}

BEGIN_MESSAGE_MAP(CMyQQDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_NCHITTEST()
	ON_BN_CLICKED(IDC_BTMIN, &CMyQQDlg::OnBnClickedBtmin)
	ON_BN_CLICKED(IDC_BTCLOSE, &CMyQQDlg::OnBnClickedBtclose)
	ON_WM_CTLCOLOR()
	ON_STN_CLICKED(IDC_USERNAME, &CMyQQDlg::OnStnClickedUsername)
	ON_STN_CLICKED(IDC_USERSIGN, &CMyQQDlg::OnStnClickedUsersign)
	ON_WM_NCLBUTTONDOWN()
	ON_WM_ERASEBKGND()
	ON_MESSAGE(WM_RECVMSG, OnRecvMsg) 
	ON_MESSAGE(WM_USERINFO, OnUserInfo) 
	ON_MESSAGE(WM_USERCLICK, OnUserClick)
	ON_MESSAGE(WM_USERCLOSE, OnUserClose)
END_MESSAGE_MAP()


// CMyQQDlg 消息处理程序

BOOL CMyQQDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	m_font1.CreatePointFont(100,"微软雅黑");
	m_font2.CreatePointFont(80,"微软雅黑");
	this->MoveWindow(0,0,WIDTH,HEIGHT);
	m_edit = NULL;
	m_cdc = NULL;
	m_bmp.LoadBitmapA(IDB_BMPBG);
	m_btmin.LoadImg(IDB_BTMIN,30,27);
	m_btmin.MoveWindow(WIDTH-60,0,30,27);
	m_btclose.LoadImg(IDB_BTCLOSE,30,27);
	m_btclose.MoveWindow(WIDTH-30,0,30,27);
	m_pichead.MoveWindow(10,50,62,62);
	m_list = new CUserList(&m_font1,&m_font2,this->m_hWnd);
	m_list->Create("",WS_CHILD|WS_VISIBLE|SS_NOTIFY,CRect(0,138,250,600),this,WM_USER+2);
	/*m_list->AddUserItem("624060229","exam","123","123456789");
	m_list->AddUserItem("624060229","exam","123","123456789");
	m_list->AddUserItem("624060229","exam","123","123456789");
	m_list->AddUserItem("624060229","exam","123","123456789");*/
	RegisterCallBack((CallRecvMsg)RecvMsgCallBack,(CallUserChange)UserInfoCallBack);
	GetUserCount();
	GetUserInfo(m_strhead.GetBufferSetLength(20),m_username.GetBufferSetLength(50),m_usersign.GetBufferSetLength(100));
	m_Hwnd = this->m_hWnd;
	SetPicHead();
	this->UpdateData(FALSE);
	//m_list->MoveWindow(CRect(0,138,250,600));
//	m_list->ShowWindow(SW_SHOW);
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CMyQQDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CPaintDC dc(this);
		if(m_cdc == NULL)
		{
			m_cdc = new CDC();
			m_cdc->CreateCompatibleDC(&dc);
			m_cdc->SelectObject(&m_bmp);
		}
		dc.BitBlt(0,0,WIDTH,HEIGHT,m_cdc,0,0,SRCCOPY);
		if(m_bmphead.m_hObject != NULL)
		{
			CDC *dc = m_pichead.GetDC();
			CDC cdc;
			cdc.CreateCompatibleDC(dc);
			cdc.SelectObject(m_bmphead);
			dc->BitBlt(1,1,60,60,&cdc,0,0,SRCCOPY);
		}
		//CDialogEx::OnPaint();
	}
}

HCURSOR CMyQQDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

LRESULT CMyQQDlg::OnNcHitTest(CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	return HTCAPTION;
	//return CDialogEx::OnNcHitTest(point);
}

void CMyQQDlg::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类

	//CDialogEx::OnOK();
}

void CMyQQDlg::OnCancel()
{
	// TODO: 在此添加专用代码和/或调用基类

	//CDialogEx::OnCancel();
}

void CMyQQDlg::OnBnClickedBtmin()
{
	// TODO: 在此添加控件通知处理程序代码
	this->ShowWindow(SW_SHOWMINIMIZED);
}

void CMyQQDlg::OnBnClickedBtclose()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnCancel();
}

HBRUSH CMyQQDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);
	if(nCtlColor == CTLCOLOR_STATIC)
	{
		pDC->SetBkMode(TRANSPARENT);
		hbr = (HBRUSH)::GetStockObject(NULL_BRUSH);
	}
	if(pWnd->GetDlgCtrlID() == IDC_TITLE || pWnd->GetDlgCtrlID() == IDC_USERNAME)
	{
		pDC->SelectObject(&m_font1);
	}
	else if(pWnd->GetDlgCtrlID() == IDC_USERSIGN)
	{
		pDC->SelectObject(&m_font2);
	}
	return hbr;
}

void CMyQQDlg::OnStnClickedUsername()
{
	// TODO: 在此添加控件通知处理程序代码
	if(m_edit != NULL)
	{
		CString str;
		m_edit->GetWindowTextA(str);
		m_static->ShowWindow(SW_SHOW);
		m_static->SetWindowTextA(str);
		this->UpdateData(TRUE);
		ChangeInfo(m_strhead.GetBuffer(0),m_username.GetBuffer(0),m_usersign.GetBuffer(0));
		delete m_edit;
		m_edit = NULL;
	}
	CString str;
	m_static = (CStatic *)GetDlgItem(IDC_USERNAME);
	m_static->GetWindowTextA(str);
	m_edit = new CEdit();
	CRect rect(86,54,86+150,54+20);
	m_edit->Create(WS_EX_NOPARENTNOTIFY,rect,this,WM_USER+1);
	m_edit->SetFont(&m_font1);
	//m_edit->MoveWindow(87,54,150,20);
	m_edit->SetWindowTextA(str);
	m_static->ShowWindow(SW_HIDE);
	m_edit->ShowWindow(SW_SHOW);
}

void CMyQQDlg::OnStnClickedUsersign()
{
	if(m_edit != NULL)
	{
		CString str;
		m_edit->GetWindowTextA(str);
		m_static->ShowWindow(SW_SHOW);
		m_static->SetWindowTextA(str);
		this->UpdateData(TRUE);
	    ChangeInfo(m_strhead.GetBuffer(0),m_username.GetBuffer(0),m_usersign.GetBuffer(0));
		delete m_edit;
		m_edit = NULL;
	}
	CString str;
	m_static = (CStatic*)GetDlgItem(IDC_USERSIGN);
	m_static->GetWindowTextA(str);
	m_edit = new CEdit();
	CRect rect(96,79,96+150,79+18);
	m_edit->Create(ES_AUTOHSCROLL,rect,this,WM_USER+1);
	m_edit->SetFont(&m_font2);
	m_edit->SetWindowTextA(str);
	m_static->ShowWindow(SW_HIDE);
	m_edit->ShowWindow(SW_SHOW);
	// TODO: 在此添加控件通知处理程序代码
}

void CMyQQDlg::OnNcLButtonDown(UINT nHitTest, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if(m_edit != NULL)
	{
		CRect rect;
		m_edit->GetClientRect(&rect);
		CPoint p = point;
		m_edit->ScreenToClient(&p);
		if(!rect.PtInRect(p))
		{
			CString str;
			m_edit->GetWindowTextA(str);
			m_static->ShowWindow(SW_SHOW);
			m_static->SetWindowTextA(str);
			this->UpdateData(TRUE);
			ChangeInfo(m_strhead.GetBuffer(0),m_username.GetBuffer(0),m_usersign.GetBuffer(0));
			delete m_edit;
			m_edit = NULL;
		}
	}
	CDialogEx::OnNcLButtonDown(nHitTest, point);
}

BOOL CMyQQDlg::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	return TRUE;
	return CDialogEx::OnEraseBkgnd(pDC);
}

void CMyQQDlg::SetPicHead()
{
	CString str;
	str.Format("res\\Head60\\%s.bmp",m_strhead);
	m_bmphead.Attach((HBITMAP)LoadImageA(NULL,str,IMAGE_BITMAP,60,60,LR_LOADFROMFILE));

}

void CMyQQDlg::RecvMsgCallBack(char* avg1,char*avg2)
{
	account.Format("%s",avg1);
	strmsg.Format("%s",avg2);
	::PostMessage(m_Hwnd, WM_RECVMSG, 0, 0);
}

void CMyQQDlg::UserInfoCallBack(char* avg1,char *avg2,char *avg3,char *avg4)
{
	account.Format("%s",avg1);
	strhead.Format("%s",avg2);
	strname.Format("%s",avg3);
	strsign.Format("%s",avg4);
	::PostMessage(m_Hwnd, WM_USERINFO, 0, 0);
}

LRESULT CMyQQDlg::OnRecvMsg(WPARAM wParam, LPARAM lParam)
{
	int i,count;
	count = m_manage.size();
	for(i=0; i<count; i++)
	{
		if(m_manage[i]->Cheek(account))
		{
			m_manage[i]->ShowWindow(SW_SHOW);
			m_manage[i]->SetFocus();
			m_manage[i]->AddRecvMsg(strmsg);
			return 0;
		}
	}
	m_list->GetInfoByAccount(account,strhead,strname,strsign);
	CSessionDlg *dlg = new CSessionDlg();
	dlg->Create(IDD_DIALOG2,this);
	dlg->SetInfo(account,strhead,m_strhead,strname,strsign);
	dlg->OnInitDialog();
	dlg->ShowWindow(SW_SHOW);
	dlg->AddRecvMsg(strmsg);
	m_manage.push_back(dlg);
	return 0;
}

LRESULT CMyQQDlg::OnUserInfo(WPARAM wParam, LPARAM lParam)
{
	m_list->AddUserItem(account,strhead,strname,strsign);
	return 0;
}

LRESULT CMyQQDlg::OnUserClick(WPARAM wParam, LPARAM lParam)
{
	int i,count;
	m_list->GetSelectInfo(account,strhead,strname,strsign);
	count = m_manage.size();
	for(i=0; i<count; i++)
	{
		if(m_manage[i]->Cheek(account))
		{
			m_manage[i]->ShowWindow(SW_SHOW);
			m_manage[i]->SetFocus();
			return 0;
		}
	}
	CSessionDlg *dlg = new CSessionDlg();
	dlg->Create(IDD_DIALOG2,this);
	dlg->SetInfo(account,strhead,m_strhead,strname,strsign);
	dlg->ShowWindow(SW_SHOW);
	m_manage.push_back(dlg);
	return 0;
}

LRESULT CMyQQDlg::OnUserClose(WPARAM wParam, LPARAM lParam)
{
	vector<CSessionDlg *> ::iterator it;
	it = m_manage.begin();
	for(it; it!=m_manage.end(); it++)
	{
		if((*it) == (CSessionDlg*)lParam)
		{
			m_manage.erase(it);
			break;
		}
	}
	return 0;
}