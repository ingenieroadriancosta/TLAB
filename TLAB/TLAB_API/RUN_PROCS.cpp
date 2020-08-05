int TLAB::RUN_PROCS( void ){
    On_Run = TRUE;
    char T2T[512];
    ZeroMemory( T2T, 511 );
    HWProgress = NULL;
    DWORD IdTh = 0;
    CreateThread( NULL, 0, ThreadCall_Progress, (PVOID)&IPtr, 0, &IdTh );
    while( HWProgress==NULL ){
        Sleep(1);
    }
    ///
    ///
    ///
    WRITE_ERROR_REPORT();
    ///
    ///
    ///
    if( TabPage==0 ){
        BOOL AllLoaded = TRUE;
        for( int i=0; i<MaxProcs; i++ ){
            AllLoaded = AllLoaded && Dim1Procs[i].IsLoaded;
        }
        if( !AllLoaded ){
            for( int i=0; i<MaxProcs; i++ ){
                B_1D[i].Enable( FALSE );
                if( Dim1Procs[i].ProcType==Filt_Signal ){
                    if( Dim1Procs[i].FDS.IsIRR ){
                        Dim1Procs[i].FDS.IIRSet();
                        Dim1Procs[i].FDS.MakeIIR2FIR_BUFF();
                    }else{
                        Dim1Procs[i].FDS.Fir1Set();
                    }
                }
            }
            int SPOPT = Pltrs.GetSpectOpt();
            Pltrs.SetSpectOpt( 0 );
            B_1D[BSel_1D].SetOn( NULL, FALSE );
            DSP_MNG.tic( 53 );
            for( int i=1; i<MaxProcs; i++ ){/// MaxProcs ---------
                if( Dim1Procs[i].IsLoaded ){
                    if( Dim1Procs[i].ProcType==PLAY_Signal ){
                        /// Dim1Procs[i].StopPlay();Dim1Procs[i].IsLoaded = FALSE;
                    }
                    continue;
                }
                sprintf( T2T, "Procesando módulo %i    (%i%%)", i, (100*(i))/MaxProcs );
                SetStatusText( T2T );
                SetWindowTextA( HWProgress, T2T );
                ///
                B_1D[i].SetOn( "Seleccionado", TRUE );
                ///
                Dim1Procs[i].SetProc( HW_MAIN, &Pltrs );
                ///
                B_1D[i].SetOn( NULL, FALSE );
                ///
                SendMessage( HW_Progr, PBM_SETPOS, (WPARAM)( ( 100*(i+1) )/MaxProcs), 0 );
                if( Dim1Procs[i].ProcType==PLAY_Signal ){
                    Dim1Procs[i].IsLoaded = FALSE;
                }
                /// MsgBoxF( " " );
            }
            ///
            ///
            ///
            ///
            for( int i=0; i<MaxProcs; i++ ){
                Dim1Procs[i].FDS.Free();
            }
            DSP_MNG.SetPlanFFT( 0 );
            ///
            ///
            ///
            ///
            for( int i=1; i<MaxProcs; i++ ){
                if( Dim1Procs[i].IsLoaded ){
                    continue;
                }
                if( Dim1Procs[i].ProcType!=PLAY_Signal ){
                    continue;
                }
                B_1D[i].SetOn( "Seleccionado", TRUE );
                sprintf( T2T, "Procesando módulo %i    (%i%%)", i, (100*(i))/MaxProcs );
                SetStatusText( T2T );
                SetWindowTextA( HWProgress, T2T );
                Dim1Procs[i].FillPlays( &Pltrs );
                if( Dim1Procs[i].PLAY_SET ){
                    CreateThread( NULL, 0, ThreadCall_PlaySignal, (PVOID)&Dim1Procs[i], 0, &IdTh );
                }
                Dim1Procs[i].IsLoaded = TRUE;
                B_1D[i].SetOn( NULL, FALSE );
            }
            DSP_MNG.toc( 53 );
            ///
            ///
            Pltrs.SetSpectOpt( SPOPT );
            B_1D[BSel_1D].SetOn( "Seleccionado", TRUE );
            for( int i=0; i<MaxProcs; i++ ){
                B_1D[i].Enable( TRUE );
                /// Dim1Procs[i].OpenPathOnly();
            }
            /// Pltrs.SetBitmap( Dim1Procs[BSel_1D].GetPicPlot() );
            SetOptionBSel();
            SetFocus( B_1D[BSel_2D].HWCTRL );
            SetStatusText( "Todos los canales de una dimensión han sido procesados.\n"
                          "Tiempo estimado del proceso: %g Segundos.", DSP_MNG.GetEndToc(53) );
        }
    }
    ///
    ///
    ///
    ///
    ///
    ///
    if( TabPage==1 ){
        BOOL AllLoaded2D = TRUE;
        for( int i=1; i<MaxProcs; i++ ){
            AllLoaded2D = AllLoaded2D && Dim2Procs[i].IsLoaded;
        }
        if( !AllLoaded2D ){
            for( int i=0; i<MaxProcs; i++ ){
                B_2D[i].Enable( FALSE );
            }
            ///
            B_2D[BSel_2D].SetOn( NULL, FALSE );
            ///
            DSP_MNG.tic( 54 );
            ///
            for( int i=1; i<MaxProcs; i++ ){
                if( Dim2Procs[i].IsLoaded ){
                    continue;
                }
                sprintf( T2T, "Procesando módulo %i    (%i%%)", i, (100*(i))/MaxProcs );
                SetStatusText( T2T );
                SetWindowTextA( HWProgress, T2T );
                ///
                if( Dim2Procs[i].ProcType==NO_PROC_Signal_2D ){
                    Dim2Procs[i].FreeAll();
                    Dim2Procs[i].IsLoaded = TRUE;
                    //SetOptionBSel_2D( i, FALSE );
                    //B_2D[i].SetOn( NULL, FALSE );
                    continue;
                }
                B_2D[i].SetOn( "Seleccionado", TRUE );
                ///
                Dim2Procs[i].SetProc( HW_IMSHOW );
                Dim2Procs[i].SetPicPlot( W_BOUT, H_BOUT );
                Dim2Procs[i].FreeImage();
                Dim2Procs[i].IsLoaded = TRUE;
                SetOptionBSel_2D( i, FALSE );
                ///
                B_2D[i].SetOn( NULL, FALSE );
                ///
                SendMessage( HW_Progr, PBM_SETPOS, (WPARAM)( ( 100*(i+1) )/MaxProcs), 0 );
                /// MsgBoxF( " " );
            }
            DSP_MNG.toc( 54 );
            ///
            B_2D[BSel_2D].SetOn( "Seleccionado", TRUE );
            ///
            SetFocus( B_2D[BSel_2D].HWCTRL );
            ///
            for( int i=0; i<MaxProcs; i++ ){
                B_2D[i].Enable( TRUE );
            }
            SetOptionBSel_2D();
        }
    }
    /// Dim1Procs[0].IsLoaded
    /*
    for( int i=0; i<MaxProcs; i++ ){
        B_1D[i].Enable( FALSE );
        B_2D[i].Enable( FALSE );
    }
    ///
    ///
    Sleep( 500 );
    for( int i=0; i<MaxProcs; i++ ){
        if( Dim1Procs[0].IsLoaded || i==0 ){
            B_1D[i].Enable( TRUE );
        }
        /// B_2D[i].Enable( TRUE );
    }
    //*/
    EnableWindow( HWTABS, TRUE );
    EnableWindow( HW_RUN, TRUE );
    //ShowWindow( HW_PBAR, 0 );
    SendMessageA( HWProgress, WM_CLOSE, 0, 0 );
    Sleep( 1 );
    SendMessageA( HW_MAIN, WM_COMMAND, CMD_SETFOCUS_1_2_D, 0 );
    if( TabPage==0 ){
        SetStatusText( "Todos los módulos procesados\n%g Segundos.", DSP_MNG.GetEndToc(53) );
    }else{
        SetStatusText( "Todos los módulos procesados\n%g Segundos.", DSP_MNG.GetEndToc(54) );
    }
    On_Run = FALSE;
    return 0;
}
