int TLAB::CmdProcs( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam ){
    OnCmds = TRUE;
    switch( LOWORD(wParam) ){
        case CMD_QUIT:
            SendMessageA( hwnd, WM_CLOSE, 0, 0 );
            break;
        ///
        ///
        case CMD_SHOW_HIDE_NEW_WIN:
            ShowWindow( hwnd, 0 );
            Sleep( 1000 );
            ShowWindow( hwnd, 1 );
            break;
        ///
        ///
        ///
        ///
        ///
        case CMD_ABOUT_TLAB:
            DialogBoxParamA( GetModuleHandleA(NULL), MAKEINTRESOURCE(Dial_ABOUT_TLAB), (HWND)hwnd,
                                (DLGPROC)&DlgAbout_TLAB, (LPARAM)0 );
            break;
        ///
        ///
        ///
        ///
        ///
        ///
        ///
        ///
        case CMD_SETPLOTALL:
            if( SetPlotimshow ){
                SetPlotimshow = FALSE;
            }else{
                SetPlotimshow = TRUE;
            }
            CheckMenuItem( HMeW, CMD_SETPLOTALL, 8*int(SetPlotimshow) );
            break;
        ///
        ///
        ///
        ///
        ///
        case CMD_CALL_FREE_MAT:{
            /*
            SetCurrentDirectoryA( "E:\\AIIF\\SegAll\\CONOCIMIENTO\\UNIVERSIDAD\\TESIS\\P_NUEVO\\APLICACIONES\\FreeMat" );
            /// system( "CD \"E:\\AIIF\\SegAll\\CONOCIMIENTO\\UNIVERSIDAD\\TESIS\\P_NUEVO\\APLICACIONES\\FreeMat\"" );
            system( "CALL \"E:\\AIIF\\SegAll\\CONOCIMIENTO\\UNIVERSIDAD\\TESIS\\P_NUEVO\\APLICACIONES\\FreeMat\\FreeMat.exe\"" );
            break;
            //*/
            ///
            HWProgress = NULL;
            DWORD IdTh = 0;
            CreateThread( NULL, 0, ThreadCall_FreeMat, (PVOID)&IPtr, 0, &IdTh );
        }
            break;
        ///
        ///
        ///
        ///
        ///
        ///
        case CMD_GET_MOD_INFO:{
            ///
            if( TabPage==0 ){
                if( Dim1Procs[BSel_1D].IsLoaded ){
                    MsgBoxF(
                            "\nInformación de la señal:\n\n"
                            "Frecuencia de muestreo:\t%i Hz\n"
                            "Franjas:\t\t\t%i\n"
                            "Duración:\t\t\t%g Segundos\n"
                            "\nTipo de proceso del módulo:\n%s\n"
                            "\n\n",
                            ///
                            Dim1Procs[0].SLOAD.Fs,
                            Dim1Procs[0].NPosBuff,
                            (Dim1Procs[0].NPosBuff)/( (double)Dim1Procs[0].SLOAD.Fs ),
                            ///
                            Dim1Procs[BSel_1D].GetInfo( BSel_1D, FALSE )
                            );
                }else{
                    MsgBoxF( "Módulo no procesado aún.\nTipo de proceso del módulo:\n%s\n\n",
                             Dim1Procs[BSel_1D].GetInfo( BSel_1D, FALSE ) );
                }
            }else{
                if( Dim2Procs[BSel_2D].IsLoaded ){
                    MsgBoxF(
                            "\nInformación de la imagen:\n\n"
                            "Tipo de imagen:\t%s\n"
                            "Ancho:\t\t%i\n"
                            "Alto:\t\t%i\n"
                            "graythresh:\t%g(%i)\n"
                            "\nTipo de proceso del módulo:\n%s\n"
                            "\n\n",
                            Dim2Procs[BSel_2D].GetImageType(),
                            Dim2Procs[BSel_2D].WPIC_OUT,
                            Dim2Procs[BSel_2D].HPIC_OUT,
                            Dim2Procs[BSel_2D].GrayValue,
                            (int)round( 255 * Dim2Procs[BSel_2D].GrayValue ),
                            ///
                            Dim2Procs[BSel_2D].GetInfo( BSel_2D, FALSE )
                            );
                }else{
                    MsgBoxF( "Módulo no procesado aún.\nTipo de proceso del módulo:\n%s\n\n",
                             Dim2Procs[BSel_2D].GetInfo( BSel_2D, FALSE ) );
                }
            }
            SendMessageA( HW_MAIN, WM_COMMAND, CMD_SETFOCUS_1_2_D, 0 );
            ///
        }
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
        case CMD_OPEN_SESION:{
            if( TabPage==0 ){
                if( OpenFileAs( FileName1D, "TLB Files (*.tlb*)\0*.tlb\0", HW_MAIN ) ){
                    if( OpenSesion1D( FileName1D ) ){
                        MsgBoxF( "\n\nLa sesión 1D fué cargada exitósamente desde:\n\n%s\n\n\n\n", FileName1D );
                    }else{
                        MessageBoxA( HW_MAIN, "La sesión no pudo ser cargada.", "", 16 );
                    }
                }
            }else{
                if( OpenFileAs( FileName2D, "TLB2 Files (*.tlb2*)\0*.tlb2\0", HW_MAIN ) ){
                    if( OpenSesion2D( FileName2D ) ){
                        MsgBoxF( "\n\nLa sesión 2D fué cargada exitósamente desde:\n\n%s\n\n\n\n", FileName2D );
                    }else{
                        MessageBoxA( HW_MAIN, "La sesión no pudo ser cargada.", "", 16 );
                    }
                }
            }
        }
            break;
        ///
        ///
        case CMD_PRUEBAS:
            PRUEBAS();
            break;
        ///
        ///
        case CMD_TIME_LAST_PROC:
            if( TabPage==0 ){
                MsgBoxF( "Todos los módulos procesados\n%g Segundos.", DSP_MNG.GetEndToc(53) );
            }else{
                MsgBoxF( "Todos los módulos procesados\n%g Segundos.", DSP_MNG.GetEndToc(54) );
            }
            SendMessageA( HW_MAIN, WM_COMMAND, CMD_SETFOCUS_1_2_D, 0 );
            break;
        ///
        ///
        case CMD_SETFOCUS_1_2_D:
            if( TabPage==0 ){
                SetFocus( B_1D[BSel_1D].HWCTRL );
            }else{
                SetFocus( B_2D[BSel_2D].HWCTRL );
            }
            break;
        ///
        ///
        case CMD_COPY_PROC_2_CLIPBOARD:
            if( TabPage==0 ){
                CopyCode1D_2_Clipboard();
            }else{
                CopyCode2D_2_Clipboard();
            }
            break;
        ///
        ///
        ///
        ///
        case CMD_UPCTRL:
        case CMD_DOWNCTRL:
            if( TabPage==0 ){
                B_1D[BSel_1D].SetOn( NULL, FALSE );
                BSel_1D = 15&( BSel_1D + 8 );
                SetFocus( B_1D[BSel_1D].HWCTRL );
                B_1D[BSel_1D].SetOn( "Seleccionado", TRUE );
                SetOptionBSel();
                //SendMessageA( hwnd, WM_COMMAND, CMD_SETFOCUS_1_2_D, 0 );
            }else{
                B_2D[BSel_2D].SetOn( NULL, FALSE );
                BSel_2D = 15&( BSel_2D + 8 );
                SetFocus( B_2D[BSel_2D].HWCTRL );
                B_2D[BSel_2D].SetOn( "Seleccionado", TRUE );
                SetOptionBSel_2D();
            }
            break;
        ///
        ///
        ///
        case CMD_SAVE_SESION:{
            if( !OnSaveSesion ){
                DWORD IdTh = 0;
                CreateThread( NULL, 0, ThreadCall_SaveSesionProc, (PVOID)&IPtr, 0, &IdTh );
            }
        }
            break;
        ///
        case CMD_SAVE_CODE:{
            if( TabPage==0 ){
                SaveCode1D();
            }else{
                SaveCode2D();
            }
        }
            break;
        ///
        case CMD_OPEN_FOLDER_DSP:
            DSP_MNG.OpenDSPFolder();
            break;
        ///
        case WITH_OUT_PROC:
            MsgBoxF( "Sin procedimiento.%i", (int)LOWORD(wParam) );
            break;
        ///
        case CMD_PIC_2D:
            if( TabPage==1 ){
                if( HIWORD(wParam)==STN_DBLCLK && Dim2Procs[BSel_2D].IsLoaded ){
                    if( Dim2Procs[BSel_2D].ProcType!=HIST_2DPROC ){
                        SendMessageA( HW_PIC_BIG, WM_CLOSE, 0, 0 );
                        DWORD IdTh = 0;
                        CreateThread( NULL, 0, ThreadCall_SHPIC, (PVOID)&IPtr, 0, &IdTh );
                    }
                }
            }
            //break;
        ///
        ///
        ///
        case CMD_PLOT_1D:{
            if( Dim1Procs[BSel_1D].IsLoaded &&
                        Dim1Procs[BSel_1D].ProcType!=SPECTOGRAM_Signal &&
                        Dim1Procs[BSel_1D].ProcType!=WIGNER_Signal &&
                        Dim1Procs[BSel_1D].ProcType!=CHOI_WIL_Signal &&
                        Dim1Procs[BSel_1D].ProcType!=PLAY_Signal &&
                        Dim1Procs[BSel_1D].ProcType!=NO_PROC_Signal &&
                        Dim1Procs[BSel_1D].ProcType!=Graph_Signal &&
                        TabPage==0
                         ){
                SendMessageA( HW_PLOT_BIG, WM_CLOSE, 0, 0 );
                DWORD IdTh = 0;
                CreateThread( NULL, 0, ThreadCall_SPLOT, (PVOID)&IPtr, 0, &IdTh );
            }
        }
            break;
        ///
        ///
        case CMD_OPEN_DSP_TAD:{
            static BOOL OnThr = FALSE;
            if( OnThr ){
                break;
            }
            DWORD IdTh = 0;
            CreateThread( NULL, 0, ThreadCall_DSP_TAD, (PVOID)&OnThr, 0, &IdTh );
            Sleep( 1 );
            }
            break;
        ///
        case CMD_ON_ALW_ONTOP:{
            AlwOnTop = !AlwOnTop;
            if( AlwOnTop ){
                SetWindowPos( hwnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW );
            }else{
                SetWindowPos( hwnd, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW );
            }
            CheckMenuItem( HMeW, CMD_ON_ALW_ONTOP, 8*int(AlwOnTop) );
        }
        break;
        ///
        ///
        ///
        case CMD_SAVE_GRAPH:
            SAVE_GRAPH_FUNC();
            break;
        ///
        ///
        ///
        ///
        case CMD_DeleteProc:{
            if( TabPage==0 ){
                if( !Dim1Procs[0].IsLoaded ){
                    MsgBoxF( "\n\nNo hay una señal cargada!!!\n\nHaga doble click en el boton habilitado.\n\n" );
                    break;
                }
                ///
                for( int i=MaxProcs-1; i>0; i-- ){
                    if( Dim1Procs[i].ProcType!=NO_PROC_Signal ){
                        B_1D[0].SetOn( NULL, FALSE );
                        B_1D[i].SetOn( "Seleccionado", TRUE );
                        SetFocus( B_1D[i].HWCTRL );
                        if( MessageBoxA( HW_MAIN,
                                        "\nSe borrará el último procedimiento, "
                                        "aparece encerrado con un cuadro de color verde "
                                        "con la palabra \"Seleccionado\"."
                                        "\n\n"
                                        "¿Desea borrar el último procedimiento?\n\n",
                                        "ADVERTENCIA",
                                        MB_YESNO|MB_ICONWARNING )==IDYES ){
                            Dim1Procs[i].StopPlay();
                            Sleep( 50 );
                            Dim1Procs[i].ProcType = NO_PROC_Signal;
                            Dim1Procs[i].FreeAll();
                            Dim1Procs[i].FDS.SetBInput( 0 );
                            B_1D[i].SetPInit = FALSE;
                            B_1D[i].SetBorderColor( 0, 255, 0 );
                            B_1D[i].SetPicOn( Dim1Procs[i].ProcType + PLOT_PNG, FILE_RES );
                        }
                        B_1D[i].SetOn( NULL, FALSE );
                        B_1D[0].SetOn( "Seleccionado", TRUE );
                        SetFocus( B_1D[0].HWCTRL );
                        break;
                    }
                }
                ///
            }else{
                ///
                if( !Dim2Procs[0].IsLoaded ){
                    MsgBoxF( "\n\nNo hay una señal cargada!!!\n\nHaga doble click en el boton habilitado.\n\n" );
                    break;
                }
                ///
                for( int i=MaxProcs-1; i>0; i-- ){
                    if( Dim2Procs[i].ProcType!=NO_PROC_Signal_2D ){
                        B_2D[0].SetOn( NULL, FALSE );
                        B_2D[i].SetOn( "Seleccionado", TRUE );
                        SetFocus( B_2D[i].HWCTRL );
                        if( MessageBoxA( HW_MAIN,
                                        "\nSe borrará el último procedimiento, "
                                        "aparece encerrado con un cuadro de color verde "
                                        "con la palabra \"Seleccionado\"."
                                        "\n\n"
                                        "¿Desea borrar el último procedimiento?\n\n",
                                        "ADVERTENCIA",
                                        MB_YESNO|MB_ICONWARNING )==IDYES ){
                            Dim2Procs[i].ProcType = NO_PROC_Signal_2D;
                            Dim2Procs[i].FreeAll();
                            Dim2Procs[i].ImageInP = 0;
                            B_2D[i].SetBorderColor( 0, 255, 0 );
                            ///B_2D[BSel_2D].SetPicOn( NO_PROC_PIC, FILE_RES );
                            B_2D[i].SetPInit = Dim2Procs[i].ImageInP>0;
                            B_2D[i].SetPicOn( Dim2Procs[i].ProcType + IMSHOW, FILE_RES );
                        }
                        B_2D[i].SetOn( NULL, FALSE );
                        B_2D[0].SetOn( "Seleccionado", TRUE );
                        SetFocus( B_2D[0].HWCTRL );
                        break;
                    }
                }
                SetOptionBSel_2D();
                ///
            }
        }
            break;
        ///
        ///
        case CMD_RE_RUN:
            for( int i=1; i<MaxProcs; i++ ){
                if( TabPage==0 ){
                    Dim1Procs[i].StopPlay();
                    Dim1Procs[i].IsLoaded = FALSE;
                }else{
                    Dim2Procs[i].IsLoaded = FALSE;
                }
            }
        case CMD_RUN:{
            ///
            /*
            if( ISDOWNKEY(VK_RETURN) && ISDOWNKEY(VK_MENU) ){
                for( int i=1; i<MaxProcs; i++ ){
                    if( TabPage==0 ){
                        Dim1Procs[i].StopPlay();
                        Dim1Procs[i].IsLoaded = FALSE;
                    }else{
                        Dim2Procs[i].IsLoaded = FALSE;
                    }
                }
            }
            //*/
            ///
            if( TabPage==0 ){
                if( !Dim1Procs[0].IsLoaded ){
                    MsgBoxF( "\n\nNo hay una señal cargada!!!\n\nHaga doble click en el boton habilitado.\n\n" );
                    break;
                }
            }else{
                if( !Dim2Procs[0].IsLoaded ){
                    MsgBoxF( "\n\nNo hay una imagen cargada!!!\n\nHaga doble click en el boton habilitado.\n\n" );
                    break;
                }
            }
            if( On_Run ){
                break;
            }
            hwndC = hwnd;
            DWORD IdTh = 0;
            CreateThread( NULL, 0, ThreadCall_RUN, (PVOID)&IPtr, 0, &IdTh );
            }
            break;
        ///
        ///
        ///
        ///

    }
    ///
    if( LOWORD(wParam)>=CMD_1_2_Ds ){
        const int BSel = LOWORD(wParam) - CMD_1_2_Ds;
        if( TabPage==0 ){
            if( ISDOWNKEY(VK_CONTROL) && HIWORD(wParam)==BN_CLICKED && !ISDOWNKEY(VK_RETURN) ){
                hwndC = hwnd;
                messageC = message;
                wParamC = wParam;
                lParamC = lParam;
                DWORD IdTh = 0;
                CreateThread( NULL, 0, ThreadCall_Procs1D, (PVOID)&IPtr, 0, &IdTh );
                Sleep( 1 );
            }else{
                switch( HIWORD(wParam) ){
                    case BN_DBLCLK:{
                        hwndC = hwnd;
                        messageC = message;
                        wParamC = wParam;
                        lParamC = lParam;
                        DWORD IdTh = 0;
                        CreateThread( NULL, 0, ThreadCall_Procs1D, (PVOID)&IPtr, 0, &IdTh );
                        Sleep( 1 );
                        /// Procs1D( hwnd, message, wParam, lParam );
                        }
                        break;
                    case BN_CLICKED:
                        if( BSel==0 ){
                            if( Dim1Procs[BSel_1D].IsLoaded ){
                                Pltrs.SetMaxYLim( Dim1Procs[BSel_1D].MaxValFFT );
                                Pltrs.SetMinYLim( -Dim1Procs[BSel_1D].MaxValFFT );
                                Pltrs.TextMx = "";
                                Pltrs.TextMy = NULL;
                                Pltrs.SetMaxXLim( (int)(Dim1Procs[BSel_1D].NPosBuff/Dim1Procs[BSel_1D].SLOAD.Fs), FALSE );
                                Pltrs.Text2ShowF( " Tiempo (Seg) " );
                                Pltrs.SetBitmap( Dim1Procs[BSel_1D].GetPicPlot(), FALSE );
                                SetStatusText( "Señal cargada, doble click ó Ctrl+click para modificar." );
                            }else{
                                Pltrs.FillGrid();
                                Pltrs.FillBuff();
                                Pltrs.SetTextMiddle( "Carga de la señal, doble click ó Ctrl+click para abrir." );
                                Pltrs.Redraw();
                                SetStatusText( "Carga de la señal, doble click ó Ctrl+click para abrir." );
                            }
                        }else{
                            if( Dim1Procs[BSel_1D].IsLoaded ){
                                if( Dim1Procs[BSel_1D].ProcType==PLAY_Signal ){
                                    if( Dim1Procs[BSel_1D].IsPlaying() ){
                                        Dim1Procs[BSel_1D].StopPlay();
                                    }else{
                                        DWORD IdTh = 0;
                                        CreateThread( NULL, 0, ThreadCall_PlaySignal, (PVOID)&Dim1Procs[BSel_1D], 0, &IdTh );
                                    }
                                    SetOptionBSel();
                                    break;
                                }
                            }
                            SetOptionBSel();
                        }
                        break;
                    case BN_KILLFOCUS:
                        break;
                    case BN_SETFOCUS:{
                        if( BSel_1D==BSel ){
                            break;
                        }
                        B_1D[BSel_1D].SetOn( NULL, FALSE );
                        BSel_1D = BSel;
                        B_1D[BSel_1D].SetOn( "Seleccionado", TRUE );
                        if( BSel==0 ){
                            if( Dim1Procs[BSel_1D].IsLoaded ){
                                Pltrs.SetMaxYLim( Dim1Procs[BSel_1D].MaxValFFT );
                                Pltrs.SetMinYLim( -Dim1Procs[BSel_1D].MaxValFFT );
                                Pltrs.TextMx = "";
                                Pltrs.TextMy = NULL;
                                Pltrs.SetMaxXLim( (int)(Dim1Procs[BSel_1D].NPosBuff/Dim1Procs[BSel_1D].SLOAD.Fs), FALSE );
                                Pltrs.Text2ShowF( " Tiempo (Seg) " );
                                Pltrs.SetBitmap( Dim1Procs[BSel_1D].GetPicPlot(), FALSE );
                                SetStatusText( "Señal cargada, doble click ó Ctrl+click para modificar." );
                            }else{
                                Pltrs.FillGrid();
                                Pltrs.FillBuff();
                                Pltrs.SetTextMiddle( "Carga de la señal, doble click ó Ctrl+click para abrir." );
                                Pltrs.Redraw();
                                SetStatusText( "Carga de la señal, doble click ó Ctrl+click para abrir." );
                            }
                        }else{
                            SetOptionBSel();
                        }
                        }
                        break;
                }
            }
        }
        ///
        ///
        ///
        if( TabPage==1 ){
            if( ISDOWNKEY(VK_CONTROL) && HIWORD(wParam)==BN_CLICKED && !ISDOWNKEY(VK_RETURN) ){
                hwndC = hwnd;
                messageC = message;
                wParamC = wParam;
                lParamC = lParam;
                DWORD IdTh = 0;
                CreateThread( NULL, 0, ThreadCall_Procs2D, (PVOID)&IPtr, 0, &IdTh );
            }else{
                switch( HIWORD(wParam) ){
                    case BN_DBLCLK:{
                        hwndC = hwnd;
                        messageC = message;
                        wParamC = wParam;
                        lParamC = lParam;
                        DWORD IdTh = 0;
                        CreateThread( NULL, 0, ThreadCall_Procs2D, (PVOID)&IPtr, 0, &IdTh );
                        ///Procs2D( hwnd, message, wParam, lParam );
                        }
                        break;
                    case BN_CLICKED:
                        SetOptionBSel_2D();
                        break;
                    case BN_KILLFOCUS:
                        break;
                    case BN_SETFOCUS:{
                        if( BSel_2D==BSel ){
                            SetOptionBSel_2D();
                            break;
                        }
                        B_2D[BSel_2D].SetOn( NULL, FALSE );
                        BSel_2D = BSel;
                        if( Dim2Procs[BSel_2D].ImageInP==0 ){
                            B_2D[BSel_2D].SetBorderColor( 0, 255, 0 );
                        }else{
                            B_2D[BSel_2D].SetBorderColor( 255, 0, 0 );
                        }
                        B_2D[BSel_2D].SetOn( "Seleccionado", TRUE );
                        SetOptionBSel_2D();
                        /// DWORD IdTh = 0;CreateThread( NULL, 0, ThreadCall_RUN, (PVOID)&IPtr, 0, &IdTh );
                        }
                        break;
                }
            }
        }
    }
    ///
    OnCmds = FALSE;
    ///
    return 0;
}
