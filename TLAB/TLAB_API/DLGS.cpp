BOOL Dlg1D_INIT( HWND hwnd, UINT  message, WPARAM wParam, LPARAM lParam ){
    static HBITMAP HBMPS[4] = {NULL};
    static BOOL BSelected = FALSE;
    static DIM1* DIM1PTR = NULL;
    static HFONT HFTemp = NULL;
    static int SignalType = 0;
    char T2T[4*1024] = {0};
    switch(message){
        case WM_INITDIALOG:{
            DIM1PTR = (DIM1*)lParam;
            BSelected = FALSE;
            for( int i=0; i<4; i++ ){
                HBMPS[i] = LoadImageR( WAV_LOAD+i, MAKEINTRESOURCEA(FILE_RES) );
                SendMessage( GetDlgItem(hwnd, 100+i), BM_SETIMAGE, IMAGE_BITMAP,(LPARAM)HBMPS[i] );
            }
            HFTemp = CreateFont( 16, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, DEFAULT_CHARSET,
                                OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, PROOF_QUALITY,
                                    DEFAULT_PITCH | FF_ROMAN, "Times" );
            SendMessage( GetDlgItem(hwnd, 99), WM_SETFONT, (WPARAM)HFTemp, MAKELPARAM(TRUE, 0) );
            SignalType = -1;
            sprintf( T2T, "%i", DIM1PTR->SLOAD.FsT );
            SetWindowTextA( GetDlgItem(hwnd, 106), T2T );
            sprintf( T2T, "%g", DIM1PTR->SLOAD.fT );
            SetWindowTextA( GetDlgItem(hwnd, 107), T2T );
            ///
            if( DIM1PTR->SLOAD.ChannSelT==0 ){
                SendDlgItemMessage( hwnd, 110, BM_SETCHECK, (WPARAM)BST_CHECKED, 0 );
                SendDlgItemMessage( hwnd, 111, BM_SETCHECK, (WPARAM)BST_UNCHECKED, 0 );
            }else{
                SendDlgItemMessage( hwnd, 111, BM_SETCHECK, (WPARAM)BST_CHECKED, 0 );
                SendDlgItemMessage( hwnd, 110, BM_SETCHECK, (WPARAM)BST_UNCHECKED, 0 );
            }
            ///
            EnableWindow( GetDlgItem(hwnd, 110), FALSE );
            EnableWindow( GetDlgItem(hwnd, 111), FALSE );
            ShowWindow( GetDlgItem(hwnd, 110), 0 );
            ShowWindow( GetDlgItem(hwnd, 111), 0 );
            ShowWindow( GetDlgItem(hwnd, 107), 0 );
            ShowWindow( GetDlgItem(hwnd, 512), 0 );
            ///
            ShowWindow( hwnd, 1 );
        }
        break;
        case WM_COMMAND:{
            if( DIM1PTR==NULL ){
                break;
            }
            switch( LOWORD(wParam) ){
                case 10:
                    for( int i=0; i<4; i++ ){
                        DeleteObject( HBMPS[i] );
                    }
                    DIM1PTR->SLOAD.STypeT = SignalType;
                    EndDialog( hwnd, 1 );
                    break;
                case 11:
                    if( BSelected ){
                        SignalType = -1;
                        EnableWindow( GetDlgItem(hwnd, 100), TRUE );
                        EnableWindow( GetDlgItem(hwnd, 101), TRUE );
                        EnableWindow( GetDlgItem(hwnd, 102), TRUE );
                        EnableWindow( GetDlgItem(hwnd, 106), FALSE );
                        EnableWindow( GetDlgItem(hwnd, 107), FALSE );
                        EnableWindow( GetDlgItem(hwnd, 110), FALSE );
                        EnableWindow( GetDlgItem(hwnd, 111), FALSE );
                        ///
                        ShowWindow( GetDlgItem(hwnd, 110), 0 );
                        ShowWindow( GetDlgItem(hwnd, 111), 0 );
                        ShowWindow( GetDlgItem(hwnd, 107), 0 );
                        ShowWindow( GetDlgItem(hwnd, 512), 0 );
                        ///
                        EnableWindow( GetDlgItem(hwnd, 10), FALSE );
                        BSelected = FALSE;
                        SetWindowTextA( GetDlgItem(hwnd, 99), "" );
                        SetWindowTextA( GetDlgItem(hwnd, 106), "" );
                        SetWindowTextA( GetDlgItem(hwnd, 107), "" );
                        break;
                    }
                    for( int i=0; i<4; i++ ){
                        DeleteObject( HBMPS[i] );
                    }
                    SignalType = -1;
                    EndDialog( hwnd, SignalType );
                    break;
                case 100:
                    if( OpenFileAs( DIM1PTR->SLOAD.FNameT, "WAV Files (*.wav*)\0*.wav\0", hwnd ) ){
                        if( DIM1PTR->PreloadWAV(hwnd) ){
                            SignalType = 0;
                            BSelected = TRUE;
                            EnableWindow( GetDlgItem(hwnd, 101), FALSE );
                            EnableWindow( GetDlgItem(hwnd, 102), FALSE );
                            EnableWindow( GetDlgItem(hwnd, 103), FALSE );
                            EnableWindow( GetDlgItem(hwnd, 106), FALSE );
                            EnableWindow( GetDlgItem(hwnd, 10), TRUE );
                            SetWindowTextA( GetDlgItem(hwnd, 99), DIM1PTR->SLOAD.FNameT );
                            sprintf( T2T, "%i", DIM1PTR->SLOAD.FsT );
                            SetWindowTextA( GetDlgItem(hwnd, 106), T2T );
                            /// MessageBoxA( hwnd, DIM1PTR->SLOAD.FNameT, " ", 0 );
                            ///
                            if( DIM1PTR->SLOAD.NChannelsT>1 ){
                                MessageBoxA( hwnd, "El archivo contiene 2 canales, elija cual canal desea procesar.", "", 0 );
                                EnableWindow( GetDlgItem(hwnd, 110), TRUE );
                                EnableWindow( GetDlgItem(hwnd, 111), TRUE );
                                ShowWindow( GetDlgItem(hwnd, 110), 1 );
                                ShowWindow( GetDlgItem(hwnd, 111), 1 );
                            }
                            ///
                        }
                    }
                    break;
                case 101:
                    if( OpenFileAs( DIM1PTR->SLOAD.FNameT, "MAT Files (*.mat*)\0*.mat\0", hwnd ) ){
                        if( DIM1PTR->PreloadMAT(hwnd) ){
                            SignalType = 1;
                            BSelected = TRUE;
                            EnableWindow( GetDlgItem(hwnd, 100), FALSE );
                            EnableWindow( GetDlgItem(hwnd, 102), FALSE );
                            EnableWindow( GetDlgItem(hwnd, 103), FALSE );
                            EnableWindow( GetDlgItem(hwnd, 106), TRUE );
                            EnableWindow( GetDlgItem(hwnd, 10), TRUE );
                            SetWindowTextA( GetDlgItem(hwnd, 99), DIM1PTR->SLOAD.FNameT );
                            ///
                            sprintf( T2T, "%s\nVariable:  %s\nEscriba la frecuencia de muestreo(Fs).", DIM1PTR->SLOAD.FNameT, DIM1PTR->SLOAD.VarNameT );
                            /// MessageBoxA( hwnd, T2T, "", 0 );
                            Text2Edit( GetDlgItem(hwnd, 99), T2T );
                            /// SetWindowTextA( GetDlgItem(hwnd, 99), T2T );
                            ///
                            sprintf( T2T, "%i", DIM1PTR->SLOAD.FsT );
                            SetWindowTextA( GetDlgItem(hwnd, 106), T2T );
                        }else{
                            EnableWindow( GetDlgItem(hwnd, 100), TRUE );
                            EnableWindow( GetDlgItem(hwnd, 101), TRUE );
                            EnableWindow( GetDlgItem(hwnd, 102), TRUE );
                            EnableWindow( GetDlgItem(hwnd, 106), FALSE );
                            EnableWindow( GetDlgItem(hwnd, 10), FALSE );
                            BSelected = FALSE;
                            SetWindowTextA( GetDlgItem(hwnd, 99), "" );
                            SetWindowTextA( GetDlgItem(hwnd, 106), "" );
                        }
                    }else{
                        EnableWindow( GetDlgItem(hwnd, 100), TRUE );
                        EnableWindow( GetDlgItem(hwnd, 101), TRUE );
                        EnableWindow( GetDlgItem(hwnd, 102), TRUE );
                        EnableWindow( GetDlgItem(hwnd, 106), FALSE );
                        EnableWindow( GetDlgItem(hwnd, 10), FALSE );
                        BSelected = FALSE;
                        SetWindowTextA( GetDlgItem(hwnd, 99), "" );
                        SetWindowTextA( GetDlgItem(hwnd, 106), "" );
                    }
                    break;
                case 102:
                    SignalType = 2;
                    BSelected = TRUE;
                    EnableWindow( GetDlgItem(hwnd, 100), FALSE );
                    EnableWindow( GetDlgItem(hwnd, 101), FALSE );
                    EnableWindow( GetDlgItem(hwnd, 102), TRUE );
                    EnableWindow( GetDlgItem(hwnd, 106), TRUE );
                    EnableWindow( GetDlgItem(hwnd, 107), TRUE );
                    EnableWindow( GetDlgItem(hwnd, 10), TRUE );
                    ShowWindow( GetDlgItem(hwnd, 512), 1 );
                    ShowWindow( GetDlgItem(hwnd, 107), 1 );
                    sprintf( T2T, "%i", DIM1PTR->SLOAD.FsT );
                    SetWindowTextA( GetDlgItem(hwnd, 106), T2T );
                    sprintf( T2T, "%g", DIM1PTR->SLOAD.fT );
                    SetWindowTextA( GetDlgItem(hwnd, 107), T2T );
                    SetWindowTextA( GetDlgItem(hwnd, 99), "Elija la frecuencia de muestreo (Fs) "
                                   "y la frecuencia (f) de la señal senoidal." );
                    break;
                case 106:{
                    GetWindowTextA( GetDlgItem(hwnd, 106), T2T, 15 );
                    switch( HIWORD(wParam) ){
                        case EN_ERRSPACE:
                            MessageBoxA( hwnd, "", "", 0 );
                            if( T2T[0]==0 ){
                                sprintf( T2T, "%i", DIM1PTR->SLOAD.FsT );
                                SetWindowTextA( GetDlgItem(hwnd, 106), T2T );
                            }else{
                                DIM1PTR->SLOAD.FsT = atoi( T2T );
                            }
                            break;
                        case EN_KILLFOCUS:{
                            if( T2T[0]==0 ){
                                sprintf( T2T, "%i", DIM1PTR->SLOAD.FsT );
                                SetWindowTextA( GetDlgItem(hwnd, 106), T2T );
                                break;
                            }
                            if( atoi(T2T)%2!=0 ){
                                DIM1PTR->SLOAD.FsT = atoi(T2T)+1;
                                sprintf( T2T, "%i", DIM1PTR->SLOAD.FsT );
                                SetWindowTextA( GetDlgItem(hwnd, 106), T2T );
                                break;
                            }
                            if( atoi(T2T)<2 ){
                                DIM1PTR->SLOAD.FsT = 2;
                                sprintf( T2T, "%i", DIM1PTR->SLOAD.FsT );
                                SetWindowTextA( GetDlgItem(hwnd, 106), T2T );
                                break;
                            }
                            }
                            break;
                        case EN_UPDATE:{
                            if( T2T[0]!=0 ){
                                if( atoi(T2T)>192000 ){
                                    SetWindowTextA( GetDlgItem(hwnd, 106), "192000" );
                                    DIM1PTR->SLOAD.FsT = 192000;
                                    SendMessage( (HWND)lParam, (UINT)EM_SETSEL, strlen(T2T), strlen(T2T) );
                                    break;
                                }else{
                                    if( atoi(T2T)<2 ){
                                        break;
                                    }
                                }
                                if( atoi(T2T)%2!=0 ){
                                    break;
                                }
                                DIM1PTR->SLOAD.FsT = atoi(T2T);
                            }
                        }
                            break;
                    }
                    }
                    break;
                case 107:
                    GetWindowTextA( GetDlgItem(hwnd, 107), T2T, 15 );
                    switch( HIWORD(wParam) ){
                        case EN_KILLFOCUS:{
                            if( T2T[0]==0 ){
                                sprintf( T2T, "%g", DIM1PTR->SLOAD.fT );
                                SetWindowTextA( GetDlgItem(hwnd, 107), T2T );
                                break;
                            }
                        case EN_CHANGE:{
                            ParseEditDouble( wParam, lParam );
                        }
                        break;
                        case EN_UPDATE:{
                            if( T2T[0]!=0 ){
                                DIM1PTR->SLOAD.fT = atof( T2T );
                            }
                        }
                        break;
                    }
                    }
                    break;
                case 110:
                    DIM1PTR->SLOAD.ChannSelT = 0;
                    SendDlgItemMessage( hwnd, 110, BM_SETCHECK, (WPARAM)BST_CHECKED, 0 );
                    SendDlgItemMessage( hwnd, 111, BM_SETCHECK, (WPARAM)BST_UNCHECKED, 0 );
                    break;
                case 111:
                    DIM1PTR->SLOAD.ChannSelT = 1;
                    SendDlgItemMessage( hwnd, 111, BM_SETCHECK, (WPARAM)BST_CHECKED, 0 );
                    SendDlgItemMessage( hwnd, 110, BM_SETCHECK, (WPARAM)BST_UNCHECKED, 0 );
                    break;

            }
        }
        break;
    }
    return FALSE;
}
/// //////////////////////////////////////////////////////////////////////////////////////////
/// //////////////////////////////////////////////////////////////////////////////////////////
/// //////////////////////////////////////////////////////////////////////////////////////////
/// //////////////////////////////////////////////////////////////////////////////////////////
/// //////////////////////////////////////////////////////////////////////////////////////////
BOOL DlgProgress12D( HWND hwnd, UINT  message, WPARAM wParam, LPARAM lParam ){
    static TLAB* DLGPTR = NULL;
    switch(message){
        case WM_INITDIALOG:
            DLGPTR = (TLAB*)lParam;
            if( DLGPTR==NULL ){
                HWPGRGEN_MAIN = hwnd;
                HWPGRGEN      = GetDlgItem( hwnd, 100 );
                SendMessage( GetDlgItem( hwnd, 100 ), PBM_SETRANGE, 0, MAKELPARAM(0, 100) );
                SetWindowTextA( HWPGRGEN_MAIN, " Iniciando TLAB " );
                break;
            }
            DLGPTR->HWProgress = hwnd;
            DLGPTR->HW_Progr = GetDlgItem( hwnd, 100 );
            SetWindowTextA( hwnd, DLGPTR->AUXTEXT );
            ///EnableWindow( GetParent(hwnd), FALSE );
            SendMessage( DLGPTR->HW_Progr, PBM_SETRANGE, 0, MAKELPARAM(0, 100) );
            break;
        case WM_DESTROY:
        case WM_CLOSE:
            if( DLGPTR!=NULL ){
                DLGPTR->HWProgress = NULL;
                DLGPTR->HW_Progr = NULL;
            }
            /// EnableWindow( GetParent(hwnd), TRUE );
            DLGPTR = NULL;
            EndDialog( hwnd, 0 );
            break;
    }
    return FALSE;
}
/// //////////////////////////////////////////////////////////////////////////////////////////
/// //////////////////////////////////////////////////////////////////////////////////////////
/// //////////////////////////////////////////////////////////////////////////////////////////
/// //////////////////////////////////////////////////////////////////////////////////////////
/// //////////////////////////////////////////////////////////////////////////////////////////
//*
BOOL CALLBACK DlgProc_Proc_1D_TAD(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam){
    static HWND HW_DP_TI = NULL;
    static HWND HW_LB_PRC = NULL;
    static FDESIGN_CLASS* FDSP = NULL;
    static int ITemp = 0;
    /// char CTINFO[1024] = {0};
    const char ProcInfo[16][32] = { "Gráfica de la señal",
                                    "Aplica un filtro a la señal",
                                    "Aplica una ventana a la señal",
                                    "Aplica la fft a la señal",
                                    "Análisis por espectrograma",
                                    "Análisis Wigner - Ville",
                                    "Análisis Choi - Williams",
                                    "Reproduce la señal",
                                    "Sin procedimiento",
                                    "Eliminar el offset"
                                     };
    switch(msg){
        case WM_INITDIALOG:{
            FDSP = (FDESIGN_CLASS*)lParam;
            HW_DP_TI = GetDlgItem( hDlg, 10 );
            ///
            HW_LB_PRC = GetDlgItem( hDlg, 20 );
            SendMessage( HW_LB_PRC, LB_ADDSTRING, 0, (LPARAM)"Gráfica");
            ///SendMessage( HW_LB_PRC, LB_ADDSTRING, 0, (LPARAM)"Convolución");
            ///SendMessage( HW_LB_PRC, LB_ADDSTRING, 0, (LPARAM)"Correlación");
            SendMessage( HW_LB_PRC, LB_ADDSTRING, 0, (LPARAM)"Filtrado");
            SendMessage( HW_LB_PRC, LB_ADDSTRING, 0, (LPARAM)"Ventanas");
            SendMessage( HW_LB_PRC, LB_ADDSTRING, 0, (LPARAM)"Analisis espectral(fft)");
            SendMessage( HW_LB_PRC, LB_ADDSTRING, 0, (LPARAM)"Espectrograma");
            SendMessage( HW_LB_PRC, LB_ADDSTRING, 0, (LPARAM)"Wigner - Ville");
            SendMessage( HW_LB_PRC, LB_ADDSTRING, 0, (LPARAM)"Choi - Williams");
            SendMessage( HW_LB_PRC, LB_ADDSTRING, 0, (LPARAM)"Reproducir señal");
            SendMessage( HW_LB_PRC, LB_ADDSTRING, 0, (LPARAM)"Sin procedimiento");
            /// SendMessage( HW_LB_PRC, LB_ADDSTRING, 0, (LPARAM)"Eliminar offset");
            SendMessage( HW_LB_PRC, LB_SETCURSEL, (WPARAM)FDSP->GetTYPE_PROC(), 0 );
            SetWindowTextA( HW_DP_TI, ProcInfo[SendMessage( HW_LB_PRC, LB_GETCURSEL, 0, 0 )] );
            ///
            ///
            SendDlgItemMessage( hDlg, 30, BM_SETCHECK, FDSP->GetBInput(), 0 );
            ///
            /// CenterWindow( hDlg, 0, 60 );
            ///
            ///
            ///
            }
            return TRUE;
        case WM_COMMAND:
            if( FDSP==NULL ){
                break;
            }
            switch( LOWORD(wParam) ){
                case 20:
                    switch( HIWORD(wParam) ){
                        case LBN_SELCHANGE:
                            //sprintf( CTINFO, "%s", ProcInfo[SendMessage( HW_LB_PRC, LB_GETCURSEL, 0, 0 )] );
                            SetWindowTextA( HW_DP_TI, ProcInfo[SendMessage( HW_LB_PRC, LB_GETCURSEL, 0, 0 )] );
                            break;
                        case LBN_DBLCLK:
                            FDSP->SetBInput( SendDlgItemMessage(hDlg, 30, BM_GETCHECK, 0, 0) == BST_CHECKED );
                            FDSP->SetTYPE_PROC( SendMessage( HW_LB_PRC, LB_GETCURSEL, 0, 0 ) );
                            ITemp = 128*FDSP->GetBInput() + FDSP->GetTYPE_PROC();
                            FDSP = NULL;
                            /// EndDialog( hDlg, SendMessage( HW_LB_PRC, LB_GETCURSEL, 0, 0 ) );
                            EndDialog( hDlg, ITemp );
                            break;
                    }
                    break;
                case IDOK:
                    FDSP->SetTYPE_PROC( SendMessage( HW_LB_PRC, LB_GETCURSEL, 0, 0 ) );
                    FDSP->SetBInput( SendDlgItemMessage(hDlg, 30, BM_GETCHECK, 0, 0) == BST_CHECKED );
                    ITemp = 128*FDSP->GetBInput() + FDSP->GetTYPE_PROC();
                    FDSP = NULL;
                    /// EndDialog( hDlg, SendMessage( HW_LB_PRC, LB_GETCURSEL, 0, 0 ) );
                    EndDialog( hDlg, ITemp );
                    break;
                case IDCANCEL:
                    FDSP = NULL;
                    EndDialog( hDlg, -2 );
                    break;
            }
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
            SetToolTip( GetDlgItem( hwndDlg, 1), "  Clase de ventana aplicada al espectrograma. ", &HWTT[0] );
            ///
            SetToolTip( GetDlgItem( hwndDlg, 256), "  Valor Alpha de la ventana aplicada al espectrograma. ", &HWTT[1] );
            SetToolTip( GetDlgItem( hwndDlg, 3), "  Franja de división del espectrograma, máximo 1 segundo. ", &HWTT[2] );
            ///
            /// ads
            SetWindowTextA( GetDlgItem( hwndDlg, 256), "" );
            EnableWindow( GetDlgItem( hwndDlg, 256), FdsnC->SPecto.WINDWN.HaveWindowAlpha() );
            if( FdsnC->SPecto.WINDWN.HaveWindowAlpha() ){
                sprintf( T2T, "%g", FdsnC->SPecto.WINDWN.GetAlpha() );
                SetWindowTextA( GetDlgItem( hwndDlg, 256), T2T );
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
            ///
            /// sprintf( T2T, "%i", (int)LOWORD(wParam) );SetWindowTextA( hwndDlg, T2T );
            if( LOWORD(wParam)==IDCANCEL ){
                if( DblPtr!=NULL ){
                    free( DblPtr );
                    DblPtr = NULL;
                }
                DeleteObject( HFontT );
                FdsnC->SetOpen( FALSE );
                FdsnC = NULL;
                PltFD.FreeAll();
                EndDialog( hwndDlg, 0 );
                break;
            }
            ///
            ///
            ///
            /// sprintf( T2T, "%i", (int)LOWORD(wParam) );SetWindowTextA( hwndDlg, T2T );
            if( LOWORD(wParam)==IDCANCEL ){
                if( DblPtr!=NULL ){
                    free( DblPtr );
                    DblPtr = NULL;
                }
                DeleteObject( HFontT );
                FdsnC->SetOpen( FALSE );
                FdsnC = NULL;
                PltFD.FreeAll();
                EndDialog( hwndDlg, 0 );
                break;
            }
            ///
            ///
            ///
            switch( LOWORD( wParam ) ){
                case 1:{
                    /// CLASE
                    if( HIWORD( wParam )==CBN_SELENDOK ){
                        FdsnC->SPecto.WINDWN.SetWinType( (int)SendMessage( GetDlgItem( hwndDlg, 1), CB_GETCURSEL, 0, 0 ) );
                        EnableWindow( GetDlgItem( hwndDlg, 256), FdsnC->SPecto.WINDWN.HaveWindowAlpha() );
                        if( FdsnC->SPecto.WINDWN.HaveWindowAlpha() ){
                            sprintf( T2T, "%g", FdsnC->SPecto.WINDWN.GetAlpha() );
                            SetWindowTextA( GetDlgItem( hwndDlg, 256), T2T );
                        }else{
                            SetWindowTextA( GetDlgItem( hwndDlg, 256), "" );
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
                case 256:{
                    /// ALPHA
                    if( HIWORD( wParam )==EN_UPDATE || HIWORD( wParam )==EN_KILLFOCUS ){
                        ParseEditDouble( wParam, lParam );
                        GetWindowTextA( (HWND)lParam, T2T, 63 );
                        if( HIWORD( wParam )==EN_KILLFOCUS && T2T[0]==0 ){
                            sprintf( T2T, "%g", FdsnC->SPecto.WINDWN.GetAlpha()  );
                            SetWindowTextA( GetDlgItem( hwndDlg, 256), T2T );
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
/// /////////////////////////////////////////////////////////////////////////////////////////////////////
/// /////////////////////////////////////////////////////////////////////////////////////////////////////
/// /////////////////////////////////////////////////////////////////////////////////////////////////////
/// /////////////////////////////////////////////////////////////////////////////////////////////////////
/// /////////////////////////////////////////////////////////////////////////////////////////////////////
/// /////////////////////////////////////////////////////////////////////////////////////////////////////
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
BOOL FWINDOWING( HWND HW_Fath, VOID* FdsnC ){
    return (BOOL)DialogBoxParamA( GetModuleHandle(NULL), MAKEINTRESOURCE(Dial_Fdesign_WIN_GUI),
                                 HW_Fath, (DLGPROC)FWINDOWING_Dlg, (LPARAM)FdsnC  );
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
/// /////////////////////////////////////////////////////////////////////////////////////////////////////
/// /////////////////////////////////////////////////////////////////////////////////////////////////////
/// /////////////////////////////////////////////////////////////////////////////////////////////////////
/// /////////////////////////////////////////////////////////////////////////////////////////////////////
/// /////////////////////////////////////////////////////////////////////////////////////////////////////
/// /////////////////////////////////////////////////////////////////////////////////////////////////////
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
            /// MessageBoxA( GetAncestor(hwndDlg, GA_ROOT), "", "", 0 );
            SendMessage( GetDlgItem( hwndDlg, 1), CB_ADDSTRING, 0, (LPARAM)"FIR" );
            SendMessage( GetDlgItem( hwndDlg, 1), CB_ADDSTRING, 0, (LPARAM)"BUTTERWORTH" );
            SendMessage( GetDlgItem( hwndDlg, 1), CB_SETCURSEL, (int)FdsnC->IsIRR, 0 );
            for( int i=0; i<18; i++ ){
                SendMessage( GetDlgItem( hwndDlg, 256), CB_ADDSTRING, 0, (LPARAM)&Wind_2_Fir[i][0] );
            }
            SendMessage( GetDlgItem( hwndDlg, 256), CB_SETCURSEL, FdsnC->GetWType(), 0 );
            EnableWindow( GetDlgItem( hwndDlg, 256), !FdsnC->IsIRR );
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
            SetToolTip( GetDlgItem( hwndDlg, 256), "Clase de ventana aplicada al filtro FIR.", &HWTT[1] );
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
            /// sprintf( T2T, "%i", FdsnC->GetFs() );MessageBoxA( hwndDlg, T2T, "", 0 );
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
            PltFD.SetDoubleBuff( FALSE );
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
            MoveWindow( hwndDlg, 1, 1, 700, 390, TRUE );
            CenterWindow( hwndDlg, 0, 0 );
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
                PltFD.SetDoubleBuff( FALSE );
                break;
            }
            ///
            ///
            /// sprintf( T2T, "%i", (int)LOWORD(wParam) );SetWindowTextA( hwndDlg, T2T );
            if( LOWORD(wParam)==IDCANCEL ){
                ///
                if( FdsnC!=NULL ){
                    if( FdsnC->Performing_sweep ){
                        FdsnC->Sweep_Cancel = TRUE;
                        return TRUE;
                    }
                }
                ///
                FdsnC->SetOpen( FALSE );
                FdsnC = NULL;
                PltFD.FreeAll();
                DeleteObject( HFontT );
                EndDialog( hwndDlg, 0 );
                break;
            }
            ///
            ///
            switch( LOWORD( wParam ) ){
                case 1:{
                    /// CLASE
                    if( HIWORD( wParam )==CBN_SELENDOK ){
                        FdsnC->IsIRR = (BOOL)SendMessage( GetDlgItem( hwndDlg, 1), CB_GETCURSEL, 0, 0 );
                        EnableWindow( GetDlgItem( hwndDlg, 256), !FdsnC->IsIRR );
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
                case 256:{
                    /// VENTANA
                    if( HIWORD( wParam )==CBN_SELENDOK ){
                        FdsnC->SetWType( (int)SendMessage( GetDlgItem( hwndDlg, 256), CB_GETCURSEL, 0, 0 ) );
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
                            if( atoi( T2T )>MaxFIR_L ){
                                sprintf( T2T, "%i", MaxFIR_L );
                                SetWindowTextA( (HWND)lParam, T2T );
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
                    // PltFD.SetDoubleBuff( TRUE );
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
            FdsnC->Free();
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
            for( int i=0; i<17; i++ ){
                SendMessage( GetDlgItem( hwndDlg, 1), CB_ADDSTRING, 0, (LPARAM)&Wind_2_Fir[i][0] );
            }
            SendMessage( GetDlgItem( hwndDlg, 1), CB_SETCURSEL, FdsnC->GetTypeWindow(), 0 );
            SetToolTip( GetDlgItem( hwndDlg, 1), "  Clase de ventana  ", &HWTT[0] );
            ///
            SetToolTip( GetDlgItem( hwndDlg, 256), "  Valor Alpha de la ventana  ", &HWTT[1] );
            ///
            SetWindowTextA( GetDlgItem( hwndDlg, 256), "" );
            EnableWindow( GetDlgItem( hwndDlg, 256), FdsnC->HaveWindowAlpha() );
            if( FdsnC->HaveWindowAlpha() ){
                sprintf( T2T, "%g", FdsnC->GetAlphaWindow() );
                SetWindowTextA( GetDlgItem( hwndDlg, 256), T2T );
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
            MoveWindow( hwndDlg, 1, 1, WCONALL+8, WCONALL+62, TRUE );
            CenterWindow( hwndDlg, 0, 0 );
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
            /// sprintf( T2T, "%i", (int)LOWORD(wParam) );SetWindowTextA( hwndDlg, T2T );
            if( LOWORD(wParam)==IDCANCEL ){
                if( DblPtr!=NULL ){
                    free( DblPtr );
                    DblPtr = NULL;
                }
                FdsnC->SetOpen( FALSE );
                FdsnC = NULL;
                PltFD.FreeAll();
                EndDialog( hwndDlg, 0 );
                break;
            }
            ///
            ///
            switch( LOWORD( wParam ) ){
                case 1:{
                    /// CLASE
                    if( HIWORD( wParam )==CBN_SELENDOK ){
                        FdsnC->SetTypeWindow( (int)SendMessage( GetDlgItem( hwndDlg, 1), CB_GETCURSEL, 0, 0 ) );
                        EnableWindow( GetDlgItem( hwndDlg, 256), FdsnC->HaveWindowAlpha() );
                        if( FdsnC->HaveWindowAlpha() ){
                            sprintf( T2T, "%g", FdsnC->GetAlphaWindow() );
                            SetWindowTextA( GetDlgItem( hwndDlg, 256), T2T );
                        }else{
                            SetWindowTextA( GetDlgItem( hwndDlg, 256), "" );
                        }
                        for( int i=0; i<65536; i++ ){
                            DblPtr[i] = 1;
                        }
                        FdsnC->SetWindowing( 65536, DblPtr );
                        PltFD.PLOTDOUBLE( DblPtr, 65536, 0, TRUE );
                    }
                }
                    break;
                case 256:{
                    /// ALPHA
                    if( HIWORD( wParam )==EN_UPDATE || HIWORD( wParam )==EN_KILLFOCUS ){
                        ParseEditDouble( wParam, lParam );
                        GetWindowTextA( (HWND)lParam, T2T, 63 );
                        if( HIWORD( wParam )==EN_KILLFOCUS && T2T[0]==0 ){
                            sprintf( T2T, "%g", FdsnC->GetAlphaWindow()  );
                            SetWindowTextA( GetDlgItem( hwndDlg, 256), T2T );
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
#include "DLG_2_D.cpp"
