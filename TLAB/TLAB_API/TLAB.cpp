#if (_WIN32_WINNT < 0x0500)
    #undef _WIN32_WINNT
    #define  _WIN32_WINNT 0x0501
    #define  _WIN32_IE    0x0500
#endif
#include <windows.h>
#include <math.h>
#include <shlwapi.h>
#include <commctrl.h>
#include <stdio.h>

const int NMEGAS = 16;
const int MaxFileSize = (NMEGAS*1024*1024);
const int MaxProcs = 16;

#include "OurCrashHandler.cpp"

HWND* HWN_MAIN = (HWND*)GetNull();

#include "IMAGE_API\IMLIB.h"
#include "SAVE_AS_JPG.cpp"



#include "IMAGE_API\IMLIB.h"
#include "DEFINE.h"
#include "WINAPI_FUNCS.cpp"
#include "PLOT_CLASS\INCLUDE_PLOT_CLASS.h"

#include "CONTROLES\ImageButton.cpp"
#include "DSP_API\DSP_LOAD.cpp"

/// E:\AIIF\SegAll\CONOCIMIENTO\UNIVERSIDAD\TESIS\P_NUEVO\APLICACIONES\TLAB\DSP_LAB\ARCHIVOS_DE_PRUEBA
/// TLAB

struct TLAB{
    ///
    LRESULT                 WProcedure                      ( HWND, UINT, WPARAM, LPARAM );
    int                     Create                          ( HWND, UINT, WPARAM, LPARAM );
    int                     CmdProcs                        ( HWND, UINT, WPARAM, LPARAM );
    int                     Procs1D                         ( HWND, UINT, WPARAM, LPARAM );
    int                     Procs2D                         ( HWND, UINT, WPARAM, LPARAM );
    int                     RUN_PROCS                       ( void );
    BOOL                    SaveRestFile                    ( void );
    BOOL                    ReadRestFile                    ( void );
    ///
    int                     Load_All                        ();
    ///
    BOOL                    SaveSesionProc                  ( void );
    BOOL                    OpenSesionProc                  ( void );
    ///
    BOOL                    SaveSesion1D                    ( LPCSTR );
    BOOL                    OpenSesion1D                    ( LPCSTR );
    ///
    BOOL                    SaveSesion2D                    ( LPCSTR );
    BOOL                    OpenSesion2D                    ( LPCSTR );
    ///
    BOOL                    SaveCode1D                      ( void );
    BOOL                    CopyCode1D_2_Clipboard          ( void );
    BOOL                    CopyCode1D                      ( LPSTR, BOOL*, BOOL* );


    BOOL                    SaveCode2D                      ( void );
    BOOL                    CopyCode2D_2_Clipboard          ( void );
    BOOL                    CopyCode2D                      ( LPSTR, BOOL* );
    ///
    BOOL                    SHOW_IMAGE                      ( void );
    BOOL                    SHOW_PLOT                       ( void );
    BOOL                    SAVE_GRAPH_FUNC                 ( void );
    BOOL                    WRITE_ERROR_REPORT              ( void );
    BOOL                    PRUEBAS                         ( void );
    ///
    ///private:
    int                                     IPtr;
    ///
    ///
    /// VENTANAS Y CONTROLES.
    HWND                                    HW_MAIN;
    HWND                                    HWTABS;
    HWND                                    HW_STATUS;
    HWND                                    HW_STATUS_LOW;
    HWND                                    HW_PBAR;
    HWND                                    HW_RUN;
    HWND                                    HW_IMSHOW;
    ImageButton                             B_1D[MaxProcs];
    ImageButton                             B_2D[MaxProcs];
    BOOL                                    AlwOnTop;
    HMENU                                   HMeW;
    BOOL                                    On_Run;
    int                                     TabPage;
    int                                     BSel_1D;
    int                                     BSel_2D;
    HBITMAP                                 HBmpOut;
    int                                     W_BOUT;
    int                                     H_BOUT;
    ///
    public:
    MSG                                     MSG_Cmds;
    BOOL                                    OnCmds;
    HWND                                    hwndC;
    UINT                                    messageC;
    WPARAM                                  wParamC;
    LPARAM                                  lParamC;
    HWND                                    HWProgress;
    HWND                                    HW_Progr;
    char                                    AUXTEXT[1024];
    ///
    ///
    DIM1                                    Dim1Procs[MaxProcs];
    DIM2                                    Dim2Procs[MaxProcs];
    private:
    ///
    ///
    ///
    ///
    PLOTEOS                                 Pltrs;
    PLOTEOS                                 HistPlot;
    char                                    FileName1D[1024];
    char                                    FileName2D[1024];
    BOOL                                    OnSaveSesion;
    public:
    BOOL                                    OnShowPic;
    BOOL                                    OnShowPlot;
    BOOL                                    WaitForClosePICBIG;
    BOOL                                    WaitForClosePLOTBIG;
    HWND                                    HW_PIC_BIG;
    HWND                                    HW_PLOT_BIG;
    BOOL                                    SetPlotimshow;
    public:
    ///
    TLAB(){
        IPtr = 0;
        AlwOnTop = FALSE;
        IsTopGlobal = &AlwOnTop;
        OnCmds = FALSE;
        OnShowPic = FALSE;
        WaitForClosePICBIG = FALSE;
        ///
        SetPlotimshow = TRUE;
        ///
        HWN_MAIN = &HW_MAIN;
        ///
        ZeroMemory( FileName1D, 1023 );
        ZeroMemory( FileName2D, 1023 );
        OnSaveSesion = FALSE;
        ///
        ///
        HW_STATUS_LOW = NULL;
        ///
        TabPage = 0;
        ///
    }
    ///
    int CreateMainWindow(){
        ShowWindow( GetConsoleWindow(), 0 );
        const char szClassName[] = " TLAB ";
        WNDCLASSEX wincl = {0};
        wincl.hInstance = GetModuleHandleA(NULL);
        wincl.lpszClassName = szClassName;
        wincl.lpfnWndProc = WindowProcedure;
        wincl.style = CS_DBLCLKS;
        wincl.cbSize = sizeof (WNDCLASSEX);
        wincl.hIcon = LoadIcon( GetModuleHandleA(NULL), MAKEINTRESOURCEA(TLAB_ICON) );
        wincl.hIconSm = LoadIcon( GetModuleHandleA(NULL), MAKEINTRESOURCEA(TLAB_ICON) );
        wincl.hCursor = LoadCursor( NULL, IDC_ARROW );
        wincl.lpszMenuName = MAKEINTRESOURCEA(MAIN_MENU);
        wincl.cbClsExtra = 0;
        wincl.cbWndExtra = 0;
        wincl.hbrBackground = CreateSolidBrush( RGB(0, 0, 0) );/// (HBRUSH) COLOR_BACKGROUND;/// COLOR_WINDOW
        wincl.hbrBackground = CreateSolidBrush( RGB(150, 150, 150) );
        if( !RegisterClassEx (&wincl) ){
            return 0;
        }
        HW_MAIN =
            CreateWindowEx( 0, szClassName, szClassName,
                            ((WS_OVERLAPPEDWINDOW - (WS_MAXIMIZEBOX | WS_SIZEBOX)) | WS_CAPTION ) | DS_CENTER,
                               100, 100,
                               /// 673, 640,
                               673, 625,
                               HWND_DESKTOP, NULL,GetModuleHandleA(NULL), NULL );
        /// /
        /// /
        HACCEL hAcelerador2 = LoadAccelerators( GetModuleHandleA(NULL) , MAKEINTRESOURCE(MAIN_ACCEL) );
        MSG messages = {0};
        /// ShowWindow( HW_MAIN, 1 );
        HMeW = GetMenu( HW_MAIN );
        CheckMenuItem( HMeW, CMD_ON_ALW_ONTOP, 8*int(AlwOnTop) );
        CheckMenuItem( HMeW, CMD_SETPLOTALL, 8*int(SetPlotimshow) );
        ///
        ///
        ///
        ///
        //*
        int W_H = 32;
        /// CMD_CALL_FREE_MAT
        HBITMAP H_B_M0 = LoadImageR( DSP_FREEMAT_ICON, MAKEINTRESOURCEA(FILE_RES) );
        H_B_M0 = imresize( H_B_M0, W_H, W_H, TRUE );
        HBITMAP H_B_M1 = H_B_M0;///LoadImageR( DSP_FREEMAT_ICON, MAKEINTRESOURCEA(FILE_RES) );
        SetMenuItemBitmaps( HMeW, CMD_CALL_FREE_MAT, MF_BYCOMMAND, H_B_M0 , H_B_M1 );
        ///
        ///
        HBITMAP H_B_M2 = LoadImageR( DSP_TADICON_ICON, MAKEINTRESOURCEA(FILE_RES) );
        H_B_M2 = imresize( H_B_M2, W_H, W_H, TRUE );
        HBITMAP H_B_M3 = H_B_M2;
        SetMenuItemBitmaps( HMeW, CMD_OPEN_DSP_TAD, MF_BYCOMMAND, H_B_M2 , H_B_M3 );
        ///
        ///
        HBITMAP H_B_M4 = LoadImageR( DSP_TLABFOLDER_ICON, MAKEINTRESOURCEA(FILE_RES) );
        H_B_M4 = imresize( H_B_M4, W_H, W_H, TRUE );
        HBITMAP H_B_M5 = H_B_M4;
        SetMenuItemBitmaps( HMeW, CMD_OPEN_FOLDER_DSP, MF_BYCOMMAND, H_B_M4 , H_B_M5 );
        //*/
        ///
        ///
        ///
        ///
        ///
        while( GetMessage (&messages, NULL, 0, 0) ){
            /*
            if( IsDialogMessage( HW_MAIN, &messages ) ){
                continue;
            }
            if( IsDialogMessage( HWTABS, &messages ) ){
                continue;
            }
            if( TranslateAccelerator( HW_MAIN, hAcelerador2, &messages ) ){
                continue;
            }
            TranslateMessage(&messages);
            DispatchMessage(&messages);
            //*/
            //*
            /// TranslateMessage(&messages);DispatchMessage(&messages);continue;
            if( !IsDialogMessage( HW_MAIN, &messages ) ){
                TranslateMessage(&messages);
                DispatchMessage(&messages);
            }else{
                if( !TranslateAccelerator( HW_MAIN, hAcelerador2, &messages ) ){ continue; }
            }
            //*/
            ///
            ///
            /*
            if( !TranslateAccelerator( HW_MAIN, hAcelerador2, &messages ) ){
                if( !IsDialogMessage( HW_MAIN, &messages ) ){
                    TranslateMessage(&messages);
                    DispatchMessage(&messages);
                }
            }
            //*/
            ///
            ///
        }
        return 0;
    }
    ///
    ///
    ///
    ///
    HBITMAP imresize( HBITMAP HBMI, int W_OUT, int H_OUT, BOOL DestroyInput ){
        ///
        ///
        HBITMAP HBMP_T = NULL;
        HDC ODC = GetDC( NULL );
        HBMP_T = CreateCompatibleBitmap( ODC , W_OUT, H_OUT );
        ///
        BITMAP BmpI = {0};
        GetObjectA( HBMI, sizeof(BITMAP), &BmpI );
        ///
        HDC hdc = CreateCompatibleDC( ODC );
        HDC memDC = CreateCompatibleDC( ODC );
        ReleaseDC( NULL, ODC );
        SelectObject( hdc , HBMI );/// HBPLOT
        SelectObject( memDC , HBMP_T );/// HB_I
        SetStretchBltMode( memDC , HALFTONE );/// COLORONCOLOR  HALFTONE
        StretchBlt( memDC, 0, 0, W_OUT, H_OUT, hdc, 0, 0, BmpI.bmWidth, BmpI.bmHeight, SRCCOPY );
        DeleteDC( hdc );
        DeleteDC( memDC );
        ///
        ///
        if( DestroyInput ){
            DeleteObject(HBMI);
        }
        return HBMP_T;
    }
    ///
    ///
    BOOL SetTranspBITMAP( HBITMAP HBMIO, BYTE V_BY ){
        BITMAP BMPI = {0};
        GetObjectA( HBMIO, sizeof(BITMAP), &BMPI );
        int MaxP = BMPI.bmWidth * BMPI.bmHeight;
        RGBQUAD* RgbIO = (RGBQUAD*)malloc( 4 * MaxP + 1024 );
        GetBitmapBits( HBMIO, 4 * MaxP, RgbIO );
        RGBQUAD* RgbT = &RgbIO[0];
        for( int i=0; i<MaxP; i++ ){
            if( RgbT->rgbRed==V_BY &&
                            RgbT->rgbGreen==V_BY &&
                                    RgbT->rgbBlue==V_BY          ){
                /// RgbT->rgbRed = 0;RgbT->rgbGreen = 0;RgbT->rgbBlue = 0;
                RgbT->rgbReserved = 255;
                /// MsgBoxF( " " );exit(0);
            }
            RgbT++;
        }
        SetBitmapBits( HBMIO, 4 * MaxP, RgbIO );
        free(RgbIO);
        return TRUE;
    }
    ///
    ///
    ///
    ///
    ///
    BOOL MsgBoxF( LPCSTR szTypes, ... ){
        if( szTypes==NULL ){
            return MessageBox( HW_MAIN, "", " ", 0 );
        }
        char* Text2 = NULL;
        DWORD LCIn = strlen( &szTypes[0] );
        Text2 = (char*)malloc( 1024 * LCIn + 16 );
        va_list arguments = NULL;
        va_start( arguments, szTypes );
        vsnprintf( &Text2[0], 1024 * LCIn, &szTypes[0], arguments );
        int VRet = MessageBox( HW_MAIN, &Text2[0], " ", 0 );
        free( &Text2[0] );
        return VRet;
    }
    ///
    ///
    ///
    ///
    BOOL SetStatusText( LPCSTR szTypes, ... ){
        if( szTypes==NULL ){
            return SetWindowTextA( HW_STATUS, "" );
        }
        char* Text2 = NULL;
        DWORD LCIn = strlen( &szTypes[0] );
        Text2 = (char*)malloc( 1024 * LCIn + 16 );
        va_list arguments = NULL;
        va_start( arguments, szTypes );
        vsnprintf( &Text2[0], 1024 * LCIn, &szTypes[0], arguments );
        int VRet = (int)SetWindowTextA( HW_STATUS, &Text2[0] );
        free( &Text2[0] );
        return VRet;
    }
    ///
    ///
    ///
    BOOL SetOptionBSel(){
        char T2TT[1024];
        char TTEMP[1024];
        ZeroMemory( T2TT, 1023 );
        ZeroMemory( TTEMP, 1023 );
        switch( Dim1Procs[BSel_1D].ProcType ){
            case FFT_Signal:
                if( !Dim1Procs[BSel_1D].IsLoaded ){
                    Pltrs.FillGrid();
                    Pltrs.FillBuff();
                    Pltrs.SetTextMiddle( "Sin procesar, presione ejecutar." );
                    SetStatusText( "Módulo %i, Sin procesar, presione ejecutar.\n"
                                  "FFT( Transformada rápida de FOURIER )", BSel_1D );
                    Pltrs.Redraw();
                    break;
                }
                SetStatusText( "Módulo %i, FFT( Transformada rápida de FOURIER )", BSel_1D );
                Pltrs.SetBitmap( Dim1Procs[BSel_1D].GetPicPlot(), FALSE );
                ShowWindow( Pltrs.GetHWSpect(), 0 );
                Pltrs.SetMaxYLim( Dim1Procs[BSel_1D].MaxValFFT );
                Pltrs.SetMinYLim( 0 );
                Pltrs.TextMx = "";//"Hz ";
                Pltrs.TextMy = NULL;
                Pltrs.SetMaxXLim( Dim1Procs[BSel_1D].SLOAD.Fs/2, FALSE );
                Pltrs.Text2ShowF( " Frecuencia (Hz) " );
                break;
            case WIGNER_Signal:
                if( !Dim1Procs[BSel_1D].IsLoaded ){
                    Pltrs.FillGrid();
                    Pltrs.FillBuff();
                    Pltrs.SetTextMiddle( "Sin procesar, presione ejecutar." );
                    SetStatusText( "Módulo %i, Sin procesar, presione ejecutar.\n"
                                  "WINGER-VILLE", BSel_1D );
                    Pltrs.Redraw();
                    break;
                }
                SetStatusText( "Módulo %i, WINGER-VILLE", BSel_1D );
                Pltrs.SetBitmap( Dim1Procs[BSel_1D].GetPicPlot(), TRUE );
                ShowWindow( Pltrs.GetHWSpect(), 1 );
                Pltrs.SetMaxYLim( Dim1Procs[BSel_1D].SLOAD.Fs/2 );Pltrs.SetMinYLim( 0 );
                Pltrs.TextMy = " Frecuencia (Hz) ";//"Hz ";
                Pltrs.SetMaxXLim( (int)(Dim1Procs[BSel_1D].NPosBuff/Dim1Procs[BSel_1D].SLOAD.Fs), FALSE );
                Pltrs.Text2ShowF( " Tiempo (Seg) " );
                break;
            case CHOI_WIL_Signal:
                if( !Dim1Procs[BSel_1D].IsLoaded ){
                    Pltrs.FillGrid();
                    Pltrs.FillBuff();
                    Pltrs.SetTextMiddle( "Sin procesar, presione ejecutar." );
                    SetStatusText( "Módulo %i, Sin procesar, presione ejecutar.\n"
                                  "CHOI-WILLIAM", BSel_1D );
                    Pltrs.Redraw();
                    break;
                }
                SetStatusText( "Módulo %i, CHOI-WILLIAM", BSel_1D );
                Pltrs.SetBitmap( Dim1Procs[BSel_1D].GetPicPlot(), TRUE );
                ShowWindow( Pltrs.GetHWSpect(), 1 );
                Pltrs.SetMaxYLim( Dim1Procs[BSel_1D].SLOAD.Fs/2 );Pltrs.SetMinYLim( 0 );
                Pltrs.TextMy = " Frecuencia (Hz) ";//"Hz ";
                Pltrs.SetMaxXLim( (int)(Dim1Procs[BSel_1D].NPosBuff/Dim1Procs[BSel_1D].SLOAD.Fs), FALSE );
                Pltrs.Text2ShowF( " Tiempo (Seg) " );
                break;
            case SPECTOGRAM_Signal:
                if( !Dim1Procs[BSel_1D].IsLoaded ){
                    Pltrs.FillGrid();
                    Pltrs.FillBuff();
                    Pltrs.SetTextMiddle( "Sin procesar, presione ejecutar." );
                    SetStatusText( "Módulo %i, Sin procesar, presione ejecutar.\n"
                                  "ESPECTOGRAMA", BSel_1D );
                    Pltrs.Redraw();
                    break;
                }
                SetStatusText( "Módulo %i, ESPECTOGRAMA", BSel_1D );
                Pltrs.SetBitmap( Dim1Procs[BSel_1D].GetPicPlot(), TRUE );
                ShowWindow( Pltrs.GetHWSpect(), 1 );
                Pltrs.SetMaxYLim( Dim1Procs[BSel_1D].SLOAD.Fs/2 );Pltrs.SetMinYLim( 0 );
                Pltrs.TextMy = " Frecuencia (Hz) ";//"Hz ";
                Pltrs.SetMaxXLim( (int)(Dim1Procs[BSel_1D].NPosBuff/Dim1Procs[BSel_1D].SLOAD.Fs), FALSE );
                Pltrs.Text2ShowF( " Tiempo (Seg) " );
                /*
                Pltrs.PLOT_SPECTOGRAM( Dim1Procs[BSel_1D].FDS.SPecto.Get_Spect_Out(),
                                       Dim1Procs[BSel_1D].FDS.SPecto.Get_xs(),
                                        Dim1Procs[BSel_1D].FDS.SPecto.Get_ys()/2, TRUE );
                //*/
                break;
            case PLAY_Signal:
                if( !Dim1Procs[BSel_1D].IsLoaded ){
                    Pltrs.FillGrid();
                    Pltrs.FillBuff();
                    Pltrs.SetTextMiddle( "Sin procesar, presione ejecutar." );
                    SetStatusText( "Módulo %i, Sin procesar, presione ejecutar.\n"
                                  "Reproducir auido.", BSel_1D );
                    Pltrs.Redraw();
                    break;
                }
                /// Dim1Procs[BSel_1D].PlaySignal();
                Pltrs.FillGrid();
                Pltrs.FillBuff();
                if( Dim1Procs[BSel_1D].OnPlay ){
                    Pltrs.SetTextMiddle( "Reproduciendo, Doble click para modificar." );
                }else{
                    Pltrs.SetTextMiddle( "Detenido, Doble click para modificar." );
                }
                Pltrs.Redraw();
                T2TT[0] = 0;
                for( int i=0; i<MaxProcs; i++ ){
                    if( Dim1Procs[BSel_1D].PLAYSELECTED[i] &&
                            (Dim1Procs[BSel_1D].D1_INIT[i].ProcType==Windowing_Signal ||
                             Dim1Procs[BSel_1D].D1_INIT[i].ProcType==Filt_Signal || i==0 )
                                                                                ){
                        sprintf( TTEMP, "%i, ", i );
                        strcat( T2TT, TTEMP );
                    }
                }
                SetStatusText( "Módulo %i, Reproducir auido. Módulos (%s)", BSel_1D, T2TT );
                break;
            case NO_PROC_Signal:
                Pltrs.FillGrid();
                Pltrs.FillBuff();
                Pltrs.SetTextMiddle( "Sin procedimiento, doble click para modificar." );
                SetStatusText( "Sin procedimiento, doble click para modificar." );
                Pltrs.Redraw();
                break;
            default:
                if( Dim1Procs[BSel_1D].ProcType==Graph_Signal ){
                    if( !Dim1Procs[BSel_1D].IsLoaded ){
                        Pltrs.FillGrid();
                        Pltrs.FillBuff();
                        Pltrs.SetTextMiddle( "Sin procesar, presione ejecutar." );
                        SetStatusText( "Módulo %i, Sin procesar, presione ejecutar.\n"
                                      "Graficar", BSel_1D );
                        Pltrs.Redraw();
                        break;
                    }
                    SetStatusText( "Módulo %i, Graficar", BSel_1D );
                }
                if( Dim1Procs[BSel_1D].ProcType==Filt_Signal ){
                    if( !Dim1Procs[BSel_1D].IsLoaded ){
                        Pltrs.FillGrid();
                        Pltrs.FillBuff();
                        Pltrs.SetTextMiddle( "Sin procesar, presione ejecutar." );
                        SetStatusText( "Módulo %i, presione ejecutar. "
                                      "Filtro %s %s", BSel_1D, Dim1Procs[BSel_1D].FDS.GetNameFilter(),
                                      Dim1Procs[BSel_1D].FDS.GetFilterInfo(T2TT) );
                        Pltrs.Redraw();
                        break;
                    }
                    SetStatusText( "Módulo %i, "
                                  "Filtro %s %s", BSel_1D, Dim1Procs[BSel_1D].FDS.GetNameFilter(),
                                  Dim1Procs[BSel_1D].FDS.GetFilterInfo(T2TT) );
                    ///
                    ///
                    /// SetStatusText( "%g", Dim1Procs[BSel_1D].MaxValFFT );
                }
                if( Dim1Procs[BSel_1D].ProcType==Windowing_Signal ){
                    if( !Dim1Procs[BSel_1D].IsLoaded ){
                        Pltrs.FillGrid();
                        Pltrs.FillBuff();
                        Pltrs.SetTextMiddle( "Sin procesar, presione ejecutar." );
                        SetStatusText( "Módulo %i, Sin procesar, presione ejecutar.\n"
                                      "Ventana %s", BSel_1D,
                                      Dim1Procs[BSel_1D].FDS.WINDWN.GetWinInfo(T2TT) );
                        Pltrs.Redraw();
                        break;
                    }
                    SetStatusText( "Módulo %i, "
                                      "Ventana %s", BSel_1D,
                                      Dim1Procs[BSel_1D].FDS.WINDWN.GetWinInfo(T2TT) );
                }
                Pltrs.SetMaxYLim( Dim1Procs[BSel_1D].MaxValFFT );
                Pltrs.SetMinYLim( -Dim1Procs[BSel_1D].MaxValFFT );
                Pltrs.SetBitmap( Dim1Procs[BSel_1D].GetPicPlot(), FALSE );
                ShowWindow( Pltrs.GetHWSpect(), 0 );
                /// Pltrs.SetMaxYLim( 1 );Pltrs.SetMinYLim( -1 );
                Pltrs.TextMx = "";
                Pltrs.TextMy = NULL;
                Pltrs.SetMaxXLim( (int)(Dim1Procs[BSel_1D].NPosBuff/Dim1Procs[BSel_1D].SLOAD.Fs), FALSE );
                Pltrs.Text2ShowF( " Tiempo (Seg) " );
                break;
        }
        return TRUE;
    }
    ///
    ///
    ///
    ///
    BOOL SetImageOut( HBITMAP HBMPIN ){
        if( HBMPIN==NULL ){
            return FALSE;
        }
        ///
        BITMAP BmpI = {0};
        GetObject( HBMPIN, sizeof(BITMAP) , (LPSTR)&BmpI );
        int W_OUT = W_BOUT;
        int H_OUT = (int)( round( (W_OUT * BmpI.bmHeight)/((double)BmpI.bmWidth) ) );
        if( H_OUT>H_BOUT ){
            H_OUT = H_BOUT;
            W_OUT = (int)( round( (H_OUT * BmpI.bmWidth)/((double)BmpI.bmHeight) ) );
        }
        ///
        ///
        /// SetStatusText( "%ix%i\n%ix%i", W_OUT, H_OUT, W_BOUT, H_BOUT );
        ///
        ///
        DeleteObject( HBmpOut );
        HDC ODC = GetDC( 0 );
        HBmpOut = CreateCompatibleBitmap( ODC , W_OUT, H_OUT );
        ReleaseDC( 0, ODC );
        DeleteDC( ODC );
        ///
        HDC hdc = CreateCompatibleDC( GetDC( 0 ) );
        HDC memDC = CreateCompatibleDC( GetDC( 0 ) );
        SelectObject( hdc , HBMPIN );/// HBPLOT
        SelectObject( memDC , HBmpOut );/// HB_I
        SetStretchBltMode( memDC , HALFTONE );/// COLORONCOLOR  HALFTONE
        StretchBlt( memDC, 0, 0, W_OUT, H_OUT, hdc, 0, 0, BmpI.bmWidth, BmpI.bmHeight, SRCCOPY );
        ReleaseDC( NULL, hdc );
        ReleaseDC( NULL, memDC );
        DeleteDC( hdc );
        DeleteDC( memDC );
        ///
        SendMessage( HW_IMSHOW, STM_SETIMAGE, IMAGE_BITMAP,(LPARAM)HBmpOut );
        ///
        ///
        return TRUE;
    }
    ///
    ///
    ///
    ///
    BOOL SetImageText( LPCSTR LSTR ){
        HBITMAP HBMPOfW = (HBITMAP)SendMessage( HW_IMSHOW, STM_GETIMAGE, IMAGE_BITMAP, 0 );
        BITMAP BmpI = {0};
        GetObject( HBMPOfW, sizeof(BITMAP) , (LPSTR)&BmpI );
        HDC memDC = CreateCompatibleDC( GetDC( 0 ) );
        SelectObject( memDC , HBMPOfW );/// HB_I
        ///
        SetTextAlign( memDC, TA_CENTER );
        if( LSTR!=NULL ){
            TextOutA( memDC, BmpI.bmWidth/2, BmpI.bmHeight/2, LSTR, strlen(LSTR) );
        }
        ///
        ReleaseDC( NULL, memDC );
        DeleteDC( memDC );
        SendMessage( HW_IMSHOW, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)HBMPOfW );
        ///
        return TRUE;
    }
    ///
    ///
    BOOL SetImageOutN( DIM2* DPTR, LPCSTR LPST1, LPCSTR LPST2 ){
        if( DPTR->GetPicPlot()==NULL || !DPTR->IsLoaded ){
            static HBITMAP BMPVOID = NULL;
            if( BMPVOID!=NULL ){
                DeleteObject( BMPVOID );
            }
            BMPVOID = LoadImageR( NO_PIC, MAKEINTRESOURCEA(FILE_RES) );
            SetImageOut( BMPVOID );
            if( !DPTR->IsLoaded && !(DPTR->ProcType==NO_PROC_Signal_2D) ){
                SetImageText( LPST1 );
            }else{
                if( DPTR->ProcType==NO_PROC_Signal_2D ){
                    SetImageText( LPST2 );
                }
            }
            return FALSE;
        }
        SendMessage( HW_IMSHOW, STM_SETIMAGE, IMAGE_BITMAP,(LPARAM)DPTR->GetPicPlot() );
        return TRUE;
    }
    ///
    ///
    ///BSel_2D
    BOOL SetOptionBSel_2D( void ){
        return SetOptionBSel_2D(BSel_2D, TRUE);
    }
    ///BSel_2D
    BOOL SetOptionBSel_2D( int BSel2D, BOOL SetStT ){
        char T2TT[1024];
        ZeroMemory( T2TT, 1023 );
        sprintf( T2TT, "Módulo %i sin procesar, presione Ejecutar.", BSel2D );
        SetImageOutN( &Dim2Procs[BSel2D],
                     T2TT,
                     "Sin procedimiento, doble click para modificar." );
        HistPlot.SetVisible( Dim2Procs[BSel2D].ProcType==HIST_2DPROC );
        ShowWindow( HW_IMSHOW, (int)(Dim2Procs[BSel2D].ProcType!=HIST_2DPROC) );
        if( SetStT ){
            if( BSel2D==0 ){
                if( !Dim2Procs[BSel2D].IsLoaded ){
                    SetStatusText( "Carga de la imagen, doble click, Ctrl+click ó Ctrl + Espacio para modificar." );
                }else{
                    SetStatusText( "Imagen cargada.\nDimensiones %ix%i",
                                  Dim2Procs[BSel2D].WPIC_OUT,
                                  Dim2Procs[BSel2D].HPIC_OUT );
                }
            }else{
                if( Dim2Procs[BSel2D].ProcType!=NO_PROC_Signal_2D ){
                    if( Dim2Procs[BSel2D].ProcType==RGB2GRAY_2DPROC && Dim2Procs[BSel2D].IsLoaded ){
                        SetStatusText( "%s\nDimensiones %ix%i.",
                                       Dim2Procs[BSel2D].GetInfo(BSel2D),
                                        Dim2Procs[BSel2D].WPIC_OUT,
                                          Dim2Procs[BSel2D].HPIC_OUT
                                      );
                                          /// P_Prom_Zeros
                    }else{
                        SetStatusText( "%s\nDimensiones %ix%i",
                                       Dim2Procs[BSel2D].GetInfo(BSel2D),
                                        Dim2Procs[BSel2D].WPIC_OUT,
                                          Dim2Procs[BSel2D].HPIC_OUT );
                        if( Dim2Procs[BSel2D].ProcType==HIST_2DPROC && Dim2Procs[BSel2D].IsLoaded ){
                            if( Dim2Procs[BSel2D].IsGray ){
                                sprintf( T2TT, "Thresholds, %3i, (!0)%3i, (!255)%3i\ngraythresh = %i (%g)",
                                        (int)round(Dim2Procs[BSel2D].P_PromR),
                                              (int)round(Dim2Procs[BSel2D].P_Prom_Zeros),
                                              (int)round(Dim2Procs[BSel2D].P_Prom_Ones),
                                              (int)round(255*Dim2Procs[BSel2D].GrayValue),
                                              Dim2Procs[BSel2D].GrayValue
                                        );
                            }else{
                                sprintf( T2TT, "Thresholds, R(%3i), G(%3i), B(%3i)",
                                        (int)round(Dim2Procs[BSel2D].P_PromR),
                                              (int)round(Dim2Procs[BSel2D].P_PromG),
                                              (int)round(Dim2Procs[BSel2D].P_PromB)
                                        );

                            }
                            HistPlot.SetTextInfoAdd(T2TT);
                        }else{
                            HistPlot.SetTextInfoAdd(" ");
                        }
                    }
                }else{
                    SetStatusText( "%s", Dim2Procs[BSel2D].GetInfo(BSel2D) );
                }
            }
        }
        ///
        if( Dim2Procs[BSel2D].ProcType==HIST_2DPROC ){
            if( Dim2Procs[BSel2D].IsLoaded ){
                if( Dim2Procs[BSel2D].GetInput()->IsGray ){
                    HistPlot.SetLineColor( (RGBQUAD){180, 100, 100, 0} );
                    HistPlot.PLOT_HIST( Dim2Procs[BSel2D].Hist, 256, 0, TRUE );
                }else{
                    HistPlot.FillGrid( FALSE );
                    HistPlot.Hold( TRUE );
                    HistPlot.SetLineColor( 255, 0, 0 );
                    HistPlot.PLOT_HIST( Dim2Procs[BSel2D].Hist_R, 256, 0, TRUE );
                    ///
                    HistPlot.SetLineColor( 0, 255, 0 );
                    HistPlot.PLOT_HIST( Dim2Procs[BSel2D].Hist_G, 256, 0, TRUE );
                    ///
                    HistPlot.SetLineColor( 0, 0, 255 );
                    HistPlot.PLOT_HIST( Dim2Procs[BSel2D].Hist_B, 256, 0, TRUE );
                    ///
                    HistPlot.FillBuff();
                    HistPlot.Hold( FALSE );
                }
                HistPlot.Redraw();
            }else{
                HistPlot.FillGrid();
                HistPlot.FillBuff();
                HistPlot.SetTextMiddle( "Sin señal, presione ejecutar." );
                HistPlot.Redraw();
            }
        }
        ///
        /// Dim2Procs[BSel2D].SetPicPlot( );
        return TRUE;
    }
    ///
    ///
};
///
///
///
DWORD WINAPI ThreadCall_CMDS( LPVOID pv ){
    TLAB* TLBPTR = (TLAB*)pv;
    TLBPTR->CmdProcs( TLBPTR->MSG_Cmds.hwnd,
                      TLBPTR->MSG_Cmds.message,
                      TLBPTR->MSG_Cmds.wParam,
                      TLBPTR->MSG_Cmds.lParam );
    return 0;
}
///
///
TLAB            TLAB_STR;
///
///
///
///
#include "REDIMP_F.cpp"
///
///
#include "StructThreads.cpp"
///
#include "WProcedure.cpp"
#include "Create.cpp"
#include "CmdProcs.cpp"
#include "Procs1D.cpp"
#include "Procs2D.cpp"
#include "RUN_PROCS.cpp"
#include "SaveRestFile.cpp"
#include "SaveSesion.cpp"
#include "SaveOpenSesion.cpp"


#include "SaveCode1D.cpp"
#include "CopyCode1D.cpp"

#include "SaveCode2D.cpp"
#include "CopyCode2D.cpp"

///
///
#include "DLGS.cpp"
#include "DLG_INIT_APP.cpp"
#include "DLGS_PLAY.cpp"
#include "SHOW_IMAGE.cpp"
#include "SHOW_PLOT.cpp"
///
#include "SAVE_GRAPH_FUNC.cpp"
#include "WRITE_ERROR_REPORT.cpp"
///
#include "CALL_FREE_MAT.cpp"
///
#include "PRUEBAS.cpp"
///
///
///
///
///

///
///
