int TLAB::Create( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam ){
    HW_MAIN = hwnd;
    /// ///////////////////////////////////////////////////////////////////////////////////////////////////
    /// ///////////////////////////////////////////////////////////////////////////////////////////////////
    RECT Rect = {0};
    GetClientRect( hwnd, &Rect );
    int W_Win = Rect.right;
    int H_Win = Rect.bottom;
    ///
    ///SBARS_SIZEGRIP
    ///HW_STATUS_LOW = CreateStatusWindow( WS_CHILD|WS_VISIBLE, "Texto de \nprueba", hwnd, 0 );
    int H_LESS = ( (int)( HW_STATUS_LOW!=NULL ) ) * 19;
    ///
    ///
    ///
    ///
    #define LWRUN 26
    HW_RUN = CreateWindowEx(
        WS_EX_TOPMOST,
        "button",
        "",
        WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_BITMAP | BS_LEFT,  // SS_BITMAP
        W_Win - 123 + LWRUN,
        2,
        120 - LWRUN + 1,
        31,
        hwnd,
        (HMENU)CMD_RUN,
        GetModuleHandle(NULL),
        NULL
        );
    SendMessage( HW_RUN, BM_SETIMAGE, IMAGE_BITMAP,(LPARAM)LoadImageR( RUN_PNG, MAKEINTRESOURCEA(FILE_RES) ) );
    SetToolTip( HW_RUN, "Procesa todos los módulos. ( Ctrl + R )", NULL );
    ///
    ///
    ///
    ///
    ///
    ///
    HWTABS = CreateWindowEx(
        0,
        WC_TABCONTROL,
        "",
        WS_CHILD | WS_VISIBLE | WS_BORDER |
        /// WS_TABSTOP |
        TCS_RIGHTJUSTIFY,
        1, 1,
        W_Win - 2, H_Win - 2 - H_LESS,
        hwnd, NULL, GetModuleHandle(NULL), NULL
        );
    ///
    ///
    HFONT HFNT = CreateFont( 24, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, DEFAULT_CHARSET,
                                OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, PROOF_QUALITY,
                                    DEFAULT_PITCH | FF_ROMAN, "Times" );
    SendMessage( HWTABS, WM_SETFONT, (WPARAM)HFNT, MAKELPARAM(TRUE, 0) );
    ///
    ///
    TCITEM tie = {0};
    tie.mask = TCIF_TEXT;
    tie.iImage = 0;
    ///
    tie.pszText = (LPSTR)"1D             ";
    TabCtrl_InsertItem(HWTABS, 0, &tie);
    ///
    tie.pszText = (LPSTR)"IMAGEN         ";
    TabCtrl_InsertItem( HWTABS, 1, &tie );
    ///
    ///
    ///
    HW_STATUS = CreateWindowEx(
        0,
        "static",
        "",
        WS_CHILD |
        WS_BORDER |
        WS_VISIBLE,
        250, -1,
        W_Win - 2 - 375 + LWRUN + 2, 31,
        HWTABS, NULL, GetModuleHandle(NULL), NULL
        );
    HFNT = CreateFont( 15, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET,
                                OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, PROOF_QUALITY,
                                    DEFAULT_PITCH | FF_ROMAN, "Times New Roman" );
    SendMessage( HW_STATUS, WM_SETFONT, (WPARAM)HFNT, MAKELPARAM(TRUE, 0) );
    ///
    ///
    ///
    /// ///////////////////////////////////////////////////////////////////////////////////////////////////
    /// ///////////////////////////////////////////////////////////////////////////////////////////////////
    const int WPGB = 80;
    const int HPGB = 28;
    HW_PBAR = CreateWindowEx( 0, PROGRESS_CLASS, (LPTSTR) NULL,
                              WS_CHILD | WS_BORDER,
                              W_Win - WPGB - 2 - 124,
                              3,
                              WPGB,
                              HPGB,
                              hwnd, (HMENU) 0, GetModuleHandleA(NULL), NULL);
    SendMessage( HW_PBAR, PBM_SETRANGE, 0, MAKELPARAM(0, 100) );
    /*
    HDC dct = GetDC( hwnd );
    HBITMAP HBMPT = CreateCompatibleBitmap( dct, W_PRCS_1, W_PRCS_1 );
    ReleaseDC( hwnd, dct );
    RGBQUAD* rgbt = (RGBQUAD*)malloc( W_PRCS_1*W_PRCS_1 * sizeof(RGBQUAD) );
    double dbuf[1024] = {0};
    for( int i=0; i<1024; i++ ){
        dbuf[i] = 0.95 * sin( (2 * 3.1415926535897932384626433832795 * i)/1024 );
        dbuf[i] = (dbuf[i] + 1)/2;
    }
    ///
    for( int i=0; i<W_PRCS_1*W_PRCS_1; i++ ){
        rgbt[i].rgbReserved = 0;
    }
    ///
    int x = 0;
    int y = 0;
    for( int i=0; i<1024; i++ ){
        x = (int)round( (W_PRCS_1 * i)/1024.0 );
        y = (int)round( W_PRCS_1 * (1 - dbuf[i]) );
        rgbt[x + y*W_PRCS_1 ].rgbReserved = 255;
        rgbt[x + y*W_PRCS_1 ].rgbRed = 210;
        rgbt[x + y*W_PRCS_1 ].rgbGreen = 0;
        rgbt[x + y*W_PRCS_1 ].rgbBlue = 0;
    }
    SetBitmapBits( HBMPT, W_PRCS_1 * W_PRCS_1 * sizeof(RGBQUAD), rgbt );
    //*/
    /// 88  --  /
    /// ShowWindow( hwnd, 1 );MsgBoxF( "%i", W_PRCS );
    ///
    ///
    const int W_PRCS   = 82;///( H_Win - 43 )/6;
    /// const int W_PRCS_1 = W_PRCS - 8;
    ///
    #define ADR_BTT ( 0 )
    for( int y=0; y<2; y++ ){
        for( int x=0; x<8; x++ ){
            B_1D[x+y*8].Set_Font( 14 );
            B_1D[x+y*8].SetAdicWStyle( BS_CENTER|BS_NOTIFY );/// BS_LEFT    BS_CENTER
            B_1D[x+y*8].SetV_Minus( 14 );
            B_1D[x+y*8].Create( hwnd, x*(W_PRCS+ADR_BTT) + 5, ( y*(W_PRCS+ADR_BTT) )+37, W_PRCS, W_PRCS, CMD_1_2_Ds+(8*y+x) );
            if( (x+y*8)==0 ){
                B_1D[x+y*8].SetPicOn( SIG_IN, FILE_RES );
                B_1D[x+y*8].SetOn( "Seleccionado", TRUE );
                B_1D[x+y*8].SetToolTip( "Carga de la señal, doble click, Ctrl+click ó Ctrl + Espacio para abrir." );
            }else{
                B_1D[x+y*8].SetPicOn( NO_PROC_PIC, FILE_RES );
                B_1D[x+y*8].SetOn( NULL, FALSE );
                B_1D[x+y*8].Enable( FALSE );
                B_1D[x+y*8].SetToolTip( "Procesado de la señal, doble click, Ctrl+click ó Ctrl + Espacio para abrir." );
            }
            B_1D[x+y*8].SetVisible( TabPage==0 );
            //B_1D[x+y*8].SetPicOn( HBMPT );
        }
    }
    ///
    for( int y=0; y<2; y++ ){
        for( int x=0; x<8; x++ ){
            B_2D[x+y*8].Set_Font( 14 );
            B_2D[x+y*8].SetAdicWStyle( BS_CENTER|BS_NOTIFY );
            B_2D[x+y*8].SetV_Minus( 14 );
            B_2D[x+y*8].Create( hwnd, x*(W_PRCS+ADR_BTT) + 5, ( y*(W_PRCS+ADR_BTT) )+37, W_PRCS, W_PRCS, CMD_1_2_Ds+(8*y+x) );
            if( (x+y*8)==0 ){
                B_2D[x+y*8].SetPicOn( IMAGEFILE, FILE_RES );
                B_2D[x+y*8].SetOn( "Seleccionado", TRUE );
                B_2D[x+y*8].SetToolTip( "Carga de la imagen, doble click, Ctrl+click ó Ctrl + Espacio para abrir." );
            }else{
                B_2D[x+y*8].SetPicOn( NO_PROC_PIC, FILE_RES );
                B_2D[x+y*8].SetOn( NULL, FALSE );
                B_2D[x+y*8].Enable( FALSE );
                B_2D[x+y*8].SetToolTip( "Procesado de la imagen, doble click, Ctrl+click ó Ctrl + Espacio para abrir." );
            }
            //B_1D[x+y*8].SetPicOn( HBMPT );
            B_2D[x+y*8].SetVisible( TabPage==1 );
            ///
        }
    }
    ///
    ///
    for( int i=1; i<MaxProcs; i++ ){
        Dim1Procs[i].Ind_File = i;
        Dim1Procs[i].ProcType = NO_PROC_Signal;
        if( i!=0 ){
            Dim1Procs[i].SetInput( &Dim1Procs[i-1] );
            Dim1Procs[i].D1_INIT = NULL;
        }else{
            Dim1Procs[i].SetInput( NULL );
            Dim1Procs[i].D1_INIT = NULL;
        }
        ///
        Dim2Procs[i].Ind_File = i;
        Dim2Procs[i].ProcType = NO_PROC_Signal_2D;
        if( i!=0 ){
            Dim2Procs[i].SetInput( &Dim2Procs[i-1] );
            Dim2Procs[i].D2_SOURCE = &Dim2Procs[0];
        }else{
            Dim2Procs[i].D2_SOURCE = NULL;
        }
        ///
    }
    ///
    ///
    ///
    ///
    ///
    /// ShowWindow( HWTABS, 0 );
    const int P_Y = ( 2*(W_PRCS+ADR_BTT) )+39;
    Pltrs.PLOTEOS_INIT();
    Pltrs.SetFather( hwnd ); /// HWTABS   hwnd                    .
    Pltrs.SetBackG( 255 );
    Pltrs.SetGridStyle( 1 );
    Pltrs.ActGridStyle( TRUE );
    Pltrs.SetVisible( TRUE );
    Pltrs.SetLocationCon( 4, P_Y );
    Pltrs.SetSizeCon( W_Win - 11, H_Win - P_Y - 4 - H_LESS );
    Pltrs.FillGrid( TRUE );
    Pltrs.SetTitle( " Señales de entrada " );
    Pltrs.Text2ShowF( " Tiempo(Segundos) " );
    Pltrs.SetMaxYLim( 1 );
    Pltrs.SetMinYLim( 0 );
    Pltrs.SetMaxXLim( 1, TRUE );
    Pltrs.SetVisible( (int)(TabPage==0) );
    Pltrs.SetTextMiddle( "Sin señal, presione ejecutar." );
    ///                          B    G   R
    const RGBQUAD Rgb2Plot = { 180,   0,  0, 0 };
    Pltrs.SetLineColor( Rgb2Plot );
    Pltrs.SetLineColor( LineColors[0] );
    ///
    ///
    ///
    ///
    ///
    HistPlot.PLOTEOS_INIT();
    HistPlot.SetFather( hwnd ); /// HWTABS   hwnd                    .
    HistPlot.SetBackG( 255 );
    HistPlot.SetGridStyle( 1 );
    HistPlot.ActGridStyle( TRUE );
    HistPlot.SetVisible( TRUE );
    HistPlot.SetLocationCon( 4, P_Y );
    HistPlot.SetSizeCon( W_Win - 11, H_Win - P_Y - 4 - H_LESS );
    HistPlot.FillGrid( TRUE );
    HistPlot.SetTitle( " Historigrama " );
    HistPlot.Text2ShowF( " Pixeles ( 0 - 255 ) " );
    HistPlot.SetMaxYLim( 1 );
    HistPlot.SetMinYLim( 0 );
    HistPlot.SetMaxXLim( 255, TRUE );
    HistPlot.SetVisible( FALSE );
    HistPlot.SetRoundValues( TRUE );
    HistPlot.SetTextInfoAddVisible( TRUE );
    HistPlot.SetTextMiddle( "Sin señal, presione ejecutar." );
    ///                          B    G   R
    HistPlot.SetLineColor( Rgb2Plot );
    HistPlot.SetLineColor( LineColors[0] );
    ///
    ///
    ///
    ///
    ///
    ///
    ///
    ///
    ///
    HW_IMSHOW = CreateWindowEx( 0, "static", NULL,
                /// WS_BORDER |
                WS_CHILD | WS_VISIBLE | SS_BITMAP | SS_REALSIZEIMAGE | SS_CENTERIMAGE |
                SS_NOTIFY,
        4, P_Y,
        W_Win - 11, H_Win - P_Y - 4 - H_LESS,
        hwnd, (HMENU)CMD_PIC_2D, GetModuleHandle(NULL), NULL
    );
    /// HBmpOut = LoadImageR( INIT_PNG_BIG, MAKEINTRESOURCEA(FILE_RES) );/// FILE_RES   INIT_PNG
    /// SendMessage( HW_IMSHOW, STM_SETIMAGE, IMAGE_BITMAP,(LPARAM)HBmpOut );
    ShowWindow( HW_IMSHOW, (int)(TabPage==1) );
    /// ShowWindow( hwnd, 1 );MsgBoxF( "%ix%i\n%g", W_Win - 11, H_Win - P_Y - 4, ( double(W_Win - 11) )/( H_Win - P_Y - 4 ) );
    ///
    RECT re = {0};
    memset( &re, 0, sizeof(RECT) );
    GetClientRect( HW_IMSHOW, &re );
    W_BOUT = re.right;
    H_BOUT = re.bottom;
    /// ShowWindow( hwnd, 1 );
    /// SetStatusText( "%ix%i\n%g", W_BOUT, H_BOUT, ( double(W_BOUT) )/( H_BOUT ) );
    ///
    ///
    HBITMAP HBT = LoadImageR( NO_PIC, MAKEINTRESOURCEA(FILE_RES) );
    SetImageOut( HBT );
    DeleteObject( HBT );
    ///
    ///
    DWORD IdTh = 0; CreateThread( NULL, 0, ThreadCall_Load_All, (PVOID)&IPtr, 0, &IdTh );
    ///
    return 0;
}

/// ////////////////////////////////////////////////////////////////////////////////
/// ////////////////////////////////////////////////////////////////////////////////
/// ////////////////////////////////////////////////////////////////////////////////
/// ////////////////////////////////////////////////////////////////////////////////
int TLAB::Load_All(){
    HWND hwnd = HW_MAIN;
    ///EnableWindow( hwnd, FALSE );
    SendMessage( HWPGRGEN, PBM_SETPOS, 30, 0 );
    BOOL ResROk = ReadRestFile();
    if( ResROk ){
        for( int i=0; i<MaxProcs; i++ ){
            if( i==0 ){
                //
            }else{
                B_1D[i].SetPInit = Dim1Procs[i].FDS.GetBInput()>0;
                B_1D[i].SetPicOn( Dim1Procs[i].ProcType + PLOT_PNG, FILE_RES );
                ///
                B_2D[i].SetPInit = Dim2Procs[i].ImageInP>0;
                B_2D[i].SetPicOn( Dim2Procs[i].ProcType + IMSHOW, FILE_RES );
            }
        }
        ///
    }
    ///
    SendMessage( HWPGRGEN, PBM_SETPOS, 40, 0 );
    ///
    ///
    ///
    for( int i=0; i<MaxProcs; i++ ){
        Dim1Procs[i].Ind_File = i;
        Dim1Procs[i].HFileMod = NULL;
        Dim1Procs[i].SetPath( DSP_MNG.GetDSPPath() );
        ///
        ///
        if( i==0 ){
            Dim1Procs[i].D1_IN = NULL;
            Dim1Procs[i].D1_INIT = NULL;
        }else{
            Dim1Procs[i].SetInput( &Dim1Procs[i-1] );
            Dim1Procs[i].D1_INIT = &Dim1Procs[0];
            if( Dim1Procs[i].FDS.GetBInput()!=0 ){
                B_1D[i].SetPInit = TRUE;
                B_1D[i].SetOn( NULL, FALSE );
            }
        }
        ///
        ///
        ///
        ///
        Dim2Procs[i].Ind_File = i;
        Dim2Procs[i].HFileMod = NULL;
        Dim2Procs[i].SetPath( DSP_MNG.GetDSPPath() );
        ///
        Dim2Procs[i].D2_SOURCE = &Dim2Procs[0];
        if( i==0 ){
            Dim2Procs[i].Dim_In = NULL;
            Dim2Procs[i].D2_SOURCE = NULL;
        }else{
            B_2D[i].SetPInit = Dim2Procs[i].ImageInP>0;
            Dim2Procs[i].SetInput( &Dim2Procs[i-1] );
            Dim2Procs[i].D2_SOURCE = &Dim2Procs[0];
            B_2D[i].SetOn( NULL, FALSE );
        }
        ///
    }
    ///
    SendMessage( HWPGRGEN, PBM_SETPOS, 50, 0 );
    ///
    ///EnableWindow( hwnd, TRUE );
    SendMessage( HWTABS, TCM_SETCURSEL, TabPage, 0 );
    if( TabPage==0 ){
        ShowWindow( HW_IMSHOW, 0 );
        Pltrs.SetVisible( TRUE );
        for( int i=0; i<16; i++ ){
            B_1D[i].SetVisible( TRUE );
            B_2D[i].SetVisible( FALSE );
        }
    }else{
        Pltrs.SetVisible( FALSE );
        ShowWindow( HW_IMSHOW, 1 );
        for( int i=0; i<16; i++ ){
            B_2D[i].SetVisible( TRUE );
            B_1D[i].SetVisible( FALSE );
        }
    }
    SetStatusText( " " );
    ///
    ///
    ///
    ///
    ///
    ///
    ///
    if( ResROk ){
        SendMessage( HWPGRGEN, PBM_SETPOS, 50, 0 );
        BOOL SetLoad = FALSE;
        BSel_1D = 0;
        switch( Dim1Procs[BSel_1D].SLOAD.SType ){
            case 0:
                if( Dim1Procs[BSel_1D].PreloadWAV(hwnd, FALSE) ){
                    SetLoad = TRUE;
                }
                break;
            case 1:
                if( Dim1Procs[BSel_1D].PreloadMAT(hwnd) ){
                    SetLoad = TRUE;
                }
                break;
            case 2:
                SetLoad = TRUE;
                break;
        }
        if( SetLoad ){
            /// 1 _ D
            Dim1Procs[BSel_1D].PreLoadInfo();
            if( Dim1Procs[BSel_1D].LoadSignal( hwnd, FALSE ) ){
                ///
                /// SetStatusText( "%i\n(%i)", BSel_1D, IndSel );
                for( int i=1; i<MaxProcs; i++ ){
                    B_1D[i].Enable( TRUE );
                    Dim1Procs[i].IsLoaded = FALSE;
                    Dim1Procs[i].StopPlay();
                    Dim1Procs[i].FDS.SetFs( Dim1Procs[BSel_1D].SLOAD.Fs );
                }
                SetStatusText( "Leyendo..." );
                SetWindowTextA( HWProgress, "Leyendo..." );
                SendMessage( HW_Progr, PBM_SETPOS, (WPARAM)40, 0);
                Dim1Procs[BSel_1D].ReadDatas();
                /// Pltrs.SetLineColor( LineColors[0] );
                SetStatusText( "Graficando..." );
                SetWindowTextA( HWProgress, "Graficando..." );
                SendMessage( HW_Progr, PBM_SETPOS, (WPARAM)60, 0);
                Pltrs.SetMaxXLim(
                                 Dim1Procs[BSel_1D].NPosBuff/( (double)Dim1Procs[BSel_1D].SLOAD.Fs ),
                                 TRUE );
                Pltrs.PLOTDOUBLE_BIPOL( Dim1Procs[BSel_1D].DBuff, Dim1Procs[BSel_1D].NPosBuff, 0, TRUE );
                Dim1Procs[BSel_1D].FreeBuff();
                Dim1Procs[BSel_1D].SetPicPlot( Pltrs.GetBitmap() );
                Dim1Procs[BSel_1D].D1_IN = NULL;
                Dim1Procs[BSel_1D].D1_INIT = NULL;
                ///
                SetStatusText( "Señal cargada!!!." );
                SetWindowTextA( HWProgress, "Señal cargada!!!." );
                SendMessageA( HWProgress, WM_CLOSE, 0, 0 );
                ///
                ///
                ///
                ///
                ///
            }else{
                Dim1Procs[0].UnLoadInfo();
                SetStatusText( "La señal no pudo ser cargada!!!." );
                SendMessageA( HWProgress, WM_CLOSE, 0, 0 );
            }
            SendMessage( HWPGRGEN, PBM_SETPOS, 70, 0 );
            /// 1 _ D   F   I   N
            ///
        }
        ///
        ///
        ///
        ///
        BSel_2D = 0;
        if( Dim2Procs[BSel_2D].PreloadImage() ){
            ZeroMemory( Dim2Procs[0].Hist, 256 * sizeof(double) );
            int NPosP = Dim2Procs[0].WPIC_IN * Dim2Procs[0].HPIC_IN;
            BYTE VGRAY = 0;
            if( Dim2Procs[0].IsGray ){
                Dim2Procs[0].IsBin = FALSE;
                int NPosP = Dim2Procs[0].WPIC_IN * Dim2Procs[0].HPIC_IN;
                Dim2Procs[0].ReadDatas();
                for( int i=0; i<NPosP; i++ ){
                    VGRAY =
                    Dim2Procs[0].RgbBuff[i].rgbRed =
                    Dim2Procs[0].RgbBuff[i].rgbGreen =
                    Dim2Procs[0].RgbBuff[i].rgbBlue =
                        (BYTE)round(
                                    0.2989 * Dim2Procs[0].RgbBuff[i].rgbRed +
                                    0.5870 * Dim2Procs[0].RgbBuff[i].rgbGreen +
                                    0.1140 * Dim2Procs[0].RgbBuff[i].rgbBlue
                                    );
                    Dim2Procs[0].Hist[VGRAY] = Dim2Procs[0].Hist[VGRAY] + 1;
                }
                Dim2Procs[0].WriteDatas();
                SetBitmapBits( Dim2Procs[0].HBMP_OUT, Dim2Procs[0].NBytesBuff, Dim2Procs[0].RgbBuff );
                SendMessage( HWPGRGEN, PBM_SETPOS, 80, 0 );
            }else{
                Dim2Procs[0].ReadDatas();
                for( int i=0; i<NPosP; i++ ){
                    VGRAY = (BYTE)round(
                                    0.2989 * Dim2Procs[0].RgbBuff[i].rgbRed +
                                    0.5870 * Dim2Procs[0].RgbBuff[i].rgbGreen +
                                    0.1140 * Dim2Procs[0].RgbBuff[i].rgbBlue
                                    );
                    Dim2Procs[0].Hist[VGRAY] = Dim2Procs[0].Hist[VGRAY] + 1;
                }
                SendMessage( HWPGRGEN, PBM_SETPOS, 80, 0 );
            }
            Dim2Procs[0].graythresh();
            SendMessage( HW_IMSHOW, STM_SETIMAGE, IMAGE_BITMAP,
                                (LPARAM)Dim2Procs[BSel_2D].SetPicPlot( W_BOUT, H_BOUT ) );
            Dim2Procs[BSel_2D].FreeImageIn();
            Dim2Procs[BSel_2D].FreeImage();
            Dim2Procs[BSel_2D].FreeBuff();
            Dim2Procs[BSel_2D].IsLoaded = TRUE;
            Dim2Procs[BSel_2D].ProcType = -2;
            ///
            SetStatusText( "Imagen cargada.\n"
                          "Dimensiones: %ix%i",
                          Dim2Procs[BSel_2D].WPIC_IN,
                          Dim2Procs[BSel_2D].HPIC_IN
                          );
            ///
            for( int i=1; i<MaxProcs; i++ ){
                Dim2Procs[i].FreeAll();
                Dim2Procs[i].IsLoaded = FALSE;
                B_2D[i].Enable( TRUE );
                ///
                if( Dim2Procs[i].ProcType==REDIM_2DPROC ){
                    Dim2Procs[i].WPIC_IN = Dim2Procs[i-1].WPIC_OUT;
                    Dim2Procs[i].HPIC_IN = Dim2Procs[i-1].HPIC_OUT;
                    //
                    if( Dim2Procs[i].WPIC_REDIM==0 ){
                        Dim2Procs[i].WPIC_REDIM = Dim2Procs[i].WPIC_IN;
                    }
                    if( Dim2Procs[i].HPIC_REDIM==0 ){
                        Dim2Procs[i].HPIC_REDIM = Dim2Procs[i].HPIC_IN;
                    }
                    //
                    Dim2Procs[i].WPIC_OUT = Dim2Procs[i].WPIC_REDIM;
                    Dim2Procs[i].HPIC_OUT = Dim2Procs[i].HPIC_REDIM;
                }else{
                    Dim2Procs[i].WPIC_IN = Dim2Procs[i-1].WPIC_OUT;
                    Dim2Procs[i].HPIC_IN = Dim2Procs[i-1].HPIC_OUT;
                    //
                    Dim2Procs[i].WPIC_OUT = Dim2Procs[i-1].WPIC_OUT;
                    Dim2Procs[i].HPIC_OUT = Dim2Procs[i-1].HPIC_OUT;
                }
                ///
            }
            SendMessage( HWPGRGEN, PBM_SETPOS, 90, 0 );
        }
        ///
        ///
        ///
    }
    ///
    ///
    ///
    SendMessage( HWPGRGEN, PBM_SETPOS, 50, 0 );
    ///
    for( int i=0; i<MaxProcs; i++ ){
        if( Dim1Procs[i].FDS.IsIRR ){
            Dim1Procs[i].FDS.IIRSet();
        }else{
            Dim1Procs[i].FDS.Fir1Set();
        }
        Dim1Procs[i].FDS.Free();
    }
    DSP_MNG.SetPlanFFT( 0 );
    /// PLAY_DESIGN( HW_MAIN, &Dim1Procs[15] );SaveRestFile();exit( 0 );
    ///
    RECT rewin = {0};
    CenterWindow( hwnd, 0, 30, &rewin );
    if( AlwOnTop ){
        SetWindowPos( hwnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW );
    }
    ///
    ///
    for( int i=50; i<101; i++ ){
        SendMessage( HWPGRGEN, PBM_SETPOS, i, 0 );
        Sleep(100);
        break;
    }
    ///
    //MsgBoxF( " " );exit(0);
    ///
    ///
    CheckMenuItem( HMeW, CMD_ON_ALW_ONTOP, 8*int(AlwOnTop) );
    CheckMenuItem( HMeW, CMD_SETPLOTALL, 8*int(SetPlotimshow) );
    ///
    ///
    SendMessageA( HWPGRGEN_MAIN, WM_CLOSE, 0, 0 );
    ///
    ShowWindow( HW_MAIN, SW_MINIMIZE );
    Sleep( 100 );
    ShowWindow( HW_MAIN, 1 );
    return 0;
}
/// ////////////////////////////////////////////////////////////////////////////////
DWORD WINAPI ThreadCall_Load_All( LPVOID pv ){
    Sleep( 1 );
    ((TLAB*)pv)->Load_All();
    return 0;
}
/// ////////////////////////////////////////////////////////////////////////////////
/// ////////////////////////////////////////////////////////////////////////////////
/// ////////////////////////////////////////////////////////////////////////////////
/// ////////////////////////////////////////////////////////////////////////////////
/// ////////////////////////////////////////////////////////////////////////////////
