
// aQRDlg.cpp : файл реализации
//

#include "stdafx.h"
#include "aQRApp.h"
#include "aQRDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// Диалоговое окно CAboutDlg используется для описания сведений о приложении

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Данные диалогового окна
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // поддержка DDX/DDV

// Реализация
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// диалоговое окно CQRDlg




CQRDlg::CQRDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CQRDlg::IDD, pParent)
    , m_strInfo(_T(""))
    , m_bShowData(TRUE)
    , m_bShowEc(TRUE)
    , m_bShowMask(TRUE)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
    m_strText = AfxGetApp()->GetProfileString("", "Text", "Clip");

    m_nEcl = (short)AfxGetApp()->GetProfileInt("", "Ecl", 0);
    m_nMask = (short)AfxGetApp()->GetProfileInt("", "Mask", 1);

    m_bReady = false;
}

void CQRDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_COMBO_ECL, m_cbEcl);
    DDX_Control(pDX, IDC_COMBO_MASK, m_cbMask);
    DDX_Text(pDX, IDC_EDIT, m_strText);
    DDX_Text(pDX, IDC_EDIT_INFO, m_strInfo);
    DDX_Check(pDX, IDC_CHECK_SHOW_DATA, m_bShowData);
    DDX_Check(pDX, IDC_CHECK_SHOW_EC, m_bShowEc);
    DDX_Check(pDX, IDC_CHECK_SHOW_MASK, m_bShowMask);
}

BEGIN_MESSAGE_MAP(CQRDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
    ON_WM_SIZE()

    ON_CBN_SELCHANGE(IDC_COMBO_ECL, &CQRDlg::OnCbnSelchangeComboEcl)
    ON_CBN_SELCHANGE(IDC_COMBO_MASK, &CQRDlg::OnCbnSelchangeComboMask)
    ON_EN_CHANGE(IDC_EDIT, &CQRDlg::OnEnChangeEdit)
    ON_BN_CLICKED(IDC_BUTTON_ENCODE, &CQRDlg::OnBnClickedButtonEncode)
    ON_BN_CLICKED(IDC_CHECK_SHOW_MASK, &CQRDlg::OnBnClickedCheckShowMask)
    ON_BN_CLICKED(IDC_CHECK_SHOW_EC, &CQRDlg::OnBnClickedCheckShowEc)
    ON_BN_CLICKED(IDC_CHECK_SHOW_DATA, &CQRDlg::OnBnClickedCheckShowData)
END_MESSAGE_MAP()


// обработчики сообщений CQRDlg

BOOL CQRDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Добавление пункта "О программе..." в системное меню.

	// IDM_ABOUTBOX должен быть в пределах системной команды.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Задает значок для этого диалогового окна. Среда делает это автоматически,
	//  если главное окно приложения не является диалоговым
	SetIcon(m_hIcon, TRUE);			// Крупный значок
	SetIcon(m_hIcon, FALSE);		// Мелкий значок

	// TODO: добавьте дополнительную инициализацию
    m_cbEcl.AddString("Low (L)");
    m_cbEcl.AddString("Medium Low (H)");
    m_cbEcl.AddString("Medium High (Q)");
    m_cbEcl.AddString("High (H)");
    m_cbEcl.SetCurSel(m_nEcl);

    m_cbMask.AddString("Auto");
    m_cbMask.AddString("0 (101)");
    m_cbMask.AddString("1 (100)");
    m_cbMask.AddString("2 (111)");
    m_cbMask.AddString("3 (110)");
    m_cbMask.AddString("4 (001)");
    m_cbMask.AddString("5 (000)");
    m_cbMask.AddString("6 (011)");
    m_cbMask.AddString("7 (010)");
    m_cbMask.SetCurSel(m_nMask);

    //
    m_wndQR.m_pQR = &m_qr;
    m_wndQR.Create(NULL, NULL, WS_CHILD|WS_VISIBLE|WS_BORDER, CFrameWnd::rectDefault, this, 0);

    CWnd* pEdit = GetDlgItem(IDC_EDIT);
    RECT rcEdit, rcClient;
    GetClientRect(&rcClient);
    pEdit->GetWindowRect(&rcEdit);
    ScreenToClient(&rcEdit);
    m_wndQR.MoveWindow(rcEdit.left, rcEdit.bottom + rcEdit.left,
        rcClient.right - rcEdit.left*2, rcClient.bottom - rcEdit.bottom - rcEdit.left*2);

    //
    OnCbnSelchangeComboEcl();
    OnCbnSelchangeComboMask();
    m_bReady = true;
    OnEnChangeEdit();

	return TRUE;
}


void CQRDlg::OnSize(UINT nType, int cx, int cy)
{
    CDialog::OnSize(nType, cx, cy);
/*
    CWnd* pEdit = GetDlgItem(IDC_EDIT);
    if (!pEdit)
        return;

    RECT rcEdit;

    pEdit->GetWindowRect(&rcEdit);
    ScreenToClient(&rcEdit);
    m_wndQR.MoveWindow(0, rcEdit.bottom, cx, cy - rcEdit.bottom);*/
}


void CQRDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// При добавлении кнопки свертывания в диалоговое окно нужно воспользоваться приведенным ниже кодом,
//  чтобы нарисовать значок. Для приложений MFC, использующих модель документов или представлений,
//  это автоматически выполняется рабочей областью.

void CQRDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // контекст устройства для рисования

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Выравнивание значка по центру клиентского прямоугольника
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Нарисуйте значок
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// Система вызывает эту функцию для получения отображения курсора при перемещении
//  свернутого окна.
HCURSOR CQRDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CQRDlg::OnBnClickedButtonEncode()
{
    Encode();
}


int ANSItoUTF8(char* pUtf8Output, int nSize, const char* pAnsiInput)
{
    if( NULL == pAnsiInput && NULL == pUtf8Output ) return FALSE ;

    WCHAR* pWchar = NULL;
    int nWcharLen = 0;

    //convert to unicode firstly
    nWcharLen = MultiByteToWideChar( CP_ACP , 0 , pAnsiInput , -1 , NULL, 0 ) ;
    pWchar = new WCHAR[nWcharLen] ;
    ASSERT(pWchar != NULL);
    if (pWchar == NULL)
        return -1;
    memset(pWchar, 0, nWcharLen * sizeof(WCHAR));

    nWcharLen = MultiByteToWideChar(CP_ACP , 0, pAnsiInput, -1, pWchar, nWcharLen);
    ASSERT(nWcharLen != 0);
    if (nWcharLen == 0)
    {
        delete [] pWchar;
        return -1;
    }

    // get output buffer size
    int nOutput = WideCharToMultiByte(CP_UTF8, 0, pWchar, -1, NULL, 0 , NULL, NULL);
    nOutput = min(nOutput, nSize);
    ASSERT(nOutput != 0);

    if (0 == nOutput)
    {
        delete [] pWchar;
        return -1;
    }

    //
    memset(pUtf8Output, 0, nOutput);
    nOutput = WideCharToMultiByte(CP_UTF8, 0, pWchar, -1, (LPSTR)pUtf8Output, nOutput, NULL, NULL);

    delete [] pWchar;

    return nOutput;
}


int CQRDlg::Encode()
{
    if (!m_bReady)
        return -1;

    char szUtf8[10000];
    ANSItoUTF8(szUtf8, sizeof szUtf8, LPCTSTR(m_strText));

    int nRC = m_qr.init((BYTE*)szUtf8, (short)strlen(szUtf8), m_nEcl, m_nMask);
    //int nRC = m_qr.init((BYTE*)LPCTSTR(m_strText), (short)m_strText.GetLength(), m_nEcl, m_nMask);

    CRect rcWnd;
    int nSize = m_qr.getSizeXY();
    if (nSize > 0)
    {
        m_wndQR.GetWindowRect(&rcWnd);
        ScreenToClient(&rcWnd);
        m_qr.m_nPixelSize = (short)(rcWnd.Height() - m_qr.m_nQuiteZone*2)/ m_qr.getSizeXY();
        m_wndQR.Invalidate();
    }

    m_strInfo.Format("Ver. %d Blocks %d Bytes %d Mask %d Pixel %d",
        m_qr.m_nVersion, m_qr.getNumBlocks(), strlen(m_strText), m_qr.m_nMask, m_qr.m_nPixelSize);

    UpdateData(FALSE);

    return nRC;
}


void CQRDlg::OnCbnSelchangeComboEcl()
{
    m_nEcl = (short)m_cbEcl.GetCurSel();
    AfxGetApp()->WriteProfileInt("", "Ecl", m_nEcl);
    Encode();
}


void CQRDlg::OnCbnSelchangeComboMask()
{
    UpdateData(TRUE);

    m_nMask = (short)m_cbMask.GetCurSel() - 1;
/*
    // временно авто не работает
    if (m_nMask < 0) {
        m_nMask = 0;
        m_cbMask.SetCurSel(1);
        AfxMessageBox("Automatic mode is not implemented now");
    }
    else*/
        AfxGetApp()->WriteProfileInt("", "Mask", m_nMask + 1);

    Encode();
}


void CQRDlg::OnEnChangeEdit()
{
    UpdateData(TRUE);
    AfxGetApp()->WriteProfileString("", "Text", m_strText);
    Encode();
}

void CQRDlg::OnBnClickedCheckShowMask()
{
    UpdateData(TRUE);
    m_qr.m_bShowMask = (m_bShowMask != FALSE);
    Encode();
}


void CQRDlg::OnBnClickedCheckShowEc()
{
    UpdateData(TRUE);
    m_qr.m_bShowEc = (m_bShowEc != FALSE);
    Encode();
}


void CQRDlg::OnBnClickedCheckShowData()
{
    UpdateData(TRUE);
    m_qr.m_bShowData = (m_bShowData != FALSE);
    Encode();
}
