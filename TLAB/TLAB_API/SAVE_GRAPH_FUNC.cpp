BOOL TLAB::SAVE_GRAPH_FUNC(){
    if( TabPage==0 ){
        if( Dim1Procs[BSel_1D].ProcType!=NO_PROC_Signal &&
                Dim1Procs[BSel_1D].ProcType!=PLAY_Signal &&
                Dim1Procs[BSel_1D].IsLoaded ){
            ///
            static char T2T_1D[1024] = {0};
            if( !SaveFileAs( T2T_1D, "JPG Files (*.jpg*)\0*.jpg\0", HW_MAIN ) ){
                return FALSE;
            }
            /// MsgBoxF( T2T_1D );
            ///
            ///
            HWND HWF = Pltrs.GetContainMain();
            int W_F = 0;
            int H_F = 0;
            WindowSize( HWF, &W_F, &H_F );
            ///
            //
            HDC hdcScreen = GetDC(NULL);
            HDC hdc = CreateCompatibleDC(hdcScreen);
            HBITMAP hbmp = CreateCompatibleBitmap( hdcScreen, W_F, H_F );
            SelectObject(hdc, hbmp);
            //Print to memory hdc
            PrintWindow( HWF, hdc, PW_CLIENTONLY );
            //copy to clipboard
            ///
            RGBQUAD* RgbPlt = (RGBQUAD*)malloc( 4 * W_F * H_F + 1024 );
            ZeroMemory( RgbPlt, 4 * W_F * H_F );
            GetBitmapBits( hbmp, 4 * W_F * H_F, RgbPlt );
            DeleteObject( hbmp );
            ///
            //release
            DeleteDC(hdc);
            DeleteObject(hbmp);
            ReleaseDC(NULL, hdcScreen);
            ///
            SAVE_AS_JPG( T2T_1D, W_F, H_F, RgbPlt );
            free( RgbPlt );
            ShellExecuteA( NULL, "open", T2T_1D, NULL, NULL, 3 );
            ///
        }
    }else{
        if( Dim2Procs[BSel_2D].ProcType==NO_PROC_Signal_2D || !Dim2Procs[BSel_2D].IsLoaded ){
            return FALSE;
        }
        static char T2T_2D[1024] = {0};
        if( !SaveFileAs( T2T_2D, "JPG Files (*.jpg*)\0*.jpg\0", HW_MAIN ) ){
            return FALSE;
        }
        ///
        if( Dim2Procs[BSel_2D].ProcType==HIST_2DPROC ){
            ///
            /// MsgBoxF( " " );
            ///
            HWND HWF = HistPlot.GetContainMain();
            int W_F = 0;
            int H_F = 0;
            WindowSize( HWF, &W_F, &H_F );
            ///
            //
            HDC hdcScreen = GetDC(NULL);
            HDC hdc = CreateCompatibleDC(hdcScreen);
            HBITMAP hbmp = CreateCompatibleBitmap( hdcScreen, W_F, H_F );
            SelectObject(hdc, hbmp);
            //Print to memory hdc
            PrintWindow( HWF, hdc, PW_CLIENTONLY );
            //copy to clipboard
            ///
            RGBQUAD* RgbPlt = (RGBQUAD*)malloc( 4 * W_F * H_F + 1024 );
            ZeroMemory( RgbPlt, 4 * W_F * H_F );
            GetBitmapBits( hbmp, 4 * W_F * H_F, RgbPlt );
            DeleteObject( hbmp );
            ///
            //release
            DeleteDC(hdc);
            DeleteObject(hbmp);
            ReleaseDC( NULL, hdcScreen );
            ///
            SAVE_AS_JPG( T2T_2D, W_F, H_F, RgbPlt );
            free( RgbPlt );
            ShellExecuteA( NULL, "open", T2T_2D, NULL, NULL, 3 );
            return FALSE;
        }else{
            Dim2Procs[BSel_2D].ReadDatas();
            SAVE_AS_JPG( T2T_2D, Dim2Procs[BSel_2D].WPIC_OUT, Dim2Procs[BSel_2D].HPIC_OUT,
                        Dim2Procs[BSel_2D].RgbBuff );
            Dim2Procs[BSel_2D].FreeBuff();
            ShellExecuteA( NULL, "open", T2T_2D, NULL, NULL, 3 );
        }
        ///
    }
    return TRUE;
}
