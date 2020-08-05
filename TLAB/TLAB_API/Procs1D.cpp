int TLAB::Procs1D( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam ){
    DWORD IdTh = 0;
    if( BSel_1D==0 ){
        int IndSel = DialogBoxParamA( GetModuleHandle(NULL), MAKEINTRESOURCE(DLG_1D_INIT), hwnd, (DLGPROC)Dlg1D_INIT,
                                     (LPARAM)&Dim1Procs[BSel_1D] );
        if( IndSel==1 ){
            // EnableWindow( HW_MAIN, FALSE );
            hwndC = HW_MAIN;
            HWProgress = NULL;
            CreateThread( NULL, 0, ThreadCall_Progress, (PVOID)&IPtr, 0, &IdTh );
            Sleep( 100 );
            while( HWProgress==NULL ){
                Sleep( 10 );
            }
            ///
            /// SetWindowTextA( HW_MAIN, "Cargando..." );
            ///
            SetStatusText( "Cargando..." );
            SendMessage( HW_Progr, PBM_SETPOS, (WPARAM)20, 0);
            Dim1Procs[BSel_1D].ProcType = -1;
            Dim1Procs[BSel_1D].PreLoadInfo();
            /// MsgBoxF( "%i", Dim1Procs[BSel_1D].NPosBuff );
            if( Dim1Procs[BSel_1D].LoadSignal( hwnd ) ){
                ///
                /// SetStatusText( "%i\n(%i)", BSel_1D, IndSel );
                for( int i=1; i<MaxProcs; i++ ){
                    B_1D[i].Enable( TRUE );
                    Dim1Procs[i].IsLoaded = FALSE;
                    Dim1Procs[i].StopPlay();
                    Dim1Procs[i].FDS.SetFs( Dim1Procs[BSel_1D].SLOAD.Fs );
                }
                SetStatusText( "Leyendo..." );
                SetWindowTextA( HWProgress, "Leyendo..." );
                SendMessage( HW_Progr, PBM_SETPOS, (WPARAM)40, 0);
                Dim1Procs[BSel_1D].ReadDatas();
                /// Pltrs.SetLineColor( LineColors[0] );
                SetStatusText( "Graficando..." );
                SetWindowTextA( HWProgress, "Graficando..." );
                SendMessage( HW_Progr, PBM_SETPOS, (WPARAM)60, 0);
                Pltrs.SetMaxXLim(
                                 Dim1Procs[BSel_1D].NPosBuff/( (double)Dim1Procs[BSel_1D].SLOAD.Fs ),
                                 TRUE );
                Pltrs.PLOTDOUBLE_BIPOL( Dim1Procs[BSel_1D].DBuff, Dim1Procs[BSel_1D].NPosBuff, 0, TRUE );
                Dim1Procs[BSel_1D].FreeBuff();
                Dim1Procs[BSel_1D].SetPicPlot( Pltrs.GetBitmap() );
                Dim1Procs[BSel_1D].D1_IN = NULL;
                Dim1Procs[BSel_1D].D1_INIT = NULL;
                ///
                SetStatusText( "Señal cargada!!!." );
                SetWindowTextA( HWProgress, "Señal cargada!!!." );
                SendMessageA( HWProgress, WM_CLOSE, 0, 0 );
                ///
                ///
                ///
                for( int i=0; i<MaxProcs; i++ ){
                    if( Dim1Procs[i].FDS.IsIRR ){
                        Dim1Procs[i].FDS.IIRSet();
                    }else{
                        Dim1Procs[i].FDS.Fir1Set();
                    }
                    Dim1Procs[i].FDS.Free();
                }
                DSP_MNG.SetPlanFFT( 0 );
                ///
                ///
            }else{
                Dim1Procs[BSel_1D].UnLoadInfo();
                SetStatusText( "La señal no pudo ser cargada!!!." );
                SendMessageA( HWProgress, WM_CLOSE, 0, 0 );
            }
        }
        SetFocus( B_1D[BSel_1D].HWCTRL );
    }else{
        int ResP = DialogBoxParam( GetModuleHandle(NULL), MAKEINTRESOURCEA(Dial_Change_Proc_1D_TAD),
                            HW_MAIN, DlgProc_Proc_1D_TAD, (LPARAM)&Dim1Procs[BSel_1D].FDS );
        if( ResP>=0 ){
            WRITE_ERROR_REPORT();
            ResP = 127&ResP;
            Dim1Procs[BSel_1D].StopPlay();
            Dim1Procs[BSel_1D].ProcType = ResP;
            Dim1Procs[BSel_1D].FDS.SetTYPE_PROC( ResP );
            BOOL BTMP = Dim1Procs[BSel_1D].FDS.IsIRR;
            Dim1Procs[BSel_1D].FDS.IsIRR = FALSE;
            Dim1Procs[BSel_1D].FDS.SetFs( Dim1Procs[0].SLOAD.Fs );
            Dim1Procs[BSel_1D].FDS.IsIRR = TRUE;
            Dim1Procs[BSel_1D].FDS.SetFs( Dim1Procs[0].SLOAD.Fs );
            Dim1Procs[BSel_1D].FDS.IsIRR = BTMP;
            Dim1Procs[BSel_1D].IsLoaded = FALSE;
            /// char T2T[128];sprintf( T2T, "%i", Dim1Procs[BSel_1D].FDS.GetFs() );MessageBoxA( HW_MAIN, T2T, "", 0 );
            if( ResP==Filt_Signal ){
                FDESIGN( HW_MAIN, &Dim1Procs[BSel_1D].FDS );
                for( int i=0; i<MaxProcs; i++ ){
                    if( Dim1Procs[i].FDS.IsIRR ){
                        Dim1Procs[i].FDS.IIRSet();
                    }else{
                        Dim1Procs[i].FDS.Fir1Set();
                    }
                    Dim1Procs[i].FDS.Free();
                }
                DSP_MNG.SetPlanFFT( 0 );
            }
            if( ResP==Windowing_Signal ){
                FWINDOWING( HW_MAIN, &Dim1Procs[BSel_1D].FDS );
            }
            ///
            if( ResP==SPECTOGRAM_Signal || ResP==WIGNER_Signal || ResP==CHOI_WIL_Signal ){
                FSPECTOGRAM( HW_MAIN, &Dim1Procs[BSel_1D].FDS );
            }
            ///
            if( ResP==PLAY_Signal ){
                PLAY_DESIGN( HW_MAIN, &Dim1Procs[BSel_1D] );
            }
            ///
            for( int i=BSel_1D; i<MaxProcs; i++ ){
                Dim1Procs[i].IsLoaded = FALSE;
                Dim1Procs[i].StopPlay();
            }
            ///
            ///
            ///
            ///
            Dim1Procs[BSel_1D].D1_IN = &Dim1Procs[BSel_1D-1];
            B_1D[BSel_1D].SetPInit = FALSE;
            if( Dim1Procs[BSel_1D].FDS.GetBInput()>0 ){
                B_1D[BSel_1D].SetPInit = TRUE;
                Dim1Procs[BSel_1D].D1_INIT = &Dim1Procs[0];
                B_1D[BSel_1D].SetBorderColor( 255, 0, 0 );
            }else{
                Dim1Procs[BSel_1D].D1_INIT = &Dim1Procs[0];
                B_1D[BSel_1D].SetBorderColor( 0, 255, 0 );
            }
            ///
            ///
            ///
            B_1D[BSel_1D].SetPicOn( PLOT_PNG+ResP, FILE_RES );
            B_1D[BSel_1D].SetOn( "Seleccionado", TRUE );
            SetOptionBSel();
            ///
            ///
            ///
            ///
            ///
        }
        SetFocus( B_1D[BSel_1D].HWCTRL );
        SendMessageA( HW_MAIN, WM_COMMAND, CMD_SETFOCUS_1_2_D, 0 );
    }
    EnableWindow( hwnd, TRUE );
    SendMessageA( HW_MAIN, WM_COMMAND, CMD_SETFOCUS_1_2_D, 0 );
    return 0;
}













