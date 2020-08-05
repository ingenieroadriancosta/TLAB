BOOL SETKIRSH( RGBQUAD* Rgb_InPtr, RGBQUAD* RgbDest, int W_P, int H_P, int Offset ){
    ///
    const int K_1[9] = {  5,  -3,  -3,   5,   0,  -3,   5,  -3,  -3, };
    const int K_2[9] = {  5,   5,  -3,   5,   0,  -3,  -3,  -3,  -3, };
    const int K_3[9] = {  5,   5,   5,  -3,   0,  -3,  -3,  -3,  -3, };
    const int K_4[9] = { -3,   5,   5,  -3,   0,   5,  -3,  -3,  -3, };
    const int K_5[9] = { -3,  -3,   5,  -3,   0,   5,  -3,  -3,   5, };
    const int K_6[9] = { -3,  -3,  -3,  -3,   0,   5,  -3,   5,   5, };
    const int K_7[9] = { -3,  -3,  -3,  -3,   0,  -3,   5,   5,   5, };
    const int K_8[9] = { -3,  -3,  -3,   5,   0,  -3,   5,   5,  -3, };
    ///
    int W_P_2 = W_P - 2;
    int H_P_2 = H_P - 2;
    ///
    RGBQUAD* RgbT_00 = NULL;
    RGBQUAD* RgbT_01 = NULL;
    RGBQUAD* RgbT_02 = NULL;
    //
    RGBQUAD* RgbT_10 = NULL;
    RGBQUAD* RgbT_11 = NULL;
    RGBQUAD* RgbT_12 = NULL;
    //
    RGBQUAD* RgbT_20 = NULL;
    RGBQUAD* RgbT_21 = NULL;
    RGBQUAD* RgbT_22 = NULL;
    ///
    RGBQUAD* RgbTOut = NULL;
    ///
    int x = 0;
    int y = 0;
    int ISum  = 0;
    for( y=0; y<H_P_2; y++ ){
        ///
        ///
        ///
        RgbT_00    = &Rgb_InPtr[y*W_P];
        RgbT_01    = &Rgb_InPtr[y*W_P+1];
        RgbT_02    = &Rgb_InPtr[y*W_P+2];
        //
        RgbT_10    = &Rgb_InPtr[(y+1)*W_P];
        RgbT_11    = &Rgb_InPtr[(y+1)*W_P+1];
        RgbT_12    = &Rgb_InPtr[(y+1)*W_P+2];
        //
        RgbT_20    = &Rgb_InPtr[(y+2)*W_P];
        RgbT_21    = &Rgb_InPtr[(y+2)*W_P+1];
        RgbT_22    = &Rgb_InPtr[(y+2)*W_P+2];
        //
        RgbTOut = &RgbDest[(y+1)*W_P+1];
        for( x=0; x<W_P_2; x++ ){
            ///
            ISum =  RgbT_00->rgbRed * K_1[0] + RgbT_01->rgbRed * K_1[1] + RgbT_02->rgbRed * K_1[2] +
                    RgbT_10->rgbRed * K_1[3] + RgbT_11->rgbRed * K_1[4] + RgbT_12->rgbRed * K_1[5] +
                    RgbT_20->rgbRed * K_1[6] + RgbT_21->rgbRed * K_1[7] + RgbT_22->rgbRed * K_1[8];
            ///
            ISum = max( ISum,
                        RgbT_00->rgbRed * K_2[0] + RgbT_01->rgbRed * K_2[1] + RgbT_02->rgbRed * K_2[2] +
                        RgbT_10->rgbRed * K_2[3] + RgbT_11->rgbRed * K_2[4] + RgbT_12->rgbRed * K_2[5] +
                        RgbT_20->rgbRed * K_2[6] + RgbT_21->rgbRed * K_2[7] + RgbT_22->rgbRed * K_2[8]
                       );
            ///
            ISum = max( ISum,
                        RgbT_00->rgbRed * K_3[0] + RgbT_01->rgbRed * K_3[1] + RgbT_02->rgbRed * K_3[2] +
                        RgbT_10->rgbRed * K_3[3] + RgbT_11->rgbRed * K_3[4] + RgbT_12->rgbRed * K_3[5] +
                        RgbT_20->rgbRed * K_3[6] + RgbT_21->rgbRed * K_3[7] + RgbT_22->rgbRed * K_3[8]
                       );
            ///
            ISum = max( ISum,
                        RgbT_00->rgbRed * K_4[0] + RgbT_01->rgbRed * K_4[1] + RgbT_02->rgbRed * K_4[2] +
                        RgbT_10->rgbRed * K_4[3] + RgbT_11->rgbRed * K_4[4] + RgbT_12->rgbRed * K_4[5] +
                        RgbT_20->rgbRed * K_4[6] + RgbT_21->rgbRed * K_4[7] + RgbT_22->rgbRed * K_4[8]
                       );
            ///
            ISum = max( ISum,
                        RgbT_00->rgbRed * K_5[0] + RgbT_01->rgbRed * K_5[1] + RgbT_02->rgbRed * K_5[2] +
                        RgbT_10->rgbRed * K_5[3] + RgbT_11->rgbRed * K_5[4] + RgbT_12->rgbRed * K_5[5] +
                        RgbT_20->rgbRed * K_5[6] + RgbT_21->rgbRed * K_5[7] + RgbT_22->rgbRed * K_5[8]
                       );
            ///
            ISum = max( ISum,
                        RgbT_00->rgbRed * K_6[0] + RgbT_01->rgbRed * K_6[1] + RgbT_02->rgbRed * K_6[2] +
                        RgbT_10->rgbRed * K_6[3] + RgbT_11->rgbRed * K_6[4] + RgbT_12->rgbRed * K_6[5] +
                        RgbT_20->rgbRed * K_6[6] + RgbT_21->rgbRed * K_6[7] + RgbT_22->rgbRed * K_6[8]
                       );
            ///
            ISum = max( ISum,
                        RgbT_00->rgbRed * K_7[0] + RgbT_01->rgbRed * K_7[1] + RgbT_02->rgbRed * K_7[2] +
                        RgbT_10->rgbRed * K_7[3] + RgbT_11->rgbRed * K_7[4] + RgbT_12->rgbRed * K_7[5] +
                        RgbT_20->rgbRed * K_7[6] + RgbT_21->rgbRed * K_7[7] + RgbT_22->rgbRed * K_7[8]
                       );
            ///
            ISum = max( ISum,
                        RgbT_00->rgbRed * K_8[0] + RgbT_01->rgbRed * K_8[1] + RgbT_02->rgbRed * K_8[2] +
                        RgbT_10->rgbRed * K_8[3] + RgbT_11->rgbRed * K_8[4] + RgbT_12->rgbRed * K_8[5] +
                        RgbT_20->rgbRed * K_8[6] + RgbT_21->rgbRed * K_8[7] + RgbT_22->rgbRed * K_8[8]
                       );
            ///
            //
            RgbTOut->rgbRed = RgbTOut->rgbGreen = RgbTOut->rgbBlue = ParseByte(ISum+Offset);
            //
            RgbT_00++;RgbT_01++;RgbT_02++;
            RgbT_10++;RgbT_11++;RgbT_12++;
            RgbT_20++;RgbT_21++;RgbT_22++;
            RgbTOut++;
        }
    }
    ///
    return TRUE;
}
