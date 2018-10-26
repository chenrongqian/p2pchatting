#pragma once


// CMyButton

class CPicButton : public CButton
{
	DECLARE_DYNAMIC(CPicButton)

public:
	CPicButton();
	virtual ~CPicButton();
	void LoadImg(UINT ID, int nWidth, int nHeight);
	bool m_IsMoveOn;
protected:
	DECLARE_MESSAGE_MAP()
private :
	CBitmap m_bitmap;
	int m_Width,m_Height;
	
	//CRect rect;

public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg LRESULT OnMouseLeave(WPARAM wParam, LPARAM lParam);
};


