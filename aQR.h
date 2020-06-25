#pragma once

// необходимые доописания
#ifndef DWORD
#define DWORD unsigned long
#endif
#ifndef WORD
#define WORD unsigned short
#endif
#ifndef BYTE
#define BYTE unsigned char
#endif

#ifndef max
#define max(a,b)            (((a) > (b)) ? (a) : (b))
#endif


//
#define QR_MAX_VERSIONS     40
#define QR_MAX_CORR_BYTES   30

#define QR_LEVEL_L  0
#define QR_LEVEL_M  1
#define QR_LEVEL_Q  2
#define QR_LEVEL_H  3

#define QR_ENC_NUMERIC      0x1 // 0001 Numeric encoding (10 bits per 3 digits)
#define QR_ENC_ALPHANUMERIC 0x2 // 0010 Alphanumeric encoding (11 bits per 2 characters)
#define QR_ENC_BYTE         0x4 // 0100 Byte encoding (8 bits per character)
#define QR_ENC_END          0x0 // 0000 End of message

#define QR_COLOR_FP1    RGB(0,0,0)
#define QR_COLOR_FP0    RGB(255,255,255)
#define QR_COLOR_D1     RGB(0,0,0)
#define QR_COLOR_D0     RGB(255,255,255)

#define QR_VALUE_FP1 255
#define QR_VALUE_FP0 254


class aQR
{
public:

    typedef struct _QR_MAXIMUMS {
        WORD wSizeXY;
        WORD wModules;
        WORD ec[4];
        BYTE blocks[4];
    } QR_MAXIMUMS;

    typedef struct _QR_MASK_ECL {
        char cLevel;
        char cMask;
        WORD wCode;
    } QR_MASK_ECL;


    typedef struct _QR_NEXT_XY {
        short x;
        short y;
        WORD nColumn;
        char nDirectionY;
        char bVerticalTiming;
    } QR_NEXT_XY;

    typedef struct tagQR_POINT
    {
        short x;
        short y;
    } QR_POINT;

public:
    aQR(void);
    ~aQR(void);

    inline int getVersion() const { return m_nVersion; };
    inline short getSizeXY() const { return m_info[m_nVersion].wSizeXY; };
    inline short getDataLengthBits() const { return m_nVersion <= 9 ? 1 : 2; };
    inline short getDataSize() const {
        return m_info[m_nVersion].wModules - getNumBlocks() * m_info[m_nVersion].ec[m_nEcl]; };
    inline short getNumBlocks() const { return m_info[m_nVersion].blocks[m_nEcl]; };

    inline BYTE getPixel(int x, int y) const { return m_pixels[x * getSizeXY() + y]; };
    inline void setPixel(int x, int y, BYTE cVal) { m_pixels[x * getSizeXY() + y] = cVal; };
    inline bool isPixelService(int x, int y) const {
        BYTE p = m_pixels[x * getSizeXY() + y]; return p == QR_VALUE_FP1 || p == QR_VALUE_FP0;
    };
    inline bool isPixelWhite(int x, int y) const {
        BYTE p = m_pixels[x * getSizeXY() + y]; return p == 0 || p == QR_VALUE_FP0;
    };
    inline bool isPixelBlack(int x, int y) const { return !isPixelWhite(x, y); };

    int init(const BYTE* pszString, int nLength, int nEcl, int nMask);
    int calcVersion(int nDataSize);
    void fillServiceInfo();
    void fillVersionCode();
    void fillFinderPattern(int nX, int nY);
    void fillAligmentPattern(int nX, int nY);
    void fillAligmentPatterns();
    void fillFormatInfo();
    void fillMask();
    WORD getMaskEcl(int nEcl, int nMask);
    bool getNextXY(QR_NEXT_XY &nxy);
    bool pushPixel(QR_NEXT_XY &nxy, BYTE cValue);
    bool pushByte(QR_NEXT_XY &nxy, BYTE cValue);
    bool pushData(BYTE* pData, int nDataSize, int& nBitsOffset, unsigned int nData, int nBits);

    int getBlockIndex(int nIndex, short nDataSize) const;
    int getBlockSize(int nBlock, int nDataSize) const;
    int getBlockOffset(int nBlock, int nDataSize) const;

    int getGenPoly(BYTE* pBytes);
    BYTE getGalois(BYTE cValue, bool bRevert);

    short getData(BYTE* pData, int nDataSize, const BYTE* pInput, int nInputSize);
    int getEc(BYTE* pEc, int nEcSize, BYTE* pData, int nDataSize);

    // маска
    void calcBestMask();
    short checkPenaltyRule1();
    short checkPenaltyRule2();
    short checkPenaltyRule3();
    short checkPenaltyRule4();

    short m_nEncoding;  // Encoding modes
    short m_nEcl;       // Error correction level
    short m_nMask;      // Mask pattern
    short m_nVersion;   // Version 1..40

    short m_nPixelSize;
    short m_nQuiteZone;
    short m_nPixels; // for debuging only

    short m_nDataSize;
    BYTE m_szData[7090];
    BYTE m_pixels[(QR_MAX_VERSIONS*4 + 17)*(QR_MAX_VERSIONS*4 + 17)];
    const static QR_MAXIMUMS m_info[QR_MAX_VERSIONS+1];

    bool m_bModeOrder; // for debuging only
    bool m_bShowData;
    bool m_bShowEc;
    bool m_bShowMask;
};
