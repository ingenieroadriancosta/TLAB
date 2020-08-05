int TLAB::Procs2D( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam ){
    DWORD IdTh = 0;
    if( BSel_2D==0 ){
        ///
        if( OpenFileAs( Dim2Procs[BSel_2D].FNameT, NULL, hwnd ) ){
            hwndC = HW_MAIN;
            HWProgress = NULL;
            CreateThread( NULL, 0, ThreadCall_Progress, (PVOID)&IPtr, 0, &IdTh );
            SendMessage( HW_Progr, PBM_SETPOS, (WPARAM)50, 0);
            strcpy( AUXTEXT, "Cargando..." );
            if( !Dim2Procs[BSel_2D].PreloadImage() ){
                while( HWProgress==NULL ){
                    Sleep( 10 );
                }
                SendMessageA( HWProgress, WM_CLOSE, 0, 0 );
                MessageBoxA( HW_MAIN, "La imagen no se pudo cargar.", " Error ", 16 );
                return 0;
            }
            ///
            ///
            while( HWProgress==NULL ){
                Sleep( 10 );
            }
            ///
            Dim2Procs[0].IsGray = FALSE;
            Dim2Procs[0].IsBin  = FALSE;
            ///
            int NPosP = Dim2Procs[0].WPIC_IN * Dim2Procs[0].HPIC_IN;
            int TopW = Dim2Procs[0].WPIC_IN;
            int TopH = Dim2Procs[0].HPIC_IN;
            BYTE VGRAY = 0;
            ///
            if( MessageBoxA( HWProgress, "\n\n¿Desea convertir la imagen a escala de grises?\n\n\n",
                            "", MB_YESNO )==IDYES ){
                Dim2Procs[0].IsGray = TRUE;
                Dim2Procs[0].IsBin  = FALSE;
                Dim2Procs[0].ReadDatas();
                int TopW = Dim2Procs[0].WPIC_IN;
                int TopH = Dim2Procs[0].HPIC_IN;
                SetWindowTextA( HWProgress, "Convirtiendo..." );
                ZeroMemory( Dim2Procs[0].Hist, 256 * sizeof(double) );
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
                    if( (i%TopW)==0 ){
                        SendMessage( HW_Progr, PBM_SETPOS, (WPARAM)( round( 100 * (i/((double)TopW))/TopH ) ), 0 );
                    }
                }
                Dim2Procs[0].WriteDatas();
                SetBitmapBits( Dim2Procs[0].HBMP_OUT, Dim2Procs[0].NBytesBuff, Dim2Procs[0].RgbBuff );
                Dim2Procs[0].graythresh();
            }else{
                ///
                Dim2Procs[0].ReadDatas();
                SetWindowTextA( HWProgress, "Analizando..." );
                ZeroMemory( Dim2Procs[0].Hist, 256 * sizeof(double) );
                for( int i=0; i<NPosP; i++ ){
                    VGRAY = (BYTE)round(
                                    0.2989 * Dim2Procs[0].RgbBuff[i].rgbRed +
                                    0.5870 * Dim2Procs[0].RgbBuff[i].rgbGreen +
                                    0.1140 * Dim2Procs[0].RgbBuff[i].rgbBlue
                                    );
                    Dim2Procs[0].Hist[VGRAY] = Dim2Procs[0].Hist[VGRAY] + 1;
                    if( (i%TopW)==0 ){
                        SendMessage( HW_Progr, PBM_SETPOS, (WPARAM)( round( 100 * (i/((double)TopW))/TopH ) ), 0 );
                    }
                }
                Dim2Procs[0].graythresh();
                ///
            }
            ///
            ///
            ///
            SendMessage( HW_IMSHOW, STM_SETIMAGE, IMAGE_BITMAP,
                                (LPARAM)Dim2Procs[BSel_2D].SetPicPlot( W_BOUT, H_BOUT ) );
            SendMessage( HW_Progr, PBM_SETPOS, (WPARAM)10, 0);
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
            ///
            while( HWProgress==NULL ){
                Sleep( 10 );
            }
        }
        ///
        ///
        SendMessageA( HWProgress, WM_CLOSE, 0, 0 );
        ///
    }else{
        ///
        int ResP = DialogBoxParam( GetModuleHandle(NULL), MAKEINTRESOURCEA(Dial_Change_Proc_2D),
                            HW_MAIN, DlgProc_Proc_2D, (LPARAM)&Dim2Procs[BSel_2D] );
        if( ResP>=0 ){
            ///
            WRITE_ERROR_REPORT();
            ///
            if( Dim2Procs[BSel_2D].ProcType==BINARY_2DPROC ){
                DialogBoxParam( GetModuleHandle(NULL), MAKEINTRESOURCEA(Dial_Select_Levels),
                            HW_MAIN, SELECT_LEVELS, (LPARAM)&Dim2Procs[BSel_2D] );
            }
            ///
            ///
            ///
            if( Dim2Procs[BSel_2D].ProcType==CONV_3x3_2DPROC ){
                DialogBoxParam( GetModuleHandle(NULL), MAKEINTRESOURCEA(Dial_Kernel_Levels),
                            HW_MAIN, SELECT_KERNELS, (LPARAM)&Dim2Procs[BSel_2D] );
            }
            ///
            ///
            ///
            if( Dim2Procs[BSel_2D].ProcType==ARITM_2DPROC ){
                DialogBoxParam( GetModuleHandle(NULL), MAKEINTRESOURCEA(Dial_Arithmetic),
                            HW_MAIN, SELECT_ARITH, (LPARAM)&Dim2Procs[BSel_2D] );
            }
            ///
            ///
            if( Dim2Procs[BSel_2D].ProcType==REDIM_2DPROC ){
                DialogBoxParam( GetModuleHandle(NULL), MAKEINTRESOURCEA(Dial_Redim),
                            HW_MAIN, SELECT_REDIM, (LPARAM)&Dim2Procs[BSel_2D] );
            }
            ///
            ///
            if( Dim2Procs[BSel_2D].ProcType==IMERODE ){
                DialogBoxParam( GetModuleHandle(NULL), MAKEINTRESOURCEA(Dial_ERODE_IMAGE),
                            HW_MAIN, SELECT_ERODE, (LPARAM)&Dim2Procs[BSel_2D] );
            }
            ///
            ///
            if( Dim2Procs[BSel_2D].ProcType==IMDILATE ){
                DialogBoxParam( GetModuleHandle(NULL), MAKEINTRESOURCEA(Dial_ERODE_IMAGE),
                            HW_MAIN, SELECT_DILATE, (LPARAM)&Dim2Procs[BSel_2D] );
            }
            ///
            ///
            if( Dim2Procs[BSel_2D].ProcType==IMOPEN ){
                DialogBoxParam( GetModuleHandle(NULL), MAKEINTRESOURCEA(Dial_ERODE_IMAGE),
                            HW_MAIN, SELECT_OPEN, (LPARAM)&Dim2Procs[BSel_2D] );
            }
            ///
            ///
            if( Dim2Procs[BSel_2D].ProcType==IMCLOSE ){
                DialogBoxParam( GetModuleHandle(NULL), MAKEINTRESOURCEA(Dial_ERODE_IMAGE),
                            HW_MAIN, SELECT_CLOSE, (LPARAM)&Dim2Procs[BSel_2D] );
            }
            ///
            ///
            if( Dim2Procs[BSel_2D].ProcType==IMFILL_OPT ){
                DialogBoxParam( GetModuleHandle(NULL), MAKEINTRESOURCEA(Dial_ImFill),
                            HW_MAIN, SELECT_IMFILL, (LPARAM)&Dim2Procs[BSel_2D] );
            }
            ///
            ///
            ///
            ///
            ///
            ///
            ///
            ///
            ///
            Dim2Procs[BSel_2D].IsBin = (Dim2Procs[BSel_2D].ProcType==BINARY_2DPROC) ||
                                        Dim2Procs[BSel_2D].GetInput()->IsBin;
            Dim2Procs[BSel_2D].IsGray = (Dim2Procs[BSel_2D].ProcType==RGB2GRAY_2DPROC) ||
                                            Dim2Procs[BSel_2D].IsBin ||
                                            Dim2Procs[BSel_2D].GetInput()->IsGray ||
                                            Dim2Procs[BSel_2D].GetInput()->IsBin;
            ///
            ///
            DIM2* D2Parent = NULL;
            for( int i=(BSel_2D+1); i<MaxProcs; i++ ){
                D2Parent = Dim2Procs[i].GetInput();
                Dim2Procs[i].IsBin = D2Parent->IsBin;
                Dim2Procs[i].IsGray = D2Parent->IsGray;
            }
            ///
            ///
            ///
            ///
            ///
            if( Dim2Procs[BSel_2D].ImageInP==0 ){
                B_2D[BSel_2D].SetBorderColor( 0, 255, 0 );
            }else{
                B_2D[BSel_2D].SetBorderColor( 255, 0, 0 );
            }
            B_2D[BSel_2D].SetPInit = Dim2Procs[BSel_2D].ImageInP>0;
            ///
            ///
            ///
            for( int i=BSel_2D+1; i<MaxProcs; i++ ){
                Dim2Procs[i].WPIC_IN = Dim2Procs[BSel_2D].WPIC_IN;
                Dim2Procs[i].HPIC_IN = Dim2Procs[BSel_2D].HPIC_IN;
            }
            ///
            ///
            ///
            for( int i=BSel_2D; i<MaxProcs; i++ ){
                Dim2Procs[i].IsLoaded = FALSE;
            }
            B_2D[BSel_2D].SetPicOn( IMSHOW+Dim2Procs[BSel_2D].ProcType, FILE_RES );
            B_2D[BSel_2D].SetOn( "Seleccionado", TRUE );
            SetFocus( B_2D[BSel_2D].HWCTRL );
            SetOptionBSel_2D();
            SetFocus( B_2D[BSel_2D].HWCTRL );
            SendMessageA( HW_MAIN, WM_COMMAND, CMD_SETFOCUS_1_2_D, 0 );
            return TRUE;
        }
        SetOptionBSel_2D();
        /// SetFocus( B_2D[BSel_2D].HWCTRL );
        HistPlot.SetVisible( Dim2Procs[BSel_2D].ProcType==HIST_2DPROC );
        Sleep( 50 );
        ShowWindow( HW_IMSHOW, (int)(Dim2Procs[BSel_2D].ProcType!=HIST_2DPROC) );
        SendMessageA( HW_MAIN, WM_COMMAND, CMD_SETFOCUS_1_2_D, 0 );
    }
    SendMessageA( HW_MAIN, WM_COMMAND, CMD_SETFOCUS_1_2_D, 0 );
    return 0;
}
/// int                     Procs2D                         ( HWND, UINT, WPARAM, LPARAM );
