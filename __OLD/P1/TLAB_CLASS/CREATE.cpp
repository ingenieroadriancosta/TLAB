BOOL TLAB_CLASS::CREATE( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam ){
    const int WBUT_PROC = 60;
    HW_MAIN_FATHER = hwnd;
    WIN_ON = TRUE;
    RECT rewin = {0};
    MoveWindow( hwnd, 100, 100, 8*WBUT_PROC+ 32 +640, (8*WBUT_PROC + 66 + 6), TRUE );
    CenterWindow( hwnd, 0, 0, &rewin );
    for( int y=0; y<8; y++ ){
        for( int x=0; x<8; x++ ){
            B_DSP_PROCS[y*8+x].SetAdicWStyle( BS_NOTIFY );
            B_DSP_PROCS[y*8+x].Create( HW_MAIN_FATHER, WBUT_PROC*x+3*x+1, WBUT_PROC*y+3*y+1,
                                      WBUT_PROC, WBUT_PROC, CMD_DSP_PROCS + (y*8+x) );
            B_DSP_PROCS[y*8+x].SetPicOn( PIC_PLOT_PROC, DSPTABPICS );
            B_DSP_PROCS[y*8+x].SetOn();
            if( x!=0 || y!=0 ){
                B_DSP_PROCS[y*8+x].SetPicOn( PIC_NONE_PROC, DSPTABPICS );
                B_DSP_PROCS[y*8+x].Enable( FALSE );
            }
        }
    }
    SetFocus( B_DSP_PROCS[0].HWCTRL );
    HW_PIC_OUT = CreateWindowEx( 0, "STATIC", NULL,
                        WS_CHILD | WS_VISIBLE | SS_BITMAP |
                        WS_BORDER |
                        SS_REALSIZEIMAGE | SS_CENTERIMAGE,
                        8*WBUT_PROC + 24, (rewin.bottom - 480)/2,
                        640, 480,
                        HW_MAIN_FATHER, NULL,GetModuleHandleA(NULL), NULL );
    SetImageOutput( TLAB_FIRST, DSPTABPICS );
    return TRUE;
}
