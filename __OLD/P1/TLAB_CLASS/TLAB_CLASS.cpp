/// #include "E:\AIIF\SegAll\CONOCIMIENTO\UNIVERSIDAD\TESIS\P_NUEVO\APLICACIONES\TLAB\DSP_LAB\DSP_TAB\DSP_TAB_ALL\DSP_TAB_MAIN.cpp"
#if (_WIN32_WINNT < 0x0500)
    #undef _WIN32_WINNT
    #define  _WIN32_WINNT 0x0500
#endif
#include <windows.h>
#include <shlwapi.h>
#include <commctrl.h>
#include <stdio.h>
#include "E:\AIIF\SegAll\CONOCIMIENTO\UNIVERSIDAD\TESIS\P_NUEVO\APLICACIONES\TLAB\DSP_LAB\DSP_TAB\DSP_TAB_ALL\Many_Funcs.h"
#include "E:\AIIF\SegAll\CONOCIMIENTO\UNIVERSIDAD\TESIS\P_NUEVO\APLICACIONES\TLAB\DSP_LAB\DSP_TAB\DSP_TAB_ALL\includes_DSP_TAB.h"
#include "E:\AIIF\SegAll\CONOCIMIENTO\UNIVERSIDAD\TESIS\P_NUEVO\APLICACIONES\TLAB\DSP_LAB\DSP_TAB\DSP_TAB_ALL\FDESIGN\TYPES_FDESIGN.cpp"
#include "E:\AIIF\SegAll\CONOCIMIENTO\UNIVERSIDAD\TESIS\P_NUEVO\APLICACIONES\TLAB\DSP_LAB\DSP_TAB\DSP_TAB_ALL\FDESIGN\FDESIGN.cpp"
/// ///////////////////////////////////////////////////////////////////////////
/// ///////////////////////////////////////////////////////////////////////////
/// ///////////////////////////////////////////////////////////////////////////
/// ///////////////////////////////////////////////////////////////////////////
/// ///////////////////////////////////////////////////////////////////////////
struct TLAB_CLASS{
/// /////////////////////////////////////////////////////////////////////////////////////////////
LRESULT                 WProcedure              ( HWND, UINT, WPARAM, LPARAM );
BOOL                    CREATE                  ( HWND, UINT, WPARAM, LPARAM );
BOOL                    CmdProcs                ( HWND, UINT, WPARAM, LPARAM );
/// DECLARE OWN FUNCS
/// /////////////////////////////////////////////////////////////////////////////////////////////
/// STRUCT VARS
int                                         IPtr;
HWND                                        HW_MAIN_FATHER;
HWND                                        HW_PIC_OUT;
BOOL                                        WIN_ON;
BOOL                                        OnTop;
HMENU                                       HMeW;
HBITMAP                                     HBMP_OUT;
///
ImageButton                                 B_DSP_PROCS[64];
FDESIGN_CLASS                               FDS[64];
/// ///
int                                         IRESERVED;
/// /////////////////////////////////////////////////////////////////////////////////////////////
    TLAB_CLASS(){
        IPtr = 0;
        ZeroMemory( &IPtr, sizeof(TLAB_CLASS) );
    }
    /// /
    ~TLAB_CLASS(){
        FreeAll();
    }
    /// /
    BOOL FreeAll(){
        for( int i=0; i<64; i++ ){
            FDS[i].Free();
            B_DSP_PROCS[i].FreeBitmaps();
        }
        return TRUE;
    }
    /// /
    BOOL DeleteBitmaps(){
        DeleteObject( HBMP_OUT );
        HBMP_OUT = NULL;
        return TRUE;
    }
    /// /
    /// /
    /// C  R  E  A  T  E
    BOOL Create( LPCSTR szClassName ){
        HW_MAIN_FATHER =
            CreateWindowEx( 0, szClassName, szClassName,
                            ((WS_OVERLAPPEDWINDOW - (WS_MAXIMIZEBOX | WS_SIZEBOX)) | WS_CAPTION ) | DS_CENTER,
                               100, 100,
                               1130, 531,
                               HWND_DESKTOP, NULL,GetModuleHandleA(NULL), NULL );
        /// /
        HACCEL hAcelerador2 = LoadAccelerators( GetModuleHandleA(NULL) , MAKEINTRESOURCE(TLAB_aceleradores) );
        MSG messages = {0};
        ShowWindow( HW_MAIN_FATHER, 1 );
        HMeW = GetMenu( HW_MAIN_FATHER );
        WIN_ON = TRUE;
        while( GetMessage (&messages, NULL, 0, 0) ){
            if( !IsDialogMessage(HW_MAIN_FATHER, &messages) && WIN_ON ){
                TranslateMessage(&messages);
                DispatchMessage(&messages);
            }else{
                if( TranslateAccelerator(HW_MAIN_FATHER, hAcelerador2, &messages ) ){ continue; }
            }
        }
        return (HW_MAIN_FATHER!=NULL);
    }
    /// /
    /// /
    /// /
    BOOL SetImageOutput( int i1, int i2 ){
        HBITMAP HBMP_T = LoadImageR( i1, MAKEINTRESOURCEA(i2) );
        if( HBMP_T!=NULL ){
            DeleteObject( HBMP_OUT );
            HBMP_OUT = HBMP_T;
            SendMessage( HW_PIC_OUT, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)HBMP_OUT );
        }
        return TRUE;
    }
    /// /
    /// /
};
#include "WProcedure.cpp"
#include "CREATE.cpp"
#include "CmdProcs.cpp"
TLAB_CLASS TLC;
