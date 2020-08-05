double SOBEL_PROC( RGBQUAD* RgbIO, RGBQUAD* RgbTmp, int W_P, int H_P, int* K3x3H, int* K3x3V, int Offset ){
    ///
    int x = 0;
    int y = 0;
    int ISumH = 0;
    int ISumV = 0;
    int W_PK = W_P + 3;
    double Mean2 = 0;
    ///
    RGBQUAD* RgbT_00 = NULL;    RGBQUAD* RgbT_01 = NULL;    RGBQUAD* RgbT_02 = NULL;
    RGBQUAD* RgbT_10 = NULL;    RGBQUAD* RgbT_11 = NULL;    RGBQUAD* RgbT_12 = NULL;
    RGBQUAD* RgbT_20 = NULL;    RGBQUAD* RgbT_21 = NULL;    RGBQUAD* RgbT_22 = NULL;
    ///
    RGBQUAD* RgbTOut = NULL;
    ///
    int KDivHP2 = 0;
    int KDivH = 0;
    int KDivV = 0;
    int numel = W_P * H_P;
    double cutoff = 0;
    double thresh = 0;
    ///
    for( int i=0; i<9; i++ ){
        KDivH = KDivH + abs(K3x3H[i]);
        KDivV = KDivV + abs(K3x3V[i]);
    }
    KDivHP2 = KDivH * KDivH;
    ///
    ZeroMemory( RgbTmp, 4 * (W_P+6) * (H_P+6) );
    for( y=0; y<H_P; y++ ){
        memcpy( &RgbTmp[(y+1)*W_PK+1], &RgbIO[y*W_P], W_P * sizeof(RGBQUAD) );
    }
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
            ISumH =  RgbT_00->rgbRed * K3x3H[0] + RgbT_01->rgbRed * K3x3H[1] + RgbT_02->rgbRed * K3x3H[2] +
                     RgbT_10->rgbRed * K3x3H[3] + RgbT_11->rgbRed * K3x3H[4] + RgbT_12->rgbRed * K3x3H[5] +
                     RgbT_20->rgbRed * K3x3H[6] + RgbT_21->rgbRed * K3x3H[7] + RgbT_22->rgbRed * K3x3H[8];
            //
            ISumV =  RgbT_00->rgbRed * K3x3V[0] + RgbT_01->rgbRed * K3x3V[1] + RgbT_02->rgbRed * K3x3V[2] +
                     RgbT_10->rgbRed * K3x3V[3] + RgbT_11->rgbRed * K3x3V[4] + RgbT_12->rgbRed * K3x3V[5] +
                     RgbT_20->rgbRed * K3x3V[6] + RgbT_21->rgbRed * K3x3V[7] + RgbT_22->rgbRed * K3x3V[8];
            //
            //
            ///ISumH = ISumH/KDivH;
            ///ISumV = ISumV/KDivV;
            //
            //
            ((int*)RgbTOut)[0] = ( (( ISumH * ISumH + ISumV * ISumV ))/KDivHP2 );
            Mean2 = Mean2 + (double)( ((int*)RgbTOut)[0] );
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
    cutoff = Mean2/((double)( numel ));
    thresh = (4*cutoff);
    ///
    /*
    RgbTOut = &RgbIO[0];
    for( i=0; i<numel; i++ ){
        RgbTOut->rgbRed   =
        RgbTOut->rgbGreen =
        RgbTOut->rgbBlue  = 255 * (((int*)RgbTOut)[0]>thresh);
        RgbTOut++;
    }
    //*/
    ///
    ///
    ///
    ///
    ///
    return thresh;
}
