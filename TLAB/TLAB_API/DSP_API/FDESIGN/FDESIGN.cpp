DWORD WINAPI Sweep_Thread( LPVOID );
BOOL CALLBACK FDESIGN_Dlg( HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam ){
    static FDESIGN_CLASS* FdsnC = NULL;
    static HWND HWTT[32] = {0};
    static char T2T[64] = {0};
    static HFONT HFontT = NULL;
    static PLOTEOS PltFD;
    switch( uMsg ){
        case WM_INITDIALOG:{
            FdsnC = (FDESIGN_CLASS*)lParam;
            FdsnC->SetOpen( TRUE );
            MoveWindow( hwndDlg, 1, 1, 700, 390, TRUE );
            CenterWindow( hwndDlg, 0, 0 );
            SendMessage( GetDlgItem( hwndDlg, 1), CB_ADDSTRING, 0, (LPARAM)"FIR" );
            SendMessage( GetDlgItem( hwndDlg, 1), CB_ADDSTRING, 0, (LPARAM)"BUTTERWORTH" );
            SendMessage( GetDlgItem( hwndDlg, 1), CB_SETCURSEL, (int)FdsnC->IsIRR, 0 );
            for( int i=0; i<18; i++ ){
                SendMessage( GetDlgItem( hwndDlg, 2), CB_ADDSTRING, 0, (LPARAM)&Wind_2_Fir[i][0] );
            }
            SendMessage( GetDlgItem( hwndDlg, 2), CB_SETCURSEL, FdsnC->GetWType(), 0 );
            EnableWindow( GetDlgItem( hwndDlg, 2), !FdsnC->IsIRR );
            EnableWindow( GetDlgItem( hwndDlg, 3), FdsnC->HaveAlpha() &&(!FdsnC->IsIRR) );
            sprintf( T2T, "%g", FdsnC->GetAlpha() );
            SetWindowTextA( GetDlgItem( hwndDlg, 3), T2T );
            EnableWindow( GetDlgItem( hwndDlg, 3), FdsnC->HaveAlpha() );
            ///
            sprintf( T2T, "%i", FdsnC->GetOrdFil() );
            SetWindowTextA( GetDlgItem( hwndDlg, 4), T2T );
            ///
            ///
            SendMessage( GetDlgItem( hwndDlg, 5), CB_ADDSTRING, 0, (LPARAM)"low" );
            SendMessage( GetDlgItem( hwndDlg, 5), CB_ADDSTRING, 0, (LPARAM)"high" );
            SendMessage( GetDlgItem( hwndDlg, 5), CB_ADDSTRING, 0, (LPARAM)"bandpass" );
            SendMessage( GetDlgItem( hwndDlg, 5), CB_ADDSTRING, 0, (LPARAM)"stop" );
            SendMessage( GetDlgItem( hwndDlg, 5), CB_SETCURSEL, FdsnC->GetFilType(), 0 );
            EnableWindow( GetDlgItem( hwndDlg, 6), FdsnC->GetFilType()==1 || FdsnC->GetFilType()>1 );
            EnableWindow( GetDlgItem( hwndDlg, 7), FdsnC->GetFilType()==0 || FdsnC->GetFilType()>1 );
            ///
            ///
            ///
            SetToolTip( GetDlgItem( hwndDlg, 1), "Clase de filtro. FIR ó IIR", &HWTT[0] );
            SetToolTip( GetDlgItem( hwndDlg, 2), "Clase de ventana aplicada al filtro FIR.", &HWTT[1] );
            SetToolTipW( GetDlgItem( hwndDlg, 3), L"Valor \u03B1 necesario para algunas ventanas.", &HWTT[2] );
            SetToolTip( GetDlgItem( hwndDlg, 4), "Orden del filtro FIR/IIR.", &HWTT[3] );
            SetToolTip( GetDlgItem( hwndDlg, 5), "Tipo del filtro FIR/IIR.    "
                       "Pasa bajos(low)      "
                       "Pasa altos(high)      "
                       "Pasa banda(bandpass)      "
                       "Para banda(stop)      "
                       , &HWTT[4] );
            ///
            ///

            HFontT = CreateFont( 14, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, DEFAULT_CHARSET,
                                OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, PROOF_QUALITY,
                                DEFAULT_PITCH | FF_ROMAN, "Times");
            SendMessage( GetDlgItem( hwndDlg, -2), WM_SETFONT, (WPARAM)HFontT, MAKELPARAM(TRUE, 0) );
            SendMessage( GetDlgItem( hwndDlg, -3), WM_SETFONT, (WPARAM)HFontT, MAKELPARAM(TRUE, 0) );
            SendMessage( GetDlgItem( hwndDlg, -4), WM_SETFONT, (WPARAM)HFontT, MAKELPARAM(TRUE, 0) );
            SetToolTip( GetDlgItem( hwndDlg, 6), "Frecuencia de corte inferior del filtro.", &HWTT[5] );
            SetToolTip( GetDlgItem( hwndDlg, 7), "Frecuencia de corte superior del filtro.", &HWTT[6] );
            SetToolTip( GetDlgItem( hwndDlg, 8), "Frecuencia de muestreo para el diseño.", &HWTT[7] );
            ///
            sprintf( T2T, "%g", FdsnC->GetFcL() );
            SetWindowTextA( GetDlgItem( hwndDlg, 6), T2T );
            ///
            sprintf( T2T, "%g", FdsnC->GetFcH() );
            SetWindowTextA( GetDlgItem( hwndDlg, 7), T2T );
            ///
            ///
            ///
            ///
            EnableWindow( GetDlgItem( hwndDlg, 8), !FdsnC->FixedFs );
            sprintf( T2T, "%i", FdsnC->GetFs() );
            SetWindowTextA( GetDlgItem( hwndDlg, 8), T2T );
            ///
            ///
            ///
            PltFD.PLOTEOS_INIT();
            PltFD.SetBackG( 255 );
            PltFD.SetFather( hwndDlg );
            PltFD.SetGridStyle( 1 );
            PltFD.ActGridStyle( TRUE );
            PltFD.SetVisible( TRUE );
            PltFD.SetLocationCon( 126, 1 );
            PltFD.SetSizeCon( 568, 358 );
            PltFD.FillGrid( TRUE );
            PltFD.SetTitle( " Espectro de frecuencia " );
            PltFD.Text2ShowF( " Frecuencia(Hz) " );
            PltFD.SetMaxYLim( 1 );
            PltFD.SetMinYLim( 0 );
            PltFD.SetLineColor( LineColors[0] );
            PltFD.TextMx = "Hz";
            if( FdsnC->IsIRR ){
                FdsnC->IIRSet();
                PltFD.PLOTDOUBLE( FdsnC->GetBuff(), FdsnC->GetMaxBuff()/2, 0, TRUE );
                PltFD.SetMaxXLim( FdsnC->GetNyq(), TRUE );
            }else{
                FdsnC->Fir1Set();
                PltFD.PLOTDOUBLE( FdsnC->GetBuff(), FdsnC->GetMaxBuff()/2, 0, TRUE );
                PltFD.SetMaxXLim( FdsnC->GetNyq(), TRUE );
            }
            ///
        }
            break;
        case WM_COMMAND:{
            ///
            if( FdsnC==NULL || !IsWindowVisible(hwndDlg) ){
                break;
            }
            ///
            if( FdsnC->Performing_sweep ){
                FdsnC->Sweep_Cancel = TRUE;
                break;
            }
            ///
            switch( LOWORD( wParam ) ){
                case 1:{
                    /// CLASE
                    if( HIWORD( wParam )==CBN_SELENDOK ){
                        FdsnC->IsIRR = (BOOL)SendMessage( GetDlgItem( hwndDlg, 1), CB_GETCURSEL, 0, 0 );
                        EnableWindow( GetDlgItem( hwndDlg, 2), !FdsnC->IsIRR );
                        EnableWindow( GetDlgItem( hwndDlg, 3), !FdsnC->IsIRR && FdsnC->HaveAlpha() );
                        sprintf( T2T, "%i", FdsnC->GetOrdFil() );
                        SetWindowTextA( GetDlgItem( hwndDlg, 4), T2T );
                        SendMessage( GetDlgItem( hwndDlg, 5), CB_SETCURSEL, FdsnC->GetFilType(), 0 );
                        sprintf( T2T, "%g", FdsnC->GetFcL() );
                        SetWindowTextA( GetDlgItem( hwndDlg, 6), T2T );
                        ///
                        sprintf( T2T, "%g", FdsnC->GetFcH() );
                        SetWindowTextA( GetDlgItem( hwndDlg, 7), T2T );
                        if( FdsnC->IsIRR ){
                            FdsnC->IIRSet();
                            PltFD.PLOTDOUBLE( FdsnC->GetBuff(), FdsnC->GetMaxBuff()/2, 0, TRUE );
                            PltFD.SetMaxXLim( FdsnC->GetNyq(), TRUE );
                        }else{
                            FdsnC->Fir1Set();
                            PltFD.PLOTDOUBLE( FdsnC->GetBuff(), FdsnC->GetMaxBuff()/2, 0, TRUE );
                            PltFD.SetMaxXLim( FdsnC->GetNyq(), TRUE );
                        }
                        EnableWindow( GetDlgItem( hwndDlg, 6),
                                     FdsnC->GetFilType()==1 || FdsnC->GetFilType()>1
                                     );
                        EnableWindow( GetDlgItem( hwndDlg, 7),
                                     FdsnC->GetFilType()==0 || FdsnC->GetFilType()>1
                                     );
                    }
                }
                    break;
                case 2:{
                    /// VENTANA
                    if( HIWORD( wParam )==CBN_SELENDOK ){
                        FdsnC->SetWType( (int)SendMessage( GetDlgItem( hwndDlg, 2), CB_GETCURSEL, 0, 0 ) );
                        if( FdsnC->HaveAlpha() ){
                            EnableWindow( GetDlgItem( hwndDlg, 3), TRUE );
                            sprintf( T2T, "%g", FdsnC->GetAlpha() );
                            SetWindowTextA( GetDlgItem( hwndDlg, 3), T2T );
                            SetFocus( GetDlgItem( hwndDlg, 3) );
                        }else{
                            EnableWindow( GetDlgItem( hwndDlg, 3), FALSE );
                            //SetWindowTextA( GetDlgItem( hwndDlg, 3), "0" );
                            SetFocus( GetDlgItem( hwndDlg, 4) );
                        }
                        if( FdsnC->IsIRR ){
                            FdsnC->IIRSet();
                        }else{
                            FdsnC->Fir1Set();
                        }
                        PltFD.PLOTDOUBLE( FdsnC->GetBuff(), FdsnC->GetMaxBuff()/2, 0, TRUE );
                        PltFD.SetMaxXLim( FdsnC->GetNyq(), TRUE );
                    }
                }
                    break;
                case 3:{
                    /// ALPHA
                    if( HIWORD( wParam )==EN_UPDATE ){
                        ParseEditDouble( wParam, lParam );
                    }else{
                        if( GetFocus()!=(HWND)lParam ){
                            break;
                        }
                        if( HIWORD( wParam )==EN_CHANGE || HIWORD( wParam )==EN_KILLFOCUS ){
                            GetWindowTextA( (HWND)lParam, T2T, 63 );
                            if( T2T[0]!=0 && HIWORD( wParam )!=EN_KILLFOCUS ){
                                FdsnC->SetAlpha( atof( T2T ) );
                                if( FdsnC->IsIRR ){
                                    FdsnC->IIRSet();
                                }else{
                                    FdsnC->Fir1Set();
                                }
                                PltFD.PLOTDOUBLE( FdsnC->GetBuff(), FdsnC->GetMaxBuff()/2, 0, TRUE );
                                PltFD.SetMaxXLim( FdsnC->GetNyq(), TRUE );
                            }else{
                                if( HIWORD( wParam )==EN_KILLFOCUS && T2T[0]==0 ){
                                    sprintf( T2T, "%g", FdsnC->GetAlpha() );
                                    SetWindowTextA( (HWND)lParam, T2T );
                                    if( FdsnC->IsIRR ){
                                        FdsnC->IIRSet();
                                    }else{
                                        FdsnC->Fir1Set();
                                    }
                                    PltFD.PLOTDOUBLE( FdsnC->GetBuff(), FdsnC->GetMaxBuff()/2, 0, TRUE );
                                    PltFD.SetMaxXLim( FdsnC->GetNyq(), TRUE );
                                    PltFD.SetMaxYLim( FdsnC->GetMaxFFTV() );
                                }
                            }
                        }
                    }
                    }
                    break;
                case 4:{
                    /// ORDEN DEL FILTRO
                    if( HIWORD( wParam )==EN_CHANGE || HIWORD( wParam )==EN_KILLFOCUS ){
                        GetWindowTextA( (HWND)lParam, T2T, 63 );
                        if( (atoi( T2T )==1 && !FdsnC->IsIRR) && HIWORD( wParam )==EN_CHANGE ){
                            break;
                        }
                        if( T2T[0]!=0 && HIWORD( wParam )==EN_CHANGE ){
                            if( atoi( T2T )>500 && FdsnC->IsIRR ){
                                SetWindowTextA( (HWND)lParam, "500" );
                                SendMessage( (HWND)lParam, (UINT)EM_SETSEL, 0, 3 );
                                break;
                            }
                            if( atoi( T2T )>512 ){
                                SetWindowTextA( (HWND)lParam, "512" );
                                SendMessage( (HWND)lParam, (UINT)EM_SETSEL, 0, 3 );
                                break;
                            }
                            FdsnC->SetOrdFil( atoi( T2T ) );
                            if( FdsnC->GetOrdFil()!=atoi( T2T ) ){
                                sprintf( T2T, "%i", FdsnC->GetOrdFil() );
                                SetWindowTextA( (HWND)lParam, T2T );
                                SendMessage( (HWND)lParam, (UINT)EM_SETSEL, 3, 3 );
                            }else{
                                if( FdsnC->IsIRR ){
                                    FdsnC->IIRSet();
                                    PltFD.PLOTDOUBLE( FdsnC->GetBuff(), FdsnC->GetMaxBuff()/2, 0, TRUE );
                                    PltFD.SetMaxXLim( FdsnC->GetNyq(), TRUE );
                                }else{
                                    FdsnC->Fir1Set();
                                    PltFD.PLOTDOUBLE( FdsnC->GetBuff(), FdsnC->GetMaxBuff()/2, 0, TRUE );
                                    PltFD.SetMaxXLim( FdsnC->GetNyq(), TRUE );
                                }
                            }
                        }else{
                            if( (HIWORD( wParam )==EN_KILLFOCUS && T2T[0]==0) ||
                                    ( FdsnC->GetOrdFil()!=atoi( T2T ) && HIWORD( wParam )==EN_KILLFOCUS ) ){
                                sprintf( T2T, "%i", FdsnC->GetOrdFil() );
                                SetWindowTextA( (HWND)lParam, T2T );
                                if( FdsnC->IsIRR ){
                                    FdsnC->IIRSet();
                                    PltFD.PLOTDOUBLE( FdsnC->GetBuff(), FdsnC->GetMaxBuff()/2, 0, TRUE );
                                    PltFD.SetMaxXLim( FdsnC->GetNyq(), TRUE );
                                }else{
                                    FdsnC->Fir1Set();
                                    PltFD.PLOTDOUBLE( FdsnC->GetBuff(), FdsnC->GetMaxBuff()/2, 0, TRUE );
                                    PltFD.SetMaxXLim( FdsnC->GetNyq(), TRUE );
                                }
                            }
                        }
                    }
                }
                    break;
                case 5:{
                    /// TIPO DEL FILTRO
                    if( HIWORD( wParam )==CBN_SELENDOK ){
                        FdsnC->SetFilType( (int)SendMessage( GetDlgItem( hwndDlg, 5), CB_GETCURSEL, 0, 0 ) );
                        FdsnC->SetOrdFil( FdsnC->GetOrdFil() );
                        EnableWindow( GetDlgItem( hwndDlg, 6),
                                     FdsnC->GetFilType()==1 || FdsnC->GetFilType()>1
                                     );
                        EnableWindow( GetDlgItem( hwndDlg, 7),
                                     FdsnC->GetFilType()==0 || FdsnC->GetFilType()>1
                                     );
                        sprintf( T2T, "%g", FdsnC->GetFcL() );
                        SetWindowTextA( GetDlgItem( hwndDlg, 6), T2T );
                        sprintf( T2T, "%g", FdsnC->GetFcH() );
                        SetWindowTextA( GetDlgItem( hwndDlg, 7), T2T );
                        ///
                        sprintf( T2T, "%i", FdsnC->GetOrdFil() );
                        SetWindowTextA( GetDlgItem( hwndDlg, 4), T2T );
                        /// SendMessage( GetDlgItem( hwndDlg, 5), CB_SETCURSEL, FdsnC->GetOrdFil(), 0 );
                        if( FdsnC->IsIRR ){
                            FdsnC->IIRSet();
                            PltFD.PLOTDOUBLE( FdsnC->GetBuff(), FdsnC->GetMaxBuff()/2, 0, TRUE );
                            PltFD.SetMaxXLim( FdsnC->GetNyq(), TRUE );
                        }else{
                            FdsnC->Fir1Set();
                            PltFD.PLOTDOUBLE( FdsnC->GetBuff(), FdsnC->GetMaxBuff()/2, 0, TRUE );
                            PltFD.SetMaxXLim( FdsnC->GetNyq(), TRUE );
                        }
                    }
                }
                    break;
                case 6:{/// FCL
                    if( HIWORD( wParam )==EN_UPDATE && ISDOWNKEY( VK_RETURN ) ){
                        goto Set6Proc;
                    }
                    if( HIWORD( wParam )==EN_UPDATE ){
                        ParseEditDouble( wParam, lParam );
                    }else{
                        if( HIWORD( wParam )==EN_KILLFOCUS ){
                            Set6Proc:
                            GetWindowTextA( (HWND)lParam, T2T, 63 );
                            if( !FdsnC->SetFcL( atof(T2T) ) ){
                                sprintf( T2T, "%g", FdsnC->GetFcL() );
                                SetWindowTextA( GetDlgItem( hwndDlg, 6), T2T );
                            }
                            if( FdsnC->IsIRR ){
                                FdsnC->IIRSet();
                                PltFD.PLOTDOUBLE( FdsnC->GetBuff(), FdsnC->GetMaxBuff()/2, 0, TRUE );
                                PltFD.SetMaxXLim( FdsnC->GetNyq(), TRUE );
                            }else{
                                FdsnC->Fir1Set();
                                PltFD.PLOTDOUBLE( FdsnC->GetBuff(), FdsnC->GetMaxBuff()/2, 0, TRUE );
                                PltFD.SetMaxXLim( FdsnC->GetNyq(), TRUE );
                            }
                        }
                    }
                }
                    break;
                    ///
                    ///
                case 7:{/// FCH
                    if( HIWORD( wParam )==EN_UPDATE && ISDOWNKEY( VK_RETURN ) ){
                        goto Set7Proc;
                    }
                    if( HIWORD( wParam )==EN_UPDATE ){
                        ParseEditDouble( wParam, lParam );
                    }else{
                        if( HIWORD( wParam )==EN_KILLFOCUS ){
                            Set7Proc:
                            GetWindowTextA( (HWND)lParam, T2T, 63 );
                            if( !FdsnC->SetFcH( atof(T2T) ) ){
                                sprintf( T2T, "%g", FdsnC->GetFcH() );
                                SetWindowTextA( GetDlgItem( hwndDlg, 7), T2T );
                            }
                            if( FdsnC->IsIRR ){
                                FdsnC->IIRSet();
                                PltFD.PLOTDOUBLE( FdsnC->GetBuff(), FdsnC->GetMaxBuff()/2, 0, TRUE );
                                PltFD.SetMaxXLim( FdsnC->GetNyq(), TRUE );
                            }else{
                                FdsnC->Fir1Set();
                                PltFD.PLOTDOUBLE( FdsnC->GetBuff(), FdsnC->GetMaxBuff()/2, 0, TRUE );
                                PltFD.SetMaxXLim( FdsnC->GetNyq(), TRUE );
                            }
                        }
                    }
                }
                    break;
/// /////////////////////////////////////////////////////////////////////////////////////////////////////////
                case 8:{/// FS
                    if( HIWORD( wParam )==EN_KILLFOCUS || (HIWORD( wParam )==EN_CHANGE && ISDOWNKEY( VK_RETURN )) ){
                        GetWindowTextA( (HWND)lParam, T2T, 63 );
                        if( T2T[0]==0 ){
                            sprintf( T2T, "%i", FdsnC->GetFs() );
                            SetWindowTextA( (HWND)lParam, T2T );
                            break;
                        }
                        if( !FdsnC->SetFs(atoi(T2T)) ){
                            sprintf( T2T, "%g", FdsnC->GetFcL() );
                            SetWindowTextA( GetDlgItem( hwndDlg, 6), T2T );
                            sprintf( T2T, "%g", FdsnC->GetFcH() );
                            SetWindowTextA( GetDlgItem( hwndDlg, 7), T2T );
                            sprintf( T2T, "%i", FdsnC->GetFs() );
                            SetWindowTextA( (HWND)lParam, T2T );
                        }
                        if( FdsnC->IsIRR ){
                            FdsnC->IIRSet();
                            PltFD.PLOTDOUBLE( FdsnC->GetBuff(), FdsnC->GetMaxBuff()/2, 0, TRUE );
                            PltFD.SetMaxXLim( FdsnC->GetNyq(), TRUE );
                        }else{
                            FdsnC->Fir1Set();
                            PltFD.PLOTDOUBLE( FdsnC->GetBuff(), FdsnC->GetMaxBuff()/2, 0, TRUE );
                            PltFD.SetMaxXLim( FdsnC->GetNyq(), TRUE );
                        }
                    }
                }
                    break;
/// /////////////////////////////////////////////////////////////////////////////////////////////////////////
/// /////////////////////////////////////////////////////////////////////////////////////////////////////////
                case 9:{/// BARRIDO
                    FdsnC->Performing_sweep = TRUE;
                    FdsnC->Sweep_Cancel = FALSE;
                    FdsnC->Ploter = (VOID*)&PltFD;
                    DWORD IdTh = 0;
                    CreateThread( NULL, 0, Sweep_Thread, (PVOID)FdsnC, 0, &IdTh );
                }
                    break;
/// /////////////////////////////////////////////////////////////////////////////////////////////////////////
/// /////////////////////////////////////////////////////////////////////////////////////////////////////////
                case 10:{/// COPIAR EJEMPLO
                    if( FdsnC->CpyExample() ){
                        MessageBoxA( hwndDlg, " Ejemplo copiado al portapapeles. ", "", 0 );
                    }else{
                        MessageBoxA( hwndDlg, " El ejemplo no se pudo copiar. ", "", 16 );
                    }
                }
                    break;
/// /////////////////////////////////////////////////////////////////////////////////////////////////////////
/// /////////////////////////////////////////////////////////////////////////////////////////////////////////
            }
            }
            break;
        case WM_CLOSE:
            ///
            ///
            if( FdsnC!=NULL ){
                if( FdsnC->Performing_sweep ){
                    FdsnC->Sweep_Cancel = TRUE;
                    return TRUE;
                }
            }
            ///
            ///
            ///
            FdsnC->SetOpen( FALSE );
            FdsnC = NULL;
            PltFD.FreeAll();
            DeleteObject( HFontT );
            EndDialog( hwndDlg, 0 );
            return TRUE;
    }
    return FALSE;
}
/// ///////////////////////////////////////////////////////////////////////////////////////////////////
/// ///////////////////////////////////////////////////////////////////////////////////////////////////
BOOL FDESIGN( HWND HW_Fath, VOID* FdsnC ){
    return (BOOL)DialogBoxParamA( GetModuleHandle(NULL), MAKEINTRESOURCE(Dial_Fdesign_GUI),
                                 HW_Fath, (DLGPROC)FDESIGN_Dlg, (LPARAM)FdsnC  );
}
/// ///////////////////////////////////////////////////////////////////////////////////////////////////
/// ///////////////////////////////////////////////////////////////////////////////////////////////////
/// ///////////////////////////////////////////////////////////////////////////////////////////////////
/// ///////////////////////////////////////////////////////////////////////////////////////////////////
/// ///////////////////////////////////////////////////////////////////////////////////////////////////
/// ///////////////////////////////////////////////////////////////////////////////////////////////////
/// ///////////////////////////////////////////////////////////////////////////////////////////////////
DWORD WINAPI Sweep_Thread( LPVOID pv=NULL ){
    Sleep( 1 );
    FDESIGN_CLASS* FdsnCSW = (FDESIGN_CLASS*)pv;
    FdsnCSW->Performing_sweep = TRUE;
    PLOTEOS* Ploter = (PLOTEOS*)FdsnCSW->Ploter;
    const double FcL = FdsnCSW->GetFcL();
    const double FcH = FdsnCSW->GetFcH();
    const double FDist = FcH - FcL;
    const int FNy = FdsnCSW->GetFNyq();
    const double TaFNy = ((double)FNy)/1000;
    for( double i=0.1; i<FdsnCSW->GetFNyq(); i = i + TaFNy ){
        if( FdsnCSW->Sweep_Cancel ){
            break;
        }
        ///
        if( FdsnCSW->GetFilType()<2 ){
            FdsnCSW->SetFcH(i);
            FdsnCSW->SetFcL(i);
        }else{
            if( (i+FDist)>=FdsnCSW->GetFNyq() ){
                break;
            }
            FdsnCSW->SetFcH(i+FDist);
            FdsnCSW->SetFcL( (double)i );
        }
        ///
        if( FdsnCSW->IsIRR ){
            FdsnCSW->IIRSet();
            Ploter->PLOTDOUBLE( FdsnCSW->GetBuff(), FdsnCSW->GetMaxBuff()/2, 0, TRUE );
        }else{
            FdsnCSW->Fir1Set();
            Ploter->PLOTDOUBLE( FdsnCSW->GetBuff(), FdsnCSW->GetMaxBuff()/2, 0, TRUE );
        }
        if( FNy<1000 ){
            Sleep( 1 );
        }
    }
    FdsnCSW->SetFcL(0.001);
    FdsnCSW->SetFcH(FcH);
    FdsnCSW->SetFcL(FcL);
    if( FdsnCSW->IsIRR ){
        FdsnCSW->IIRSet();
        Ploter->PLOTDOUBLE( FdsnCSW->GetBuff(), FdsnCSW->GetMaxBuff()/2, 0, TRUE );
    }else{
        FdsnCSW->Fir1Set();
        Ploter->PLOTDOUBLE( FdsnCSW->GetBuff(), FdsnCSW->GetMaxBuff()/2, 0, TRUE );
    }
    FdsnCSW->Sweep_Cancel = FALSE;
    FdsnCSW->Performing_sweep = FALSE;
    return 0;
}

/// ///////////////////////////////////////////////////////////////////////////////////////////////////
/// ///////////////////////////////////////////////////////////////////////////////////////////////////
/// ///////////////////////////////////////////////////////////////////////////////////////////////////
/// ///////////////////////////////////////////////////////////////////////////////////////////////////
/// ///////////////////////////////////////////////////////////////////////////////////////////////////
/// ///////////////////////////////////////////////////////////////////////////////////////////////////
/// ///////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CALLBACK FWINDOWING_Dlg( HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam ){
    static FDESIGN_CLASS* FdsnC = NULL;
    static double* DblPtr = NULL;
    static HWND HWTT[32] = {0};
    static char T2T[64] = {0};
    static PLOTEOS PltFD;
    switch( uMsg ){
        case WM_INITDIALOG:{
            const int WCONALL = 410;
            FdsnC = (FDESIGN_CLASS*)lParam;
            FdsnC->SetOpen( TRUE );
            MoveWindow( hwndDlg, 1, 1, WCONALL+8, WCONALL+62, TRUE );
            CenterWindow( hwndDlg, 0, 0 );
            for( int i=0; i<17; i++ ){
                SendMessage( GetDlgItem( hwndDlg, 1), CB_ADDSTRING, 0, (LPARAM)&Wind_2_Fir[i][0] );
            }
            SendMessage( GetDlgItem( hwndDlg, 1), CB_SETCURSEL, FdsnC->GetTypeWindow(), 0 );
            SetToolTip( GetDlgItem( hwndDlg, 1), "  Clase de ventana  ", &HWTT[0] );
            ///
            SetToolTip( GetDlgItem( hwndDlg, 2), "  Valor Alpha de la ventana  ", &HWTT[1] );
            ///
            SetWindowTextA( GetDlgItem( hwndDlg, 2), "" );
            EnableWindow( GetDlgItem( hwndDlg, 2), FdsnC->HaveWindowAlpha() );
            if( FdsnC->HaveWindowAlpha() ){
                sprintf( T2T, "%g", FdsnC->GetAlphaWindow() );
                SetWindowTextA( GetDlgItem( hwndDlg, 2), T2T );
            }
            ///
            ///
            PltFD.PLOTEOS_INIT();
            PltFD.SetBackG( 255 );
            PltFD.SetFather( hwndDlg );
            PltFD.SetTextVisible( FALSE );
            PltFD.SetGridStyle( 1 );
            PltFD.ActGridStyle( TRUE );
            PltFD.SetVisible( TRUE );
            PltFD.SetLocationCon( 1, 32 );
            PltFD.SetSizeCon( WCONALL, WCONALL );
            PltFD.FillGrid( TRUE );
            PltFD.SetTitle( " " );
            PltFD.Text2ShowF( "" );
            PltFD.SetMaxXLim( 1, TRUE );
            PltFD.SetMaxYLim( 1 );
            PltFD.SetMinYLim( 0 );
            PltFD.SetLineColor( LineColors[0] );
            PltFD.TextMx = NULL;
            ///
            ///
            DblPtr = (double*)malloc( 65536 * sizeof(double) + 1024 );
            for( int i=0; i<65536; i++ ){
                DblPtr[i] = 1;
            }
            FdsnC->SetWindowing( 65536, DblPtr );
            PltFD.PLOTDOUBLE( DblPtr, 65536, 0, TRUE );
            ///
            ///
        }
        break;
        ///
        ///
        ///

        case WM_COMMAND:{
            ///
            if( FdsnC==NULL || !IsWindowVisible(hwndDlg) ){
                break;
            }
            ///
            switch( LOWORD( wParam ) ){
                case 1:{
                    /// CLASE
                    if( HIWORD( wParam )==CBN_SELENDOK ){
                        FdsnC->SetTypeWindow( (int)SendMessage( GetDlgItem( hwndDlg, 1), CB_GETCURSEL, 0, 0 ) );
                        EnableWindow( GetDlgItem( hwndDlg, 2), FdsnC->HaveWindowAlpha() );
                        if( FdsnC->HaveWindowAlpha() ){
                            sprintf( T2T, "%g", FdsnC->GetAlphaWindow() );
                            SetWindowTextA( GetDlgItem( hwndDlg, 2), T2T );
                        }else{
                            SetWindowTextA( GetDlgItem( hwndDlg, 2), "" );
                        }
                        for( int i=0; i<65536; i++ ){
                            DblPtr[i] = 1;
                        }
                        FdsnC->SetWindowing( 65536, DblPtr );
                        PltFD.PLOTDOUBLE( DblPtr, 65536, 0, TRUE );
                    }
                }
                    break;
                case 2:{
                    /// ALPHA
                    if( HIWORD( wParam )==EN_UPDATE || HIWORD( wParam )==EN_KILLFOCUS ){
                        ParseEditDouble( wParam, lParam );
                        GetWindowTextA( (HWND)lParam, T2T, 63 );
                        if( HIWORD( wParam )==EN_KILLFOCUS && T2T[0]==0 ){
                            sprintf( T2T, "%g", FdsnC->GetAlphaWindow()  );
                            SetWindowTextA( GetDlgItem( hwndDlg, 2), T2T );
                        }else{
                            if( T2T[0]==0 ){
                                break;
                            }
                            FdsnC->SetAlphaWindow( atof(T2T) );
                        }
                    }
                    for( int i=0; i<65536; i++ ){
                        DblPtr[i] = 1;
                    }
                    FdsnC->SetWindowing( 65536, DblPtr );
                    PltFD.PLOTDOUBLE( DblPtr, 65536, 0, TRUE );
                }
            }
        }
        break;
        ///
        ///
        ///
        ///
        case WM_CLOSE:
            ///
            ///
            ///
            if( DblPtr!=NULL ){
                free( DblPtr );
                DblPtr = NULL;
            }
            FdsnC->SetOpen( FALSE );
            FdsnC = NULL;
            PltFD.FreeAll();
            EndDialog( hwndDlg, 0 );
            return TRUE;
    }
    return FALSE;
}
/// ///////////////////////////////////////////////////////////////////////////////////////////////////
/// ///////////////////////////////////////////////////////////////////////////////////////////////////
/// ///////////////////////////////////////////////////////////////////////////////////////////////////
/// ///////////////////////////////////////////////////////////////////////////////////////////////////
/// ///////////////////////////////////////////////////////////////////////////////////////////////////
/// ///////////////////////////////////////////////////////////////////////////////////////////////////
BOOL FWINDOWING( HWND HW_Fath, VOID* FdsnC ){
    return (BOOL)DialogBoxParamA( GetModuleHandle(NULL), MAKEINTRESOURCE(Dial_Fdesign_WIN_GUI),
                                 HW_Fath, (DLGPROC)FWINDOWING_Dlg, (LPARAM)FdsnC  );
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

/// ///////////////////////////////////////////////////////////////////////////////////////////////////
/// ///////////////////////////////////////////////////////////////////////////////////////////////////
/// ///////////////////////////////////////////////////////////////////////////////////////////////////
/// ///////////////////////////////////////////////////////////////////////////////////////////////////
/// ///////////////////////////////////////////////////////////////////////////////////////////////////
/// ///////////////////////////////////////////////////////////////////////////////////////////////////
/// ///////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CALLBACK FSPECTOGRAM_Dlg( HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam ){
    static FDESIGN_CLASS* FdsnC = NULL;
    static double* DblPtr = NULL;
    static HWND HWTT[32] = {0};
    static char T2T[64] = {0};
    static HFONT HFontT = NULL;
    static PLOTEOS PltFD;
    switch( uMsg ){
        case WM_INITDIALOG:{
            const int WCONALL = 450;
            FdsnC = (FDESIGN_CLASS*)lParam;
            FdsnC->SetOpen( TRUE );
            MoveWindow( hwndDlg, 1, 1, WCONALL+8, WCONALL+62, TRUE );
            CenterWindow( hwndDlg, 0, 0 );
            for( int i=0; i<17; i++ ){
                SendMessage( GetDlgItem( hwndDlg, 1), CB_ADDSTRING, 0, (LPARAM)&Wind_2_Fir[i][0] );
            }
            SendMessage( GetDlgItem( hwndDlg, 1), CB_SETCURSEL, FdsnC->SPecto.WINDWN.GetWinType(), 0 );
            SetToolTip( GetDlgItem( hwndDlg, 1), "  Clase de ventana aplicada al espectograma. ", &HWTT[0] );
            ///
            SetToolTip( GetDlgItem( hwndDlg, 2), "  Valor Alpha de la ventana aplicada al espectograma. ", &HWTT[1] );
            SetToolTip( GetDlgItem( hwndDlg, 3), "  Franja de división del espectograma, máximo 1 segundo. ", &HWTT[2] );
            ///
            /// ads
            SetWindowTextA( GetDlgItem( hwndDlg, 2), "" );
            EnableWindow( GetDlgItem( hwndDlg, 2), FdsnC->SPecto.WINDWN.HaveWindowAlpha() );
            if( FdsnC->SPecto.WINDWN.HaveWindowAlpha() ){
                sprintf( T2T, "%g", FdsnC->SPecto.WINDWN.GetAlpha() );
                SetWindowTextA( GetDlgItem( hwndDlg, 2), T2T );
            }
            ///
            ///
            sprintf( T2T, "%g", FdsnC->SPecto.GetTimeSeg()  );
            SetWindowTextA( GetDlgItem( hwndDlg, 3), T2T );
            ///
            ///
            PltFD.PLOTEOS_INIT();
            PltFD.SetBackG( 255 );
            PltFD.SetFather( hwndDlg );
            PltFD.SetTextVisible( FALSE );
            PltFD.SetGridStyle( 1 );
            PltFD.ActGridStyle( TRUE );
            PltFD.SetVisible( TRUE );
            PltFD.SetLocationCon( 1, 32 );
            PltFD.SetSizeCon( WCONALL, WCONALL );
            PltFD.FillGrid( TRUE );
            PltFD.SetTitle( " " );
            PltFD.Text2ShowF( "" );
            PltFD.SetMaxXLim( 1, TRUE );
            PltFD.SetMaxYLim( 1 );
            PltFD.SetMinYLim( 0 );
            PltFD.SetLineColor( LineColors[0] );
            PltFD.TextMx = NULL;
            ///
            ///
            HFontT = CreateFont( 16, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, DEFAULT_CHARSET,
                                OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, PROOF_QUALITY,
                                DEFAULT_PITCH | FF_ROMAN, "Times");
            SendMessage( GetDlgItem( hwndDlg, -1), WM_SETFONT, (WPARAM)HFontT, MAKELPARAM(TRUE, 0) );
            // SendMessage( GetDlgItem( hwndDlg, -2), WM_SETFONT, (WPARAM)HFontT, MAKELPARAM(TRUE, 0) );
            ///
            ///
            DblPtr = (double*)malloc( 65536 * sizeof(double) + 1024 );
            for( int i=0; i<65536; i++ ){
                DblPtr[i] = 1;
            }
            FdsnC->SPecto.WINDWN.Apply_Window( 65536, DblPtr );
            PltFD.PLOTDOUBLE( DblPtr, 65536, 0, TRUE );
            ///
            ///
        }
        break;
        ///
        ///
        ///
        case WM_COMMAND:{
            ///
            if( FdsnC==NULL || !IsWindowVisible(hwndDlg) ){
                break;
            }
            ///
            switch( LOWORD( wParam ) ){
                case 1:{
                    /// CLASE
                    if( HIWORD( wParam )==CBN_SELENDOK ){
                        FdsnC->SPecto.WINDWN.SetWinType( (int)SendMessage( GetDlgItem( hwndDlg, 1), CB_GETCURSEL, 0, 0 ) );
                        EnableWindow( GetDlgItem( hwndDlg, 2), FdsnC->SPecto.WINDWN.HaveWindowAlpha() );
                        if( FdsnC->SPecto.WINDWN.HaveWindowAlpha() ){
                            sprintf( T2T, "%g", FdsnC->SPecto.WINDWN.GetAlpha() );
                            SetWindowTextA( GetDlgItem( hwndDlg, 2), T2T );
                        }else{
                            SetWindowTextA( GetDlgItem( hwndDlg, 2), "" );
                        }
                        for( int i=0; i<65536; i++ ){
                            DblPtr[i] = 1;
                        }
                        FdsnC->SPecto.WINDWN.Apply_Window( 65536, DblPtr );
                        PltFD.PLOTDOUBLE( DblPtr, 65536, 0, TRUE );
                    }
                }
                    break;
/// ////////////////////////////////////////////////////////////////////////////////////////////////////////////
                case 2:{
                    /// ALPHA
                    if( HIWORD( wParam )==EN_UPDATE || HIWORD( wParam )==EN_KILLFOCUS ){
                        ParseEditDouble( wParam, lParam );
                        GetWindowTextA( (HWND)lParam, T2T, 63 );
                        if( HIWORD( wParam )==EN_KILLFOCUS && T2T[0]==0 ){
                            sprintf( T2T, "%g", FdsnC->SPecto.WINDWN.GetAlpha()  );
                            SetWindowTextA( GetDlgItem( hwndDlg, 2), T2T );
                        }else{
                            if( T2T[0]==0 ){
                                break;
                            }
                            FdsnC->SPecto.WINDWN.SetAlpha( atof(T2T) );
                        }
                    }
                    for( int i=0; i<65536; i++ ){
                        DblPtr[i] = 1;
                    }
                    FdsnC->SPecto.WINDWN.Apply_Window( 65536, DblPtr );
                    PltFD.PLOTDOUBLE( DblPtr, 65536, 0, TRUE );
                }
                break;
/// ////////////////////////////////////////////////////////////////////////////////////////////////////////////
                case 3:{
                    /// TIME_DIV
                    if( HIWORD( wParam )==EN_UPDATE || HIWORD( wParam )==EN_KILLFOCUS ){
                        ParseEditDouble( wParam, lParam );
                        GetWindowTextA( (HWND)lParam, T2T, 63 );
                        if( HIWORD( wParam )==EN_KILLFOCUS ){
                            if( T2T[0]==0 || atof(T2T)<0.0001 ){
                                sprintf( T2T, "%g", FdsnC->SPecto.GetTimeSeg()  );
                                SetWindowTextA( GetDlgItem( hwndDlg, 3), T2T );
                            }
                        }else{
                            if( atof(T2T)<0.0001 ){
                                break;
                            }
                            FdsnC->SPecto.SetTimeSeg( atof(T2T) );
                        }
                        ///
                        for( int i=0; i<65536; i++ ){
                            DblPtr[i] = 1;
                        }
                        FdsnC->SPecto.WINDWN.Apply_Window( 65536, DblPtr );
                        PltFD.PLOTDOUBLE( DblPtr, 65536, 0, TRUE );
                    }
                }
                break;
/// ////////////////////////////////////////////////////////////////////////////////////////////////////////////
            }
        }
        break;
        ///
        ///
        ///
        ///
        case WM_CLOSE:
            ///
            ///
            ///
            if( DblPtr!=NULL ){
                free( DblPtr );
                DblPtr = NULL;
            }
            DeleteObject( HFontT );
            FdsnC->SetOpen( FALSE );
            FdsnC = NULL;
            PltFD.FreeAll();
            EndDialog( hwndDlg, 0 );
            return TRUE;
    }
    return FALSE;
}
/// ///////////////////////////////////////////////////////////////////////////////////////////////////
/// ///////////////////////////////////////////////////////////////////////////////////////////////////
/// ///////////////////////////////////////////////////////////////////////////////////////////////////
/// ///////////////////////////////////////////////////////////////////////////////////////////////////
/// ///////////////////////////////////////////////////////////////////////////////////////////////////
/// ///////////////////////////////////////////////////////////////////////////////////////////////////
BOOL FSPECTOGRAM( HWND HW_Fath, VOID* FdsnC ){
    return (BOOL)DialogBoxParamA( GetModuleHandle(NULL), MAKEINTRESOURCE(Dial_Fspectogram_WIN_GUI),
                                 HW_Fath, (DLGPROC)FSPECTOGRAM_Dlg, (LPARAM)FdsnC  );
}
/// ///////////////////////////////////////////////////////////////////////////////////////////////////
/// ///////////////////////////////////////////////////////////////////////////////////////////////////
/// ///////////////////////////////////////////////////////////////////////////////////////////////////
/// ///////////////////////////////////////////////////////////////////////////////////////////////////
/// ///////////////////////////////////////////////////////////////////////////////////////////////////
/// ///////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CALLBACK CENT_SIGNAL_Dlg( HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam ){
    static FDESIGN_CLASS* FdsnC = NULL;
    static double* DblPtr = NULL;
    static short*  ShrPtr = NULL;
    static char T2T[64] = {0};
    static HFONT HFontT = NULL;
    static PLOTEOS PltFD;
    ///
    ///
    ///
    ///
    switch( uMsg ){
        case WM_INITDIALOG:{
            const int WCONALL = 450;
            FdsnC = (FDESIGN_CLASS*)lParam;
            FdsnC->SetOpen( TRUE );
            MoveWindow( hwndDlg, 1, 1, WCONALL+8, WCONALL/2+62, TRUE );
            CenterWindow( hwndDlg, 0, 0 );
            ///
            ///
            PltFD.PLOTEOS_INIT();
            PltFD.SetBackG( 255 );
            PltFD.SetFather( hwndDlg );
            PltFD.SetTextVisible( FALSE );
            PltFD.SetGridStyle( 1 );
            PltFD.ActGridStyle( TRUE );
            PltFD.SetVisible( TRUE );
            PltFD.SetLocationCon( 1, 32 );
            PltFD.SetSizeCon( WCONALL, WCONALL/2 );
            PltFD.FillGrid( TRUE );
            PltFD.SetTitle( " " );
            PltFD.Text2ShowF( " Tiempo(Segundos) " );
            PltFD.SetMaxXLim( 1, TRUE );
            PltFD.SetMaxYLim( 1 );
            PltFD.SetMinYLim( -1 );
            PltFD.SetLineColor( LineColors[0] );
            PltFD.TextMx = NULL;
            ///
            ///
            DblPtr = (double*)malloc( 2048 * sizeof(double) );
            ShrPtr = (short*)malloc( 2048 * sizeof(short) );
            ///
            ///
            sprintf( T2T, "%g", FdsnC->GetCentralValue()  );
            SetWindowTextA( GetDlgItem( hwndDlg, 3), T2T );
            ///
            ///
            ///
            double DVCent = FdsnC->GetCentralValue();
            double DVTemp = 0;
            double DVMax = 0;
            for( int i=0; i<2000; i++ ){
                DVTemp = ((double)FdsnC->ShortTemp[i])/32767;
                DVTemp = DVTemp - DVCent;
                DblPtr[i] = DVTemp;
                DVMax = fmax( fabs(DblPtr[i]), DVMax );
                /*
                if( fabs(DblPtr[i])>1 ){
                    if( DblPtr[i]<0 ){
                        DblPtr[i] = -1;
                    }else{
                        DblPtr[i] = 1;
                    }
                }
                //*/
            }
            ///
            ///
            DVTemp = -1e100;
            for( int i=0; i<2000; i++ ){
                ShrPtr[i] = (short)( 32767 * DblPtr[i]/DVMax );
            }
            PltFD.PLOTSHORT( ShrPtr, 2000, 0, TRUE );
            ///
            PltFD.SetTextVisible( TRUE );
            ///
            ///
            ///
            /// sprintf( T2T, "%g", DVMax );MessageBoxA( hwndDlg, T2T, "", 0 );
            ///
        }
        break;
        ///
        ///
        ///
        ///
        ///
        case WM_COMMAND:{
            if( FdsnC==NULL || !IsWindowVisible(hwndDlg) || DblPtr==NULL ){
                break;
            }
            switch( LOWORD( wParam ) ){
/// ////////////////////////////////////////////////////////////////////////////////////////////////////////////
                case 3:{
                    if( HIWORD( wParam )==EN_UPDATE || HIWORD( wParam )==EN_KILLFOCUS ){
                        ParseEditDouble( wParam, lParam );
                        GetWindowTextA( (HWND)lParam, T2T, 63 );
                        if( HIWORD( wParam )==EN_KILLFOCUS ){
                            if( T2T[0]==0 ){
                                sprintf( T2T, "%g", FdsnC->GetCentralValue()  );
                                SetWindowTextA( GetDlgItem( hwndDlg, 3), T2T );
                            }
                        }else{
                            if( T2T[0]!=0 ){
                                if( !FdsnC->SetCentralValue( atof(T2T) ) ){
                                    sprintf( T2T, "%g", FdsnC->GetCentralValue()  );
                                    SetWindowTextA( GetDlgItem( hwndDlg, 3), T2T );
                                }
                            }
                        }
                        ///
                        ///
                        ///
                        ///
                        ///
                        ///
                        ///
                        double DVCent = FdsnC->GetCentralValue();
                        double DVTemp = 0;
                        double DVMax = 0;
                        for( int i=0; i<2000; i++ ){
                            DVTemp = ((double)FdsnC->ShortTemp[i])/32767;
                            DVTemp = DVTemp - DVCent;
                            DblPtr[i] = DVTemp;
                            DVMax = fmax( fabs(DblPtr[i]), DVMax );
                            /*
                            if( fabs(DblPtr[i])>1 ){
                                if( DblPtr[i]<0 ){
                                    DblPtr[i] = -1;
                                }else{
                                    DblPtr[i] = 1;
                                }
                            }
                            //*/
                        }
                        ///
                        ///
                        DVTemp = -1e100;
                        for( int i=0; i<2000; i++ ){
                            ShrPtr[i] = (short)( 32767 * DblPtr[i]/DVMax );
                        }
                        PltFD.PLOTSHORT( ShrPtr, 2000, 0, TRUE );
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
                }
                break;
            }
        }
        break;
        ///
        ///
        ///
        case WM_CLOSE:
            if( DblPtr!=NULL ){
                free( DblPtr );
                DblPtr = NULL;
            }
            DeleteObject( HFontT );
            FdsnC->SetOpen( FALSE );
            FdsnC = NULL;
            PltFD.FreeAll();
            EndDialog( hwndDlg, 0 );
            return TRUE;
            break;
    }
    return FALSE;
}

/// ///////////////////////////////////////////////////////////////////////////////////////////////////
/// ///////////////////////////////////////////////////////////////////////////////////////////////////
/// ///////////////////////////////////////////////////////////////////////////////////////////////////
/// ///////////////////////////////////////////////////////////////////////////////////////////////////
/// ///////////////////////////////////////////////////////////////////////////////////////////////////
/// ///////////////////////////////////////////////////////////////////////////////////////////////////
/// ///////////////////////////////////////////////////////////////////////////////////////////////////
BOOL SIGNAL_CENTER( HWND HW_Fath, VOID* FdsnC ){
    return (BOOL)DialogBoxParamA( GetModuleHandle(NULL), MAKEINTRESOURCE(Dial_Center_Signal_WIN_GUI),
                                 HW_Fath, (DLGPROC)CENT_SIGNAL_Dlg, (LPARAM)FdsnC  );
}
/// ///////////////////////////////////////////////////////////////////////////////////////////////////
/// ///////////////////////////////////////////////////////////////////////////////////////////////////
/// ///////////////////////////////////////////////////////////////////////////////////////////////////
/// ///////////////////////////////////////////////////////////////////////////////////////////////////
/// ///////////////////////////////////////////////////////////////////////////////////////////////////
/// ///////////////////////////////////////////////////////////////////////////////////////////////////








