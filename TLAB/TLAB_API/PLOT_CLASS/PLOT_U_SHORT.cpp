BOOL PLOTEOS::PLOT_U_SHORT( short* SBuff, int MaxPos, int TopBreak, BOOL ForcePlot ){
    if( MaxPos<=0 ){
        FillGrid( TRUE );
        return FALSE;
    }
    ///
    if( PLTS==NULL && SBuff==NULL ){
        return FALSE;
    }
    ///
    if( SBuff!=NULL ){
        PLTS = SBuff;
    }
    ///
    if( !BVisible && ForcePlot==FALSE ){
        return FALSE;
    }
    ///
    short* SPTR = (short*)PLTS;
    if( !HoldV ){
        FillGrid( FALSE );
    }
    const int WGR_1 = WidthGraph - 1;
    const int HGR_1 = HeightGraph - 1;
    ///                                                           ///
    int xpos = 0;
    int Nxpos = 0;
    int ypos = 0;
    int Nypos = 0;
    /// if( MaxPos<500000 || ForcePlot ){/// 2400000
    if( MaxPos>0 ){/// 2400000
        for( int i=0; i<TopBreak - 1; i++ ){
            xpos = (int)( (i*((double)WGR_1))/MaxPos );
            ypos = ( HGR_1 - ( HGR_1 * (int)SPTR[i] )/32767 );
            setRGB( xpos, ypos, ColorLineG );
            ///
            ///
            Nxpos = (int)( ((i+1)*(double)WGR_1)/MaxPos );
            Nypos = ( HGR_1 - ( HGR_1 * (int)SPTR[i+1] )/32767 );
            if( Nxpos==xpos && Nypos==ypos ){
                //i++;
                continue;
            }
            ///
            /*
            while( xpos!=Nxpos ){
                xpos++;
                setRGB( xpos, ypos, ColorLineG );
            }
            //*/
            ///
            while( ypos>Nypos ){
                ypos--;
                setRGB( xpos, ypos, ColorLineG );
            }
            while( ypos!=Nypos ){
                ypos++;
                setRGB( xpos, ypos, ColorLineG );
            }
            ///
            ///
        }
    }else{
        for( int i=0; i<MaxPos-1; i++ ){
            /*
            xpos = (int)( (i*((double)WGR_1))/MaxPos );
            ypos = ( HGR_1 - ( HGR_1 * (int)SPTR[i] )/32768 );
            setRGB( xpos, ypos, ColorLineG );
            //*/
            xpos = (int)( (i*((double)WGR_1))/MaxPos );
            ypos = ( HGR_1 - ( HGR_1 * (int)SPTR[i] )/32767 );
            setRGB( xpos, ypos, ColorLineG );
            ///
            ///
            Nxpos = (int)( ((i+1)*(double)WGR_1)/MaxPos );
            Nypos = ( HGR_1 - ( HGR_1 * (int)SPTR[i+1] )/32767 );
            if( Nxpos==xpos && Nypos==ypos ){
                i++;
                continue;
            }
            while( xpos!=Nxpos ){
                xpos++;
                setRGB( xpos, ypos, ColorLineG );
            }
            ///
            while( ypos>Nypos ){
                ypos--;
                setRGB( xpos, ypos, ColorLineG );
            }
            while( ypos!=Nypos ){
                ypos++;
                setRGB( xpos, ypos, ColorLineG );
            }
            ///
            ///
            i = i + 2;
        }
    }
    ///
    if( !HoldV ){
        FillBuff();
    }
    return TRUE;
}
/// /////////////////////////////////////////////////////////////////////////////////////////////////
/// /////////////////////////////////////////////////////////////////////////////////////////////////
/// /////////////////////////////////////////////////////////////////////////////////////////////////
/// /////////////////////////////////////////////////////////////////////////////////////////////////
/// /////////////////////////////////////////////////////////////////////////////////////////////////
/// /////////////////////////////////////////////////////////////////////////////////////////////////
/// /////////////////////////////////////////////////////////////////////////////////////////////////
BOOL PLOTEOS::PLOT_U_SHORT_FFT( short* SBuff, int MaxPos, int InitFrames, BOOL ForcePlot ){
    if( MaxPos<=0 ){
        FillGrid( TRUE );
        return FALSE;
    }
    ///
    if( PLTS==NULL && SBuff==NULL ){
        return FALSE;
    }
    ///
    if( SBuff!=NULL ){
        PLTS = SBuff;
    }
    ///
    if( !BVisible && ForcePlot==FALSE ){
        return FALSE;
    }
    ///
    short* SPTR = (short*)PLTS;
    if( !HoldV ){
        FillGrid( FALSE );
    }
    const int WGR_1 = WidthGraph - 1;
    const int HGR_1 = HeightGraph - 1;
    ///                                                           ///
    int xpos = 0;
    int Nxpos = 0;
    int ypos = 0;
    int Nypos = 0;
    //if( MaxPos<500000 || ForcePlot || TRUE ){/// 2400000
    if( MaxPos>0 || ForcePlot ){/// 2400000
    ///if( MaxPos<1  ){/// 2400000
        for( int i=0; i<MaxPos-1; i++ ){
            xpos = (int)( (i*((double)WGR_1))/MaxPos );
            ypos = ( HGR_1 - ( HGR_1 * (int)SPTR[i] )/32767 );
            setRGB( xpos, ypos, ColorLineG );
            ///
            ///
            Nxpos = (int)( ((i+1)*(double)WGR_1)/MaxPos );
            Nypos = ( HGR_1 - ( HGR_1 * (int)SPTR[i+1] )/32767 );
            if( Nxpos==xpos && Nypos==ypos ){
                i++;
                continue;
            }
            if( Nxpos!=xpos && Nypos==ypos ){
                i++;
                continue;
            }
            while( xpos!=Nxpos ){
                xpos++;
                setRGB( xpos, ypos, ColorLineG );
            }
            ///
            while( ypos>Nypos ){
                ypos--;
                setRGB( xpos, ypos, ColorLineG );
            }
            while( ypos!=Nypos ){
                ypos++;
                setRGB( xpos, ypos, ColorLineG );
            }
            ///
        }
    }else{
        for( int i=0; i<MaxPos-1; i++ ){
            xpos = (int)( (i*((double)WGR_1))/MaxPos );
            ypos = ( HGR_1 - ( HGR_1 * (int)SPTR[i] )/32768 );
            setRGB( xpos, ypos, ColorLineG );

        }
    }
    ///
    if( !HoldV ){
        FillBuff();
    }
    return TRUE;
}

/// /////////////////////////////////////////////////////////////////////////////////////////////////
/// /////////////////////////////////////////////////////////////////////////////////////////////////
/// /////////////////////////////////////////////////////////////////////////////////////////////////
/// /////////////////////////////////////////////////////////////////////////////////////////////////
/// /////////////////////////////////////////////////////////////////////////////////////////////////
/// /////////////////////////////////////////////////////////////////////////////////////////////////
/// /////////////////////////////////////////////////////////////////////////////////////////////////
/// /////////////////////////////////////////////////////////////////////////////////////////////////
/// /////////////////////////////////////////////////////////////////////////////////////////////////
/// /////////////////////////////////////////////////////////////////////////////////////////////////
/// /////////////////////////////////////////////////////////////////////////////////////////////////
BOOL PLOTEOS::PLOT_SPECTOGRAM( BYTE* BBuff, int x_s, int y_s, BOOL ForcePlot ){
    /// asd1
    if( !ForcePlot ){
        /// return FALSE;
    }
    if( BBuff==NULL ){
        return FALSE;
    }
    /// const int WGR_1 = WidthGraph - 1;
    /// const int HGR_1 = HeightGraph - 1;
    if( WSPEC!=x_s || HSPEC!=y_s || HBSPECT==NULL || RGB_SPECT==NULL ){
        if( RGB_SPECT!=NULL ){
            free( RGB_SPECT );
            RGB_SPECT = NULL;
        }
        WSPEC = x_s;
        HSPEC = y_s;
        RGB_SPECT = (RGBQUAD*)malloc( 4 * WSPEC * HSPEC + 1024 );
        if( HBSPECT!=NULL ){
            DeleteObject( HBSPECT );
        }
        HBSPECT = NewBitmap( WSPEC, HSPEC );
    }
    ZeroMemory( RGB_SPECT, 4 * WSPEC * HSPEC );
    ///
    ///
    ///
    /// MIO_MNG.SaveMatFile_SPECT( "E:\\AIIF\\SegAll\\CONOCIMIENTO\\UNIVERSIDAD\\TESIS\\P_NUEVO\\APLICACIONES\\MATLAB\\W_B\\AWV.mat", "PIC", BBuff, x_s, y_s );
    ///
    ///
    ///
    ///
    RGBQUAD ColorLineT = {0};
    ColorLineT.rgbRed = 255;
    ColorLineT.rgbGreen = 0;
    ColorLineT.rgbBlue = 0;
    for( int y = 0; y<HSPEC; y++ ){
        for( int x = 0; x<WSPEC; x++ ){
            ColorLineT.rgbRed = BBuff[(y)*WSPEC + x];
            setRGB_Spec( x, y, ColorLineT );
            //*/
            setRGB_Spec( x, y-6, ColorLineT );
            setRGB_Spec( x, y-5, ColorLineT );
            setRGB_Spec( x, y-4, ColorLineT );
            setRGB_Spec( x, y-3, ColorLineT );
            setRGB_Spec( x, y-2, ColorLineT );
            setRGB_Spec( x, y-1, ColorLineT );
            setRGB_Spec( x, y+1, ColorLineT );
            setRGB_Spec( x, y+2, ColorLineT );
            setRGB_Spec( x, y+3, ColorLineT );
            setRGB_Spec( x, y+4, ColorLineT );
            setRGB_Spec( x, y+5, ColorLineT );
            setRGB_Spec( x, y+6, ColorLineT );
            //*/
        }
    }
    ///
    ///
    ///
    return PLOT_SPECTOGRAM_OPT( ForcePlot );
    /*
    SetBitmapBits( HBSPECT, 4 * WSPEC * HSPEC, &RGB_SPECT[0] );
    ///
    HDC hdc = CreateCompatibleDC( GetDC( 0 ) );
    HDC memDC = CreateCompatibleDC( GetDC( 0 ) );
    SelectObject( hdc , HBSPECT );
    SelectObject( memDC , HBitmapP );
    SetStretchBltMode( memDC , HALFTONE|COLORONCOLOR );
    SetStretchBltMode( memDC , COLORONCOLOR );
    SetStretchBltMode( memDC , HALFTONE );
    StretchBlt( memDC, 0, 0, WidthGraph, HeightGraph, hdc, 0, 0, WSPEC, HSPEC, SRCCOPY );
    ReleaseDC( NULL, hdc );
    ReleaseDC( NULL, memDC );
    DeleteDC( hdc );
    DeleteDC( memDC );
    ///
    ///
    ///
    ///
    GetBitmapBits( HBitmapP, 4*WidthGraph*HeightGraph, &RGB_BUFF[0]);
    int IMax = -1;
    for( int n=0; n<WidthGraph*HeightGraph; n++ ){
        IMax = max( IMax, RGB_BUFF[n].rgbRed );
    }
    if( IMax==0 ){
        IMax = 1;
    }
    for( int n=0; n<WidthGraph*HeightGraph; n++ ){
        RGB_BUFF[n].rgbRed = (BYTE)( 255 * ( ((double)RGB_BUFF[n].rgbRed)/IMax ) );
        //RGB_BUFF[n] = jet[255-RGB_BUFF[n].rgbRed];
        //RGB_BUFF[n] = jet[RGB_BUFF[n].rgbRed];
    }
    ///
    ///
    const int W10 = WidthGraph/10;
    for( int y=0; y<4; y++ ){
        for( int x=W10; x<WidthGraph; x = x + W10 ){
            RGB_BUFF[y*WidthGraph+x] = WHITECOLOR;
            RGB_BUFF[(HGR_1-y)*WidthGraph+x] = WHITECOLOR;
        }
    }
    SetBitmapBits( HBitmapP, 4*WidthGraph*HeightGraph, &RGB_BUFF[0]);
    SendMessage( HWGraph, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)HBitmapP );
    return TRUE;
    //*/
}
/// /////////////////////////////////////////////////////////////////////////////////////////////////
/// /////////////////////////////////////////////////////////////////////////////////////////////////
/// /////////////////////////////////////////////////////////////////////////////////////////////////
/// /////////////////////////////////////////////////////////////////////////////////////////////////
/// /////////////////////////////////////////////////////////////////////////////////////////////////
/// /////////////////////////////////////////////////////////////////////////////////////////////////
/// /////////////////////////////////////////////////////////////////////////////////////////////////
/// /////////////////////////////////////////////////////////////////////////////////////////////////
/// /////////////////////////////////////////////////////////////////////////////////////////////////
/// /////////////////////////////////////////////////////////////////////////////////////////////////
/// /////////////////////////////////////////////////////////////////////////////////////////////////
BOOL PLOTEOS::PLOT_SPECTOGRAM_OPT( BOOL ForcePlot ){
    ShowWindow( HW_SPECTOPT, 1 );
    SetBitmapBits( HBSPECT, 4 * WSPEC * HSPEC, &RGB_SPECT[0] );
    ///
    ZeroMemory( RGB_BUFF, 4*WidthGraph*HeightGraph );
    SetBitmapBits( HBitmapP, 4*WidthGraph*HeightGraph, &RGB_BUFF[0] );
    ///
    HDC hdc = CreateCompatibleDC( GetDC( 0 ) );
    HDC memDC = CreateCompatibleDC( GetDC( 0 ) );
    SelectObject( hdc , HBSPECT );
    SelectObject( memDC , HBitmapP );
    //SetStretchBltMode( memDC , HALFTONE|COLORONCOLOR );
    ///SetStretchBltMode( memDC , COLORONCOLOR );
    SetStretchBltMode( memDC , HALFTONE );
    StretchBlt( memDC, 0, 0, WidthGraph, HeightGraph, hdc, 0, 0, WSPEC, HSPEC, SRCCOPY );
    ReleaseDC( NULL, hdc );
    ReleaseDC( NULL, memDC );
    DeleteDC( hdc );
    DeleteDC( memDC );
    ///
    ///
    ///
    ///
    GetBitmapBits( HBitmapP, 4*WidthGraph*HeightGraph, &RGB_BUFF[0]);
    int IMax = -1;
    for( int n=0; n<WidthGraph*HeightGraph; n++ ){
        IMax = max( IMax, RGB_BUFF[n].rgbRed );
    }
    if( IMax==0 ){
        IMax = 1;
    }
    /// SpectOpt = 1;
    for( int n=0; n<WidthGraph*HeightGraph; n++ ){
        RGB_BUFF[n].rgbRed = (BYTE)ParceBYTE( round(255*( ((double)RGB_BUFF[n].rgbRed)/IMax )) );
    }
    ///
    const int W10 = WidthGraph/10;
    const int HGR_1 = HeightGraph - 1;
    switch( SpectOpt ){ ///         BLACKCOLOR              WHITECOLOR
        case 0:
            for( int y=0; y<4; y++ ){
                for( int x=W10; x<WidthGraph; x = x + W10 ){
                    RGB_BUFF[y*WidthGraph+x] = WHITECOLOR;
                    RGB_BUFF[(HGR_1-y)*WidthGraph+x] = WHITECOLOR;
                }
            }
            break;
        case 1:
            for( int n=0; n<WidthGraph*HeightGraph; n++ ){
                /// RGB_BUFF[n].rgbRed = 255 - (BYTE)round( 255 * ( ((double)RGB_BUFF[n].rgbRed)/IMax ) );
                RGB_BUFF[n].rgbRed = RGB_BUFF[n].rgbGreen = RGB_BUFF[n].rgbBlue = (RGB_BUFF[n].rgbRed);
            }
            for( int y=0; y<4; y++ ){
                for( int x=W10; x<WidthGraph; x = x + W10 ){
                    RGB_BUFF[y*WidthGraph+x] = WHITECOLOR;
                    RGB_BUFF[(HGR_1-y)*WidthGraph+x] = WHITECOLOR;
                }
            }
            break;
        case 2:
            for( int n=0; n<WidthGraph*HeightGraph; n++ ){
                /// RGB_BUFF[n].rgbRed = 255 - (BYTE)round( 255 * ( ((double)RGB_BUFF[n].rgbRed)/IMax ) );
                RGB_BUFF[n].rgbRed = RGB_BUFF[n].rgbGreen = RGB_BUFF[n].rgbBlue = (255 - RGB_BUFF[n].rgbRed);
            }
            for( int y=0; y<4; y++ ){
                for( int x=W10; x<WidthGraph; x = x + W10 ){
                    RGB_BUFF[y*WidthGraph+x] = BLACKCOLOR;
                    RGB_BUFF[(HGR_1-y)*WidthGraph+x] = BLACKCOLOR;
                }
            }
            break;
        case 3:
            for( int n=0; n<WidthGraph*HeightGraph; n++ ){
                RGB_BUFF[n] = jet[255-RGB_BUFF[n].rgbRed];
            }
            for( int y=0; y<4; y++ ){
                for( int x=W10; x<WidthGraph; x = x + W10 ){
                    RGB_BUFF[y*WidthGraph+x] = WHITECOLOR;
                    RGB_BUFF[(HGR_1-y)*WidthGraph+x] = WHITECOLOR;
                }
            }
            break;
        case 4:
            for( int n=0; n<WidthGraph*HeightGraph; n++ ){
                RGB_BUFF[n] = jet[RGB_BUFF[n].rgbRed];
            }
            for( int y=0; y<4; y++ ){
                for( int x=W10; x<WidthGraph; x = x + W10 ){
                    RGB_BUFF[y*WidthGraph+x] = WHITECOLOR;
                    RGB_BUFF[(HGR_1-y)*WidthGraph+x] = WHITECOLOR;
                }
            }
            break;
        default:
            for( int n=0; n<WidthGraph*HeightGraph; n++ ){
                /// RGB_BUFF[n].rgbRed = 255 - (BYTE)round( 255 * ( ((double)RGB_BUFF[n].rgbRed)/IMax ) );
                RGB_BUFF[n].rgbRed = RGB_BUFF[n].rgbGreen = RGB_BUFF[n].rgbBlue = (255 - RGB_BUFF[n].rgbRed);
            }
            for( int y=0; y<4; y++ ){
                for( int x=W10; x<WidthGraph; x = x + W10 ){
                    RGB_BUFF[y*WidthGraph+x] = WHITECOLOR;
                    RGB_BUFF[(HGR_1-y)*WidthGraph+x] = WHITECOLOR;
                }
            }
            break;

    }
    SetBitmapBits( HBitmapP, 4*WidthGraph*HeightGraph, &RGB_BUFF[0]);
    SendMessage( HWGraph, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)HBitmapP );
    ///
    ///
    return TRUE;
}

/// /////////////////////////////////////////////////////////////////////////////////////////////////
/// /////////////////////////////////////////////////////////////////////////////////////////////////
/// /////////////////////////////////////////////////////////////////////////////////////////////////
/// /////////////////////////////////////////////////////////////////////////////////////////////////
/// /////////////////////////////////////////////////////////////////////////////////////////////////
/// /////////////////////////////////////////////////////////////////////////////////////////////////
/// /////////////////////////////////////////////////////////////////////////////////////////////////
BOOL PLOTEOS::PLOT_DOUBLE_FFT( double* DBuff, int MaxPos, int InitFrames, BOOL ForcePlot ){
    if( MaxPos<=0 ){
        FillGrid( TRUE );
        return FALSE;
    }
    ///
    if( DBuff==NULL ){
        return FALSE;
    }
    ///
    if( !BVisible && ForcePlot==FALSE ){
        return FALSE;
    }
    ///
    if( !HoldV ){
        FillGrid( FALSE );
    }
    const int WGR_1 = WidthGraph - 1;
    const int HGR_1 = HeightGraph - 1;
    ///                                                           ///
    double MaxVal = -1e100;
    for( int i=0; i<MaxPos; i++ ){
        MaxVal = fmax( MaxVal, DBuff[i] );
    }
    ///                                                           ///
    if( MaxVal<1 && MaxVal>0.099 ){
        MaxVal = 1;
    }
    if( MaxVal<1 ){
        //MaxVal = 1;
    }
    ///                                                           ///
    int xpos = 0;
    int Nxpos = 0;
    int ypos = 0;
    int Nypos = 0;
    //if( MaxPos<500000 || ForcePlot || TRUE ){/// 2400000
    if( MaxPos>0 || TRUE ){/// 2400000
    ///if( MaxPos<1  ){/// 2400000
        for( int i=0; i<MaxPos-1; i++ ){
            xpos = (int)( (i*((double)WGR_1))/MaxPos );
            ypos = (int)( ( HGR_1 * (MaxVal - DBuff[i]) )/MaxVal );
            setRGB( xpos, ypos, ColorLineG );
            ///
            ///
            Nxpos = (int)( ((i+1)*(double)WGR_1)/MaxPos );
            Nypos = ( HGR_1 * ( MaxVal - DBuff[i+1] ) )/MaxVal;
            if( Nxpos==xpos && Nypos==ypos ){
                i++;
                continue;
            }
            /// if( Nxpos!=xpos && Nypos==ypos ){ i++;continue; }
            while( xpos!=Nxpos ){
                xpos++;
                setRGB( xpos, ypos, ColorLineG );
            }
            ///
            while( ypos>Nypos ){
                ypos--;
                setRGB( xpos, ypos, ColorLineG );
            }
            while( ypos!=Nypos ){
                ypos++;
                setRGB( xpos, ypos, ColorLineG );
            }
            ///
        }
    }else{
        for( int i=0; i<MaxPos-1; i++ ){
            xpos = (int)( (i*((double)WGR_1))/MaxPos );
            ypos = (int)( ( HGR_1 * (MaxVal - DBuff[i]) )/MaxVal );
            setRGB( xpos, ypos, ColorLineG );
        }
    }
    ///
    if( !HoldV ){
        FillBuff();
    }
    return TRUE;
}

/// /////////////////////////////////////////////////////////////////////////////////////////////////
/// /////////////////////////////////////////////////////////////////////////////////////////////////
/// /////////////////////////////////////////////////////////////////////////////////////////////////
/// /////////////////////////////////////////////////////////////////////////////////////////////////
/// /////////////////////////////////////////////////////////////////////////////////////////////////
/// /////////////////////////////////////////////////////////////////////////////////////////////////
/// /////////////////////////////////////////////////////////////////////////////////////////////////
BOOL PLOTEOS::PLOT_HIST( double* IBuff, int MaxPos, int InitFrames, BOOL ForcePlot ){
    if( MaxPos<=0 ){
        FillGrid( TRUE );
        return FALSE;
    }
    ///
    if( IBuff==NULL ){
        return FALSE;
    }
    ///
    if( !BVisible && ForcePlot==FALSE ){
        return FALSE;
    }
    ///
    if( !HoldV ){
        FillGrid( FALSE );
    }
    const int WGR_1 = WidthGraph - 1;
    const int HGR_1 = HeightGraph - 1;
    ///                                                           ///
    double MaxVal = -1;
    for( int i=0; i<MaxPos; i++ ){
        MaxVal = fmax( MaxVal, IBuff[i] );
    }
    ///                                                           ///
    if( MaxVal<1 ){
        MaxVal = 1;
    }
    SetMinYLim( 0 );
    SetMaxYLim( MaxVal );
    ///                                                           ///
    int xpos = 0;
    int Nxpos = 0;
    int ypos = 0;
    int Nypos = 0;
    //if( MaxPos<500000 || ForcePlot || TRUE ){/// 2400000
    if( MaxPos>0 || TRUE ){/// 2400000
    ///if( MaxPos<1  ){/// 2400000
        for( int i=0; i<MaxPos-1; i++ ){
            xpos = (int)( (i*((double)WGR_1))/MaxPos );
            ypos = (int)( ( HGR_1 * (MaxVal - (double)IBuff[i]) )/MaxVal );
            setRGB( xpos, ypos, ColorLineG );
            ///
            ///
            Nxpos = (int)( ((i+1)*(double)WGR_1)/MaxPos );
            Nypos = (int)(( HGR_1 * ( MaxVal - (double)IBuff[i+1] ) )/MaxVal);
            if( Nxpos==xpos && Nypos==ypos ){
                i++;
                continue;
            }
            /// if( Nxpos!=xpos && Nypos==ypos ){ i++;continue; }
            while( xpos!=Nxpos ){
                xpos++;
                setRGB( xpos, ypos, ColorLineG );
            }
            ///
            while( ypos>Nypos ){
                ypos--;
                setRGB( xpos, ypos, ColorLineG );
            }
            while( ypos!=Nypos ){
                ypos++;
                setRGB( xpos, ypos, ColorLineG );
            }
            ///
        }
    }else{
        for( int i=0; i<MaxPos-1; i++ ){
            xpos = (int)( (i*((double)WGR_1))/MaxPos );
            ypos = (int)( ( HGR_1 * (MaxVal - (double)IBuff[i]) )/MaxVal );
            setRGB( xpos, ypos, ColorLineG );
        }
    }
    ///
    if( !HoldV ){
        FillBuff();
    }
    return TRUE;
}
