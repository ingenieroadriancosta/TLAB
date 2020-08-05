BOOL DIM2::ERODEPROC( DIM2* D2PTR ){
    ///
    RGBQUAD* RgbT = NULL;
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
    ///
    ///
    ///
    NPosBuff = WPIC_IN * HPIC_IN;
    NBytesBuff = 4  * WPIC_IN * HPIC_IN;
    ///
    ///
    ///
    ///
    WPIC_OUT = WPIC_IN;
    HPIC_OUT = HPIC_IN;
    ///
    ///
    ///
    ///
    ///
    ///
    D2PTR->FreeBuff();
    D2PTR->RgbBuff = (RGBQUAD*)malloc( 4 * (WPIC_IN+512) * (HPIC_IN+512) );
    D2PTR->ReadDatas( D2PTR->RgbBuff );
    ///
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
    const int NEADV = 2*NOrdErode;
    #define EroWFill 0
    ///
    ///
    ///
    ///
    ///
    if( !IsGray ){
        IsGray = TRUE;
        RgbT = &D2PTR->RgbBuff[0];
        for( int i=0; i<(int)NPosBuff; i++ ){ /// 0.2989 * R + 0.5870 * G + 0.1140 * B
            RgbT->rgbRed =
            RgbT->rgbGreen =
            RgbT->rgbBlue =
                (BYTE)round(
                            0.2989 * RgbT->rgbRed +
                            0.5870 * RgbT->rgbGreen +
                            0.1140 * RgbT->rgbBlue
                            );
            Hist[RgbT->rgbRed] = Hist[RgbT->rgbRed] + 1;
            RgbT++;
        }
        graythresh();
        int GVALI = (int)round( 255*GrayValue );
        for( int i=0; i<(int)NPosBuff; i++ ){
            D2PTR->RgbBuff[i].rgbRed = 255 * (int)( D2PTR->RgbBuff[i].rgbRed>GVALI );
        }
    }else{
        if( !IsBin ){
            RgbT = &D2PTR->RgbBuff[0];
            for( int i=0; i<(int)NPosBuff; i++ ){
                Hist[RgbT->rgbRed] = Hist[RgbT->rgbRed] + 1;
                RgbT++;
            }
            graythresh();
            int GVALI = (int)round( 255*GrayValue );
            RgbT = &D2PTR->RgbBuff[0];
            for( int i=0; i<(int)NPosBuff; i++ ){
                RgbT->rgbRed = 255 * (int)( RgbT->rgbRed>GVALI );
                RgbT++;
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
        WPIC_OUT = WPIC_IN;
        HPIC_OUT = HPIC_IN;
        int W_RED = 0;
        int H_RED = 0;
        FreeImage();
        double FactNO = 1;
        double FactWH = 0;
        if( WPIC_OUT>HPIC_OUT ){
            FactNO = ( (double)(HPIC_OUT+2*NOrdErode)/((double)(WPIC_OUT+2*NOrdErode)) );
            FactWH = ( (double)(HPIC_OUT)/((double)(WPIC_OUT)) );
            W_RED = WH_NE;///4096 - 2*NOrdErode;
            H_RED = (int)round( W_RED * FactWH );
            FactNO = WH_NE/((double)WPIC_OUT);
        }else{
            FactNO = ( (double)(WPIC_OUT+2*NOrdErode)/((double)(HPIC_OUT+2*NOrdErode)) );
            FactWH = ( (double)(WPIC_OUT)/((double)(HPIC_OUT)) );
            H_RED = WH_NE;///4096 - 2*NOrdErode;
            W_RED = (int)round( H_RED * FactWH );
            FactNO = WH_NE/((double)HPIC_OUT);
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
        //*
        FreeBuff();
        RgbBuff = (RGBQUAD*)malloc( 4*(W_RED+4*(int)FactNO)*(H_RED+4*(int)FactNO) + 1024 );
        RGBQUAD* RgbY = (RGBQUAD*)malloc( 4*(W_RED+4*(int)FactNO)*(H_RED+4*(int)FactNO) + 1024 );
        memset( RgbY, 255, 4*(W_RED+4*(int)FactNO)*(H_RED+4*(int)FactNO) + 1023 );
        GetBitmapBits( HBRED, 4*W_RED*H_RED, &RgbBuff[0] );
        ///
        ///
        for( int y=0; y<H_RED; y++ ){
            memcpy( &RgbY[(y+(int)FactNO)*(W_RED+2*(int)FactNO) + (int)FactNO], &RgbBuff[y*W_RED], 4*W_RED );
        }
        free( RgbBuff );
        RgbBuff = RgbY;
        ///
        ///
        DSP_MNG.imerode( RgbBuff, (W_RED+2*(int)FactNO), (H_RED+2*(int)FactNO), Hist, (int)(FactNO), OptErode, BFront );
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
        SetStretchBltMode( memDC , HALFTONE|COLORONCOLOR );/// COLORONCOLOR  HALFTONE
        StretchBlt( memDC, 0, 0, WPIC_OUT, HPIC_OUT, hdc, 0, 0, W_RED, H_RED, SRCCOPY );
        DeleteDC( hdc );
        DeleteDC( memDC );
        //*/
        DeleteObject( HBRED );
        ///
        ///
        ///
        NBytesBuff = 4 * WPIC_OUT * HPIC_OUT;
        GetBitmapBits( HBMP_OUT, NBytesBuff, D2PTR->RgbBuff );
        ///
        ///
        ///
        ///
        ///
        ///
        ///
    }else{
        /*
        Corrección al lado negro 1024x1024
        asdasdasd;
        WH
        //*/

    #ifdef EroWFill
        NPosBuff = WPIC_IN * HPIC_IN;
        NBytesBuff = 4  * WPIC_IN * HPIC_IN;
        WPIC_OUT = WPIC_IN + 2*NEADV;
        HPIC_OUT = HPIC_IN + 2*NEADV;
        RgbT = (RGBQUAD*)malloc( 4 * WPIC_OUT * HPIC_OUT + 16 );
        memset( RgbT, 255, 4 * WPIC_OUT * HPIC_OUT );
        ///
        for( int y = 0; y<HPIC_IN; y++ ){
            memcpy( &RgbT[(y+NEADV)*WPIC_OUT+NEADV], &(D2PTR->RgbBuff[y*WPIC_IN]), 4*WPIC_IN);
        }
        ///
        D2PTR->FreeBuff();
        D2PTR->RgbBuff = RgbT;
        ///
        ///free( RgbT );
    #endif // EroWFill

        /// MaxWH
        int WC2_2 = (1<<nextpow2( max(WPIC_OUT+NOrdErode, HPIC_OUT+NOrdErode) ));
        int WC2   = WC2_2;/// (1<<nextpow2( max(WPIC_OUT, HPIC_OUT) ));
        if( WC2!=WC2_2 ){
            RGBQUAD* RgbTmp = (RGBQUAD*)malloc( 4 * (WC2_2*WC2_2) );
            ZeroMemory( RgbTmp, 4 * (WC2_2*WC2_2) );
            for( int y=0; y<HPIC_OUT; y++ ){
                memcpy( &RgbTmp[y*WC2_2], &D2PTR->RgbBuff[y*WPIC_OUT], 4*WPIC_OUT );
            }
            DSP_MNG.imerode( RgbTmp, WC2_2, WC2_2, Hist, NOrdErode, OptErode, BFront );
            for( int y=0; y<HPIC_OUT; y++ ){
                memcpy( &D2PTR->RgbBuff[y*WPIC_OUT], &RgbTmp[y*WC2_2], 4*WPIC_OUT );
            }
            free( RgbTmp );
        }else{
            DSP_MNG.imerode( D2PTR->RgbBuff, WPIC_OUT, HPIC_OUT, Hist, NOrdErode, OptErode, BFront );
        }
        ///
        ///
        ///
        FreeImage();
        ///
        ///
        ///
        #ifdef EroWFill
            for( int y = 0; y<HPIC_IN; y++ ){
                memcpy( &(D2PTR->RgbBuff[y*WPIC_IN]), &D2PTR->RgbBuff[(y+NEADV)*WPIC_OUT+NEADV], 4*WPIC_IN);
            }
            WPIC_OUT = WPIC_IN;
            HPIC_OUT = HPIC_IN;
        #endif // EroWFill
        ///
        ///
        NBytesBuff = 4 * WPIC_OUT * HPIC_OUT;
        ///
        ///
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
