BOOL CALLBACK DlgShowPlotBig( HWND hwnd, UINT  message, WPARAM wParam, LPARAM lParam ){
    static DIM1*            DMPTR = NULL;
    static double*          DblPtr = NULL;
    static double           MaxValFFT = 0;
    static double           BLen = 0;
    static int              IMod = 0;
    static int              F_s = 0;
    ///
    static int              WWIN = 640;
    static int              HWIN = 480;
    static int              WWIN_I = 640;
    static int              HWIN_I = 480;
    ///
    static int              XPWIN_I = 120;
    static int              YPWIN_I = 60;
    static int              XPWIN = 120;
    static int              YPWIN = 60;
    static BOOL             isMaximized = FALSE;
    static BOOL             isShow = FALSE;
    static HWND             HWCursor = NULL;
    static PLOTEOS          Pltr;
    switch( message ){
        case WM_INITDIALOG:{
            WWIN = WWIN_I;
            HWIN = HWIN_I;
            ///
            XPWIN = XPWIN_I;
            YPWIN = YPWIN_I;
            ///
            isShow = FALSE;
            TLAB* TLBPTR = (TLAB*)lParam;
            DMPTR = &(TLBPTR->Dim1Procs[TLBPTR->BSel_1D]);
            TLBPTR->HW_PLOT_BIG = hwnd;
            ///
            SendMessage( hwnd , WM_SETICON , ICON_SMALL , (LPARAM)LoadIconA( GetModuleHandleA(NULL), MAKEINTRESOURCEA(TLAB_ICON) ) );
            SendMessage( hwnd , WM_SETICON , ICON_BIG , (LPARAM)LoadIconA( GetModuleHandleA(NULL) , MAKEINTRESOURCEA(TLAB_ICON) ) );
            ///
            ///
            HWCursor = CreateWindowEx( WS_EX_TOPMOST, "STATIC", "X: 3600\nY: 65535",
                                      SS_NOTIFY | WS_BORDER | WS_POPUP | WS_CHILD | WS_VISIBLE, 100, 100, 64, 64,
                               hwnd, NULL,GetModuleHandleA(NULL), NULL );
            ///SetWindowPos( GetDlgItem( hwnd, 128 ), HWND_TOPMOST, 100, 100, 64, 64, 1 );
            MoveWindow( HWCursor, 100, 100, 64, 64, TRUE );
            ShowWindow( HWCursor, 0 );
            ///
            ///
            ///
            BLen = DMPTR->NPosFFT;
            if( DMPTR->ProcType!=FFT_Signal ){
                BLen = DMPTR->NPosBuff;
            }
            F_s = DMPTR->FDS.GetFs();
            MaxValFFT = DMPTR->MaxValFFT;
            ///
            IMod = DMPTR->Ind_File;
            ///
            DblPtr = (double*)malloc( BLen * sizeof(double) + 1024 );
            ZeroMemory( DblPtr, BLen * sizeof(double) + 1023 );
            DMPTR->ReadDatas( DblPtr, BLen * sizeof(double) );
            ///
            Pltr.PLOTEOS_INIT();
            Pltr.SetFather( hwnd ); /// HWTABS   hwnd                    .
            Pltr.SetBackG( 255 );
            Pltr.SetGridStyle( 1 );
            Pltr.ActGridStyle( TRUE );
            Pltr.SetVisible( TRUE );
            Pltr.SetLocationCon( 0, 0 );
            Pltr.SetSizeCon( WWIN, HWIN );
            Pltr.SetTitle( " Señales de entrada " );
            Pltr.Text2ShowF( " Tiempo(Segundos) " );
            Pltr.SetMaxYLim( 1 );
            Pltr.SetMinYLim( 0 );
            Pltr.SetMaxXLim( 1, TRUE );
            Pltr.SetVisible( TRUE );
            ///
            MoveWindow( hwnd, XPWIN-8, YPWIN-30, WWIN+16, HWIN+39, TRUE );
            ///
        }
            break;
        /// ///////////////////////////
        /// ///////////////////////////
        /// ///////////////////////////
        case WM_SHOWWINDOW:{
            if( !isShow ){
                char T2T[1024] = {0};
                WindowSize( hwnd, &WWIN, &HWIN );
                Pltr.SetSizeCon( WWIN, HWIN );
                Pltr.SetLineColor( 0, 0, 0 );
                Pltr.FillGrid( FALSE );
                Pltr.SetLineColor( 0, 0, 200 );
                switch( DMPTR->ProcType ){
                    case FFT_Signal:{
                        ShowWindow( Pltr.GetHWSpect(), 0 );
                        Pltr.SetMaxYLim( MaxValFFT );
                        Pltr.SetMinYLim( 0 );
                        Pltr.TextMx = "";//"Hz ";
                        Pltr.TextMy = NULL;
                        Pltr.SetMaxXLim( F_s/2, FALSE );
                        Pltr.Text2ShowF( " Frecuencia (Hz) " );
                        sprintf( T2T, "Módulo %i, FFT( Transformada rápida de FOURIER )", IMod );
                        Pltr.SetTitle( T2T );
                        Pltr.PLOT_DOUBLE_FFT( DblPtr, BLen/2, 0, TRUE );
                    }
                        break;
                    default:
                        Pltr.PLOTDOUBLE_BIPOL( DblPtr, BLen, 0, TRUE );
                        break;
                }
                isShow = TRUE;
            }
        }
            break;
        /// /////////////////////////////////
        /// /////////////////////////////////
        /// /////////////////////////////////
        case WM_MOVE:{
            break;
            short* BPtr = (short*)&lParam;
            MoveWindow( HWCursor, BPtr[0]+100, BPtr[1]+100, 64, 32, TRUE );
        }
            break;
        /// /////////////////////////////////
        /// /////////////////////////////////
        /// /////////////////////////////////
        case WM_SIZE:{
            if( wParam==SIZE_MAXIMIZED ){
                isMaximized = TRUE;
            }else{
                if( isMaximized ){
                    isMaximized = FALSE;
                }else{
                    break;
                }
            }
        }
            ///break;
        case WM_EXITSIZEMOVE:{
            WindowPos( hwnd, &XPWIN, &YPWIN, FALSE );
            if( wParam!=SIZE_MAXIMIZED && wParam!=SIZE_MINIMIZED ){
                WindowPos( hwnd, &XPWIN_I, &YPWIN_I, FALSE );
            }
            int W_WIN = 0;
            int H_WIN = 0;
            WindowSize( hwnd, &W_WIN, &H_WIN );
            if( WWIN!=W_WIN || HWIN!=H_WIN ){
                char T2T[1024] = {0};
                WWIN = W_WIN;
                HWIN = H_WIN;
                if( wParam!=SIZE_MAXIMIZED ){
                    WindowSize( hwnd, &WWIN_I, &HWIN_I );
                }
                Pltr.SetLineColor( 0, 0, 0 );
                Pltr.SetSizeCon( WWIN, HWIN );
                Pltr.FillGrid( FALSE );
                Pltr.SetLineColor( 0, 0, 200 );
                switch( DMPTR->ProcType ){
                    case FFT_Signal:{
                        ShowWindow( Pltr.GetHWSpect(), 0 );
                        Pltr.SetMaxYLim( MaxValFFT );
                        Pltr.SetMinYLim( 0 );
                        Pltr.TextMx = "";//"Hz ";
                        Pltr.TextMy = NULL;
                        Pltr.SetMaxXLim( F_s/2, FALSE );
                        Pltr.Text2ShowF( " Frecuencia (Hz) " );
                        sprintf( T2T, "Módulo %i, FFT( Transformada rápida de FOURIER )", IMod );
                        Pltr.SetTitle( T2T );
                        Pltr.PLOT_DOUBLE_FFT( DblPtr, BLen/2, 0, TRUE );
                        Pltr.PLOT_DOUBLE_FFT( DblPtr, BLen/2, 0, TRUE );
                    }
                        break;
                    default:
                        Pltr.SetMaxYLim( MaxValFFT );
                        Pltr.SetMinYLim( -MaxValFFT );
                        ShowWindow( Pltr.GetHWSpect(), 0 );
                        /// Pltrs.SetMaxYLim( 1 );Pltrs.SetMinYLim( -1 );
                        Pltr.TextMx = "";
                        Pltr.TextMy = NULL;
                        Pltr.SetMaxXLim( (int)(BLen/F_s), FALSE );
                        Pltr.Text2ShowF( " Tiempo (Seg) " );
                        Pltr.PLOTDOUBLE_BIPOL( DblPtr, BLen, 0, TRUE );
                        break;
                }
                ///   Pltrs.Redraw();
                ///
                ///
                ///char    T2T[128] = {0};sprintf( T2T, "%5i, %5i", WWIN, HWIN );SetWindowTextA( hwnd, T2T );
            }
        }
            break;
        /// ///////////////////////////
        /// ///////////////////////////
        /// ///////////////////////////
        case WM_COMMAND:{
            if( LOWORD(wParam)==IDCANCEL ){
                SendMessageA( hwnd, WM_CLOSE, 0, 0 );
            }
            if( LOWORD(wParam)==0 && HIWORD(wParam)==STN_CLICKED ){
                SendMessageA( hwnd, WM_CLOSE, 0, 0 );
            }
        }
            break;
        /// /////////////////////////////////
        /// /////////////////////////////////
        /// /////////////////////////////////
        case WM_GETMINMAXINFO:{
            MINMAXINFO*      mmiStruct = (MINMAXINFO*)lParam;
            POINT            ptPoint = {0};
            double Dbl2P = 1.2;
            ptPoint.x = (int)round( Dbl2P * 410 );
            ptPoint.y = (int)round( Dbl2P * 280 );
            mmiStruct->ptMinTrackSize = ptPoint;
            ptPoint.x = GetSystemMetrics(SM_CXSCREEN);
            ptPoint.y = GetSystemMetrics(SM_CYSCREEN);
            mmiStruct->ptMaxTrackSize = ptPoint;
        }
            break;
        /// /////////////////////////////////
        /// /////////////////////////////////
        case WM_CLOSE:
            if( DblPtr!=NULL ){
                free( DblPtr );
            }
            DblPtr = NULL;
            Pltr.FreeAll();
            EndDialog( hwnd, 0 );
            break;
        /// /////////////////////////////////
        /// /////////////////////////////////
        /// /////////////////////////////////
        /// /////////////////////////////////
    }
    return FALSE;
}
/// /
/// /
BOOL TLAB::SHOW_PLOT(){
    if( WaitForClosePLOTBIG ){
        return 0;
    }
    WaitForClosePLOTBIG = TRUE;
    while( OnShowPlot ){
        SendMessageA( HW_PLOT_BIG, WM_CLOSE, 0, 0 );
        Sleep(10);
    }
    //Dim1Procs[BSel_1D].SetProc( );
    WaitForClosePLOTBIG = FALSE;
    OnShowPlot = TRUE;
    DialogBoxParamA( GetModuleHandle(NULL), MAKEINTRESOURCE(Dial_SHOWPLOT_GUI), NULL, (DLGPROC)DlgShowPlotBig,
                                 (LPARAM)(&IPtr) );
    OnShowPlot = FALSE;
    return TRUE;
}
/// /////////////////////////////////////////////////////////////////////////////////////////////////////
/// /////////////////////////////////////////////////////////////////////////////////////////////////////
/// /////////////////////////////////////////////////////////////////////////////////////////////////////
/// /////////////////////////////////////////////////////////////////////////////////////////////////////
/// /////////////////////////////////////////////////////////////////////////////////////////////////////
/// /////////////////////////////////////////////////////////////////////////////////////////////////////
DWORD WINAPI ThreadCall_SPLOT( LPVOID pv ){
    TLAB* TLBPTR = (TLAB*)pv;
    TLBPTR->SHOW_PLOT();
    return 0;
}
/// /////////////////////////////////////////////////////////////////////////////////////////////////////
/// /////////////////////////////////////////////////////////////////////////////////////////////////////
/// /////////////////////////////////////////////////////////////////////////////////////////////////////
/// /////////////////////////////////////////////////////////////////////////////////////////////////////
/// /////////////////////////////////////////////////////////////////////////////////////////////////////
/// /////////////////////////////////////////////////////////////////////////////////////////////////////
