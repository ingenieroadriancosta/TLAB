int DIM2::BWLABEL_NEW4( RGBQUAD* RgbIO, int W_W, int H_H ){
    // return 0;
    DSP_MNG.tic( 99 );
    /// VARIABLES
    int nobjetosP   = 0;
    int WPH = ( W_W * H_H );
    int MaxLabels = WPH/4;
    int W_WL = W_W+2;
    int H_HL = H_H+2;
    int SizeWHL = (W_WL) * (H_HL);
    int SizeWHL_TOP = SizeWHL - W_WL - 1;
    int S_Z = SizeWHL;
    RGBQUAD* RgbPtr = NULL;
    int* LPtr = NULL;
    int* labels = NULL;
    int y           = 0;
    int x           = 0;
    int i           = 0;
    int colorn = 0;
    int x_1 = 0;
    int m_v = 0;
    int m_vP = 0;
    int nlabel = 0;
    int npix = 0;
    ///
    int* LPtr00 = NULL;
    int* LPtr01 = NULL;
    int* LPtr02 = NULL;
    ///
    int* LPtr10 = NULL;
    int* LPtr11 = NULL;
    int* LPtr12 = NULL;
    ///
    int* LPtr20 = NULL;
    int* LPtr21 = NULL;
    int* LPtr22 = NULL;
    ///
    ///
    ///
    ///
    labels = (int*)malloc( 4 * SizeWHL + 8 );
    ZeroMemory( labels, 4 * SizeWHL );
    ///
    int* lblptr = (int*)malloc( 4 * SizeWHL + 8 );
    ZeroMemory( lblptr, 4 * SizeWHL );
    ///
    ///
    LPtr = &labels[(H_HL-1)*W_WL];
    for( i=0; i<W_WL; i++ ){
        labels[i] = S_Z;
        LPtr[i] = S_Z;
    }
    ///
    ///
    for( y = 0; y<H_H; y++ ){
        RgbPtr = (RGBQUAD*)&RgbIO[y*W_W];
        i = (y+1)*W_WL+1;
        LPtr   = &labels[i];
        labels[(y+1)*W_WL] = S_Z;
        for( x = 0; x<W_W; x++ ){
            LPtr[x] = S_Z;
            if( RgbPtr[x].rgbRed>0 ){
                npix++;
                lblptr[npix] = ( x + i );
                ///
                LPtr11 = &LPtr[x];
                ///
                LPtr01 = LPtr11 - W_WL;
                LPtr00 = LPtr01 - 1;
                LPtr02 = LPtr01 + 1;
                ///
                LPtr10 = LPtr11 - 1;
                ///
                m_v = min( (*LPtr10), min( (*LPtr00), min((*LPtr01), (*LPtr02)) ) );
                if( m_v<S_Z ){
                    *LPtr11 = m_v;
                }else{
                    nlabel = nlabel + 1;
                    *LPtr11 = x + i;
                    m_vP = x + i;
                }
                ///
            }
        }
        labels[(y+1)*W_WL+W_WL-1] = S_Z;
    }
    ///
    ///
    ///  IF --->>> MAX LABELS
    if( nlabel==MaxLabels || nlabel==1 ){
        for( y = 0; y<H_H; y++ ){
            LPtr   = &labels[(y+1)*W_WL+1];
            for( x = 0; x<W_W; x++ ){
                x_1 = LPtr[x];
                if( x_1==S_Z ){
                    RgbIO[y*W_W+x].rgbRed   =
                    RgbIO[y*W_W+x].rgbGreen =
                    RgbIO[y*W_W+x].rgbBlue  = 255;
                }else{
                    colorn = (int)round( 255*(x_1/((double)(m_vP))) );
                    RgbIO[y*W_W+x] = jet[255&colorn];
                }

            }
        }
        free(labels);
        free(lblptr);
        return nlabel;
    }
    ///   IF --->>> MAX LABELS
    ///
    ///
    ///
    ///
    nobjetosP = nlabel;
    m_vP = nlabel;
    ///
    ///
    ///
    ///
    ///
    ///
    ///
    BOOL Setbre = FALSE;
    ///
    int* LB0 = &labels[0];
    int NTim = 0;
    while( true ){
        NTim++;
        Setbre = TRUE;
    ///
    ///for( i = SizeWHL_TOP-1; i>W_WL; i-- ){
    //for( i = W_WL+1; i<SizeWHL_TOP; i++ ){
    for( i = 1; i<=npix; i++ ){
        /// lblptr[npix]
        LPtr11 = (int*)( LB0 + lblptr[i] );
        //LPtr11 = (int*)( &labels[i] );
        ///
        ///
        ///
        LPtr01 = LPtr11 - W_WL;
        LPtr00 = LPtr01 - 1;
        LPtr02 = LPtr01 + 1;
        ///
        LPtr10 = LPtr11 - 1;
        LPtr12 = LPtr11 + 1;
        ///
        LPtr21 = LPtr11 + W_WL;
        LPtr20 = LPtr21 - 1;
        LPtr22 = LPtr21 + 1;
        ///
        ///if( (*LPtr11)<S_Z ){
            m_v = min(
                      min( *LPtr00, min(*LPtr01, *LPtr02) ),
                      min( min( *LPtr10, min(*LPtr11, *LPtr12) ),
                      min( *LPtr20, min(*LPtr21, *LPtr22) ) )
                      );
            ///
            if( m_v!=(*LPtr00) && (*LPtr00)<S_Z ){
                Setbre = FALSE;
                *LPtr00 = m_v;
            }
            ///
            if( m_v!=(*LPtr01) && (*LPtr01)<S_Z ){
                Setbre = FALSE;
                *LPtr01 = m_v;
            }
            ///
            if( m_v!=(*LPtr02) && (*LPtr02)<S_Z ){
                Setbre = FALSE;
                *LPtr02 = m_v;
            }
            ///
            ///
            ///
            if( m_v!=(*LPtr10) && (*LPtr10)<S_Z ){
                Setbre = FALSE;
                *LPtr10 = m_v;
            }
            ///
            if( m_v!=(*LPtr11) && (*LPtr11)<S_Z ){
                Setbre = FALSE;
                *LPtr11 = m_v;
            }
            ///
            if( m_v!=(*LPtr12) && (*LPtr12)<S_Z ){
                Setbre = FALSE;
                *LPtr12 = m_v;
            }
            ///
            ///
            ///
            ///
            if( m_v!=(*LPtr20) && (*LPtr20)<S_Z ){
                Setbre = FALSE;
                *LPtr20 = m_v;
            }
            ///
            if( m_v!=(*LPtr21) && (*LPtr21)<S_Z ){
                Setbre = FALSE;
                *LPtr21 = m_v;
            }
            ///
            if( m_v!=(*LPtr22) && (*LPtr22)<S_Z ){
                Setbre = FALSE;
                *LPtr22 = m_v;
            }
        ///}
        ///
        ///

        ///
        ///
        ///
        ///
        ///
        ///
    }
    ///
    ///
    ///
    ///
    /// for( i = SizeWHL_TOP-1; i>W_WL; i-- ){
    ///for( i = W_WL+1; i<SizeWHL_TOP; i++ ){
    for( i = npix; i>0; i-- ){
        /// lblptr[npix]
        LPtr11 = (int*)( LB0 + lblptr[i] );
        //LPtr11 = (int*)( &labels[i] );
        ///
        ///
        ///
        LPtr01 = LPtr11 - W_WL;
        LPtr00 = LPtr01 - 1;
        LPtr02 = LPtr01 + 1;
        ///
        LPtr10 = LPtr11 - 1;
        LPtr12 = LPtr11 + 1;
        ///
        LPtr21 = LPtr11 + W_WL;
        LPtr20 = LPtr21 - 1;
        LPtr22 = LPtr21 + 1;
        ///
        ///if( (*LPtr11)<S_Z ){
            m_v = min(
                      min( *LPtr00, min(*LPtr01, *LPtr02) ),
                      min( min( *LPtr10, min(*LPtr11, *LPtr12) ),
                      min( *LPtr20, min(*LPtr21, *LPtr22) ) )
                      );
            ///
            if( m_v!=(*LPtr00) && (*LPtr00)<S_Z ){
                Setbre = FALSE;
                *LPtr00 = m_v;
            }
            ///
            if( m_v!=(*LPtr01) && (*LPtr01)<S_Z ){
                Setbre = FALSE;
                *LPtr01 = m_v;
            }
            ///
            if( m_v!=(*LPtr02) && (*LPtr02)<S_Z ){
                Setbre = FALSE;
                *LPtr02 = m_v;
            }
            ///
            ///
            ///
            if( m_v!=(*LPtr10) && (*LPtr10)<S_Z ){
                Setbre = FALSE;
                *LPtr10 = m_v;
            }
            ///
            if( m_v!=(*LPtr11) && (*LPtr11)<S_Z ){
                Setbre = FALSE;
                *LPtr11 = m_v;
            }
            ///
            if( m_v!=(*LPtr12) && (*LPtr12)<S_Z ){
                Setbre = FALSE;
                *LPtr12 = m_v;
            }
            ///
            ///
            ///
            ///
            if( m_v!=(*LPtr20) && (*LPtr20)<S_Z ){
                Setbre = FALSE;
                *LPtr20 = m_v;
            }
            ///
            if( m_v!=(*LPtr21) && (*LPtr21)<S_Z ){
                Setbre = FALSE;
                *LPtr21 = m_v;
            }
            ///
            if( m_v!=(*LPtr22) && (*LPtr22)<S_Z ){
                Setbre = FALSE;
                *LPtr22 = m_v;
            }
        ///}
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
    }
    ///
    ///
    ///
    ///
    ///
    if( Setbre ){
        break;
    }
    }
    ///
    ///
    nobjetosP = 0;
    m_vP = 0;
    for( i = W_WL; i<SizeWHL_TOP; i++ ){
            break;
        m_vP = max( m_vP, labels[i] );
        nobjetosP = nobjetosP + (int)(labels[i]==i);
    }
    ///
    //*
    nobjetosP = 0;
    m_vP = 0;
    for( i = 1; i<=npix; i++ ){
            break;
        m_vP = max( m_vP, labels[lblptr[i]] );
        nobjetosP = nobjetosP + (int)( labels[lblptr[i]]==lblptr[i] );
    }
    //*/
    ///
    ///
    nobjetosP = 0;
    for( i = 1; i<=npix; i++ ){
        ///
        if( labels[lblptr[i]]==lblptr[i] ){
            labels[lblptr[i]] = nobjetosP;
            nobjetosP = nobjetosP + 1;
        }else{
            labels[lblptr[i]] = labels[labels[lblptr[i]]];
        }
        ///
        ///m_vP = max( m_vP, labels[lblptr[i]] );
    }
    ///
    m_vP = nobjetosP;
    ///
    ///
    ///
    ///
    ///
    ///
    ///
    ///
    ///
    if( m_vP==0 ){
        m_vP = 1;
    }
    ///
    ///
    ///
    if( nobjetosP<256 ){
        for( y = 0; y<H_H; y++ ){
            LPtr   = &labels[(y+1)*W_WL+1];
            for( x = 0; x<W_W; x++ ){
                x_1 = LPtr[x];
                if( x_1==S_Z ){
                    RgbIO[y*W_W+x].rgbRed   =
                    RgbIO[y*W_W+x].rgbGreen =
                    RgbIO[y*W_W+x].rgbBlue  = 255;
                }else{
                    colorn = (int)round( 255*( (x_1-1)/((double)(m_vP))) );
                    RgbIO[y*W_W+x] = jet[255&colorn];
                }

            }
        }
    }else{
        for( y = 0; y<H_H; y++ ){
            LPtr   = &labels[(y+1)*W_WL+1];
            for( x = 0; x<W_W; x++ ){
                x_1 = LPtr[x];
                if( x_1==S_Z ){
                    RgbIO[y*W_W+x].rgbRed   =
                    RgbIO[y*W_W+x].rgbGreen =
                    RgbIO[y*W_W+x].rgbBlue  = 255;
                }else{
                    colorn = (x_1-1);
                    RgbIO[y*W_W+x] = jet[255&colorn];
                }

            }
        }
    }
    ///
    ///
    ///
    DSP_MNG.toc( 99 );
    char T2T[256] = {0};
    sprintf( T2T, "%g Segundos. npix = %i,  WPH = %i  (%i)", DSP_MNG.GetEndToc(99), npix, WPH, NTim );
    /// SetWindowText( HWN_MAIN[0], T2T );
    ///
    free(labels);
    free(lblptr);
    return nobjetosP;
}
