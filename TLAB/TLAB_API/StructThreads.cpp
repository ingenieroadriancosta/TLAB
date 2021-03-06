DWORD WINAPI ThreadCall_DSP_TAD( LPVOID pv ){
    static BOOL In_Dsp_Tad = FALSE;
    if( In_Dsp_Tad ){
        return 0;
    }
    In_Dsp_Tad = TRUE;
    BOOL* OnThr = (BOOL*)pv;
    OnThr[0] = TRUE;
    char FNP[1024] = {0};
    DSP_MNG.WriteDSPTAD( FNP );
    system( FNP );
    In_Dsp_Tad = FALSE;
    OnThr[0] = FALSE;
    return 0;
}
/// ////////////////////////////////////////////////////////////////////////////////
/// ////////////////////////////////////////////////////////////////////////////////
/// ////////////////////////////////////////////////////////////////////////////////
/// ////////////////////////////////////////////////////////////////////////////////
/// ////////////////////////////////////////////////////////////////////////////////
DWORD WINAPI ThreadCall_RUN( LPVOID pv ){
    TLAB* TLB = (TLAB*)pv;
    TLB->RUN_PROCS();
    return 0;
}
/// ////////////////////////////////////////////////////////////////////////////////
/// ////////////////////////////////////////////////////////////////////////////////
/// ////////////////////////////////////////////////////////////////////////////////
/// ////////////////////////////////////////////////////////////////////////////////
/// ////////////////////////////////////////////////////////////////////////////////
DWORD WINAPI ThreadCall_Procs1D( LPVOID pv ){
    TLAB* TLPTR = (TLAB*)pv;
    TLPTR->Procs1D( TLPTR->hwndC, TLPTR->messageC, TLPTR->wParamC, TLPTR->lParamC );
    return 0;
}
/// ////////////////////////////////////////////////////////////////////////////////
/// ////////////////////////////////////////////////////////////////////////////////
/// ////////////////////////////////////////////////////////////////////////////////
/// ////////////////////////////////////////////////////////////////////////////////
/// ////////////////////////////////////////////////////////////////////////////////
DWORD WINAPI ThreadCall_Procs2D( LPVOID pv ){
    TLAB* TLPTR = (TLAB*)pv;
    TLPTR->Procs2D( TLPTR->hwndC, TLPTR->messageC, TLPTR->wParamC, TLPTR->lParamC );
    return 0;
}
/// ////////////////////////////////////////////////////////////////////////////////
/// ////////////////////////////////////////////////////////////////////////////////
/// ////////////////////////////////////////////////////////////////////////////////
/// ////////////////////////////////////////////////////////////////////////////////
/// ////////////////////////////////////////////////////////////////////////////////
DWORD WINAPI ThreadCall_Progress( LPVOID pv ){
    Sleep( 1 );
    TLAB* TLPTR = (TLAB*)pv;
    HWND HWM = NULL;
    if( TLPTR!=NULL ){
        HWM = TLPTR->HW_MAIN;
    }
    DialogBoxParamA( GetModuleHandleA(NULL), MAKEINTRESOURCE(18), (HWND)HWM,
                            (DLGPROC)&DlgProgress12D, (LPARAM)TLPTR );
    return 0;
}
/// ////////////////////////////////////////////////////////////////////////////////
/// ////////////////////////////////////////////////////////////////////////////////
/// ////////////////////////////////////////////////////////////////////////////////
/// ////////////////////////////////////////////////////////////////////////////////
/// ////////////////////////////////////////////////////////////////////////////////
DWORD WINAPI ThreadCall_INIT_PROGRESS( LPVOID pv ){
    INIT_PROGRESS();
    return 0;
}
/// ////////////////////////////////////////////////////////////////////////////////
/// ////////////////////////////////////////////////////////////////////////////////
DWORD WINAPI ThreadCall_Progress_INIT( LPVOID pv ){
    DWORD IdTh = 0;
    CreateThread( NULL, 0, ThreadCall_INIT_PROGRESS, NULL, 0, &IdTh );
    DialogBoxParamA( GetModuleHandleA(NULL), MAKEINTRESOURCE(Dial_LOAD_INIT), (HWND)NULL,
                            (DLGPROC)&DlgProgress12D_INIT, (LPARAM)0 );
    return 0;
}
/// ////////////////////////////////////////////////////////////////////////////////
/// ////////////////////////////////////////////////////////////////////////////////
/// ////////////////////////////////////////////////////////////////////////////////
/// ////////////////////////////////////////////////////////////////////////////////
/// ////////////////////////////////////////////////////////////////////////////////

