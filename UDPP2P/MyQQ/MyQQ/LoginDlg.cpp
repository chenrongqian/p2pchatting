// LoginDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "MyQQ.h"
#include "LoginDlg.h"
#include "afxdialogex.h"


// CLoginDlg 对话框

IMPLEMENT_DYNAMIC(CLoginDlg, CDialogEx)

CLoginDlg::CLoginDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CLoginDlg::IDD, pParent)
	, m_username(_T(""))
	, m_password(_T(""))
{

}

CLoginDlg::~CLoginDlg()
{
}

void CLoginDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_username);
	DDX_Text(pDX, IDC_EDIT2, m_password);
}


BEGIN_MESSAGE_MAP(CLoginDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CLoginDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CLoginDlg::OnBnClickedCancel)
//	ON_WM_KILLFOCUS()
ON_WM_NCLBUTTONDOWN()
END_MESSAGE_MAP()


// CLoginDlg 消息处理程序


void CLoginDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	this->UpdateData(TRUE);

	if(Login(m_username.GetBuffer(0),m_password.GetBuffer(0))!=0)
	{
		MessageBoxA("密码或账号不正确");
		return;
	}
	else
		CDialogEx::OnOK();
}


void CLoginDlg::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnCancel();
}


//void CLoginDlg::OnKillFocus(CWnd* pNewWnd)
//{
//	CDialogEx::OnKillFocus(pNewWnd);
//	if(CW)
//	// TODO: 在此处添加消息处理程序代码
//}


void CLoginDlg::OnNcLButtonDown(UINT nHitTest, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	CDialogEx::OnNcLButtonDown(nHitTest, point);
}


BOOL CLoginDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	if(InitP2P()!=0)
	{
		MessageBoxA("连接服务器失败");
		CDialogEx::OnCancel();
	}
	return TRUE;
}


void CLoginDlg::PreSubclassWindow()
{
	// TODO: 在此添加专用代码和/或调用基类
	
	CDialogEx::PreSubclassWindow();
}
