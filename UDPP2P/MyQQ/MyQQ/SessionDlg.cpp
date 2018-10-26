// SessionDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "MyQQ.h"
#include "SessionDlg.h"
#include "afxdialogex.h"


// CSessionDlg 对话框

IMPLEMENT_DYNAMIC(CSessionDlg, CDialogEx)

CSessionDlg::CSessionDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSessionDlg::IDD, pParent)
	, m_name(_T(""))
	, m_sign(_T(""))
{
	ISCREATE = FALSE;
	ISRECV = FALSE;
}

CSessionDlg::~CSessionDlg()
{

}

void CSessionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BTMIN, m_btmin);
	DDX_Control(pDX, IDC_BTCLOSE, m_btclose);

	DDX_Text(pDX, IDC_USERNAME, m_name);
	DDX_Text(pDX, IDC_USERSIGN, m_sign);
	DDX_Control(pDX, IDC_PICHEAD, m_pichead);
}


BEGIN_MESSAGE_MAP(CSessionDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_NCHITTEST()
	ON_BN_CLICKED(IDC_BTMIN, &CSessionDlg::OnBnClickedBtmin)
	ON_BN_CLICKED(IDC_BTCLOSE, &CSessionDlg::OnBnClickedBtclose)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BUTTON2, &CSessionDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON1, &CSessionDlg::OnBnClickedButton1)
END_MESSAGE_MAP()


// CSessionDlg 消息处理程序

void CSessionDlg::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类

	//CDialogEx::OnOK();
}

void CSessionDlg::OnCancel()
{
	// TODO: 在此添加专用代码和/或调用基类

	//CDialogEx::OnCancel();
}

BOOL CSessionDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	m_font1.CreatePointFont(100,"微软雅黑");
	m_font2.CreatePointFont(80,"微软雅黑");
	int w = GetSystemMetrics(SM_CXSCREEN);
	int h = GetSystemMetrics(SM_CYSCREEN);
	this->MoveWindow((w-550)/2,(h-510)/2,550,510);
	m_btmin.LoadImg(IDB_BTMIN1,30,27);
	m_btmin.MoveWindow(490,0,30,27);
	m_btclose.LoadImg(IDB_BTCLOSE1,30,27);
	m_btclose.MoveWindow(520,0,30,27);
	m_cdc = NULL;
	m_bmp.LoadBitmapA(IDB_BMPBG1);
	m_pichead.MoveWindow(15,15,42,42);
	m_edit = new CMyEdit();
	m_edit->CreateEdit(CRect(5,375,395,485),this,WM_USER+3);
	m_chitchat.Create("",SS_OWNERDRAW|WS_CHILD|WS_VISIBLE|SS_NOTIFY,CRect(2,72,397,366),this,WM_USER+4);
	ISCREATE = TRUE;
	if(ISRECV == TRUE) m_chitchat.AddOtherMsg(strmsg);
	//m_edit->MoveWindow(5,375,390,130);
	return TRUE;  // return TRUE unless you set the focus to a control

}

void CSessionDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	if(m_cdc == NULL)
	{
		m_cdc = new CDC();
		m_cdc->CreateCompatibleDC(&dc);
		m_cdc->SelectObject(&m_bmp);
		//SetBrushOrgEx(cdc->m_hDC,1,1,NULL);
	}
	dc.BitBlt(0,0,550,510,m_cdc,0,0,SRCCOPY);
	if(m_bmpother.m_hObject != NULL)
	{
		CDC *dc = m_pichead.GetDC();
		CDC cdc;
		cdc.CreateCompatibleDC(dc);
		cdc.SelectObject(m_bmpother);
		dc->BitBlt(1,1,40,40,&cdc,0,0,SRCCOPY);
		m_pichead.ReleaseDC(dc);
	}
}

LRESULT CSessionDlg::OnNcHitTest(CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	return HTCAPTION;
	return CDialogEx::OnNcHitTest(point);
}

void CSessionDlg::OnBnClickedBtmin()
{
	// TODO: 在此添加控件通知处理程序代码
	this->ShowWindow(SW_SHOWMINIMIZED);
}

void CSessionDlg::OnBnClickedBtclose()
{
	// TODO: 在此添加控件通知处理程序代码
	::PostMessageA(NULL,WM_USER+206,0,(LPARAM)this);
	CDialogEx::OnCancel();
}


HBRUSH CSessionDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);
    if(pWnd->GetDlgCtrlID() == IDC_USERNAME)
	{
		pDC->SetBkMode(TRANSPARENT);
		hbr = (HBRUSH)::GetStockObject(NULL_BRUSH);
		pDC->SelectObject(&m_font1);
	}
    else if(pWnd->GetDlgCtrlID() == IDC_USERSIGN)
	{
		pDC->SetBkMode(TRANSPARENT);
		hbr = (HBRUSH)::GetStockObject(NULL_BRUSH);
		pDC->SelectObject(&m_font2);
	}
	return hbr;
}

void CSessionDlg::LoadHead()
{
	CString str;
	str.Format("res\\Head40\\%s.bmp",m_struser);
	m_bmpuser.Attach((HBITMAP)LoadImageA(NULL,str,IMAGE_BITMAP,40,40,LR_LOADFROMFILE));
	str.Format("res\\Head40\\%s.bmp",m_strother);
	m_bmpother.Attach((HBITMAP)LoadImageA(NULL,str,IMAGE_BITMAP,40,40,LR_LOADFROMFILE));
}

void CSessionDlg::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	CString str;
	m_edit->GetWindowTextA(str);
	m_edit->SetWindowTextA("");
	SendMsg(m_account.GetBuffer(0),str.GetBuffer(0));
	m_chitchat.AddUserMsg(str);
	InvalidateRect(CRect(5,375,395,485),FALSE);
}


void CSessionDlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	::PostMessageA(NULL,WM_USER+206,0,(LPARAM)this);
	CDialogEx::OnCancel();
	//m_chitchat.AddOtherMsg("123456");
}

void CSessionDlg::SetInfo(CString account,CString ohead,CString uhead,CString name,CString sign)
{
	m_account = account;
	m_struser = uhead;
	m_strother = ohead;
	m_name = name;
	m_sign = sign;
	this->UpdateData(FALSE);
	LoadHead();
}

BOOL CSessionDlg::Cheek(CString account)
{
	if(account == m_account) return TRUE;
	return FALSE;
}

void CSessionDlg::AddRecvMsg(CString msg)
{
	if(ISCREATE == FALSE) 
	{
		ISRECV = TRUE;
		strmsg = msg;
	}
	else
		m_chitchat.AddOtherMsg(msg);
}