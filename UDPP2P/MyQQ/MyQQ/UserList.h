#pragma once
#include "afxwin.h"
#include "SessionDlg.h"
#include <vector>
using namespace std;

struct MyStruct
{
	CString account;
	CString str_head;
	CBitmap bmp;
	CString username;
	CString usersign;
	BOOL IsSelect;
	BOOL Hover;
	int Top;
};

class CUserList :public CStatic
{
public:
	CUserList(CFont *font1,CFont *font2,HWND hwnd);
	~CUserList(void);
	DECLARE_MESSAGE_MAP()
	afx_msg void OnPaint();
public:
	void AddUserItem(CString account,CString _head,CString username,CString usersign);
	void GetSelectInfo(CString &account,CString &head,CString &username,CString &usersign);
	void GetInfoByAccount(CString account,CString &head,CString &username,CString &usersign);
private:
	MyStruct *m_select;
	HWND m_hwnd;
	MyStruct* GetItemForPoint(CPoint p);
	void DrawUserItem(MyStruct *user);
	MyStruct* Cheek(CString account);
	void DeleteUserItem(CString account);
	virtual void PreSubclassWindow();
	vector<MyStruct*> m_manage;
	CFont *m_font1,*m_font2;
	CDC *m_dc,*m_cdc;
	CBitmap *m_bmp,*m_itembg;
	CRect rect;
	CPen m_pen;
	MyStruct* OldHover,*OldSelect;
	BOOL m_IsMoveOn;
public:
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnMouseLeave();
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
};

