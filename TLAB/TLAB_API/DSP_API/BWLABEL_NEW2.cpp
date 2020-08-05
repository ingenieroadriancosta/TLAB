int DIM2::BWLABEL_NEW2( RGBQUAD* RgbIO, int W_W, int H_H ){
    /// return BWLABEL_NEW3( RgbIO, W_W, H_H );
    int* labels = (int*)malloc( 4 * (W_W+8) * (H_H+8) );
    ZeroMemory( labels, 4 * (W_W+8) * (H_H+8) - 1 );
    int MV_N        = (W_W+2) * (H_H+2);
    int MV          = MV_N;
    int MV1         = MV + 1;
    int lbcnt       = 0;
    int y           = 0;
    int x           = 0;
    int x1          = 0;
    int nobjetosP   = 0;
    ///
    for( y=0; y<H_H; y++ ){
        memcpy( &labels[y*(W_W+2)+1], &RgbIO[y*W_W], 4*W_W );
    }
    ///
    for( y = 0; y<H_H; y++ ){
        for( x = 0; x<W_W; x++ ){
            labels[y*(W_W+2)+x+1] = (RgbIO[y*W_W+x].rgbRed>0);
        }
    }
    ///
    for( int i=0; i<MV_N; i++ ){
        labels[i] = ( MV + (int)(labels[i]!=0) );
    }
    ///
    ///
    int YPW = 0;
    int YPW_1 = 0;
    int YPWP1 = 0;
    int mn = 0;
    int W_W2 = W_W+2;
    int WN_1 = W_W+2 - 1;
    int HN_1 = H_H+2 - 1;
    int lbval = 0;
    for( y = 1; y<HN_1; y++ ){
        YPW     =  y   *W_W2;
        YPW_1   = (y-1)*W_W2;
        YPWP1   = (y+1)*W_W2;
        for( x = 1; x<WN_1; x++ ){
            if( labels[ YPW + x ]==MV1 || labels[ YPW + x ]<MV ){
                mn = min( labels[YPW_1+x-1], min( labels[YPW_1+x], labels[YPW_1+x+1] ) );
                if( mn<MV ){
                    labels[ YPW + x ] = mn;
                }else{
                    if( labels[ YPW + x - 1 ]==MV ){
                        lbcnt = lbcnt + 1;
                        labels[ YPW + x ] = lbcnt;
                    }else{
                        labels[ YPW + x ] = labels[ YPW + x - 1 ];
                    }
                }
                ///
                lbval = labels[ YPW + x ];
                ///
                for( x1=x+1; x1<WN_1; x1++ ){
                    if( labels[YPW + x1]==MV1 ){
                        labels[YPW + x1] = lbval;
                    }else{
                        break;
                    }
                }
                ///
                ///
                for( x1=x; x1>0; x1-- ){
                    if( labels[YPWP1 + x1]==MV1 ){
                        labels[YPWP1 + x1] = lbval;
                    }else{
                        break;
                    }
                }
                ///
                ///
                for( x1=x+1; x1<WN_1; x1++ ){
                    if( labels[YPWP1 + x1]==MV1 ){
                        labels[YPWP1 + x1] = lbval;
                    }else{
                        break;
                    }
                }
                ///
                ///
            }
        }
    }
    ///
    ///
    ///
    int lbcnt1 = lbcnt+1;
    int inl = 0;
    int* lbl = (int*)malloc( 4*( (lbcnt1)*(lbcnt1)) + 32 );
    ZeroMemory( lbl, 4*( (lbcnt1)*(lbcnt1)) + 31 );
    ///
    int* lbl1 = (int*)malloc( 4*(lbcnt1) + 32 );
    ZeroMemory( lbl1, 4*(lbcnt1) + 31 );
    ///
    for( int x=1; x<lbcnt1; x++ ){
        for( int y=1; y<lbcnt1; y++ ){
            lbl[y*lbcnt1+x] = lbcnt1+2;
        }
    }
    ///
    ///
    for( y = HN_1; y>0; y-- ){
        YPW     =  y   *W_W2;
        YPW_1   = (y-1)*W_W2;
        YPWP1   = (y+1)*W_W2;
        for( x = WN_1; x>0; x-- ){
            if( labels[YPW+x]<=lbcnt1 ){
                mn = min( labels[YPW_1+x-1], min( labels[YPW_1+x], labels[YPW_1+x+1] ) );
                mn = min( mn, min( labels[YPWP1+x-1], min( labels[YPWP1+x], labels[YPWP1+x+1] ) ) );
                mn = min( mn, min( labels[YPW+x-1], min( labels[YPW+x], labels[YPW+x+1] ) ) );
                inl = labels[YPW+x];
                lbl[mn*lbcnt1 + inl] = mn;
                labels[YPW+x] = mn;
            }
        }
    }
    ///
    ///
    for( y = HN_1; y>0; y-- ){
        YPW     =  y   *W_W2;
        YPW_1   = (y-1)*W_W2;
        YPWP1   = (y+1)*W_W2;
        for( x = WN_1; x>0; x-- ){
            if( labels[YPW+x]<=lbcnt1 ){
                mn = min( labels[YPW_1+x-1], min( labels[YPW_1+x], labels[YPW_1+x+1] ) );
                mn = min( mn, min( labels[YPWP1+x-1], min( labels[YPWP1+x], labels[YPWP1+x+1] ) ) );
                mn = min( mn, min( labels[YPW+x-1], min( labels[YPW+x], labels[YPW+x+1] ) ) );
                inl = labels[YPW+x];
                lbl[mn*lbcnt1 + inl] = mn;
                labels[YPW+x] = mn;
            }
        }
    }
    ///
    ///
    ///
    ///
    for( x=1; x<lbcnt1; x++ ){
        mn = lbcnt1+2;
        for( y=1; y<lbcnt1; y++ ){
            mn = min( lbl[y*lbcnt1 + x], mn );
        }
        lbl1[x] = mn;
    }
    ///
    ///
    ///
    ///
    ///
    int l_i = 0;
    for( int i=lbcnt+1; i>0; i-- ){
        l_i = lbl1[i];
        while( l_i!=lbl1[l_i] ){
            lbl1[i] = lbl1[l_i];
            l_i = lbl1[i];
        }
    }
    nobjetosP = 0;///sum( lbl, lbcnt+1, false );
    for( int i=1; i<lbcnt+1; i++ ){
        nobjetosP = nobjetosP + (lbl1[i]==i);
    }
    ///
    ///
    ///
    ///
    ///
    ///
    ///
    ///
    ///

    ///
    /// nobjetosP
    int colorn = 0;
    for( y = 0; y<H_H; y++ ){
        for( x = 0; x<W_W; x++ ){
            x1 = labels[y*(W_W+2)+x+1];
            if( x1==MV ){
                RgbIO[y*W_W+x].rgbRed   =
                RgbIO[y*W_W+x].rgbGreen =
                RgbIO[y*W_W+x].rgbBlue  = 255;
            }else{
                if( x1<=lbcnt ){
                    colorn = (int)round( 255*(lbl1[x1]/((double)(nobjetosP-1))) );
                    RgbIO[y*W_W+x] = jet[255&colorn];
                }
            }

        }
    }
    ///
    free(labels);
    free(lbl);
    free(lbl1);
    return nobjetosP;
}
