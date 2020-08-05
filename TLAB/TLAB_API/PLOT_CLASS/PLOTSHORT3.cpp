BOOL PLOTEOS::PLOTSHORT3( short* SBuff0, short* SBuff1, short* SBuff2, int MaxPos, int TopBreak,
                                                int InitFrames, BOOL ForcePlot ){
    if( MaxPos<=0 ){
        FillGrid( TRUE );
        return FALSE;
    }
    ///
    if( PLTS==NULL && SBuff0==NULL ){
        return FALSE;
    }
    ///
    if( SBuff0!=NULL ){
        PLT0 = SBuff0;
        PLT1 = SBuff1;
        PLT2 = SBuff2;
    }
    ///
    if( !BVisible && ForcePlot==FALSE ){
        return FALSE;
    }
    ///
    short* SPTR0 = (short*)PLT0;
    short* SPTR1 = (short*)PLT1;
    short* SPTR2 = (short*)PLT2;
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
        for( int i=0; i<TopBreak-1; i++ ){
            /// C H A N N 0  ////////////////////////////////////////////////////////////////////////////////////
            xpos = (int)( (i*((double)WGR_1))/MaxPos );
            ypos = ( HGR_1 - ( HGR_1 * (int)SPTR0[i] )/32767 );
            setRGB( xpos, ypos, LineColors[0] );
            ///
            ///
            Nxpos = (int)( ((i+1)*(double)WGR_1)/MaxPos );
            Nypos = ( HGR_1 - ( HGR_1 * (int)SPTR0[i+1] )/32767 );
            if( Nxpos!=xpos || Nypos!=ypos ){
                while( xpos!=Nxpos ){
                    xpos++;
                    setRGB( xpos, ypos, LineColors[0] );
                }
                ///
                while( ypos>Nypos ){
                    ypos--;
                    setRGB( xpos, ypos, LineColors[0] );
                }
                while( ypos!=Nypos ){
                    ypos++;
                    setRGB( xpos, ypos, LineColors[0] );
                }
            }
            ///
            ///
            ///
            ///
            /// C H A N N 1  ////////////////////////////////////////////////////////////////////////////////////
            xpos = (int)( (i*((double)WGR_1))/MaxPos );
            ypos = ( HGR_1 - ( HGR_1 * (int)SPTR1[i] )/32767 );
            setRGB( xpos, ypos, LineColors[1] );
            ///
            ///
            Nxpos = (int)( ((i+1)*(double)WGR_1)/MaxPos );
            Nypos = ( HGR_1 - ( HGR_1 * (int)SPTR1[i+1] )/32767 );
            if( Nxpos!=xpos || Nypos!=ypos ){
                while( xpos!=Nxpos ){
                    xpos++;
                    setRGB( xpos, ypos, LineColors[1] );
                }
                ///
                while( ypos>Nypos ){
                    ypos--;
                    setRGB( xpos, ypos, LineColors[1] );
                }
                while( ypos!=Nypos ){
                    ypos++;
                    setRGB( xpos, ypos, LineColors[1] );
                }
            }
            ///
            ///
            ///
            ///
            ///
            /// C H A N N 2  ////////////////////////////////////////////////////////////////////////////////////
            xpos = (int)( (i*((double)WGR_1))/MaxPos );
            ypos = ( HGR_1 - ( HGR_1 * (int)SPTR2[i] )/32767 );
            setRGB( xpos, ypos, LineColors[2] );
            ///
            ///
            Nxpos = (int)( ((i+1)*(double)WGR_1)/MaxPos );
            Nypos = ( HGR_1 - ( HGR_1 * (int)SPTR2[i+1] )/32767 );
            if( Nxpos!=xpos || Nypos!=ypos ){
                while( xpos!=Nxpos ){
                    xpos++;
                    setRGB( xpos, ypos, LineColors[2] );
                }
                ///
                while( ypos>Nypos ){
                    ypos--;
                    setRGB( xpos, ypos, LineColors[2] );
                }
                while( ypos!=Nypos ){
                    ypos++;
                    setRGB( xpos, ypos, LineColors[2] );
                }
            }
            ///
        }
    }else{
        for( int i=0; i<TopBreak-1; i++ ){
            /*
            /// CHANN1
            xpos = (int)( (i*((double)WGR_1))/MaxPos );
            ypos = ( HGR_1 - ( HGR_1 * (int)SPTR0[i] )/32767 );
            setRGB( xpos, ypos, LineColors[0] );
            ///
            ///
            /// CHANN1
            xpos = (int)( (i*((double)WGR_1))/MaxPos );
            ypos = ( HGR_1 - ( HGR_1 * (int)SPTR1[i] )/32767 );
            setRGB( xpos, ypos, LineColors[1] );
            ///
            ///
            /// CHANN2
            xpos = (int)( (i*((double)WGR_1))/MaxPos );
            ypos = ( HGR_1 - ( HGR_1 * (int)SPTR2[i] )/32767 );
            setRGB( xpos, ypos, LineColors[2] );
            ///
            //*/
            ///
            ///

            /// C H A N N 0  ////////////////////////////////////////////////////////////////////////////////////
            xpos = (int)( (i*((double)WGR_1))/MaxPos );
            ypos = ( HGR_1 - ( HGR_1 * (int)SPTR0[i] )/32767 );
            setRGB( xpos, ypos, LineColors[0] );
            ///
            ///
            Nxpos = (int)( ((i+1)*(double)WGR_1)/MaxPos );
            Nypos = ( HGR_1 - ( HGR_1 * (int)SPTR0[i+1] )/32767 );
            if( Nypos!=ypos ){
                ///
                while( ypos>Nypos ){
                    ypos--;
                    setRGB( xpos, ypos, LineColors[0] );
                }
                while( ypos!=Nypos ){
                    ypos++;
                    setRGB( xpos, ypos, LineColors[0] );
                }
            }
            ///
            ///
            ///
            ///
            /// C H A N N 1  ////////////////////////////////////////////////////////////////////////////////////
            xpos = (int)( (i*((double)WGR_1))/MaxPos );
            ypos = ( HGR_1 - ( HGR_1 * (int)SPTR1[i] )/32767 );
            setRGB( xpos, ypos, LineColors[1] );
            ///
            ///
            Nxpos = (int)( ((i+1)*(double)WGR_1)/MaxPos );
            Nypos = ( HGR_1 - ( HGR_1 * (int)SPTR1[i+1] )/32767 );
            if( Nypos!=ypos ){
                ///
                while( ypos>Nypos ){
                    ypos--;
                    setRGB( xpos, ypos, LineColors[1] );
                }
                while( ypos!=Nypos ){
                    ypos++;
                    setRGB( xpos, ypos, LineColors[1] );
                }
            }
            ///
            ///
            ///
            ///
            ///
            /// C H A N N 2  ////////////////////////////////////////////////////////////////////////////////////
            xpos = (int)( (i*((double)WGR_1))/MaxPos );
            ypos = ( HGR_1 - ( HGR_1 * (int)SPTR2[i] )/32767 );
            setRGB( xpos, ypos, LineColors[2] );
            ///
            ///
            Nxpos = (int)( ((i+1)*(double)WGR_1)/MaxPos );
            Nypos = ( HGR_1 - ( HGR_1 * (int)SPTR2[i+1] )/32767 );
            if( Nypos!=ypos ){
                ///
                while( ypos>Nypos ){
                    ypos--;
                    setRGB( xpos, ypos, LineColors[2] );
                }
                while( ypos!=Nypos ){
                    ypos++;
                    setRGB( xpos, ypos, LineColors[2] );
                }
            }
            ///
            ///
            ///
        }
    }
    ///
    if( !HoldV ){
        FillBuff();
    }
    return TRUE;
}

