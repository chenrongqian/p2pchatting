
// MyQQ.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CMyQQApp:
// �йش����ʵ�֣������ MyQQ.cpp
//

class CMyQQApp : public CWinApp
{
public:
	CMyQQApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CMyQQApp theApp;