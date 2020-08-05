BOOL TLAB_CLASS::CmdProcs( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam ){
    switch( LOWORD(wParam) ){
        case CMD_SALIR:{
            SendMessageA( hwnd, WM_CLOSE, 0, 0 );
        }
        break;
        ///
        case CMD_OPEN_TLAB:{
            MIO_MNG.OpenTLAB();
        }
        break;
        ///
        case CMD_ON_TOP:{
            OnTop = !OnTop;
            if( OnTop ){
                SetWindowPos( hwnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW );
            }else{
                SetWindowPos( hwnd, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW );
            }
            CheckMenuItem( HMeW, CMD_ON_TOP, 8*int(OnTop) );
        }
        break;
        ///
        case CMD_SAVE_DATAS:
            MessageBoxA( hwnd, "", "", 0 );
            break;
        ///
    }
    return 0;
}
