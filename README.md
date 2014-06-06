C++ class for generation QR Code

Usage:

  #define MASK_AUTO -1
  
    aQR qr;
    qr.init("Hello, World!", strlen(Hello, World!), QR_LEVEL_L, MASK_AUTO);

    for (int x=0; x<qr.getSizeXY(); x++)
    {
      for (int y=0; y<qr.getSizeXY(); y++)
      {
        if (qr.isPixelWhite(x, y))
            ... draw white pixel
        else
            ... draw black pixel
      }
    }
