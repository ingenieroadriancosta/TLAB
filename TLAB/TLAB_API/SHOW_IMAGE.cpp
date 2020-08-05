BOOL CALLBACK DlgShowPicBig( HWND hwnd, UINT  message, WPARAM wParam, LPARAM lParam ){
    static HWND*    HWPAR = NULL;
    static DIM2*    DMPTR = NULL;
    static HWND     HWPIC = NULL;
    static HBITMAP  HBMP = NULL;
    ///
    static int WP_OR = 0;
    static int HP_OR = 0;
    ///
    static int WPO = 0;
    static int HPO = 0;
    static int WWIN = 0;
    static int HWIN = 0;
    static int WWIN_I = 0;
    static int HWIN_I = 0;
    static int XPWIN = 120;
    static int YPWIN = 60;
    static RGBQUAD* RgbIn = NULL;
    static BOOL     isMaximized = FALSE;
    static BOOL     isShow = FALSE;
    const int GTHS = 0;
    switch( message ){
        case WM_INITDIALOG:{
            isShow = FALSE;
            TLAB* TLBPTR = (TLAB*)lParam;
            DMPTR = &(TLBPTR->Dim2Procs[TLBPTR->BSel_2D]);
            HWPAR = &TLBPTR->HW_PIC_BIG;
            HWPAR[0] = hwnd;
            ///
            ///
            WWIN = WWIN_I;
            HWIN = HWIN_I;
            ///
            if( WWIN==0 || HWIN==0 ){
                WWIN = 640;
                HWIN = 480;
            }
            ///
            HWPIC = GetDlgItem( hwnd, 128 );
            ///
            SendMessage( hwnd , WM_SETICON , ICON_SMALL , (LPARAM)LoadIconA( GetModuleHandleA(NULL), MAKEINTRESOURCEA(TLAB_ICON) ) );
            SendMessage( hwnd , WM_SETICON , ICON_BIG , (LPARAM)LoadIconA( GetModuleHandleA(NULL) , MAKEINTRESOURCEA(TLAB_ICON) ) );
            ///
            WP_OR = DMPTR->WPIC_OUT;
            HP_OR = DMPTR->HPIC_OUT;
            ///
            WPO = WWIN;
            HPO = ( HP_OR * WPO )/WP_OR;
            if( HPO>HWIN ){
                HPO = HWIN;
                WPO = ( WP_OR * HPO )/HP_OR;
            }
            RgbIn = (RGBQUAD*)malloc( DMPTR->NBytesBuff + 1024*1024 );
            DMPTR->ReadDatas( RgbIn );
            HBITMAP HBMT = DMPTR->NewBitmap( WP_OR, HP_OR );
            SetBitmapBits( HBMT, 4 * WP_OR * HP_OR, RgbIn );
            HBMP = DMPTR->Redim_PIC( HBMT, WPO, HPO );
            DeleteObject( HBMT );
            ///
            if( DMPTR->IsBin ){
                RGBQUAD* RgbQ = (RGBQUAD*)malloc( 4 * WPO * HPO );
                GetBitmapBits( HBMP, 4 * WPO * HPO, RgbQ );
                for( int i=0; i<(WPO * HPO); i++ ){
                    RgbQ[i].rgbRed   =
                    RgbQ[i].rgbGreen =
                    RgbQ[i].rgbBlue  = 255*(RgbQ[i].rgbRed>GTHS);
                }
                SetBitmapBits( HBMP, 4 * WPO * HPO, RgbQ );
                free( RgbQ );
            }
            ///
            SendMessage( HWPIC, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)HBMP );
            ///
            MoveWindow( hwnd, XPWIN-8, YPWIN-30, WWIN+16, HWIN+39, TRUE );
            //WindowSize( hwnd, &W_WIN, &H_WIN );
            //MoveWindow( HWPIC, 0, 0, W_WIN, H_WIN, TRUE );
            ///
            ShowWindow( hwnd, 1 );
            // CenterWindow( hwnd,)
            ///
        }
            break;
        /// ///////////////////////////
        /// ///////////////////////////
        /// ///////////////////////////
        case WM_SHOWWINDOW:{
            if( !isShow ){
                WindowSize( hwnd, &WWIN, &HWIN );
                WPO = WWIN;
                HPO = ( HP_OR * WPO )/WP_OR;
                if( HPO>HWIN ){
                    HPO = HWIN;
                    WPO = ( WP_OR * HPO )/HP_OR;
                }
                MoveWindow( HWPIC, 0, 0, WWIN, HWIN, TRUE );
                DeleteObject( HBMP );
                HBITMAP HBMT = DMPTR->NewBitmap( WP_OR, HP_OR );
                SetBitmapBits( HBMT, 4 * WP_OR * HP_OR, RgbIn );
                HBMP = DMPTR->Redim_PIC( HBMT, WPO, HPO );
                DeleteObject( HBMT );
                ///
                if( DMPTR->IsBin ){
                    RGBQUAD* RgbQ = (RGBQUAD*)malloc( 4 * WPO * HPO );
                    GetBitmapBits( HBMP, 4 * WPO * HPO, RgbQ );
                    for( int i=0; i<(WPO * HPO); i++ ){
                        RgbQ[i].rgbRed   =
                        RgbQ[i].rgbGreen =
                        RgbQ[i].rgbBlue  = 255*(RgbQ[i].rgbRed>GTHS);
                    }
                    SetBitmapBits( HBMP, 4 * WPO * HPO, RgbQ );
                    free( RgbQ );
                }
                ///
                ///
                SendMessage( HWPIC, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)HBMP );
                isShow = TRUE;
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
        }
            break;
        /// /////////////////////////////////
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
            //break;
        case WM_EXITSIZEMOVE:{
            if( wParam!=SIZE_MAXIMIZED && wParam!=SIZE_MINIMIZED ){
                WindowPos( hwnd, &XPWIN, &YPWIN, FALSE );
            }
            int W_WIN = 0;
            int H_WIN = 0;
            WindowSize( hwnd, &W_WIN, &H_WIN );
            if( WWIN!=W_WIN || HWIN!=H_WIN ){
                WWIN = W_WIN;
                HWIN = H_WIN;
                ///
                ///
                if( wParam!=SIZE_MAXIMIZED ){
                    WindowSize( hwnd, &WWIN_I, &HWIN_I );
                }
                ///
                ///
                WPO = WWIN;
                HPO = ( HP_OR * WPO )/WP_OR;
                if( HPO>HWIN ){
                    HPO = HWIN;
                    WPO = ( WP_OR * HPO )/HP_OR;
                }
                MoveWindow( HWPIC, 0, 0, WWIN, HWIN, TRUE );
                DeleteObject( HBMP );
                HBITMAP HBMT = DMPTR->NewBitmap( WP_OR, HP_OR );
                SetBitmapBits( HBMT, 4 * WP_OR * HP_OR, RgbIn );
                HBMP = DMPTR->Redim_PIC( HBMT, WPO, HPO );
                DeleteObject( HBMT );
                ///
                ///
                if( DMPTR->IsBin ){
                    RGBQUAD* RgbQ = (RGBQUAD*)malloc( 4 * WPO * HPO );
                    GetBitmapBits( HBMP, 4 * WPO * HPO, RgbQ );
                    for( int i=0; i<(WPO * HPO); i++ ){
                        RgbQ[i].rgbRed   =
                        RgbQ[i].rgbGreen =
                        RgbQ[i].rgbBlue  = 255*(RgbQ[i].rgbRed>GTHS);
                    }
                    SetBitmapBits( HBMP, 4 * WPO * HPO, RgbQ );
                    free( RgbQ );
                }
                ///
                ///
                SendMessage( HWPIC, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)HBMP );
                ///
                ///char    T2T[128] = {0};sprintf( T2T, "%5i, %5i", WWIN, HWIN );SetWindowTextA( hwnd, T2T );
            }
        }
            break;
        /// /////////////////////////////////
        /// /////////////////////////////////
        /// /////////////////////////////////
        /// /////////////////////////////////
        case WM_LBUTTONDBLCLK:
            if( ISDOWNKEY(VK_CONTROL) ){
                char T2T[256] = {0};
                sprintf( T2T, "%s00lksfdksdf.jpg", DSP_MNG.GetDSPPath() );
                SAVE_AS_JPG( T2T, WP_OR, HP_OR, RgbIn );
                ShellExecuteA( NULL, "open", T2T, NULL, NULL, 3 );
            }
            break;
        /// /////////////////////////////////
        /// /////////////////////////////////
        /// /////////////////////////////////
        /// /////////////////////////////////
        /// /////////////////////////////////
        /// /////////////////////////////////
        case WM_MOUSEMOVE:{
            char    T2T[128] = {0};
            int     x_P = 0;
            int     y_P = 0;
            int     fwKeys = wParam; // banderas de tecla
            int     xPos = LOWORD(lParam); // posición horizontal del cursor
            int     yPos = HIWORD(lParam); // posición vertical del cursor
            BYTE    R = 0;
            BYTE    G = 0;
            BYTE    B = 0;
            if( fwKeys==MK_LBUTTON ){
                if( WWIN==WPO ){
                    x_P = (int)round( (double)((xPos * WP_OR))/((double)WWIN) );
                    y_P = (int)round( ((yPos-((double)HWIN)/2+((double)HPO)/2) * HP_OR )/((double)HPO) );
                }else{
                    y_P = (int)round( (yPos * (double)HP_OR )/((double)HWIN) );
                    x_P = (int)round( ((xPos-((double)WWIN)/2+((double)WPO)/2) * WP_OR )/((double)WPO) );
                }
                int W_WIN = 0;
                int H_WIN = 0;
                WindowSize( hwnd, &W_WIN, &H_WIN );
                if( xPos>=W_WIN-1 ){
                    x_P = WP_OR - 1;
                }
                if( yPos>=H_WIN-1 ){
                    y_P = HP_OR - 1;
                }
                if( x_P>=WP_OR ){
                    x_P = WP_OR - 1;
                }
                if( y_P>=HP_OR ){
                    y_P = HP_OR - 1;
                }
                if( x_P<0 ){
                    x_P = 0;
                }
                if( y_P<0 ){
                    y_P = 0;
                }
                if( !(x_P<0 || y_P<0 ) ){
                    R = RgbIn[ WP_OR * y_P + x_P ].rgbRed;
                    G = RgbIn[ WP_OR * y_P + x_P ].rgbGreen;
                    B = RgbIn[ WP_OR * y_P + x_P ].rgbBlue;
                    if( DMPTR->IsBin ){
                        sprintf( T2T, "Pixel(%5i, %5i)  %3i", x_P, y_P, R/255 );
                    }else{
                        if( DMPTR->IsGray ){
                            sprintf( T2T, "Pixel(%5i, %5i)  %3i", x_P, y_P, R );
                        }else{
                            sprintf( T2T, "Pixel(%5i, %5i)  RGB[%3i  %3i  %3i]", x_P, y_P, R, G, B );
                        }
                    }
                    SetWindowTextA( hwnd, T2T );
                }
            }
        }
            break;
        /// /////////////////////////////////
        /// /////////////////////////////////
        case WM_GETMINMAXINFO:{
            MINMAXINFO*      mmiStruct = (MINMAXINFO*)lParam;
            POINT            ptPoint;
            ptPoint.x = 410;
            ptPoint.y = 240;
            mmiStruct->ptMinTrackSize = ptPoint;
            ptPoint.x = GetSystemMetrics(SM_CXSCREEN);
            ptPoint.y = GetSystemMetrics(SM_CYSCREEN);
            mmiStruct->ptMaxTrackSize = ptPoint;
        }
            break;
        /// /////////////////////////////////
        /// /////////////////////////////////
        case WM_CLOSE:
            if( RgbIn!=NULL ){
                free( RgbIn );
            }
            RgbIn = NULL;
            DeleteObject( HBMP );
            HWPAR[0] = NULL;
            EndDialog( hwnd, 0 );
            break;
    }
    return FALSE;
}
/// /
BOOL TLAB::SHOW_IMAGE(){
    OnShowPic = TRUE;
    DialogBoxParamA( GetModuleHandle(NULL), MAKEINTRESOURCE(Dial_SHOWPIC_GUI), NULL, (DLGPROC)DlgShowPicBig,
                                 (LPARAM)(&IPtr) );
    OnShowPic = FALSE;
    return TRUE;
}
/// /////////////////////////////////////////////////////////////////////////////////////////////////////
/// /////////////////////////////////////////////////////////////////////////////////////////////////////
/// /////////////////////////////////////////////////////////////////////////////////////////////////////
/// /////////////////////////////////////////////////////////////////////////////////////////////////////
/// /////////////////////////////////////////////////////////////////////////////////////////////////////
/// /////////////////////////////////////////////////////////////////////////////////////////////////////
DWORD WINAPI ThreadCall_SHPIC( LPVOID pv ){
    TLAB* TLBPTR = (TLAB*)pv;
    if( TLBPTR->WaitForClosePICBIG ){
        return 0;
    }
    TLBPTR->WaitForClosePICBIG = TRUE;
    while( TLBPTR->OnShowPic ){
        SendMessageA( TLBPTR->HW_PIC_BIG, WM_CLOSE, 0, 0 );
        Sleep(10);
    }
    TLBPTR->WaitForClosePICBIG = FALSE;
    TLBPTR->SHOW_IMAGE();
    return 0;
}
/// /////////////////////////////////////////////////////////////////////////////////////////////////////
/// /////////////////////////////////////////////////////////////////////////////////////////////////////
/// /////////////////////////////////////////////////////////////////////////////////////////////////////
/// /////////////////////////////////////////////////////////////////////////////////////////////////////
/// /////////////////////////////////////////////////////////////////////////////////////////////////////
/// /////////////////////////////////////////////////////////////////////////////////////////////////////
