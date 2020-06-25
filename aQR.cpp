/*
 * aQR - QR Code encoder class C++
 *
 * version 0.3.2 - 19.05.2014
 *
 * Copyright (C) 2014 Tyumenska region, Surgut
 * Konstantin Slabouzov <clip71@inbox.ru>
 *
 * temporary restrictions:
 *   support QR_ENC_BYTE only
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "aQR.h"

const aQR::QR_MAXIMUMS aQR::m_info[QR_MAX_VERSIONS+1] = {
     0,    0, {  0,  0,  0,  0 }, {  0,  0,  0,  0 }, 
    21,   26, {  7, 10, 13, 17 }, {  1,  1,  1,  1 }, // 1
    25,   44, { 10, 16, 22, 28 }, {  1,  1,  1,  1 }, // 2
    29,   70, { 15, 26, 18, 22 }, {  1,  1,  2,  2 }, // 3
    33,  100, { 20, 18, 26, 16 }, {  1,  2,  2,  4 }, // 4
    37,  134, { 26, 24, 18, 22 }, {  1,  2,  4,  4 }, // 5
    41,  172, { 18, 16, 24, 28 }, {  2,  4,  4,  4 }, // 6
    45,  196, { 20, 18, 18, 26 }, {  2,  4,  6,  5 }, // 7
    49,  242, { 24, 22, 22, 26 }, {  2,  4,  6,  6 }, // 8
    53,  292, { 30, 22, 20, 24 }, {  2,  5,  8,  8 }, // 9
    57,  346, { 18, 26, 24, 28 }, {  4,  5,  8,  8 }, // 10
    61,  404, { 20, 30, 28, 24 }, {  4,  5,  8, 11 }, // 11
    65,  466, { 24, 22, 26, 28 }, {  4,  8, 10, 11 }, // 12
    69,  532, { 26, 22, 24, 22 }, {  4,  9, 12, 16 }, // 13
    73,  581, { 30, 24, 20, 24 }, {  4,  9, 16, 16 }, // 14
    77,  655, { 22, 24, 30, 24 }, {  6, 10, 12, 18 }, // 15
    81,  733, { 24, 28, 24, 30 }, {  6, 10, 17, 16 }, // 16
    85,  815, { 28, 28, 28, 28 }, {  6, 11, 16, 19 }, // 17
    89,  901, { 30, 26, 28, 28 }, {  6, 13, 18, 21 }, // 18
    93,  991, { 28, 26, 26, 26 }, {  7, 14, 21, 25 }, // 19
    97, 1085, { 28, 26, 30, 28 }, {  8, 16, 20, 25 }, // 20
   101, 1156, { 28, 26, 28, 30 }, {  8, 17, 23, 25 }, // 21
   105, 1258, { 28, 28, 30, 24 }, {  9, 17, 23, 34 }, // 22
   109, 1364, { 30, 28, 30, 30 }, {  9, 18, 25, 30 }, // 23
   113, 1474, { 30, 28, 30, 30 }, { 10, 20, 27, 32 }, // 24
   117, 1588, { 26, 28, 30, 30 }, { 12, 21, 29, 35 }, // 25
   121, 1706, { 28, 28, 28, 30 }, { 12, 23, 34, 37 }, // 26
   125, 1828, { 30, 28, 30, 30 }, { 12, 25, 34, 40 }, // 27
   129, 1921, { 30, 28, 30, 30 }, { 13, 26, 35, 42 }, // 28
   133, 2051, { 30, 28, 30, 30 }, { 14, 28, 38, 45 }, // 29
   137, 2185, { 30, 28, 30, 30 }, { 15, 29, 40, 48 }, // 30
   141, 2323, { 30, 28, 30, 30 }, { 16, 31, 43, 51 }, // 31
   145, 2465, { 30, 28, 30, 30 }, { 17, 33, 45, 54 }, // 32
   149, 2611, { 30, 28, 30, 30 }, { 18, 35, 48, 57 }, // 33
   153, 2761, { 30, 28, 30, 30 }, { 19, 37, 51, 60 }, // 34
   157, 2876, { 30, 28, 30, 30 }, { 19, 38, 53, 63 }, // 35
   161, 3034, { 30, 28, 30, 30 }, { 20, 40, 56, 66 }, // 36
   165, 3196, { 30, 28, 30, 30 }, { 21, 43, 59, 70 }, // 37
   169, 3362, { 30, 28, 30, 30 }, { 22, 45, 62, 74 }, // 38
   173, 3532, { 30, 28, 30, 30 }, { 24, 47, 65, 77 }, // 39
   177, 3706, { 30, 28, 30, 30 }, { 25, 49, 68, 81 }, // 40
};


aQR::aQR(void)
{
    memset(m_pixels, 0, sizeof m_pixels);
    memset(m_szData, 0, sizeof m_szData);

    m_nEncoding = QR_ENC_BYTE; // пока только QR_ENC_BYTE
    m_nVersion = 0;
    m_nMask = -1;
    m_nEcl = QR_LEVEL_M;

    m_nDataSize = 0;
    m_nPixels = 0;

    m_nPixelSize = 10;
    m_nQuiteZone = 30;

    m_bModeOrder = false;
    m_bShowData = m_bShowEc = m_bShowMask = true;
}


aQR::~aQR(void)
{
}



int aQR::init(const BYTE* pszString, int nLength, int nEcl, int nMask)
{
    memset(m_pixels, 0, sizeof m_pixels);
    memset(m_szData, 0, sizeof m_szData);

    m_nEcl = (short)nEcl;
    m_nMask = (short)nMask;
    m_nPixels = 0;

    //
    int nRC = calcVersion(nLength);
    if (nRC <= 0 || nRC > QR_MAX_VERSIONS)
        return -1;

    fillServiceInfo();
    fillVersionCode();

    //
    m_nDataSize = getData(m_szData, sizeof m_szData, pszString, nLength);

    // дополняем байтами 236 и 17 не заполненные поля
    int nBitsOffset = m_nDataSize * 8;
    for (int i=m_nDataSize,nOdd=0; i<getDataSize(); i++,nOdd++)
    {
        pushData(m_szData, sizeof m_szData, nBitsOffset,
            nOdd & 1 ? 17 : 236, 8); // 00010001 | 11101100
        m_nDataSize++;
    }

    // заполнение матрицы
    QR_NEXT_XY nxy;
    short nBlocks = getNumBlocks();
    int nOffset;
    memset(&nxy, 0, sizeof nxy);

    for (int i=0; i<m_nDataSize; i++)
    {
        nOffset = getBlockIndex(i, m_nDataSize);

        pushByte(nxy, m_bShowData ? m_szData[nOffset] : 0);
    }

    BYTE ec[81 * 30];
    short nEcBlockSize = m_info[m_nVersion].ec[m_nEcl];

    for (short b=0; b<nBlocks; b++)
    {
        int nDataBlockSize = getBlockSize(b, m_nDataSize);
        int nDataBlockStart = getBlockOffset(b, m_nDataSize);

        short nEcBlockStart = b * nEcBlockSize;

        getEc(&ec[nEcBlockStart], nEcBlockSize, &m_szData[nDataBlockStart], nDataBlockSize);
    }

    for (int i=0; i<nEcBlockSize*nBlocks; i++)
    {
        nOffset = getBlockIndex(i, nEcBlockSize*nBlocks);
        pushByte(nxy, m_bShowEc ? ec[nOffset] : 0);
    }

    // маска
    if (m_bShowMask)
    {
        // если маска не задана, то поищем автоматически
        if (m_nMask < 0)
        {
            calcBestMask();
        }

        fillMask();
    }

    return 0;
}


int aQR::calcVersion(int nDataSize)
{
    for (m_nVersion=1; m_nVersion<=QR_MAX_VERSIONS; m_nVersion++)
    {
        WORD wSize = m_info[m_nVersion].wModules - m_info[m_nVersion].ec[m_nEcl] * getNumBlocks();

        if (wSize == 0)
            break;

        // проверим на вместимость: данных + размера поля данных + (0.5 + 0.5) тип данных и конец
        if (wSize >= nDataSize + getDataLengthBits() + 1)
            return m_nVersion;
    }

    //ASSERT(0); // таблица не заполнена или переполнение
    return -1;
}


void aQR::fillServiceInfo()
{
    // поисковые узоры
    fillFinderPattern(0, 0);
    fillFinderPattern(0, getSizeXY() - 7);
    fillFinderPattern(getSizeXY() - 7, 0);

    for (int x=0; x<=7; x++) {
        setPixel(x, 7, QR_VALUE_FP0);
        setPixel(7, x, QR_VALUE_FP0);

        setPixel(getSizeXY() - 8 + x, 7, QR_VALUE_FP0);
        setPixel(getSizeXY() - 8, x, QR_VALUE_FP0);

        setPixel(x, getSizeXY() - 8, QR_VALUE_FP0);
        setPixel(7, getSizeXY() - 8 + x, QR_VALUE_FP0);
    }

    // 
    fillAligmentPatterns();

    // пиксель всегда черен
    setPixel(8, getSizeXY() - 8, QR_VALUE_FP1);

    // Полосы синхронизации
    // всегда после fillAligmentPatterns(), т.к. пересекаются с выравнивающими узорами
    for (int x=8; x<getSizeXY()-8; x++) {
        setPixel(x, 6, x & 1 ? QR_VALUE_FP0 : QR_VALUE_FP1);
        setPixel(6, x, x & 1 ? QR_VALUE_FP0 : QR_VALUE_FP1);
    }

    // служебная информация
    fillFormatInfo();
}


// Код версии
void aQR::fillVersionCode()
{
    if (m_nVersion < 7)
        return;

    const char* pszCode = NULL;

    switch (m_nVersion) {
    case  7: pszCode = "000010011110100110"; break;
    case  8: pszCode = "010001011100111000"; break;
    case  9: pszCode = "110111011000000100"; break;
    case 10: pszCode = "101001111110000000"; break;
    case 11: pszCode = "001111111010111100"; break;
    case 12: pszCode = "001101100100011010"; break;
    case 13: pszCode = "101011100000100110"; break;
    case 14: pszCode = "110101000110100010"; break;
    case 15: pszCode = "010011000010011110"; break;
    case 16: pszCode = "011100010001011100"; break;
    case 17: pszCode = "111010010101100000"; break;
    case 18: pszCode = "100100110011100100"; break;
    case 19: pszCode = "000010110111011000"; break;
    case 20: pszCode = "000000101001111110"; break;
    case 21: pszCode = "100110101101000010"; break;
    case 22: pszCode = "111000001011000110"; break;
    case 23: pszCode = "011110001111111010"; break;
    case 24: pszCode = "001101001101100100"; break;
    case 25: pszCode = "101011001001011000"; break;
    case 26: pszCode = "110101101111011100"; break;
    case 27: pszCode = "010011101011100000"; break;
    case 28: pszCode = "010001110101000110"; break;
    case 29: pszCode = "110111110001111010"; break;
    case 30: pszCode = "101001010111111110"; break;
    case 31: pszCode = "001111010011000010"; break;
    case 32: pszCode = "101000011000101101"; break;
    case 33: pszCode = "001110011100010001"; break;
    case 34: pszCode = "010000111010010101"; break;
    case 35: pszCode = "110110111110101001"; break;
    case 36: pszCode = "110100100000001111"; break;
    case 37: pszCode = "010010100100110011"; break;
    case 38: pszCode = "001100000010110111"; break;
    case 39: pszCode = "101010000110001011"; break;
    case 40: pszCode = "111001000100010101"; break;
    default: return;
    }

    DWORD dwCode = strtol(pszCode, 0, 2);
    DWORD dwMask = 0x20000;
    int nX = 0, nY = getSizeXY() - 11;
    for (int i=0; i<18; i++)
    {
        setPixel(nX, nY, dwMask & dwCode ? QR_VALUE_FP1 : QR_VALUE_FP0);
        setPixel(nY, nX, dwMask & dwCode ? QR_VALUE_FP1 : QR_VALUE_FP0);

        if (++nX >= 6) {
            nX = 0;
            nY++;
        }
        dwMask >>= 1;
    }
}


void aQR::fillFinderPattern(int nX, int nY)
{
    for (int x=0; x<7; x++) {
        for (int y=0; y<7; y++) {
            setPixel(nX + x, nY + y, QR_VALUE_FP1);
        }
    }

    for (int x=1; x<6; x++) {
        setPixel(nX + x, nY + 1, QR_VALUE_FP0);
        setPixel(nX + x, nY + 5, QR_VALUE_FP0);
    }
    setPixel(nX + 1, nY + 2, QR_VALUE_FP0);
    setPixel(nX + 1, nY + 3, QR_VALUE_FP0);
    setPixel(nX + 1, nY + 4, QR_VALUE_FP0);
    setPixel(nX + 5, nY + 2, QR_VALUE_FP0);
    setPixel(nX + 5, nY + 3, QR_VALUE_FP0);
    setPixel(nX + 5, nY + 4, QR_VALUE_FP0);
}


void aQR::fillAligmentPattern(int nX, int nY)
{
    // проверка на попадание на служебную область (поисковые узоры)
    bool bFree = true;
    for (int x=0; x<5 && bFree; x++) {
        for (int y=0; y<5 && bFree; y++) {
            if (isPixelService(nX + x - 2, nY + y - 2))
                bFree = false;
        }
    }
    if (!bFree)
        return;

    // нарисуем выравнивающий узор на пустом месте
    for (int x=0; x<5; x++) {
        for (int y=0; y<5; y++) {
            setPixel(nX + x - 2, nY + y - 2, QR_VALUE_FP1);
        }
    }
    for (int x=0; x<3; x++) {
        for (int y=0; y<3; y++) {
            setPixel(nX + x - 1, nY + y - 1, QR_VALUE_FP0);
        }
    }
    setPixel(nX, nY, QR_VALUE_FP1);
}


void aQR::fillAligmentPatterns()
{
    const static WORD ap2[] = { 18 };
    const static WORD ap3[] = { 22 };
    const static WORD ap4[] = { 26 };
    const static WORD ap5[] = { 30 };
    const static WORD ap6[] = { 34 };
    const static WORD ap7[] = { 6, 22, 38 };
    const static WORD ap8[] = { 6, 24, 42 };
    const static WORD ap9[] = { 6, 26, 46 };
    const static WORD ap10[] = { 6, 28, 50 };
    const static WORD ap11[] = { 6, 30, 54 };
    const static WORD ap12[] = { 6, 32, 58 };
    const static WORD ap13[] = { 6, 34, 62 };
    const static WORD ap14[] = { 6, 26, 46, 66 };
    const static WORD ap15[] = { 6, 26, 48, 70 };
    const static WORD ap16[] = { 6, 26, 50, 74 };
    const static WORD ap17[] = { 6, 30, 54, 78 };
    const static WORD ap18[] = { 6, 30, 56, 82 };
    const static WORD ap19[] = { 6, 30, 58, 86 };
    const static WORD ap20[] = { 6, 34, 62, 90 };
    const static WORD ap21[] = { 6, 28, 50, 72, 94 };
    const static WORD ap22[] = { 6, 26, 50, 74, 98 };
    const static WORD ap23[] = { 6, 30, 54, 78, 102 };
    const static WORD ap24[] = { 6, 28, 54, 80, 106 };
    const static WORD ap25[] = { 6, 32, 58, 84, 110 };
    const static WORD ap26[] = { 6, 30, 58, 86, 114 };
    const static WORD ap27[] = { 6, 34, 62, 90, 118 };
    const static WORD ap28[] = { 6, 26, 50, 74, 98, 122 };
    const static WORD ap29[] = { 6, 30, 54, 78, 102, 126 };
    const static WORD ap30[] = { 6, 26, 52, 78, 104, 130 };
    const static WORD ap31[] = { 6, 30, 56, 82, 108, 134 };
    const static WORD ap32[] = { 6, 34, 60, 86, 112, 138 };
    const static WORD ap33[] = { 6, 30, 58, 86, 114, 142 };
    const static WORD ap34[] = { 6, 34, 62, 90, 118, 146 };
    const static WORD ap35[] = { 6, 30, 54, 78, 102, 126, 150 };
    const static WORD ap36[] = { 6, 24, 50, 76, 102, 128, 154 };
    const static WORD ap37[] = { 6, 28, 54, 80, 106, 132, 158 };
    const static WORD ap38[] = { 6, 32, 58, 84, 110, 136, 162 };
    const static WORD ap39[] = { 6, 26, 54, 82, 110, 138, 166 };
    const static WORD ap40[] = { 6, 30, 58, 86, 114, 142, 170 };
    WORD xy[8];
    memset(&xy, 0, sizeof xy);

#define initxy(a) memcpy(xy, a, sizeof a)

    switch (m_nVersion) {
    case 1: return;
    case 2: initxy(ap2); break;
    case 3: initxy(ap3); break;
    case 4: initxy(ap4); break;
    case 5: initxy(ap5); break;
    case 6: initxy(ap6); break;
    case 7: initxy(ap7); break;
    case 8: initxy(ap8); break;
    case 9: initxy(ap9); break;
    case 10: initxy(ap10); break;
    case 11: initxy(ap11); break;
    case 12: initxy(ap12); break;
    case 13: initxy(ap13); break;
    case 14: initxy(ap14); break;
    case 15: initxy(ap15); break;
    case 16: initxy(ap16); break;
    case 17: initxy(ap17); break;
    case 18: initxy(ap18); break;
    case 19: initxy(ap19); break;
    case 20: initxy(ap20); break;
    case 21: initxy(ap21); break;
    case 22: initxy(ap22); break;
    case 23: initxy(ap23); break;
    case 24: initxy(ap24); break;
    case 25: initxy(ap25); break;
    case 26: initxy(ap26); break;
    case 27: initxy(ap27); break;
    case 28: initxy(ap28); break;
    case 29: initxy(ap29); break;
    case 30: initxy(ap30); break;
    case 31: initxy(ap31); break;
    case 32: initxy(ap32); break;
    case 33: initxy(ap33); break;
    case 34: initxy(ap34); break;
    case 35: initxy(ap35); break;
    case 36: initxy(ap36); break;
    case 37: initxy(ap37); break;
    case 38: initxy(ap38); break;
    case 39: initxy(ap39); break;
    case 40: initxy(ap40); break;
    default:
        //ASSERT(0);
        return;
    }

    for (int i=0; xy[i]; i++)
        for (int j=0; xy[j]; j++)
            fillAligmentPattern(xy[i], xy[j]);
}


WORD aQR::getMaskEcl(int nEcl, int nMask)
{
    const char* pszCode = NULL;

    switch (nEcl) {
    case QR_LEVEL_L:
        switch (nMask) {
        case 0: pszCode = "111011111000100"; break;
        case 1: pszCode = "111001011110011"; break;
        case 2: pszCode = "111110110101010"; break;
        case 3: pszCode = "111100010011101"; break;
        case 4: pszCode = "110011000101111"; break;
        case 5: pszCode = "110001100011000"; break;
        case 6: pszCode = "110110001000001"; break;
        case 7: pszCode = "110100101110110"; break;
        }
        break;
    case QR_LEVEL_M:
        switch (nMask) {
        case 0: pszCode = "101010000010010"; break;
        case 1: pszCode = "101000100100101"; break;
        case 2: pszCode = "101111001111100"; break;
        case 3: pszCode = "101101101001011"; break;
        case 4: pszCode = "100010111111001"; break;
        case 5: pszCode = "100000011001110"; break;
        case 6: pszCode = "100111110010111"; break;
        case 7: pszCode = "100101010100000"; break;
        }
        break;
    case QR_LEVEL_Q:
        switch (nMask) {
        case 0: pszCode = "011010101011111"; break;
        case 1: pszCode = "011000001101000"; break;
        case 2: pszCode = "011111100110001"; break;
        case 3: pszCode = "011101000000110"; break;
        case 4: pszCode = "010010010110100"; break;
        case 5: pszCode = "010000110000011"; break;
        case 6: pszCode = "010111011011010"; break;
        case 7: pszCode = "010101111101101"; break;
        }
        break;
    case QR_LEVEL_H:
        switch (nMask) {
        case 0: pszCode = "001011010001001"; break;
        case 1: pszCode = "001001110111110"; break;
        case 2: pszCode = "001110011100111"; break;
        case 3: pszCode = "001100111010000"; break;
        case 4: pszCode = "000011101100010"; break;
        case 5: pszCode = "000001001010101"; break;
        case 6: pszCode = "000110100001100"; break;
        case 7: pszCode = "000100000111011"; break;
        }
        break;
    }

    return (WORD)(pszCode ? strtol(pszCode, 0, 2) : 0);
}


void aQR::fillFormatInfo()
{
    static QR_POINT mecl1[15] = {
        { 0, 8 }, { 1, 8 }, { 2, 8 }, { 3, 8 }, { 4, 8 }, { 5, 8 }, { 7, 8 }, { 8, 8 },
        { 8, 7 }, { 8, 5 }, { 8, 4 }, { 8, 3 }, { 8, 2 }, { 8, 1 }, { 8, 0 }
    };

    short nSize = getSizeXY();

    QR_POINT mecl2[15] = {
        { 8, nSize - 1 }, { 8, nSize - 2 }, { 8, nSize - 3 }, { 8, nSize - 4 },
        { 8, nSize - 5 }, { 8, nSize - 6 }, { 8, nSize - 7 }, { nSize - 8, 8 },
        { nSize - 7, 8 }, { nSize - 6, 8 }, { nSize - 5, 8 }, { nSize - 4, 8 },
        { nSize - 3, 8 }, { nSize - 2, 8 }, { nSize - 1, 8 }
    };

    WORD wMaskEcl = getMaskEcl(m_nEcl, m_nMask);

    for (int i=0; i<15; i++)
    {
        setPixel(mecl1[i].x, mecl1[i].y, wMaskEcl & 0x4000 ? QR_VALUE_FP1 : QR_VALUE_FP0);
        setPixel(mecl2[i].x, mecl2[i].y, wMaskEcl & 0x4000 ? QR_VALUE_FP1 : QR_VALUE_FP0);
        wMaskEcl <<= 1;
    }
}


void aQR::fillMask()
{
    if (m_nMask < 0 || m_nMask > 7)
        return;

    BYTE bValue, bMask;

    for (int x=0; x<getSizeXY(); x++)
        for (int y=0; y<getSizeXY(); y++)
        {
            bValue = getPixel(x, y);
            if (bValue == QR_VALUE_FP1 || bValue == QR_VALUE_FP0)
                continue;

            switch (m_nMask) {
            case 0:
                bMask = (x + y) % 2 == 0;
                break;
            case 1:
                bMask = y % 2 == 0;
                break;
            case 2:
                bMask = x % 3 == 0;
                break;
            case 3:
                bMask = (x + y) % 3 == 0;
                break;
            case 4:
                bMask = (x/3 + y/2) % 2 == 0;
                break;
            case 5:
                bMask = ((x*y) % 2 + (x*y) % 3) == 0;
                break;
            case 6:
                bMask = ((x * y) % 2 + (x * y) % 3) % 2 == 0;
                break;
            case 7:
                bMask = ((x * y) % 3 + (x + y) % 2) % 2 == 0;
                break;
            default:
                //ASSERT(0);
                bMask = 0;
                break;
            }
            if (bMask)
                setPixel(x, y, !bValue);
        }
}


int aQR::getBlockIndex(int nIndex, short nDataSize) const
{
    int nBlocks = getNumBlocks();
    if (nBlocks == 0)
        return 0;

    int nBlockSize = getBlockSize(0, nDataSize);
    if (nIndex < nBlockSize * nBlocks)
    {
        int nBlock = nIndex % nBlocks;
        int nBlockOffset = getBlockOffset(nBlock, nDataSize);
        return nBlockOffset + nIndex / nBlocks;
    }
    else
    {
        return nIndex;
    }
}


int aQR::getBlockSize(int nBlock, int nDataSize) const
{
    int nBlocks = getNumBlocks();
    int nBlockSize = nDataSize / nBlocks;

    int nExtra = nDataSize - nBlockSize * nBlocks;
    if (nExtra > 0)
    {
        if (nBlocks - nBlock % nBlocks <= nExtra)
            return nBlockSize + 1;
    }

    return nBlockSize;
}


int aQR::getBlockOffset(int nBlock, int nDataSize) const
{
    int nBlocks = getNumBlocks();

    int nBlockSize = nDataSize / nBlocks;
    int nExtra = nDataSize - nBlockSize * nBlocks;
    if (nExtra > 0)
    {
        if (nBlock > nBlocks - nExtra)
            nExtra = nBlock - (nBlocks - nExtra);
        else
            nExtra = 0;
    }

    int nBlockStart = nBlock * (nDataSize / nBlocks) + nExtra;
    return nBlockStart;
}


int aQR::getGenPoly(BYTE* pBytes)
{
    int nBytes = m_info[m_nVersion].ec[m_nEcl];
    //ASSERT(nBytes != 0);

    //
    const static BYTE cb7[7] = { 87, 229, 146, 149, 238, 102, 21 };
    const static BYTE cb10[10] = { 251, 67, 46, 61, 118, 70, 64, 94, 32, 45 };
    const static BYTE cb13[13] = { 74, 152, 176, 100, 86, 100, 106, 104, 130, 218, 206, 140, 78 };
    const static BYTE cb15[15] = { 8, 183, 61, 91, 202, 37, 51, 58, 58, 237, 140, 124, 5, 99, 105 };
    const static BYTE cb16[16] = { 120, 104, 107, 109, 102, 161, 76, 3, 91, 191, 147, 169, 182, 194, 225, 120 };
    const static BYTE cb17[17] = { 43, 139, 206, 78, 43, 239, 123, 206, 214, 147, 24, 99, 150, 39, 243, 163, 136 };
    const static BYTE cb18[18] = { 215, 234, 158, 94, 184, 97, 118, 170, 79, 187,
        152, 148, 252, 179, 5, 98, 96, 153 };
    const static BYTE cb20[20] = { 17, 60, 79, 50, 61, 163, 26, 187, 202, 180, 221,
        225, 83, 239, 156, 164, 212, 212, 188, 190 };
    const static BYTE cb22[22] = { 210, 171, 247, 242, 93, 230, 14, 109, 221, 53,
        200, 74, 8, 172, 98, 80, 219, 134, 160, 105, 165, 231 };
    const static BYTE cb24[24] = { 229, 121, 135, 48, 211, 117, 251, 126, 159, 180,
        169, 152, 192, 226, 228, 218, 111, 0, 117, 232, 87, 96, 227, 21 };
    const static BYTE cb26[26] = { 173, 125, 158, 2, 103, 182, 118, 17, 145, 201, 111,
        28, 165, 53, 161, 21, 245, 142, 13, 102, 48, 227, 153, 145, 218, 70 };
    const static BYTE cb28[28] = { 168, 223, 200, 104, 224, 234, 108, 180, 110, 190,
        195, 147, 205, 27, 232, 201, 21, 43, 245, 87, 42, 195, 212, 119, 242, 37, 9, 123 };
    const static BYTE cb30[30] = { 41, 173, 145, 152, 216, 31, 179, 182, 50, 48, 110,
        86, 239, 96, 222, 125, 42, 173, 226, 193, 224, 130, 156, 37, 251, 216, 238, 40, 192, 180 };

    switch (nBytes) {
    case 7:
        memcpy(pBytes, cb7, 7);
        break;
    case 10:
        memcpy(pBytes, cb10, 10);
        break;
    case 13:
        memcpy(pBytes, cb13, 13);
        break;
    case 15:
        memcpy(pBytes, cb15, 15);
        break;
    case 16:
        memcpy(pBytes, cb16, 16);
        break;
    case 17:
        memcpy(pBytes, cb17, 17);
        break;
    case 18:
        memcpy(pBytes, cb18, 18);
        break;
    case 20:
        memcpy(pBytes, cb20, 20);
        break;
    case 22:
        memcpy(pBytes, cb22, 22);
        break;
    case 24:
        memcpy(pBytes, cb24, 24);
        break;
    case 26:
        memcpy(pBytes, cb26, 26);
        break;
    case 28:
        memcpy(pBytes, cb28, 28);
        break;
    case 30:
        memcpy(pBytes, cb30, 30);
        break;
    default:
        //ASSERT(0);
        break;
    }

    return nBytes;
}


BYTE aQR::getGalois(BYTE cValue, bool bRevert)
{
    const static BYTE aGalois[256] = {
        1, 2, 4, 8, 16, 32, 64, 128, 29, 58, 116, 232, 205, 135, 19, 38,
        76, 152, 45, 90, 180, 117, 234, 201, 143, 3, 6, 12, 24, 48, 96, 192,
        157, 39, 78, 156, 37, 74, 148, 53, 106, 212, 181, 119, 238, 193, 159, 35,
        70, 140, 5, 10, 20, 40, 80, 160, 93, 186, 105, 210, 185, 111, 222, 161,
        95, 190, 97, 194, 153, 47, 94, 188, 101, 202, 137, 15, 30, 60, 120, 240,
        253, 231, 211, 187, 107, 214, 177, 127, 254, 225, 223, 163, 91, 182, 113, 226,
        217, 175, 67, 134, 17, 34, 68, 136, 13, 26, 52, 104, 208, 189, 103, 206,
        129, 31, 62, 124, 248, 237, 199, 147, 59, 118, 236, 197, 151, 51, 102, 204,
        133, 23, 46, 92, 184, 109, 218, 169, 79, 158, 33, 66, 132, 21, 42, 84,
        168, 77, 154, 41, 82, 164, 85, 170, 73, 146, 57, 114, 228, 213, 183, 115,
        230, 209, 191, 99, 198, 145, 63, 126, 252, 229, 215, 179, 123, 246, 241, 255,
        227, 219, 171, 75, 150, 49, 98, 196, 149, 55, 110, 220, 165, 87, 174, 65,
        130, 25, 50, 100, 200, 141, 7, 14, 28, 56, 112, 224, 221, 167, 83, 166,
        81, 162, 89, 178, 121, 242, 249, 239, 195, 155, 43, 86, 172, 69, 138, 9,
        18, 36, 72, 144, 61, 122, 244, 245, 247, 243, 251, 235, 203, 139, 11, 22,
        44, 88, 176, 125, 250, 233, 207, 131, 27, 54, 108, 216, 173, 71, 142, 1 };

    if (bRevert)
    {
        for (short rc=0; rc<256; rc++)
            if (aGalois[rc] == cValue)
                return (BYTE)rc;
        return 0;
    }

    return aGalois[cValue];
}


short aQR::getData(BYTE* pData, int nDataSize, const BYTE* pInput, int nInputSize)
{
    int nBitsOffset = 0;

    // формат данных
    // возможно вставить сюда автоопределение
    pushData(pData, nDataSize, nBitsOffset, m_nEncoding, 4);

    // поле количества данных
    pushData(pData, nDataSize, nBitsOffset, nInputSize, getDataLengthBits() * 8);

    for (int i=0; i<nInputSize; i++)
        pushData(pData, nDataSize, nBitsOffset, pInput[i], 8);

    // конец данных
    pushData(pData, nDataSize, nBitsOffset, 0, 4);

    return (short)(nBitsOffset-1) / 8 + 1;
}


int aQR::getEc(BYTE* pEc, int nEcSize, BYTE* pData, int nDataSize)
{
    // байты коррекции
    BYTE aGenPoly[QR_MAX_CORR_BYTES];
    memset(aGenPoly, 0, sizeof aGenPoly);

    int nGenPoly = getGenPoly(aGenPoly);
    int nEc = max(nDataSize, nGenPoly);

    memset(pEc, 0, nEcSize);
    memcpy(pEc, pData, nDataSize);

    int nz;
    for (int d=0; d<nDataSize; d++)
    {
        BYTE z = pEc[0];

        for (int i=0; i<nEc; i++)
            pEc[i] = pEc[i + 1];
        pEc[nEc - 1] = 0;

        if (z == 0)
            continue;

        z = getGalois(z, true);

        for (WORD i=0; i<nGenPoly; i++)
        {
            nz = aGenPoly[i] + z;
            if (nz > 254)
                nz = nz % 255;
            BYTE zz = getGalois((BYTE)nz, false);
            pEc[i] ^= zz;
        }

        z = 0;
    }

    return nGenPoly;
}


bool aQR::getNextXY(QR_NEXT_XY &nxy)
{
    do
    {
        nxy.x--;
        if (!nxy.bVerticalTiming && nxy.x < 6) {
            nxy.nColumn--;
            nxy.bVerticalTiming = true;
        }

        if (nxy.x < nxy.nColumn - 1) {
            nxy.x = nxy.nColumn;
            nxy.y += nxy.nDirectionY;
        }

        if (nxy.x < 0)
            return false;

        if (nxy.y < 0) {
            nxy.y = 0;
            nxy.nColumn -= 2;
            nxy.x = nxy.nColumn;
            nxy.nDirectionY = -nxy.nDirectionY;
        }

        if (nxy.y >= getSizeXY()) {
            nxy.y = getSizeXY() - 1;
            nxy.nColumn -= 2;
            nxy.x = nxy.nColumn;
            nxy.nDirectionY = -nxy.nDirectionY;
        }
    }
    while (isPixelService(nxy.x, nxy.y));

    return true;
}


bool aQR::pushPixel(QR_NEXT_XY &nxy, BYTE cValue)
{
    if (nxy.x == 0 && nxy.y == 0)
    {
        nxy.x = nxy.y = nxy.nColumn = getSizeXY() - 1;
        nxy.nDirectionY = -1;
        nxy.bVerticalTiming = 0;
        m_nPixels = 0;
    }

    setPixel(nxy.x, nxy.y, m_bModeOrder ? (BYTE)m_nPixels : cValue);
    m_nPixels++;
    return getNextXY(nxy);
}


bool aQR::pushByte(QR_NEXT_XY &nxy, BYTE cByte)
{
    for (int j=0; j<8 && nxy.x>=0; j++)
    {
        if (!pushPixel(nxy, cByte & 0x80 ? 1 : 0))
            return false; // что то не так, не хватило места
        cByte <<= 1;
    }
    return true;
}


bool aQR::pushData(BYTE* pData, int nDataSize, int& nBitsOffset, unsigned int nData, int nBits)
{
    // если вдруг переполнение, то прервемся
    if (nBitsOffset/8 >= nDataSize)
        return false;

    DWORD dwMask = 1;
    for (int i=0; i<nBits-1; i++)
        dwMask <<= 1;

    DWORD dwMaskData = 0x80;
    for (int i=0; i<(nBitsOffset%8); i++)
        dwMaskData >>= 1;

    BYTE* pPtr = pData + nBitsOffset/8;
    for (int i=0; i<nBits; i++)
    {
        if (nData & dwMask)
            *pPtr |= dwMaskData;

        dwMask >>= 1;

        if (++nBitsOffset % 8 == 0)
        {
            dwMaskData = 0x80;
            pPtr++;
        }
        else
        {
            dwMaskData >>= 1;
        }

        // если вдруг переполнение, то прервемся
        if (nBitsOffset/8 >= nDataSize)
            return false;
    }

    return true;
}


void aQR::calcBestMask()
{
    short nBestMask = -1;
    short nBestCost = -1;
    for (m_nMask=0; m_nMask<8; m_nMask++)
    {
        fillFormatInfo();
        fillMask(); // выставим маску
        short nCost1 = checkPenaltyRule1();
        short nCost2 = checkPenaltyRule2();
        short nCost3 = checkPenaltyRule3();
        short nCost4 = checkPenaltyRule4();
        fillMask(); // снесем маску

        short nCost = nCost1 + nCost2 + nCost3 + nCost4;
        if (nCost < nBestCost || nBestCost < 0) {
            nBestMask = m_nMask;
            nBestCost = nCost;
        }

    }

    m_nMask = nBestMask;
    fillFormatInfo();
}


short aQR::checkPenaltyRule1()
{
    short nPenaltyX = 0;
    short nPenaltyY = 0;

    for (int x=0; x<getSizeXY(); x++)
    {
        bool bPreviousWhiteX = !isPixelWhite(x, 0);
        int nSeriesX = 0;

        bool bPreviousWhiteY = !isPixelWhite(0, x);
        int nSeriesY = 0;

        for (int y=0; y<getSizeXY(); y++)
        {
            if (x == 7) {
                x = 7;
            }

            // X
            if (bPreviousWhiteX == isPixelWhite(x, y))
            {
                nSeriesX++;
            }
            else
            {
                bPreviousWhiteX = !bPreviousWhiteX;
                nSeriesX = 1;
            }

            if (nSeriesX == 5)
                nPenaltyX += 3;
            else if (nSeriesX > 5)
                nPenaltyX++;

            // Y
            if (bPreviousWhiteY == isPixelWhite(y, x))
            {
                nSeriesY++;
            }
            else
            {
                bPreviousWhiteY = !bPreviousWhiteY;
                nSeriesY = 1;
            }

            if (nSeriesY == 5)
                nPenaltyY += 3;
            else if (nSeriesY > 5)
                nPenaltyY++;
        }
    }

    return nPenaltyX + nPenaltyY;
}


short aQR::checkPenaltyRule2()
{
    short nPenalty = 0;

    for (int x=0; x<getSizeXY()-2; x++)
        for (int y=0; y<getSizeXY()-2; y++)
            if (isPixelWhite(x, y) == isPixelWhite(x + 1, y)
             && isPixelWhite(x + 1, y) == isPixelWhite(x, y + 1)
             && isPixelWhite(x, y + 1) == isPixelWhite(x + 1, y + 1)
                )
                nPenalty += 3;

    return nPenalty;
}


short aQR::checkPenaltyRule3()
{
    short nPenalty = 0;

    // X
    for (int y=0; y<getSizeXY()-11; y++)
        for (int x=0; x<getSizeXY()-11; x++)
        {
            if (x < getSizeXY()-15)
            {
                if ( isPixelWhite(x, y)
                 &&  isPixelWhite(x + 1, y)
                 &&  isPixelWhite(x + 2, y)
                 &&  isPixelWhite(x + 3, y)
                 && !isPixelWhite(x + 4, y)
                 &&  isPixelWhite(x + 5, y)
                 && !isPixelWhite(x + 6, y)
                 && !isPixelWhite(x + 7, y)
                 && !isPixelWhite(x + 8, y)
                 &&  isPixelWhite(x + 9, y)
                 && !isPixelWhite(x + 10, y)
                 &&  isPixelWhite(x + 11, y)
                 &&  isPixelWhite(x + 12, y)
                 &&  isPixelWhite(x + 13, y)
                 &&  isPixelWhite(x + 14, y)
                 ) {
                    nPenalty += 40;
                    x += 15;
                }
            }

            if (x < getSizeXY()-11)
            {
                if (!isPixelWhite(x, y)
                 &&  isPixelWhite(x + 1, y)
                 && !isPixelWhite(x + 2, y)
                 && !isPixelWhite(x + 3, y)
                 && !isPixelWhite(x + 4, y)
                 &&  isPixelWhite(x + 5, y)
                 && !isPixelWhite(x + 6, y)
                 &&  isPixelWhite(x + 7, y)
                 &&  isPixelWhite(x + 8, y)
                 &&  isPixelWhite(x + 9, y)
                 &&  isPixelWhite(x + 10, y)) {
                    nPenalty += 40;
                    x += 11;
                }
            }

            if (x < getSizeXY()-11)
            {
                if ( isPixelWhite(x, y)
                 &&  isPixelWhite(x + 1, y)
                 &&  isPixelWhite(x + 2, y)
                 &&  isPixelWhite(x + 3, y)
                 && !isPixelWhite(x + 4, y)
                 &&  isPixelWhite(x + 5, y)
                 && !isPixelWhite(x + 6, y)
                 && !isPixelWhite(x + 7, y)
                 && !isPixelWhite(x + 8, y)
                 &&  isPixelWhite(x + 9, y)
                 && !isPixelWhite(x + 10, y)) {
                    nPenalty += 40;
                    x += 11;
                }
            }
        }


    // Y
    for (int x=0; x<getSizeXY()-11; x++)
        for (int y=0; y<getSizeXY()-11; y++)
        {
            if (y < getSizeXY()-15)
            {
                if ( isPixelWhite(y, x)
                 &&  isPixelWhite(y + 1, x)
                 &&  isPixelWhite(y + 2, x)
                 &&  isPixelWhite(y + 3, x)
                 && !isPixelWhite(y + 4, x)
                 &&  isPixelWhite(y + 5, x)
                 && !isPixelWhite(y + 6, x)
                 && !isPixelWhite(y + 7, x)
                 && !isPixelWhite(y + 8, x)
                 &&  isPixelWhite(y + 9, x)
                 && !isPixelWhite(y + 10, x)
                 &&  isPixelWhite(y + 11, x)
                 &&  isPixelWhite(y + 12, x)
                 &&  isPixelWhite(y + 13, x)
                 &&  isPixelWhite(y + 14, x)
                 ) {
                    nPenalty += 40;
                    y += 15;
                }
            }

            if (y < getSizeXY()-11)
            {
                if (!isPixelWhite(y, y)
                 &&  isPixelWhite(y + 1, x)
                 && !isPixelWhite(y + 2, x)
                 && !isPixelWhite(y + 3, x)
                 && !isPixelWhite(y + 4, x)
                 &&  isPixelWhite(y + 5, x)
                 && !isPixelWhite(y + 6, x)
                 &&  isPixelWhite(y + 7, x)
                 &&  isPixelWhite(y + 8, x)
                 &&  isPixelWhite(y + 9, x)
                 &&  isPixelWhite(y + 10, x)) {
                    nPenalty += 40;
                    y += 11;
                }
            }

            if (y < getSizeXY()-11)
            {
                if ( isPixelWhite(y, y)
                 &&  isPixelWhite(y + 1, x)
                 &&  isPixelWhite(y + 2, x)
                 &&  isPixelWhite(y + 3, x)
                 && !isPixelWhite(y + 4, x)
                 &&  isPixelWhite(y + 5, x)
                 && !isPixelWhite(y + 6, x)
                 && !isPixelWhite(y + 7, x)
                 && !isPixelWhite(y + 8, x)
                 &&  isPixelWhite(y + 9, x)
                 && !isPixelWhite(y + 10, x)) {
                    nPenalty += 40;
                    y += 11;
                }
            }
        }

    return nPenalty;
}


short aQR::checkPenaltyRule4()
{
    int nWhite = 0;
    int nBlack = 0;

    for (int x=0; x<getSizeXY(); x++)
        for (int y=0; y<getSizeXY(); y++)
            if (isPixelWhite(x, y))
                nWhite++;
            else
                nBlack++;

    double d = (double)nBlack / nWhite;
    d *= 100;
    d -= 50;
    d *= 2;
    if (d < 0)
        d = -d;

    return (short)d;
}
