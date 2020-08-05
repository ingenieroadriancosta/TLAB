BOOL CALLBACK PLAYDESIGN_Dlg( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam ){
    const int ModsId = 128;
    static ImageButton HWMOD[MaxProcs];
    static HWND HWPERCENT[MaxProcs];
    static HWND HWPERCENT_TT[MaxProcs];
    static HWND HWPERCENT_TEXT[MaxProcs];
    static DIM1* DMPTR = NULL;
    static char T2T[1024] = {0};
    switch(message){
        case WM_INITDIALOG:{
            DMPTR = (DIM1*)lParam;
            ///
            SendMessageA( GetDlgItem(hwnd, 12000), BM_SETCHECK, BST_CHECKED * (int)DMPTR->PLAY_SET, 0 );
            ///
            char TTemp[1024] = {0};
            int W_PRCS_A = 80;
            int ADR_BTT_W = 16;
            int ADR_BTT_H = 84;
            for( int y=0; y<2; y++ ){
                for( int x=0; x<8; x++ ){
                    HWMOD[x+y*8].Set_Font( 14 );
                    HWMOD[x+y*8].SetAdicWStyle( WS_BORDER | BS_CENTER|BS_AUTOCHECKBOX );/// BS_LEFT    BS_CENTER
                    HWMOD[x+y*8].SetV_Minus( 20 );
                    HWMOD[x+y*8].Create( hwnd,
                                        x*(W_PRCS_A+ADR_BTT_W) + 5,
                                        ( y*(W_PRCS_A+ADR_BTT_H) )+37,
                                        W_PRCS_A, W_PRCS_A, ModsId+(8*y+x) );
                    HWPERCENT[x+y*8] = CreateWindowEx(    0, "EDIT", NULL,
                                                      WS_BORDER | WS_CHILD | WS_VISIBLE | WS_TABSTOP | ES_NUMBER,
                                                x*(W_PRCS_A+ADR_BTT_W) + 5,
                                                ( y*(W_PRCS_A+ADR_BTT_H) )+37 + W_PRCS_A + 6,
                                               30, 20, hwnd, (HMENU)((8*y+x)+1024), GetModuleHandle(NULL), NULL );
                    sprintf( T2T, "%i", (int)round(100*DMPTR->PLAY_PERCENT[x+y*8]) );
                    SetWindowTextA( HWPERCENT[x+y*8], T2T );
                    SetToolTip( HWPERCENT[x+y*8], "Porcentaje de la señal", &HWPERCENT_TT[x+y*8] );
                    HWPERCENT_TEXT[x+y*8] = CreateWindowEx(    0, "STATIC", "%", WS_CHILD | WS_VISIBLE,
                                                x*(W_PRCS_A+ADR_BTT_W) + 5+32,
                                                ( y*(W_PRCS_A+ADR_BTT_H) )+37 + W_PRCS_A + 6,
                                               30, 20, hwnd, NULL, GetModuleHandle(NULL), NULL );
                    if( (x+y*8)==0 ){
                        HWMOD[x+y*8].SetPicOn( SIG_IN, FILE_RES );
                        HWMOD[x+y*8].SetOn( NULL, TRUE );
                        HWMOD[x+y*8].SetToolTip( "Señal de entrada" );
                        HWMOD[x+y*8].SetCheck( DMPTR->PLAYSELECTED[x+y*8] );
                        SetFocus( HWMOD[x+y*8].HWCTRL );
                        if( !DMPTR->PLAYSELECTED[x+y*8] ){
                            EnableWindow( HWPERCENT[x+y*8], FALSE );
                        }
                        HWMOD[x+y*8].SetOn( NULL, DMPTR->PLAYSELECTED[x+y*8] );
                    }else{
                        HWMOD[x+y*8].SetPicOn( PLOT_PNG + DMPTR->D1_INIT[x+y*8].ProcType, FILE_RES );
                        HWMOD[x+y*8].SetOn( NULL, FALSE );
                        HWMOD[x+y*8].Enable( FALSE );
                        if( DMPTR->D1_INIT[x+y*8].ProcType==Windowing_Signal ||
                            DMPTR->D1_INIT[x+y*8].ProcType==Filt_Signal             ){
                            HWMOD[x+y*8].Enable( TRUE );
                            HWMOD[x+y*8].SetOn( NULL, TRUE );
                            if( DMPTR->PLAYSELECTED[x+y*8] ){
                                EnableWindow( HWPERCENT[x+y*8], TRUE );
                            }else{
                                EnableWindow( HWPERCENT[x+y*8], FALSE );
                            }
                            ///
                            HWMOD[x+y*8].SetCheck( DMPTR->PLAYSELECTED[x+y*8] );
                            ///
                            if( DMPTR->D1_INIT[x+y*8].ProcType==Filt_Signal ){
                                sprintf( TTemp, "Módulo con Filtro tipo %s", DMPTR->D1_INIT[x+y*8].FDS.GetFilterInfo(T2T) );
                                HWMOD[x+y*8].SetToolTip( TTemp );
                            }else{
                                sprintf( TTemp, "Módulo con ventana tipo %s", DMPTR->D1_INIT[x+y*8].FDS.WINDWN.GetWinInfo(T2T) );
                                HWMOD[x+y*8].SetToolTip( TTemp );
                            }
                            HWMOD[x+y*8].SetOn( NULL, DMPTR->PLAYSELECTED[x+y*8] );
                            ///
                        }else{
                            EnableWindow( HWPERCENT[x+y*8], FALSE );
                        }
                    }
                }
            }
            ///
            ///
            MoveWindow( hwnd, 1, 1, 765+6, 356+29, TRUE );
            RECT re;
            ZeroMemory( &re, sizeof(RECT) );
            CenterWindow( hwnd, 0, 0, &re );
            ///
            sprintf( T2T, " Reproduccion de audio (Módulo %i)", DMPTR->Ind_File );
            SetWindowTextA( hwnd, T2T );
            ///
            /// sprintf( T2T, "%i  --  %i", re.right, re.bottom );MessageBoxA( hwnd, T2T, "", 0 );
        }
            break;
        ///
        case WM_COMMAND:{
            if( DMPTR==NULL ){
                break;
            }
            switch( LOWORD(wParam) ){
                case 12000:
                    DMPTR->PLAY_SET = (BOOL)SendMessageA( GetDlgItem(hwnd, 12000), BM_GETCHECK, 0, 0 )!=0;
                    break;
                case IDCANCEL:
                case IDOK:
                    for( int i=0; i<MaxProcs; i++ ){
                        HWMOD[i].DestroyW();
                        DestroyWindow( HWPERCENT_TEXT[i] );
                        DestroyWindow( HWPERCENT_TT[i] );
                        DestroyWindow( HWPERCENT[i] );
                    }
                    DMPTR = NULL;
                    EndDialog( hwnd, 0 );
                    break;

            }
            if( LOWORD(wParam)>=ModsId && LOWORD(wParam)<(ModsId+MaxProcs) ){
                int I_N = LOWORD(wParam) - ModsId;
                DMPTR->PLAYSELECTED[I_N] = HWMOD[I_N].GetCheck();
                HWMOD[I_N].SetOn( NULL, DMPTR->PLAYSELECTED[I_N] );
                if( DMPTR->PLAYSELECTED[I_N] ){
                    EnableWindow( HWPERCENT[I_N], TRUE );
                }else{
                    EnableWindow( HWPERCENT[I_N], FALSE );
                }
                break;
            }
            if( LOWORD(wParam)>=1024 && LOWORD(wParam)<(1024+MaxProcs) ){
                if( HIWORD(wParam)==EN_CHANGE ){
                    GetWindowTextA( ((HWND)lParam), T2T, 8 );
                    int I_N = LOWORD(wParam) - 1024;
                    if( atoi(T2T)>100 ){
                        SetWindowTextA( ((HWND)lParam), "100" );
                        DMPTR->PLAY_PERCENT[I_N] = 1;
                    }else{
                        DMPTR->PLAY_PERCENT[I_N] = ((double)atoi(T2T))/100;
                    }
                }
                break;
            }
        }
            break;
        ///
        ///
        ///
        ///
    }
    return FALSE;
}
/// /////////////////////////////////////////////////////////////////////////////////////////////////////
/// /////////////////////////////////////////////////////////////////////////////////////////////////////
/// /////////////////////////////////////////////////////////////////////////////////////////////////////
/// /////////////////////////////////////////////////////////////////////////////////////////////////////
/// /////////////////////////////////////////////////////////////////////////////////////////////////////
/// /////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL PLAY_DESIGN( HWND HW_Fath, VOID* FdsnC ){
    return (BOOL)DialogBoxParamA( GetModuleHandle(NULL), MAKEINTRESOURCE(Dial_PlayDesign_GUI),
                                 HW_Fath, (DLGPROC)PLAYDESIGN_Dlg, (LPARAM)FdsnC );
}
