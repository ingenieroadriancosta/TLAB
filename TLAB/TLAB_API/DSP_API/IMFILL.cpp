int DIM2::IMFILL( RGBQUAD* RgbIO, int W_W, int H_H, BYTE BOptFill ){
    ///
    ///
    int W_P  = W_W + 4;
    int H_P  = H_H + 4;
    int WH_P = W_P * H_P;
    ///
    int x    = 0;
    int y    = 0;
    ///
    RGBQUAD* RgbTIN = NULL;
    RGBQUAD* RgbT = NULL;
    ///
    int* I_RgbT = NULL;
    ///
    RGBQUAD* Rgb2Cnt = (RGBQUAD*)malloc( 4 * WH_P + 1024 );
    memset( Rgb2Cnt, 255, 4 * WH_P + 1023 );
    ///
    for( y=0; y<H_H; y++ ){
        RgbTIN = &RgbIO[y*W_W];
        RgbT   = &Rgb2Cnt[(y+1)*W_P+1];
        for( x=0; x<W_W; x++ ){
            if( RgbTIN->rgbRed>0 ){
                *RgbT = BLACKCOLOR;
            }else{
                *RgbT = WHITECOLOR;
            }
            /// RgbT->rgbRed = ~RgbTIN->rgbRed;
            RgbTIN++;
            RgbT++;
        }
    }
    ///
    NObjBWL = BWLABEL( Rgb2Cnt, W_P, H_P );
    ///
    int Rgb0 = 0;
    memcpy( &Rgb0, &Rgb2Cnt[0], 4 );
    ///
    int* I_W_COL = (int*)&WHITECOLOR;
    ///
    BOOL BFL = FALSE;
    /// BOptFill = 0;
    if( BOptFill==0 ){
        for( y=0; y<H_H; y++ ){
            RgbTIN = &RgbIO[y*W_W];
            I_RgbT   = (int*)&Rgb2Cnt[(y+1)*W_P+1];
            for( x=0; x<W_W; x++ ){
                ///
                BFL = ( ( ( (I_RgbT[0])!=I_W_COL[0]) && ((I_RgbT[0])!=Rgb0) ) || RgbTIN->rgbRed>0 );
                /// BFL = ( ( ( (I_RgbT[0])!=I_W_COL[0]) && ((I_RgbT[0])!=Rgb0) ) || RgbTIN->rgbRed<1 );
                /// BFL = ( ( ( (I_RgbT[0])!=I_W_COL[0]) && ((I_RgbT[0])!=Rgb0) )  ) && RgbTIN->rgbRed<1;
                if( BFL ){
                    *RgbTIN = WHITECOLOR;
                }else{
                    *RgbTIN = BLACKCOLOR;
                }
                ///
                ///
                RgbTIN++;
                I_RgbT++;
            }
        }
    }else{
        for( y=0; y<H_H; y++ ){
            RgbTIN = &RgbIO[y*W_W];
            I_RgbT   = (int*)&Rgb2Cnt[(y+1)*W_P+1];
            for( x=0; x<W_W; x++ ){
                ///
                BFL = ( ( ( (I_RgbT[0])!=I_W_COL[0]) && ((I_RgbT[0])!=Rgb0) )  ) && RgbTIN->rgbRed<1;
                if( BFL ){
                    *RgbTIN = WHITECOLOR;
                }else{
                    *RgbTIN = BLACKCOLOR;
                }
                ///
                ///
                RgbTIN++;
                I_RgbT++;
            }
        }
    }
    ///
    ///
    ///
    ///
    ///
    ///
    free( Rgb2Cnt );
    ///
    return 1;
}
