HBITMAP REDIMP_F( HBITMAP HBMI, int W_OUT, int H_OUT ){
    HBITMAP HBMP_T = NULL;
    HDC ODC = GetDC( NULL );
    HBMP_T = CreateCompatibleBitmap( ODC , W_OUT, H_OUT );
    ///
    BITMAP BmpI = {0};
    GetObjectA( HBMI, sizeof(BITMAP), &BmpI );
    ///
    HDC hdc = CreateCompatibleDC( ODC );
    HDC memDC = CreateCompatibleDC( ODC );
    ReleaseDC( NULL, ODC );
    SelectObject( hdc , HBMI );/// HBPLOT
    SelectObject( memDC , HBMP_T );/// HB_I
    SetStretchBltMode( memDC , COLORONCOLOR | HALFTONE );/// COLORONCOLOR  HALFTONE
    StretchBlt( memDC, 0, 0, W_OUT, H_OUT, hdc, 0, 0, BmpI.bmWidth, BmpI.bmHeight, SRCCOPY );
    DeleteDC( hdc );
    DeleteDC( memDC );
    return HBMP_T;
}
