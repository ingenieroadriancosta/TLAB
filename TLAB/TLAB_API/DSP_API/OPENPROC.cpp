BOOL DIM2::OPENPROC( DIM2* D2PTR ){
    ///
    ///
    RGBQUAD* RgbT = NULL;
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
    ///
    ///
    ///
    ///
    ///
    ///
    #define OpenWFill 0
    ///
    ///
    ///
    ///
    ///
    ///
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
            D2PTR->RgbBuff[i].rgbRed = 255 * (int)(D2PTR->RgbBuff[i].rgbRed>GVALI);
        }
    }else{
        if( !IsBin ){
            for( int i=0; i<(int)NPosBuff; i++ ){
                Hist[D2PTR->RgbBuff[i].rgbRed] = Hist[D2PTR->RgbBuff[i].rgbRed] + 1;
            }
            graythresh();
            int GVALI = (int)round( 255*GrayValue );
            for( int i=0; i<(int)NPosBuff; i++ ){
                D2PTR->RgbBuff[i].rgbRed = 255 * (int)(D2PTR->RgbBuff[i].rgbRed>GVALI);
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
        double FactNO = 0;
        double FactWH = 0;
        if( WPIC_OUT>HPIC_OUT ){
            FactWH = ( (double)(HPIC_OUT)/((double)(WPIC_OUT)) );
            W_RED = WH_NE;
            H_RED = (int)round( W_RED * FactWH );
            FactNO = WH_NE/( (double)WPIC_OUT );
        }else{
            FactWH = ( (double)(WPIC_OUT)/((double)(HPIC_OUT)) );
            H_RED = WH_NE;
            W_RED = (int)round( H_RED * FactWH );
            FactNO = WH_NE/( (double)HPIC_OUT );
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
        ///
        //*
        FreeBuff();
        ///
        RgbBuff = (RGBQUAD*)malloc( 4*(W_RED+2*(int)FactNO)*(H_RED+2*(int)FactNO) + 1024 );
        RGBQUAD* RgbY = (RGBQUAD*)malloc( 4*(W_RED+2*(int)FactNO)*(H_RED+2*(int)FactNO) + 1024 );
        memset( RgbY, 255, 4*(W_RED+2*(int)FactNO)*(H_RED+2*(int)FactNO) + 1023 );
        GetBitmapBits( HBRED, 4*W_RED*H_RED, RgbBuff );
        ///
        ///
        ///
        for( int y=0; y<H_RED; y++ ){
            memcpy( &RgbY[(y+(int)FactNO)*(W_RED+2*(int)FactNO) + (int)FactNO], &RgbBuff[y*W_RED], 4*W_RED );
        }
        free( RgbBuff );
        RgbBuff = RgbY;
        ///
        ///
        DSP_MNG.imopen( RgbBuff, (W_RED+2*(int)FactNO), (H_RED+2*(int)FactNO), Hist, (int)(FactNO), OptErode, BFront );
        ///
        ///
        ///
        ///
        RGBQUAD* RgbDes = NULL;
        for( int y=0; y<H_RED; y++ ){
            RgbY = &RgbBuff[(y+(int)FactNO)*(W_RED+2*(int)FactNO) + (int)FactNO];
            RgbDes = &RgbBuff[y*W_RED];
            for( int x=0; x<W_RED; x++ ){
                if( RgbY->rgbRed>240 ){
                    RgbDes[0] = WHITECOLOR;
                }else{
                    RgbDes[0] = BLACKCOLOR;
                }
                RgbY++;
                RgbDes++;
                ///memcpy( &RgbBuff[y*W_RED], &RgbBuff[(y+(int)FactNO)*W_RED + (int)FactNO], 4*W_RED );
            }
        }
        ///
        ///
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
        ///GetBitmapBits( HBMP_OUT, NBytesBuff, D2PTR->RgbBuff );
        ///
        NBytesBuff = 4 * WPIC_OUT * HPIC_OUT;
        GetBitmapBits( HBMP_OUT, NBytesBuff, D2PTR->RgbBuff );
        ///
            NBytesBuff = 4 * WPIC_OUT * HPIC_OUT;
            ///DeleteObject( HBMP_OUT );
            ///HBMP_OUT = NewBitmap( WPIC_OUT, HPIC_OUT );
            ///SetBitmapBits( HBMP_OUT, NBytesBuff, D2PTR->RgbBuff );
        ///
    }else{
        /*
        Corrección al lado negro 1024x1024
        asdasdasd;
        WH
        //*/
    #ifdef OpenWFill
        NPosBuff = WPIC_IN * HPIC_IN;
        NBytesBuff = 4  * WPIC_IN * HPIC_IN;
        WPIC_OUT = WPIC_IN + 2*NOrdErode;
        HPIC_OUT = HPIC_IN + 2*NOrdErode;
        RgbT = (RGBQUAD*)malloc( 4 * WPIC_OUT * HPIC_OUT );
        memset( RgbT, 255, 4 * WPIC_OUT * HPIC_OUT );
        ///
        for( int y = 0; y<HPIC_IN; y++ ){
            memcpy( &RgbT[(y+NOrdErode)*WPIC_OUT+NOrdErode], &(D2PTR->RgbBuff[y*WPIC_IN]), 4*WPIC_IN);
        }
        ///
        D2PTR->FreeBuff();
        D2PTR->RgbBuff = RgbT;
        ///
        ///free( RgbT );
    #endif // OpenWFill
        /// MaxWH
        int WC2_2 = (1<<nextpow2( max(WPIC_OUT+NOrdErode, HPIC_OUT+NOrdErode) ));
        int WC2   = WC2_2;/// (1<<nextpow2( max(WPIC_OUT, HPIC_OUT) ));
        if( WC2!=WC2_2 ){
            RGBQUAD* RgbTmp = (RGBQUAD*)malloc( 4 * (WC2_2*WC2_2) );
            ZeroMemory( RgbTmp, 4 * (WC2_2*WC2_2) );
            for( int y=0; y<HPIC_OUT; y++ ){
                memcpy( &RgbTmp[y*WC2_2], &D2PTR->RgbBuff[y*WPIC_OUT], 4*WPIC_OUT );
            }
            DSP_MNG.imopen( RgbTmp, WC2_2, WC2_2, Hist, NOrdErode, OptErode, BFront );
            for( int y=0; y<HPIC_OUT; y++ ){
                memcpy( &D2PTR->RgbBuff[y*WPIC_OUT], &RgbTmp[y*WC2_2], 4*WPIC_OUT );
            }
            free( RgbTmp );
        }else{
            DSP_MNG.imopen( D2PTR->RgbBuff, WPIC_OUT, HPIC_OUT, Hist, NOrdErode, OptErode, BFront );
        }
        ///
        ///
        ///
        #ifdef OpenWFill
            for( int y = 0; y<HPIC_IN; y++ ){
                memcpy( &(D2PTR->RgbBuff[y*WPIC_IN]), &D2PTR->RgbBuff[(y+NOrdErode)*WPIC_OUT+NOrdErode], 4*WPIC_IN);
            }
            WPIC_OUT = WPIC_IN;
            HPIC_OUT = HPIC_IN;
        #endif // OpenWFill
        ///
        ///
        NBytesBuff = 4 * WPIC_OUT * HPIC_OUT;
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
