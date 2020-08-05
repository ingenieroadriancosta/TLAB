BOOL PLOTEOS::PLOTDOUBLE( double* DBuff, int MaxPos, int InitFrames, BOOL ForcePlot ){
    if( MaxPos<=0 ){
        FillGrid( TRUE );
        return FALSE;
    }
    ///
    ///
    double MaxV = -1e100;
    for( int i=0; i<MaxPos; i++ ){
        MaxV = fmax( MaxV, fabs(DBuff[i]) );
    }
    if( MaxV<1 ){
        MaxV = 1;
    }
    ///
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
    int xpos = 0;
    int Nxpos = 0;
    int ypos = 0;
    int Nypos = 0;
    if( MaxPos<500000 || ForcePlot ){/// 2400000
    /// if( MaxPos>0 ){/// 2400000
        for( int i=0; i<MaxPos-1; i++ ){
            xpos = (int)( (i*((double)WGR_1))/MaxPos );
            ypos = (int)( HGR_1 - ( HGR_1 * DBuff[i] )/MaxV );
            setRGB( xpos, ypos, ColorLineG );
            ///
            ///
            Nxpos = (int)( ((i+1)*(double)WGR_1)/MaxPos );
            Nypos = (int)( HGR_1 - ( HGR_1 * DBuff[i+1] )/MaxV );
            if( Nxpos==xpos && Nypos==ypos ){
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
            ypos = (int)( HGR_1 - ( HGR_1 * DBuff[i] )/MaxV );
            setRGB( xpos, ypos, ColorLineG );

        }
    }
    ///
    if( !HoldV ){
        FillBuff();
    }
    return TRUE;
}
/// ///////////////////////////////////////////////////////////////////////////////////////////////////////
/// ///////////////////////////////////////////////////////////////////////////////////////////////////////
/// ///////////////////////////////////////////////////////////////////////////////////////////////////////
/// ///////////////////////////////////////////////////////////////////////////////////////////////////////
/// ///////////////////////////////////////////////////////////////////////////////////////////////////////
/// ///////////////////////////////////////////////////////////////////////////////////////////////////////
#define PLOTBIPOL_DEFINE 0
BOOL PLOTEOS::PLOTDOUBLE_BIPOL( double* DBuff, int MaxPos, int InitFrames, BOOL ForcePlot ){
    return PLOTDOUBLE_BIPOL( DBuff, MaxPos, InitFrames, ForcePlot, 64 );
}
BOOL PLOTEOS::PLOTDOUBLE_BIPOL( double* DBuff, int MaxPos, int InitFrames, BOOL ForcePlot, int NAdvIn ){
    if( MaxPos<=0 ){
        FillGrid( TRUE );
        return FALSE;
    }
    ///
    ///
    double MaxV = -1e100;
    for( int i=0; i<MaxPos; i++ ){
        MaxV = fmax( MaxV, fabs(DBuff[i]) );
    }
    if( MaxV<1 ){
        MaxV = 1;
    }
    SetMaxYLim( MaxV );
    SetMinYLim( -MaxV );
    Redraw();
    ///
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
    const double HGDBL = 0.5 * HGR_1;
    const double MaxVDiv = HGDBL/MaxV;
    ///                                                           ///
    int xpos = 0;
    int Nxpos = 0;
    int ypos = 0;
    int Nypos = 0;
    int NAdv = 1;
    if( MaxPos>(MaxFileSize/2) ){
        NAdv = NAdvIn;
    }else{
        if( MaxPos>(MaxFileSize/3) ){
            NAdv = NAdvIn/2;
        }else{
            if( MaxPos>(MaxFileSize/4) ){
                NAdv = NAdvIn/4;
            }else{
                NAdv = 1;
            }
        }
    }
    if( MaxPos<500000 || ForcePlot || TRUE ){/// 2400000
    /// if( MaxPos>0 ){/// 2400000
        /// NAdv = 2;
        for( int i=0; i<MaxPos-1; i = i + NAdv ){
            xpos = (int)( (i*((double)WGR_1))/MaxPos );
            ypos = (int)( HGDBL - MaxVDiv * DBuff[i] );
            setRGB( xpos, ypos, ColorLineG );
            ///
            ///
            ///
            ///
            Nxpos = (int)( ((i+NAdv)*(double)WGR_1)/MaxPos );
            Nypos = (int)( HGDBL - MaxVDiv * DBuff[i+NAdv] );
            if( Nxpos==xpos && Nypos==ypos ){
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
            ///
            ///
            ///
        }
    }else{
        for( int i=0; i<MaxPos-1; i++ ){
            xpos = (int)( (i*((double)WGR_1))/MaxPos );
            ypos = (int)( HGR_1 - ( HGR_1 * DBuff[i] )/MaxV );
            setRGB( xpos, ypos, ColorLineG );

        }
    }
    ///
    if( !HoldV && ForcePlot ){
        FillBuff();
    }
    return TRUE;
}
