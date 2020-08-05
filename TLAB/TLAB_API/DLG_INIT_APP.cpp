BOOL DlgProgress12D_INIT( HWND hwnd, UINT  message, WPARAM wParam, LPARAM lParam ){
    static HICON HIc = NULL;
    switch(message){
        case WM_INITDIALOG:{
            HWPGRGEN_MAIN = hwnd;
            HWPGRGEN      = GetDlgItem( hwnd, 100 );
            SendMessage( GetDlgItem( hwnd, 100 ), PBM_SETRANGE, 0, MAKELPARAM(0, 100) );
            SetWindowTextA( HWPGRGEN_MAIN, " Iniciando TLAB " );
            char T2T2[1024] = {0};
            LoadString( GetModuleHandleA(NULL), ID_TEXT_ABOUT, T2T2, 1023 );
            SetWindowTextA( GetDlgItem( hwnd, 20 ), T2T2 );
            ///
            ///
            ///
            ///
            HIc = LoadIcon( GetModuleHandleA(NULL), MAKEINTRESOURCEA(TLAB_ICON) );
            SendMessage( GetDlgItem( hwnd, 123 ), STM_SETICON, (WPARAM)HIc, 0 );
            SendMessage( GetDlgItem( hwnd, 123 ), STM_SETIMAGE, IMAGE_ICON, (LPARAM)HIc );
            ///
            ///
            ///
            ///
        }
            break;
        case WM_DESTROY:
        case WM_CLOSE:
            DeleteObject(HIc);
            /// EnableWindow( GetParent(hwnd), TRUE );
            EndDialog( hwnd, 0 );
            break;
    }
    return FALSE;
}
/// ////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// ////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// ////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// ////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// ////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL DlgAbout_TLAB( HWND hwnd, UINT  message, WPARAM wParam, LPARAM lParam ){
    static HICON HIc = NULL;
    switch(message){
        case WM_INITDIALOG:{
            ///
            HIc = LoadIcon( GetModuleHandleA(NULL), MAKEINTRESOURCEA(TLAB_ICON) );
            SendMessage( GetDlgItem( hwnd, 123 ), STM_SETICON, (WPARAM)HIc, 0 );
            SendMessage( GetDlgItem( hwnd, 123 ), STM_SETIMAGE, IMAGE_ICON, (LPARAM)HIc );
            ///
            ///
            SetWindowTextA( hwnd, " Acerca de TLAB " );
            char T2T2[1024] = {0};
            LoadString( GetModuleHandleA(NULL), ID_TEXT_ABOUT, T2T2, 1023 );
            SetWindowTextA( GetDlgItem( hwnd, 20 ), T2T2 );
            SetFocus( GetDlgItem( hwnd, IDYES ) );
            /// ShowWindow( GetDlgItem( hwnd, 20 ), 0 );
        }
            break;
        case WM_COMMAND:
            if( LOWORD(wParam)==IDYES || LOWORD(wParam)==IDCANCEL ){
                DeleteObject(HIc);
                EndDialog( hwnd, 0 );
            }
            break;
        case WM_DESTROY:
        case WM_CLOSE:
            DeleteObject(HIc);
            /// EnableWindow( GetParent(hwnd), TRUE );
            EndDialog( hwnd, 0 );
            break;
    }
    return FALSE;
}



/// ////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// ////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// ////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// ////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// ////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL INIT_PROGRESS(){
    ///
    /// TLAB_STR.MsgBoxF( " " );
    ///
    return TRUE;
}
