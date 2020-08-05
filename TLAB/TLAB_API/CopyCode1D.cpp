BOOL TLAB::CopyCode1D( LPSTR T2T, BOOL* writewigner, BOOL* writechoi_william ){
    char* TextT = (char*)malloc( 1024 * 1024 );
    ZeroMemory( TextT, 1024 * 1024 - 1 );
    ///
    ///
    ///

    strcat( T2T, TextT );
    strcat( T2T, "clc;close all;clear all;\n\n" );
    ///
    strcat( T2T, "\n\ntic\n\n" );
    ///
    switch( Dim1Procs[0].SLOAD.SType ){
        case 0:
            sprintf( TextT, "[y0, Fs] = audioread( '%s' ); \n"
                    "y0 = y0( :, %i );\ny0 = y0(:);"
                    "\n\n",
                    Dim1Procs[0].SLOAD.FName,
                    Dim1Procs[0].SLOAD.ChannSel+1 );
            break;
        case 1:
            sprintf( TextT, "Fs = %i;\ny0 = load( '%s', '%s' ); \n"
                    "y0 = y0.%s(:);"
                    "\n\n",
                    Dim1Procs[0].SLOAD.Fs, Dim1Procs[0].SLOAD.FName,
                    Dim1Procs[0].SLOAD.VarName,
                    Dim1Procs[0].SLOAD.VarName );
            break;
        case 2:
            sprintf( TextT, "Fs = %i;\ny0 = sin( 2 * pi * %g * (0:%i-1)/Fs ); \ny0 = y0(:);\n"
                    "\n\n", Dim1Procs[0].SLOAD.Fs,
                    Dim1Procs[0].SLOAD.f,
                    Dim1Procs[0].SLOAD.LOfVar );
            break;
    }
    strcat( T2T, TextT );
    ///
    ///
    ///
    strcat( T2T, "FNyq = Fs/2;\n" );
    /// strcat( T2T, "aup = audioplayer(y0, Fs);\nplay(aup)\n\n" );
    ///
    ///
    ///
    ///
    /// strcat( T2T, "clc;close all;\n\n" );
    ///
    int i_from = 0;
    for( int i=1; i<MaxProcs; i++ ){
        i_from = i-1;
        while( (Dim1Procs[i_from].ProcType==NO_PROC_Signal) ||
                        (Dim1Procs[i_from].ProcType==FFT_Signal) ||
                        (Dim1Procs[i_from].ProcType==SPECTOGRAM_Signal) ||
                        (Dim1Procs[i_from].ProcType==WIGNER_Signal) ||
                        (Dim1Procs[i_from].ProcType==CHOI_WIL_Signal) ||
                        (Dim1Procs[i_from].ProcType==PLAY_Signal) ||
                        (Dim1Procs[i_from].ProcType==Graph_Signal)
                                                            ){
            i_from--;
        }
        ///
        if( Dim1Procs[i].FDS.GetBInput()!=0 ){
            i_from = 0;
        }
        ///
        switch( Dim1Procs[i].ProcType ){
            ///
            ///
            ///
            case Graph_Signal:
                sprintf( TextT, "%%Procesamiento del módulo %i(Graficar)\n"
                        "figure;\nplot( y%i );\n", i, i_from );
                strcat( T2T, TextT );
                sprintf( TextT, "title( 'Módulo %i' );\ndrawnow;\n", i );
                strcat( T2T, TextT );
                ///
                strcat( T2T, "\n\n" );
                break;
            ///
            ///
            ///
            ///
            ///
            case Filt_Signal:
                sprintf( TextT, "%%Procesamiento del módulo %i(Filtrar)\n", i );
                strcat( T2T, TextT );
                if( Dim1Procs[i].FDS.IsIRR ){
                    switch( Dim1Procs[i].FDS.GetFilType() ){
                        case 0:
                            sprintf( TextT, "[b, a] = butter( %i, %g/FNyq, 'low' );\n",
                                            Dim1Procs[i].FDS.GetOrdFil(),
                                                Dim1Procs[i].FDS.GetFcH() );
                            break;
                        case 1:
                            sprintf( TextT, "[b, a] = butter( %i, %g/FNyq, 'high' );\n",
                                            Dim1Procs[i].FDS.GetOrdFil(),
                                                Dim1Procs[i].FDS.GetFcL() );
                            break;
                        case 2:
                            sprintf( TextT, "[b, a] = butter( %i, [%g, %g]/FNyq, 'bandpass' );\n",
                                            Dim1Procs[i].FDS.GetOrdFil(),
                                                Dim1Procs[i].FDS.GetFcL(), Dim1Procs[i].FDS.GetFcH() );
                            break;
                        case 3:
                            sprintf( TextT, "[b, a] = butter( %i, [%g, %g]/FNyq, 'stop' );\n",
                                            Dim1Procs[i].FDS.GetOrdFil(),
                                                Dim1Procs[i].FDS.GetFcL(), Dim1Procs[i].FDS.GetFcH() );
                            break;
                    }
                    strcat( T2T, TextT );
                    sprintf( TextT, "H = fft( b, length(y%i) )./fft( a, length(y%i) );\n",
                                    i_from, i_from );
                    strcat( T2T, TextT );
                    sprintf( TextT, "Y = fft( y%i, length(y%i) );\n",
                                    i_from, i_from );
                    strcat( T2T, TextT );
                    sprintf( TextT, "y%i = real( ifft( Y(:) .* H(:) ) );\n", i );
                    strcat( T2T, TextT );
                }else{
                    switch( Dim1Procs[i].FDS.GetFilType() ){
                        case 0:
                            if( Dim1Procs[i].FDS.ExistOnMatlab() ){
                                if( Dim1Procs[i].FDS.HaveAlpha() ){
                                    sprintf( TextT, "b = fir1( %i, %g/FNyq, 'low'%s( %i, %g ) );\n",
                                                Dim1Procs[i].FDS.GetOrdFil(),
                                                        Dim1Procs[i].FDS.GetFcH(),
                                                            &Wind_2_Fir_name[Dim1Procs[i].FDS.GetWType()][0],
                                                Dim1Procs[i].FDS.GetOrdFil()+1, Dim1Procs[i].FDS.GetAlpha() );
                                    strcat( T2T, TextT );
                                }else{
                                    sprintf( TextT, "b = fir1( %i, %g/FNyq, 'low'%s( %i ) );\n",
                                                Dim1Procs[i].FDS.GetOrdFil(),
                                                        Dim1Procs[i].FDS.GetFcH(),
                                                            &Wind_2_Fir_name[Dim1Procs[i].FDS.GetWType()][0],
                                                Dim1Procs[i].FDS.GetOrdFil()+1 );
                                    strcat( T2T, TextT );
                                }
                            }else{
                                sprintf( TextT, "b = fir1( %i, %g/FNyq, 'low'%s( %i ) );%% La ventana seleccionada no existe en matlab.\n",
                                            Dim1Procs[i].FDS.GetOrdFil(),
                                                    Dim1Procs[i].FDS.GetFcH(),
                                                        &Wind_2_Fir_name[0][0],
                                            Dim1Procs[i].FDS.GetOrdFil()+1 );
                                strcat( T2T, TextT );
                            }
                            break;
                        case 1:
                            if( Dim1Procs[i].FDS.ExistOnMatlab() ){
                                if( Dim1Procs[i].FDS.HaveAlpha() ){
                                    sprintf( TextT, "b = fir1( %i, %g/FNyq, 'high'%s( %i, %g ) );\n",
                                                Dim1Procs[i].FDS.GetOrdFil(),
                                                        Dim1Procs[i].FDS.GetFcL(),
                                                            &Wind_2_Fir_name[Dim1Procs[i].FDS.GetWType()][0],
                                                Dim1Procs[i].FDS.GetOrdFil()+1, Dim1Procs[i].FDS.GetAlpha() );
                                    strcat( T2T, TextT );
                                }else{
                                    sprintf( TextT, "b = fir1( %i, %g/FNyq, 'high'%s( %i ) );\n",
                                                Dim1Procs[i].FDS.GetOrdFil(),
                                                        Dim1Procs[i].FDS.GetFcL(),
                                                            &Wind_2_Fir_name[Dim1Procs[i].FDS.GetWType()][0],
                                                Dim1Procs[i].FDS.GetOrdFil()+1 );
                                    strcat( T2T, TextT );
                                }
                            }else{
                                sprintf( TextT, "b = fir1( %i, %g/FNyq, 'high'%s( %i ) );%% La ventana seleccionada no existe en matlab.\n",
                                            Dim1Procs[i].FDS.GetOrdFil(),
                                                    Dim1Procs[i].FDS.GetFcL(),
                                                        &Wind_2_Fir_name[0][0],
                                            Dim1Procs[i].FDS.GetOrdFil()+1 );
                                strcat( T2T, TextT );
                            }
                            break;
                        case 2:
                            if( Dim1Procs[i].FDS.ExistOnMatlab() ){
                                if( Dim1Procs[i].FDS.HaveAlpha() ){
                                    sprintf( TextT, "b = fir1( %i, [%g, %g]/FNyq, 'DC-0'%s( %i, %g ) );\n",
                                                Dim1Procs[i].FDS.GetOrdFil(),
                                                        Dim1Procs[i].FDS.GetFcL(), Dim1Procs[i].FDS.GetFcH(),
                                                            &Wind_2_Fir_name[Dim1Procs[i].FDS.GetWType()][0],
                                                Dim1Procs[i].FDS.GetOrdFil()+1, Dim1Procs[i].FDS.GetAlpha() );
                                    strcat( T2T, TextT );
                                }else{
                                    sprintf( TextT, "b = fir1( %i, [%g, %g]/FNyq, 'DC-0'%s( %i ) );\n",
                                                Dim1Procs[i].FDS.GetOrdFil(),
                                                        Dim1Procs[i].FDS.GetFcL(), Dim1Procs[i].FDS.GetFcH(),
                                                            &Wind_2_Fir_name[Dim1Procs[i].FDS.GetWType()][0],
                                                Dim1Procs[i].FDS.GetOrdFil()+1 );
                                    strcat( T2T, TextT );
                                }
                            }else{
                                sprintf( TextT, "b = fir1( %i, [%g, %g]/FNyq, 'DC-0'%s( %i ) );%% La ventana seleccionada no existe en matlab.\n",
                                            Dim1Procs[i].FDS.GetOrdFil(),
                                                    Dim1Procs[i].FDS.GetFcL(), Dim1Procs[i].FDS.GetFcH(),
                                                        &Wind_2_Fir_name[0][0],
                                            Dim1Procs[i].FDS.GetOrdFil()+1 );
                                strcat( T2T, TextT );
                            }
                            break;
                        case 3:
                            if( Dim1Procs[i].FDS.ExistOnMatlab() ){
                                if( Dim1Procs[i].FDS.HaveAlpha() ){
                                    sprintf( TextT, "b = fir1( %i, [%g, %g]/FNyq, 'DC-1'%s( %i, %g ) );\n",
                                                Dim1Procs[i].FDS.GetOrdFil(),
                                                        Dim1Procs[i].FDS.GetFcL(), Dim1Procs[i].FDS.GetFcH(),
                                                            &Wind_2_Fir_name[Dim1Procs[i].FDS.GetWType()][0],
                                                Dim1Procs[i].FDS.GetOrdFil()+1, Dim1Procs[i].FDS.GetAlpha() );
                                    strcat( T2T, TextT );
                                }else{
                                    sprintf( TextT, "b = fir1( %i, [%g, %g]/FNyq, 'DC-1'%s( %i ) );\n",
                                                Dim1Procs[i].FDS.GetOrdFil(),
                                                        Dim1Procs[i].FDS.GetFcL(), Dim1Procs[i].FDS.GetFcH(),
                                                            &Wind_2_Fir_name[Dim1Procs[i].FDS.GetWType()][0],
                                                Dim1Procs[i].FDS.GetOrdFil()+1 );
                                    strcat( T2T, TextT );
                                }
                            }else{
                                sprintf( TextT, "b = fir1( %i, [%g, %g]/FNyq, 'DC-1%s( %i ) );%% La ventana seleccionada no existe en matlab.\n",
                                            Dim1Procs[i].FDS.GetOrdFil(),
                                                    Dim1Procs[i].FDS.GetFcL(), Dim1Procs[i].FDS.GetFcH(),
                                                        &Wind_2_Fir_name[0][0],
                                            Dim1Procs[i].FDS.GetOrdFil()+1 );
                                strcat( T2T, TextT );
                            }
                            break;
                    }
                    sprintf( TextT, "B = fft( b, length(y%i) );\n", i_from );
                    strcat( T2T, TextT );
                    sprintf( TextT, "Y = fft( y%i, length(y%i) ) .* B(:);\n", i_from, i_from );
                    strcat( T2T, TextT );
                    sprintf( TextT, "y%i = real( ifft( Y, length(y%i) ) );\n",
                                            i, i_from );
                    strcat( T2T, TextT );
                }
                sprintf( TextT, "figure;\nplot( y%i );\ntitle( 'Módulo %i' );\ndrawnow;\n", i, i );
                strcat( T2T, TextT );
                strcat( T2T, "\n\n" );
                break;
            ///
            ///
            ///
            ///
            ///
            ///   Dim1Procs[0].NPosBuff
            ///
            case Windowing_Signal:
                sprintf( TextT, "%%Procesamiento del módulo %i(Enventanado)\n", i );
                strcat( T2T, TextT );
                if( Dim1Procs[i].FDS.WINDWN.ExistOnMatlab() ){
                    if( Dim1Procs[i].FDS.WINDWN.HaveWindowAlpha() ){
                        sprintf( TextT, "y%i = y%i(:) .* %s( %i, %g );\n",
                                    i,
                                    i_from,
                                        &Wind_2_Fir_nameFUNC[Dim1Procs[i].FDS.GetTypeWindow()][0],
                                        (int)Dim1Procs[0].NPosBuff,
                                        Dim1Procs[i].FDS.GetAlphaWindow()
                            );
                    }else{
                        sprintf( TextT, "y%i = y%i(:) .* %s( %i );\n",
                                    i,
                                    i_from,
                                        &Wind_2_Fir_nameFUNC[Dim1Procs[i].FDS.GetTypeWindow()][0],
                                        (int)Dim1Procs[0].NPosBuff
                            );
                    }
                }else{
                    sprintf( TextT, "y%i = y%i(:) .* %s( %i );%% La función de ventana no existe en matlab.\n",
                                    i,
                                    i_from,
                                        &Wind_2_Fir_nameFUNC[0][0],
                                        (int)Dim1Procs[0].NPosBuff
                            );
                }
                strcat( T2T, TextT );
                sprintf( TextT, "figure;\nplot( y%i );\ntitle( 'Módulo %i' );\ndrawnow;\n", i, i );
                strcat( T2T, TextT );
                strcat( T2T, "\n\n" );
                break;
            ///
            ///
            ///
            ///
            ///
            ///
            ///
            ///
            ///
            case FFT_Signal:
                sprintf( TextT, "%%Procesamiento del módulo %i(Análisis espectral, FFT)\n"
                        "figure;\nY%i = fft( y%i, length(y%i) );\n"
                        "Y%i = abs( 2*Y%i(1:round(end/2))/length(Y%i) );\n"
                        "F = Fs/2 * (0:length(Y%i)-1)/length(Y%i);\n"
                        "plot( F, Y%i )"
                        "\n\n", i, i, i_from, i_from, i, i, i, i, i, i );
                strcat( T2T, TextT );
                sprintf( TextT, "title( 'Módulo %i' );\ndrawnow;\n", i );
                strcat( T2T, TextT );
                strcat( T2T, "\n\n" );
                break;
            /// /////////////////////////////////////////////////////////////////////////////////////// SPECTOGRAM_Signal 4
            case SPECTOGRAM_Signal:{/// SPECTO                    Dim1Procs[i].FDS
                /// spectrogram( Var_1_1, blackman(Fs), 1, 1024, Fs,'yaxis' );
                sprintf( TextT, "%%Procesamiento del módulo %i(Espectrograma)\n", i );
                strcat( T2T, TextT );
                int NFRM = 4096;
                if( ((int)Dim1Procs[i].SLOAD.Fs)>((int)Dim1Procs[0].NPosBuff) ){
                    NFRM = 1<<( Dim1Procs[i].FDS.nextpow2( Dim1Procs[0].NPosBuff )-1 );
                }
                if( Dim1Procs[i].FDS.SPecto.WINDWN.ExistOnMatlab() ){
                    if( Dim1Procs[i].FDS.SPecto.WINDWN.HaveWindowAlpha() ){
                        strcat( T2T, "figure;\n" );
                        sprintf( TextT, "spectrogram( y%i, %s(%i, %g), 1, 1024, Fs, 'yaxis' );\n",
                                                i_from,
                                                &Wind_2_Fir_nameFUNC[Dim1Procs[i].FDS.SPecto.WINDWN.GetWinType()][0],
                                                NFRM,
                                                Dim1Procs[i].FDS.SPecto.WINDWN.GetAlpha()
                                                 );
                        strcat( T2T, TextT );
                    }else{
                        strcat( T2T, "figure;\n" );
                        sprintf( TextT, "spectrogram( y%i, %s(%i), 1, 1024, Fs, 'yaxis' );\n",
                                                i_from,
                                                &Wind_2_Fir_nameFUNC[Dim1Procs[i].FDS.SPecto.WINDWN.GetWinType()][0],
                                                NFRM
                                                 );
                        strcat( T2T, TextT );
                    }
                }else{
                    strcat( T2T, "figure;\n" );
                    sprintf( TextT, "spectrogram( y%i, %s(%i), 1, 1024, Fs, 'yaxis' );%%La ventana aplicada no existe en matlab.\n",
                                                i_from,
                                                &Wind_2_Fir_nameFUNC[0][0],
                                                NFRM );
                    strcat( T2T, TextT );
                }
                sprintf( TextT, "title( 'Módulo %i' );\ndrawnow;\n", i );
                strcat( T2T, TextT );
                strcat( T2T, "\n\n" );
            }
                break;
                /// ENDNEWCASE 4
            ///
            ///

            /// /////////////////////////////////////////////////////////////////////////////////////// WIGNER_Signal
            case WIGNER_Signal:/// WIGNER
                if( writewigner!=NULL ){
                    writewigner[0] = TRUE;
                }
                sprintf( TextT, "%%Procesamiento del módulo %i(WIGNER - VILLE)\n", i );
                strcat( T2T, TextT );
                if( Dim1Procs[i].FDS.SPecto.WINDWN.ExistOnMatlab() ){
                    if( Dim1Procs[i].FDS.SPecto.WINDWN.HaveWindowAlpha() ){
                        strcat( T2T, "figure;\n" );
                        sprintf( TextT, "wigner( y%i, Fs, %s(Fs, %g) );\n",
                                                i_from,
                                                &Wind_2_Fir_nameFUNC[Dim1Procs[i].FDS.SPecto.WINDWN.GetWinType()][0],
                                                Dim1Procs[i].FDS.SPecto.WINDWN.GetAlpha()
                                                 );
                        strcat( T2T, TextT );
                    }else{
                        strcat( T2T, "figure;\n" );
                        sprintf( TextT, "wigner( y%i, Fs, %s(Fs) );\n",
                                                i_from,
                                                &Wind_2_Fir_nameFUNC[Dim1Procs[i].FDS.SPecto.WINDWN.GetWinType()][0]
                                                 );
                        strcat( T2T, TextT );
                    }
                }else{
                    strcat( T2T, "figure;\n" );
                    sprintf( TextT, "wigner( y%i, Fs, %s(Fs) );%%La ventana aplicada no existe en matlab.\n",
                                                i_from,
                                                &Wind_2_Fir_nameFUNC[0][0]
                                                 );
                    strcat( T2T, TextT );
                }
                sprintf( TextT, "title( 'Módulo %i WIGNER-VILLE' );\ndrawnow;\n", i );
                strcat( T2T, TextT );
                strcat( T2T, "\n\n" );
                break;
                /// WIGNER_Signal
            ///
            ///
            /// /////////////////////////////////////////////////////////////////////////////////////// CHOI_WIL_Signal
            case CHOI_WIL_Signal:/// CHOI - WILLIAM
                if( writechoi_william!=NULL ){
                    writechoi_william[0] = TRUE;
                }
                sprintf( TextT, "%%Procesamiento del módulo %i(CHOI - WILLIAM)\n", i );
                strcat( T2T, TextT );
                if( Dim1Procs[i].FDS.SPecto.WINDWN.ExistOnMatlab() ){
                    if( Dim1Procs[i].FDS.SPecto.WINDWN.HaveWindowAlpha() ){
                        strcat( T2T, "figure;\n" );
                        sprintf( TextT, "choi_william( y%i, Fs, %s(Fs, %g) );\n",
                                                i_from,
                                                &Wind_2_Fir_nameFUNC[Dim1Procs[i].FDS.SPecto.WINDWN.GetWinType()][0],
                                                Dim1Procs[i].FDS.SPecto.WINDWN.GetAlpha()
                                                 );
                        strcat( T2T, TextT );
                    }else{
                        strcat( T2T, "figure;\n" );
                        sprintf( TextT, "choi_william( y%i, Fs, %s(Fs) );\n",
                                                i_from,
                                                &Wind_2_Fir_nameFUNC[Dim1Procs[i].FDS.SPecto.WINDWN.GetWinType()][0]
                                                 );
                        strcat( T2T, TextT );
                    }
                }else{
                    strcat( T2T, "figure;\n" );
                    sprintf( TextT, "choi_william( y%i, Fs, %s(Fs) );%%La ventana aplicada no existe en matlab.\n",
                                                i_from,
                                                &Wind_2_Fir_nameFUNC[0][0]
                                                 );
                    strcat( T2T, TextT );
                }
                sprintf( TextT, "title( 'Módulo %i CHOI - WILLIAM' );\ndrawnow;\n", i );
                strcat( T2T, TextT );
                strcat( T2T, "\n\n" );
                break;
                /// CHOI_WIL_Signal
            ///
            ///
            ///
            ///
            case PLAY_Signal:
                break;
                sprintf( TextT, "%%Procesamiento del módulo %i(Reproducir audio)\n"
                        "aup%i = audioplayer( y%i, Fs );\n"
                        "play( aup%i );"
                        "\n\n", i, i, i_from, i );
                strcat( T2T, TextT );
                break;
            ///
            ///
                break;
        }
        ///
    }
    ///
    ///
    strcat( T2T, "\n\n" );
    ///
    int NModS = 0;
    int NModSTop = 0;
    for( int i=1; i<MaxProcs; i++ ){
        if( Dim1Procs[i].ProcType==PLAY_Signal ){
            NModS = 0;
            for( int x=0; x<MaxProcs; x++ ){
                if( Dim1Procs[i].PLAYSELECTED[x] && (Dim1Procs[i].D1_INIT[x].ProcType==Windowing_Signal || Dim1Procs[i].D1_INIT[x].ProcType==Filt_Signal || x==0) ){
                    NModS++;
                }
            }
            ///
            if( NModS==0 ){
                continue;
            }
            ///
            sprintf( TextT, "%%Procesamiento del módulo %i(Reproducir audio)\n", i );
            strcat( T2T, TextT );
            ///
            sprintf( TextT, "y%i = ( ", i );
            strcat( T2T, TextT );
            ///
            NModSTop = 0;
            for( int x=0; x<MaxProcs; x++ ){
                if( Dim1Procs[i].PLAYSELECTED[x] && (Dim1Procs[i].D1_INIT[x].ProcType==Windowing_Signal || Dim1Procs[i].D1_INIT[x].ProcType==Filt_Signal || x==0) ){
                    if( NModSTop==(NModS-1) ){
                        sprintf( TextT, "%g * y%i );\n", Dim1Procs[i].PLAY_PERCENT[x], x );
                        strcat( T2T, TextT );
                    }else{
                        sprintf( TextT, "%g * y%i + ", Dim1Procs[i].PLAY_PERCENT[x], x );
                        strcat( T2T, TextT );
                    }
                    NModSTop++;
                }
            }
            sprintf( TextT, "aup%i = audioplayer( y%i, Fs );\n", i, i );
            strcat( T2T, TextT );
            if( Dim1Procs[i].PLAY_SET ){
                sprintf( TextT, "play( aup%i );\n\n", i );
            }else{
                sprintf( TextT, "%%play( aup%i );\n\n", i );
            }
            strcat( T2T, TextT );
            ///
        }
    }
    /*
            case PLAY_Signal:
                sprintf( TextT, "%%Procesamiento del módulo %i(Reproducir audio)\n"
                        "aup%i = audioplayer( y%i, Fs );\n"
                        "play( aup%i );"
                        "\n\n", i, i, i_from, i );
                strcat( T2T, TextT );
                break;
    //*/
    ///
    ///
    strcat( T2T, "\n\ntoc\n\n" );
    ///
    ///
    ///
    ///
    free( TextT );
    ///
    return TRUE;
}

/// ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL TLAB::CopyCode1D_2_Clipboard( void ){
    char* T2T = (char*)malloc( 1024 * 1024 );
    ZeroMemory( T2T, 1024 * 1024 - 1 );
    BOOL writewigner = FALSE;
    BOOL writechoi_william = FALSE;
    ///
    ///
    T2T[0] = 0;
    CopyCode1D( T2T, &writewigner, &writechoi_william );
    if( writewigner && writechoi_william ){
        MessageBoxA( HW_MAIN,
                     "Se necesitan las funciones (wigner) y (choi_william) para que funcione el código, "
                     " asegúrese que está en el directorio de trabajo en el entorno matlab."
                            , "Advertencia", MB_ICONEXCLAMATION );
    }else{
        if( writewigner ){
            MessageBoxA( HW_MAIN,
                         "Se necesita la función (wigner) para que funcione el código, "
                         " asegúrese que está en el directorio de trabajo en el entorno matlab."
                                , "Advertencia", MB_ICONEXCLAMATION );
        }
        if( writechoi_william ){
            MessageBoxA( HW_MAIN,
                         "Se necesita la función (choi_william) para que funcione el código, "
                         " asegúrese que está en el directorio de trabajo en el entorno matlab."
                                , "Advertencia", MB_ICONEXCLAMATION );
        }
    }
    ///
    Copy2ClipBoard2( T2T );
    ///MsgBoxF( "Código copiado al portapapeles." );
    DialogBoxParam( GetModuleHandle(NULL), MAKEINTRESOURCEA(Dial_MSGBOX),
                            HW_MAIN, DLGMSGBOX, (LPARAM)"Código copiado al portapapeles." );
    Copy2ClipBoard2( T2T );
    ///
    free( T2T );
    ///
    return TRUE;
}
