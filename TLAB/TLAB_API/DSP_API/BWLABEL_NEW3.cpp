int DIM2::BWLABEL_NEW3( RGBQUAD* RgbIO, int W_W, int H_H ){
    int nobjetosP   = 0;
    nobjetosP = BWLABEL_NEW4( RgbIO, W_W, H_H );
    if( nobjetosP>0 ){
        return nobjetosP;
    }
    /// VARIABLES
    int MaxLabels = ( W_W * H_H )/4;
    int W_WL = W_W+2;
    int H_HL = H_H+2;
    int SizeWHL = (W_WL) * (H_HL);
    int SizeWHL_TOP = SizeWHL - W_WL - 1;
    int S_Z = SizeWHL;
    int S_Z1 = S_Z + 255;
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
    LPtr = &labels[(H_HL-1)*W_WL];
    for( i=0; i<W_WL; i++ ){
        labels[i] = S_Z;
        LPtr[i] = S_Z;
    }
    for( y = 0; y<H_H; y++ ){
        RgbPtr = (RGBQUAD*)&RgbIO[y*W_W];
        LPtr   = &labels[(y+1)*W_WL+1];
        for( x = 0; x<W_W; x++ ){
            RgbPtr[x].rgbRed = RgbPtr[x].rgbRed;
            LPtr[x] = (RgbPtr[x].rgbRed + S_Z);
        }
        labels[(y+1)*W_WL] = S_Z;
        labels[(y+1)*W_WL+W_WL-1] = S_Z;
    }
    ///
    ///
    ///
    ///
    ///
    LPtr00 = &labels[0];
    LPtr01 = &labels[1];
    LPtr02 = &labels[2];
    LPtr10 = &labels[W_WL];
    LPtr11 = &labels[W_WL+1];
    for( i = W_WL+1; i<SizeWHL_TOP; i++ ){
        if( (*LPtr11)==S_Z1 ){
            m_v = min( (*LPtr10), min( (*LPtr00), min((*LPtr01), (*LPtr02)) ) );
            if( m_v<S_Z ){
                *LPtr11 = m_v;
            }else{
                nlabel = nlabel + 1;
                *LPtr11 = i;
                m_vP = i;
            }
        }
        ///
        ///
        //
        LPtr10++;
        LPtr11++;
        //
        LPtr00++;
        LPtr01++;
        LPtr02++;
    }
    ///
    ///
    ///
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
        return nlabel;
    }
    ///
    ///   IF --->>> MAX LABELS
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
    while( true ){
        Setbre = TRUE;
    ///
    LPtr00 = &labels[SizeWHL_TOP-1 - W_WL - 1];
    LPtr01 = &labels[SizeWHL_TOP-1 - W_WL];
    LPtr02 = &labels[SizeWHL_TOP-1 - W_WL + 1];
    ///
    LPtr10 = &labels[SizeWHL_TOP-1-1];
    LPtr11 = &labels[SizeWHL_TOP-1];
    LPtr12 = &labels[SizeWHL_TOP-1+1];
    ///
    LPtr20 = &labels[SizeWHL_TOP-1 + W_WL-1];
    LPtr21 = &labels[SizeWHL_TOP-1 + W_WL];
    LPtr22 = &labels[SizeWHL_TOP-1 + W_WL+1];
    ///
    for( i = SizeWHL_TOP-1; i>W_WL; i-- ){
        if( (*LPtr11)<S_Z ){
            m_v = min(
                      min( *LPtr00, min(*LPtr01, *LPtr02) ),
                      min( min( *LPtr10, min(*LPtr11, *LPtr12) ),
                      min( *LPtr20, min(*LPtr21, *LPtr22) ) )
                      );
            ///
            if( m_v!=(*LPtr00) && (*LPtr00)<S_Z ){
                Setbre = TRUE;
                *LPtr00 = m_v;
            }
            ///
            if( m_v!=(*LPtr01) && (*LPtr01)<S_Z ){
                Setbre = TRUE;
                *LPtr01 = m_v;
            }
            ///
            if( m_v!=(*LPtr02) && (*LPtr02)<S_Z ){
                Setbre = TRUE;
                *LPtr02 = m_v;
            }
            ///
            ///
            ///
            if( m_v!=(*LPtr10) && (*LPtr10)<S_Z ){
                Setbre = TRUE;
                *LPtr10 = m_v;
            }
            ///
            if( m_v!=(*LPtr11) && (*LPtr11)<S_Z ){
                Setbre = FALSE;
                *LPtr11 = m_v;
            }
            ///
            if( m_v!=(*LPtr12) && (*LPtr12)<S_Z ){
                Setbre = TRUE;
                *LPtr12 = m_v;
            }
            ///
            ///
            ///
            ///
            if( m_v!=(*LPtr20) && (*LPtr20)<S_Z ){
                Setbre = TRUE;
                *LPtr20 = m_v;
            }
            ///
            if( m_v!=(*LPtr21) && (*LPtr21)<S_Z ){
                Setbre = TRUE;
                *LPtr21 = m_v;
            }
            ///
            if( m_v!=(*LPtr22) && (*LPtr22)<S_Z ){
                Setbre = TRUE;
                *LPtr22 = m_v;
            }
        }
        ///
        LPtr00--;
        LPtr01--;
        LPtr02--;
        ///
        LPtr10--;
        LPtr11--;
        LPtr12--;
        ///
        LPtr20--;
        LPtr21--;
        LPtr22--;
        ///
        ///
        ///
        ///
        ///
    }
    if( Setbre ){
        break;
    }
    }
    ///
    ///
    nobjetosP = 0;
    for( i = W_WL; i<SizeWHL_TOP; i++ ){
        nobjetosP = nobjetosP + (int)(labels[i]==i);
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
    if( nlabel==0 ){
        nlabel = 1;
    }
    ///
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
    ///
    ///
    ///
    free(labels);
    return nobjetosP;
}
