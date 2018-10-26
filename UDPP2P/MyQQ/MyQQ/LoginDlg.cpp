// LoginDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MyQQ.h"
#include "LoginDlg.h"
#include "afxdialogex.h"


// CLoginDlg �Ի���

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


// CLoginDlg ��Ϣ�������


void CLoginDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	this->UpdateData(TRUE);

	if(Login(m_username.GetBuffer(0),m_password.GetBuffer(0))!=0)
	{
		MessageBoxA("������˺Ų���ȷ");
		return;
	}
	else
		CDialogEx::OnOK();
}


void CLoginDlg::OnBnClickedCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CDialogEx::OnCancel();
}


//void CLoginDlg::OnKillFocus(CWnd* pNewWnd)
//{
//	CDialogEx::OnKillFocus(pNewWnd);
//	if(CW)
//	// TODO: �ڴ˴������Ϣ����������
//}


void CLoginDlg::OnNcLButtonDown(UINT nHitTest, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	CDialogEx::OnNcLButtonDown(nHitTest, point);
}


BOOL CLoginDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	if(InitP2P()!=0)
	{
		MessageBoxA("���ӷ�����ʧ��");
		CDialogEx::OnCancel();
	}
	return TRUE;
}


void CLoginDlg::PreSubclassWindow()
{
	// TODO: �ڴ����ר�ô����/����û���
	
	CDialogEx::PreSubclassWindow();
}
