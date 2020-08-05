/// PICS AND RESOURCES.
#define TLAB_ICON                   1
#define TLAB_ICON2                  2
#define RUN_ICON                    3
#define RUN_PNG                     4
#define INIT_PNG_BIG                5
#define INIT_PNG                    6
#define LENA_JPG                    7
#define WAV_LOAD                    8
#define MAT_LOAD                    9
#define SIN_MAKE                   10
#define CANCEL_B                   11
#define NO_PIC                     12
#define WIGNER_M                   13
#define CHOI_WILLIAM_M             14
#define KIRSH_M                    15
///
//
//

/// #define NO_PROC                   510
// 1D
#define SIG_IN                    511
#define PLOT_PNG                  512
#define FILTER_PIC                513
#define WINDOWING_PIC             514
#define FFT_PIC                   515
#define SPEC_PIC                  516
#define WIGNER_PIC                517
#define CHOI_WILL_PIC             518
#define PLAY_PIC                  519
///
#define NO_PROC_PIC               520
//
//
// 2D
#define IMAGEFILE                1023
#define IMSHOW                   1024
#define RGB2GRAY_PIC             1025
#define BINAR_PIC                1026
#define CONV_PIC                 1027
#define HIST_PIC                 1028
#define REDIM_PIC                1029
#define ARIT_PIC                 1030
#define BWL_PIC                  1031
#define NOPROC2D_PIC             1032
#define ERODE_PIC                1033
#define DILATE_PIC               1034
#define OPEN_PIC                 1035
#define CLOSE_PIC                1036
#define IMFILL_PIC               1037
///
#define DSP_FREEMAT              1099
#define DSP_FREEMAT_ICON         1100
#define DSP_TADICON_ICON         1101
#define DSP_TLABFOLDER_ICON      1102
#define DSP_MSVCR100             1103
#define FRONTIER_PIC             1104
#define FRONTIERDIL_PIC          1105
#define TH_W_PIC                 1106
#define TH_B_PIC                 1107
//
//
//
#define FILE_RES                    1
#define DSP_TAD_EXE                 2
#define DSP_MANIFIEST               3
///
///
///
/// WINDOW, DLG, MENU.
#define MAIN_MENU                   1
#define MAIN_ACCEL                  2

///
///
///
///
///
///
/// PROCS IDYES
#define CMD_CORRI                   20
#define WITH_OUT_PROC               (CMD_CORRI+1)
#define CMD_QUIT                    (CMD_CORRI+2)
#define CMD_OPEN_DSP_TAD            (CMD_CORRI+3)
#define CMD_ON_ALW_ONTOP            (CMD_CORRI+4)
#define CMD_RUN                     (CMD_CORRI+5)
#define CMD_OPEN_FOLDER_DSP         (CMD_CORRI+6)
#define CMD_DeleteProc              (CMD_CORRI+7)
#define CMD_OPEN_SESION             (CMD_CORRI+8)
#define CMD_SAVE_SESION             (CMD_CORRI+9)
#define CMD_PIC_2D                  (CMD_CORRI+10)
#define CMD_SAVE_CODE               (CMD_CORRI+11)
#define CMD_SETFOCUS_1_2_D          (CMD_CORRI+13)
#define CMD_RE_RUN                  (CMD_CORRI+14)
//
#define CMD_UPCTRL                  (CMD_CORRI+15)
#define CMD_DOWNCTRL                (CMD_CORRI+16)
//
#define CMD_PRUEBAS                 (CMD_CORRI+17)
//
#define CMD_PLOT_1D                 (CMD_CORRI+18)
#define CMD_SAVE_GRAPH              (CMD_CORRI+19)
#define CMD_TIME_LAST_PROC          (CMD_CORRI+20)
#define CMD_COPY_PROC_2_CLIPBOARD   (CMD_CORRI+21)
#define CMD_SHOW_HIDE_NEW_WIN       (CMD_CORRI+22)
#define CMD_GET_MOD_INFO            (CMD_CORRI+23)
#define CMD_CALL_FREE_MAT           (CMD_CORRI+24)
#define CMD_ABOUT_TLAB              (CMD_CORRI+25)
#define CMD_SETPLOTALL              (CMD_CORRI+26)



#define CMD_1_2_Ds                  512



///
///
///
///0DIALOGS000000000000000000000000000
#define DLG_LOAD_VAR_1D             19
#define DLG_1D_INIT                 20
#define Dial_Change_Proc_1D_TAD     21
#define Dial_Fdesign_GUI            22
#define Dial_Fdesign_WIN_GUI        23
#define Dial_Fspectogram_WIN_GUI    24
#define Dial_Change_Proc_2D         25
#define Dial_Select_Levels          26
#define Dial_Kernel_Levels          27
#define Dial_Arithmetic             28
#define Dial_Redim                  29
#define Dial_PlayDesign_GUI         30
#define Dial_SHOWPIC_GUI            31
#define Dial_SHOWPLOT_GUI           32
#define Dial_ERODE_IMAGE            33
#define Dial_MSGBOX                 34
#define Dial_LOAD_INIT              35
#define Dial_ABOUT_TLAB             36
#define Dial_ImFill                 37
///
///
///
///
///
/// STRINGTABLE:
#define ID_TEXT_ABOUT              129
///
///
///
///
/// DEFINE FUNCTIONS.
LRESULT CALLBACK WindowProcedure( HWND, UINT, WPARAM, LPARAM );
DWORD WINAPI ThreadCall_DSP_TAD( LPVOID pv );
DWORD WINAPI ThreadCall_RUN( LPVOID pv );
DWORD WINAPI ThreadCall_Procs1D( LPVOID pv );
DWORD WINAPI ThreadCall_Progress( LPVOID pv );
DWORD WINAPI ThreadCall_Progress_INIT( LPVOID pv );
DWORD WINAPI ThreadCall_INIT_PROGRESS( LPVOID pv );
DWORD WINAPI ThreadCall_Load_All( LPVOID pv );
DWORD WINAPI ThreadCall_SaveSesionProc( LPVOID pv );
DWORD WINAPI ThreadCall_OpenSesionProc( LPVOID pv );
DWORD WINAPI ThreadCall_CMDS( LPVOID pv );
DWORD WINAPI ThreadCall_SHPIC( LPVOID pv );
DWORD WINAPI ThreadCall_SPLOT( LPVOID pv );
DWORD WINAPI ThreadCall_FreeMat( LPVOID pv );
///
///
BOOL FDESIGN( HWND HW_Fath, VOID* FdsnC );
BOOL FWINDOWING( HWND HW_Fath, VOID* FdsnC );
BOOL FSPECTOGRAM( HWND HW_Fath, VOID* FdsnC );
BOOL PLAY_DESIGN( HWND HW_Fath, VOID* FdsnC );
BOOL PLAY_DESIGN( HWND HW_Fath, VOID* FdsnC );
///
///
HBITMAP REDIMP_F( HBITMAP HBMI, int W_OUT, int H_OUT );
///
///
BOOL Dlg1D_INIT( HWND hwnd, UINT  message, WPARAM wParam, LPARAM lParam );
BOOL DlgProgress12D( HWND hwnd, UINT  message, WPARAM wParam, LPARAM lParam );
BOOL DlgProgress12D_INIT( HWND hwnd, UINT  message, WPARAM wParam, LPARAM lParam );
BOOL DlgAbout_TLAB( HWND hwnd, UINT  message, WPARAM wParam, LPARAM lParam );
BOOL INIT_PROGRESS( );
BOOL CALLBACK DlgProc_Proc_1D_TAD(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK DlgProc_Proc_2D(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK SELECT_LEVELS(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK SELECT_KERNELS(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK SELECT_ARITH(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK SELECT_REDIM(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK SELECT_ERODE(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK SELECT_DILATE(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK SELECT_OPEN(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK SELECT_CLOSE(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK PLAYDESIGN_Dlg(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK DlgShowPicBig( HWND hwnd, UINT  message, WPARAM wParam, LPARAM lParam );
BOOL CALLBACK DlgShowPlotBig( HWND hwnd, UINT  message, WPARAM wParam, LPARAM lParam );
BOOL CALLBACK DLGMSGBOX( HWND hwnd, UINT  message, WPARAM wParam, LPARAM lParam );

BOOL CALLBACK SELECT_IMFILL(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam);

