BOOL PLOTEOS::PLOTSHORT( short* SBuff, int MaxPos, int InitFrames, BOOL ForcePlot ){
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
        for( int i=0; i<MaxPos-1; i++ ){
            xpos = (int)( (i*((double)WGR_1))/MaxPos );
            ypos = ( HGR_1 - ( HGR_1 * (int)SPTR[i] )/32768 )/2;
            setRGB( xpos, ypos, ColorLineG );
            ///
            ///
            Nxpos = (int)( ((i+1)*(double)WGR_1)/MaxPos );
            Nypos = ( HGR_1 - ( HGR_1 * (int)SPTR[i+1] )/32768 )/2;
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
            ///
        }
    }else{
        for( int i=0; i<MaxPos-1; i++ ){
            xpos = (int)( (i*((double)WGR_1))/MaxPos );
            ypos = ( HGR_1 - ( HGR_1 * (int)SPTR[i] )/32768 )/2;
            setRGB( xpos, ypos, ColorLineG );

        }
    }
    ///
    if( !HoldV ){
        FillBuff();
    }
    return TRUE;
}
