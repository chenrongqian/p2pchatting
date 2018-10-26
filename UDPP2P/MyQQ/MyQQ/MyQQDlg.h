
// MyQQDlg.h : 头文件
//

#pragma once
#include "PicButton.h"
#include "afxwin.h"
#include "UserList.h"
#include <vector>
using namespace std;

#define WM_RECVMSG WM_USER + 200
#define WM_USERINFO WM_USER + 201
#define WM_USERCLICK WM_USER + 202
#define WM_USERCLOSE WM_USER + 206
// CMyQQDlg 对话框
class CMyQQDlg : public CDialogEx
{
// 构造
public:
	CMyQQDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_MYQQ_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持

private:
	CUserList *m_list;
	CEdit *m_edit;
	CDC *m_cdc;
	CFont m_font1,m_font2;
	CBitmap m_bmp;
	CStatic *m_static;
// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	static void CALLBACK RecvMsgCallBack(char* avg1,char*avg2);
	static void CALLBACK UserInfoCallBack(char* avg1,char *avg2,char *avg3,char *avg4);
	afx_msg LRESULT OnRecvMsg(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnUserClick(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnUserInfo(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnUserClose(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnNcHitTest(CPoint point);
	CPicButton m_btmin;
	CPicButton m_btclose;
	void SetPicHead();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnBnClickedBtmin();
	afx_msg void OnBnClickedBtclose();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	CStatic m_pichead;
	CString m_strhead;
	CBitmap m_bmphead;
	CString m_username;
	CString m_usersign;
	vector<CSessionDlg*> m_manage;
	afx_msg void OnStnClickedUsername();
	afx_msg void OnStnClickedUsersign();
	afx_msg void OnNcLButtonDown(UINT nHitTest, CPoint point);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};
