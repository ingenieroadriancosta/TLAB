DWORD WINAPI ThreadCall_FreeMat( LPVOID pv ){
    static BOOL OnCallFM = FALSE;
    if( OnCallFM ){
        return 0;
    }
    OnCallFM = TRUE;
    HWND* HWPRGPTR = &TLAB_STR.HWProgress;
    DSP_MNG.ExecuteFreeMat( pv, HWPRGPTR );
    OnCallFM = FALSE;
    return 0;
}
