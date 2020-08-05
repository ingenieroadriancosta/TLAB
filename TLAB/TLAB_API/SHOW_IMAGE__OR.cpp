BOOL CALLBACK DlgShowPicBig( HWND hwnd, UINT  message, WPARAM wParam, LPARAM lParam ){
    static HWND*    HWPAR = NULL;
    static DIM2*    DMPTR = NULL;
    static HWND     HWPIC = NULL;
    static HBITMAP  HBMP = NULL;
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
            WPO = WWIN;
            HPO = ( DMPTR->HPIC_OUT * WPO )/DMPTR->WPIC_OUT;
            if( HPO>HWIN ){
                HPO = HWIN;
                WPO = ( DMPTR->WPIC_OUT * HPO )/DMPTR->HPIC_OUT;
            }
            RgbIn = (RGBQUAD*)malloc( DMPTR->NBytesBuff + 1024*1024 );
            DMPTR->ReadDatas( RgbIn );
            HBITMAP HBMT = DMPTR->NewBitmap( DMPTR->WPIC_OUT, DMPTR->HPIC_OUT );
            SetBitmapBits( HBMT, 4 * DMPTR->WPIC_OUT * DMPTR->HPIC_OUT, RgbIn );
            HBMP = DMPTR->Redim_PIC( HBMT, WPO, HPO );
            DeleteObject( HBMT );
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
                HPO = ( DMPTR->HPIC_OUT * WPO )/DMPTR->WPIC_OUT;
                if( HPO>HWIN ){
                    HPO = HWIN;
                    WPO = ( DMPTR->WPIC_OUT * HPO )/DMPTR->HPIC_OUT;
                }
                MoveWindow( HWPIC, 0, 0, WWIN, HWIN, TRUE );
                DeleteObject( HBMP );
                HBITMAP HBMT = DMPTR->NewBitmap( DMPTR->WPIC_OUT, DMPTR->HPIC_OUT );
                SetBitmapBits( HBMT, 4 * DMPTR->WPIC_OUT * DMPTR->HPIC_OUT, RgbIn );
                HBMP = DMPTR->Redim_PIC( HBMT, WPO, HPO );
                DeleteObject( HBMT );
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
                HPO = ( DMPTR->HPIC_OUT * WPO )/DMPTR->WPIC_OUT;
                if( HPO>HWIN ){
                    HPO = HWIN;
                    WPO = ( DMPTR->WPIC_OUT * HPO )/DMPTR->HPIC_OUT;
                }
                MoveWindow( HWPIC, 0, 0, WWIN, HWIN, TRUE );
                DeleteObject( HBMP );
                HBITMAP HBMT = DMPTR->NewBitmap( DMPTR->WPIC_OUT, DMPTR->HPIC_OUT );
                SetBitmapBits( HBMT, 4 * DMPTR->WPIC_OUT * DMPTR->HPIC_OUT, RgbIn );
                HBMP = DMPTR->Redim_PIC( HBMT, WPO, HPO );
                DeleteObject( HBMT );
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
                SAVE_AS_JPG( T2T, DMPTR->WPIC_OUT, DMPTR->HPIC_IN, RgbIn );
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
                    x_P = (int)round( (double)((xPos * DMPTR->WPIC_OUT))/((double)WWIN) );
                    y_P = (int)round( ((yPos-((double)HWIN)/2+((double)HPO)/2) * DMPTR->HPIC_OUT )/((double)HPO) );
                }else{
                    y_P = (int)round( (yPos * (double)DMPTR->HPIC_OUT )/((double)HWIN) );
                    x_P = (int)round( ((xPos-((double)WWIN)/2+((double)WPO)/2) * DMPTR->WPIC_OUT )/((double)WPO) );
                }
                int W_WIN = 0;
                int H_WIN = 0;
                WindowSize( hwnd, &W_WIN, &H_WIN );
                if( xPos>=W_WIN-1 ){
                    x_P = DMPTR->WPIC_OUT - 1;
                }
                if( yPos>=H_WIN-1 ){
                    y_P = DMPTR->HPIC_OUT - 1;
                }
                if( x_P>=DMPTR->WPIC_OUT ){
                    x_P = DMPTR->WPIC_OUT - 1;
                }
                if( y_P>=DMPTR->HPIC_OUT ){
                    y_P = DMPTR->HPIC_OUT - 1;
                }
                if( x_P<0 ){
                    x_P = 0;
                }
                if( y_P<0 ){
                    y_P = 0;
                }
                if( !(x_P<0 || y_P<0 ) ){
                    R = RgbIn[ DMPTR->WPIC_OUT * y_P + x_P ].rgbRed;
                    G = RgbIn[ DMPTR->WPIC_OUT * y_P + x_P ].rgbGreen;
                    B = RgbIn[ DMPTR->WPIC_OUT * y_P + x_P ].rgbBlue;
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
