BOOL ParseEditDouble( WPARAM wParam, LPARAM lParam ){
    char T2T[32] = {0};
    char T2T2[32] = {0};
    BOOL IsPSet = FALSE;
    GetWindowTextA( (HWND)lParam, T2T, 16 );
    wParam = T2T[strlen(T2T)-1];
    int SLen = strlen(T2T);
    int IndT = 0;
    BOOL SetTT = FALSE;
    int indsel = strlen(T2T);
    for( int i=0; i<SLen; i++ ){
        wParam = T2T[i];
        if( !((wParam >= '0' && wParam <= '9') || wParam == '.' ///|| wParam == VK_RETURN || wParam == VK_DELETE
                ///|| wParam == VK_BACK
                ) ){
            SetTT = TRUE;
            indsel = i;
        }else{
            if( IsPSet && T2T[i]=='.' ){
                SetTT = TRUE;
                indsel = i;
                continue;
            }else{
                if( T2T[i]=='.' ){
                    IsPSet = TRUE;
                }
                T2T2[IndT] = T2T[i];
                IndT++;
                T2T2[IndT] = 0;
            }
        }
    }
    if( SetTT ){
        SetWindowTextA( (HWND)lParam, T2T2 );
        SendMessage( (HWND)lParam, (UINT)EM_SETSEL, indsel, indsel );
    }
    return TRUE;
}
/// ///////////////////////////////////////////////////////////////////////////////////////////////////
/// ///////////////////////////////////////////////////////////////////////////////////////////////////
/// ///////////////////////////////////////////////////////////////////////////////////////////////////
/// ///////////////////////////////////////////////////////////////////////////////////////////////////
BOOL ParseEditInt( WPARAM wParam, LPARAM lParam ){
    char T2T[32] = {0};
    char T2T2[32] = {0};
    BOOL IsPSet = FALSE;
    GetWindowTextA( (HWND)lParam, T2T, 16 );
    wParam = T2T[strlen(T2T)-1];
    int SLen = strlen(T2T);
    int IndT = 0;
    BOOL SetTT = FALSE;
    int indsel = strlen(T2T);
    for( int i=0; i<SLen; i++ ){
        wParam = T2T[i];
        if( !((wParam >= '0' && wParam <= '9') || wParam == '-' ///|| wParam == VK_RETURN || wParam == VK_DELETE
                ///|| wParam == VK_BACK
                ) ){
            SetTT = TRUE;
            indsel = i;
        }else{
            if( IsPSet && T2T[i]=='-' ){
                SetTT = TRUE;
                indsel = i;
                continue;
            }else{
                if( T2T[i]=='-' ){
                    IsPSet = TRUE;
                }
                T2T2[IndT] = T2T[i];
                IndT++;
                T2T2[IndT] = 0;
            }
        }
    }
    if( SetTT ){
        SetWindowTextA( (HWND)lParam, T2T2 );
        SendMessage( (HWND)lParam, (UINT)EM_SETSEL, indsel, indsel );
    }
    return TRUE;
}
/// ///////////////////////////////////////////////////////////////////////////////////////////////////
/// ///////////////////////////////////////////////////////////////////////////////////////////////////
/// ///////////////////////////////////////////////////////////////////////////////////////////////////
/// ///////////////////////////////////////////////////////////////////////////////////////////////////
BOOL SetToolTip( HWND HWCTRL, LPCSTR LPTT, HWND* HWndT ){
    // if( HWndT==NULL ){ return FALSE; }
    HWND HWTT = NULL;
    RECT Rect = {0};
    TOOLINFO           TInfo = {0};
    HWTT = CreateWindowEx( WS_EX_TOPMOST, TOOLTIPS_CLASS, NULL,
                                   WS_POPUP | TTS_NOPREFIX | TTS_ALWAYSTIP,
                                   0, 0, 0, 0, HWCTRL, NULL, NULL, NULL );
    SetWindowPos( HWTT, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);
    GetClientRect ( HWCTRL, &Rect );
    TInfo.cbSize = sizeof(TOOLINFO);
    TInfo.uFlags = TTF_SUBCLASS;
    TInfo.hwnd = HWCTRL;
    TInfo.hinst = GetModuleHandleA(NULL);
    TInfo.uId = 0;
    TInfo.lpszText = (LPSTR)LPTT;
    TInfo.rect.left = Rect.left;
    TInfo.rect.top = Rect.top;
    TInfo.rect.right = Rect.right;
    TInfo.rect.bottom = Rect.bottom;
    SendMessage( HWTT, TTM_ADDTOOL, 0, (LPARAM) (LPTOOLINFO) &TInfo );
    if( HWndT!=NULL ){
        HWndT[0] = HWTT;
    }
    return TRUE;
}
/// ///////////////////////////////////////////////////////////////////////////////////////////////////
/// ///////////////////////////////////////////////////////////////////////////////////////////////////
BOOL Ch2WCh( LPWCH LPW, LPCSTR LPS, int IMax ){
    for( int i=0; i<IMax; i++ ){
        LPW[i] = LPS[i];
    }
    LPW[IMax] = 0;
    return TRUE;
}
/// ///////////////////////////////////////////////////////////////////////////////////////////////////
/// ///////////////////////////////////////////////////////////////////////////////////////////////////
BOOL SetToolTipW( HWND HWCTRL, LPCWSTR LPTT, HWND* HWndT ){
    if( HWndT==NULL ){ return TRUE; }
    RECT Rect = {0};
    TOOLINFOW           TInfo = {0};
    DestroyWindow( HWndT[0] );
    HWndT[0] = CreateWindowExW( WS_EX_TOPMOST, TOOLTIPS_CLASSW, NULL,
                                   WS_POPUP | TTS_NOPREFIX | TTS_ALWAYSTIP,
                                   0, 0, 0, 0, HWCTRL, NULL, NULL, NULL );
    SetWindowPos( HWndT[0], HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);
    GetClientRect ( HWCTRL, &Rect );
    TInfo.cbSize = sizeof(TOOLINFOW);
    TInfo.uFlags = TTF_SUBCLASS;
    TInfo.hwnd = HWCTRL;
    TInfo.hinst = NULL;
    TInfo.uId = 0;
    TInfo.hinst = GetModuleHandleA( NULL );
    TInfo.lpszText = (LPWSTR)LPTT;
    TInfo.rect.left = Rect.left;
    TInfo.rect.top = Rect.top;
    TInfo.rect.right = Rect.right;
    TInfo.rect.bottom = Rect.bottom;
    SendMessageW( HWndT[0], TTM_ADDTOOLW, 0, (LPARAM) (LPTOOLINFOW) &TInfo );
    return TRUE;
}
/// ////////////////////////////////////////////////////////////////////////////////
/// ////////////////////////////////////////////////////////////////////////////////
/// ////////////////////////////////////////////////////////////////////////////////
/// ////////////////////////////////////////////////////////////////////////////////
inline int ISDOWNKEY( int Key = VK_SPACE ){ return int(GetKeyState( Key )& 0x80); }
/// ////////////////////////////////////////////////////////////////////////////////
/// ////////////////////////////////////////////////////////////////////////////////
/// ////////////////////////////////////////////////////////////////////////////////
/// ////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
int Copy2ClipBoard2( char* Texto ){
    size_t len = strlen( Texto );
    HGLOBAL hMem =  GlobalAlloc(GMEM_MOVEABLE, len);
    memcpy( GlobalLock(hMem), Texto, len );
    GlobalUnlock(hMem);
    OpenClipboard(0);
    EmptyClipboard();
    SetClipboardData(CF_TEXT, hMem);
    CloseClipboard();
    return 0;
}
/// ////////////////////////////////////////////////////////////////////////////////
/// ////////////////////////////////////////////////////////////////////////////////
/// ////////////////////////////////////////////////////////////////////////////////
/// ////////////////////////////////////////////////////////////////////////////////
BYTE ParceBYTE( double DVL ){
    if( DVL>255 ){
        return 255;
    }
    return ( (BYTE)DVL );
}




/// ////////////////////////////////////////////////////////////////////////////////
/// ////////////////////////////////////////////////////////////////////////////////
/// ////////////////////////////////////////////////////////////////////////////////
/// ////////////////////////////////////////////////////////////////////////////////

/// ///////////////////////////////////////////////////////////////////////////////////////////
/// ///////////////////////////////////////////////////////////////////////////////////////////
/// ///////////////////////////////////////////////////////////////////////////////////////////
/// ///////////////////////////////////////////////////////////////////////////////////////////
/// ///////////////////////////////////////////////////////////////////////////////////////////
/// ///////////////////////////////////////////////////////////////////////////////////////////
BOOL* IsTopGlobal = NULL;
BOOL CenterWindow( HWND HW_W, int XCorri, int YCorri, RECT* RecOut=NULL ){
    const int CXSCREEN2 = GetSystemMetrics( SM_CXSCREEN );
    const int CYSCREEN2 = GetSystemMetrics( SM_CYSCREEN );
    RECT re = {0};
    memset( &re, 0, sizeof(RECT) );
    GetClientRect( HW_W, &re );
    ///
    const int WDD = re.right;
    const int HDD = re.bottom;
    const int bX = (WDD+CXSCREEN2)/2 - WDD - XCorri;
    const int bY = (HDD+CYSCREEN2)/2 - HDD - YCorri;
    ///
    //HWND HWFTOP = GetTopWindow( GetAncestor(HW_W, GA_ROOT) );
    //if( HWFTOP ){
    if( IsTopGlobal[0] ){
        SetWindowPos( HW_W, HWND_TOPMOST, bX, bY, WDD, HDD, 1 );
    }else{
        SetWindowPos( HW_W, HWND_TOP, bX, bY, WDD, HDD, 1 );
    }
    ///
    /*
    if( GetWindowLongA( GetAncestor(HW_W, GA_ROOT), GWL_EXSTYLE )==WS_EX_TOPMOST ){
        SetWindowPos( HW_W, HWND_BOTTOM, bX, bY, WDD, HDD, 1 );
    }else{
        SetWindowPos( HW_W, HWND_BOTTOM, bX, bY, WDD, HDD, 1 );
    }
    //*/
    if( RecOut!=NULL ){
        memcpy( RecOut, &re, sizeof(RECT) );
    }
    return TRUE;
}
/// //////////////////////////////////////////////////////////////////////////////////////////////////////
/// //////////////////////////////////////////////////////////////////////////////////////////////////////
/// //////////////////////////////////////////////////////////////////////////////////////////////////////
/// //////////////////////////////////////////////////////////////////////////////////////////////////////
/// //////////////////////////////////////////////////////////////////////////////////////////////////////
inline void WindowSize(HWND hwnd, int* w , int* h ){
    RECT re;
    memset( &re, 0, sizeof(RECT) );
    GetClientRect(hwnd, &re);
    w[0] = re.right;
    h[0] = re.bottom;
}
/// //////////////////////////////////////////////////////////////////////////////////////////////////////
/// //////////////////////////////////////////////////////////////////////////////////////////////////////
/// //////////////////////////////////////////////////////////////////////////////////////////////////////
/// //////////////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int WINAPI SaveFileAs( char *FileName, LPCSTR LPFilter, HWND HWFath ){
    OPENFILENAME ofn;
    ZeroMemory( &ofn, sizeof(OPENFILENAME) );
    ofn.lStructSize = sizeof(OPENFILENAME);
    ofn.hwndOwner = HWFath;
    ofn.nFilterIndex = 0;
    ofn.lpstrFileTitle = NULL;
    /// ofn.lpstrInitialDir= "C:";
    if( LPFilter==NULL ){
        ofn.lpstrFilter = "BMP Files (*.bmp*)\0*.bmp\0"
                          "JPG Files (*.jpg*)\0*.jpg\0";

    }else{
        ofn.lpstrFilter = LPFilter;
    }
    ofn.lpstrFile = FileName;
    ofn.nMaxFile = MAX_PATH;
    ofn.Flags = OFN_EXPLORER | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT;
    ofn.lpstrDefExt = "";
    if( !GetSaveFileNameA(&ofn) ){
        return FALSE;
    }
    return TRUE;
}
/// //////////////////////////////////////////////////////////////////////////////////////////////////////
/// //////////////////////////////////////////////////////////////////////////////////////////////////////
/// //////////////////////////////////////////////////////////////////////////////////////////////////////
/// //////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL OpenFileAs( char *FileName, LPCSTR LPFilter, HWND HWFath ){
    OPENFILENAME  openFileDialogfunction = {0};
    ZeroMemory(&openFileDialogfunction, sizeof(openFileDialogfunction));
    openFileDialogfunction.lStructSize= sizeof(openFileDialogfunction);
    openFileDialogfunction.hwndOwner = HWFath;
    openFileDialogfunction.nFilterIndex=0;
    openFileDialogfunction.lpstrTitle="Abrir";
    openFileDialogfunction.hInstance = GetModuleHandleA( NULL );
    if( LPFilter!=NULL ){
        openFileDialogfunction.lpstrFilter = LPFilter;
    }else{
        openFileDialogfunction.lpstrFilter = "All images (*.png, *.jpg, *.jpeg, *.gif, *.bmp)\0" "*.png;*.jpg;*.jpeg;*.gif;*.bmp\0"
                                                "BMP Files (*.bmp*)\0*.bmp\0"
                                                "JPG Files (*.jpg, *.jpeg)\0*.jpg;*.jpeg\0"
                                                "PNG Files (*.png*)\0*.png\0"
                                                "GIF Files (*.gif*)\0*.gif\0"
                                                "All Files(*.*)\0*.*\0\0";
    }
    openFileDialogfunction.lpstrFile = FileName;
    ///
    char IDIR[1024] = {0};
    sprintf( IDIR, "%s\\TLAB\\", getenv( "USERPROFILE" ) );
    openFileDialogfunction.lpstrInitialDir = IDIR;
    ///
    openFileDialogfunction.nMaxFile = 8*MAX_PATH;
    openFileDialogfunction.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST;
    openFileDialogfunction.lpstrDefExt = "";
    if( !GetOpenFileName(&openFileDialogfunction) ){
       return FALSE;
    }
    return TRUE;
}
/// //////////////////////////////////////////////////////////////////////////////////////////////////////
/// //////////////////////////////////////////////////////////////////////////////////////////////////////
/// //////////////////////////////////////////////////////////////////////////////////////////////////////
/// //////////////////////////////////////////////////////////////////////////////////////////////////////
/// //////////////////////////////////////////////////////////////////////////////////////////////////////
void Text2Edit( char* TextEditOut , char* TextEditIn )
{
TextEditOut[0] = '\0';
int indiceInit = 0;
int MaxLenStr = strlen( TextEditIn );
TextEditOut[0] = 0;
for( int i=0; i<MaxLenStr; i++ ){
    if( TextEditIn[i]=='\n' ){
       TextEditOut[indiceInit] = 13;
       indiceInit++;
       TextEditOut[indiceInit] = '\n';
    }else{
       TextEditOut[indiceInit] = TextEditIn[i];
    }
    indiceInit++;
}
TextEditOut[indiceInit] = '\0';
TextEditOut[indiceInit+1] = '\0';
}
//
//
void Text2Edit( HWND ToHWnd , char* TextEditIn ){
int LenStr = strlen( TextEditIn );
char* C2Str = (char*)malloc( 4*LenStr );
ZeroMemory( C2Str, 4*LenStr - 1 );
Text2Edit( &C2Str[0], &TextEditIn[0] );
SetWindowText( ToHWnd, C2Str );
free( C2Str );
}
/// //////////////////////////////////////////////////////////////////////////////////////////////////////
/// //////////////////////////////////////////////////////////////////////////////////////////////////////
/// //////////////////////////////////////////////////////////////////////////////////////////////////////
/// //////////////////////////////////////////////////////////////////////////////////////////////////////
/// //////////////////////////////////////////////////////////////////////////////////////////////////////
/// ///////////////////////////////////////////////////////
static inline BYTE ParseByte( int I_B ){
    if( I_B>255 ){
        return 255;
    }
    if( I_B<0 ){
        return 0;
    }
    return I_B;
}
/// //////////////////////////////////////////////////////////////////////////////////////////////////////
/// //////////////////////////////////////////////////////////////////////////////////////////////////////
/// //////////////////////////////////////////////////////////////////////////////////////////////////////
/// //////////////////////////////////////////////////////////////////////////////////////////////////////
/// //////////////////////////////////////////////////////////////////////////////////////////////////////
/// ///////////////////////////////////////////////////////
static inline short ParseShort( double I_B ){
    if( I_B>32767 ){
        return 32767;
    }
    if( I_B<-32768 ){
        return -32768;
    }
    return (short)(I_B);
}
/// //////////////////////////////////////////////////////////////////////////////////////////////////////
/// //////////////////////////////////////////////////////////////////////////////////////////////////////
/// //////////////////////////////////////////////////////////////////////////////////////////////////////
/// //////////////////////////////////////////////////////////////////////////////////////////////////////
/// //////////////////////////////////////////////////////////////////////////////////////////////////////
BYTE nextpow2( int IVal ){
    if( IVal<1 ){
        return 0;
    }
    int IVal2Proc = IVal;
    BYTE B2R = 0;
    while( IVal2Proc>1 ){
        IVal2Proc = IVal2Proc/2;
        B2R = B2R + 1;
    }
    if( (1<<B2R)==IVal ){
        return B2R;
    }
    B2R = B2R + 1;
    return B2R;
}
/// //////////////////////////////////////////////////////////////////////////////////////////////////////
/// //////////////////////////////////////////////////////////////////////////////////////////////////////
/// //////////////////////////////////////////////////////////////////////////////////////////////////////
/// //////////////////////////////////////////////////////////////////////////////////////////////////////
/// //////////////////////////////////////////////////////////////////////////////////////////////////////

inline void WindowPos(HWND hwnd, int* x,int* y, BOOL NOParental=TRUE )
{
POINT pos;
POINT posF;
pos.x=0;
pos.y=0;

ClientToScreen( hwnd, &pos );

HWND hwndP = GetParent( hwnd );
if( hwndP==NULL || NOParental ){
    x[0] = pos.x;
    y[0] = pos.y;
}else{
    ClientToScreen( hwnd, &posF);
    x[0] = posF.x - pos.x;
    y[0] = posF.y - pos.y;
}

//GetWindowPos(hwnd, pos);

}

/// //////////////////////////////////////////////////////////////////////////////////////////////////////
/// //////////////////////////////////////////////////////////////////////////////////////////////////////
/// //////////////////////////////////////////////////////////////////////////////////////////////////////
/// //////////////////////////////////////////////////////////////////////////////////////////////////////
/// //////////////////////////////////////////////////////////////////////////////////////////////////////
