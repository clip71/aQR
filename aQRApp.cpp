
// aQR.cpp : ���������� ��������� ������� ��� ����������.
//

#include "stdafx.h"
#include "aQRApp.h"
#include "aQRDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CQRApp

BEGIN_MESSAGE_MAP(CQRApp, CWinAppEx)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// �������� CQRApp

CQRApp::CQRApp()
{
	// TODO: �������� ��� ��������,
	// ��������� ���� ������ ��� ������������� � InitInstance
}


// ������������ ������ CQRApp

CQRApp theApp;


// ������������� CQRApp

BOOL CQRApp::InitInstance()
{
	CWinAppEx::InitInstance();

	// ����������� �������������
	SetRegistryKey(_T("Introtest"));

	CQRDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: ������� ��� ��� ��������� �������� ����������� ����
		//  � ������� ������ "��"
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: ������� ��� ��� ��������� �������� ����������� ����
		//  � ������� ������ "������"
	}

	// ��������� ���������� ���� �������, ���������� �������� FALSE, ����� ����� ���� ����� ��
	//  ���������� ������ ������� ���������� ��������� ����������.
	return FALSE;
}
