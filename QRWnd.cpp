// QRWnd.cpp: файл реализации
//

#include "stdafx.h"
#include "aQR.h"
#include "QRWnd.h"


// CQRWnd

IMPLEMENT_DYNAMIC(CQRWnd, CWnd)

CQRWnd::CQRWnd()
{
    m_pQR = NULL;
}

CQRWnd::~CQRWnd()
{
}


BEGIN_MESSAGE_MAP(CQRWnd, CWnd)
    ON_WM_PAINT()
END_MESSAGE_MAP()



// обработчики сообщений CQRWnd



void CQRWnd::OnPaint()
{
    CPaintDC dc(this);

    RECT rc;
    GetClientRect(&rc);
    dc.FillSolidRect(&rc, RGB(255,255,255));

    if (m_pQR)
        draw(&dc, 0, 0);
}

void CQRWnd::draw(CDC* pDC, int nX, int nY)
{
    int nSize = m_pQR->getSizeXY();
    BYTE cValue;
    COLORREF rgb;
    RECT rc;
    char szText[8];

    pDC->FillSolidRect(nX, nY,
        nX + m_pQR->m_nQuiteZone * 2 + m_pQR->getSizeXY()*m_pQR->m_nPixelSize,
        nY + m_pQR->m_nQuiteZone * 2 + m_pQR->getSizeXY()*m_pQR->m_nPixelSize, RGB(255,255,255));
/*
    CFont font;
    LOGFONT lf;

    memset(&lf, 0, sizeof lf);
    lf.lfHeight = -8;
    strcpy_s(lf.lfFaceName, sizeof lf.lfFaceName, "Courier");
    font.CreateFontIndirect(&lf);
    CFont* pFontOld = pDC->SelectObject(&font);
*/
    for (int x=0; x<nSize; x++)
    {
        for (int y=0; y<nSize; y++)
        {
            cValue = m_pQR->getPixel(x, y);

            switch (cValue) {
            case QR_VALUE_FP1:
                rgb = QR_COLOR_FP1;
                break;
            case QR_VALUE_FP0:
                rgb = QR_COLOR_FP0;
                break;
            default:
                rgb = m_pQR->m_bModeOrder ? QR_COLOR_D0 :
                    cValue ? QR_COLOR_D1 : QR_COLOR_D0;
                break;
            }

            rc.left = nX + x * m_pQR->m_nPixelSize + m_pQR->m_nQuiteZone;
            rc.top = nY + y * m_pQR->m_nPixelSize + m_pQR->m_nQuiteZone;
            rc.right = rc.left + m_pQR->m_nPixelSize;
            rc.bottom = rc.top + m_pQR->m_nPixelSize;

            if (m_pQR->m_bModeOrder && cValue != QR_VALUE_FP1 && cValue != QR_VALUE_FP0)
            {
                pDC->SetBkColor(cValue ? RGB(100,100,100) : RGB(255, 255, 255));
                sprintf_s(szText, sizeof szText, "%d", cValue % 100);
                pDC->DrawText(szText, 2, &rc, DT_CENTER | DT_VCENTER);
            }
            else
                pDC->FillSolidRect(&rc, rgb);

        }
    }
/*
    int nVerticalOffset = 0;
    for (int n=0; n<getSizeXY()/2; n++)
    {
        if (n > 3)
            nVerticalOffset = 1;
        pDC->MoveTo(nX + (n*2+nVerticalOffset)*m_nPixelSize + m_nQuiteZone, nY + m_nQuiteZone);
        pDC->LineTo(nX + (n*2+nVerticalOffset)*m_nPixelSize + m_nQuiteZone, nY + getSizeXY()*m_nPixelSize + m_nQuiteZone);
    }

    for (int n=0; n<getSizeXY()/4; n++)
    {
        pDC->MoveTo(nX + m_nQuiteZone, nY + (n*4+nVerticalOffset)*m_nPixelSize + m_nQuiteZone);
        pDC->LineTo(nX + getSizeXY()*m_nPixelSize + m_nQuiteZone, nY + (n*4+nVerticalOffset)*m_nPixelSize + m_nQuiteZone);
    }
*/
    //pDC->SelectObject(pFontOld);
    //font.DeleteObject();
}
