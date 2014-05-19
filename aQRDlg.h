
// aQRDlg.h : файл заголовка
//

#pragma once
#include "afxwin.h"

#include "aQR.h"
#include "QRWnd.h"

// диалоговое окно CQRDlg
class CQRDlg : public CDialog
{
// Создание
public:
	CQRDlg(CWnd* pParent = NULL);	// стандартный конструктор

    int Encode();

// Данные диалогового окна
	enum { IDD = IDD_AQR_DIALOG };

    aQR m_qr;
    bool m_bReady; // признак готовности, для избежания излишней работы

    short m_nEcl;
    short m_nMask;
    CString m_strText;
    CString m_strInfo;
    BOOL m_bShowData;
    BOOL m_bShowEc;
    BOOL m_bShowMask;

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// поддержка DDX/DDV


// Реализация
protected:
	HICON m_hIcon;

    CComboBox m_cbEcl;
    CComboBox m_cbMask;
    CQRWnd m_wndQR;

	// Созданные функции схемы сообщений
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnCbnSelchangeComboEcl();
    afx_msg void OnCbnSelchangeComboMask();
    afx_msg void OnEnChangeEdit();
    afx_msg void OnSize(UINT nType, int cx, int cy);
    afx_msg void OnBnClickedButtonEncode();
    afx_msg void OnBnClickedCheckShowMask();
    afx_msg void OnBnClickedCheckShowEc();
    afx_msg void OnBnClickedCheckShowData();
};
