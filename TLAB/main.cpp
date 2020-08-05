/// TLAB_STR.Dim2Procs.BWLABEL_NEW4
/// TLAB_STR.Dim2Procs.IMFILLPROC
/// TLAB_STR.Dim2Procs.CONV3x3
/// TLAB_STR.Dim2Procs.AVERAGEPROC
/// DSP_MNG.imerode
/// Window_F
#include "TLAB_API\TLAB.cpp"

/// //////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// //////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// //////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// //////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// //////////////////////////////////////////////////////////////////////////////////////////////////////////////
int WINAPI WinMain( HINSTANCE hThisInstance, HINSTANCE hPrevInstance, LPSTR lpszArgument, int nCmdShow ){
    INITCOMMONCONTROLSEX icex;
    icex.dwSize = sizeof(INITCOMMONCONTROLSEX);
    icex.dwICC = ICC_TAB_CLASSES;
    InitCommonControlsEx(&icex);
    InitCommonControls();
    if( FindWindow( NULL, " TLAB CON " )!=NULL || FindWindow( NULL, " TLAB " )!=NULL ){
        SendMessageA( FindWindow( NULL, " TLAB " ), WM_COMMAND, CMD_SHOW_HIDE_NEW_WIN, 0 );
        return 0;
    }
    SetWindowTextA( GetConsoleWindow(), " TLAB CON " );
    ///
    ///
    ///
    ///
    ///
    ///
    ///
    ///
    ///
    /*/
    fftprepare fftin;
    fftin.Prepare( MaxFileSize );
    double DblPi = (2 * pi_dbl * 100)/44100;
    for( int i=0; i<MaxFileSize; i++ ){
        fftin.InputV[i] = sin( DblPi * i );
    }
    free( fftin.RealV );fftin.RealV = NULL;
    free( fftin.ImagV );fftin.ImagV = NULL;
    for( int i=0; i<2; i++ ){
        DSP_MNG.tic( 645 );
        DSP_MNG.FFT( MaxFileSize, fftin.InputV, NULL, NULL, FALSE );
        DSP_MNG.toc( 645 );
        TLAB_STR.MsgBoxF( "%f segundos.\n%i Megas", DSP_MNG.GetEndToc(645), DSP_MNG.NPosPot2FFT/(1024*1024) );
        DSP_MNG.SetPlanFFT( 0 );
    }
    exit( 0 );
    //*/
    ///
    ///
    /// char* Dela = NULL;for( int i=0; i<1024*1024; i++ ){ Dela[i] = 0; }
    /// DialogBoxParam( GetModuleHandle(NULL), MAKEINTRESOURCEA(Dial_Arithmetic), NULL, SELECT_ARITH, (LPARAM)0 );exit( 0 );
    ///
    /*
    int* IK = &ConvKernelEmboss[0][0];
    for( int i=0; i<9; i++ ){
        TLAB_STR.MsgBoxF( "%i", IK[i] );
    }
    //*/
    /// TLAB_STR.MsgBoxF( "%i", sizeof(TLAB_STR.Dim2Procs[0].Hist) );exit( 0 );
    //DIM2 Dim2Procs;DialogBoxParam( GetModuleHandle(NULL), MAKEINTRESOURCEA(Dial_Change_Proc_2D), NULL, DlgProc_Proc_2D, (LPARAM)&Dim2Procs );
    /// TLAB_STR.MsgBoxF( "%i\n%i", Dim2Procs.ProcType, Dim2Procs.ImageInP );
    //exit( 0 );
    ///TLAB_STR.Procs2D
    /*
    //TLAB_STR.MsgBoxF( "%i", sizeof(long long) );
    DIM1 Dim1_mng;
    DialogBoxParamA( GetModuleHandle(NULL), MAKEINTRESOURCE(DLG_1D_INIT), NULL, (DLGPROC)Dlg1D_INIT,
                                     (LPARAM)(&Dim1_mng) );exit( 0 );
    //*/
    ///
    ///
    return TLAB_STR.CreateMainWindow();
}
/// //////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// //////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// //////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// //////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// //////////////////////////////////////////////////////////////////////////////////////////////////////////////
LRESULT CALLBACK WindowProcedure( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam ){
    return TLAB_STR.WProcedure( hwnd, message, wParam, lParam );
}
/// //////////////////////////////////////////////////////////////////////////////////////////////////////////////
