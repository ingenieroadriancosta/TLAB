BOOL DIM2::DILATEPROC( DIM2* D2PTR ){
    ///
    ///
    ///
    /// NOrdErode = NOrd;
    /// OptErode = 2;
    ///
    ///
    ///
    ZeroMemory( Hist, 256 * sizeof(double) );
    ///
    ///
    ///
    ///
    WPIC_OUT = WPIC_IN;
    HPIC_OUT = HPIC_IN;
    NPosBuff = WPIC_OUT * HPIC_OUT;
    NBytesBuff = 4  * WPIC_OUT * HPIC_OUT;
    D2PTR->ReadDatas();
    if( D2PTR->ProcType==BW_LABEL ){
        for( int i=0; i<(int)NPosBuff; i++ ){
            D2PTR->RgbBuff[i].rgbRed   =
            D2PTR->RgbBuff[i].rgbGreen =
            D2PTR->RgbBuff[i].rgbBlue  = 255 * (int)((D2PTR->RgbBuff[i].rgbRed!=255) || (D2PTR->RgbBuff[i].rgbGreen!=255) ||
                                                        (D2PTR->RgbBuff[i].rgbBlue!=255));
        }
    }
    if( !IsGray ){
        IsGray = TRUE;
        for( int i=0; i<(int)NPosBuff; i++ ){ /// 0.2989 * R + 0.5870 * G + 0.1140 * B
            D2PTR->RgbBuff[i].rgbRed =
            D2PTR->RgbBuff[i].rgbGreen =
            D2PTR->RgbBuff[i].rgbBlue =
                (BYTE)round(
                            0.2989 * D2PTR->RgbBuff[i].rgbRed +
                            0.5870 * D2PTR->RgbBuff[i].rgbGreen +
                            0.1140 * D2PTR->RgbBuff[i].rgbBlue
                            );
            Hist[D2PTR->RgbBuff[i].rgbRed] = Hist[D2PTR->RgbBuff[i].rgbRed] + 1;
        }
        graythresh();
        int GVALI = (int)round( 255*GrayValue );
        for( int i=0; i<(int)NPosBuff; i++ ){
            D2PTR->RgbBuff[i].rgbRed = 255 * (int)( D2PTR->RgbBuff[i].rgbRed>GVALI );
        }
    }else{
        if( !IsBin ){
            for( int i=0; i<(int)NPosBuff; i++ ){
                Hist[D2PTR->RgbBuff[i].rgbRed] = Hist[D2PTR->RgbBuff[i].rgbRed] + 1;
            }
            graythresh();
            int GVALI = (int)round( 255*GrayValue );
            for( int i=0; i<(int)NPosBuff; i++ ){
                D2PTR->RgbBuff[i].rgbRed = 255 * (int)( D2PTR->RgbBuff[i].rgbRed>GVALI );
            }
        }
    }
    ///
    ///
    ///
    ///
    /// NOrdErode = (int)round( 0.02 * max( WPIC_OUT, HPIC_OUT ) );
    int MaxWH = max( WPIC_OUT+2*NOrdErode, HPIC_OUT+2*NOrdErode );
    int ResEQ4 = MaxWH;
    MaxWH = 1<<nextpow2( MaxWH );
    int WH_NE = 3500;/// 4096 - 5*NOrdErode;
    if( MaxWH>WH_NE || ResEQ4>=WH_NE ){
        int W_RED = 0;
        int H_RED = 0;
        FreeImage();
        double FactWH = 1;
        double FactNO = 1;
        if( WPIC_OUT>HPIC_OUT ){
            FactWH = ( (double)(HPIC_OUT)/((double)(WPIC_OUT)) );
            W_RED = WH_NE;
            H_RED = (int)round( W_RED * FactWH );
            FactNO = (WH_NE)/((double)WPIC_OUT);
        }else{
            FactWH = ( (double)(WPIC_OUT)/((double)(HPIC_OUT)) );
            H_RED = WH_NE;
            W_RED = (int)round( H_RED * FactWH );
            FactNO = (WH_NE)/((double)HPIC_OUT);
        }
        FactNO = round( FactNO * ((double)(NOrdErode)) );
        /// FactNO = round((double)NOrdErode);
        //FactNO = round(NOrdErode);
        if( FactNO<1 ){
            FactNO = 1;
        }
        DeleteObject( HBMP_OUT );
        /// HBMP_OUT = NewBitmap( WPIC_OUT, HPIC_OUT );
        ///
        ///
        HDC HDCN = GetDC( NULL );
        HBMP_OUT = NULL;
        HBMP_OUT = CreateCompatibleBitmap( HDCN, WPIC_OUT, HPIC_OUT );
        ReleaseDC( NULL, HDCN );
        ///
        ///
        DWORD GLE = GetLastError();
        if( HBMP_OUT==NULL ){
            exit( GLE );
        }
        SetBitmapBits( HBMP_OUT, NBytesBuff, D2PTR->RgbBuff );
        ///
        HBITMAP HBRED = Redim_PIC( HBMP_OUT, W_RED, H_RED );
        if( HBRED==NULL ){
            exit( -2 );
        }
        /// DeleteObject( HBMP_OUT );
        //*
        FreeBuff();
        RgbBuff = (RGBQUAD*)malloc( 4*W_RED*H_RED + 1024 );
        GetBitmapBits( HBRED, 4*W_RED*H_RED, RgbBuff );
        DSP_MNG.imdilate( RgbBuff, W_RED, H_RED, Hist, (int)(FactNO), OptErode, BFront );
        ///
        SetBitmapBits( HBRED, 4*W_RED*H_RED, RgbBuff );
        FreeBuff();
        ///
        DeleteObject( HBMP_OUT );
        HBMP_OUT = Redim_PIC( HBRED, WPIC_OUT, HPIC_OUT );
        ///
        /*
        HDC hdc = CreateCompatibleDC( GetDC( 0 ) );
        HDC memDC = CreateCompatibleDC( GetDC( 0 ) );
        SelectObject( hdc , HBRED );/// HBPLOT
        SelectObject( memDC , HBMP_OUT );/// HB_I
        SetStretchBltMode( memDC , HALFTONE );/// COLORONCOLOR  HALFTONE
        StretchBlt( memDC, 0, 0, WPIC_OUT, HPIC_OUT, hdc, 0, 0, W_RED, H_RED, SRCCOPY );
        DeleteDC( hdc );
        DeleteDC( memDC );
        //*/
        DeleteObject( HBRED );
        //*/
        ///
        GetBitmapBits( HBMP_OUT, NBytesBuff, D2PTR->RgbBuff );
        ///
    }else{
        /*
        Corrección al lado negro 1024x1024
        asdasdasd;
        WH
        //*/
        /// MaxWH
        int WC2_2 = (1<<nextpow2( max(WPIC_OUT+NOrdErode, HPIC_OUT+NOrdErode) ));
        int WC2   = WC2_2;/// (1<<nextpow2( max(WPIC_OUT, HPIC_OUT) ));
        if( WC2!=WC2_2 ){
            RGBQUAD* RgbTmp = (RGBQUAD*)malloc( 4 * (WC2_2*WC2_2) );
            ZeroMemory( RgbTmp, 4 * (WC2_2*WC2_2) );
            for( int y=0; y<HPIC_OUT; y++ ){
                memcpy( &RgbTmp[y*WC2_2], &D2PTR->RgbBuff[y*WPIC_OUT], 4*WPIC_OUT );
            }
            DSP_MNG.imdilate( RgbTmp, WC2_2, WC2_2, Hist, NOrdErode, OptErode, BFront );
            for( int y=0; y<HPIC_OUT; y++ ){
                memcpy( &D2PTR->RgbBuff[y*WPIC_OUT], &RgbTmp[y*WC2_2], 4*WPIC_OUT );
            }
            free( RgbTmp );
        }else{
            DSP_MNG.imdilate( D2PTR->RgbBuff, WPIC_OUT, HPIC_OUT, Hist, NOrdErode, OptErode, BFront );
        }
        ///
        ///
        ///
        FreeImage();
        HBMP_OUT = NewBitmap( WPIC_OUT, HPIC_OUT );
        SetBitmapBits( HBMP_OUT, NBytesBuff, D2PTR->RgbBuff );
    }
    ///
    graythresh();
    ///
    ///
    ///
    ///
    ///
    WriteDatas( D2PTR->RgbBuff, WPIC_OUT, HPIC_OUT );
    ///
    ///
    D2PTR->FreeBuff();
    IsGray = TRUE;
    IsBin = TRUE;
    ///
    ///
    return TRUE;
}
