LRESULT TLAB::WProcedure( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam ){
    switch( message ){
        /*
        case WM_SETFOCUS:
            /// RedrawWindow( hwnd, NULL, NULL, RDW_UPDATENOW | RDW_ALLCHILDREN );
            /// MsgBoxF( " " );
            for( int i=0; i<64; i++ ){
                /// B_1D[i].SetVisible( FALSE );B_1D[i].SetVisible( TRUE );
            }
            /// UpdateWindow( hwnd );
            break;
            //*/
            /*
        case WM_SHOWWINDOW:{
            RECT rewin = {0};
            CenterWindow( hwnd, 0, 30, &rewin );
        }
            break;
            ///*/
        case WM_CREATE:
            return Create( hwnd, message, wParam, lParam );
            break;
        case WM_SIZE:
           SendMessage( HW_STATUS_LOW, WM_SIZE, wParam, lParam );
           break;
        case WM_COMMAND:{///
            return CmdProcs( hwnd, message, wParam, lParam );
            if( OnCmds ){
                break;
            }
            MSG_Cmds.hwnd = hwnd;
            MSG_Cmds.message = message;
            MSG_Cmds.wParam = wParam;
            MSG_Cmds.lParam =  lParam;
            DWORD IdTh = 0;
            CreateThread( NULL, 0, ThreadCall_CMDS, (PVOID)&IPtr, 0, &IdTh );
            break;
        }
            ///return CmdProcs( hwnd, message, wParam, lParam );
            break;
        ///
        case WM_NOTIFY:{
            LPNMHDR lpnmhdr = (LPNMHDR)lParam;
            if (lpnmhdr->code == TCN_SELCHANGE){
                TabPage = SendMessage( HWTABS, TCM_GETCURSEL, 0, 0 );
                /// char TT2[128] = {0};sprintf( TT2, "TabPage. %i", TabPage );SetWindowTextA( HW_STATUS, TT2 );
                Pltrs.SetVisible( FALSE );
                ShowWindow( HW_IMSHOW, 0 );
                HistPlot.SetVisible( FALSE );
                if( TabPage==0 ){
                    Pltrs.SetVisible( TRUE );
                    for( int i=0; i<16; i++ ){
                        B_1D[i].SetVisible( TRUE );
                        B_2D[i].SetVisible( FALSE );
                    }
                    SetOptionBSel();
                }else{
                    //ShowWindow( HW_IMSHOW, 1 );
                    if( Dim2Procs[BSel_2D].ProcType==HIST_2DPROC ){
                        HistPlot.SetVisible( TRUE );
                    }else{
                        ShowWindow( HW_IMSHOW, 1 );
                    }
                    for( int i=0; i<16; i++ ){
                        B_2D[i].SetVisible( TRUE );
                        B_1D[i].SetVisible( FALSE );
                    }
                    SetOptionBSel_2D();
                    UpdateWindow( HW_MAIN );
                    InvalidateRect( HW_MAIN, NULL, TRUE );
                    /// RedrawWindow()
                }
                SetStatusText( " " );
            }
            }
            break;
        case WM_CLOSE:
            DSP_MNG.QuitOfFreeMat();
            for( int i=0; i<MaxProcs; i++ ){
                Dim1Procs[i].StopPlay();
            }
            SaveRestFile();
            if( DSP_MNG.IsOnFreeMat ){
                //SendMessageA( FindWindowA(NULL, "FreeMat v4.0 Command Window"), WM_CLOSE, 0, 0 );
                //SetWindowTextA( GetConsoleWindow(), " TLAB CON F " );
            }
            return DefWindowProc (hwnd, message, wParam, lParam);
            break;
        case WM_DESTROY:
            PostQuitMessage( 0 );
            break;
        default:
            return DefWindowProc (hwnd, message, wParam, lParam);
    }
    return 0;
}
