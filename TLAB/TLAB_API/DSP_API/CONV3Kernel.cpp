BOOL CONV3Kernel( RGBQUAD* RgbIO, RGBQUAD* RgbTmp, int W_P, int H_P, int* K3x3, int Offset ){
    ///
    int x = 0;
    int y = 0;
    int ISumH = 0;
    int W_PK = W_P + 3;
    ///
    RGBQUAD* RgbT_00 = NULL;    RGBQUAD* RgbT_01 = NULL;    RGBQUAD* RgbT_02 = NULL;
    RGBQUAD* RgbT_10 = NULL;    RGBQUAD* RgbT_11 = NULL;    RGBQUAD* RgbT_12 = NULL;
    RGBQUAD* RgbT_20 = NULL;    RGBQUAD* RgbT_21 = NULL;    RGBQUAD* RgbT_22 = NULL;
    ///
    RGBQUAD* RgbTOut = NULL;
    ///
    int KDiv = 0;
    ///
    for( int i=0; i<9; i++ ){
        KDiv = KDiv + K3x3[i];
    }
    ///
    if( KDiv==0 ){
        KDiv = 1;
    }
    ///
    double DKDiv = (double)KDiv;
    DKDiv = 1/DKDiv;
    ///
    ZeroMemory( RgbTmp, 4 * (W_P+6) * (H_P+6) );
    for( y=0; y<H_P; y++ ){
        memcpy( &RgbTmp[(y+1)*W_PK+1], &RgbIO[y*W_P], W_P * sizeof(RGBQUAD) );
    }
    ///
    ///
    ///
    ///
    for( y=0; y<H_P; y++ ){
        RgbT_00    = &RgbTmp[y*W_PK];
        RgbT_01    = &RgbTmp[y*W_PK+1];
        RgbT_02    = &RgbTmp[y*W_PK+2];
        //
        RgbT_10    = &RgbTmp[(y+1)*W_PK];
        RgbT_11    = &RgbTmp[(y+1)*W_PK+1];
        RgbT_12    = &RgbTmp[(y+1)*W_PK+2];
        //
        RgbT_20    = &RgbTmp[(y+2)*W_PK];
        RgbT_21    = &RgbTmp[(y+2)*W_PK+1];
        RgbT_22    = &RgbTmp[(y+2)*W_PK+2];
        //
        RgbTOut = &RgbIO[y*W_P];
        //
        for( x=0; x<W_P; x++ ){
            //
            //
            ISumH =  RgbT_00->rgbRed * K3x3[0] + RgbT_01->rgbRed * K3x3[1] + RgbT_02->rgbRed * K3x3[2] +
                     RgbT_10->rgbRed * K3x3[3] + RgbT_11->rgbRed * K3x3[4] + RgbT_12->rgbRed * K3x3[5] +
                     RgbT_20->rgbRed * K3x3[6] + RgbT_21->rgbRed * K3x3[7] + RgbT_22->rgbRed * K3x3[8];
            //
            //
            //
            RgbTOut->rgbRed   =
            RgbTOut->rgbGreen =
            RgbTOut->rgbBlue  = ParseByte( ((int)(round(ISumH * DKDiv))) + Offset );
            ///
            RgbT_00++;RgbT_01++;RgbT_02++;
            RgbT_10++;RgbT_11++;RgbT_12++;
            RgbT_20++;RgbT_21++;RgbT_22++;
            RgbTOut++;
            ///
        }
        //
    }
    ///
    ///
    ///
    ///
    ///
    ///
    ///
    return TRUE;
}
