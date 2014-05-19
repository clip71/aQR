
// aQR.cpp : Определяет поведение классов для приложения.
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


// создание CQRApp

CQRApp::CQRApp()
{
	// TODO: добавьте код создания,
	// Размещает весь важный код инициализации в InitInstance
}


// Единственный объект CQRApp

CQRApp theApp;


// инициализация CQRApp

BOOL CQRApp::InitInstance()
{
	CWinAppEx::InitInstance();

	// Стандартная инициализация
	SetRegistryKey(_T("Introtest"));

	CQRDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: Введите код для обработки закрытия диалогового окна
		//  с помощью кнопки "ОК"
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: Введите код для обработки закрытия диалогового окна
		//  с помощью кнопки "Отмена"
	}

	// Поскольку диалоговое окно закрыто, возвратите значение FALSE, чтобы можно было выйти из
	//  приложения вместо запуска генератора сообщений приложения.
	return FALSE;
}
