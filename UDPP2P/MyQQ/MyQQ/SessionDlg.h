#pragma once
#include "resource.h"
#include "PicButton.h"
#include "afxwin.h"
#include "MyEdit.h"
#include "Chitchat.h"
// CSessionDlg 对话框

class CSessionDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CSessionDlg)

public:
	CSessionDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CSessionDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG2 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
	virtual void OnOK();
	virtual void OnCancel();
public:
	void SetInfo(CString account,CString ohead,CString uhead,CString name,CString sign);
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg LRESULT OnNcHitTest(CPoint point);
	afx_msg void OnBnClickedBtmin();
	afx_msg void OnBnClickedBtclose();
	CDC *m_cdc;
	CString m_struser;
	CBitmap m_bmpuser;
	CString m_strother;
	CBitmap m_bmpother;
	CBitmap m_bmp;
	CPicButton m_btmin;
	CPicButton m_btclose;
	CString m_account;
private:
	void LoadHead();
	CFont m_font1,m_font2;
	CChitchat m_chitchat;
public:
	void AddRecvMsg(CString msg);
	BOOL Cheek(CString account);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	CString m_name;
	CString m_sign;
	CStatic m_pichead;
	CMyEdit *m_edit;
	BOOL ISRECV;
	BOOL ISCREATE;
	CString strmsg;
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton1();
};
