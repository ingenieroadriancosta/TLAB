BOOL CALLBACK DlgProc_Proc_2D(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam){
    static HWND HW_DP_TI = NULL;
    static HWND HW_LB_PRC = NULL;
    static DIM2* D2PTR = NULL;
    const char ProcInfo[16][128] = { "Muestra la imagen de salida.",
                                     "Convierte la imagen a escala de grises.",
                                     "Aplica la binarización con un nivel de offset",
                                     "Aplica la convolución con un kernel de 3x3",
                                     "Muestra el historigrama",
                                     "Redimensiona la imagen",
                                     "Realiza operaciones aritmeticas con la imagen.",
                                     "Etiqueta componentes conectados en una imagen binaria.",
                                     "No realiza ninguna acción",
                                     "Erosiona la imagen.",
                                     "Dilata la imagen.",
                                     "Apertura binaria de la imagen.",
                                     "Cierre binario de la imagen.",
                                     "Rellena regiones de imagen binaria y agujeros.",
                                     "Convierte la imagen de grises a RGB con el mapa JET."
                                     };
    switch(msg){
        case WM_INITDIALOG:{
            D2PTR = (DIM2*)lParam;
            HW_DP_TI = GetDlgItem( hDlg, 10 );
            ///
            HW_LB_PRC = GetDlgItem( hDlg, 20 );
            SendMessage( HW_LB_PRC, LB_ADDSTRING, 0, (LPARAM)"Mostrar imagen");
            SendMessage( HW_LB_PRC, LB_ADDSTRING, 0, (LPARAM)"Convertir a  escala de grises");
            SendMessage( HW_LB_PRC, LB_ADDSTRING, 0, (LPARAM)"Binarización");
            SendMessage( HW_LB_PRC, LB_ADDSTRING, 0, (LPARAM)"Convolución 3x3");
            SendMessage( HW_LB_PRC, LB_ADDSTRING, 0, (LPARAM)"Historigrama");
            SendMessage( HW_LB_PRC, LB_ADDSTRING, 0, (LPARAM)"Redimensionar");
            SendMessage( HW_LB_PRC, LB_ADDSTRING, 0, (LPARAM)"Aritmética");
            SendMessage( HW_LB_PRC, LB_ADDSTRING, 0, (LPARAM)"BWLABEL");
            SendMessage( HW_LB_PRC, LB_ADDSTRING, 0, (LPARAM)"Sin procedimiento");
            SendMessage( HW_LB_PRC, LB_ADDSTRING, 0, (LPARAM)"IMERODE");
            SendMessage( HW_LB_PRC, LB_ADDSTRING, 0, (LPARAM)"IMDILATE");
            SendMessage( HW_LB_PRC, LB_ADDSTRING, 0, (LPARAM)"IMOPEN");
            SendMessage( HW_LB_PRC, LB_ADDSTRING, 0, (LPARAM)"IMCLOSE");
            SendMessage( HW_LB_PRC, LB_ADDSTRING, 0, (LPARAM)"IMFILL");
            SendMessage( HW_LB_PRC, LB_SETCURSEL, (WPARAM)D2PTR->ProcType, 0 );
            SetWindowTextA( HW_DP_TI, ProcInfo[SendMessage( HW_LB_PRC, LB_GETCURSEL, 0, 0 )] );
            ///
            ///
            SendDlgItemMessage( hDlg, 30, BM_SETCHECK, D2PTR->ImageInP>0, 0 );
            ///
            /// CenterWindow( hDlg, 0, 60 );
            ///
            ///
            ///
            }
            return TRUE;
        case WM_COMMAND:
            if( D2PTR==NULL ){
                break;
            }
            switch( LOWORD(wParam) ){
                case 20:
                    switch( HIWORD(wParam) ){
                        case LBN_SELCHANGE:
                            //sprintf( CTINFO, "%s", ProcInfo[SendMessage( HW_LB_PRC, LB_GETCURSEL, 0, 0 )] );
                            SetWindowTextA( HW_DP_TI, ProcInfo[15&SendMessage( HW_LB_PRC, LB_GETCURSEL, 0, 0 )] );
                            break;
                        case LBN_DBLCLK:{
                            ///
                            ///
                            /*
                            DIM2* D2Parent = D2PTR->GetInput();
                            if( SendDlgItemMessage(hDlg, 30, BM_GETCHECK, 0, 0)==BST_CHECKED ){
                                D2Parent = D2PTR->D2_SOURCE;
                            }else{
                                D2Parent = D2PTR->Dim_In;
                            }
                            if( D2Parent->IsBin &&
                               SendMessage( HW_LB_PRC, LB_GETCURSEL, 0, 0 )==BINARY_2DPROC
                               ){
                                MessageBoxA( hDlg,
                                            "La entrada ya es binaria, cambie el procedimiento.",
                                                " Error ", 16 );
                                break;
                            }
                            ///
                            if( (D2Parent->IsBin || D2Parent->IsGray) &&
                               SendMessage( HW_LB_PRC, LB_GETCURSEL, 0, 0 )==RGB2GRAY_2DPROC
                               ){
                                MessageBoxA( hDlg,
                                            "La entrada ya está en escala de grises ó es binaria,"
                                            " cambie el procedimiento.",
                                                " Error ", 16 );
                                break;
                            }
                            //*/
                            ///
                            ///
                            D2PTR->ImageInP = (int)( SendDlgItemMessage(hDlg, 30, BM_GETCHECK, 0, 0) == BST_CHECKED );
                            D2PTR->ProcType = (int)( SendMessage( HW_LB_PRC, LB_GETCURSEL, 0, 0 ) );
                            D2PTR = NULL;
                            EndDialog( hDlg, 0 );
                            }
                            break;
                    }
                    break;
                case IDOK:{
                    /*
                    DIM2* D2Parent = D2PTR->GetInput();
                    if( SendDlgItemMessage(hDlg, 30, BM_GETCHECK, 0, 0)!=BST_CHECKED ){
                        //D2Parent = D2PTR->Dim_In;
                    }
                    if( D2Parent->IsBin &&
                        SendMessage( HW_LB_PRC, LB_GETCURSEL, 0, 0 )==BINARY_2DPROC  &&
                               SendDlgItemMessage(hDlg, 30, BM_GETCHECK, 0, 0)!=BST_CHECKED
                        ){
                        MessageBoxA( hDlg,
                                    "La entrada ya es binaria, cambie el procedimiento.",
                                        " Error ", 16 );
                        break;
                    }
                    ///
                    if( (D2Parent->IsBin || D2Parent->IsGray) &&
                       SendMessage( HW_LB_PRC, LB_GETCURSEL, 0, 0 )==RGB2GRAY_2DPROC &&
                       SendDlgItemMessage(hDlg, 30, BM_GETCHECK, 0, 0)!=BST_CHECKED
                       ){
                        MessageBoxA( hDlg,
                                    "La entrada ya está en escala de grises ó es binaria,"
                                    " cambie el procedimiento.",
                                        " Error ", 16 );
                        break;
                    }
                    ///
                    if( (!D2Parent->IsGray) &&
                        SendMessage( HW_LB_PRC, LB_GETCURSEL, 0, 0 )==CONV_3x3_2DPROC ){
                        MessageBoxA( hDlg,
                                    "La entrada debe estar en escala de grises ó debe ser binaria,"
                                    " cambie el procedimiento.",
                                        " Error ", 16 );
                        break;
                    }
                    //*/
                    ///
                    D2PTR->ImageInP = (int)( SendDlgItemMessage(hDlg, 30, BM_GETCHECK, 0, 0) == BST_CHECKED );
                    D2PTR->ProcType = (int)( SendMessage( HW_LB_PRC, LB_GETCURSEL, 0, 0 ) );
                    D2PTR = NULL;
                    EndDialog( hDlg, 0 );
                    }
                    break;
                case IDCANCEL:
                    D2PTR = NULL;
                    EndDialog( hDlg, -2 );
                    break;
            }
            return TRUE;
    }
        return FALSE;
}

/// //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CALLBACK SELECT_LEVELS(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam){
    static HWND HW_INFO2 = NULL;
    static HWND HW_LEVEL = NULL;
    static HWND HW_PIC_O = NULL;
    static HBITMAP HBMP_P = NULL;
    static RGBQUAD* RgbI = NULL;
    static RGBQUAD* RgbO = NULL;
    static DIM2* D2PTR = NULL;
    static int W_OUT = 0;
    static int H_OUT = 0;
    static double TH_GTH    = 0;
    static double TH_P      = 0;
    static double TH_0      = 0;
    static double TH_255    = 0;
    char T2T[1024] = {0};
    switch(msg){
        case WM_INITDIALOG:{
            ///
            ///
            ///
            D2PTR = (DIM2*)lParam;
            HW_INFO2 = GetDlgItem( hDlg, 10 );
            HW_PIC_O = GetDlgItem( hDlg, 101 );
            sprintf( T2T, "Nivel de offset: %5.4f(%i)",
                                ((double)D2PTR->LevelBIN)/255, D2PTR->LevelBIN );
            SetWindowText( HW_INFO2, T2T );
            HW_LEVEL = GetDlgItem( hDlg, 100 );
            SCROLLINFO si = {0};
            si.cbSize = sizeof(si);
            si.fMask  = SIF_RANGE | SIF_PAGE | SIF_POS;
            si.nMin   = 0;
            si.nMax   = 255;
            si.nPage  = 0;
            si.nPos   = 0;
            //SetScrollInfo( HW_LEVEL , SB_CTL , &si , TRUE );
            SetScrollRange( HW_LEVEL, SB_CTL,0, 255, TRUE);
            SetScrollPos( HW_LEVEL , SB_CTL , D2PTR->LevelBIN, TRUE );
            // SendDlgItemMessage(hDlg, ID_SCROLLV, SBM_SETRANGE, (WPARAM)0, (LPARAM)50);
            ///
            ///
            ///
            SendDlgItemMessage( hDlg, 512, CB_ADDSTRING, 0, (LPARAM)"Manual" );
            SendDlgItemMessage( hDlg, 512, CB_ADDSTRING, 0, (LPARAM)"graythresh" );
            SendDlgItemMessage( hDlg, 512, CB_ADDSTRING, 0, (LPARAM)"Promedio" );
            SendDlgItemMessage( hDlg, 512, CB_ADDSTRING, 0, (LPARAM)"Promedio!=0" );
            SendDlgItemMessage( hDlg, 512, CB_ADDSTRING, 0, (LPARAM)"not graythresh" );
            SendDlgItemMessage( hDlg, 512, CB_ADDSTRING, 0, (LPARAM)"graythresh plus" );
            SendDlgItemMessage( hDlg, 512, CB_SETCURSEL, D2PTR->OffsetType, 0 );
            /// if( HIWORD(wParam)==CBN_SELENDOK ){
            ///
            ///
            ///
            HBITMAP BMSRC = NULL;
            if( D2PTR->GetInput()->IsLoaded &&
                                    D2PTR->GetInput()->ProcType!=NO_PROC_Signal_2D ){
                D2PTR->GetInput()->ReadDatas();
                if( D2PTR->GetInput()->ProcType==BW_LABEL ){
                    int NPosBuffT = D2PTR->GetInput()->WPIC_OUT * D2PTR->GetInput()->HPIC_OUT;
                    for( int i=0; i<(int)NPosBuffT; i++ ){
                        D2PTR->GetInput()->RgbBuff[i].rgbRed   =
                        D2PTR->GetInput()->RgbBuff[i].rgbGreen =
                        D2PTR->GetInput()->RgbBuff[i].rgbBlue  = 255 * (int)((D2PTR->GetInput()->RgbBuff[i].rgbRed!=255) || (D2PTR->GetInput()->RgbBuff[i].rgbGreen!=255) ||
                                                        (D2PTR->GetInput()->RgbBuff[i].rgbBlue!=255));
                    }
                }
                BMSRC = D2PTR->NewBitmap( D2PTR->GetInput()->WPIC_OUT, D2PTR->GetInput()->HPIC_OUT );
                SetBitmapBits( BMSRC, 4 * D2PTR->GetInput()->WPIC_OUT * D2PTR->GetInput()->HPIC_OUT,
                               D2PTR->GetInput()->RgbBuff );
                D2PTR->GetInput()->FreeBuff();
            }else{
                D2PTR->D2_SOURCE->ReadDatas();
                BMSRC = D2PTR->NewBitmap( D2PTR->D2_SOURCE->WPIC_OUT, D2PTR->D2_SOURCE->HPIC_OUT );
                SetBitmapBits( BMSRC, 4 * D2PTR->D2_SOURCE->WPIC_OUT * D2PTR->D2_SOURCE->HPIC_OUT,
                               D2PTR->D2_SOURCE->RgbBuff );
                D2PTR->D2_SOURCE->FreeBuff();
            }
            ///
            ///
            ///
            /* /// OLD
            HBITMAP BMSRC = D2PTR->GetInput()->GetPicPlot();
            if( BMSRC==NULL ){
                BMSRC = D2PTR->D2_SOURCE->GetPicPlot();
            }
            //*/
            ///
            ///
            ///
            BITMAP BmpI = {0};
            GetObject( BMSRC, sizeof(BITMAP) , (LPSTR)&BmpI );
            ///
            int WP = 0;
            int HP = 0;
            ///
            WindowSize( hDlg, &WP, &HP );
            ///
            ///
            ///
            ///
            WindowSize( HW_PIC_O, &WP, &HP );
            ///
            W_OUT = WP;
            H_OUT = (int)( round( (W_OUT * BmpI.bmHeight)/((double)BmpI.bmWidth) ) );
            ///
            if( (H_OUT+99+30)>640 ){
                H_OUT = HP;
                W_OUT =(int)( round( (H_OUT * BmpI.bmWidth)/((double)BmpI.bmHeight) ) );
                /// MoveWindow( HW_PIC_O, 1, 66, WP, HP, TRUE );
            }else{
                MoveWindow( hDlg, 1, 66, W_OUT+8, H_OUT + 99, TRUE );
                MoveWindow( HW_PIC_O, 1, 66, W_OUT, H_OUT, TRUE );
            }
            ///
            HDC ODC = GetDC( 0 );
            HBMP_P = CreateCompatibleBitmap( ODC , W_OUT, H_OUT );
            ReleaseDC( 0, ODC );
            DeleteDC( ODC );
            ///
            HDC hdc = CreateCompatibleDC( GetDC( 0 ) );
            HDC memDC = CreateCompatibleDC( GetDC( 0 ) );
            SelectObject( hdc , BMSRC );/// HBPLOT
            SelectObject( memDC , HBMP_P );/// HB_I
            SetStretchBltMode( memDC , HALFTONE );/// COLORONCOLOR  HALFTONE
            StretchBlt( memDC, 0, 0, W_OUT, H_OUT, hdc, 0, 0, BmpI.bmWidth, BmpI.bmHeight, SRCCOPY );
            ReleaseDC( NULL, hdc );
            ReleaseDC( NULL, memDC );
            DeleteDC( hdc );
            DeleteDC( memDC );
            ///
            DeleteObject( BMSRC );
            ///
            RgbI = (RGBQUAD*)malloc( 4 * W_OUT * H_OUT +1024 );
            ZeroMemory( RgbI, 4 * W_OUT * H_OUT );
            RgbO = (RGBQUAD*)malloc( 4 * W_OUT * H_OUT +1024 );
            ZeroMemory( RgbO, 4 * W_OUT * H_OUT );
            ///
            GetBitmapBits( HBMP_P, 4 * W_OUT * H_OUT, RgbI );
            ///
            ///
            ///
            double Hist[256] = {0};
            ///
            TH_P    = 0;
            TH_0    = 0;
            TH_255  = 0;
            for( int i=0; i<(W_OUT * H_OUT); i++ ){
                RgbI[i].rgbRed =
                RgbI[i].rgbGreen =
                RgbI[i].rgbBlue =
                            (BYTE)round(
                                    0.2989 * RgbI[i].rgbRed +
                                    0.5870 * RgbI[i].rgbGreen +
                                    0.1140 * RgbI[i].rgbBlue
                                    );
                //
                Hist[RgbI[i].rgbRed] = Hist[RgbI[i].rgbRed] + 1;
                //
                TH_P    = TH_P      + RgbI[i].rgbRed;
                TH_0    = TH_0      + (double)(RgbI[i].rgbRed!=0);
            }
            ///
            TH_0    = TH_P/TH_0;
            TH_P    = TH_P/(W_OUT * H_OUT);
            ///
            TH_GTH = round( 255 * D2PTR->graythreshHist( Hist ) );
            ///
            ///
            ///
            ///
            ///
            BYTE PBPOS = 0;
            BYTE THGBY = (BYTE)TH_GTH;
            ZeroMemory( Hist, 256 * sizeof(double) );
            for( int i=0; i<(W_OUT * H_OUT); i++ ){
                //
                if( RgbI[i].rgbRed<=TH_GTH ){
                    Hist[THGBY] = Hist[THGBY] + 1;
                }else{
                    PBPOS = RgbI[i].rgbRed;
                    Hist[PBPOS] = Hist[PBPOS] + 1;
                }
                //
            }
            /// TH_255  = TH_255    + (double)(RgbI[i].rgbRed!=255);
            ///
            TH_255 = ParceBYTE( round( 255 * D2PTR->graythreshHist( Hist ) ) );
            ///
            ///
            ///
            ///
            ///
            ///
            /// BW_OPTS
            SendMessage( GetDlgItem(hDlg, 2000), BM_SETCHECK, BST_CHECKED * D2PTR->BW_OPTS, 0 );
            /// D2PTR->BW_OPTS
            ///
            ///
            BYTE BLEV = 0;
            switch( D2PTR->OffsetType ){
                case 0:
                    BLEV = D2PTR->LevelBIN;
                    break;
                case 1:
                    BLEV = (BYTE)TH_GTH;
                    break;
                case 2:
                    BLEV = (BYTE)TH_P;
                    break;
                case 3:
                    BLEV = (BYTE)TH_0;
                    break;
                case 4:
                    BLEV = (BYTE)(255 - TH_GTH);
                    break;
                case 5:
                    BLEV = (BYTE)TH_255;
                    break;
            }
            if( D2PTR->BW_OPTS==0 ){
                for( int i=0; i<(W_OUT * H_OUT); i++ ){
                    RgbO[i].rgbRed = RgbO[i].rgbGreen = RgbO[i].rgbBlue = 255 * (RgbI[i].rgbRed>BLEV);
                }
            }else{
                for( int i=0; i<(W_OUT * H_OUT); i++ ){
                    RgbO[i].rgbRed = RgbO[i].rgbGreen = RgbO[i].rgbBlue = 255 * (RgbI[i].rgbRed<=BLEV);
                }
            }
            ///
            ///
            ///
            ///
            SetBitmapBits( HBMP_P, 4 * W_OUT * H_OUT, RgbO );
            SendMessage( HW_PIC_O, STM_SETIMAGE, IMAGE_BITMAP,(LPARAM)HBMP_P );
            /// sprintf( T2T, "%ix%i", WP, HP );SetWindowTextA( hDlg, T2T );
            ///
            ///
            CenterWindow( hDlg, 0, 0 );
            ///
            EnableWindow(HW_LEVEL, D2PTR->OffsetType==0 );
            ///
            /// SetFocus( HW_LEVEL );
            ///
            }
            return TRUE;
        case WM_HSCROLL:{
            int Position = GetScrollPos( HW_LEVEL , SB_CTL );
            int Posicion = (int)HIWORD(wParam);
            int Codigo = (int)LOWORD(wParam);
            switch( Codigo ){
              case SB_BOTTOM:
                 Position = 0;
                 break;
              case SB_TOP:
                 Position = 255;
                 break;
              case SB_LINERIGHT:
                 Position++;
                 break;
              case SB_LINELEFT:
                 Position--;
                 break;
              case SB_PAGERIGHT:
                 Position += 5;
                 break;
              case SB_PAGELEFT:
                 Position -= 5;
                 break;
              case SB_THUMBPOSITION:
              case SB_THUMBTRACK:
                 Position = Posicion;
              case SB_ENDSCROLL:
                 break;
            }
            if( Position<=0) Position = 0;
            if( Position>=255 ) Position = 255;
            SetScrollPos( HW_LEVEL , SB_CTL , Position, TRUE );
            if( D2PTR!=NULL ){
                D2PTR->LevelBIN = Position;
                /// BW_OPTS
                if( D2PTR->BW_OPTS==0 ){
                    for( int i=0; i<(W_OUT * H_OUT); i++ ){
                        RgbO[i].rgbRed = RgbO[i].rgbGreen = RgbO[i].rgbBlue = 255 * (RgbI[i].rgbRed>D2PTR->LevelBIN);
                    }
                }else{
                    for( int i=0; i<(W_OUT * H_OUT); i++ ){
                        RgbO[i].rgbRed = RgbO[i].rgbGreen = RgbO[i].rgbBlue = 255 * ((RgbI[i].rgbRed<=D2PTR->LevelBIN));
                    }
                }
                ///
            }
            SetBitmapBits( HBMP_P, 4 * W_OUT * H_OUT, RgbO );
            SendMessage( HW_PIC_O, STM_SETIMAGE, IMAGE_BITMAP,(LPARAM)HBMP_P );
            sprintf( T2T, "Nivel de offset: %5.4f(%i)",
                                ((double)Position)/255, Position );
            SetWindowText( HW_INFO2, T2T );
        }
        return FALSE;
        case WM_COMMAND:
            switch( LOWORD(wParam) ){
                case 512:
                    if( HIWORD(wParam)==CBN_SELENDOK ){
                        D2PTR->OffsetType = (int)SendDlgItemMessage( hDlg, 512, CB_GETCURSEL, 0, 0 );
                        if( D2PTR->OffsetType==0 ){
                            EnableWindow(HW_LEVEL, TRUE );
                            if( D2PTR!=NULL ){
                                ///
                                if( D2PTR->BW_OPTS==0 ){
                                    for( int i=0; i<(W_OUT * H_OUT); i++ ){
                                        RgbO[i].rgbRed = RgbO[i].rgbGreen = RgbO[i].rgbBlue = 255 * (RgbI[i].rgbRed>D2PTR->LevelBIN);
                                    }
                                }else{
                                    for( int i=0; i<(W_OUT * H_OUT); i++ ){
                                        RgbO[i].rgbRed = RgbO[i].rgbGreen = RgbO[i].rgbBlue = 255 * (RgbI[i].rgbRed<=D2PTR->LevelBIN);
                                    }
                                }
                                ///
                            }
                            SetBitmapBits( HBMP_P, 4 * W_OUT * H_OUT, RgbO );
                            SendMessage( HW_PIC_O, STM_SETIMAGE, IMAGE_BITMAP,(LPARAM)HBMP_P );
                            sprintf( T2T, "Nivel de offset: %5.4f(%i)",
                                                ((double)D2PTR->LevelBIN)/255, D2PTR->LevelBIN );
                            SetWindowText( HW_INFO2, T2T );
                        }else{
                            BYTE BLEV = 0;
                            switch( D2PTR->OffsetType ){
                                case 1:
                                    BLEV = (BYTE)TH_GTH;
                                    SetWindowText( HW_INFO2, "graythresh offset" );
                                    break;
                                case 2:
                                    BLEV = (BYTE)TH_P;
                                    SetWindowText( HW_INFO2, "Offset promedio" );
                                    break;
                                case 3:
                                    BLEV = (BYTE)TH_0;
                                    SetWindowText( HW_INFO2, "Offset promedio!=0" );
                                    break;
                                case 4:
                                    BLEV = (BYTE)(255 - TH_GTH);
                                    SetWindowText( HW_INFO2, "(not graythresh) offset" );
                                    break;
                                case 5:
                                    BLEV = (BYTE)(TH_255);
                                    SetWindowText( HW_INFO2, "(graythresh plus) offset" );
                                    break;

                            }
                            if( D2PTR->BW_OPTS==0 ){
                                for( int i=0; i<(W_OUT * H_OUT); i++ ){
                                    RgbO[i].rgbRed = RgbO[i].rgbGreen = RgbO[i].rgbBlue = 255 * (RgbI[i].rgbRed>BLEV);
                                }
                            }else{
                                for( int i=0; i<(W_OUT * H_OUT); i++ ){
                                    RgbO[i].rgbRed = RgbO[i].rgbGreen = RgbO[i].rgbBlue = 255 * (RgbI[i].rgbRed<=BLEV);
                                }
                            }
                            SetBitmapBits( HBMP_P, 4 * W_OUT * H_OUT, RgbO );
                            SendMessage( HW_PIC_O, STM_SETIMAGE, IMAGE_BITMAP,(LPARAM)HBMP_P );
                            EnableWindow(HW_LEVEL, FALSE );
                        }
                    }
                    break;
                ///

                case 2000:
                    D2PTR->BW_OPTS = (int)( SendMessage( GetDlgItem(hDlg, 2000), BM_GETCHECK, 0, 0)==BST_CHECKED );
                    if( D2PTR->OffsetType==0 ){
                        EnableWindow(HW_LEVEL, TRUE );
                        if( D2PTR!=NULL ){
                            ///
                            if( D2PTR->BW_OPTS==0 ){
                                for( int i=0; i<(W_OUT * H_OUT); i++ ){
                                    RgbO[i].rgbRed = RgbO[i].rgbGreen = RgbO[i].rgbBlue = 255 * (RgbI[i].rgbRed>D2PTR->LevelBIN);
                                }
                            }else{
                                for( int i=0; i<(W_OUT * H_OUT); i++ ){
                                    RgbO[i].rgbRed = RgbO[i].rgbGreen = RgbO[i].rgbBlue = 255 * (RgbI[i].rgbRed<=D2PTR->LevelBIN);
                                }
                            }
                            ///
                        }
                        SetBitmapBits( HBMP_P, 4 * W_OUT * H_OUT, RgbO );
                        SendMessage( HW_PIC_O, STM_SETIMAGE, IMAGE_BITMAP,(LPARAM)HBMP_P );
                        sprintf( T2T, "Nivel de offset: %5.4f(%i)",
                                            ((double)D2PTR->LevelBIN)/255, D2PTR->LevelBIN );
                        SetWindowText( HW_INFO2, T2T );
                    }else{
                        BYTE BLEV = 0;
                        switch( D2PTR->OffsetType ){
                            case 1:
                                BLEV = (BYTE)TH_GTH;
                                SetWindowText( HW_INFO2, "graythresh offset" );
                                break;
                            case 2:
                                BLEV = (BYTE)TH_P;
                                SetWindowText( HW_INFO2, "Offset promedio" );
                                break;
                            case 3:
                                BLEV = (BYTE)TH_0;
                                SetWindowText( HW_INFO2, "Offset promedio!=0" );
                                break;
                            case 4:
                                BLEV = (BYTE)(255 - TH_GTH);
                                SetWindowText( HW_INFO2, "(not graythresh) offset" );
                                break;
                            case 5:
                                BLEV = (BYTE)(TH_255);
                                SetWindowText( HW_INFO2, "(graythresh plus) offset" );
                                break;
                        }
                        if( D2PTR->BW_OPTS==0 ){
                            for( int i=0; i<(W_OUT * H_OUT); i++ ){
                                RgbO[i].rgbRed = RgbO[i].rgbGreen = RgbO[i].rgbBlue = 255 * (RgbI[i].rgbRed>BLEV);
                            }
                        }else{
                            for( int i=0; i<(W_OUT * H_OUT); i++ ){
                                RgbO[i].rgbRed = RgbO[i].rgbGreen = RgbO[i].rgbBlue = 255 * (RgbI[i].rgbRed<=BLEV);
                            }
                        }
                        SetBitmapBits( HBMP_P, 4 * W_OUT * H_OUT, RgbO );
                        SendMessage( HW_PIC_O, STM_SETIMAGE, IMAGE_BITMAP,(LPARAM)HBMP_P );
                        EnableWindow(HW_LEVEL, FALSE );
                    }
                    break;
                ///
                case IDOK:
                    free( RgbI );
                    free( RgbO );
                    D2PTR = NULL;
                    DeleteObject( HBMP_P);
                    EndDialog( hDlg, TRUE );
                    break;
                case IDCANCEL:
                    free( RgbI );
                    free( RgbO );
                    DeleteObject( HBMP_P);
                    D2PTR = NULL;
                    EndDialog( hDlg, FALSE );
                    break;
            }
            return TRUE;
        case WM_CLOSE:
            free( RgbI );
            free( RgbO );
            DeleteObject( HBMP_P);
            EndDialog( hDlg, TRUE );
            break;

    }
    return FALSE;
}

/// //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CALLBACK DLGMSGBOX(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam){
    switch(msg){
        case WM_INITDIALOG:{
            ///
            LPCSTR LPS = (LPCSTR)lParam;
            int SLen = strlen( LPS );
            ///
            int W_WIN = 8*SLen + 10;
            int H_WIN = 140;
            MoveWindow( hDlg, 1, 1, W_WIN, H_WIN, TRUE );
            ///
            int WST = W_WIN - 12;
            int HST = 20;
            MoveWindow( GetDlgItem(hDlg, 100), 1, 1, WST, HST, TRUE );
            ///
            int WBUT = 120;
            int HBUT = 30;
            MoveWindow( GetDlgItem(hDlg, IDYES), W_WIN - WBUT - 8, H_WIN - HBUT - 30, WBUT, HBUT, TRUE );
            ///
            SetWindowTextA( GetDlgItem(hDlg, IDYES), "Aceptar" );
            SetWindowTextA( GetDlgItem(hDlg, 100), (char*)lParam );
            CenterWindow( hDlg, 0, 0, NULL );
        }
        return TRUE;
        break;
        ///
        ///
        ///
        ///
        case WM_COMMAND:
            if( LOWORD(wParam)==IDCANCEL ||
                        LOWORD(wParam)==IDOK ||
                        LOWORD(wParam)==IDYES
                                ){
                EndDialog( hDlg, FALSE );
            }
            return TRUE;
        break;
        ///
        ///
        ///
        ///
    }
    return FALSE;
}
/// //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CALLBACK SELECT_KERNELS(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam){
    static HWND HW_INFO2 = NULL;
    static HWND HW_PIC_O = NULL;
    static HBITMAP HBMP_P = NULL;
    static RGBQUAD* RgbI = NULL;
    static RGBQUAD* RgbO = NULL;
    static DIM2* D2PTR = NULL;
    static int W_OUT = 0;
    static int H_OUT = 0;
    static int K3x3H[9] = {0};
    static int K3x3V[9] = {0};
    static char OffSets[16] = {0};
    char T2T[1024] = {0};
    switch(msg){
        case WM_INITDIALOG:{
            D2PTR = (DIM2*)lParam;
            if( D2PTR==NULL ){
                CenterWindow( hDlg, 0, 0 );return FALSE;
            }
            HW_INFO2 = GetDlgItem( hDlg, 10 );
            HW_PIC_O = GetDlgItem( hDlg, 101 );
            SetWindowText( HW_INFO2, T2T );
            // SendDlgItemMessage(hDlg, ID_SCROLLV, SBM_SETRANGE, (WPARAM)0, (LPARAM)50);
            ///
            ///
            ///
            HBITMAP BMSRC = NULL;
            if( D2PTR->GetInput()->IsLoaded &&
                                    D2PTR->GetInput()->ProcType!=NO_PROC_Signal_2D ){
                D2PTR->GetInput()->ReadDatas();

                if( D2PTR->GetInput()->ProcType==BW_LABEL ){
                    int NPosBuffT = D2PTR->GetInput()->WPIC_OUT * D2PTR->GetInput()->HPIC_OUT;
                    for( int i=0; i<(int)NPosBuffT; i++ ){
                        D2PTR->GetInput()->RgbBuff[i].rgbRed   =
                        D2PTR->GetInput()->RgbBuff[i].rgbGreen =
                        D2PTR->GetInput()->RgbBuff[i].rgbBlue  = 255 * (int)((D2PTR->GetInput()->RgbBuff[i].rgbRed!=255) || (D2PTR->GetInput()->RgbBuff[i].rgbGreen!=255) ||
                                                        (D2PTR->GetInput()->RgbBuff[i].rgbBlue!=255));
                    }
                }

                BMSRC = D2PTR->NewBitmap( D2PTR->GetInput()->WPIC_OUT, D2PTR->GetInput()->HPIC_OUT );
                SetBitmapBits( BMSRC, 4 * D2PTR->GetInput()->WPIC_OUT * D2PTR->GetInput()->HPIC_OUT,
                               D2PTR->GetInput()->RgbBuff );
                D2PTR->GetInput()->FreeBuff();
            }else{
                D2PTR->D2_SOURCE->ReadDatas();
                BMSRC = D2PTR->NewBitmap( D2PTR->D2_SOURCE->WPIC_OUT, D2PTR->D2_SOURCE->HPIC_OUT );
                SetBitmapBits( BMSRC, 4 * D2PTR->D2_SOURCE->WPIC_OUT * D2PTR->D2_SOURCE->HPIC_OUT,
                               D2PTR->D2_SOURCE->RgbBuff );
                D2PTR->D2_SOURCE->FreeBuff();
            }
            ///
            ///
            if( D2PTR->NOrd==0 ){
                D2PTR->NOrd = 1;
            }
            ///
            ///
            ///
            /*
            HBITMAP BMSRC = D2PTR->GetInput()->GetPicPlot();
            if( BMSRC==NULL ){
                BMSRC = D2PTR->D2_SOURCE->GetPicPlot();
            }
            //*/
            ///
            ///
            ///
            ///
            BITMAP BmpI = {0};
            GetObject( BMSRC, sizeof(BITMAP) , (LPSTR)&BmpI );
            ///
            int WP = 0;
            int HP = 0;
            ///
            WindowSize( hDlg, &WP, &HP );
            ///
            ///
            ///
            ///
            WindowSize( HW_PIC_O, &WP, &HP );
            ///
            W_OUT = WP;
            H_OUT = (int)( round( (W_OUT * BmpI.bmHeight)/((double)BmpI.bmWidth) ) );
            if( H_OUT>640 ){
                H_OUT = HP;
                W_OUT =(int)( round( (H_OUT * BmpI.bmWidth)/((double)BmpI.bmHeight) ) );
                /// MoveWindow( HW_PIC_O, 1, 66, WP, HP, TRUE );
            }else{
                const int Y_P_PIC = 80;
                MoveWindow( hDlg, 1, 66, W_OUT+8, H_OUT + (Y_P_PIC+33), TRUE );
                MoveWindow( HW_PIC_O, 1, Y_P_PIC, W_OUT, H_OUT, TRUE );
            }
            ///
            ///
            ///
            HDC ODC = GetDC( 0 );
            HBMP_P = CreateCompatibleBitmap( ODC , W_OUT, H_OUT );
            ReleaseDC( 0, ODC );
            DeleteDC( ODC );
            ///
            HDC hdc = CreateCompatibleDC( GetDC( 0 ) );
            HDC memDC = CreateCompatibleDC( GetDC( 0 ) );
            SelectObject( hdc , BMSRC );/// HBPLOT
            SelectObject( memDC , HBMP_P );/// HB_I
            SetStretchBltMode( memDC , HALFTONE );/// COLORONCOLOR  HALFTONE
            StretchBlt( memDC, 0, 0, W_OUT, H_OUT, hdc, 0, 0, BmpI.bmWidth, BmpI.bmHeight, SRCCOPY );
            ReleaseDC( NULL, hdc );
            ReleaseDC( NULL, memDC );
            DeleteDC( hdc );
            DeleteDC( memDC );
            ///
            DeleteObject( BMSRC );
            ///
            RgbI = (RGBQUAD*)malloc( 4 * W_OUT * H_OUT +1024 );
            ZeroMemory( RgbI, 4 * W_OUT * H_OUT );
            RgbO = (RGBQUAD*)malloc( 4 * W_OUT * H_OUT +1024 );
            ZeroMemory( RgbO, 4 * W_OUT * H_OUT );
            ///
            GetBitmapBits( HBMP_P, 4 * W_OUT * H_OUT, RgbI );
            ///
            for( int i=0; i<(W_OUT * H_OUT); i++ ){
                RgbI[i].rgbRed =
                RgbI[i].rgbGreen =
                RgbI[i].rgbBlue =
                            (BYTE)round(
                                    0.2989 * RgbI[i].rgbRed +
                                    0.5870 * RgbI[i].rgbGreen +
                                    0.1140 * RgbI[i].rgbBlue
                                    );
            }
            ///
            ///
            ///SetBitmapBits( HBMP_P, 4 * W_OUT * H_OUT, RgbO );
            ///SendMessage( HW_PIC_O, STM_SETIMAGE, IMAGE_BITMAP,(LPARAM)HBMP_P );
            /// sprintf( T2T, "%ix%i", WP, HP );SetWindowTextA( hDlg, T2T );
            ///
            ///
            CenterWindow( hDlg, 0, 0 );
            ///
            ///
            memcpy( OffSets, D2PTR->Kernel3x3_Offset, 16 );
            ///
            ///
            for( int i=-128; i<128; i++ ){
                sprintf( T2T, "%i", i );
                SendDlgItemMessage( hDlg, 99, CB_ADDSTRING, 0, (LPARAM)T2T );
            }
            SendDlgItemMessage( hDlg, 99, CB_SETCURSEL, OffSets[D2PTR->Kernel3x3_Type]+128, 0 );
            ///
            SendDlgItemMessage( hDlg, 100, CB_ADDSTRING, 0, (LPARAM)"Blur" );
            SendDlgItemMessage( hDlg, 100, CB_ADDSTRING, 0, (LPARAM)"Emboss" );
            SendDlgItemMessage( hDlg, 100, CB_ADDSTRING, 0, (LPARAM)"Sharpen" );
            SendDlgItemMessage( hDlg, 100, CB_ADDSTRING, 0, (LPARAM)"Smooth" );
            SendDlgItemMessage( hDlg, 100, CB_ADDSTRING, 0, (LPARAM)"MeanRenoval" );
            SendDlgItemMessage( hDlg, 100, CB_ADDSTRING, 0, (LPARAM)"Pasa Bajos(Media)" );
            SendDlgItemMessage( hDlg, 100, CB_ADDSTRING, 0, (LPARAM)"Pasa Bajos(Ponderado)" );
            SendDlgItemMessage( hDlg, 100, CB_ADDSTRING, 0, (LPARAM)"Pasa Altos(Media)" );
            SendDlgItemMessage( hDlg, 100, CB_ADDSTRING, 0, (LPARAM)"Pasa Altos(Laplace)" );
            SendDlgItemMessage( hDlg, 100, CB_ADDSTRING, 0, (LPARAM)"Pasa Altos(Laplace Menos)" );
            SendDlgItemMessage( hDlg, 100, CB_ADDSTRING, 0, (LPARAM)"Sobel" );
            SendDlgItemMessage( hDlg, 100, CB_ADDSTRING, 0, (LPARAM)"Prewitt" );
            SendDlgItemMessage( hDlg, 100, CB_ADDSTRING, 0, (LPARAM)"Kirsh" );
            SendDlgItemMessage( hDlg, 100, CB_ADDSTRING, 0, (LPARAM)"Usuario" );
            SendDlgItemMessage( hDlg, 100, CB_ADDSTRING, 0, (LPARAM)"Average" );
            SendDlgItemMessage( hDlg, 100, CB_SETCURSEL, D2PTR->Kernel3x3_Type, 0 );
            ///
            ///
            memcpy( RgbO, RgbI, 4 * W_OUT * H_OUT );
            ///
            ShowWindow( GetDlgItem(hDlg, 63), 0 );
            ShowWindow( GetDlgItem(hDlg, 64), 0 );
            ///
            if( D2PTR->Kernel3x3_Type==USUARIO ){
                memcpy( K3x3H, D2PTR->Kernel3x3User, 9*sizeof(int) );
            }
            sprintf( T2T, "%i", D2PTR->NOrd );
            SetWindowTextA( GetDlgItem(hDlg, 64), T2T );
            if( D2PTR->Kernel3x3_Type==AVERAGE ){
                DSP_MNG.FSPECIAL_AVERAGE( RgbO, W_OUT, H_OUT, D2PTR->NOrd,
                                         SendDlgItemMessage( hDlg, 99, CB_GETCURSEL, 0, 0 )-128 );
                ShowWindow( GetDlgItem(hDlg, 63), 1 );
                ShowWindow( GetDlgItem(hDlg, 64), 1 );
            }else{
                D2PTR->CONV3x3_TRY( RgbO, W_OUT, H_OUT, K3x3H, K3x3V,
                            SendDlgItemMessage( hDlg, 100, CB_GETCURSEL, 0, 0 ),
                            SendDlgItemMessage( hDlg, 99, CB_GETCURSEL, 0, 0 )-128, D2PTR->BSobel_Remove );
            }
            ///
            ShowWindow( GetDlgItem(hDlg, 2000), ((int)D2PTR->Kernel3x3_Type>=SOBEL) && ((int)D2PTR->Kernel3x3_Type<=PREWITT) );
            SendMessage( GetDlgItem(hDlg, 2000), BM_SETCHECK, D2PTR->BSobel_Remove, 0);
            ///
            ///
            ///
            ///
            SetBitmapBits( HBMP_P, 4 * W_OUT * H_OUT, RgbO );
            SendMessage( HW_PIC_O, STM_SETIMAGE, IMAGE_BITMAP,(LPARAM)HBMP_P );
            ///
            for( int i=0; i<9; i++ ){
                EnableWindow( GetDlgItem(hDlg, 256+i), D2PTR->Kernel3x3_Type==USUARIO );
            }
            ///
            if( !(D2PTR->Kernel3x3_Type!=SOBEL &&
                    D2PTR->Kernel3x3_Type!=PREWITT &&
                    D2PTR->Kernel3x3_Type!=KIRSH &&
                    D2PTR->Kernel3x3_Type!=AVERAGE)
                                                            ){
                ShowWindow( GetDlgItem(hDlg, 255), 0 );
                for( int i=0; i<9; i++ ){
                    ShowWindow( GetDlgItem(hDlg, 256+i), 0 );
                }
            }else{
                ShowWindow( GetDlgItem(hDlg, 255), 1 );
                for( int i=0; i<9; i++ ){
                    ShowWindow( GetDlgItem(hDlg, 256+i), 1 );
                    sprintf( T2T, "%i", K3x3H[i] );
                    SetWindowTextA( GetDlgItem(hDlg, 256+i), T2T );
                    EnableWindow( GetDlgItem(hDlg, 256+i), D2PTR->Kernel3x3_Type==USUARIO );
                }
            }
            ///
            }
            return TRUE;
        case WM_COMMAND:
            if( LOWORD(wParam)==IDCANCEL && D2PTR==NULL ){
                EndDialog( hDlg, FALSE );
            }
            if( D2PTR==NULL ){
                return FALSE;
            }
            ///
            ///
            if( LOWORD(wParam)==64 ){
                if( HIWORD( wParam )==EN_UPDATE || HIWORD( wParam )==EN_KILLFOCUS ){
                    GetWindowTextA( GetDlgItem(hDlg, 64), T2T, 15 );
                    if( HIWORD( wParam )==EN_KILLFOCUS ){
                        if( T2T[0]==0 ){
                            sprintf( T2T, "%i", D2PTR->NOrd );
                            SetWindowTextA( GetDlgItem(hDlg, 64), T2T );
                        }
                    }else{
                        if( T2T[0]!=0 ){
                            if( atoi(T2T)==0 ){
                                sprintf( T2T, "%i", D2PTR->NOrd );
                                SetWindowTextA( GetDlgItem(hDlg, 64), T2T );
                                break;
                            }
                            if( atoi(T2T)!=D2PTR->NOrd ){
                                memcpy( RgbO, RgbI, 4 * W_OUT * H_OUT );
                                D2PTR->NOrd = atoi(T2T);
                                int OffOpt = SendDlgItemMessage( hDlg, 99, CB_GETCURSEL, 0, 0 );
                                DSP_MNG.FSPECIAL_AVERAGE( RgbO, W_OUT, H_OUT, D2PTR->NOrd, OffOpt-128 );
                                SetBitmapBits( HBMP_P, 4 * W_OUT * H_OUT, RgbO );
                                SendMessage( HW_PIC_O, STM_SETIMAGE, IMAGE_BITMAP,(LPARAM)HBMP_P );
                            }
                        }
                    }
                }
                break;
            }
            ///
            ///
            if( LOWORD(wParam)==2000 ){
                D2PTR->BSobel_Remove = (int)( SendMessage( GetDlgItem(hDlg, 2000), BM_GETCHECK, 0, 0)==BST_CHECKED );
                int KOpt   = SendDlgItemMessage( hDlg, 100, CB_GETCURSEL, 0, 0 );
                int OffOpt = SendDlgItemMessage( hDlg, 99, CB_GETCURSEL, 0, 0 );
                memcpy( RgbO, RgbI, 4 * W_OUT * H_OUT );
                D2PTR->CONV3x3_TRY( RgbO, W_OUT, H_OUT, K3x3H, K3x3V, KOpt, OffOpt-128, D2PTR->BSobel_Remove );
                SetBitmapBits( HBMP_P, 4 * W_OUT * H_OUT, RgbO );
                SendMessage( HW_PIC_O, STM_SETIMAGE, IMAGE_BITMAP,(LPARAM)HBMP_P );
                break;
            }
            ///
            ///
            ///
            ///
            if( LOWORD(wParam)>=256 ){
                int KOpt   = SendDlgItemMessage( hDlg, 100, CB_GETCURSEL, 0, 0 );
                if( KOpt!=USUARIO ){
                    break;
                }
                int OffOpt = SendDlgItemMessage( hDlg, 99, CB_GETCURSEL, 0, 0 );
                int ID_CTRL = LOWORD(wParam) - 256;
                if( HIWORD( wParam )==EN_UPDATE || HIWORD( wParam )==EN_KILLFOCUS ){
                    ParseEditInt( wParam, lParam );
                    GetWindowTextA( GetDlgItem(hDlg, LOWORD(wParam)), T2T, 15 );
                    if( HIWORD( wParam )==EN_KILLFOCUS && T2T[0]==0 ){
                        sprintf( T2T, "%i", D2PTR->Kernel3x3User[ID_CTRL] );
                        SetWindowTextA( GetDlgItem(hDlg, LOWORD(wParam)), T2T );
                        memcpy( RgbO, RgbI, 4 * W_OUT * H_OUT );
                        D2PTR->CONV3x3_TRY( RgbO, W_OUT, H_OUT, K3x3H, K3x3V, KOpt, OffOpt-128, D2PTR->BSobel_Remove );
                        SetBitmapBits( HBMP_P, 4 * W_OUT * H_OUT, RgbO );
                        SendMessage( HW_PIC_O, STM_SETIMAGE, IMAGE_BITMAP,(LPARAM)HBMP_P );
                        break;
                    }
                    if( T2T[0]!=0 ){
                        D2PTR->Kernel3x3User[ID_CTRL] = atoi( T2T );
                        memcpy( K3x3H, D2PTR->Kernel3x3User, 9*sizeof(int) );
                        memcpy( RgbO, RgbI, 4 * W_OUT * H_OUT );
                        D2PTR->CONV3x3_TRY( RgbO, W_OUT, H_OUT, K3x3H, K3x3V, KOpt, OffOpt-128, D2PTR->BSobel_Remove );
                        SetBitmapBits( HBMP_P, 4 * W_OUT * H_OUT, RgbO );
                        SendMessage( HW_PIC_O, STM_SETIMAGE, IMAGE_BITMAP,(LPARAM)HBMP_P );
                    }
                }
                break;
            }
            ///
            ///
            switch( LOWORD(wParam) ){
                case 99:
                    if( HIWORD(wParam)==CBN_SELENDOK ){
                        OffSets[SendDlgItemMessage( hDlg, 100, CB_GETCURSEL, 0, 0 )] =
                                    SendDlgItemMessage( hDlg, 99, CB_GETCURSEL, 0, 0 ) - 128;
                    }else{
                        break;
                    }
                case 100:{
                    if( HIWORD(wParam)==CBN_SELENDOK ){
                        int KOpt   = SendDlgItemMessage( hDlg, 100, CB_GETCURSEL, 0, 0 );
                        ShowWindow( GetDlgItem(hDlg, 2000), ((int)KOpt==SOBEL) );
                        ShowWindow( GetDlgItem(hDlg, 2000), ((int)KOpt>=SOBEL) && ((int)KOpt<=PREWITT) );
                        int OffOpt = SendDlgItemMessage( hDlg, 99, CB_GETCURSEL, 0, 0 );
                        if( LOWORD(wParam)==100 ){
                            SendDlgItemMessage( hDlg, 99, CB_SETCURSEL, OffSets[KOpt]+128, 0 );
                            OffOpt = SendDlgItemMessage( hDlg, 99, CB_GETCURSEL, 0, 0 );
                            if(KOpt==USUARIO){
                                memcpy( K3x3H, D2PTR->Kernel3x3User, 9*sizeof(int) );
                            }
                        }
                        memcpy( RgbO, RgbI, 4 * W_OUT * H_OUT );
                        if(KOpt==AVERAGE){
                            ShowWindow( GetDlgItem(hDlg, 63), 1 );
                            ShowWindow( GetDlgItem(hDlg, 64), 1 );
                            DSP_MNG.FSPECIAL_AVERAGE( RgbO, W_OUT, H_OUT, D2PTR->NOrd, OffOpt-128 );
                        }else{
                            ShowWindow( GetDlgItem(hDlg, 63), 0 );
                            ShowWindow( GetDlgItem(hDlg, 64), 0 );
                            D2PTR->CONV3x3_TRY( RgbO, W_OUT, H_OUT, K3x3H, K3x3V, KOpt, OffOpt-128, D2PTR->BSobel_Remove );
                        }
                        SetBitmapBits( HBMP_P, 4 * W_OUT * H_OUT, RgbO );
                        SendMessage( HW_PIC_O, STM_SETIMAGE, IMAGE_BITMAP,(LPARAM)HBMP_P );
                        if( !(KOpt!=SOBEL &&
                                KOpt!=PREWITT &&
                                KOpt!=KIRSH &&
                                KOpt!=AVERAGE)
                                                                        ){
                        //if( KOpt>9 && KOpt<13 ){
                            ShowWindow( GetDlgItem(hDlg, 255), 0 );
                            for( int i=0; i<9; i++ ){
                                ShowWindow( GetDlgItem(hDlg, 256+i), 0 );
                            }
                        }else{
                            ShowWindow( GetDlgItem(hDlg, 255), 1 );
                            for( int i=0; i<9; i++ ){
                                ShowWindow( GetDlgItem(hDlg, 256+i), 1 );
                                sprintf( T2T, "%i", K3x3H[i] );
                                SetWindowTextA( GetDlgItem(hDlg, 256+i), T2T );
                                EnableWindow( GetDlgItem(hDlg, 256+i), KOpt==USUARIO );
                            }
                        }
                    }
                }
                    break;
                case IDCANCEL:
                case IDOK:
                    D2PTR->Kernel3x3_Type = (int)SendDlgItemMessage( hDlg, 100, CB_GETCURSEL, 0, 0 );
                    memcpy( D2PTR->Kernel3x3_Offset, OffSets, 16 );
                    if( D2PTR->Kernel3x3_Type==USUARIO ){
                        memcpy( D2PTR->Kernel3x3User, K3x3H, 9*sizeof(int) );
                    }
                    free( RgbI );
                    free( RgbO );
                    D2PTR = NULL;
                    DeleteObject( HBMP_P);
                    EndDialog( hDlg, TRUE );
                    break;
                    free( RgbI );
                    free( RgbO );
                    DeleteObject( HBMP_P);
                    D2PTR = NULL;
                    EndDialog( hDlg, FALSE );
                    break;
            }
            return TRUE;
        case WM_CLOSE:
            free( RgbI );
            free( RgbO );
            DeleteObject( HBMP_P);
            EndDialog( hDlg, TRUE );
            break;

    }
    return FALSE;
}
/// //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CALLBACK SELECT_REDIM(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam){
    static HWND HW_INFO2 = NULL;
    static HWND HW_PIC_O = NULL;
    static HBITMAP HBMP_O = NULL;
    static HBITMAP HBMP_R = NULL;
    static HBITMAP HBMP_P = NULL;
    static DIM2* D2PTR = NULL;
    static int W_OR = 0;
    static int H_OR = 0;
    static int W_OUT = 0;
    static int H_OUT = 0;
    char T2T[1024] = {0};
    switch(msg){
        case WM_INITDIALOG:{
            D2PTR = (DIM2*)lParam;
            if( D2PTR==NULL ){
                CenterWindow( hDlg, 0, 0 );return FALSE;
            }
            SetFocus( GetDlgItem( hDlg, IDOK ) );
            HW_INFO2 = GetDlgItem( hDlg, 64 );
            /// D2PTR->WPIC_IN = D2PTR->HPIC_IN = 9999;
            sprintf( T2T, "Dimensiones originales(WxH):    %4d    x    %4d", D2PTR->WPIC_IN, D2PTR->HPIC_IN );
            SetWindowTextA( HW_INFO2, T2T );
            ///
            if( D2PTR->WPIC_REDIM==0 ){
                D2PTR->WPIC_REDIM = D2PTR->WPIC_IN;
            }
            sprintf( T2T, "%i", D2PTR->WPIC_REDIM );
            SetWindowTextA( GetDlgItem(hDlg,256), T2T );
            //
            if( D2PTR->HPIC_REDIM==0 ){
                D2PTR->HPIC_REDIM = D2PTR->HPIC_IN;
            }
            sprintf( T2T, "%i", D2PTR->HPIC_REDIM );
            SetWindowTextA( GetDlgItem(hDlg,257), T2T );
            ///
            SendMessageA( GetDlgItem(hDlg, 512), BM_SETCHECK, D2PTR->RELASP * BST_CHECKED, 0 );
            ///
            ///
            HW_PIC_O = GetDlgItem( hDlg, 101 );
            // SendDlgItemMessage(hDlg, ID_SCROLLV, SBM_SETRANGE, (WPARAM)0, (LPARAM)50);
            ///
            ///
            if( D2PTR->GetInput()->IsLoaded &&
                    D2PTR->GetInput()->ProcType!=NO_PROC_Signal_2D ){
                D2PTR->GetInput()->ReadDatas();

                if( D2PTR->GetInput()->ProcType==BW_LABEL ){
                    int NPosBuffT = D2PTR->GetInput()->WPIC_OUT * D2PTR->GetInput()->HPIC_OUT;
                    for( int i=0; i<(int)NPosBuffT; i++ ){
                        D2PTR->GetInput()->RgbBuff[i].rgbRed   =
                        D2PTR->GetInput()->RgbBuff[i].rgbGreen =
                        D2PTR->GetInput()->RgbBuff[i].rgbBlue  = 255 * (int)((D2PTR->GetInput()->RgbBuff[i].rgbRed!=255) ||
                                                                                (D2PTR->GetInput()->RgbBuff[i].rgbGreen!=255) ||
                                                        (D2PTR->GetInput()->RgbBuff[i].rgbBlue!=255));
                    }
                }

                HBMP_O = D2PTR->NewBitmap( D2PTR->GetInput()->WPIC_OUT, D2PTR->GetInput()->HPIC_OUT );
                SetBitmapBits( HBMP_O, 4 * D2PTR->GetInput()->WPIC_OUT * D2PTR->GetInput()->HPIC_OUT,
                               D2PTR->GetInput()->RgbBuff );
                D2PTR->GetInput()->FreeBuff();
                W_OR = D2PTR->GetInput()->WPIC_OUT;
                H_OR = D2PTR->GetInput()->HPIC_OUT;
            }else{
                D2PTR->D2_SOURCE->ReadDatas();
                HBMP_O = D2PTR->NewBitmap( D2PTR->D2_SOURCE->WPIC_OUT, D2PTR->D2_SOURCE->HPIC_OUT );
                SetBitmapBits( HBMP_O, 4 * D2PTR->D2_SOURCE->WPIC_OUT * D2PTR->D2_SOURCE->HPIC_OUT,
                               D2PTR->D2_SOURCE->RgbBuff );
                HBMP_R = D2PTR->Redim_PIC( HBMP_O, D2PTR->WPIC_REDIM, D2PTR->HPIC_REDIM );
                DeleteObject( HBMP_O );
                HBMP_O = HBMP_R;
                D2PTR->D2_SOURCE->FreeBuff();
                W_OR = D2PTR->D2_SOURCE->WPIC_OUT;
                H_OR = D2PTR->D2_SOURCE->HPIC_OUT;
            }
            ///
            ///
            ///
            //WindowSize( hDlg, &WP, &HP );
            ///
            ///
            ///
            ///

            ///
            HBMP_R = D2PTR->Redim_PIC( HBMP_O, D2PTR->WPIC_REDIM, D2PTR->HPIC_REDIM );
            int WP = 0;
            int HP = 0;
            WindowSize( HW_PIC_O, &WP, &HP );
            W_OUT = WP;
            H_OUT = (int)( round( (W_OUT * D2PTR->HPIC_REDIM)/((double)D2PTR->WPIC_REDIM) ) );
            const int Y_P_PIC = 64;
            if( H_OUT>640 ){
                H_OUT = HP;
                W_OUT =(int)( round( (H_OUT * D2PTR->WPIC_REDIM)/((double)D2PTR->HPIC_REDIM) ) );
                MoveWindow( hDlg, 1, 66, WP+10, H_OUT + (Y_P_PIC+33), TRUE );
                MoveWindow( HW_PIC_O, 1, Y_P_PIC, WP+2, H_OUT, TRUE );
            }else{
                MoveWindow( hDlg, 1, 66, W_OUT+10, H_OUT + (Y_P_PIC+33), TRUE );
                MoveWindow( HW_PIC_O, 1, Y_P_PIC, W_OUT+2, H_OUT, TRUE );
            }
            HBMP_P = D2PTR->Redim_PIC( HBMP_R, W_OUT, H_OUT );
            SendMessage( HW_PIC_O, STM_SETIMAGE, IMAGE_BITMAP,(LPARAM)HBMP_P );
            ///
            ///
            ///
            ///
            ///
            ///         O  L  D
            /*
            HBMP_R = D2PTR->Redim_PIC( HBMP_O, D2PTR->WPIC_REDIM, D2PTR->HPIC_REDIM );
            int WP = 0;
            int HP = 0;
            WindowSize( HW_PIC_O, &WP, &HP );
            W_OUT = WP;
            H_OUT = (int)( round( (W_OUT * D2PTR->HPIC_REDIM)/((double)D2PTR->WPIC_REDIM) ) );
            if( H_OUT>HP ){
                H_OUT = HP;
                W_OUT =(int)( round( (H_OUT * D2PTR->WPIC_REDIM)/((double)D2PTR->HPIC_REDIM) ) );
            }
            HBMP_P = D2PTR->Redim_PIC( HBMP_R, W_OUT, H_OUT );
            SendMessage( HW_PIC_O, STM_SETIMAGE, IMAGE_BITMAP,(LPARAM)HBMP_P );
            //*/
            ///
            ///
            ///
            ///
            ///
            ///
            ///
            ///
            ///
            CenterWindow( hDlg, 0, 0 );
            ///
            ///
            ///
            }
            return TRUE;
        case WM_COMMAND:
            if( LOWORD(wParam)==IDCANCEL && D2PTR==NULL ){
                EndDialog( hDlg, FALSE );
            }
            if( D2PTR==NULL ){
                return FALSE;
            }
            ///
            ///
            switch( LOWORD(wParam) ){
                case 512:{
                    D2PTR->RELASP = SendMessageA( GetDlgItem(hDlg, 512), BM_GETCHECK, 0, 0 )==BST_CHECKED;
                }
                    break;
                ///
                ///
                ///
                ///
                case 256:{
                    if( HIWORD(wParam)==EN_KILLFOCUS ){
                        GetWindowTextA( GetDlgItem(hDlg,256), T2T, 8 );
                        if( T2T[0]==0 || atoi(T2T)==0 ){
                            sprintf( T2T, "%i", D2PTR->WPIC_REDIM );
                            SetWindowTextA( GetDlgItem(hDlg,256), T2T );
                        }
                        break;
                    }
                    if( GetFocus()!=GetDlgItem(hDlg,256) ){
                        break;
                    }
                    if( HIWORD(wParam)==EN_CHANGE ){
                        /// MessageBoxA( hDlg, "", "", 0 );
                        GetWindowTextA( (HWND)lParam, T2T, 8 );
                        if( T2T[0]!=0 ){
                            if( atoi(T2T)==0 ){
                                sprintf( T2T, "%i", D2PTR->WPIC_REDIM );
                                //SetWindowTextA( (HWND)lParam, T2T );
                            }else{
                                D2PTR->WPIC_REDIM = atoi(T2T);
                                if( D2PTR->RELASP ){
                                    D2PTR->HPIC_REDIM =
                                        (int)round( D2PTR->WPIC_REDIM *
                                        ( H_OR/((double)W_OR) ) );
                                    if( D2PTR->HPIC_REDIM<1 ){
                                        D2PTR->HPIC_REDIM = 1;
                                    }
                                    if( D2PTR->HPIC_REDIM>9999 ){
                                        D2PTR->HPIC_REDIM = 9999;
                                    }
                                    sprintf( T2T, "%i", D2PTR->HPIC_REDIM );
                                    SetWindowTextA( GetDlgItem(hDlg, 257), T2T );
                                }
                                ///

            HBMP_R = D2PTR->Redim_PIC( HBMP_O, D2PTR->WPIC_REDIM, D2PTR->HPIC_REDIM );
            int WP = 0;
            int HP = 0;
            WindowSize( HW_PIC_O, &WP, &HP );
            W_OUT = WP;
            H_OUT = (int)( round( (W_OUT * D2PTR->HPIC_REDIM)/((double)D2PTR->WPIC_REDIM) ) );
            if( H_OUT>HP ){
                H_OUT = HP;
                W_OUT =(int)( round( (H_OUT * D2PTR->WPIC_REDIM)/((double)D2PTR->HPIC_REDIM) ) );
            }
            HBMP_P = D2PTR->Redim_PIC( HBMP_R, W_OUT, H_OUT );
            SendMessage( HW_PIC_O, STM_SETIMAGE, IMAGE_BITMAP,(LPARAM)HBMP_P );

                                ///
                            }
                        }
                    }
                }
                    break;
                ///
                ///
                ///
                ///
                ///
                ///
                ///
                ///
                case 257:{
                    if( HIWORD(wParam)==EN_KILLFOCUS ){
                        GetWindowTextA( GetDlgItem(hDlg,257), T2T, 8 );
                        if( T2T[0]==0 || atoi(T2T)==0 ){
                            sprintf( T2T, "%i", D2PTR->HPIC_REDIM );
                            //SetWindowTextA( GetDlgItem(hDlg,257), T2T );
                        }
                        break;
                    }
                    if( GetFocus()!=GetDlgItem(hDlg,257) ){
                        break;
                    }
                    if( HIWORD(wParam)==EN_CHANGE ){
                        /// MessageBoxA( hDlg, "", "", 0 );
                        GetWindowTextA( (HWND)lParam, T2T, 8 );
                        if( T2T[0]!=0 ){
                            if( atoi(T2T)==0 ){
                                sprintf( T2T, "%i", D2PTR->HPIC_REDIM );
                                SetWindowTextA( (HWND)lParam, T2T );
                            }else{
                                D2PTR->HPIC_REDIM = atoi(T2T);
                                if( D2PTR->RELASP ){
                                    D2PTR->WPIC_REDIM =
                                        (int)round( D2PTR->HPIC_REDIM *
                                        ( W_OR/((double)H_OR) ) );
                                    if( D2PTR->WPIC_REDIM<1 ){
                                        D2PTR->WPIC_REDIM = 1;
                                    }
                                    if( D2PTR->WPIC_REDIM>9999 ){
                                        D2PTR->WPIC_REDIM = 9999;
                                    }
                                    sprintf( T2T, "%i", D2PTR->WPIC_REDIM );
                                    SetWindowTextA( GetDlgItem(hDlg, 256), T2T );
                                }
                                ///
            HBMP_R = D2PTR->Redim_PIC( HBMP_O, D2PTR->WPIC_REDIM, D2PTR->HPIC_REDIM );
            int WP = 0;
            int HP = 0;
            WindowSize( HW_PIC_O, &WP, &HP );
            W_OUT = WP;
            H_OUT = (int)( round( (W_OUT * D2PTR->HPIC_REDIM)/((double)D2PTR->WPIC_REDIM) ) );
            if( H_OUT>HP ){
                H_OUT = HP;
                W_OUT =(int)( round( (H_OUT * D2PTR->WPIC_REDIM)/((double)D2PTR->HPIC_REDIM) ) );
            }
            HBMP_P = D2PTR->Redim_PIC( HBMP_R, W_OUT, H_OUT );
            SendMessage( HW_PIC_O, STM_SETIMAGE, IMAGE_BITMAP,(LPARAM)HBMP_P );
                                ///
                            }
                        }
                    }
                }
                    break;
                ///
                ///
                ///
                ///
                case IDCANCEL:
                case IDOK:
                    D2PTR = NULL;
                    DeleteObject( HBMP_O );
                    DeleteObject( HBMP_R );
                    DeleteObject( HBMP_P );
                    EndDialog( hDlg, TRUE );
                    break;
            }
            return TRUE;
        case WM_CLOSE:
            DeleteObject( HBMP_P);
            EndDialog( hDlg, TRUE );
            break;

    }
    return FALSE;
}
/// //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CALLBACK SELECT_ARITH(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam){
    static HWND HW_INFO2 = NULL;
    static HWND HW_PIC_O = NULL;
    static HBITMAP HBMP_P = NULL;
    static RGBQUAD* RgbO = NULL;
    static RGBQUAD* RgbOut = NULL;
    static DIM2* D2PTR = NULL;
    static DIM2* D2PTR_T = NULL;
    static int W_OUT = 0;
    static int H_OUT = 0;
    char T2T[1024] = {0};
    /// D2PTR = D2PTR_T;
    switch(msg){
        case WM_SHOWWINDOW:{
            D2PTR = D2PTR_T;
            for( int i=0; i<8; i++ ){
                if( D2PTR->ArithType==i ){
                    ///SendDlgItemMessage(hDlg, I200, BM_SETCHECK, (WPARAM)BST_CHECKED, 0 );
                    EnableWindow( GetDlgItem(hDlg, 100+i ), TRUE );
                    EnableWindow( GetDlgItem(hDlg,  50+i ), TRUE );
                }else{
                    ///SendDlgItemMessage(hDlg, I200, BM_SETCHECK, (WPARAM)BST_UNCHECKED, 0 );
                    EnableWindow( GetDlgItem(hDlg, 100+i ), FALSE );
                    EnableWindow( GetDlgItem(hDlg, 50+i ), FALSE );
                }
            }
            SetFocus( GetDlgItem(hDlg, 200+D2PTR->ArithType) );
        }
            break;
        case WM_INITDIALOG:{
            D2PTR_T = (DIM2*)lParam;
            if( D2PTR_T==NULL ){
                CenterWindow( hDlg, 0, 0 );return FALSE;
            }
            ///
            ///
            ///
            ///
            ///
            HW_INFO2 = GetDlgItem( hDlg, 10 );
            HW_PIC_O = GetDlgItem( hDlg, 1001 );
            SetWindowText( HW_INFO2, T2T );
            // SendDlgItemMessage(hDlg, ID_SCROLLV, SBM_SETRANGE, (WPARAM)0, (LPARAM)50);
            ///
            ///
            sprintf( T2T, "%i", D2PTR_T->SumOper );
            SetWindowTextA( GetDlgItem(hDlg, 100), T2T );
            ///
            sprintf( T2T, "%g", D2PTR_T->ProdOper );
            SetWindowTextA( GetDlgItem(hDlg, 101), T2T );
            ///
            sprintf( T2T, "%i", D2PTR_T->LowValue );
            SetWindowTextA( GetDlgItem(hDlg, 102), T2T );
            ///
            sprintf( T2T, "%i", D2PTR_T->HiValue );
            SetWindowTextA( GetDlgItem(hDlg, 52), T2T );
            ///
            ///
            SendMessageA( GetDlgItem(hDlg, 512), BM_SETCHECK, D2PTR_T->InvOut, 0 );
            ///
            ///
            //D2PTR_T->ArithType;
            //D2PTR_T->ARITH_PROCS();
            ///
            ///
            DIM2* DSOULAS = NULL;
            HBITMAP BMSRC = NULL;
            if( D2PTR_T->GetInput_Try()->IsLoaded ){
                DSOULAS = D2PTR_T->GetInput_Try();
                D2PTR_T->GetInput_Try()->ReadDatas();
                if( D2PTR_T->GetInput_Try()->ProcType==BW_LABEL ){
                    int NPosBuffT = D2PTR_T->GetInput_Try()->WPIC_OUT * D2PTR_T->GetInput_Try()->HPIC_OUT;
                    for( int i=0; i<(int)NPosBuffT; i++ ){
                        D2PTR_T->GetInput_Try()->RgbBuff[i].rgbRed   =
                        D2PTR_T->GetInput_Try()->RgbBuff[i].rgbGreen =
                        D2PTR_T->GetInput_Try()->RgbBuff[i].rgbBlue  = 255 * (int)((D2PTR_T->GetInput_Try()->RgbBuff[i].rgbRed!=255) || (D2PTR_T->GetInput_Try()->RgbBuff[i].rgbGreen!=255) ||
                                                        (D2PTR_T->GetInput_Try()->RgbBuff[i].rgbBlue!=255));
                    }
                }
                BMSRC = D2PTR_T->NewBitmap( D2PTR_T->GetInput_Try()->WPIC_OUT, D2PTR_T->GetInput_Try()->HPIC_OUT );
                SetBitmapBits( BMSRC, 4 * D2PTR_T->GetInput_Try()->WPIC_OUT * D2PTR_T->GetInput_Try()->HPIC_OUT,
                               D2PTR_T->GetInput_Try()->RgbBuff );
                D2PTR_T->GetInput_Try()->FreeBuff();
            }else{
                DSOULAS = D2PTR_T->D2_SOURCE;
                D2PTR_T->D2_SOURCE->ReadDatas();
                BMSRC = D2PTR_T->NewBitmap( D2PTR_T->D2_SOURCE->WPIC_OUT, D2PTR_T->D2_SOURCE->HPIC_OUT );
                SetBitmapBits( BMSRC, 4 * D2PTR_T->D2_SOURCE->WPIC_OUT * D2PTR_T->D2_SOURCE->HPIC_OUT,
                               D2PTR_T->D2_SOURCE->RgbBuff );
                D2PTR_T->D2_SOURCE->FreeBuff();
            }
            ///
            ///
            ///
            ///
            BITMAP BmpI = {0};
            GetObject( BMSRC, sizeof(BITMAP), (LPSTR)&BmpI );
            ///
            ///
            ///
            ///
            int WP = 0;
            int HP = 0;
            ///
            WindowSize( hDlg, &WP, &HP );
            ///
            ///
            ///
            ///
            WindowSize( HW_PIC_O, &WP, &HP );
            //sprintf( T2T, "bmWidth   %i - %i", WP, HP );MessageBoxA( hDlg, T2T, "", 0 );
            ///
            const int Y_P_PIC = 70;
            W_OUT = WP;
            H_OUT = (int)( round( (W_OUT * BmpI.bmHeight)/((double)BmpI.bmWidth) ) );
            if( H_OUT>600 ){
                H_OUT = 600;
                W_OUT =(int)( round( (H_OUT * BmpI.bmWidth)/((double)BmpI.bmHeight) ) );
                MoveWindow( hDlg, 1, 66, WP+10, H_OUT + (Y_P_PIC+33), TRUE );
                MoveWindow( HW_PIC_O, 1, Y_P_PIC, WP+2, H_OUT, TRUE );
            }else{
                MoveWindow( hDlg, 1, 66, W_OUT+10, H_OUT + (Y_P_PIC+33), TRUE );
                MoveWindow( HW_PIC_O, 1, Y_P_PIC, W_OUT+2, H_OUT, TRUE );
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
            ///
            ///
            ///
            HDC ODC = GetDC( 0 );
            HBMP_P = CreateCompatibleBitmap( ODC , W_OUT, H_OUT );
            ReleaseDC( 0, ODC );
            DeleteDC( ODC );
            ///
            HDC hdc = CreateCompatibleDC( GetDC( 0 ) );
            HDC memDC = CreateCompatibleDC( GetDC( 0 ) );
            SelectObject( hdc , BMSRC );/// HBPLOT
            SelectObject( memDC , HBMP_P );/// HB_I
            SetStretchBltMode( memDC , HALFTONE );/// COLORONCOLOR  HALFTONE
            StretchBlt( memDC, 0, 0, W_OUT, H_OUT, hdc, 0, 0, BmpI.bmWidth, BmpI.bmHeight, SRCCOPY );
            ReleaseDC( NULL, hdc );
            ReleaseDC( NULL, memDC );
            DeleteDC( hdc );
            DeleteDC( memDC );
            ///
            ///
            RgbO = (RGBQUAD*)malloc( 4 * W_OUT * H_OUT + 1024 );
            RgbOut = (RGBQUAD*)malloc( 4 * W_OUT * H_OUT + 1024 );
            ZeroMemory( RgbO, 4 * W_OUT * H_OUT );
            ZeroMemory( RgbOut, 4 * W_OUT * H_OUT );
            GetBitmapBits( HBMP_P, 4 * W_OUT * H_OUT, RgbO );
            GetBitmapBits( HBMP_P, 4 * W_OUT * H_OUT, RgbOut );
            ///
            ///
            ///
            if( !DSOULAS->IsGray ){
                int LBF = W_OUT * H_OUT;
                for( int i=0; i<LBF; i++ ){
                    RgbO[i].rgbRed =
                    RgbO[i].rgbGreen =
                    RgbO[i].rgbBlue = ParseByte(
                                    (int)round(0.2989 * RgbO[i].rgbRed +
                                    0.5870 * RgbO[i].rgbGreen +
                                    0.1140 * RgbO[i].rgbBlue)
                                    );
                }
                memcpy( RgbOut, RgbO, 4 * W_OUT * H_OUT );
            }
            ///
            ///
            ///
            ///
            ///
            ///
            D2PTR_T->ARITH_PROCS( RgbOut,
                                   W_OUT * H_OUT,
                                   D2PTR_T->ArithType,
                                   D2PTR_T->SumOper,
                                   D2PTR_T->ProdOper,
                                   D2PTR_T->LowValue,
                                   D2PTR_T->HiValue,
                                   D2PTR_T->BSetPix,
                                   D2PTR_T->InvOut
                                   );
            ///
            ///
            ///
            ///
            DeleteObject( BMSRC );
            ///
            SetBitmapBits( HBMP_P, 4 * W_OUT * H_OUT, RgbOut );
            SendMessage( HW_PIC_O, STM_SETIMAGE, IMAGE_BITMAP,(LPARAM)HBMP_P );
            ///
            ///
            CenterWindow( hDlg, 0, 0 );
            ///
            ///
            ///
            ///
            ///
            ///
            ///
            /// D2PTR_T->ArithType = 2;ShowWindow( hDlg, 1 );
            /*
            for( int i=0; i<8; i++ ){
                EnableWindow( GetDlgItem(hDlg, 200+i), FALSE );
            }
            //*/
            EnableWindow( GetDlgItem(hDlg, 200+D2PTR_T->ArithType), TRUE );
            SetFocus( GetDlgItem(hDlg, 200+D2PTR_T->ArithType) );
            ///
            ///
            ///
            }
            return TRUE;
        case WM_COMMAND:
            if( LOWORD(wParam)==IDCANCEL && D2PTR==NULL ){
                EndDialog( hDlg, FALSE );
            }
            if( D2PTR==NULL || RgbOut==NULL || RgbO==NULL ){
                return FALSE;
            }
            ///
            ///
            ///
            if( LOWORD(wParam)==512 ){
                D2PTR->InvOut = SendMessageA( GetDlgItem(hDlg, 512), BM_GETCHECK, 0, 0 );
                memcpy( RgbOut, RgbO, 4 * W_OUT * H_OUT );
                D2PTR->ARITH_PROCS( RgbOut,
                                   W_OUT * H_OUT,
                                   D2PTR->ArithType,
                                   D2PTR->SumOper,
                                   D2PTR->ProdOper,
                                   D2PTR->LowValue,
                                   D2PTR->HiValue,
                                   D2PTR->BSetPix,
                                   D2PTR->InvOut
                                   );
                SetBitmapBits( HBMP_P, 4 * W_OUT * H_OUT, RgbOut );
                SendMessage( HW_PIC_O, STM_SETIMAGE, IMAGE_BITMAP,(LPARAM)HBMP_P );
                break;
            }
            ///
            ///
            ///
            if( LOWORD(wParam)>=200 && LOWORD(wParam)<300 && HIWORD(wParam)==BN_SETFOCUS ){
                int ICtrl = (int)LOWORD(wParam);
                int I200 = 0;
                SendDlgItemMessage(hDlg, 200, BM_SETCHECK, (WPARAM)BST_CHECKED, 0);
                D2PTR->ArithType = ICtrl - 200;
                for( int i=0; i<8; i++ ){
                    I200 = i + 200;
                    if( I200==ICtrl ){
                        ///SendDlgItemMessage(hDlg, I200, BM_SETCHECK, (WPARAM)BST_CHECKED, 0 );
                        EnableWindow( GetDlgItem(hDlg, I200-100 ), TRUE );
                        EnableWindow( GetDlgItem(hDlg, I200-150 ), TRUE );
                    }else{
                        ///SendDlgItemMessage(hDlg, I200, BM_SETCHECK, (WPARAM)BST_UNCHECKED, 0 );
                        EnableWindow( GetDlgItem(hDlg, I200-100 ), FALSE );
                        EnableWindow( GetDlgItem(hDlg, I200-150 ), FALSE );
                    }
                }
                SetFocus( GetDlgItem(hDlg, LOWORD(wParam)) );
                memcpy( RgbOut, RgbO, 4 * W_OUT * H_OUT );
                D2PTR->ARITH_PROCS( RgbOut,
                                   W_OUT * H_OUT,
                                   D2PTR->ArithType,
                                   D2PTR->SumOper,
                                   D2PTR->ProdOper,
                                   D2PTR->LowValue,
                                   D2PTR->HiValue,
                                   D2PTR->BSetPix,
                                   D2PTR->InvOut
                                   );
                SetBitmapBits( HBMP_P, 4 * W_OUT * H_OUT, RgbOut );
                SendMessage( HW_PIC_O, STM_SETIMAGE, IMAGE_BITMAP,(LPARAM)HBMP_P );
                break;
            }
            ///
            ///
            ///
            if( (LOWORD(wParam)>=50 && LOWORD(wParam)<100) ){
                if( HIWORD(wParam)==EN_UPDATE ){
                    ParseEditInt( wParam, lParam );
                    break;
                }
                if( HIWORD(wParam)==EN_KILLFOCUS && T2T[0]==0 ){
                    GetWindowTextA( GetDlgItem( hDlg, LOWORD(wParam)), T2T, 16 );
                    sprintf( T2T, "%i", D2PTR->HiValue );
                    SetWindowTextA( GetDlgItem( hDlg, LOWORD(wParam)), T2T );
                    break;
                }
                if( HIWORD(wParam)!=EN_CHANGE ){
                    break;
                }
                GetWindowTextA( GetDlgItem( hDlg, LOWORD(wParam)), T2T, 16 );
                if( T2T[0]==0 ){
                    break;
                }
                D2PTR->HiValue = atoi( T2T );
                memcpy( RgbOut, RgbO, 4 * W_OUT * H_OUT );
                D2PTR->ARITH_PROCS( RgbOut,
                                   W_OUT * H_OUT,
                                   D2PTR->ArithType,
                                   D2PTR->SumOper,
                                   D2PTR->ProdOper,
                                   D2PTR->LowValue,
                                   D2PTR->HiValue,
                                   D2PTR->BSetPix,
                                   D2PTR->InvOut
                                   );
                SetBitmapBits( HBMP_P, 4 * W_OUT * H_OUT, RgbOut );
                SendMessage( HW_PIC_O, STM_SETIMAGE, IMAGE_BITMAP,(LPARAM)HBMP_P );
                break;
            }
            ///
            ///
            ///
            ///
            ///
            if( (LOWORD(wParam)>=100 && LOWORD(wParam)<200) ){
                if( GetDlgItem( hDlg, LOWORD(wParam))!=GetFocus() ){
                    //break;
                }
                if( HIWORD(wParam)==EN_UPDATE ){
                    switch( D2PTR->ArithType ){
                        case 0:
                        case 2:
                            ParseEditInt( wParam, lParam );
                            break;
                        case 1:
                            ParseEditDouble( wParam, lParam );
                            break;
                    }
                    break;
                }
                ///
                if( HIWORD(wParam)==EN_KILLFOCUS && T2T[0]==0 ){
                    GetWindowTextA( GetDlgItem( hDlg, LOWORD(wParam)), T2T, 16 );
                    switch( D2PTR->ArithType ){
                        case 0:
                            sprintf( T2T, "%i", D2PTR->SumOper );
                            SetWindowTextA( GetDlgItem( hDlg, LOWORD(wParam)), T2T );
                            break;
                        case 1:
                            sprintf( T2T, "%g", D2PTR->ProdOper );
                            SetWindowTextA( GetDlgItem( hDlg, LOWORD(wParam)), T2T );
                            break;
                        case 2:
                            sprintf( T2T, "%i", D2PTR->LowValue );
                            SetWindowTextA( GetDlgItem( hDlg, LOWORD(wParam)), T2T );
                            break;
                    }
                }
                ///
                if( HIWORD(wParam)!=EN_CHANGE ){
                    break;
                }
                GetWindowTextA( GetDlgItem( hDlg, LOWORD(wParam)), T2T, 16 );
                if( T2T[0]==0 ){
                    break;
                }
                switch( D2PTR->ArithType ){
                    case 0:
                        D2PTR->SumOper = atoi( T2T );
                        break;
                    case 1:
                        D2PTR->ProdOper = atof( T2T );
                        break;
                    case 2:
                        D2PTR->LowValue = atoi( T2T );
                        break;
                }
                memcpy( RgbOut, RgbO, 4 * W_OUT * H_OUT );
                D2PTR->ARITH_PROCS( RgbOut,
                                   W_OUT * H_OUT,
                                   D2PTR->ArithType,
                                   D2PTR->SumOper,
                                   D2PTR->ProdOper,
                                   D2PTR->LowValue,
                                   D2PTR->HiValue,
                                   D2PTR->BSetPix,
                                   D2PTR->InvOut
                                   );
                SetBitmapBits( HBMP_P, 4 * W_OUT * H_OUT, RgbOut );
                SendMessage( HW_PIC_O, STM_SETIMAGE, IMAGE_BITMAP,(LPARAM)HBMP_P );
                break;
            }
            ///
            ///
            ///
            switch( LOWORD(wParam) ){
                ///
                ///
                case IDCANCEL:
                case IDOK:
                    D2PTR = NULL;
                    free( RgbO );
                    free( RgbOut );
                    RgbO = NULL;
                    RgbOut = NULL;
                    DeleteObject( HBMP_P );
                    EndDialog( hDlg, TRUE );
                    break;
            }
            return TRUE;
        case WM_CLOSE:
            D2PTR = NULL;
            free( RgbO );
            free( RgbOut );
            RgbO = NULL;
            RgbOut = NULL;
            DeleteObject( HBMP_P );
            EndDialog( hDlg, TRUE );
            break;

    }
    return FALSE;
}
/// //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// //////////////////////////////////////////////////////////////////////////////////////////////////////////////////




/// //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CALLBACK SELECT_ERODE(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam){
    /// static HWND             HW_INFO2  = NULL;
    static HWND             HW_LB_PRC       = NULL;
    static HWND             HW_PIC_O        = NULL;
    static HWND             HW_PIC_STREL    = NULL;
    static HWND             HW_BF           = NULL;
    static HBITMAP          HBMP_O          = NULL;
    static HBITMAP          HBMP_STREL      = NULL;
    static HBITMAP          HBMP_P          = NULL;
    static HBITMAP          HBMP_FRONTIER   = NULL;
    static RGBQUAD*         RgbI            = NULL;
    static RGBQUAD*         RgbO            = NULL;
    static RGBQUAD*         RGBStrel        = NULL;
    static DIM2*            D2PTR           = NULL;
    static int              W_OUT           = 0;
    static int              H_OUT           = 0;
    static double           HistE[256]      = {0};
    static int              W_P_H           = 0;
    static int              NB_B            = 0;
    static double           PerWHN          = 1;
    static char             Text20[8][16] = { "Radio:", "Lado:", "Radio:" };
    char T2T[1024] = {0};
    static int WH32P = 58;
    switch(msg){
        case WM_INITDIALOG:{
            D2PTR = (DIM2*)lParam;
            ///
            if( D2PTR->OptErode>8 ){
                D2PTR->OptErode = 0;
            }
            ///
            if( D2PTR->NOrdErode<1 ){
                D2PTR->NOrdErode = 1;
            }
            ///
            ShowWindow( GetDlgItem( hDlg, 1025 ), 0 );
            ShowWindow( GetDlgItem( hDlg,  257 ), 0 );
            if( D2PTR==NULL ){
                CenterWindow( hDlg, 0, 0 );return FALSE;
            }
            HW_PIC_O = GetDlgItem( hDlg, 101 );
            SetFocus( GetDlgItem( hDlg, IDOK ) );
            HW_PIC_STREL = GetDlgItem( hDlg, 102 );
            /// HW_INFO2 = GetDlgItem( hDlg, 64 );
            HW_LB_PRC = GetDlgItem( hDlg, 512 );
            SendMessage( HW_LB_PRC, CB_ADDSTRING, 0, (LPARAM)"BALL");
            SendMessage( HW_LB_PRC, CB_ADDSTRING, 0, (LPARAM)"SQUARE");
            SendMessage( HW_LB_PRC, CB_ADDSTRING, 0, (LPARAM)"DIAMOND");
            SendMessage( HW_LB_PRC, CB_SETCURSEL, D2PTR->OptErode, (LPARAM)D2PTR->OptErode );
            ///
            ///
            ///
            ///
            ///
            RGBStrel = (RGBQUAD*)malloc( 4 * 1024 * 1024 );
            ///
            ///
            ///
            sprintf( T2T, "%i", D2PTR->NOrdErode );
            SetWindowTextA( GetDlgItem(hDlg,256), T2T );
            ///
            sprintf( T2T, "%s", Text20[D2PTR->OptErode] );
            SetWindowTextA( GetDlgItem(hDlg,1024), T2T );
            ///
            ///
            ///
            ///
            ///
            BOOL IsGrayS = TRUE;
            BOOL IsBiS = TRUE;
            if( D2PTR->GetInput_Try()->IsLoaded &&
                    D2PTR->GetInput_Try()->ProcType!=NO_PROC_Signal_2D ){
                IsGrayS = D2PTR->GetInput_Try()->IsGray;
                IsBiS   = D2PTR->GetInput_Try()->IsBin;
                D2PTR->GetInput_Try()->ReadDatas();

                if( D2PTR->GetInput_Try()->ProcType==BW_LABEL ){
                    int NPosBuffT = D2PTR->GetInput_Try()->WPIC_OUT * D2PTR->GetInput_Try()->HPIC_OUT;
                    for( int i=0; i<(int)NPosBuffT; i++ ){
                        D2PTR->GetInput_Try()->RgbBuff[i].rgbRed   =
                        D2PTR->GetInput_Try()->RgbBuff[i].rgbGreen =
                        D2PTR->GetInput_Try()->RgbBuff[i].rgbBlue  = 255 * (int)((D2PTR->GetInput_Try()->RgbBuff[i].rgbRed!=255) ||
                                                                                (D2PTR->GetInput_Try()->RgbBuff[i].rgbGreen!=255) ||
                                                        (D2PTR->GetInput_Try()->RgbBuff[i].rgbBlue!=255));
                    }
                }

                HBMP_O = D2PTR->NewBitmap( D2PTR->GetInput_Try()->WPIC_OUT, D2PTR->GetInput_Try()->HPIC_OUT );
                SetBitmapBits( HBMP_O, 4 * D2PTR->GetInput_Try()->WPIC_OUT * D2PTR->GetInput_Try()->HPIC_OUT,
                               D2PTR->GetInput_Try()->RgbBuff );
                D2PTR->GetInput_Try()->FreeBuff();
                PerWHN = D2PTR->GetInput_Try()->WPIC_OUT;
            }else{
                IsGrayS = D2PTR->D2_SOURCE->IsGray;
                IsBiS   = D2PTR->D2_SOURCE->IsBin;
                D2PTR->D2_SOURCE->ReadDatas();
                HBMP_O = D2PTR->NewBitmap( D2PTR->D2_SOURCE->WPIC_OUT, D2PTR->D2_SOURCE->HPIC_OUT );
                SetBitmapBits( HBMP_O, 4 * D2PTR->D2_SOURCE->WPIC_OUT * D2PTR->D2_SOURCE->HPIC_OUT,
                               D2PTR->D2_SOURCE->RgbBuff );
                D2PTR->D2_SOURCE->FreeBuff();
                PerWHN = D2PTR->D2_SOURCE->WPIC_OUT;
            }
            ///
            ///
            ///
            ///
            ///
            ///
            WH32P = 74;
            ///
            ///
            int WP = 0;
            int HP = 0;
            WindowSize( HW_PIC_O, &WP, &HP );
            W_OUT = WP;
            H_OUT = (int)( round( (W_OUT * D2PTR->HPIC_OUT)/((double)D2PTR->WPIC_OUT) ) );
            const int Y_P_PIC = 79;
            if( H_OUT>640 ){
                H_OUT = HP;
                W_OUT =(int)( round( (H_OUT * D2PTR->WPIC_OUT)/((double)D2PTR->HPIC_OUT) ) );
                PerWHN = W_OUT/PerWHN;
                MoveWindow( hDlg, 1, 66, WP+10, H_OUT + (Y_P_PIC+33), TRUE );
                MoveWindow( HW_PIC_O, 1, Y_P_PIC, WP+2, H_OUT, TRUE );
            }else{
                MoveWindow( hDlg, 1, 66, W_OUT+10, H_OUT + (Y_P_PIC+33), TRUE );
                MoveWindow( HW_PIC_O, 1, Y_P_PIC, W_OUT+2, H_OUT, TRUE );
                PerWHN = W_OUT/PerWHN;
            }
            HBMP_P = D2PTR->Redim_PIC( HBMP_O, W_OUT, H_OUT );
            DeleteObject( HBMP_O );
            ///
            int NORP = (int)round(D2PTR->NOrdErode * PerWHN);
            if( NORP<1 ){
                NORP = 1;
            }
            ///
            ///
            ///
            NB_B = 4 * (W_OUT+1024) * (H_OUT+1024);
            ///
            RgbI = (RGBQUAD*)malloc( NB_B );
            ZeroMemory( RgbI, NB_B );
            RgbO = (RGBQUAD*)malloc( NB_B );
            memset( RgbO, 255, NB_B );
            ///
            GetBitmapBits( HBMP_P, 4 * W_OUT * H_OUT, RgbI );
            D2PTR->graythreshConvert( RgbI, W_OUT * H_OUT, IsGrayS, IsBiS );
            SetBitmapBits( HBMP_P, 4 * W_OUT * H_OUT, RgbI );
            ///
            ///
            SendMessage( HW_PIC_O, STM_SETIMAGE, IMAGE_BITMAP,(LPARAM)HBMP_P );
            ///
            ///
            ///
            /// ERODE_TRY
            ///
            /// memcpy( RgbO, RgbI, 4 * W_OUT * H_OUT );
            ///
            ///
            memset( RgbO, 255, NB_B );
            for( int y=0; y<H_OUT; y++ ){
                memcpy( &RgbO[ (y+NORP)*(W_OUT+2*NORP) + NORP ], &RgbI[y*W_OUT], 4*W_OUT );
            }
            ///
            ///
            ZeroMemory( RGBStrel, 4 * 1024 * 1024 - 256 );
            DSP_MNG.imerode( RgbO, (W_OUT+2*NORP), (H_OUT+2*NORP), HistE, NORP, D2PTR->OptErode, D2PTR->BFront,
                                                        RGBStrel, &W_P_H );
            ///
            for( int y=0; y<H_OUT; y++ ){
                memcpy( &RgbO[y*W_OUT], &RgbO[ (y+NORP)*(W_OUT+2*NORP) + NORP ], 4*W_OUT );
            }
            ///
            DeleteObject( HBMP_STREL );
            HBMP_STREL = D2PTR->NewBitmap( W_P_H, W_P_H );
            SetBitmapBits( HBMP_STREL, 4 * W_P_H * W_P_H, RGBStrel );
            HBITMAP HBM_T = D2PTR->Redim_PIC( HBMP_STREL, WH32P, WH32P );
            DeleteObject( HBMP_STREL );
            HBMP_STREL = HBM_T;
            ////
            GetBitmapBits( HBMP_STREL, 4 * WH32P * WH32P, RGBStrel );
            D2PTR->graythreshConvert( RGBStrel, WH32P * WH32P, FALSE, TRUE );
            SetBitmapBits( HBMP_STREL, 4 * WH32P * WH32P, RGBStrel );
            ///
            SendMessage( HW_PIC_STREL, STM_SETIMAGE, IMAGE_BITMAP,(LPARAM)HBMP_STREL );
            ///
            ///
            ///
            ///
            SetBitmapBits( HBMP_P, 4 * W_OUT * H_OUT, RgbO );
            SendMessage( HW_PIC_O, STM_SETIMAGE, IMAGE_BITMAP,(LPARAM)HBMP_P );
            ///
            ///
            ///
            ///
            ///
            ///
            ///
            ///
            ///
            ///
            ///
            ///
            RECT RecOut = {0};
            CenterWindow( hDlg, 0, 0, &RecOut );
            ///
            ///
            ///
            ///
            ///
            ///
            ///
            #define XPF 70
            HW_BF = CreateWindowEx( 0, "BUTTON", NULL,
                                WS_TABSTOP | BS_AUTOCHECKBOX | WS_BORDER | WS_VISIBLE | WS_CHILD | BS_BITMAP,
                               RecOut.right - XPF, 30,
                               XPF - 4, 46,
                               hDlg, (HMENU)2000, GetModuleHandleA(NULL), NULL );
            HBMP_FRONTIER = LoadImageR( FRONTIER_PIC, MAKEINTRESOURCEA(FILE_RES) );
            SendMessage( HW_BF, BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)HBMP_FRONTIER );
            SendMessage( GetDlgItem(hDlg, 2000), BM_SETCHECK, BST_CHECKED * D2PTR->BFront, BST_CHECKED );
            ///
            ///
            ///
            ///
            ///
            ///
            ///
            ///
            ///
            ///
            ///
            ///
            ///
            ///
            ///
            }
            return TRUE;
        case WM_COMMAND:
            if( LOWORD(wParam)==IDCANCEL && D2PTR==NULL ){
                D2PTR = NULL;
                DeleteObject( HBMP_FRONTIER );
                DestroyWindow( HW_BF );
                free( RgbI );
                free( RgbO );
                free( RGBStrel );
                DeleteObject( HBMP_O );
                DeleteObject( HBMP_STREL );
                DeleteObject( HBMP_P );
                EndDialog( hDlg, TRUE );
                EndDialog( hDlg, TRUE );
                break;
            }
            if( D2PTR==NULL ){
                return FALSE;
            }
            ///
            ///
            switch( LOWORD(wParam) ){
                ///
                ///
                ///
                ///
                ///
                case 512:{
                    if( HIWORD(wParam)!=CBN_SELENDOK ){
                        break;
                    }
                    D2PTR->OptErode = SendMessage( HW_LB_PRC, CB_GETCURSEL, 0, (LPARAM)0 );
                    sprintf( T2T, "%s", Text20[D2PTR->OptErode] );
                    SetWindowTextA( GetDlgItem(hDlg,1024), T2T );

            ///
            ///
            ///
            /// ERODE_TRY
            ///
            memcpy( RgbO, RgbI, 4 * W_OUT * H_OUT );
            int NORP = (int)round(D2PTR->NOrdErode * PerWHN);
            if( NORP<1 ){
                NORP = 1;
            }
            ZeroMemory( RGBStrel, 4 * 1024 * 1024 - 256 );

            ///
            memset( RgbO, 255, NB_B );
            for( int y=0; y<H_OUT; y++ ){
                memcpy( &RgbO[ (y+NORP)*(W_OUT+2*NORP) + NORP ], &RgbI[y*W_OUT], 4*W_OUT );
            }
            ///
            ///
            ZeroMemory( RGBStrel, 4 * 1024 * 1024 - 256 );
            DSP_MNG.imerode( RgbO, (W_OUT+2*NORP), (H_OUT+2*NORP), HistE, NORP, D2PTR->OptErode, D2PTR->BFront,
                                                        RGBStrel, &W_P_H );
            ///
            for( int y=0; y<H_OUT; y++ ){
                memcpy( &RgbO[y*W_OUT], &RgbO[ (y+NORP)*(W_OUT+2*NORP) + NORP ], 4*W_OUT );
            }
            ///
            ///
            ///
            DeleteObject( HBMP_STREL );
            HBMP_STREL = D2PTR->NewBitmap( W_P_H, W_P_H );
            SetBitmapBits( HBMP_STREL, 4 * W_P_H * W_P_H, RGBStrel );
            HBITMAP HBM_T = D2PTR->Redim_PIC( HBMP_STREL, WH32P, WH32P );
            DeleteObject( HBMP_STREL );
            HBMP_STREL = HBM_T;
            ////
            GetBitmapBits( HBMP_STREL, 4 * WH32P * WH32P, RGBStrel );
            D2PTR->graythreshConvert( RGBStrel, WH32P * WH32P, FALSE, TRUE );
            SetBitmapBits( HBMP_STREL, 4 * WH32P * WH32P, RGBStrel );
            ///
            SendMessage( HW_PIC_STREL, STM_SETIMAGE, IMAGE_BITMAP,(LPARAM)HBMP_STREL );
            ///
            ///
            ///
            ///
            SetBitmapBits( HBMP_P, 4 * W_OUT * H_OUT, RgbO );
            SendMessage( HW_PIC_O, STM_SETIMAGE, IMAGE_BITMAP,(LPARAM)HBMP_P );
                }
                break;
                ///
                ///
                ///
                ///
                ///
                ///
                case 2000:{
                    D2PTR->BFront = (int)( SendMessage( GetDlgItem(hDlg, 2000), BM_GETCHECK, 0, 0)==BST_CHECKED );
                    /// sprintf( T2T, "D2PTR->BFront = %i", (int)SendMessage( GetDlgItem(hDlg, 2000), BM_GETCHECK, 0, 0) );
                    /// MessageBoxA( hDlg, T2T, "", 0 );
            ///
            ///
            ///
            /// ERODE_TRY
            ///
            memcpy( RgbO, RgbI, 4 * W_OUT * H_OUT );
            int NORP = (int)round(D2PTR->NOrdErode * PerWHN);
            if( NORP<1 ){
                NORP = 1;
            }
            ZeroMemory( RGBStrel, 4 * 1024 * 1024 - 256 );
            ///
            memset( RgbO, 255, NB_B );
            for( int y=0; y<H_OUT; y++ ){
                memcpy( &RgbO[ (y+NORP)*(W_OUT+2*NORP) + NORP ], &RgbI[y*W_OUT], 4*W_OUT );
            }
            ///
            ///
            ZeroMemory( RGBStrel, 4 * 1024 * 1024 - 256 );
            DSP_MNG.imerode( RgbO, (W_OUT+2*NORP), (H_OUT+2*NORP), HistE, NORP, D2PTR->OptErode, D2PTR->BFront,
                                                        RGBStrel, &W_P_H );
            ///
            for( int y=0; y<H_OUT; y++ ){
                memcpy( &RgbO[y*W_OUT], &RgbO[ (y+NORP)*(W_OUT+2*NORP) + NORP ], 4*W_OUT );
            }
            ///
            ///
            ///
            DeleteObject( HBMP_STREL );
            HBMP_STREL = D2PTR->NewBitmap( W_P_H, W_P_H );
            SetBitmapBits( HBMP_STREL, 4 * W_P_H * W_P_H, RGBStrel );
            HBITMAP HBM_T = D2PTR->Redim_PIC( HBMP_STREL, WH32P, WH32P );
            DeleteObject( HBMP_STREL );
            HBMP_STREL = HBM_T;
            ////
            GetBitmapBits( HBMP_STREL, 4 * WH32P * WH32P, RGBStrel );
            D2PTR->graythreshConvert( RGBStrel, WH32P * WH32P, FALSE, TRUE );
            SetBitmapBits( HBMP_STREL, 4 * WH32P * WH32P, RGBStrel );
            ///
            SendMessage( HW_PIC_STREL, STM_SETIMAGE, IMAGE_BITMAP,(LPARAM)HBMP_STREL );
            ///
            ///
            ///
            ///
            ///
            SetBitmapBits( HBMP_P, 4 * W_OUT * H_OUT, RgbO );
            SendMessage( HW_PIC_O, STM_SETIMAGE, IMAGE_BITMAP,(LPARAM)HBMP_P );
                                ///
                                ///
                }
                break;
                case 256:{
                    if( HIWORD(wParam)==EN_KILLFOCUS ){
                        GetWindowTextA( GetDlgItem(hDlg,256), T2T, 8 );
                        if( T2T[0]==0 || atoi(T2T)==0 ){
                            sprintf( T2T, "%i", D2PTR->NOrdErode );
                            SetWindowTextA( GetDlgItem(hDlg,256), T2T );
                        }
                        break;
                    }
                    if( GetFocus()!=GetDlgItem(hDlg,256) ){
                        break;
                    }
                    if( HIWORD(wParam)==EN_CHANGE ){
                        /// MessageBoxA( hDlg, "", "", 0 );
                        GetWindowTextA( (HWND)lParam, T2T, 8 );
                        if( T2T[0]!=0 ){
                            if( atoi(T2T)==0 ){
                                sprintf( T2T, "%i", D2PTR->NOrdErode );
                                //SetWindowTextA( (HWND)lParam, T2T );
                            }else{
                                D2PTR->NOrdErode = atoi(T2T);
                                ///
                                ///

            ///
            ///
            ///
            /// ERODE_TRY
            ///
            memcpy( RgbO, RgbI, 4 * W_OUT * H_OUT );
            int NORP = (int)round(D2PTR->NOrdErode * PerWHN);
            if( NORP<1 ){
                NORP = 1;
            }
            ZeroMemory( RGBStrel, 4 * 1024 * 1024 - 256 );
            ///
            memset( RgbO, 255, NB_B );
            for( int y=0; y<H_OUT; y++ ){
                memcpy( &RgbO[ (y+NORP)*(W_OUT+2*NORP) + NORP ], &RgbI[y*W_OUT], 4*W_OUT );
            }
            ///
            ///
            ZeroMemory( RGBStrel, 4 * 1024 * 1024 - 256 );
            DSP_MNG.imerode( RgbO, (W_OUT+2*NORP), (H_OUT+2*NORP), HistE, NORP, D2PTR->OptErode, D2PTR->BFront,
                                                        RGBStrel, &W_P_H );
            ///
            for( int y=0; y<H_OUT; y++ ){
                memcpy( &RgbO[y*W_OUT], &RgbO[ (y+NORP)*(W_OUT+2*NORP) + NORP ], 4*W_OUT );
            }
            ///
            ///
            ///
            DeleteObject( HBMP_STREL );
            HBMP_STREL = D2PTR->NewBitmap( W_P_H, W_P_H );
            SetBitmapBits( HBMP_STREL, 4 * W_P_H * W_P_H, RGBStrel );
            HBITMAP HBM_T = D2PTR->Redim_PIC( HBMP_STREL, WH32P, WH32P );
            DeleteObject( HBMP_STREL );
            HBMP_STREL = HBM_T;
            ////
            GetBitmapBits( HBMP_STREL, 4 * WH32P * WH32P, RGBStrel );
            D2PTR->graythreshConvert( RGBStrel, WH32P * WH32P, FALSE, TRUE );
            SetBitmapBits( HBMP_STREL, 4 * WH32P * WH32P, RGBStrel );
            ///
            SendMessage( HW_PIC_STREL, STM_SETIMAGE, IMAGE_BITMAP,(LPARAM)HBMP_STREL );
            ///
            ///
            ///
            ///
            SetBitmapBits( HBMP_P, 4 * W_OUT * H_OUT, RgbO );
            SendMessage( HW_PIC_O, STM_SETIMAGE, IMAGE_BITMAP,(LPARAM)HBMP_P );
                                ///
                                ///
                            }
                        }
                    }
                }
                    break;
                ///
                ///
                ///
                ///
                ///
                ///
                ///
                ///
                ///
                ///
                case IDCANCEL:
                case IDOK:
                    D2PTR = NULL;
                    free( RgbI );
                    free( RgbO );
                    free( RGBStrel );
                    DeleteObject( HBMP_O );
                    DeleteObject( HBMP_STREL );
                    DeleteObject( HBMP_P );
                    EndDialog( hDlg, TRUE );
                    break;
            }
            return TRUE;
        case WM_CLOSE:
            D2PTR = NULL;
            DeleteObject( HBMP_FRONTIER );
            DestroyWindow( HW_BF );
            free( RgbI );
            free( RgbO );
            free( RGBStrel );
            DeleteObject( HBMP_O );
            DeleteObject( HBMP_STREL );
            DeleteObject( HBMP_P );
            EndDialog( hDlg, TRUE );
            EndDialog( hDlg, TRUE );
            break;

    }
    return FALSE;
}




/// //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CALLBACK SELECT_DILATE(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam){
    /// static HWND             HW_INFO2  = NULL;
    static HWND             HW_LB_PRC       = NULL;
    static HWND             HW_PIC_O        = NULL;
    static HWND             HW_PIC_STREL    = NULL;
    static HWND             HW_BF           = NULL;
    static HBITMAP          HBMP_O          = NULL;
    static HBITMAP          HBMP_STREL      = NULL;
    static HBITMAP          HBMP_P          = NULL;
    static HBITMAP          HBMP_FRONTIER   = NULL;
    static RGBQUAD*         RgbI            = NULL;
    static RGBQUAD*         RgbO            = NULL;
    static RGBQUAD*         RGBStrel        = NULL;
    static DIM2*            D2PTR           = NULL;
    static int              W_OUT           = 0;
    static int              H_OUT           = 0;
    static double           HistE[256]      = {0};
    static int              W_P_H           = 0;
    static double           PerWHN          = 1;
    static char             Text20[8][16] = { "Radio:", "Lado:", "Radio:" };
    char T2T[1024] = {0};
    static int WH32P = 58;
    switch(msg){
        case WM_INITDIALOG:{
            SetWindowTextA( hDlg, " Dilatar (Imagen de prueba) " );
            D2PTR = (DIM2*)lParam;
            ///
            if( D2PTR->OptErode>8 ){
                D2PTR->OptErode = 0;
            }
            ///
            if( D2PTR->NOrdErode<1 ){
                D2PTR->NOrdErode = 1;
            }
            ///
            ShowWindow( GetDlgItem( hDlg, 1025 ), 0 );
            ShowWindow( GetDlgItem( hDlg,  257 ), 0 );
            if( D2PTR==NULL ){
                CenterWindow( hDlg, 0, 0 );return FALSE;
            }
            HW_PIC_O = GetDlgItem( hDlg, 101 );
            SetFocus( GetDlgItem( hDlg, IDOK ) );
            HW_PIC_STREL = GetDlgItem( hDlg, 102 );
            /// HW_INFO2 = GetDlgItem( hDlg, 64 );
            HW_LB_PRC = GetDlgItem( hDlg, 512 );
            SendMessage( HW_LB_PRC, CB_ADDSTRING, 0, (LPARAM)"BALL");
            SendMessage( HW_LB_PRC, CB_ADDSTRING, 0, (LPARAM)"SQUARE");
            SendMessage( HW_LB_PRC, CB_ADDSTRING, 0, (LPARAM)"DIAMOND");
            SendMessage( HW_LB_PRC, CB_SETCURSEL, D2PTR->OptErode, (LPARAM)D2PTR->OptErode );
            ///
            ///
            ///
            ///
            ///
            ///
            ///
            RGBStrel = (RGBQUAD*)malloc( 4 * 1024 * 1024 );
            ///
            ///
            ///
            sprintf( T2T, "%i", D2PTR->NOrdErode );
            SetWindowTextA( GetDlgItem(hDlg,256), T2T );
            ///
            sprintf( T2T, "%s", Text20[D2PTR->OptErode] );
            SetWindowTextA( GetDlgItem(hDlg,1024), T2T );
            ///
            ///
            ///
            ///
            ///
            BOOL IsGrayS = TRUE;
            BOOL IsBiS = TRUE;
            if( D2PTR->GetInput_Try()->IsLoaded &&
                            D2PTR->GetInput_Try()->ProcType!=NO_PROC_Signal_2D ){
                IsGrayS = D2PTR->GetInput_Try()->IsGray;
                IsBiS   = D2PTR->GetInput_Try()->IsBin;
                D2PTR->GetInput_Try()->ReadDatas();

                if( D2PTR->GetInput_Try()->ProcType==BW_LABEL ){
                    int NPosBuffT = D2PTR->GetInput_Try()->WPIC_OUT * D2PTR->GetInput_Try()->HPIC_OUT;
                    for( int i=0; i<(int)NPosBuffT; i++ ){
                        D2PTR->GetInput_Try()->RgbBuff[i].rgbRed   =
                        D2PTR->GetInput_Try()->RgbBuff[i].rgbGreen =
                        D2PTR->GetInput_Try()->RgbBuff[i].rgbBlue  = 255 * (int)((D2PTR->GetInput_Try()->RgbBuff[i].rgbRed!=255) ||
                                                                                (D2PTR->GetInput_Try()->RgbBuff[i].rgbGreen!=255) ||
                                                        (D2PTR->GetInput_Try()->RgbBuff[i].rgbBlue!=255));
                    }
                }

                HBMP_O = D2PTR->NewBitmap( D2PTR->GetInput_Try()->WPIC_OUT, D2PTR->GetInput_Try()->HPIC_OUT );
                SetBitmapBits( HBMP_O, 4 * D2PTR->GetInput_Try()->WPIC_OUT * D2PTR->GetInput_Try()->HPIC_OUT,
                               D2PTR->GetInput_Try()->RgbBuff );
                D2PTR->GetInput_Try()->FreeBuff();
                PerWHN = D2PTR->GetInput_Try()->WPIC_OUT;
            }else{
                IsGrayS = D2PTR->D2_SOURCE->IsGray;
                IsBiS   = D2PTR->D2_SOURCE->IsBin;
                D2PTR->D2_SOURCE->ReadDatas();
                HBMP_O = D2PTR->NewBitmap( D2PTR->D2_SOURCE->WPIC_OUT, D2PTR->D2_SOURCE->HPIC_OUT );
                SetBitmapBits( HBMP_O, 4 * D2PTR->D2_SOURCE->WPIC_OUT * D2PTR->D2_SOURCE->HPIC_OUT,
                               D2PTR->D2_SOURCE->RgbBuff );
                D2PTR->D2_SOURCE->FreeBuff();
                PerWHN = D2PTR->D2_SOURCE->WPIC_OUT;
            }
            ///
            ///
            ///
            ///
            ///
            ///
            WH32P = 74;
            ///
            ///
            int WP = 0;
            int HP = 0;
            WindowSize( HW_PIC_O, &WP, &HP );
            W_OUT = WP;
            H_OUT = (int)( round( (W_OUT * D2PTR->HPIC_OUT)/((double)D2PTR->WPIC_OUT) ) );
            const int Y_P_PIC = 79;
            if( H_OUT>640 ){
                H_OUT = HP;
                W_OUT =(int)( round( (H_OUT * D2PTR->WPIC_OUT)/((double)D2PTR->HPIC_OUT) ) );
                PerWHN = W_OUT/PerWHN;
                MoveWindow( hDlg, 1, 66, WP+10, H_OUT + (Y_P_PIC+33), TRUE );
                MoveWindow( HW_PIC_O, 1, Y_P_PIC, WP+2, H_OUT, TRUE );
            }else{
                MoveWindow( hDlg, 1, 66, W_OUT+10, H_OUT + (Y_P_PIC+33), TRUE );
                MoveWindow( HW_PIC_O, 1, Y_P_PIC, W_OUT+2, H_OUT, TRUE );
                PerWHN = W_OUT/PerWHN;
            }
            HBMP_P = D2PTR->Redim_PIC( HBMP_O, W_OUT, H_OUT );
            DeleteObject( HBMP_O );
            ///
            ///
            ///
            RgbI = (RGBQUAD*)malloc( 4 * W_OUT * H_OUT );
            ZeroMemory( RgbI, 4 * W_OUT * H_OUT );
            RgbO = (RGBQUAD*)malloc( 4 * W_OUT * H_OUT );
            ZeroMemory( RgbO, 4 * W_OUT * H_OUT );
            ///
            GetBitmapBits( HBMP_P, 4 * W_OUT * H_OUT, RgbI );
            D2PTR->graythreshConvert( RgbI, W_OUT * H_OUT, IsGrayS, IsBiS );
            SetBitmapBits( HBMP_P, 4 * W_OUT * H_OUT, RgbI );
            ///
            ///
            SendMessage( HW_PIC_O, STM_SETIMAGE, IMAGE_BITMAP,(LPARAM)HBMP_P );
            ///
            ///
            ///
            /// ERODE_TRY
            ///
            memcpy( RgbO, RgbI, 4 * W_OUT * H_OUT );
            int NORP = (int)round(D2PTR->NOrdErode * PerWHN);
            if( NORP<1 ){
                NORP = 1;
            }
            ZeroMemory( RGBStrel, 4 * 1024 * 1024 - 256 );
            DSP_MNG.imdilate( RgbO, W_OUT, H_OUT, HistE, NORP, D2PTR->OptErode, D2PTR->BFront,
                                                        RGBStrel, &W_P_H );
            ///
            ///
            DeleteObject( HBMP_STREL );
            HBMP_STREL = D2PTR->NewBitmap( W_P_H, W_P_H );
            SetBitmapBits( HBMP_STREL, 4 * W_P_H * W_P_H, RGBStrel );
            HBITMAP HBM_T = D2PTR->Redim_PIC( HBMP_STREL, WH32P, WH32P );
            DeleteObject( HBMP_STREL );
            HBMP_STREL = HBM_T;
            ////
            GetBitmapBits( HBMP_STREL, 4 * WH32P * WH32P, RGBStrel );
            D2PTR->graythreshConvert( RGBStrel, WH32P * WH32P, FALSE, TRUE );
            SetBitmapBits( HBMP_STREL, 4 * WH32P * WH32P, RGBStrel );
            ///
            SendMessage( HW_PIC_STREL, STM_SETIMAGE, IMAGE_BITMAP,(LPARAM)HBMP_STREL );
            ///
            ///
            ///
            ///
            SetBitmapBits( HBMP_P, 4 * W_OUT * H_OUT, RgbO );
            SendMessage( HW_PIC_O, STM_SETIMAGE, IMAGE_BITMAP,(LPARAM)HBMP_P );
            ///
            ///
            ///
            ///
            ///
            RECT RecOut = {0};
            CenterWindow( hDlg, 0, 0, &RecOut );
            ///
            HW_BF = CreateWindowEx( 0, "BUTTON", NULL,
                                WS_TABSTOP | BS_AUTOCHECKBOX | WS_BORDER | WS_VISIBLE | WS_CHILD | BS_BITMAP,
                               RecOut.right - XPF, 30,
                               XPF - 4, 46,
                               hDlg, (HMENU)2000, GetModuleHandleA(NULL), NULL );
            HBMP_FRONTIER = LoadImageR( FRONTIERDIL_PIC, MAKEINTRESOURCEA(FILE_RES) );
            SendMessage( HW_BF, BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)HBMP_FRONTIER );
            SendMessage( GetDlgItem(hDlg, 2000), BM_SETCHECK, BST_CHECKED * D2PTR->BFront, BST_CHECKED );
            ///
            ///
            ///
            ///
            ///
            ///
            ///
            ///
            ///
            }
            return TRUE;
        case WM_COMMAND:
            if( LOWORD(wParam)==IDCANCEL && D2PTR==NULL ){
                EndDialog( hDlg, FALSE );
            }
            if( D2PTR==NULL ){
                return FALSE;
            }
            ///
            ///
            switch( LOWORD(wParam) ){
                ///
                ///
                ///
                ///
                ///
                case 512:{
                    if( HIWORD(wParam)!=CBN_SELENDOK ){
                        break;
                    }
                    D2PTR->OptErode = SendMessage( HW_LB_PRC, CB_GETCURSEL, 0, (LPARAM)0 );
                    sprintf( T2T, "%s", Text20[D2PTR->OptErode] );
                    SetWindowTextA( GetDlgItem(hDlg,1024), T2T );

            ///
            ///
            ///
            /// ERODE_TRY
            ///
            memcpy( RgbO, RgbI, 4 * W_OUT * H_OUT );
            int NORP = (int)round(D2PTR->NOrdErode * PerWHN);
            if( NORP<1 ){
                NORP = 1;
            }
            ZeroMemory( RGBStrel, 4 * 1024 * 1024 - 256 );
            DSP_MNG.imdilate( RgbO, W_OUT, H_OUT, HistE, NORP, D2PTR->OptErode, D2PTR->BFront,
                                                        RGBStrel, &W_P_H );
            ///
            ///
            DeleteObject( HBMP_STREL );
            HBMP_STREL = D2PTR->NewBitmap( W_P_H, W_P_H );
            SetBitmapBits( HBMP_STREL, 4 * W_P_H * W_P_H, RGBStrel );
            HBITMAP HBM_T = D2PTR->Redim_PIC( HBMP_STREL, WH32P, WH32P );
            DeleteObject( HBMP_STREL );
            HBMP_STREL = HBM_T;
            ////
            GetBitmapBits( HBMP_STREL, 4 * WH32P * WH32P, RGBStrel );
            D2PTR->graythreshConvert( RGBStrel, WH32P * WH32P, FALSE, TRUE );
            SetBitmapBits( HBMP_STREL, 4 * WH32P * WH32P, RGBStrel );
            ///
            SendMessage( HW_PIC_STREL, STM_SETIMAGE, IMAGE_BITMAP,(LPARAM)HBMP_STREL );
            ///
            ///
            ///
            ///
            SetBitmapBits( HBMP_P, 4 * W_OUT * H_OUT, RgbO );
            SendMessage( HW_PIC_O, STM_SETIMAGE, IMAGE_BITMAP,(LPARAM)HBMP_P );
                }
                break;
                ///
                ///
                ///
                ///
                ///
                ///
                case 2000:{
                    D2PTR->BFront = (int)( SendMessage( GetDlgItem(hDlg, 2000), BM_GETCHECK, 0, 0)==BST_CHECKED );
                    /// sprintf( T2T, "D2PTR->BFront = %i", (int)SendMessage( GetDlgItem(hDlg, 2000), BM_GETCHECK, 0, 0) );
                    /// MessageBoxA( hDlg, T2T, "", 0 );
            ///
            ///
            ///
            /// ERODE_TRY
            ///
            memcpy( RgbO, RgbI, 4 * W_OUT * H_OUT );
            int NORP = (int)round(D2PTR->NOrdErode * PerWHN);
            if( NORP<1 ){
                NORP = 1;
            }
            ZeroMemory( RGBStrel, 4 * 1024 * 1024 - 256 );
            DSP_MNG.imdilate( RgbO, W_OUT, H_OUT, HistE, NORP, D2PTR->OptErode, D2PTR->BFront,
                                                        RGBStrel, &W_P_H );
            ///
            ///
            DeleteObject( HBMP_STREL );
            HBMP_STREL = D2PTR->NewBitmap( W_P_H, W_P_H );
            SetBitmapBits( HBMP_STREL, 4 * W_P_H * W_P_H, RGBStrel );
            HBITMAP HBM_T = D2PTR->Redim_PIC( HBMP_STREL, WH32P, WH32P );
            DeleteObject( HBMP_STREL );
            HBMP_STREL = HBM_T;
            ////
            GetBitmapBits( HBMP_STREL, 4 * WH32P * WH32P, RGBStrel );
            D2PTR->graythreshConvert( RGBStrel, WH32P * WH32P, FALSE, TRUE );
            SetBitmapBits( HBMP_STREL, 4 * WH32P * WH32P, RGBStrel );
            ///
            SendMessage( HW_PIC_STREL, STM_SETIMAGE, IMAGE_BITMAP,(LPARAM)HBMP_STREL );
            ///
            ///
            ///
            ///
            ///
            SetBitmapBits( HBMP_P, 4 * W_OUT * H_OUT, RgbO );
            SendMessage( HW_PIC_O, STM_SETIMAGE, IMAGE_BITMAP,(LPARAM)HBMP_P );
                                ///
                                ///
                }
                break;
                ///
                ///
                ///
                ///
                ///
                ///
                case 256:{
                    if( HIWORD(wParam)==EN_KILLFOCUS ){
                        GetWindowTextA( GetDlgItem(hDlg,256), T2T, 8 );
                        if( T2T[0]==0 || atoi(T2T)==0 ){
                            sprintf( T2T, "%i", D2PTR->NOrdErode );
                            SetWindowTextA( GetDlgItem(hDlg,256), T2T );
                        }
                        break;
                    }
                    if( GetFocus()!=GetDlgItem(hDlg,256) ){
                        break;
                    }
                    if( HIWORD(wParam)==EN_CHANGE ){
                        /// MessageBoxA( hDlg, "", "", 0 );
                        GetWindowTextA( (HWND)lParam, T2T, 8 );
                        if( T2T[0]!=0 ){
                            if( atoi(T2T)==0 ){
                                sprintf( T2T, "%i", D2PTR->NOrdErode );
                                //SetWindowTextA( (HWND)lParam, T2T );
                            }else{
                                D2PTR->NOrdErode = atoi(T2T);
                                ///
                                ///

            ///
            ///
            ///
            /// ERODE_TRY
            ///
            memcpy( RgbO, RgbI, 4 * W_OUT * H_OUT );
            int NORP = (int)round(D2PTR->NOrdErode * PerWHN);
            if( NORP<1 ){
                NORP = 1;
            }
            ZeroMemory( RGBStrel, 4 * 1024 * 1024 - 256 );
            DSP_MNG.imdilate( RgbO, W_OUT, H_OUT, HistE, NORP, D2PTR->OptErode, D2PTR->BFront,
                                                        RGBStrel, &W_P_H );
            ///
            ///
            DeleteObject( HBMP_STREL );
            HBMP_STREL = D2PTR->NewBitmap( W_P_H, W_P_H );
            SetBitmapBits( HBMP_STREL, 4 * W_P_H * W_P_H, RGBStrel );
            HBITMAP HBM_T = D2PTR->Redim_PIC( HBMP_STREL, WH32P, WH32P );
            DeleteObject( HBMP_STREL );
            HBMP_STREL = HBM_T;
            ////
            GetBitmapBits( HBMP_STREL, 4 * WH32P * WH32P, RGBStrel );
            D2PTR->graythreshConvert( RGBStrel, WH32P * WH32P, FALSE, TRUE );
            SetBitmapBits( HBMP_STREL, 4 * WH32P * WH32P, RGBStrel );
            ///
            SendMessage( HW_PIC_STREL, STM_SETIMAGE, IMAGE_BITMAP,(LPARAM)HBMP_STREL );
            ///
            ///
            ///
            ///
            SetBitmapBits( HBMP_P, 4 * W_OUT * H_OUT, RgbO );
            SendMessage( HW_PIC_O, STM_SETIMAGE, IMAGE_BITMAP,(LPARAM)HBMP_P );
                                ///
                                ///
                            }
                        }
                    }
                }
                    break;
                ///
                ///
                ///
                ///
                ///
                ///
                ///
                ///
                ///
                ///
                case IDCANCEL:
                case IDOK:
                    D2PTR = NULL;
                    DeleteObject( HBMP_FRONTIER );
                    DestroyWindow( HW_BF );
                    free( RgbI );
                    free( RgbO );
                    free( RGBStrel );
                    DeleteObject( HBMP_O );
                    DeleteObject( HBMP_STREL );
                    DeleteObject( HBMP_P );
                    EndDialog( hDlg, TRUE );
                    break;
            }
            return TRUE;
        case WM_CLOSE:
            D2PTR = NULL;
            free( RgbI );
            free( RgbO );
            free( RGBStrel );
            DeleteObject( HBMP_FRONTIER );
            DestroyWindow( HW_BF );
            DeleteObject( HBMP_O );
            DeleteObject( HBMP_STREL );
            DeleteObject( HBMP_P );
            EndDialog( hDlg, TRUE );
            EndDialog( hDlg, TRUE );
            break;

    }
    return FALSE;
}

/// //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CALLBACK SELECT_OPEN(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam){
    /// static HWND             HW_INFO2  = NULL;
    static HWND             HW_LB_PRC       = NULL;
    static HWND             HW_PIC_O        = NULL;
    static HWND             HW_PIC_STREL    = NULL;
    static HWND             HW_BF           = NULL;
    static HBITMAP          HBMP_O          = NULL;
    static HBITMAP          HBMP_STREL      = NULL;
    static HBITMAP          HBMP_P          = NULL;
    static HBITMAP          HBMP_FRONTIER   = NULL;
    static RGBQUAD*         RgbI            = NULL;
    static RGBQUAD*         RgbO            = NULL;
    static RGBQUAD*         RGBStrel        = NULL;
    static DIM2*            D2PTR           = NULL;
    static int              W_OUT           = 0;
    static int              H_OUT           = 0;
    static double           HistE[256]      = {0};
    static int              NB_B            = 0;
    static int              W_P_H           = 0;
    static double           PerWHN          = 1;
    static char             Text20[8][16] = { "Radio:", "Lado:", "Radio:" };
    char T2T[1024] = {0};
    static int WH32P = 58;
    switch(msg){
        case WM_INITDIALOG:{
            SetWindowTextA( hDlg, " Apertura binaria (Imagen de prueba) " );
            D2PTR = (DIM2*)lParam;
            ///
            if( D2PTR->OptErode>8 ){
                D2PTR->OptErode = 0;
            }
            ///
            if( D2PTR->NOrdErode<1 ){
                D2PTR->NOrdErode = 1;
            }
            ///
            ShowWindow( GetDlgItem( hDlg, 1025 ), 0 );
            ShowWindow( GetDlgItem( hDlg,  257 ), 0 );
            if( D2PTR==NULL ){
                CenterWindow( hDlg, 0, 0 );return FALSE;
            }
            HW_PIC_O = GetDlgItem( hDlg, 101 );
            SetFocus( GetDlgItem( hDlg, IDOK ) );
            HW_PIC_STREL = GetDlgItem( hDlg, 102 );
            /// HW_INFO2 = GetDlgItem( hDlg, 64 );
            HW_LB_PRC = GetDlgItem( hDlg, 512 );
            SendMessage( HW_LB_PRC, CB_ADDSTRING, 0, (LPARAM)"BALL");
            SendMessage( HW_LB_PRC, CB_ADDSTRING, 0, (LPARAM)"SQUARE");
            SendMessage( HW_LB_PRC, CB_ADDSTRING, 0, (LPARAM)"DIAMOND");
            SendMessage( HW_LB_PRC, CB_SETCURSEL, D2PTR->OptErode, (LPARAM)D2PTR->OptErode );
            ///
            ///
            ///
            ///
            ///
            ///
            ///
            RGBStrel = (RGBQUAD*)malloc( 4 * 1024 * 1024 );
            ///
            ///
            ///
            sprintf( T2T, "%i", D2PTR->NOrdErode );
            SetWindowTextA( GetDlgItem(hDlg,256), T2T );
            ///
            sprintf( T2T, "%s", Text20[D2PTR->OptErode] );
            SetWindowTextA( GetDlgItem(hDlg,1024), T2T );
            ///
            ///
            ///
            ///
            ///
            BOOL IsGrayS = TRUE;
            BOOL IsBiS = TRUE;
            if( D2PTR->GetInput_Try()->IsLoaded &&
                D2PTR->GetInput_Try()->ProcType!=NO_PROC_Signal_2D ){
                IsGrayS = D2PTR->GetInput_Try()->IsGray;
                IsBiS   = D2PTR->GetInput_Try()->IsBin;
                D2PTR->GetInput_Try()->ReadDatas();

                if( D2PTR->GetInput_Try()->ProcType==BW_LABEL ){
                    int NPosBuffT = D2PTR->GetInput_Try()->WPIC_OUT * D2PTR->GetInput_Try()->HPIC_OUT;
                    for( int i=0; i<(int)NPosBuffT; i++ ){
                        D2PTR->GetInput_Try()->RgbBuff[i].rgbRed   =
                        D2PTR->GetInput_Try()->RgbBuff[i].rgbGreen =
                        D2PTR->GetInput_Try()->RgbBuff[i].rgbBlue  = 255 * (int)((D2PTR->GetInput_Try()->RgbBuff[i].rgbRed!=255) ||
                                                                                (D2PTR->GetInput_Try()->RgbBuff[i].rgbGreen!=255) ||
                                                        (D2PTR->GetInput_Try()->RgbBuff[i].rgbBlue!=255));
                    }
                }

                HBMP_O = D2PTR->NewBitmap( D2PTR->GetInput_Try()->WPIC_OUT, D2PTR->GetInput_Try()->HPIC_OUT );
                SetBitmapBits( HBMP_O, 4 * D2PTR->GetInput_Try()->WPIC_OUT * D2PTR->GetInput_Try()->HPIC_OUT,
                               D2PTR->GetInput_Try()->RgbBuff );
                D2PTR->GetInput_Try()->FreeBuff();
                PerWHN = D2PTR->GetInput_Try()->WPIC_OUT;
            }else{
                IsGrayS = D2PTR->D2_SOURCE->IsGray;
                IsBiS   = D2PTR->D2_SOURCE->IsBin;
                D2PTR->D2_SOURCE->ReadDatas();
                HBMP_O = D2PTR->NewBitmap( D2PTR->D2_SOURCE->WPIC_OUT, D2PTR->D2_SOURCE->HPIC_OUT );
                SetBitmapBits( HBMP_O, 4 * D2PTR->D2_SOURCE->WPIC_OUT * D2PTR->D2_SOURCE->HPIC_OUT,
                               D2PTR->D2_SOURCE->RgbBuff );
                D2PTR->D2_SOURCE->FreeBuff();
                PerWHN = D2PTR->D2_SOURCE->WPIC_OUT;
            }
            ///
            ///
            ///
            ///
            ///
            ///
            WH32P = 74;
            ///
            ///
            int WP = 0;
            int HP = 0;
            WindowSize( HW_PIC_O, &WP, &HP );
            W_OUT = WP;
            H_OUT = (int)( round( (W_OUT * D2PTR->HPIC_OUT)/((double)D2PTR->WPIC_OUT) ) );
            const int Y_P_PIC = 79;
            if( H_OUT>640 ){
                H_OUT = HP;
                W_OUT =(int)( round( (H_OUT * D2PTR->WPIC_OUT)/((double)D2PTR->HPIC_OUT) ) );
                MoveWindow( hDlg, 1, 66, WP+10, H_OUT + (Y_P_PIC+33), TRUE );
                MoveWindow( HW_PIC_O, 1, Y_P_PIC, WP+2, H_OUT, TRUE );
            }else{
                MoveWindow( hDlg, 1, 66, W_OUT+10, H_OUT + (Y_P_PIC+33), TRUE );
                MoveWindow( HW_PIC_O, 1, Y_P_PIC, W_OUT+2, H_OUT, TRUE );
            }
            PerWHN = ( W_OUT/( (double)PerWHN ) );
            HBMP_P = D2PTR->Redim_PIC( HBMP_O, W_OUT, H_OUT );
            DeleteObject( HBMP_O );
            ///
            ///
            ///
            ///
            ///
            ///
            NB_B = 4 * (W_OUT+1024) * (H_OUT+1024);
            ///
            RgbI = (RGBQUAD*)malloc( NB_B );
            ZeroMemory( RgbI, NB_B );
            RgbO = (RGBQUAD*)malloc( NB_B );
            memset( RgbO, 255, NB_B );
            ///
            ///
            GetBitmapBits( HBMP_P, 4 * W_OUT * H_OUT, RgbI );
            D2PTR->graythreshConvert( RgbI, W_OUT * H_OUT, IsGrayS, IsBiS );
            SetBitmapBits( HBMP_P, 4 * W_OUT * H_OUT, RgbI );
            ///
            ///
            SendMessage( HW_PIC_O, STM_SETIMAGE, IMAGE_BITMAP,(LPARAM)HBMP_P );
            ///
            ///
            ///
            /// ERODE_TRY
            ///
            ///memcpy( RgbO, RgbI, 4 * W_OUT * H_OUT );
            int NORP = (int)round(D2PTR->NOrdErode * PerWHN);
            if( NORP<1 ){
                NORP = 1;
            }
            ///
            ///
            ///
            memset( RgbO, 255, NB_B );
            for( int y=0; y<H_OUT; y++ ){
                memcpy( &RgbO[ (y+NORP)*(W_OUT+2*NORP) + NORP ], &RgbI[y*W_OUT], 4*W_OUT );
            }
            ///
            ///
            ZeroMemory( RGBStrel, 4 * 1024 * 1024 - 256 );
            DSP_MNG.imopen( RgbO, (W_OUT+2*NORP), (H_OUT+2*NORP), HistE, NORP, D2PTR->OptErode, D2PTR->BFront,
                                                        RGBStrel, &W_P_H );
            ///
            ///
            for( int y=0; y<H_OUT; y++ ){
                memcpy( &RgbO[y*W_OUT], &RgbO[ (y+NORP)*(W_OUT+2*NORP) + NORP ], 4*W_OUT );
            }
            ///
            ///
            DeleteObject( HBMP_STREL );
            HBMP_STREL = D2PTR->NewBitmap( W_P_H, W_P_H );
            SetBitmapBits( HBMP_STREL, 4 * W_P_H * W_P_H, RGBStrel );
            HBITMAP HBM_T = D2PTR->Redim_PIC( HBMP_STREL, WH32P, WH32P );
            DeleteObject( HBMP_STREL );
            HBMP_STREL = HBM_T;
            ////
            GetBitmapBits( HBMP_STREL, 4 * WH32P * WH32P, RGBStrel );
            D2PTR->graythreshConvert( RGBStrel, WH32P * WH32P, FALSE, TRUE );
            SetBitmapBits( HBMP_STREL, 4 * WH32P * WH32P, RGBStrel );
            ///
            SendMessage( HW_PIC_STREL, STM_SETIMAGE, IMAGE_BITMAP,(LPARAM)HBMP_STREL );
            ///
            ///
            ///
            ///
            SetBitmapBits( HBMP_P, 4 * W_OUT * H_OUT, RgbO );
            SendMessage( HW_PIC_O, STM_SETIMAGE, IMAGE_BITMAP,(LPARAM)HBMP_P );
            ///
            ///
            ///
            ///
            ///
            ///
            ///
            ///
            ///
            ///
            ///
            ///
            /// CenterWindow( hDlg, 0, 0 );
            ///
            ///
            ///
            ///
            ///
            ///
            ///
            RECT RecOut = {0};
            CenterWindow( hDlg, 0, 0, &RecOut );
            ///
            HW_BF = CreateWindowEx( 0, "BUTTON", NULL,
                                WS_TABSTOP | BS_AUTOCHECKBOX | WS_BORDER | WS_VISIBLE | WS_CHILD | BS_BITMAP,
                               RecOut.right - XPF, 30,
                               XPF - 4, 46,
                               hDlg, (HMENU)2000, GetModuleHandleA(NULL), NULL );
            HBMP_FRONTIER = LoadImageR( TH_W_PIC, MAKEINTRESOURCEA(FILE_RES) );
            SendMessage( HW_BF, BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)HBMP_FRONTIER );
            SendMessage( GetDlgItem(hDlg, 2000), BM_SETCHECK, BST_CHECKED * D2PTR->BFront, BST_CHECKED );
            ///
            ///
            ///
            ///
            ///
            ///
            ///
            ///
            }
            return TRUE;
        case WM_COMMAND:
            if( LOWORD(wParam)==IDCANCEL && D2PTR==NULL ){
                EndDialog( hDlg, FALSE );
            }
            if( D2PTR==NULL ){
                return FALSE;
            }
            ///
            ///
            switch( LOWORD(wParam) ){
                ///
                ///
                ///
                ///
                ///
                case 512:{
                    if( HIWORD(wParam)!=CBN_SELENDOK ){
                        break;
                    }
                    D2PTR->OptErode = SendMessage( HW_LB_PRC, CB_GETCURSEL, 0, (LPARAM)0 );
                    sprintf( T2T, "%s", Text20[D2PTR->OptErode] );
                    SetWindowTextA( GetDlgItem(hDlg,1024), T2T );

            ///
            ///
            ///
            /// ERODE_TRY
            ///
            memcpy( RgbO, RgbI, 4 * W_OUT * H_OUT );
            int NORP = (int)round(D2PTR->NOrdErode * PerWHN);
            if( NORP<1 ){
                NORP = 1;
            }
            ZeroMemory( RGBStrel, 4 * 1024 * 1024 - 256 );

            ///
            ///
            ///
            memset( RgbO, 255, NB_B );
            for( int y=0; y<H_OUT; y++ ){
                memcpy( &RgbO[ (y+NORP)*(W_OUT+2*NORP) + NORP ], &RgbI[y*W_OUT], 4*W_OUT );
            }
            ///
            ///
            ZeroMemory( RGBStrel, 4 * 1024 * 1024 - 256 );
            DSP_MNG.imopen( RgbO, (W_OUT+2*NORP), (H_OUT+2*NORP), HistE, NORP, D2PTR->OptErode, D2PTR->BFront,
                                                        RGBStrel, &W_P_H );
            ///
            ///
            for( int y=0; y<H_OUT; y++ ){
                memcpy( &RgbO[y*W_OUT], &RgbO[ (y+NORP)*(W_OUT+2*NORP) + NORP ], 4*W_OUT );
            }
            ///
            ///
            ///
            ///
            DeleteObject( HBMP_STREL );
            HBMP_STREL = D2PTR->NewBitmap( W_P_H, W_P_H );
            SetBitmapBits( HBMP_STREL, 4 * W_P_H * W_P_H, RGBStrel );
            HBITMAP HBM_T = D2PTR->Redim_PIC( HBMP_STREL, WH32P, WH32P );
            DeleteObject( HBMP_STREL );
            HBMP_STREL = HBM_T;
            ////
            GetBitmapBits( HBMP_STREL, 4 * WH32P * WH32P, RGBStrel );
            D2PTR->graythreshConvert( RGBStrel, WH32P * WH32P, FALSE, TRUE );
            SetBitmapBits( HBMP_STREL, 4 * WH32P * WH32P, RGBStrel );
            ///
            SendMessage( HW_PIC_STREL, STM_SETIMAGE, IMAGE_BITMAP,(LPARAM)HBMP_STREL );
            ///
            ///
            ///
            ///
            SetBitmapBits( HBMP_P, 4 * W_OUT * H_OUT, RgbO );
            SendMessage( HW_PIC_O, STM_SETIMAGE, IMAGE_BITMAP,(LPARAM)HBMP_P );
                }
                break;
                ///
                ///
                ///
                ///
                ///
                ///
                case 256:{
                    if( HIWORD(wParam)==EN_KILLFOCUS ){
                        GetWindowTextA( GetDlgItem(hDlg,256), T2T, 8 );
                        if( T2T[0]==0 || atoi(T2T)==0 ){
                            sprintf( T2T, "%i", D2PTR->NOrdErode );
                            SetWindowTextA( GetDlgItem(hDlg,256), T2T );
                        }
                        break;
                    }
                    if( GetFocus()!=GetDlgItem(hDlg,256) ){
                        break;
                    }
                    if( HIWORD(wParam)==EN_CHANGE ){
                        /// MessageBoxA( hDlg, "", "", 0 );
                        GetWindowTextA( (HWND)lParam, T2T, 8 );
                        if( T2T[0]!=0 ){
                            if( atoi(T2T)==0 ){
                                sprintf( T2T, "%i", D2PTR->NOrdErode );
                                //SetWindowTextA( (HWND)lParam, T2T );
                            }else{
                                D2PTR->NOrdErode = atoi(T2T);
                                ///
                                ///

            ///
            ///
            ///
            /// ERODE_TRY
            ///
            memcpy( RgbO, RgbI, 4 * W_OUT * H_OUT );
            int NORP = (int)round(D2PTR->NOrdErode * PerWHN);
            if( NORP<1 ){
                NORP = 1;
            }
            ZeroMemory( RGBStrel, 4 * 1024 * 1024 - 256 );

            ///
            ///
            ///
            memset( RgbO, 255, NB_B );
            for( int y=0; y<H_OUT; y++ ){
                memcpy( &RgbO[ (y+NORP)*(W_OUT+2*NORP) + NORP ], &RgbI[y*W_OUT], 4*W_OUT );
            }
            ///
            ///
            ZeroMemory( RGBStrel, 4 * 1024 * 1024 - 256 );
            DSP_MNG.imopen( RgbO, (W_OUT+2*NORP), (H_OUT+2*NORP), HistE, NORP, D2PTR->OptErode, D2PTR->BFront,
                                                        RGBStrel, &W_P_H );
            ///
            ///
            for( int y=0; y<H_OUT; y++ ){
                memcpy( &RgbO[y*W_OUT], &RgbO[ (y+NORP)*(W_OUT+2*NORP) + NORP ], 4*W_OUT );
            }
            ///
            ///
            ///
            ///
            DeleteObject( HBMP_STREL );
            HBMP_STREL = D2PTR->NewBitmap( W_P_H, W_P_H );
            SetBitmapBits( HBMP_STREL, 4 * W_P_H * W_P_H, RGBStrel );
            HBITMAP HBM_T = D2PTR->Redim_PIC( HBMP_STREL, WH32P, WH32P );
            DeleteObject( HBMP_STREL );
            HBMP_STREL = HBM_T;
            ////
            GetBitmapBits( HBMP_STREL, 4 * WH32P * WH32P, RGBStrel );
            D2PTR->graythreshConvert( RGBStrel, WH32P * WH32P, FALSE, TRUE );
            SetBitmapBits( HBMP_STREL, 4 * WH32P * WH32P, RGBStrel );
            ///
            SendMessage( HW_PIC_STREL, STM_SETIMAGE, IMAGE_BITMAP,(LPARAM)HBMP_STREL );
            ///
            ///
            ///
            ///
            SetBitmapBits( HBMP_P, 4 * W_OUT * H_OUT, RgbO );
            SendMessage( HW_PIC_O, STM_SETIMAGE, IMAGE_BITMAP,(LPARAM)HBMP_P );
                                ///
                                ///
                            }
                        }
                    }
                }
                    break;
                ///
                ///
                ///

                ///
                ///
                ///
                ///
                ///
                case 2000:{
                    D2PTR->BFront = (int)( SendMessage( GetDlgItem(hDlg, 2000), BM_GETCHECK, 0, 0)==BST_CHECKED );

            ///
            ///
            ///
            /// ERODE_TRY
            ///
            memcpy( RgbO, RgbI, 4 * W_OUT * H_OUT );
            int NORP = (int)round(D2PTR->NOrdErode * PerWHN);
            if( NORP<1 ){
                NORP = 1;
            }
            ZeroMemory( RGBStrel, 4 * 1024 * 1024 - 256 );

            ///
            ///
            ///
            memset( RgbO, 255, NB_B );
            for( int y=0; y<H_OUT; y++ ){
                memcpy( &RgbO[ (y+NORP)*(W_OUT+2*NORP) + NORP ], &RgbI[y*W_OUT], 4*W_OUT );
            }
            ///
            ///
            ZeroMemory( RGBStrel, 4 * 1024 * 1024 - 256 );
            DSP_MNG.imopen( RgbO, (W_OUT+2*NORP), (H_OUT+2*NORP), HistE, NORP, D2PTR->OptErode, D2PTR->BFront,
                                                        RGBStrel, &W_P_H );
            ///
            ///
            for( int y=0; y<H_OUT; y++ ){
                memcpy( &RgbO[y*W_OUT], &RgbO[ (y+NORP)*(W_OUT+2*NORP) + NORP ], 4*W_OUT );
            }
            ///
            ///
            ///
            ///
            DeleteObject( HBMP_STREL );
            HBMP_STREL = D2PTR->NewBitmap( W_P_H, W_P_H );
            SetBitmapBits( HBMP_STREL, 4 * W_P_H * W_P_H, RGBStrel );
            HBITMAP HBM_T = D2PTR->Redim_PIC( HBMP_STREL, WH32P, WH32P );
            DeleteObject( HBMP_STREL );
            HBMP_STREL = HBM_T;
            ////
            GetBitmapBits( HBMP_STREL, 4 * WH32P * WH32P, RGBStrel );
            D2PTR->graythreshConvert( RGBStrel, WH32P * WH32P, FALSE, TRUE );
            SetBitmapBits( HBMP_STREL, 4 * WH32P * WH32P, RGBStrel );
            ///
            SendMessage( HW_PIC_STREL, STM_SETIMAGE, IMAGE_BITMAP,(LPARAM)HBMP_STREL );
            ///
            ///
            ///
            ///
            SetBitmapBits( HBMP_P, 4 * W_OUT * H_OUT, RgbO );
            SendMessage( HW_PIC_O, STM_SETIMAGE, IMAGE_BITMAP,(LPARAM)HBMP_P );
                }
                break;
                ///
                ///
                ///
                ///
                ///
                ///
                ///
                ///
                ///
                ///
                ///
                case IDCANCEL:
                case IDOK:
                    D2PTR = NULL;
                    free( RgbI );
                    free( RgbO );
                    free( RGBStrel );
                    DestroyWindow( HW_BF );
                    DeleteObject( HBMP_FRONTIER );
                    DeleteObject( HBMP_O );
                    DeleteObject( HBMP_STREL );
                    DeleteObject( HBMP_P );
                    EndDialog( hDlg, TRUE );
                    break;
            }
            return TRUE;
        case WM_CLOSE:
            D2PTR = NULL;
            free( RgbI );
            free( RgbO );
            free( RGBStrel );
            DestroyWindow( HW_BF );
            DeleteObject( HBMP_FRONTIER );
            DeleteObject( HBMP_O );
            DeleteObject( HBMP_STREL );
            DeleteObject( HBMP_P );
            EndDialog( hDlg, TRUE );
            EndDialog( hDlg, TRUE );
            break;

    }
    return FALSE;
}

/// //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CALLBACK SELECT_CLOSE(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam){
    /// static HWND             HW_INFO2  = NULL;
    static HWND             HW_LB_PRC       = NULL;
    static HWND             HW_PIC_O        = NULL;
    static HWND             HW_PIC_STREL    = NULL;
    static HWND             HW_BF           = NULL;
    static HBITMAP          HBMP_O          = NULL;
    static HBITMAP          HBMP_STREL      = NULL;
    static HBITMAP          HBMP_P          = NULL;
    static HBITMAP          HBMP_FRONTIER   = NULL;
    static RGBQUAD*         RgbI            = NULL;
    static RGBQUAD*         RgbO            = NULL;
    static RGBQUAD*         RgbT            = NULL;
    static RGBQUAD*         RGBStrel        = NULL;
    static DIM2*            D2PTR           = NULL;
    static int              W_OUT           = 0;
    static int              H_OUT           = 0;
    static double           HistE[256]      = {0};
    static int              W_P_H           = 0;
    static double           PerWHN          = 1;
    static char             Text20[8][16] = { "Radio:", "Lado:", "Radio:" };
    char T2T[1024] = {0};
    static int WH32P = 58;
    const BOOL ExBFront = FALSE;
    switch(msg){
        case WM_INITDIALOG:{
            SetWindowTextA( hDlg, " Cierre binario (Imagen de prueba) " );
            D2PTR = (DIM2*)lParam;
            ///
            ///
            ///
            if( D2PTR->OptErode>8 ){
                D2PTR->OptErode = 0;
            }
            ///
            if( D2PTR->NOrdErode<1 ){
                D2PTR->NOrdErode = 1;
            }
            ///
            ShowWindow( GetDlgItem( hDlg, 1025 ), 0 );
            ShowWindow( GetDlgItem( hDlg,  257 ), 0 );
            if( D2PTR==NULL ){
                CenterWindow( hDlg, 0, 0 );return FALSE;
            }
            HW_PIC_O = GetDlgItem( hDlg, 101 );
            SetFocus( GetDlgItem( hDlg, IDOK ) );
            HW_PIC_STREL = GetDlgItem( hDlg, 102 );
            /// HW_INFO2 = GetDlgItem( hDlg, 64 );
            HW_LB_PRC = GetDlgItem( hDlg, 512 );
            SendMessage( HW_LB_PRC, CB_ADDSTRING, 0, (LPARAM)"BALL");
            SendMessage( HW_LB_PRC, CB_ADDSTRING, 0, (LPARAM)"SQUARE");
            SendMessage( HW_LB_PRC, CB_ADDSTRING, 0, (LPARAM)"DIAMOND");
            SendMessage( HW_LB_PRC, CB_SETCURSEL, D2PTR->OptErode, (LPARAM)D2PTR->OptErode );
            ///
            ///
            ///
            ///
            ///
            ///
            ///
            RGBStrel = (RGBQUAD*)malloc( 4 * 1024 * 1024 );
            ///
            ///
            ///
            sprintf( T2T, "%i", D2PTR->NOrdErode );
            SetWindowTextA( GetDlgItem(hDlg,256), T2T );
            ///
            sprintf( T2T, "%s", Text20[D2PTR->OptErode] );
            SetWindowTextA( GetDlgItem(hDlg,1024), T2T );
            ///
            ///
            ///
            ///
            ///
            BOOL IsGrayS = TRUE;
            BOOL IsBiS = TRUE;
            if( D2PTR->GetInput_Try()->IsLoaded &&
                    D2PTR->GetInput_Try()->ProcType!=NO_PROC_Signal_2D ){
                IsGrayS = D2PTR->GetInput_Try()->IsGray;
                IsBiS   = D2PTR->GetInput_Try()->IsBin;
                D2PTR->GetInput_Try()->ReadDatas();

                if( D2PTR->GetInput_Try()->ProcType==BW_LABEL ){
                    int NPosBuffT = D2PTR->GetInput_Try()->WPIC_OUT * D2PTR->GetInput_Try()->HPIC_OUT;
                    for( int i=0; i<(int)NPosBuffT; i++ ){
                        D2PTR->GetInput_Try()->RgbBuff[i].rgbRed   =
                        D2PTR->GetInput_Try()->RgbBuff[i].rgbGreen =
                        D2PTR->GetInput_Try()->RgbBuff[i].rgbBlue  = 255 * (int)((D2PTR->GetInput_Try()->RgbBuff[i].rgbRed!=255) ||
                                                                                (D2PTR->GetInput_Try()->RgbBuff[i].rgbGreen!=255) ||
                                                        (D2PTR->GetInput_Try()->RgbBuff[i].rgbBlue!=255));
                    }
                }

                HBMP_O = D2PTR->NewBitmap( D2PTR->GetInput_Try()->WPIC_OUT, D2PTR->GetInput_Try()->HPIC_OUT );
                SetBitmapBits( HBMP_O, 4 * D2PTR->GetInput_Try()->WPIC_OUT * D2PTR->GetInput_Try()->HPIC_OUT,
                               D2PTR->GetInput_Try()->RgbBuff );
                D2PTR->GetInput_Try()->FreeBuff();
                PerWHN = D2PTR->GetInput_Try()->WPIC_OUT;
            }else{
                IsGrayS = D2PTR->D2_SOURCE->IsGray;
                IsBiS   = D2PTR->D2_SOURCE->IsBin;
                D2PTR->D2_SOURCE->ReadDatas();
                HBMP_O = D2PTR->NewBitmap( D2PTR->D2_SOURCE->WPIC_OUT, D2PTR->D2_SOURCE->HPIC_OUT );
                SetBitmapBits( HBMP_O, 4 * D2PTR->D2_SOURCE->WPIC_OUT * D2PTR->D2_SOURCE->HPIC_OUT,
                               D2PTR->D2_SOURCE->RgbBuff );
                D2PTR->D2_SOURCE->FreeBuff();
                PerWHN = D2PTR->D2_SOURCE->WPIC_OUT;
            }
            ///
            ///
            ///
            ///
            ///
            ///
            WH32P = 74;
            ///
            ///
            int WP = 0;
            int HP = 0;
            WindowSize( HW_PIC_O, &WP, &HP );
            W_OUT = WP;
            H_OUT = (int)( round( (W_OUT * D2PTR->HPIC_OUT)/((double)D2PTR->WPIC_OUT) ) );
            const int Y_P_PIC = 79;
            if( H_OUT>640 ){
                H_OUT = HP;
                W_OUT =(int)( round( (H_OUT * D2PTR->WPIC_OUT)/((double)D2PTR->HPIC_OUT) ) );
                MoveWindow( hDlg, 1, 66, WP+10, H_OUT + (Y_P_PIC+33), TRUE );
                MoveWindow( HW_PIC_O, 1, Y_P_PIC, WP+2, H_OUT, TRUE );
            }else{
                MoveWindow( hDlg, 1, 66, W_OUT+10, H_OUT + (Y_P_PIC+33), TRUE );
                MoveWindow( HW_PIC_O, 1, Y_P_PIC, W_OUT+2, H_OUT, TRUE );
            }
            ///
            PerWHN = ( W_OUT/( (double)PerWHN ) );
            ///
            HBMP_P = D2PTR->Redim_PIC( HBMP_O, W_OUT, H_OUT );
            DeleteObject( HBMP_O );
            ///
            ///
            ///
            RgbI = (RGBQUAD*)malloc( 4 * W_OUT * H_OUT );
            ZeroMemory( RgbI, 4 * W_OUT * H_OUT );
            ///
            RgbO = (RGBQUAD*)malloc( 4 * (W_OUT+1024) * (H_OUT+1024) );
            ZeroMemory( RgbO, 4 * (W_OUT+1024) * (H_OUT+1024) );
            ///
            RgbT = (RGBQUAD*)malloc( 4 * (W_OUT+1024) * (H_OUT+1024) );
            memset( RgbT, 255, 4 * (W_OUT+1024) * (H_OUT+1024) );
            ///
            GetBitmapBits( HBMP_P, 4 * W_OUT * H_OUT, RgbI );
            D2PTR->graythreshConvert( RgbI, W_OUT * H_OUT, IsGrayS, IsBiS );
            ///
            ///
            ///
            ///
            ///
            ///
            ///
            memcpy( RgbO, RgbI, 4 * W_OUT * H_OUT );
            int NORP = (int)round( D2PTR->NOrdErode * PerWHN );
            if( NORP<1 ){
                NORP = 1;
            }
            ZeroMemory( RGBStrel, 4 * 1024 * 1024 - 256 );
            DSP_MNG.imdilate( RgbO, W_OUT, H_OUT, HistE, NORP, D2PTR->OptErode, 0,
                                                        RGBStrel, &W_P_H );
            ///
            ///
            ///
            memset( RgbT, 255, 4 * (W_OUT+1024) * (H_OUT+1024) );
            ///
            ///
            ///
            for( int y=0; y<H_OUT; y++ ){
                memcpy( &RgbT[(y+NORP)*(W_OUT+2*NORP) + NORP], &RgbO[y*W_OUT], 4*W_OUT );
            }
            ///
            ///
            ///
            ///
            DSP_MNG.imerode( RgbT, (W_OUT+2*NORP), (H_OUT+2*NORP), HistE, NORP, D2PTR->OptErode, 0,
                                                        RGBStrel, &W_P_H );
            ///
            if( D2PTR->BFront==0 || ExBFront ){
                for( int y=0; y<H_OUT; y++ ){
                    memcpy( &RgbO[y*W_OUT], &RgbT[(y+NORP)*(W_OUT+2*NORP) + NORP], 4*W_OUT );
                }
            }else{
                RGBQUAD* RgbDe = NULL;
                RGBQUAD* RgbF = NULL;
                RGBQUAD* RgbP = NULL;
                for( int y=0; y<H_OUT; y++ ){
                    RgbP = &RgbI[y*W_OUT];
                    RgbDe = &RgbO[y*W_OUT];
                    RgbF = &RgbT[(y+NORP)*(W_OUT+2*NORP) + NORP];
                    for( int x=0; x<W_OUT; x++ ){
                        ///
                        ///
                        RgbDe->rgbRed   =
                        RgbDe->rgbGreen =
                        RgbDe->rgbBlue  = 255 * (int)( RgbF->rgbRed>0 && (RgbP->rgbRed<1) );
                        ///
                        ///
                        RgbDe++;
                        RgbP++;
                        RgbF++;
                        /// memcpy( &RgbO[y*W_OUT], &RgbT[(y+NORP)*(W_OUT+NORP) + NORP], 4*W_OUT );
                        ///
                        ///
                    }
                }
            }
            ///
            ///
            ///
            ///
            ///
            DeleteObject( HBMP_STREL );
            HBMP_STREL = D2PTR->NewBitmap( W_P_H, W_P_H );
            SetBitmapBits( HBMP_STREL, 4 * W_P_H * W_P_H, RGBStrel );
            HBITMAP HBM_T = D2PTR->Redim_PIC( HBMP_STREL, WH32P, WH32P );
            DeleteObject( HBMP_STREL );
            HBMP_STREL = HBM_T;
            ////
            GetBitmapBits( HBMP_STREL, 4 * WH32P * WH32P, RGBStrel );
            D2PTR->graythreshConvert( RGBStrel, WH32P * WH32P, FALSE, TRUE );
            SetBitmapBits( HBMP_STREL, 4 * WH32P * WH32P, RGBStrel );
            ///
            SendMessage( HW_PIC_STREL, STM_SETIMAGE, IMAGE_BITMAP,(LPARAM)HBMP_STREL );
            ///
            ///
            ///
            ///
            SetBitmapBits( HBMP_P, 4 * W_OUT * H_OUT, RgbO );
            SendMessage( HW_PIC_O, STM_SETIMAGE, IMAGE_BITMAP,(LPARAM)HBMP_P );
            ///
            ///
            ///
            ///
            ///
            ///
            ///
            ///
            ///
            ///
            ///
            ///
            /// CenterWindow( hDlg, 0, 0 );
            ///
            ///
            ///
            ///
            ///
            ///
            ///
            RECT RecOut = {0};
            CenterWindow( hDlg, 0, 0, &RecOut );
            ///
            HW_BF = CreateWindowEx( 0, "BUTTON", NULL,
                                WS_TABSTOP | BS_AUTOCHECKBOX | WS_BORDER | WS_VISIBLE | WS_CHILD | BS_BITMAP,
                               RecOut.right - XPF, 30,
                               XPF - 4, 46,
                               hDlg, (HMENU)2000, GetModuleHandleA(NULL), NULL );
            HBMP_FRONTIER = LoadImageR( TH_B_PIC, MAKEINTRESOURCEA(FILE_RES) );
            SendMessage( HW_BF, BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)HBMP_FRONTIER );
            SendMessage( GetDlgItem(hDlg, 2000), BM_SETCHECK, BST_CHECKED * D2PTR->BFront, BST_CHECKED );
            ShowWindow( HW_BF, (int)(!ExBFront) );
            ///
            ///
            ///
            ///
            ///
            ///
            ///
            ///
            }
            return TRUE;
        case WM_COMMAND:
            if( LOWORD(wParam)==IDCANCEL && D2PTR==NULL ){
                EndDialog( hDlg, FALSE );
            }
            if( D2PTR==NULL ){
                return FALSE;
            }
            ///
            ///
            switch( LOWORD(wParam) ){
                ///
                ///
                ///
                ///
                ///
                case 512:{
                    if( HIWORD(wParam)!=CBN_SELENDOK ){
                        break;
                    }
                    D2PTR->OptErode = SendMessage( HW_LB_PRC, CB_GETCURSEL, 0, (LPARAM)0 );
                    sprintf( T2T, "%s", Text20[D2PTR->OptErode] );
                    SetWindowTextA( GetDlgItem(hDlg,1024), T2T );

            ///
            ///
            ///
            ///
            ///
            ///
            ///
            memcpy( RgbO, RgbI, 4 * W_OUT * H_OUT );
            int NORP = (int)round( D2PTR->NOrdErode * PerWHN );
            if( NORP<1 ){
                NORP = 1;
            }
            ZeroMemory( RGBStrel, 4 * 1024 * 1024 - 256 );
            DSP_MNG.imdilate( RgbO, W_OUT, H_OUT, HistE, NORP, D2PTR->OptErode, 0,
                                                        RGBStrel, &W_P_H );
            ///
            ///
            ///
            memset( RgbT, 255, 4 * (W_OUT+1024) * (H_OUT+1024) );
            ///
            ///
            ///
            for( int y=0; y<H_OUT; y++ ){
                memcpy( &RgbT[(y+NORP)*(W_OUT+2*NORP) + NORP], &RgbO[y*W_OUT], 4*W_OUT );
            }
            ///
            ///
            ///
            ///
            DSP_MNG.imerode( RgbT, (W_OUT+2*NORP), (H_OUT+2*NORP), HistE, NORP, D2PTR->OptErode, 0,
                                                        RGBStrel, &W_P_H );
            ///
            if( D2PTR->BFront==0 || ExBFront ){
                for( int y=0; y<H_OUT; y++ ){
                    memcpy( &RgbO[y*W_OUT], &RgbT[(y+NORP)*(W_OUT+2*NORP) + NORP], 4*W_OUT );
                }
            }else{
                RGBQUAD* RgbDe = NULL;
                RGBQUAD* RgbF = NULL;
                RGBQUAD* RgbP = NULL;
                for( int y=0; y<H_OUT; y++ ){
                    RgbP = &RgbI[y*W_OUT];
                    RgbDe = &RgbO[y*W_OUT];
                    RgbF = &RgbT[(y+NORP)*(W_OUT+2*NORP) + NORP];
                    for( int x=0; x<W_OUT; x++ ){
                        ///
                        ///
                        RgbDe->rgbRed   =
                        RgbDe->rgbGreen =
                        RgbDe->rgbBlue  = 255 * (int)( RgbF->rgbRed>0 && (RgbP->rgbRed<1) );
                        ///
                        ///
                        RgbDe++;
                        RgbP++;
                        RgbF++;
                        /// memcpy( &RgbO[y*W_OUT], &RgbT[(y+NORP)*(W_OUT+NORP) + NORP], 4*W_OUT );
                        ///
                        ///
                    }
                }
            }
            ///
            ///
            ///
            ///
            ///
            DeleteObject( HBMP_STREL );
            HBMP_STREL = D2PTR->NewBitmap( W_P_H, W_P_H );
            SetBitmapBits( HBMP_STREL, 4 * W_P_H * W_P_H, RGBStrel );
            HBITMAP HBM_T = D2PTR->Redim_PIC( HBMP_STREL, WH32P, WH32P );
            DeleteObject( HBMP_STREL );
            HBMP_STREL = HBM_T;
            ////
            GetBitmapBits( HBMP_STREL, 4 * WH32P * WH32P, RGBStrel );
            D2PTR->graythreshConvert( RGBStrel, WH32P * WH32P, FALSE, TRUE );
            SetBitmapBits( HBMP_STREL, 4 * WH32P * WH32P, RGBStrel );
            ///
            SendMessage( HW_PIC_STREL, STM_SETIMAGE, IMAGE_BITMAP,(LPARAM)HBMP_STREL );
            ///
            ///
            ///
            ///
            SetBitmapBits( HBMP_P, 4 * W_OUT * H_OUT, RgbO );
            SendMessage( HW_PIC_O, STM_SETIMAGE, IMAGE_BITMAP,(LPARAM)HBMP_P );
            ///
            ///
            ///
            ///
            ///
            ///
            ///
            ///
            ///
            ///
            ///
                }
                break;
                ///
                ///
                ///
                ///
                ///
                ///
                case 256:{
                    if( HIWORD(wParam)==EN_KILLFOCUS ){
                        GetWindowTextA( GetDlgItem(hDlg,256), T2T, 8 );
                        if( T2T[0]==0 || atoi(T2T)==0 ){
                            sprintf( T2T, "%i", D2PTR->NOrdErode );
                            SetWindowTextA( GetDlgItem(hDlg,256), T2T );
                        }
                        break;
                    }
                    if( GetFocus()!=GetDlgItem(hDlg,256) ){
                        break;
                    }
                    if( HIWORD(wParam)==EN_CHANGE ){
                        /// MessageBoxA( hDlg, "", "", 0 );
                        GetWindowTextA( (HWND)lParam, T2T, 8 );
                        if( T2T[0]!=0 ){
                            if( atoi(T2T)==0 ){
                                sprintf( T2T, "%i", D2PTR->NOrdErode );
                                //SetWindowTextA( (HWND)lParam, T2T );
                            }else{
                                D2PTR->NOrdErode = atoi(T2T);
                                ///
                                ///

            ///
            ///
            ///
            ///
            ///
            ///
            ///
            memcpy( RgbO, RgbI, 4 * W_OUT * H_OUT );
            int NORP = (int)round( D2PTR->NOrdErode * PerWHN );
            if( NORP<1 ){
                NORP = 1;
            }
            ZeroMemory( RGBStrel, 4 * 1024 * 1024 - 256 );
            DSP_MNG.imdilate( RgbO, W_OUT, H_OUT, HistE, NORP, D2PTR->OptErode, 0,
                                                        RGBStrel, &W_P_H );
            ///
            ///
            ///
            memset( RgbT, 255, 4 * (W_OUT+1024) * (H_OUT+1024) );
            ///
            ///
            ///
            for( int y=0; y<H_OUT; y++ ){
                memcpy( &RgbT[(y+NORP)*(W_OUT+2*NORP) + NORP], &RgbO[y*W_OUT], 4*W_OUT );
            }
            ///
            ///
            ///
            ///
            DSP_MNG.imerode( RgbT, (W_OUT+2*NORP), (H_OUT+2*NORP), HistE, NORP, D2PTR->OptErode, 0,
                                                        RGBStrel, &W_P_H );
            ///
            if( D2PTR->BFront==0 || ExBFront ){
                for( int y=0; y<H_OUT; y++ ){
                    memcpy( &RgbO[y*W_OUT], &RgbT[(y+NORP)*(W_OUT+2*NORP) + NORP], 4*W_OUT );
                }
            }else{
                RGBQUAD* RgbDe = NULL;
                RGBQUAD* RgbF = NULL;
                RGBQUAD* RgbP = NULL;
                for( int y=0; y<H_OUT; y++ ){
                    RgbP = &RgbI[y*W_OUT];
                    RgbDe = &RgbO[y*W_OUT];
                    RgbF = &RgbT[(y+NORP)*(W_OUT+2*NORP) + NORP];
                    for( int x=0; x<W_OUT; x++ ){
                        ///
                        ///
                        RgbDe->rgbRed   =
                        RgbDe->rgbGreen =
                        RgbDe->rgbBlue  = 255 * (int)( RgbF->rgbRed>0 && (RgbP->rgbRed<1) );
                        ///
                        ///
                        RgbDe++;
                        RgbP++;
                        RgbF++;
                        /// memcpy( &RgbO[y*W_OUT], &RgbT[(y+NORP)*(W_OUT+NORP) + NORP], 4*W_OUT );
                        ///
                        ///
                    }
                }
            }
            ///
            ///
            ///
            ///
            ///
            DeleteObject( HBMP_STREL );
            HBMP_STREL = D2PTR->NewBitmap( W_P_H, W_P_H );
            SetBitmapBits( HBMP_STREL, 4 * W_P_H * W_P_H, RGBStrel );
            HBITMAP HBM_T = D2PTR->Redim_PIC( HBMP_STREL, WH32P, WH32P );
            DeleteObject( HBMP_STREL );
            HBMP_STREL = HBM_T;
            ////
            GetBitmapBits( HBMP_STREL, 4 * WH32P * WH32P, RGBStrel );
            D2PTR->graythreshConvert( RGBStrel, WH32P * WH32P, FALSE, TRUE );
            SetBitmapBits( HBMP_STREL, 4 * WH32P * WH32P, RGBStrel );
            ///
            SendMessage( HW_PIC_STREL, STM_SETIMAGE, IMAGE_BITMAP,(LPARAM)HBMP_STREL );
            ///
            ///
            ///
            ///
            SetBitmapBits( HBMP_P, 4 * W_OUT * H_OUT, RgbO );
            SendMessage( HW_PIC_O, STM_SETIMAGE, IMAGE_BITMAP,(LPARAM)HBMP_P );
            ///
            ///
            ///
            ///
            ///
            ///
            ///
            ///
            ///
            ///
            ///
                                ///
                                ///
                            }
                        }
                    }
                }
                    break;
                ///
                ///
                ///
                ///
                ///

                ///
                ///
                ///
                ///
                ///
                case 2000:{
                    D2PTR->BFront = (int)( SendMessage( GetDlgItem(hDlg, 2000), BM_GETCHECK, 0, 0)==BST_CHECKED );
            ///
            ///
            ///
            ///
            ///
            ///
            ///
            ///
            memcpy( RgbO, RgbI, 4 * W_OUT * H_OUT );
            int NORP = (int)round( D2PTR->NOrdErode * PerWHN );
            if( NORP<1 ){
                NORP = 1;
            }
            ZeroMemory( RGBStrel, 4 * 1024 * 1024 - 256 );
            DSP_MNG.imdilate( RgbO, W_OUT, H_OUT, HistE, NORP, D2PTR->OptErode, 0,
                                                        RGBStrel, &W_P_H );
            ///
            ///
            ///
            memset( RgbT, 255, 4 * (W_OUT+1024) * (H_OUT+1024) );
            ///
            ///
            ///
            for( int y=0; y<H_OUT; y++ ){
                memcpy( &RgbT[(y+NORP)*(W_OUT+2*NORP) + NORP], &RgbO[y*W_OUT], 4*W_OUT );
            }
            ///
            ///
            ///
            ///
            DSP_MNG.imerode( RgbT, (W_OUT+2*NORP), (H_OUT+2*NORP), HistE, NORP, D2PTR->OptErode, 0,
                                                        RGBStrel, &W_P_H );
            ///
            if( D2PTR->BFront==0 || ExBFront ){
                for( int y=0; y<H_OUT; y++ ){
                    memcpy( &RgbO[y*W_OUT], &RgbT[(y+NORP)*(W_OUT+2*NORP) + NORP], 4*W_OUT );
                }
            }else{
                RGBQUAD* RgbDe = NULL;
                RGBQUAD* RgbF = NULL;
                RGBQUAD* RgbP = NULL;
                for( int y=0; y<H_OUT; y++ ){
                    RgbP = &RgbI[y*W_OUT];
                    RgbDe = &RgbO[y*W_OUT];
                    RgbF = &RgbT[(y+NORP)*(W_OUT+2*NORP) + NORP];
                    for( int x=0; x<W_OUT; x++ ){
                        ///
                        ///
                        RgbDe->rgbRed   =
                        RgbDe->rgbGreen =
                        RgbDe->rgbBlue  = 255 * (int)( RgbF->rgbRed>0 && (RgbP->rgbRed<1) );
                        ///
                        ///
                        RgbDe++;
                        RgbP++;
                        RgbF++;
                        /// memcpy( &RgbO[y*W_OUT], &RgbT[(y+NORP)*(W_OUT+NORP) + NORP], 4*W_OUT );
                        ///
                        ///
                    }
                }
            }
            ///
            ///
            ///
            ///
            ///
            DeleteObject( HBMP_STREL );
            HBMP_STREL = D2PTR->NewBitmap( W_P_H, W_P_H );
            SetBitmapBits( HBMP_STREL, 4 * W_P_H * W_P_H, RGBStrel );
            HBITMAP HBM_T = D2PTR->Redim_PIC( HBMP_STREL, WH32P, WH32P );
            DeleteObject( HBMP_STREL );
            HBMP_STREL = HBM_T;
            ////
            GetBitmapBits( HBMP_STREL, 4 * WH32P * WH32P, RGBStrel );
            D2PTR->graythreshConvert( RGBStrel, WH32P * WH32P, FALSE, TRUE );
            SetBitmapBits( HBMP_STREL, 4 * WH32P * WH32P, RGBStrel );
            ///
            SendMessage( HW_PIC_STREL, STM_SETIMAGE, IMAGE_BITMAP,(LPARAM)HBMP_STREL );
            ///
            ///
            ///
            ///
            SetBitmapBits( HBMP_P, 4 * W_OUT * H_OUT, RgbO );
            SendMessage( HW_PIC_O, STM_SETIMAGE, IMAGE_BITMAP,(LPARAM)HBMP_P );
            ///
            ///
            ///
            ///
            ///
            ///
            ///
            ///
            ///
            ///
            ///
            ///
            ///
                }
                break;
                ///
                ///
                ///
                ///
                ///
                ///
                ///
                ///
                case IDCANCEL:
                case IDOK:
                    D2PTR = NULL;
                    free( RgbI );
                    free( RgbO );
                    free( RgbT );
                    free( RGBStrel );
                    DestroyWindow( HW_BF );
                    DeleteObject( HBMP_FRONTIER );
                    DeleteObject( HBMP_O );
                    DeleteObject( HBMP_STREL );
                    DeleteObject( HBMP_P );
                    EndDialog( hDlg, TRUE );
                    break;
            }
            return TRUE;
        case WM_CLOSE:
            D2PTR = NULL;
            free( RgbI );
            free( RgbO );
            free( RgbT );
            free( RGBStrel );
            DestroyWindow( HW_BF );
            DeleteObject( HBMP_FRONTIER );
            DeleteObject( HBMP_O );
            DeleteObject( HBMP_STREL );
            DeleteObject( HBMP_P );
            EndDialog( hDlg, TRUE );
            EndDialog( hDlg, TRUE );
            break;

    }
    return FALSE;
}

/// //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// //////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/// //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CALLBACK SELECT_IMFILL(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam ){
    static HWND HW_INFO2 = NULL;
    static HWND HW_LEVEL = NULL;
    static HWND HW_PIC_O = NULL;
    static HBITMAP HBMP_P = NULL;
    static RGBQUAD* RgbI = NULL;
    static RGBQUAD* RgbO = NULL;
    static DIM2* D2PTR = NULL;
    static int W_OUT = 0;
    static int H_OUT = 0;
    static double TH_GTH    = 0;
    char T2T[1024] = {0};
    switch(msg){
        case WM_INITDIALOG:{
            ///
            ///
            ///
            D2PTR = (DIM2*)lParam;
            HW_INFO2 = GetDlgItem( hDlg, 10 );
            HW_PIC_O = GetDlgItem( hDlg, 101 );
            sprintf( T2T, "Nivel de offset: %5.4f(%i)",
                                ((double)D2PTR->LevelBIN)/255, D2PTR->LevelBIN );
            SetWindowText( HW_INFO2, T2T );
            HW_LEVEL = GetDlgItem( hDlg, 100 );
            SCROLLINFO si = {0};
            si.cbSize = sizeof(si);
            si.fMask  = SIF_RANGE | SIF_PAGE | SIF_POS;
            si.nMin   = 0;
            si.nMax   = 255;
            si.nPage  = 0;
            si.nPos   = 0;
            //SetScrollInfo( HW_LEVEL , SB_CTL , &si , TRUE );
            SetScrollRange( HW_LEVEL, SB_CTL,0, 255, TRUE);
            SetScrollPos( HW_LEVEL , SB_CTL , D2PTR->LevelBIN, TRUE );
            // SendDlgItemMessage(hDlg, ID_SCROLLV, SBM_SETRANGE, (WPARAM)0, (LPARAM)50);
            ///
            ///
            ///
            SendDlgItemMessage( hDlg, 512, CB_ADDSTRING, 0, (LPARAM)"Normal" );
            SendDlgItemMessage( hDlg, 512, CB_ADDSTRING, 0, (LPARAM)"Invertido" );
            SendDlgItemMessage( hDlg, 512, CB_SETCURSEL, D2PTR->BOptFill, 0 );
            ///
            ///
            ///
            HBITMAP BMSRC = NULL;
            if( D2PTR->GetInput()->IsLoaded &&
                                    D2PTR->GetInput()->ProcType!=NO_PROC_Signal_2D ){
                D2PTR->GetInput()->ReadDatas();
                if( D2PTR->GetInput()->ProcType==BW_LABEL ){
                    int NPosBuffT = D2PTR->GetInput()->WPIC_OUT * D2PTR->GetInput()->HPIC_OUT;
                    for( int i=0; i<(int)NPosBuffT; i++ ){
                        D2PTR->GetInput()->RgbBuff[i].rgbRed   =
                        D2PTR->GetInput()->RgbBuff[i].rgbGreen =
                        D2PTR->GetInput()->RgbBuff[i].rgbBlue  = 255 * (int)((D2PTR->GetInput()->RgbBuff[i].rgbRed!=255) || (D2PTR->GetInput()->RgbBuff[i].rgbGreen!=255) ||
                                                        (D2PTR->GetInput()->RgbBuff[i].rgbBlue!=255));
                    }
                }
                BMSRC = D2PTR->NewBitmap( D2PTR->GetInput()->WPIC_OUT, D2PTR->GetInput()->HPIC_OUT );
                SetBitmapBits( BMSRC, 4 * D2PTR->GetInput()->WPIC_OUT * D2PTR->GetInput()->HPIC_OUT,
                               D2PTR->GetInput()->RgbBuff );
                D2PTR->GetInput()->FreeBuff();
            }else{
                D2PTR->D2_SOURCE->ReadDatas();
                BMSRC = D2PTR->NewBitmap( D2PTR->D2_SOURCE->WPIC_OUT, D2PTR->D2_SOURCE->HPIC_OUT );
                SetBitmapBits( BMSRC, 4 * D2PTR->D2_SOURCE->WPIC_OUT * D2PTR->D2_SOURCE->HPIC_OUT,
                               D2PTR->D2_SOURCE->RgbBuff );
                D2PTR->D2_SOURCE->FreeBuff();
            }
            ///
            ///
            ///
            ///
            ///
            ///
            BITMAP BmpI = {0};
            GetObject( BMSRC, sizeof(BITMAP) , (LPSTR)&BmpI );
            ///
            int WP = 0;
            int HP = 0;
            ///
            WindowSize( hDlg, &WP, &HP );
            ///
            ///
            ///
            ///
            WindowSize( HW_PIC_O, &WP, &HP );
            ///
            W_OUT = WP;
            H_OUT = (int)( round( (W_OUT * BmpI.bmHeight)/((double)BmpI.bmWidth) ) );
            ///
            if( (H_OUT+99+30)>640 ){
                H_OUT = HP;
                W_OUT =(int)( round( (H_OUT * BmpI.bmWidth)/((double)BmpI.bmHeight) ) );
                /// MoveWindow( HW_PIC_O, 1, 66, WP, HP, TRUE );
            }else{
                MoveWindow( hDlg, 1, 66, W_OUT+8, H_OUT + 99, TRUE );
                MoveWindow( HW_PIC_O, 1, 66, W_OUT, H_OUT, TRUE );
            }
            ///
            HDC ODC = GetDC( 0 );
            HBMP_P = CreateCompatibleBitmap( ODC , W_OUT, H_OUT );
            ReleaseDC( 0, ODC );
            DeleteDC( ODC );
            ///
            HDC hdc = CreateCompatibleDC( GetDC( 0 ) );
            HDC memDC = CreateCompatibleDC( GetDC( 0 ) );
            SelectObject( hdc , BMSRC );/// HBPLOT
            SelectObject( memDC , HBMP_P );/// HB_I
            SetStretchBltMode( memDC , HALFTONE );/// COLORONCOLOR  HALFTONE
            StretchBlt( memDC, 0, 0, W_OUT, H_OUT, hdc, 0, 0, BmpI.bmWidth, BmpI.bmHeight, SRCCOPY );
            ReleaseDC( NULL, hdc );
            ReleaseDC( NULL, memDC );
            DeleteDC( hdc );
            DeleteDC( memDC );
            ///
            DeleteObject( BMSRC );
            ///
            RgbI = (RGBQUAD*)malloc( 4 * W_OUT * H_OUT +1024 );
            ZeroMemory( RgbI, 4 * W_OUT * H_OUT );
            RgbO = (RGBQUAD*)malloc( 4 * W_OUT * H_OUT +1024 );
            ZeroMemory( RgbO, 4 * W_OUT * H_OUT );
            ///
            GetBitmapBits( HBMP_P, 4 * W_OUT * H_OUT, RgbI );
            ///
            ///
            ///
            double Hist[256] = {0};
            ///
            for( int i=0; i<(W_OUT * H_OUT); i++ ){
                RgbI[i].rgbRed =
                RgbI[i].rgbGreen =
                RgbI[i].rgbBlue =
                            (BYTE)round(
                                    0.2989 * RgbI[i].rgbRed +
                                    0.5870 * RgbI[i].rgbGreen +
                                    0.1140 * RgbI[i].rgbBlue
                                    );
                //
                Hist[RgbI[i].rgbRed] = Hist[RgbI[i].rgbRed] + 1;
                //
            }
            ///
            ///
            TH_GTH = round( 255 * D2PTR->graythreshHist( Hist ) );
            ///
            ///
            ///
            ///
            ///
            ///
            ///
            ///
            ///
            ///
            ///
            ///
            ///
            ///
            BYTE BLEV = 0;
            BLEV = (BYTE)round( TH_GTH );
            for( int i=0; i<(W_OUT * H_OUT); i++ ){
                RgbI[i].rgbRed = RgbI[i].rgbGreen = RgbI[i].rgbBlue = 255 * (RgbI[i].rgbRed>BLEV);
            }
            ///
            memcpy( RgbO, RgbI, 4 * (W_OUT * H_OUT) );
            D2PTR->IMFILL( RgbO, W_OUT, H_OUT, D2PTR->BOptFill );
            ///
            ///
            ///
            SetBitmapBits( HBMP_P, 4 * W_OUT * H_OUT, RgbO );
            SendMessage( HW_PIC_O, STM_SETIMAGE, IMAGE_BITMAP,(LPARAM)HBMP_P );
            /// sprintf( T2T, "%ix%i", WP, HP );SetWindowTextA( hDlg, T2T );
            ///
            ///
            CenterWindow( hDlg, 0, 0 );
            ///
            ///
            /// SetFocus( HW_LEVEL );
            ///
            }
            return TRUE;
        case WM_COMMAND:
            switch( LOWORD(wParam) ){
                case 512:
                    if( HIWORD(wParam)==CBN_SELENDOK ){
                        /// MessageBoxA( hDlg, "", 0, 0 );
                        D2PTR->BOptFill = (int)SendDlgItemMessage( hDlg, 512, CB_GETCURSEL, 0, 0 );
                        memcpy( RgbO, RgbI, 4 * (W_OUT * H_OUT) );
                        D2PTR->IMFILL( RgbO, W_OUT, H_OUT, D2PTR->BOptFill );
                        SetBitmapBits( HBMP_P, 4 * W_OUT * H_OUT, RgbO );
                        SendMessage( HW_PIC_O, STM_SETIMAGE, IMAGE_BITMAP,(LPARAM)HBMP_P );
                    }
                    break;
                ///

                ///
                case IDOK:
                    free( RgbI );
                    free( RgbO );
                    D2PTR = NULL;
                    DeleteObject( HBMP_P);
                    EndDialog( hDlg, TRUE );
                    break;
                case IDCANCEL:
                    free( RgbI );
                    free( RgbO );
                    DeleteObject( HBMP_P);
                    D2PTR = NULL;
                    EndDialog( hDlg, FALSE );
                    break;
            }
            return TRUE;
        case WM_CLOSE:
            free( RgbI );
            free( RgbO );
            DeleteObject( HBMP_P);
            EndDialog( hDlg, TRUE );
            break;

    }
    return FALSE;
}
