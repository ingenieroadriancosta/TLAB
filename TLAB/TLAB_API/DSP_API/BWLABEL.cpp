int DIM2::BWLABEL( RGBQUAD* RgbIO, int W_W, int H_H ){
    return BWLABEL_NEW( RgbIO, W_W, H_H );
    int* labels = (int*)malloc( 4 * (W_W+8) * (H_H+8) );
    ZeroMemory( labels, 4 * (W_W+8) * (H_H+8) - 1 );
    int MV_N    = (W_W+2) * (H_H+2);
    int MV      = MV_N;
    int MV1     = MV + 1;
    int lbcnt   = 0;
    int y       = 0;
    int x       = 0;
    int x1      = 0;
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
    int YPW = 0;
    int YPW_1 = 0;
    int YPWP1 = 0;
    int mn = 0;
    int mx = 0;
    int W_W2 = W_W+2;
    int WN_1 = W_W+2 - 1;
    int HN_1 = H_H+2 - 1;
    for( y = 1; y<HN_1; y++ ){
        YPW     =  y   *W_W2;
        YPW_1   = (y-1)*W_W2;
        YPWP1   = (y+1)*W_W2;
        for( x = 1; x<WN_1; x++ ){
            if( labels[ YPW + x ]==MV1 ){
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
                ///
                for( x1=x; x1>0; x1-- ){
                    if( labels[YPWP1 + x1]==MV1 ){
                        labels[YPWP1 + x1] = labels[ YPW + x ];
                    }else{
                        break;
                    }
                }
                ///
                ///
                for( x1=x+1; x1<WN_1; x1++ ){
                    if( labels[YPWP1 + x1]==MV1 ){
                        labels[YPWP1 + x1] = labels[ YPW + x ];
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
    ///
    ///
    /// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    int* lbl = (int*)malloc( 4*lbcnt + 32 );
    ZeroMemory( lbl, 4*lbcnt + 31 );
    ///
    for( y = HN_1; y>0; y-- ){
        YPW     =  y   *W_W2;
        YPWP1   = (y+1)*W_W2;
        for( x = 1; x<WN_1; x++ ){
            if( labels[YPW+x]!=MV ){
                mn = min( labels[YPWP1+x-1], min( labels[YPWP1+x], labels[YPWP1+x+1] ) );
                mn = min( mn, labels[YPW+x-1] );
                if( mn!=MV && mn!=labels[YPW+x] ){
                    labels[YPW+x] = mn;
                }
                lbl[labels[YPW+x]] = 1;
            }
        }
    }
    /// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    /// jet
    ///
    ///
    ///
    mx++;
    for( y = 1; y<HN_1; y++ ){
        YPW     =  y   *W_W2;
        YPWP1   = (y+1)*W_W2;
        for( x = 1; x<WN_1; x++ ){
            if( labels[YPW+x]!=labels[YPW+x+1] && labels[YPW+x]!=MV ){
                mx = max( labels[YPW+x], labels[YPW+x+1] );
                mn = min( labels[YPW+x], labels[YPW+x+1] );
                if( mx<MV && mn<MV && mn!=0 && mx!=0 ){
                    lbl[mn] = mx;
                }
            }
        }
    }
    ///
    int nobjetosP = 0;///sum( lbl, lbcnt+1, false );
    for( int i=0; i<lbcnt+1; i++ ){
        nobjetosP = nobjetosP + (lbl[i]==1);
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
    ///
    ///
    ///
    ///
    int colorn = 0;
    for( y = 1; y<HN_1; y++ ){
        YPW     =  y   *W_W2;
        YPWP1   = (y-1)*W_W2;
        for( x = 1; x<WN_1; x++ ){
            x1 = labels[YPW+x];
            if( x1==MV ){
                continue;
            }else{
                if( labels[YPW+x-1]!=MV ){
                    labels[YPW+x] = labels[YPW+x-1];
                }else{
                    if( labels[YPWP1+x-1]!=MV ){
                        labels[YPW+x] = labels[YPWP1+x-1];
                    }else{
                        if( labels[YPWP1+x]!=MV ){
                            labels[YPW+x] = labels[YPWP1+x];
                        }else{
                            if( labels[YPWP1+x+1]!=MV ){
                                labels[YPW+x] = labels[YPWP1+x+1];
                            }
                        }
                    }
                }
            }

        }
    }
    ///
    ///
    ///
    ///
    ///
    ///
    ///
    ///
    /// nobjetosP
    for( y = 0; y<H_H; y++ ){
        for( x = 0; x<W_W; x++ ){
            x1 = labels[y*(W_W+2)+x+1];
            if( x1==MV ){
                RgbIO[y*W_W+x].rgbRed   =
                RgbIO[y*W_W+x].rgbGreen =
                RgbIO[y*W_W+x].rgbBlue  = 255;
            }else{
                if( x1<=lbcnt ){
                    if( lbl[x1]==1 ){
                        colorn = (int)round( 255*(x1/((double)(nobjetosP-1))) );
                        RgbIO[y*W_W+x] = jet[255&colorn];
                    }else{
                        colorn = (int)round( 255*(lbl[x1]/((double)(nobjetosP-1))) );
                        RgbIO[y*W_W+x] = jet[255&colorn];
                    }
                }
            }

        }
    }
    ///
    free(labels);
    free(lbl);
    return nobjetosP;
}
