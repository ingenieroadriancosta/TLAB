BOOL DIM2::IMFILLPROC( DIM2* D2PTR ){
    ///
    ZeroMemory( Hist, 256 * sizeof(double) );
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
    IMFILL( D2PTR->RgbBuff, WPIC_OUT, HPIC_OUT, BOptFill );
    ///
    ///
    ///
    FreeImage();
    HBMP_OUT = NewBitmap( WPIC_OUT, HPIC_OUT );
    SetBitmapBits( HBMP_OUT, NBytesBuff, D2PTR->RgbBuff );
    ///
    WriteDatas( D2PTR->RgbBuff, WPIC_OUT, HPIC_OUT );
    ///
    D2PTR->FreeBuff();
    IsGray = TRUE;
    IsBin = TRUE;
    ///
    return TRUE;
}
