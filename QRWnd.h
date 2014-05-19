#pragma once


// CQRWnd

class aQR;

class CQRWnd : public CWnd
{
	DECLARE_DYNAMIC(CQRWnd)

public:
	CQRWnd();
	virtual ~CQRWnd();
    void draw(CDC* pDC, int nX, int nY);

    aQR* m_pQR;

protected:
	DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnPaint();
};
