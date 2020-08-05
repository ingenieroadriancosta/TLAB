BOOL DIM2::AVERAGEPROC( DIM2* D2PTR ){
    ///
    int MaxWH = max( WPIC_OUT+2*NOrd, HPIC_OUT+2*NOrd );
    int MWH = MaxWH;
    MaxWH = 1<<nextpow2( MaxWH );
    int WH_NE = 4096 - 4*NOrd;
    if( MaxWH>4096 || MWH>3700 ){
        int W_RED = 0;
        int H_RED = 0;
        FreeImage();
        double FactNO = 0;
        double FactWH = 0;
        if( WPIC_OUT>HPIC_OUT ){
            FactWH = ( (double)(HPIC_OUT)/((double)(WPIC_OUT)) );
            W_RED = WH_NE;
            H_RED = (int)round( W_RED * FactWH );
            if( H_RED<1 ){
                H_RED = 1;
            }
            FactNO = WH_NE/((double)WPIC_OUT);
        }else{
            FactWH = ( (double)(WPIC_OUT)/((double)(HPIC_OUT)) );
            H_RED = WH_NE;
            W_RED = (int)round( H_RED * FactWH );
            if( W_RED<1 ){
                W_RED = 1;
            }
            FactNO = WH_NE/((double)HPIC_OUT);
        }
        FactNO = round( FactNO * ((double)NOrd) );
        ///
        ///
        if( FactNO<1 ){
            FactNO = 1;
        }
        DeleteObject( HBMP_OUT );
        ///
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
        ///
        HBITMAP HBRED = Redim_PIC( HBMP_OUT, W_RED, H_RED );
        if( HBRED==NULL ){
            exit( -2 );
        }
        ///
        ///
        ///
        FreeBuff();
        ///
        RgbBuff = (RGBQUAD*)malloc( 4*(W_RED+4*(int)FactNO)*(H_RED+4*(int)FactNO) + 1024 );
        RGBQUAD* RgbY = (RGBQUAD*)malloc( 4*(W_RED+4*(int)FactNO)*(H_RED+4*(int)FactNO) + 1024 );
        memset( RgbY, 0, 4*(W_RED+4*(int)FactNO)*(H_RED+4*(int)FactNO) + 1023 );
        GetBitmapBits( HBRED, 4*W_RED*H_RED, &RgbBuff[0] );
        ///
        int FNI = (int)FactNO;
        int W_FN = W_RED+2*FNI;
        for( int y=0; y<H_RED; y++ ){
            memcpy( &RgbY[(y+FNI)*(W_FN) + FNI], &RgbBuff[y*W_RED], 4*W_RED );
        }
        free( RgbBuff );
        RgbBuff = RgbY;
        ///
        ///
        ///
        ///
        ///
        DSP_MNG.FSPECIAL_AVERAGE( RgbBuff, (W_RED+2*FNI), (H_RED+2*FNI), (int)(FactNO), Kernel3x3_Offset[Kernel3x3_Type] );
        ///
        ///
        ///
        for( int y=0; y<H_RED; y++ ){
            memcpy( &RgbBuff[y*W_RED], &RgbBuff[(y+FNI)*(W_FN) + FNI], 4*W_RED );
        }
        ///
        ///
        SetBitmapBits( HBRED, 4*W_RED*H_RED, RgbBuff );
        FreeBuff();
        ///
        DeleteObject( HBMP_OUT );
        HBMP_OUT = Redim_PIC( HBRED, WPIC_OUT, HPIC_OUT );
        ///
        DeleteObject( HBRED );
        ///
        GetBitmapBits( HBMP_OUT, NBytesBuff, D2PTR->RgbBuff );
        ///
    }else{
    /// ///////////////////////////////////////////////////////////////////////////////////////////////////
    /// ///////////////////////////////////////////////////////////////////////////////////////////////////
    /// ///////////////////////////////////////////////////////////////////////////////////////////////////
    /// ///////////////////////////////////////////////////////////////////////////////////////////////////
    /// ///////////////////////////////////////////////////////////////////////////////////////////////////
    /// ///////////////////////////////////////////////////////////////////////////////////////////////////
            const int NEADV = NOrd;
            NPosBuff = WPIC_IN * HPIC_IN;
            NBytesBuff = 4  * WPIC_IN * HPIC_IN;
            WPIC_OUT = WPIC_IN + 2*NEADV;
            HPIC_OUT = HPIC_IN + 2*NEADV;
            RGBQUAD* RgbT = (RGBQUAD*)malloc( 4 * WPIC_OUT * HPIC_OUT + 16 );
            memset( RgbT, 0, 4 * WPIC_OUT * HPIC_OUT );
            ///
            for( int y = 0; y<HPIC_IN; y++ ){
                memcpy( &RgbT[(y+NEADV)*WPIC_OUT+NEADV], &(D2PTR->RgbBuff[y*WPIC_IN]), 4*WPIC_IN);
            }
            ///
            D2PTR->FreeBuff();
            D2PTR->RgbBuff = RgbT;
            DSP_MNG.FSPECIAL_AVERAGE( D2PTR->RgbBuff, WPIC_OUT, HPIC_OUT, NOrd,
                                                Kernel3x3_Offset[Kernel3x3_Type] );
            for( int y = 0; y<HPIC_IN; y++ ){
                memcpy( &(D2PTR->RgbBuff[y*WPIC_IN]), &D2PTR->RgbBuff[(y+NEADV)*WPIC_OUT+NEADV], 4*WPIC_IN);
            }
            WPIC_OUT = WPIC_IN;
            HPIC_OUT = HPIC_IN;
            NBytesBuff = 4 * WPIC_OUT * HPIC_OUT;
            FreeImage();
            HBMP_OUT = NewBitmap( WPIC_OUT, HPIC_OUT );
            SetBitmapBits( HBMP_OUT, NBytesBuff, D2PTR->RgbBuff );
    /// ///////////////////////////////////////////////////////////////////////////////////////////////////
    /// ///////////////////////////////////////////////////////////////////////////////////////////////////
    /// ///////////////////////////////////////////////////////////////////////////////////////////////////
    /// ///////////////////////////////////////////////////////////////////////////////////////////////////
    /// ///////////////////////////////////////////////////////////////////////////////////////////////////
    /// ///////////////////////////////////////////////////////////////////////////////////////////////////
    }
    ///
    ///
    ///
    ///
    return TRUE;
}


