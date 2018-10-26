#pragma once
#include "afxwin.h"
#include <vector>
using namespace std;

enum MyEnum
{
	USER,
	OTHER
};

struct Chat
{
	int Top;
	int Height;
	CRect TopLeft;
	CRect TopCentre;
	CRect TopRight;
	CRect CentreLeft;
	CRect CentreCentre;
	CRect CentreRight;
	CRect BottonLeft;
	CRect BottonCentre;
	CRect BottonRight;
	CRect Head;
	CString Msg;
	MyEnum flag;
};

class CChitchat : public CStatic
{
public:
	CChitchat(void);
	~CChitchat(void);
	DECLARE_MESSAGE_MAP()
	afx_msg void OnPaint();
	void Init(CDC *dc);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
public:
	void DrawMsg(Chat *chat);
	void AddUserMsg(CString msg);
	void AddOtherMsg(CString msg);
	void Calculation(CString str,int &w,int &h);
	int GetTextWidth(CString str);
	void Split(CString str);
	vector<CString> m_arry;
	vector<Chat*> m_mange;
	CBitmap m_bmp,m_bg;
	CBitmap m_user,m_other,m_headuser,m_headother;
	CDC *m_cdc,*m_dc;
	CRect rect;
	CPen m_pen;
	CFont m_font;
	int TextHeight;
	void AutoRoll();
};

