
// aQR.h : ������� ���� ��������� ��� ���������� PROJECT_NAME
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�������� stdafx.h �� ��������� ����� ����� � PCH"
#endif

#include "resource.h"		// �������� �������


// CQRApp:
// � ���������� ������� ������ ��. aQR.cpp
//

class CQRApp : public CWinAppEx
{
public:
	CQRApp();

// ���������������
	public:
	virtual BOOL InitInstance();

// ����������

	DECLARE_MESSAGE_MAP()
};

extern CQRApp theApp;