void Get24BitImage ( const int &nWidth, const int &nHeight,const HBITMAP &hBitmap , BYTE *lpDesBits)
{
  HDC hDC = ::GetDC( 0 );

  HDC memDC1 = ::CreateCompatibleDC ( hDC );
  HDC memDC2 = ::CreateCompatibleDC ( hDC );

  BYTE *lpBits = NULL;

  BITMAPINFO bmi;
  ::ZeroMemory( &bmi, sizeof(BITMAPINFO) );
  bmi.bmiHeader.biSize        = sizeof(BITMAPINFOHEADER);
  bmi.bmiHeader.biWidth       = nWidth;
  bmi.bmiHeader.biHeight      = nHeight;
  bmi.bmiHeader.biPlanes      = 1;
  bmi.bmiHeader.biBitCount    = 24;
  bmi.bmiHeader.biCompression = BI_RGB;

  HBITMAP hDIBMemBM  = ::CreateDIBSection( 0, &bmi, DIB_RGB_COLORS, (void**)&lpBits, NULL, NULL );
  HBITMAP hOldBmp1  = (HBITMAP)::SelectObject(memDC1, hDIBMemBM );
    
  HBITMAP hOldBmp2  = (HBITMAP) ::SelectObject ( memDC2,hBitmap);

  ::BitBlt( memDC1, 0, 0, nWidth, nHeight, memDC2, 0, 0, SRCCOPY );
    
  for ( int i = 0 ; i < nHeight ; i++)
    ::CopyMemory(&lpDesBits[i*3*nWidth],&lpBits[nWidth*3*(nHeight-1-i)],nWidth*3);

  // clean up
  ::SelectObject  ( memDC1, hOldBmp1  );
  ::SelectObject  ( memDC2,hOldBmp2  );
  ::ReleaseDC    ( 0, hDC      );
  ::DeleteObject  ( hDIBMemBM  );
  ::DeleteObject  ( hOldBmp1  );
  ::DeleteObject  ( hOldBmp2  );
  ::DeleteDC  ( memDC1  );
  ::DeleteDC  ( memDC2  );
}