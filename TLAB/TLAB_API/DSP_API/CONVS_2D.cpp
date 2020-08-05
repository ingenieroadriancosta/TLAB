#include "SETKIRSH.cpp"
#include "SOBEL_PROC.cpp"
#include "CONV3Kernel.cpp"
#include "CONV3x3_TRY.cpp"
BOOL DIM2::CONV3x3( RGBQUAD* RgbIn, int W_P, int H_P, int* K3x3H, int* K3x3V, int KType, int Offset ){
    GetKernel( K3x3H, K3x3V, KType );
    const int TopXY = W_P * H_P;
    RGBQUAD* RgbDest = (RGBQUAD*)malloc( 4 * (W_P+6) * (H_P+6) + 1024 );
    ZeroMemory( RgbDest, 4 * (W_P+6) * (H_P+6) + 1023 );
    /// memcpy( RgbDest, RgbIn, 4 * W_P * H_P );/// OJO!!!
    ///
    RGBQUAD* Rgb_InPtr = RgbIn;
    ///
    int KDivH = 0;
    int KDivV = 0;
    int numel = W_P * H_P;
    for( int i=0; i<9; i++ ){
        KDivH = KDivH + K3x3H[i];
        KDivV = KDivV + K3x3V[i];
    }
    if( KDivH==0 ){
        KDivH = 1;
    }
    if( KDivV==0 ){
        KDivV = 1;
    }
    ///
    int W_P_2 = W_P - 2;
    int H_P_2 = H_P - 2;
    int x = 0;
    int y = 0;
    int ISum = 0;
    int ISumH = 0;
    int ISumV = 0;
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
    ///
    ///
    /*
    ISum =  Rgb_InPtr[0].rgbRed   * K3x3[4] +   Rgb_InPtr[1].rgbRed     * K3x3[5] +
            Rgb_InPtr[W_P].rgbRed * K3x3[7] +   Rgb_InPtr[W_P+1].rgbRed * K3x3[8];
    RgbDest[0].rgbRed   = ParseByte(ISum/KDiv+Offset);
    RgbDest[0].rgbGreen = ParseByte(ISum/KDiv+Offset);
    RgbDest[0].rgbBlue  = ParseByte(ISum/KDiv+Offset);
    for( x=1; x<W_P_2; x++ ){
        ISum =  Rgb_InPtr[x-1].rgbRed     * K3x3[3] +   Rgb_InPtr[x].rgbRed     * K3x3[4] + Rgb_InPtr[x+1].rgbRed     * K3x3[5] +
                Rgb_InPtr[W_P+x-1].rgbRed * K3x3[6] +   Rgb_InPtr[W_P+x].rgbRed * K3x3[7] + Rgb_InPtr[W_P+x+1].rgbRed * K3x3[8];
        RgbDest[x].rgbRed = RgbDest[x].rgbGreen = RgbDest[x].rgbBlue = ParseByte(ISum/KDiv+Offset);
    }
    //*/
    ///
    ///
    ///
    ///
    ///
    ///
    ///
    if( KType==BLUR_K || KType==EMBOSS || KType==SHARPEN || KType==SMOOTH || KType==MEANRENOVAL ||
                KType==LP_MEDIA || KType==LP_POND || KType==HP_MEDIA || KType==HP_LAPLACE ||
                    KType==HP_LAPLACE_MIN || KType==USUARIO ){
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
                //
                ISum =  RgbT_00->rgbRed * K3x3H[0] + RgbT_01->rgbRed * K3x3H[1] + RgbT_02->rgbRed * K3x3H[2] +
                        RgbT_10->rgbRed * K3x3H[3] + RgbT_11->rgbRed * K3x3H[4] + RgbT_12->rgbRed * K3x3H[5] +
                        RgbT_20->rgbRed * K3x3H[6] + RgbT_21->rgbRed * K3x3H[7] + RgbT_22->rgbRed * K3x3H[8];
                //
                RgbTOut->rgbRed = RgbTOut->rgbGreen = RgbTOut->rgbBlue = ParseByte(ISum/KDivH+Offset);
                //
                RgbT_00++;RgbT_01++;RgbT_02++;
                RgbT_10++;RgbT_11++;RgbT_12++;
                RgbT_20++;RgbT_21++;RgbT_22++;
                RgbTOut++;
            }
        }
    }else{
        ///if( KType==SOBEL || KType==PREWITT ){
        if( KType==PREWITT ){
            long long Mean2 = 0;
            KDivH = 0;
            KDivV = 0;
            for( int i=0; i<9; i++ ){
                KDivH = KDivH + abs(K3x3H[i]);
                KDivV = KDivV + abs(K3x3V[i]);
            }
            int KDivHP2 = KDivH * KDivH;
            ///
            ///
            ///
            //*
            /// PIXEL (0, 0)
            RgbTOut = &RgbDest[0];
            ISumH =  Rgb_InPtr[0].rgbRed   * K3x3H[4] + Rgb_InPtr[1].rgbRed     * K3x3H[5] +
                     Rgb_InPtr[W_P].rgbRed * K3x3H[7] + Rgb_InPtr[W_P+1].rgbRed * K3x3H[8];
            //
            ISumV =  Rgb_InPtr[0].rgbRed   * K3x3V[4] + Rgb_InPtr[1].rgbRed     * K3x3V[5] +
                     Rgb_InPtr[W_P].rgbRed * K3x3V[7] + Rgb_InPtr[W_P+1].rgbRed * K3x3V[8];
            ((int*)RgbTOut)[0] = ( (( ISumH * ISumH + ISumV * ISumV ))/KDivHP2 );
            Mean2 = Mean2 + ((int*)RgbTOut)[0];
            //
            //
            /// PIXEL (W_P-1, 0)
            RgbTOut = &RgbDest[W_P-1];
            ISumH =  Rgb_InPtr[W_P-2].rgbRed   * K3x3H[3] + Rgb_InPtr[W_P-1].rgbRed   * K3x3H[4] +
                     Rgb_InPtr[2*W_P-2].rgbRed * K3x3H[6] + Rgb_InPtr[2*W_P-1].rgbRed * K3x3H[7];
            //
            ISumV =  Rgb_InPtr[W_P-2].rgbRed   * K3x3V[3] + Rgb_InPtr[W_P-1].rgbRed   * K3x3V[4] +
                     Rgb_InPtr[2*W_P-2].rgbRed * K3x3V[6] + Rgb_InPtr[2*W_P-1].rgbRed * K3x3V[7];
            ((int*)RgbTOut)[0] = ( (( ISumH * ISumH + ISumV * ISumV ))/KDivHP2 );
            Mean2 = Mean2 + ((int*)RgbTOut)[0];
            //
            //
            /// PIXEL (0, H_P-1)
            RgbTOut = &RgbDest[W_P*(H_P-1)];
            ISumH =  Rgb_InPtr[W_P*(H_P-2)].rgbRed * K3x3H[1] + Rgb_InPtr[W_P*(H_P-2)+1].rgbRed * K3x3H[2] +
                     Rgb_InPtr[W_P*(H_P-1)].rgbRed * K3x3H[4] + Rgb_InPtr[W_P*(H_P-1)+1].rgbRed * K3x3H[5];
            //
            ISumV =  Rgb_InPtr[W_P*(H_P-2)].rgbRed * K3x3V[1] + Rgb_InPtr[W_P*(H_P-2)+1].rgbRed * K3x3V[2] +
                     Rgb_InPtr[W_P*(H_P-1)].rgbRed * K3x3V[4] + Rgb_InPtr[W_P*(H_P-1)+1].rgbRed * K3x3V[5];
            ((int*)RgbTOut)[0] = ( (( ISumH * ISumH + ISumV * ISumV ))/KDivHP2 );
            Mean2 = Mean2 + ((int*)RgbTOut)[0];
            //
            //
            /// PIXEL (W_P-1, H_P-1)
            RgbTOut = &RgbDest[W_P*(H_P-1) + W_P-1];
            ISumH =  Rgb_InPtr[W_P*(H_P-2) + W_P-2].rgbRed * K3x3H[0] + Rgb_InPtr[W_P*(H_P-2) + W_P-1].rgbRed * K3x3H[1] +
                     Rgb_InPtr[W_P*(H_P-1) + W_P-1].rgbRed * K3x3H[3] + Rgb_InPtr[W_P*(H_P-1) + W_P-1].rgbRed * K3x3H[4];
            //
            ISumV =  Rgb_InPtr[W_P*(H_P-2) + W_P-2].rgbRed * K3x3V[0] + Rgb_InPtr[W_P*(H_P-2) + W_P-1].rgbRed * K3x3V[1] +
                     Rgb_InPtr[W_P*(H_P-1) + W_P-1].rgbRed * K3x3V[3] + Rgb_InPtr[W_P*(H_P-1) + W_P-1].rgbRed * K3x3V[4];
            ((int*)RgbTOut)[0] = ( (( ISumH * ISumH + ISumV * ISumV ))/KDivHP2 );
            Mean2 = Mean2 + ((int*)RgbTOut)[0];
            ///
            ///
            ///
            ///
            ///
            //
            //
            for( y=1; y<H_P-1; y++ ){
                /// break;
                RgbTOut = &RgbDest[y*W_P];
                ISumH =  Rgb_InPtr[(y-1)*W_P].rgbRed * K3x3H[1] + Rgb_InPtr[(y-1)*W_P+1].rgbRed * K3x3H[2] +
                         Rgb_InPtr[y*W_P].rgbRed     * K3x3H[4] + Rgb_InPtr[y*W_P+1].rgbRed     * K3x3H[5] +
                         Rgb_InPtr[(y+1)*W_P].rgbRed * K3x3H[7] + Rgb_InPtr[(y+1)*W_P+1].rgbRed * K3x3H[8];
                //
                ISumV =  Rgb_InPtr[(y-1)*W_P].rgbRed * K3x3V[1] + Rgb_InPtr[(y-1)*W_P+1].rgbRed * K3x3V[2] +
                         Rgb_InPtr[y*W_P].rgbRed     * K3x3V[4] + Rgb_InPtr[y*W_P+1].rgbRed     * K3x3V[5] +
                         Rgb_InPtr[(y+1)*W_P].rgbRed * K3x3V[7] + Rgb_InPtr[(y+1)*W_P+1].rgbRed * K3x3V[8];
                ((int*)RgbTOut)[0] = ( (( ISumH * ISumH + ISumV * ISumV ))/KDivHP2 );
                Mean2 = Mean2 + ((int*)RgbTOut)[0];
                ///
                ///
                RgbTOut = &RgbDest[y*W_P+W_P-1];
                ISumH =  Rgb_InPtr[(y-1)*W_P+W_P-2].rgbRed * K3x3H[0] + Rgb_InPtr[(y-1)*W_P+W_P-1].rgbRed * K3x3H[1] +
                         Rgb_InPtr[y*W_P+W_P-2].rgbRed     * K3x3H[3] + Rgb_InPtr[y*W_P+W_P-1].rgbRed     * K3x3H[4] +
                         Rgb_InPtr[(y+1)*W_P+W_P-2].rgbRed * K3x3H[6] + Rgb_InPtr[(y+1)*W_P+W_P-1].rgbRed * K3x3H[7];
                //
                ISumV =  Rgb_InPtr[(y-1)*W_P+W_P-2].rgbRed * K3x3V[0] + Rgb_InPtr[(y-1)*W_P+W_P-1].rgbRed * K3x3V[1] +
                         Rgb_InPtr[y*W_P+W_P-2].rgbRed     * K3x3V[3] + Rgb_InPtr[y*W_P+W_P-1].rgbRed     * K3x3V[4] +
                         Rgb_InPtr[(y+1)*W_P+W_P-2].rgbRed * K3x3V[6] + Rgb_InPtr[(y+1)*W_P+W_P-1].rgbRed * K3x3V[7];
                ((int*)RgbTOut)[0] = ( (( ISumH * ISumH + ISumV * ISumV ))/KDivHP2 );
                Mean2 = Mean2 + ((int*)RgbTOut)[0];
            }
            ///
            ///
            ///
            ///
            ///
            for( x=1; x<W_P-1; x++ ){
                RgbTOut = &RgbDest[x];
ISumH =  Rgb_InPtr[x-1].rgbRed * K3x3H[3] + Rgb_InPtr[x].rgbRed * K3x3H[4] + Rgb_InPtr[x+1].rgbRed * K3x3H[5] +
         Rgb_InPtr[W_P+x-1].rgbRed * K3x3H[6] + Rgb_InPtr[W_P+x].rgbRed * K3x3H[7] + Rgb_InPtr[W_P+x+1].rgbRed * K3x3H[8];
                //
ISumV =  Rgb_InPtr[x-1].rgbRed * K3x3V[3] + Rgb_InPtr[x].rgbRed * K3x3V[4] + Rgb_InPtr[x+1].rgbRed * K3x3V[5] +
         Rgb_InPtr[W_P+x-1].rgbRed * K3x3V[6] + Rgb_InPtr[W_P+x].rgbRed * K3x3V[7] + Rgb_InPtr[W_P+x+1].rgbRed * K3x3V[8];
                ((int*)RgbTOut)[0] = ( (( ISumH * ISumH + ISumV * ISumV ))/KDivHP2 );
                Mean2 = Mean2 + ((int*)RgbTOut)[0];
                ///
                ///
                RgbTOut = &RgbDest[W_P*(H_P-1) + x];
ISumH =  Rgb_InPtr[W_P*(H_P-2) + x-1].rgbRed * K3x3H[0] + Rgb_InPtr[W_P*(H_P-2) + x].rgbRed * K3x3H[1] + Rgb_InPtr[W_P*(H_P-2) + x+1].rgbRed * K3x3H[2] +
         Rgb_InPtr[W_P*(H_P-1) + x-1].rgbRed * K3x3H[3] + Rgb_InPtr[W_P*(H_P-1) + x].rgbRed * K3x3H[4] + Rgb_InPtr[W_P*(H_P-1) + x+1].rgbRed * K3x3H[5];
                //
ISumV =  Rgb_InPtr[W_P*(H_P-2) + x-1].rgbRed * K3x3V[0] + Rgb_InPtr[W_P*(H_P-2) + x].rgbRed * K3x3V[1] + Rgb_InPtr[W_P*(H_P-2) + x+1].rgbRed * K3x3V[2] +
         Rgb_InPtr[W_P*(H_P-1) + x-1].rgbRed * K3x3V[3] + Rgb_InPtr[W_P*(H_P-1) + x].rgbRed * K3x3V[4] + Rgb_InPtr[W_P*(H_P-1) + x+1].rgbRed * K3x3V[5];
                ((int*)RgbTOut)[0] = ( (( ISumH * ISumH + ISumV * ISumV ))/KDivHP2 );
                Mean2 = Mean2 + ((int*)RgbTOut)[0];
                ///
            }
            ///
            //*/
            ///
            ///
            ///
            ///
            ///
            ///
            ///
            ///
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
                    Mean2 = Mean2 + ((int*)RgbTOut)[0];
                    /*
                    RgbTOut->rgbRed =
                    RgbTOut->rgbGreen =
                    RgbTOut->rgbBlue = ParseByte(
                                                 max( abs(ISumH/KDivH) + abs(ISumV/KDivV), Offset+128 )
                                                    );
                    Mean2 = Mean2 + RgbTOut->rgbRed;
                    //*/
                    //*/
                    /*/
                    RgbTOut->rgbRed = RgbTOut->rgbGreen = RgbTOut->rgbBlue = 255*ParseByteD(
                                                                                        _hypot(
                                                                                               (double)ISumH/(double)KDivH,
                                                                                               (double)ISumV/(double)KDivV
                                                                                               )
                                                                                        )>(Offset+128);
                    //*/
                    //
                    //
                    RgbT_00++;RgbT_01++;RgbT_02++;
                    RgbT_10++;RgbT_11++;RgbT_12++;
                    RgbT_20++;RgbT_21++;RgbT_22++;
                    RgbTOut++;
                }/// End for x
            }/// End for y
            ///
            Mean2 = (4*Mean2)/TopXY;
            if( Offset!=-128 ){
                Offset = Offset + 128;
                double O_D = ((double)Offset)/((double)255);
                O_D = (130050/2) * O_D * O_D;
                //Mean2 = 4 * Offset * Offset;
                Mean2 = (int)round( O_D );
                /// char T2T[1024] = {0};sprintf( T2T, "%f", O_D );MessageBoxA( *HWN_MAIN, T2T, "", 0 );
                ///
                int* IPtr = (int*)RgbDest;
                for( int i=0; i<TopXY; i++ ){
                    ///RgbDest[i].rgbRed = RgbDest[i].rgbGreen = RgbDest[i].rgbBlue = RgbDest[i].rgbRed>Mean2;
                    RgbDest[i].rgbRed = RgbDest[i].rgbGreen = RgbDest[i].rgbBlue = 255*(int)( (IPtr[i]) >=Mean2 );
                }
                ///
            }else{
                int* IPtr = (int*)RgbDest;
                for( int i=0; i<TopXY; i++ ){
                    ///RgbDest[i].rgbRed = RgbDest[i].rgbGreen = RgbDest[i].rgbBlue = RgbDest[i].rgbRed>Mean2;
                    RgbDest[i].rgbRed = RgbDest[i].rgbGreen = RgbDest[i].rgbBlue = 255*(int)( IPtr[i] >=Mean2 );
                }
            }
            ///
            memcpy( Rgb_InPtr, RgbDest, 4 * W_P * H_P );
            ///
        }else{
            if( KType==KIRSH ){
                SETKIRSH( Rgb_InPtr, RgbDest, W_P, H_P, Offset );
                memcpy( Rgb_InPtr, RgbDest, 4 * W_P * H_P );
            }
            if( KType==SOBEL ){
                double thresh = SOBEL_PROC( RgbIn, RgbDest, W_P, H_P, K3x3H, K3x3V, Offset );
                RgbTOut = &RgbIn[0];
                for( int i=0; i<numel; i++ ){
                    RgbTOut->rgbRed   =
                    RgbTOut->rgbGreen =
                    RgbTOut->rgbBlue  = 255 * (((int*)RgbTOut)[0]>thresh);
                    RgbTOut++;
                }
                memcpy( RgbDest, Rgb_InPtr, 4 * W_P * H_P );
            }
        }
    }
    ///
    ///
    ///
    ZeroMemory( Hist, 256 * sizeof(double));
    for( int i=0; i<TopXY; i++ ){
        Hist[RgbDest[i].rgbRed] = Hist[RgbDest[i].rgbRed] + 1;
    }
    ///
    free( RgbDest );
    ///
    return TRUE;
}
///
///
