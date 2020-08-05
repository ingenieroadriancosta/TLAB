#include <windows.h>
#include <math.h>
#include <stdio.h>
#include <direct.h>
#include <sys/stat.h>
#include <time.h>
#include "fftw3.h"
#include "matio.h"
const int MaxFIR_L = 999;
///
///
HWND HWPGRGEN_MAIN = NULL;
HWND HWPGRGEN = NULL;
BOOL SetProgress(){
    DWORD IdTh = 0;
    CreateThread( NULL, 0, ThreadCall_Progress_INIT, NULL, 0, &IdTh );
    return TRUE;
}
///
///
///
///
///
struct fftprepare{
    int                 NPosP;
    double*             InputV;
    double*             RealV;
    double*             ImagV;
    double              MaxVal;
    fftprepare(){
        NPosP = 0;
        ZeroMemory( &NPosP, sizeof(fftprepare) );
        MaxVal = -1e100;
        FreeAll();
    }
    BOOL FreeAll(){
        if( RealV!=NULL ){
            free( RealV );
            RealV = NULL;
        }
        if( ImagV!=NULL ){
            free( ImagV );
            ImagV = NULL;
        }
        if( InputV!=NULL ){
            free( InputV );
            InputV = NULL;
        }
        NPosP = 0;
        return TRUE;
    }
    BOOL Prepare( int NPos ){
        if( NPos<1 ){
            return FALSE;
        }
        if( NPosP==NPos ){
            if( RealV ){
                ZeroMemory( RealV, sizeof(double) * NPos + 1024 );
            }
            if( ImagV ){
                ZeroMemory( ImagV, sizeof(double) * NPos + 1024 );
            }
            if( InputV ){
                ZeroMemory( InputV, sizeof(double) * NPos + 1024 );
            }
            return TRUE;
        }
        FreeAll();
        int NPOSMIN = NPos;
        if( NPOSMIN<2048 ){
            NPOSMIN = 2048;
        }
        int NPOSPLUS = 2048;
        if( NPOSMIN<192000 ){
            NPOSPLUS = 192000;
        }
        ///
        int SizeofOut = sizeof(double) * (NPOSMIN + NPOSPLUS);
        ///
        RealV = (double*)malloc( SizeofOut );
        ImagV = (double*)malloc( SizeofOut );
        InputV = (double*)malloc( SizeofOut );
        if( RealV ){
            ZeroMemory( RealV, SizeofOut );
        }
        if( ImagV ){
            ZeroMemory( ImagV, SizeofOut );
        }
        if( InputV ){
            ZeroMemory( InputV, SizeofOut );
        }
        NPosP = NPos;
        return ( RealV!=NULL && ImagV!=NULL && InputV!=NULL );
    }
    ///
    ///
    BYTE nextpow2( int IVal ){
        if( IVal<1 ){
            return 0;
        }
        int IVal2Proc = IVal;
        BYTE B2R = 0;
        while( IVal2Proc>1 ){
            IVal2Proc = IVal2Proc/2;
            B2R = B2R + 1;
        }
        if( (1<<B2R)==IVal ){
            return B2R;
        }
        B2R = B2R + 1;
        return B2R;
    }
    ///
    ///
};
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
struct VarsInfo{
    int                         IPtr;
    int                         LenOfV;
    double                      FirstValue;
    char                        VName[128];
    BOOL                        IsArrVect;
    int                         NVARS;
    VarsInfo(){
        IPtr        = 0;
        ZeroMemory( &IPtr, sizeof(VarsInfo) );
    }
    BOOL CpyStr( LPCSTR StrIn ){
        strcpy( VName, StrIn );
        return TRUE;
    }

};
///
///
///

typedef LPVOID (__stdcall * DLLMATIO)( ... );
struct DSP_LOAD{
    private:
    int                             IPtr;
    char                            PATH[1024];
    HMODULE                         hMod_libmatio;
    HMODULE                         hMod_fft;
    public:
    ///
    DLLMATIO                        Mat_Open;
    DLLMATIO                        Mat_VarWrite;
    DLLMATIO                        Mat_VarCreate;
    DLLMATIO                        Mat_VarFree;
    DLLMATIO                        Mat_VarReadNextInfo;
    DLLMATIO                        Mat_VarReadInfo;
    DLLMATIO                        Mat_VarRead;
    DLLMATIO                        Mat_Close;
    ///
    DLLMATIO                        UnZip;
    DLLMATIO                        UnZipWin;
    ///
    DLLMATIO                        fftw_malloc;
    DLLMATIO                        fftw_plan_dft_1d;
    DLLMATIO                        fftw_plan_dft_2d;
    ///
    DLLMATIO                        fftw_plan_dft_r2c_2d;
    /// fftw_plan fftw_plan_dft_r2c_2d(int n0, int n1, double *in, fftw_complex *out, unsigned flags);
    ///
    DLLMATIO                        fftw_plan_dft;
    DLLMATIO                        fftw_execute;
    DLLMATIO                        fftw_free;
    DLLMATIO                        fftw_destroy_plan;
    private:
    DLLMATIO                        fftw_cleanup;
    DLLMATIO                        fftw_init_threads;
    DLLMATIO                        fftw_plan_with_nthreads;
    public:
    ///
    DLLMATIO                        FFT_Dll;
    DLLMATIO                        IFFT_Dll;
    ///
    ///
    double                          TIME_SERIES_CTIC[1024];//VARIABLES GLOBALES DE TIEMPO.
    double                          TIME_SERIES_CTOC[1024];//VARIABLES GLOBALES DE TIEMPO.
    public:
    double                          EndToc[1024];
    ///
    DSP_LOAD(){
        if( FindWindow( NULL, " Iniciando TLAB " )!=NULL || FindWindow( NULL, " Iniciando TLAB " )!=NULL ){
            /// SendMessageA( FindWindow( NULL, " TLAB " ), WM_COMMAND, CMD_SHOW_HIDE_NEW_WIN, 0 );
            exit( 0 );
        }
        if( FindWindow( NULL, " TLAB CON " )!=NULL || FindWindow( NULL, " TLAB " )!=NULL ){
            SendMessageA( FindWindow( NULL, " TLAB " ), WM_COMMAND, CMD_SHOW_HIDE_NEW_WIN, 0 );
            exit( 0 );
        }
        AllocConsole();
        ShowWindow( GetConsoleWindow(), 0 );
        SetProgress();
        ///
        IsOnFreeMat = FALSE;
        ///
        char UName[1024] = {0};
        DWORD PDW = 0;
        GetUserNameA( UName, &PDW );
        sprintf( PATH, "%s\\TLAB\\", getenv( "USERPROFILE" ) );
        /// ShowError( PATH, 0 );
        mkdir( PATH );
        SetCurrentDirectoryA( PATH );
        sprintf( UName, "CD \"%s\"", PATH );
        system( UName );
        Write_Resource( "msvcr100.dll", DSP_MSVCR100, FILE_RES );
        if( !Write_Resource( "DSP_TAD.exe", DSP_TAD_EXE, FILE_RES ) ){
            /*
            ShowWindow( GetConsoleWindow(), 3 );
            MessageBoxA( GetConsoleWindow(),
                         "No se pueden escribir los archivos,\n"
                         "ejecute el software como administrador."   , " ", 16 );
            exit( -1 );
            //*/
        }
        SendMessage( HWPGRGEN, PBM_SETPOS, 10, 10 );
        sprintf( UName, "CALL \"%s\\DSP_TAD.exe\" -1", PATH  );
        int IRes = system( UName );
        if( IRes!=-20 ){
            ShowWindow( GetConsoleWindow(), 3 );
            MessageBoxA( GetConsoleWindow(),
                         "No se pueden escribir los archivos,\n"
                         "ejecute el software como administrador."   , " ", 16 );
            exit( -1 );
        }
        SetCurrentDirectoryA( PATH );
        sprintf( UName, "%slibmatio.dll", PATH );
        hMod_libmatio = LoadLibrary( UName );
        //*
        Mat_Open                = LoadAnyFunc( hMod_libmatio, "Mat_Open" );
        Mat_VarWrite            = LoadAnyFunc( hMod_libmatio, "Mat_VarWrite" );
        Mat_VarCreate           = LoadAnyFunc( hMod_libmatio, "Mat_VarCreate" );
        Mat_VarFree             = LoadAnyFunc( hMod_libmatio, "Mat_VarFree" );
        Mat_VarReadNextInfo     = LoadAnyFunc( hMod_libmatio, "Mat_VarReadNextInfo" );
        Mat_VarReadInfo         = LoadAnyFunc( hMod_libmatio, "Mat_VarReadInfo" );
        Mat_VarRead             = LoadAnyFunc( hMod_libmatio, "Mat_VarRead" );
        Mat_Close               = LoadAnyFunc( hMod_libmatio, "Mat_Close" );
        ///
        ///
        ///
        SetCurrentDirectory( PATH );
        sprintf( UName, "%stest.dll" , PATH );
        HMODULE hMod = LoadLibrary( UName );
        UnZip       = LoadAnyFunc( hMod, "UnZip" );
        UnZipWin    = LoadAnyFunc( hMod, "UnZipWin" );
        ///
        ///
        ///
        ///
        ///
        ///
        sprintf( UName, "%s%s" , PATH, "FFT.dll" );
        /// ShowError( TempChar, 0 );
        hMod_fft = LoadLibrary( UName );
        ParseErrNull( (VOID*)hMod_fft, -2, "LoadLibrary load" );
        FFT_Dll       = LoadAnyFunc( hMod_fft, "FFT" );
        IFFT_Dll       = LoadAnyFunc( hMod_fft, "IFFT" );
        ///
        ///
        ///
        ///
        ///
        ///
        ///
        sprintf( UName, "%s%s" , PATH, "libfftw3-3.dll" );
        hMod_fft = LoadLibrary( UName );
        ParseErrNull( (VOID*)hMod_fft, -2, "LoadLibrary load" );
        fftw_malloc                 = LoadAnyFunc( hMod_fft, "fftw_malloc" );
        fftw_plan_dft_1d            = LoadAnyFunc( hMod_fft, "fftw_plan_dft_1d" );
        fftw_plan_dft_2d            = LoadAnyFunc( hMod_fft, "fftw_plan_dft_2d" );
        fftw_plan_dft_r2c_2d        = LoadAnyFunc( hMod_fft, "fftw_plan_dft_r2c_2d" );
        fftw_plan_dft               = LoadAnyFunc( hMod_fft, "fftw_plan_dft" );
        fftw_execute                = LoadAnyFunc( hMod_fft, "fftw_execute" );
        fftw_free                   = LoadAnyFunc( hMod_fft, "fftw_free" );
        fftw_destroy_plan           = LoadAnyFunc( hMod_fft, "fftw_destroy_plan" );
        fftw_cleanup                = LoadAnyFunc( hMod_fft, "fftw_cleanup" );
        fftw_init_threads           = LoadAnyFunc( hMod_fft, "fftw_init_threads" );
        fftw_plan_with_nthreads     = LoadAnyFunc( hMod_fft, "fftw_plan_with_nthreads" );
        ///
        fftw_init_threads();
        fftw_plan_with_nthreads( 2 );
        //*/
    }
    ///
    ///
    ///
    LPCSTR GetDSPPath( void ){
        return PATH;
    }
    ///
    ///

    //MTIME///////////////////////// T I M E___S E R I E S /////////////////////////
                                           //
    double tic( int whotic )   //TIC -TIC-TIC -TIC-TIC -TIC-TIC -TIC-TIC -TIC-TIC -TIC-TIC " TIC "
    {//INICIA LA VARIABLE DE TEIMPO GLOBAL adrianti.
         TIME_SERIES_CTIC[1023&whotic]=(double)clock();
         return(TIME_SERIES_CTIC[1023&whotic]);
    }//END TIC.
                                           //
    double toc( int whotoc )  //TOC -TOC-TOC -TOC-TOC -TOC-TOC -TOC-TOC -TOC-TOC -TOC-TOC " TOC "
     {//RETORNA EL TIEMPO TRANSCURRIDO DESDE QUE SE LLAMÓ LA FUNCION TOC.
          TIME_SERIES_CTOC[1023&whotoc]=((double)clock()-TIME_SERIES_CTIC[1023&whotoc])/1000.0;
          EndToc[1023&whotoc] = TIME_SERIES_CTOC[1023&whotoc];
          return(TIME_SERIES_CTOC[1023&whotoc]);
     }//END TOC.
    ///
    double GetEndToc( int whotoc ){
        return EndToc[1023&whotoc];
    }
    ///
    ///
    BOOL SetWintocText( LPCSTR LP2S, int whotoc ){
        double ENT = toc(whotoc);
        char* T2TT = (char*)malloc( 1024 );
        ZeroMemory( T2TT, 1023 );
        sprintf( T2TT, "%s    %g Segundos.", LP2S, ENT );
        SetWindowTextA( *HWN_MAIN, T2TT );
        free( T2TT );
        return TRUE;
    }
    ///
    ///
    ///
    ///
    BOOL IsOnFreeMat;
    PROCESS_INFORMATION pi_infor;
    ///
    BOOL QuitOfFreeMat(){
        if( IsOnFreeMat ){
            PostThreadMessage(pi_infor.dwThreadId, WM_CLOSE, 0, 0);
            PostThreadMessage(pi_infor.dwThreadId, WM_QUIT, 0, 0);
        }
        return TRUE;
    }
    ///
    BOOL ExecuteFreeMat( LPVOID pv, HWND* HWPRGPTR ){
        ///
        char UName[1024] = {0};
        ///
        ///
        ///
        ///
        ///
        //*
        if( FindWindowA(NULL, "FreeMat v4.0 Command Window")!=NULL ){
            ShowWindow( FindWindowA(NULL, "FreeMat v4.0 Command Window"), SW_MINIMIZE );
            Sleep( 100 );
            ShowWindow( FindWindowA(NULL, "FreeMat v4.0 Command Window"), SW_RESTORE );
            // FreeMat v4.0 Command Window
            return FALSE;
        }
        sprintf( UName, "%sFreeMat" , PATH );
        mkdir( UName );
        SetCurrentDirectoryA( UName );
        DeleteFileA( "FreeMat.zip" );
        sprintf( UName, "%sFreeMat\\bin\\FreeMat.exe" , PATH );
        sprintf( UName, "%sFreeMat\\FreeMat.exe" , PATH );
        /// MessageBox( *HWN_MAIN, UName, "", 0 );
        STARTUPINFO si = {0};
        si.cb = sizeof si;
        ZeroMemory( &pi_infor, sizeof(PROCESS_INFORMATION) );
        const TCHAR* target = UName;
        BOOL Bres = CreateProcess( target, 0, 0, FALSE, 0, 0, 0, 0, &si, &pi_infor );
        Sleep( 100 );
        if( !Bres ){
            DWORD IdTh = 0;
            CreateThread( NULL, 0, ThreadCall_Progress, (PVOID)pv, 0, &IdTh );
            Sleep(1);
            while( HWPRGPTR[0]==NULL ){
                Sleep(1);
            }
            ///
            SetWindowTextA( HWPRGPTR[0], "Copiando archivos..." );
            HWND HW_Progr = GetDlgItem( HWPRGPTR[0], 100 );
            ///
            Write_Resource( "FreeMat.zip", DSP_FREEMAT, FILE_RES );
            sprintf( UName, "%sFreeMat.zip" , PATH );
            UnZipWin( UName, PATH, HW_Progr );
            ///
            sprintf( UName, "%sFreeMat.zip" , PATH );
            DeleteFileA( UName );
            ///
            SendMessage( HWPRGPTR[0], WM_CLOSE, 0, 0 );
            sprintf( UName, "%sFreeMat" , PATH );
            mkdir( UName );
            SetCurrentDirectoryA( UName );
            sprintf( UName, "%sFreeMat\\bin\\FreeMat.exe" , PATH );
            sprintf( UName, "%sFreeMat\\FreeMat.exe" , PATH );
            ZeroMemory( &si, sizeof(STARTUPINFO) );
            si.cb = sizeof si;
            ZeroMemory( &pi_infor, sizeof(PROCESS_INFORMATION) );
            CreateProcess( target, 0, 0, FALSE, 0, 0, 0, 0, &si, &pi_infor );
        }
        //Sleep( 5000 );
        IsOnFreeMat = TRUE;
        WaitForSingleObject(pi_infor.hProcess, INFINITE );
        IsOnFreeMat = FALSE;
        //PostThreadMessage(pi_infor.dwThreadId, WM_CLOSE, 0, 0);
        //PostThreadMessage(pi_infor.dwThreadId, WM_QUIT, 0, 0);
        //exit( 0 );
        return TRUE;
        /// TerminateProcess();
        //*/
        ///
        ///
        ///
        ///
        ///
        ///
        ///
        ///
        if( FindWindowA(NULL, "FreeMat v4.0 Command Window")!=NULL ){
            ShowWindow( FindWindowA(NULL, "FreeMat v4.0 Command Window"), SW_MINIMIZE );
            Sleep( 100 );
            ShowWindow( FindWindowA(NULL, "FreeMat v4.0 Command Window"), SW_RESTORE );
            // FreeMat v4.0 Command Window
            return FALSE;
        }
        IsOnFreeMat = TRUE;
        sprintf( UName, "%sFreeMat" , PATH );
        mkdir( UName );
        SetCurrentDirectoryA( UName );
        sprintf( UName, "CALL \"%sFreeMat\\FreeMat.exe\"" , PATH );
        if( system(UName)!=0 ){
            DWORD IdTh = 0;
            CreateThread( NULL, 0, ThreadCall_Progress, (PVOID)pv, 0, &IdTh );
            Sleep(1);
            while( HWPRGPTR[0]==NULL ){
                Sleep(1);
            }
            ///
            SetWindowTextA( HWPRGPTR[0], "Copiando archivos..." );
            HWND HW_Progr = GetDlgItem( HWPRGPTR[0], 100 );
            ///
            Write_Resource( "FreeMat.zip", DSP_FREEMAT, FILE_RES );
            sprintf( UName, "%sFreeMat.zip" , PATH );
            UnZipWin( UName, PATH, HW_Progr );
            SendMessage( HWPRGPTR[0], WM_CLOSE, 0, 0 );
            sprintf( UName, "%sFreeMat" , PATH );
            mkdir( UName );
            SetCurrentDirectoryA( UName );
            sprintf( UName, "CALL \"%sFreeMat\\FreeMat.exe\"" , PATH );
            system(UName);
        }
        IsOnFreeMat = FALSE;
        return TRUE;
    }
    ///
    ///
    ///
    ///
    ///
    BYTE nextpow2( int IVal ){
        if( IVal<1 ){
            return 0;
        }
        int IVal2Proc = IVal;
        BYTE B2R = 0;
        while( IVal2Proc>1 ){
            IVal2Proc = IVal2Proc/2;
            B2R = B2R + 1;
        }
        if( (1<<B2R)==IVal ){
            return B2R;
        }
        B2R = B2R + 1;
        return B2R;
    }
    ///
    ///
    ///
    ///
    DLLMATIO LoadAnyFunc( HMODULE dllHnd, LPCSTR FuncName ){
        static int NCnt = 0;
        FARPROC FPrc = GetProcAddress( dllHnd, FuncName );
        ParseErrNull( (VOID*)FPrc, NCnt, FuncName );
        return DLLMATIO(FPrc);
    }
    ///
    ///
    /// /////////////////////////////////////////////////////////////////////////////////////////////
    BOOL ParseErrNull( VOID* VVAR, int Err, LPCSTR TInfor ){
        return ParseErrNull( VVAR, Err, TInfor, TRUE );
    }
    BOOL ParseErrNull( VOID* VVAR, int Err, LPCSTR TInfor, BOOL SetExit ){
        if( VVAR!=NULL ){
            return FALSE;
        }
        char T2T[1024] = {0};
        sprintf( T2T, "%i\n%s\n\n", Err, TInfor );
        MessageBoxA( GetConsoleWindow(), T2T, "", 16 );
        if( SetExit ){
            exit(Err);
        }
        return TRUE;
    }
    ///
    ///
    ///
    ///
    ///
    ///
    BOOL Write_Resource( LPCSTR FName, LPCSTR PATH_I, int N_0, int N_1 ){
        char TempChar[1024] = {0};
        if( PATH_I!=NULL ){
            sprintf( TempChar, "%s%s", PATH_I, FName );
        }else{
            sprintf( TempChar, "%s", FName );
        }
        BYTE* BuffB = NULL;
        DWORD dwResourceSize = 0;
        HGLOBAL hLoadedResource = NULL;
        HRSRC hResource = NULL;
        hResource = FindResourceA( GetModuleHandle(NULL), MAKEINTRESOURCE(N_0), MAKEINTRESOURCE(N_1) );
        DeleteFileA( TempChar );
        ///ShowError( TempChar, -1 );
        if( hResource ){
            hLoadedResource = LoadResource( GetModuleHandle(NULL), hResource );
            if( !hLoadedResource ){
                FreeResource( hLoadedResource );
                return FALSE;
            }
            //LPVOID pLockedResource = LockResource( hLoadedResource );
            BuffB = (BYTE*)LockResource( hLoadedResource );
            if( !BuffB ){
                FreeResource( hLoadedResource );
                return FALSE;
            }
            dwResourceSize = SizeofResource( GetModuleHandle(NULL), hResource );
            if( dwResourceSize==0 ){
                FreeResource( hLoadedResource );
                return FALSE;
            }
            // msgbox();
            HANDLE HFile = CreateFile( TempChar, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
            DWORD LastError = GetLastError();
            if( HFile==NULL ){
                FreeResource( hLoadedResource );
                return FALSE;
            }
            if( LastError==ERROR_SHARING_VIOLATION ){
                CloseHandle( HFile );
                FreeResource( hLoadedResource );
                return FALSE;
            }
            DWORD written = 0;
            WriteFile( HFile, &BuffB[0], dwResourceSize, &written, NULL );
            if( dwResourceSize!=written ){
                CloseHandle( HFile );
                FreeResource( hLoadedResource );
                return FALSE;
            }
            CloseHandle( HFile );
            FreeResource( hLoadedResource );
        }
        return TRUE;
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
    BOOL Write_Resource( LPCSTR FName, int N_0, int N_1 ){
        char TempChar[1024] = {0};
        sprintf( TempChar, "%s%s", PATH, FName );
        BYTE* BuffB = NULL;
        DWORD dwResourceSize = 0;
        HGLOBAL hLoadedResource = NULL;
        HRSRC hResource = NULL;
        hResource = FindResourceA( GetModuleHandle(NULL), MAKEINTRESOURCE(N_0), MAKEINTRESOURCE(N_1) );
        DeleteFileA( TempChar );
        if( hResource ){
            hLoadedResource = LoadResource( GetModuleHandle(NULL), hResource );
            if( !hLoadedResource ){
                FreeResource( hLoadedResource );
                return FALSE;
            }
            //LPVOID pLockedResource = LockResource( hLoadedResource );
            BuffB = (BYTE*)LockResource( hLoadedResource );
            if( !BuffB ){
                FreeResource( hLoadedResource );
                return FALSE;
            }
            dwResourceSize = SizeofResource( GetModuleHandle(NULL), hResource );
            if( dwResourceSize==0 ){
                FreeResource( hLoadedResource );
                return FALSE;
            }
            // msgbox();
            HANDLE HFile = CreateFile( TempChar, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
            DWORD LastError = GetLastError();
            if( HFile==NULL ){
                FreeResource( hLoadedResource );
                return FALSE;
            }
            if( LastError==ERROR_SHARING_VIOLATION ){
                CloseHandle( HFile );
                FreeResource( hLoadedResource );
                return FALSE;
            }
            DWORD written = 0;
            WriteFile( HFile, &BuffB[0], dwResourceSize, &written, NULL );
            if( dwResourceSize!=written ){
                CloseHandle( HFile );
                FreeResource( hLoadedResource );
                return FALSE;
            }
            CloseHandle( HFile );
            FreeResource( hLoadedResource );
        }
        return TRUE;
    }
    ///
    ///
    BOOL OpenDSPFolder(){
        ShellExecuteA( NULL, "open", PATH, NULL, NULL, 3 );
        return TRUE;
    }
    ///
    BOOL OpenDSPTAD(){
        Write_Resource( "DSP_TAD.exe", DSP_TAD_EXE, FILE_RES );
        char UName[1024] = {0};
        sprintf( UName, "%s\\DSP_TAD.exe", PATH  );
        ShellExecuteA( NULL, "open", UName, NULL, NULL, 1 );
        return TRUE;
    }
    ///
    ///
    ///
    ///
    BOOL LoadVar( LPCSTR FName, LPCSTR VName, double* DBuff ){
        mat_t*      matfp = NULL;
        matvar_t*   matvar = NULL;
        BOOL B2Ret = TRUE;
        matfp = (mat_t*)Mat_Open( FName, 1 );
        if( matfp==NULL ){
            return FALSE;
        }
        matvar = (matvar_t*)Mat_VarRead( matfp, VName );
        int NPos = (int)( fmax( matvar->dims[0], matvar->dims[1] ) );
        switch( matvar->data_type ){ ///                           MAT_T_INT32
            case MAT_T_DOUBLE:{
                double* DPTR = (double*)matvar->data;
                for( int i=0; i<NPos; i++ ){
                    DBuff[i] = DPTR[i];
                }
                }
                break;
            case MAT_T_INT8:{
                char* C_B = (char*)matvar->data;
                for( int i=0; i<NPos; i++ ){
                    DBuff[i] = ((double)C_B[i]);
                }
                }
                break;
            case MAT_T_UINT8:{
                unsigned char* UCB = (unsigned char*)matvar->data;
                for( int i=0; i<NPos; i++ ){
                    DBuff[i] = ((double)UCB[i]);
                }
                }
                break;
            case MAT_T_UINT16:{
                unsigned short* USB = (unsigned short*)matvar->data;
                for( int i=0; i<NPos; i++ ){
                    DBuff[i] = ((double)(USB[i]));
                }
                }
                break;
            case MAT_T_INT16:{
                short* USB = (short*)matvar->data;
                for( int i=0; i<NPos; i++ ){
                    DBuff[i] = ( (double)USB[i] );
                }
                }
                ///
                /// MessageBoxA( GetActiveWindow(), "", "", 0 );
                ///
                break;
            case MAT_T_UINT32:{
                unsigned int* UIB = (unsigned int*)matvar->data;
                for( int i=0; i<NPos; i++ ){
                    DBuff[i] = (double)UIB[i];
                }
                }
                break;
            case MAT_T_INT32:{
                int* I_B = (int*)matvar->data;
                for( int i=0; i<NPos; i++ ){
                    DBuff[i] = (double)I_B[i];
                }
                }
                break;
            case MAT_T_UINT64:{
                unsigned long long* ULLB = (unsigned long long*)matvar->data;
                for( int i=0; i<NPos; i++ ){
                    DBuff[i] = (double)ULLB[i];
                }
                }
                break;
            case MAT_T_INT64:{
                long long* LLB = (long long*)matvar->data;
                for( int i=0; i<NPos; i++ ){
                    DBuff[i] = (double)LLB[i];
                }
                }
                break;
            default:
                B2Ret = FALSE;
                break;
        }
        Mat_VarFree(matvar);
        Mat_Close( matfp );
        return B2Ret;
    }
    ///
    ///
    ///
    BOOL GetVarsInfo1D( LPCSTR FName, VarsInfo* VINFOUT, int* NVarsMax, HWND hwnd ){
        int NVarsCP = 0;
        mat_t    *matfp = NULL;
        matvar_t *matvar = NULL;
        matfp = (mat_t*)Mat_Open( FName, 1 );
        if( matfp==NULL ){
            return FALSE;
        }
        int LOnBytes = 0;
        while( (matvar = (matvar_t*)Mat_VarReadNextInfo(matfp)) != NULL ){
            if( matvar->dims!=NULL && strlen(matvar->name)>0 ){
                if( matvar->isComplex ){
                    continue;
                }
                //sprintf( VINFOUT[NVarsCP].VName, "matvar->data_size %i", matvar_T->data_size );
                //MessageBoxA( hwnd, VINFOUT[NVarsCP].VName, " ", 0 );
                if( !(matvar->dims[0]!=1 && matvar->dims[1]!=1) ){
                    LOnBytes = max( matvar->dims[0], matvar->dims[1] );
                    /*
                    switch( matvar->class_type ){
                        case MAT_C_DOUBLE:
                            LOnBytes = 8*max( matvar->dims[0], matvar->dims[1] );
                            break;
                        case MAT_C_INT8:
                        case MAT_C_CHAR:
                        case MAT_C_UINT8:
                            LOnBytes = max( matvar->dims[0], matvar->dims[1] );
                            break;
                        case MAT_C_INT16:
                        case MAT_C_UINT16:
                            LOnBytes = 2*max( matvar->dims[0], matvar->dims[1] );
                            break;
                        case MAT_C_INT32:
                        case MAT_C_UINT32:
                            LOnBytes = 4*max( matvar->dims[0], matvar->dims[1] );
                            break;
                        case MAT_C_INT64:
                        case MAT_C_UINT64:
                            LOnBytes = 8*max( matvar->dims[0], matvar->dims[1] );
                            break;
                        default:
                            LOnBytes = 2*MaxFileSize;
                            break;
                    }
                    //*/
                    if( LOnBytes>(MaxFileSize) ){
                        continue;
                    }
                    VINFOUT[NVarsCP].CpyStr( matvar->name );
                    VINFOUT[NVarsCP].LenOfV = max( matvar->dims[0], matvar->dims[1] );
                    //
                    if( matvar->dims[0]==1 && matvar->dims[1]==1 ){
                        VINFOUT[NVarsCP].IsArrVect = FALSE;
                        switch( matvar->class_type ){
                            case MAT_C_DOUBLE:{
                                    mat_t* matfpget = (mat_t*)Mat_Open( FName, 1 );
                                    matvar_t* matvarget = (matvar_t*)Mat_VarRead( matfpget, matvar->name );
                                    VINFOUT[NVarsCP].FirstValue = ((double*)(matvarget->data))[0];
                                    Mat_VarFree(matvarget);
                                    Mat_Close(matfpget);
                                }
                                break;
                            case MAT_C_INT32:{
                                    mat_t* matfpget = (mat_t*)Mat_Open( FName, 1 );
                                    matvar_t* matvarget = (matvar_t*)Mat_VarRead( matfpget, matvar->name );
                                    VINFOUT[NVarsCP].FirstValue = ((int*)(matvarget->data))[0];
                                    Mat_VarFree(matvarget);
                                    Mat_Close(matfpget);
                                }
                                break;
                            default:
                                VINFOUT[NVarsCP].FirstValue = 0;
                                break;
                        }
                    }else{
                        VINFOUT[NVarsCP].IsArrVect = TRUE;
                    }
                    //
                    Mat_VarFree(matvar);
                    matvar = NULL;
                    NVarsCP++;
                    if( NVarsCP>NVarsMax[0] ){
                        Mat_Close(matfp);
                        return FALSE;
                    }
                }
            }
        }
        VINFOUT[0].NVARS = NVarsCP;
        NVarsMax[0] = NVarsCP;
        Mat_Close(matfp);
        return TRUE;
    }
    ///
    ///
    BOOL WriteDSPTAD( LPSTR LPFN ){
        Write_Resource( "DSP_TAD.exe", DSP_TAD_EXE, FILE_RES );
        if( LPFN!=NULL ){
            sprintf( LPFN, "CALL \"%s\\DSP_TAD.exe\"", PATH  );
            return TRUE;
        }
        return FALSE;
    }
    ///
    ///
    ///
    ///
    fftw_plan       planfft;
    fftw_plan       plan_I_fft;
    fftw_complex*   in_fft_ifft;
    fftw_complex*   out_fft_ifft;
    int             NPosFFT;
    int             NPosPot2FFT;
    ///
    BOOL SetPlanFFT( int NPos ){
        BOOL SetOrSet = FALSE;
        if( (NPos<1 || NPos!=NPosFFT) || SetOrSet ){
            if( planfft!=NULL ){
                fftw_destroy_plan(planfft);
            }
            if( plan_I_fft!=NULL ){
                fftw_destroy_plan(plan_I_fft);
            }
            if( in_fft_ifft!=NULL ){
                fftw_free(in_fft_ifft);
            }
            if( out_fft_ifft!=NULL ){
                fftw_free(out_fft_ifft);
            }
            NPosFFT = 0;
            NPosPot2FFT = 0;
            planfft         = NULL;
            plan_I_fft      = NULL;
            in_fft_ifft     = NULL;
            out_fft_ifft    = NULL;
        }
        if( (NPos!=NPosFFT && NPos>0) || (SetOrSet && NPos>0) ){
            /// NPosPot2FFT = NPos;///( 1<<nextpow2(NPos) );
            /// fftw_init_threads();fftw_plan_with_nthreads( 2 );
            ///
            in_fft_ifft     = (fftw_complex*)fftw_malloc( sizeof(fftw_complex) * (NPos + 2) );
            ZeroMemory( in_fft_ifft, sizeof(fftw_complex) * (NPos + 1) );
            ///
            out_fft_ifft    = (fftw_complex*)fftw_malloc( sizeof(fftw_complex) * (NPos + 2) );
            ZeroMemory( out_fft_ifft, sizeof(fftw_complex) * (NPos + 1) );
            ///
            plan_I_fft      = (fftw_plan)fftw_plan_dft_1d( NPos, in_fft_ifft, out_fft_ifft, FFTW_BACKWARD, FFTW_ESTIMATE );
            NPosFFT         = NPos;
            return TRUE;
        }
        return FALSE;
    }
    ///
    ///
    ///
    ///
    ///
    ///
    BOOL FFT( int NPos, double* DIn, double* DOutR, double* DOutI, BOOL Debug ){
        /// typedef double fftw_complex[2];
        /// NPos = 1<<nextpow2(NPos);
        /// if( (NPos%2)!=0 ){ NPos = NPos - 1; }
        SetPlanFFT( NPos );
        ///
        ///
        if( in_fft_ifft==NULL ){
            /// if( Debug ){ MessageBox( SetMainW(NULL), "if( in==NULL ){", "", 0 ); }
            return FALSE;
        }
        if( out_fft_ifft==NULL ){
            /// if( Debug ){ MessageBox( SetMainW(NULL), "if( out==NULL ){", "", 0 ); }
            return FALSE;
        }
        ///
        ///
        for( int i=0; i<NPos; i++ ){
            in_fft_ifft[i][0] = DIn[i];
            in_fft_ifft[i][1] = 0;
            ///
            out_fft_ifft[i][0] = 0;
            out_fft_ifft[i][1] = 0;
            ///
        }
        /// if( Debug ){ MessageBox( SetMainW(NULL), "for( int i=0; i<NPos; i++ ){", "", 0 ); }
        ///
        /// if( p==NULL ){ exit( 0 ); }
        /// if( Debug ){ MessageBox( SetMainW(NULL), "fftw_plan_dft_1d", "", 0 ); }
        ///
        planfft         = (fftw_plan)fftw_plan_dft_1d( NPos, in_fft_ifft, out_fft_ifft, FFTW_FORWARD, FFTW_ESTIMATE );
        fftw_execute( planfft ); // repeat as needed
        fftw_destroy_plan(planfft);
        planfft = NULL;
        /// if( Debug ){ MessageBox( SetMainW(NULL), "fftw_execute", "", 0 ); }
        ///
        if( DOutR!=NULL && DOutI!=NULL ){
            for( int i=0; i<NPos; i++ ){
                DOutR[i] = out_fft_ifft[i][0];
                DOutI[i] = out_fft_ifft[i][1];
            }
        }
        /// if( Debug ){ MessageBox( SetMainW(NULL), "for( int i=0; i<NPos; i++ ){", "", 0 ); }
        /// if( Debug ){ MessageBox( SetMainW(NULL), "fftw_destroy_plan", "", 0 ); }
        /// fftw_cleanup();
        /// if( Debug ){ MessageBox( SetMainW(NULL), "fftw_free", "", 0 ); }
        return TRUE;
    }
    ///
    ///
    ///
    ///
    ///
    ///
    ///
    ///
    BOOL IFFT( int NPos, double* DInR, double* DInI, double* DOut ){
        SetPlanFFT( NPos );
        ///if( (NPos%2)!=0 ){ NPos = NPos - 1;}
        ///
        ///
        if( DInR!=NULL && DInI!=NULL ){
            for( int i=0; i<NPos; i++ ){
                in_fft_ifft[i][0] = DInR[i];
                in_fft_ifft[i][1] = DInI[i];
                ///
                out_fft_ifft[i][0] = 0;
                out_fft_ifft[i][1] = 0;
                ///
            }
        }else{
            for( int i=0; i<NPos; i++ ){
                ///
                out_fft_ifft[i][0] = 0;
                out_fft_ifft[i][1] = 0;
                ///
            }
        }
        ///
        ///
        fftw_execute(plan_I_fft); // repeat as needed
        ///
        ///double NPos_1 = ((double)1)/((double)NPos);
        for( int i=0; i<NPos; i++ ){
            DOut[i] = out_fft_ifft[i][0]/NPos;
        }
        /// fftw_cleanup();
        return TRUE;
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
    BOOL SET_FIR_FILTER( int NPos, fftprepare* fftpIn, double* D_B, int NPos_B, BOOL Debug ){
        int NPosPot2 = ( 1<<nextpow2(NPos) );
        SetPlanFFT( NPosPot2 );
        ///
        ///
        ///
        double* InputB = (double*)malloc( (NPosPot2+8) * sizeof(double) );
        ZeroMemory( InputB, (NPosPot2) * sizeof(double) );
        /// double FsTemp = GetFs();
        FFT( NPosPot2, fftpIn->InputV,  fftpIn->RealV,  fftpIn->ImagV,  FALSE );
        ///
        memcpy( InputB, D_B, NPos_B * sizeof(double) );
        FFT( NPosPot2, InputB,    NULL,    NULL,    FALSE );
        free( InputB );
        InputB = NULL;
        ///
        double DVal_in_R = 0;
        double DVal_in_I = 0;
        double DVal_B_R  = 0;
        double DVal_B_I  = 0;
        ///
        for( int i=0; i<NPosPot2; i++ ){
            DVal_in_R = fftpIn->RealV[i];
            DVal_in_I = fftpIn->ImagV[i];
            ///
            DVal_B_R = out_fft_ifft[i][0];///fftpB.RealV[i];
            DVal_B_I = out_fft_ifft[i][1];///fftpB.ImagV[i];
            ///
            fftpIn->RealV[i] = DVal_in_R * DVal_B_R - DVal_in_I * DVal_B_I;
            fftpIn->ImagV[i] = DVal_in_I * DVal_B_R + DVal_B_I * DVal_in_R;
        }
        IFFT( NPosPot2, fftpIn->RealV, fftpIn->ImagV, fftpIn->InputV );
        ///
        ///
        ///
        return TRUE;
    }
    ///
    ///
    ///
    ///
    ///
    ///
    ///
    ///
    BOOL FSPECIAL_AVERAGE( RGBQUAD* Rgb_In_Out, int W_PIC, int H_PIC, int NOrd, int Offset ){
        ///
        fftw_plan       plan2d = NULL;
        fftw_complex*   in_fft2d = NULL;
        ///
        fftw_complex*   out_fft2d = NULL;
        ///
        fftw_complex*   out_NxN = NULL;
        ///
        ///
        ///
        ///
        int x = 0;
        int y = 0;
        ///
        int HMOV = 0;
        int HMOV_F = 0;
        const int W_1 = W_PIC - 1;
        const int H_1 = H_PIC - 1;
        ///
        fftw_complex*   ftwptr = NULL;
        RGBQUAD*        rgbqptr = NULL;
        ///
        ///
        ///
        /// fftw_plan_dft_2d
        ///
            int MaxofWH = max( W_PIC, H_PIC );
            MaxofWH = max( MaxofWH, NOrd );
            int W_N = 1<<nextpow2(MaxofWH);//1<<nextpow2(MaxofWH);
            int H_N = 1<<nextpow2(MaxofWH);
            ///
            int rankWH[2] = { W_N, H_N };///
            ///
            ///
            ///
            double NOrd_2 = ((double)NOrd) * ((double)NOrd);
            double NOrd_2_Inv = 1.0/NOrd_2;
            ///
            ///
            ///
            in_fft2d    = (fftw_complex*)fftw_malloc( sizeof(fftw_complex) * (W_N*H_N + W_N*NOrd) );
            ///
            if( in_fft2d==NULL ){
                exit( 1221 );
            }
            ZeroMemory(  in_fft2d, sizeof(fftw_complex) * (W_N*H_N + 15) );
            ///
            out_fft2d   = (fftw_complex*)fftw_malloc( sizeof(fftw_complex) * (W_N*H_N + W_N*NOrd) );
            ///
            if( out_fft2d==NULL ){
                exit( 1222 );
            }
            ZeroMemory( out_fft2d, sizeof(fftw_complex) * (W_N*H_N + 15) );
            ///
            out_NxN     = (fftw_complex*)fftw_malloc( sizeof(fftw_complex) * (W_N*H_N + W_N*NOrd) );
            ///
            if( out_NxN==NULL ){
                exit( 1223 );
            }
            ZeroMemory( out_NxN, sizeof(fftw_complex) * (W_N*H_N + 15) );
            ///
            ///ZeroMemory(  in_fft2d, sizeof(fftw_complex) * (W_N*H_N + 15) );
            ///ZeroMemory( out_fft2d, sizeof(fftw_complex) * (W_N*H_N + 15) );
            ///ZeroMemory( out_NxN, sizeof(fftw_complex) * (W_N*H_N + 15) );
            ///
            for( y=0; y<NOrd; y++ ){
                for( x=0; x<NOrd; x++ ){
                    in_fft2d[x+y*W_N][0] = NOrd_2_Inv;
                }
            }
            ///
            /// in_fft2d[NOrd/2+(NOrd/2)*W_N][0] = -((NOrd_2-1))/NOrd_2;
            ///
            ///
            ///
            ///
            ///
            ///
            ///
            ///
            plan2d = (fftw_plan)fftw_plan_dft( 2, rankWH, in_fft2d, out_NxN, FFTW_FORWARD, FFTW_ESTIMATE );
            fftw_execute( plan2d );
            fftw_destroy_plan( plan2d );
            ///
            ///
            ///
            ///
            ZeroMemory( in_fft2d, sizeof(fftw_complex) * (W_N*H_N + 15) );
            for( y=0; y<H_PIC; y++ ){
                HMOV   = (H_1 - y)*W_PIC + W_1;
                rgbqptr = &Rgb_In_Out[HMOV];
                //
                HMOV_F = y*W_N;
                ftwptr  = &in_fft2d[HMOV_F];
                //
                for( x=0; x<W_PIC; x++ ){
                    ftwptr[0][0] = rgbqptr->rgbRed;
                    ftwptr++;
                    rgbqptr--;
                }
            }
            ///
            plan2d = (fftw_plan)fftw_plan_dft( 2, rankWH, in_fft2d, out_fft2d, FFTW_FORWARD, FFTW_ESTIMATE );
            fftw_execute( plan2d );
            fftw_destroy_plan( plan2d );
            ///
            ///
            ///
            int WH_N = W_N * H_N;
            ///
            double DVal_in_R = 0;
            double DVal_in_I = 0;
            ///
            double DVal_B_R = 0;
            double DVal_B_I = 0;
            ///
            for( int i=0; i<WH_N; i++ ){
                ///
                DVal_in_R = out_fft2d[i][0];
                DVal_in_I = out_fft2d[i][1];
                ///
                DVal_B_R = out_NxN[i][0];
                DVal_B_I = out_NxN[i][1];
                ///
                ///
                in_fft2d[i][0] = DVal_in_R * DVal_B_R - DVal_in_I * DVal_B_I;
                in_fft2d[i][1] = DVal_in_I * DVal_B_R + DVal_B_I  * DVal_in_R;
                ///
                ///in_fft2d[i][0] = out_NxN[i][0];
                ///in_fft2d[i][1] = out_NxN[i][1];
                ///
            }
            ///
            ///
            ///
            /// ZeroMemory( out_fft2d, sizeof(fftw_complex) * (W_N*H_N + 15) );
            ///
            plan2d = (fftw_plan)fftw_plan_dft( 2, rankWH, in_fft2d, out_fft2d, FFTW_BACKWARD, FFTW_ESTIMATE );
            fftw_execute( plan2d );
            fftw_destroy_plan( plan2d );
            ///
            ///
            ///
            int NAdv = NOrd/2;
            if( (NOrd%2)==0 ){
                NAdv--;
            }
            double dblPrd = 1/((double)WH_N);
            for( int y=0; y<H_PIC; y++ ){
                HMOV   = (H_1 - y)*W_PIC + W_1;
                rgbqptr = &Rgb_In_Out[HMOV];
                //
                HMOV_F = NAdv + (y+NAdv)*W_N;
                ftwptr = &out_fft2d[HMOV_F];
                //
                for( int x=0; x<W_PIC; x++ ){
                    rgbqptr->rgbRed    =
                    rgbqptr->rgbGreen  =
                    rgbqptr->rgbBlue   = ParseByte( round( dblPrd * ftwptr[0][0] ) + Offset );
                    rgbqptr--;
                    ftwptr++;
                }
            }
            ///
            ///
            ///
            ///
            ///
            ///
        ///
        ///
        fftw_free(in_fft2d);
        fftw_free(out_NxN);
        fftw_free(out_fft2d);
        ///
        return TRUE;
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
    ///
    ///
    ///
    double imerode( RGBQUAD* Rgb_In_Out, int W_PIC, int H_PIC, double* Hist, int NOrdErode, BYTE OptErode, BYTE BFront ){
        return imerode( Rgb_In_Out, W_PIC, H_PIC, Hist, NOrdErode, OptErode, BFront, NULL, NULL );
    }
    double imerode( RGBQUAD* Rgb_In_Out, int W_PIC, int H_PIC, double* Hist, int NOrdErode, BYTE OptErode,
                                                        BYTE BFront, RGBQUAD* RgbStrl, int* W_P_H ){
        ///
        fftw_plan       plan2d = NULL;
        fftw_complex*   in_fft2d = NULL;
        fftw_complex*   out_fft2d = NULL;
        fftw_complex*   out_NxN = NULL;
        ///
        ///
        ///
        ///
        ///
        ///
        ///
        int NOrd_2 = 0;
        int NORM = 0;
        ///
        int IRes = 0;
            int MaxofWH = max( W_PIC, H_PIC );
            MaxofWH = max( W_PIC+NOrdErode, H_PIC+NOrdErode );
            int W_N = 1<<nextpow2(MaxofWH);//1<<nextpow2(MaxofWH);
            int H_N = 1<<nextpow2(MaxofWH);
            ///
            int rankWH[2] = { W_N, H_N };///
            ///
            ///
            ///
            ///
            ///
            ///
            in_fft2d    = (fftw_complex*)fftw_malloc( sizeof(fftw_complex) * (W_N*H_N + W_N*NOrdErode) );
            ///
            if( in_fft2d==NULL ){
                exit( 1221 );
            }
            ZeroMemory(  in_fft2d, sizeof(fftw_complex) * (W_N*H_N + 15) );
            ///
            out_fft2d   = (fftw_complex*)fftw_malloc( sizeof(fftw_complex) * (W_N*H_N + W_N*NOrdErode) );
            ///
            if( out_fft2d==NULL ){
                exit( 1222 );
            }
            ZeroMemory( out_fft2d, sizeof(fftw_complex) * (W_N*H_N + 15) );
            ///
            out_NxN     = (fftw_complex*)fftw_malloc( sizeof(fftw_complex) * (W_N*H_N + W_N*NOrdErode) );
            ///
            if( out_NxN==NULL ){
                exit( 1223 );
            }
            ZeroMemory( out_NxN, sizeof(fftw_complex) * (W_N*H_N + 15) );
            ///
            ///ZeroMemory(  in_fft2d, sizeof(fftw_complex) * (W_N*H_N + 15) );
            ///ZeroMemory( out_fft2d, sizeof(fftw_complex) * (W_N*H_N + 15) );
            ///ZeroMemory( out_NxN, sizeof(fftw_complex) * (W_N*H_N + 15) );
            ///
            ///
            ///
            ///
            ///
            ///
            ///
            if( RgbStrl!=NULL ){
                ZeroMemory( RgbStrl, 4 * 256 * 256 );
            }
            NORM = 0;
            switch( OptErode ){
                case 0:{/// BALL
                    NORM = NOrdErode + NOrdErode + 1;
                    int R_2 = NOrdErode * NOrdErode;
                    int Radius = 0;
                    NOrd_2 = 0;
                    for( int y=0; y<NORM; y++ ){
                        for( int x=0; x<NORM; x++ ){
                            Radius = ( (NOrdErode - x)*(NOrdErode - x) + (NOrdErode - y)*(NOrdErode - y) );
                            if( Radius<=R_2 ){
                                in_fft2d[x+y*W_N][0] = 1;
                                if( RgbStrl!=NULL ){
                                    RgbStrl[x+y*NORM].rgbRed     =
                                    RgbStrl[x+y*NORM].rgbGreen   =
                                    RgbStrl[x+y*NORM].rgbBlue    = 255;
                                }
                                NOrd_2++;
                            }
                        }
                    }
                }
                break;
                ///
                ///
                ///
                ///
                case 1:{/// SQUARE
                    NORM = NOrdErode;
                    NOrd_2 = NOrdErode * NOrdErode;
                    for( int y=0; y<NORM; y++ ){
                        for( int x=0; x<NORM; x++ ){
                            in_fft2d[x+y*W_N][0] = 1;
                            if( RgbStrl!=NULL ){
                                RgbStrl[x+y*NORM].rgbRed     =
                                RgbStrl[x+y*NORM].rgbGreen   =
                                RgbStrl[x+y*NORM].rgbBlue    = 255;
                            }
                        }
                    }
                }
                break;
                ///
                ///
                ///
                ///
                case 2:{/// DIAMOND
                    NORM = NOrdErode + NOrdErode + 1;
                    int R_2 = NOrdErode;
                    int Radius = 0;
                    NOrd_2 = 0;
                    for( int y=0; y<NORM; y++ ){
                        for( int x=0; x<NORM; x++ ){
                            Radius = ( abs(NOrdErode - x) + abs(NOrdErode - y) );
                            if( Radius<=R_2 ){
                                in_fft2d[x+y*W_N][0] = 1;
                                if( RgbStrl!=NULL ){
                                    RgbStrl[x+y*NORM].rgbRed     =
                                    RgbStrl[x+y*NORM].rgbGreen   =
                                    RgbStrl[x+y*NORM].rgbBlue    = 255;
                                }
                                NOrd_2++;
                            }
                        }
                    }
                }
                break;
            }
            ///
            if( W_P_H!=NULL ){
                W_P_H[0] = NORM;
            }
            ///
            ///
            ///
            ///
            ///
            ///
            ///
            plan2d = (fftw_plan)fftw_plan_dft( 2, rankWH, in_fft2d, out_NxN, FFTW_FORWARD, FFTW_ESTIMATE );
            fftw_execute( plan2d );
            fftw_destroy_plan( plan2d );
            ///
            ///
            ///
            ZeroMemory( in_fft2d, sizeof(fftw_complex) * (W_N*H_N + 15) );
            for( int y=0; y<H_PIC; y++ ){
                for( int x=0; x<W_PIC; x++ ){
                    in_fft2d[x+y*W_N][0] = Rgb_In_Out[x+y*W_PIC].rgbRed>250;
                }
            }
            ///
            plan2d = (fftw_plan)fftw_plan_dft( 2, rankWH, in_fft2d, out_fft2d, FFTW_FORWARD, FFTW_ESTIMATE );
            fftw_execute( plan2d );
            ///
            ///
            ///
            int WH_N = W_N * H_N;
            ///
            double DVal_in_R = 0;
            double DVal_in_I = 0;
            ///
            double DVal_B_R = 0;
            double DVal_B_I = 0;
            ///
            for( int i=0; i<WH_N; i++ ){
                ///
                DVal_in_R = out_fft2d[i][0];
                DVal_in_I = out_fft2d[i][1];
                ///
                DVal_B_R = out_NxN[i][0];
                DVal_B_I = out_NxN[i][1];
                ///
                ///
                in_fft2d[i][0] = DVal_in_R * DVal_B_R - DVal_in_I * DVal_B_I;
                in_fft2d[i][1] = DVal_in_I * DVal_B_R + DVal_B_I  * DVal_in_R;
                ///
                ///
            }
            ///
            fftw_free(out_NxN);
            ///
            ///
            /// ZeroMemory( out_fft2d, sizeof(fftw_complex) * (W_N*H_N + 15) );
            ///
            plan2d = (fftw_plan)fftw_plan_dft( 2, rankWH, in_fft2d, out_fft2d, FFTW_BACKWARD, FFTW_ESTIMATE );
            fftw_execute( plan2d );
            fftw_destroy_plan( plan2d );
            ///
            fftw_free(in_fft2d);
            ///
            ///
            ZeroMemory( Hist, 256 * sizeof(double) );
            int NAdv = NORM/2;
            if( BFront==0 ){
                for( int y=0; y<H_PIC; y++ ){
                    for( int x=0; x<W_PIC; x++ ){
                        IRes = ParseByte( 255*(int)( (int)( round( out_fft2d[(x+NAdv)+(y+NAdv)*W_N][0]/WH_N ) )==NOrd_2 ) );
                        /// IRes = 255*ParseByte( Rgb_In_Out[x+y*W_PIC].rgbRed>0 & (IRes<1) );
                        Rgb_In_Out[x+y*W_PIC].rgbRed    =
                        Rgb_In_Out[x+y*W_PIC].rgbGreen  =
                        Rgb_In_Out[x+y*W_PIC].rgbBlue   = IRes;///255 * (Rgb_In_Out[x+y*W_PIC].rgbRed>0 && (IRes<1));
                        Hist[IRes] = Hist[IRes] + 1;
                    }
                }
            }else{
                for( int y=0; y<H_PIC; y++ ){
                    for( int x=0; x<W_PIC; x++ ){
                        IRes = ( (int)round( out_fft2d[(x+NAdv)+(y+NAdv)*W_N][0]/WH_N ) )==NOrd_2;
                        /// IRes = 255*ParseByte( Rgb_In_Out[x+y*W_PIC].rgbRed>0 & (IRes<1) );
                        Rgb_In_Out[x+y*W_PIC].rgbRed    =
                        Rgb_In_Out[x+y*W_PIC].rgbGreen  =
                        Rgb_In_Out[x+y*W_PIC].rgbBlue   = 255 * (Rgb_In_Out[x+y*W_PIC].rgbRed>0 && (IRes<1));
                        Hist[IRes] = Hist[IRes] + 1;
                    }
                }
            }
        ///
        ///
        fftw_free(out_fft2d);
        ///
        return TRUE;
    }
    /// imerode
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
    ///
    ///
    ///
    ///
    ///
    ///
    ///
    ///
    /// dilate
    ///
    ///
    double imdilate( RGBQUAD* Rgb_In_Out, int W_PIC, int H_PIC, double* Hist, int NOrdErode, BYTE OptErode, BYTE BFront ){
        return imdilate( Rgb_In_Out, W_PIC, H_PIC, Hist, NOrdErode, OptErode, BFront, NULL, NULL );
    }
    double imdilate( RGBQUAD* Rgb_In_Out, int W_PIC, int H_PIC, double* Hist, int NOrdErode, BYTE OptErode, BYTE BFront,
                                                        RGBQUAD* RgbStrl, int* W_P_H ){
        ///
        fftw_plan       plan2d = NULL;
        fftw_complex*   in_fft2d = NULL;
        fftw_complex*   out_fft2d = NULL;
        fftw_complex*   out_NxN = NULL;
        ///
        ///
        ///
        ///
        ///
        ///
        ///
        int x = 0;
        int y = 0;
        ///
        const int W_1 = W_PIC - 1;
        const int H_1 = H_PIC - 1;
        ///
        fftw_complex*   ftwptr = NULL;
        RGBQUAD*        rgbqptr = NULL;
        ///
        ///
        ///
        ///
        ///
        ///
        int NOrd_2 = 0;
        int NORM = 0;
        ///
        int IRes = 0;
            int MaxofWH = max( W_PIC, H_PIC );
            MaxofWH = max( W_PIC+NOrdErode, H_PIC+NOrdErode );
            int W_N = 1<<nextpow2(MaxofWH);//1<<nextpow2(MaxofWH);
            int H_N = 1<<nextpow2(MaxofWH);
            ///
            int rankWH[2] = { W_N, H_N };///
            ///
            ///
            ///
            ///
            ///
            ///
            in_fft2d    = (fftw_complex*)fftw_malloc( sizeof(fftw_complex) * (W_N*H_N + W_N*NOrdErode) );
            ///
            if( in_fft2d==NULL ){
                exit( 1221 );
            }
            ZeroMemory(  in_fft2d, sizeof(fftw_complex) * (W_N*H_N + 15) );
            ///
            out_fft2d   = (fftw_complex*)fftw_malloc( sizeof(fftw_complex) * (W_N*H_N + W_N*NOrdErode) );
            ///
            if( out_fft2d==NULL ){
                exit( 1222 );
            }
            ZeroMemory( out_fft2d, sizeof(fftw_complex) * (W_N*H_N + 15) );
            ///
            out_NxN     = (fftw_complex*)fftw_malloc( sizeof(fftw_complex) * (W_N*H_N + W_N*NOrdErode) );
            ///
            if( out_NxN==NULL ){
                exit( 1223 );
            }
            ZeroMemory( out_NxN, sizeof(fftw_complex) * (W_N*H_N + 15) );
            ///
            ///ZeroMemory(  in_fft2d, sizeof(fftw_complex) * (W_N*H_N + 15) );
            ///ZeroMemory( out_fft2d, sizeof(fftw_complex) * (W_N*H_N + 15) );
            ///ZeroMemory( out_NxN, sizeof(fftw_complex) * (W_N*H_N + 15) );
            ///
            ///
            ///
            ///
            ///
            ///
            ///
            if( RgbStrl!=NULL ){
                ZeroMemory( RgbStrl, 4 * 256 * 256 );
            }
            NORM = 0;
            switch( OptErode ){
                case 0:{/// BALL
                    NORM = NOrdErode + NOrdErode + 1;
                    int R_2 = NOrdErode * NOrdErode;
                    int Radius = 0;
                    NOrd_2 = 0;
                    for( y=0; y<NORM; y++ ){
                        for( x=0; x<NORM; x++ ){
                            Radius = ( (NOrdErode - x)*(NOrdErode - x) + (NOrdErode - y)*(NOrdErode - y) );
                            if( Radius<=R_2 ){
                                in_fft2d[x+y*W_N][0] = 1;
                                if( RgbStrl!=NULL ){
                                    RgbStrl[x+y*NORM].rgbRed     =
                                    RgbStrl[x+y*NORM].rgbGreen   =
                                    RgbStrl[x+y*NORM].rgbBlue    = 255;
                                }
                                NOrd_2++;
                            }
                        }
                    }
                }
                break;
                ///
                ///
                ///
                ///
                case 1:{/// SQUARE
                    NORM = NOrdErode;
                    NOrd_2 = NOrdErode * NOrdErode;
                    for( y=0; y<NORM; y++ ){
                        for( x=0; x<NORM; x++ ){
                            in_fft2d[x+y*W_N][0] = 1;
                            if( RgbStrl!=NULL ){
                                RgbStrl[x+y*NORM].rgbRed     =
                                RgbStrl[x+y*NORM].rgbGreen   =
                                RgbStrl[x+y*NORM].rgbBlue    = 255;
                            }
                        }
                    }
                }
                break;
                ///
                ///
                ///
                ///
                case 2:{/// DIAMOND
                    NORM = NOrdErode + NOrdErode + 1;
                    int R_2 = NOrdErode;
                    int Radius = 0;
                    NOrd_2 = 0;
                    for( y=0; y<NORM; y++ ){
                        for( x=0; x<NORM; x++ ){
                            Radius = ( abs(NOrdErode - x) + abs(NOrdErode - y) );
                            if( Radius<=R_2 ){
                                in_fft2d[x+y*W_N][0] = 1;
                                if( RgbStrl!=NULL ){
                                    RgbStrl[x+y*NORM].rgbRed     =
                                    RgbStrl[x+y*NORM].rgbGreen   =
                                    RgbStrl[x+y*NORM].rgbBlue    = 255;
                                }
                                NOrd_2++;
                            }
                        }
                    }
                }
                break;
            }
            ///
            if( W_P_H!=NULL ){
                W_P_H[0] = NORM;
            }
            ///
            ///
            ///
            ///
            ///
            ///
            ///
            plan2d = (fftw_plan)fftw_plan_dft( 2, rankWH, in_fft2d, out_NxN, FFTW_FORWARD, FFTW_ESTIMATE );
            fftw_execute( plan2d );
            fftw_destroy_plan( plan2d );
            ///
            ///
            ///
        ///
        ///
            ///
            ZeroMemory( in_fft2d, sizeof(fftw_complex) * (W_N*H_N + 15) );
            for( y=0; y<H_PIC; y++ ){
                ftwptr = &in_fft2d[y*W_N];
                rgbqptr = &Rgb_In_Out[(W_1)+(H_1-y)*W_PIC];
                for( x=0; x<W_PIC; x++ ){
                    ftwptr[0][0] = rgbqptr->rgbRed>250;
                    ftwptr++;
                    rgbqptr--;
                }
            }
            ///
            plan2d = (fftw_plan)fftw_plan_dft( 2, rankWH, in_fft2d, out_fft2d, FFTW_FORWARD, FFTW_ESTIMATE );
            fftw_execute( plan2d );
            ///
            ///
            ///
            int WH_N = W_N * H_N;
            ///
            double DVal_in_R = 0;
            double DVal_in_I = 0;
            ///
            double DVal_B_R = 0;
            double DVal_B_I = 0;
            ///
            for( int i=0; i<WH_N; i++ ){
                ///
                DVal_in_R = out_fft2d[i][0];
                DVal_in_I = out_fft2d[i][1];
                ///
                DVal_B_R = out_NxN[i][0];
                DVal_B_I = out_NxN[i][1];
                ///
                ///
                in_fft2d[i][0] = DVal_in_R * DVal_B_R - DVal_in_I * DVal_B_I;
                in_fft2d[i][1] = DVal_in_I * DVal_B_R + DVal_B_I  * DVal_in_R;
                ///
                ///
            }
            ///
            fftw_free(out_NxN);
            ///
            ///
            /// ZeroMemory( out_fft2d, sizeof(fftw_complex) * (W_N*H_N + 15) );
            ///
            plan2d = (fftw_plan)fftw_plan_dft( 2, rankWH, in_fft2d, out_fft2d, FFTW_BACKWARD, FFTW_ESTIMATE );
            fftw_execute( plan2d );
            fftw_destroy_plan( plan2d );
            ///
            fftw_free(in_fft2d);
            ///
            ///
            ZeroMemory( Hist, 256 * sizeof(double) );
            int NAdv = NORM/2;
            if( BFront==0 ){
                for( y=0; y<H_PIC; y++ ){
                    rgbqptr = &Rgb_In_Out[(W_1)+(H_1-y)*W_PIC];
                    for( x=0; x<W_PIC; x++ ){
                        IRes = ParseByte( 255*(int)( (int)( round( out_fft2d[(x+NAdv)+(y+NAdv)*W_N][0]/WH_N ) )>0 ) );
                        rgbqptr->rgbRed    =
                        rgbqptr->rgbGreen  =
                        rgbqptr->rgbBlue   = IRes;///255 * (Rgb_In_Out[x+y*W_PIC].rgbRed>0 && (IRes<1));
                        Hist[IRes] = Hist[IRes] + 1;
                        rgbqptr--;
                    }
                }
            }else{
                for( y=0; y<H_PIC; y++ ){
                    rgbqptr = &Rgb_In_Out[(W_1)+(H_1-y)*W_PIC];
                    for( x=0; x<W_PIC; x++ ){
                        IRes = ( (int)round( out_fft2d[(x+NAdv)+(y+NAdv)*W_N][0]/WH_N ) )>0;
                        /// IRes = 255*ParseByte( Rgb_In_Out[x+y*W_PIC].rgbRed>0 & (IRes<1) );
                        rgbqptr->rgbRed    =
                        rgbqptr->rgbGreen  =
                        rgbqptr->rgbBlue   = 255 * (rgbqptr->rgbRed<1 && (IRes>0));
                        Hist[IRes] = Hist[IRes] + 1;
                        rgbqptr--;
                    }
                }
            }
        ///
        ///
        fftw_free(out_fft2d);
        ///
        return TRUE;
    }
    /// dilate
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
    ///
    ///
    /// imopen
    ///
    ///
    double imopen( RGBQUAD* Rgb_In_Out, int W_PIC, int H_PIC, double* Hist, int NOrdErode, BYTE OptErode, BYTE BFront ){
        return imopen( Rgb_In_Out, W_PIC, H_PIC, Hist, NOrdErode, OptErode, BFront, NULL, NULL );
    }
    double imopen( RGBQUAD* Rgb_In_Out, int W_PIC, int H_PIC, double* Hist, int NOrdErode, BYTE OptErode, BYTE BFront,
                                                        RGBQUAD* RgbStrl, int* W_P_H ){
        ///
        fftw_plan       plan2d = NULL;
        fftw_complex*   in_fft2d = NULL;
        fftw_complex*   out_fft2d = NULL;
        fftw_complex*   out_NxN = NULL;
        ///
        ///
        ///
        ///
        ///
        ///
        ///
        int x = 0;
        int y = 0;
        ///
        const int W_1 = W_PIC - 1;
        const int H_1 = H_PIC - 1;
        ///
        fftw_complex*   ftwptr = NULL;
        RGBQUAD*        rgbqptr = NULL;
        ///
        ///
        ///
        ///
        ///
        ///
        ///
        int NOrd_2 = 0;
        int NORM = 0;
        ///
        int IRes = 0;
            int MaxofWH = max( W_PIC, H_PIC );
            MaxofWH = max( W_PIC+NOrdErode, H_PIC+NOrdErode );
            int W_N = 1<<nextpow2(MaxofWH);//1<<nextpow2(MaxofWH);
            int H_N = 1<<nextpow2(MaxofWH);
            ///
            int rankWH[2] = { W_N, H_N };///
            ///
            ///
            ///
            ///
            ///
            ///
            in_fft2d    = (fftw_complex*)fftw_malloc( sizeof(fftw_complex) * (W_N*H_N + W_N*NOrdErode) );
            ///
            if( in_fft2d==NULL ){
                exit( 1221 );
            }
            ZeroMemory(  in_fft2d, sizeof(fftw_complex) * (W_N*H_N + 15) );
            ///
            out_fft2d   = (fftw_complex*)fftw_malloc( sizeof(fftw_complex) * (W_N*H_N + W_N*NOrdErode) );
            ///
            if( out_fft2d==NULL ){
                exit( 1222 );
            }
            ZeroMemory( out_fft2d, sizeof(fftw_complex) * (W_N*H_N + 15) );
            ///
            out_NxN     = (fftw_complex*)fftw_malloc( sizeof(fftw_complex) * (W_N*H_N + W_N*NOrdErode) );
            ///
            if( out_NxN==NULL ){
                exit( 1223 );
            }
            ZeroMemory( out_NxN, sizeof(fftw_complex) * (W_N*H_N + 15) );
            ///
            ///
            ///
            ///
            ///
            ///
            ///
            if( RgbStrl!=NULL ){
                ZeroMemory( RgbStrl, 4 * 256 * 256 );
            }
            NORM = 0;
            switch( OptErode ){
                case 0:{/// BALL
                    NORM = NOrdErode + NOrdErode + 1;
                    int R_2 = NOrdErode * NOrdErode;
                    int Radius = 0;
                    NOrd_2 = 0;
                    for( y=0; y<NORM; y++ ){
                        for( x=0; x<NORM; x++ ){
                            Radius = ( (NOrdErode - x)*(NOrdErode - x) + (NOrdErode - y)*(NOrdErode - y) );
                            if( Radius<=R_2 ){
                                in_fft2d[x+y*W_N][0] = 1;
                                if( RgbStrl!=NULL ){
                                    RgbStrl[x+y*NORM].rgbRed     =
                                    RgbStrl[x+y*NORM].rgbGreen   =
                                    RgbStrl[x+y*NORM].rgbBlue    = 255;
                                }
                                NOrd_2++;
                            }
                        }
                    }
                }
                break;
                ///
                ///
                ///
                ///
                case 1:{/// SQUARE
                    NORM = NOrdErode;
                    NOrd_2 = NOrdErode * NOrdErode;
                    for( y=0; y<NORM; y++ ){
                        for( x=0; x<NORM; x++ ){
                            in_fft2d[x+y*W_N][0] = 1;
                            if( RgbStrl!=NULL ){
                                RgbStrl[x+y*NORM].rgbRed     =
                                RgbStrl[x+y*NORM].rgbGreen   =
                                RgbStrl[x+y*NORM].rgbBlue    = 255;
                            }
                        }
                    }
                }
                break;
                ///
                ///
                ///
                ///
                case 2:{/// DIAMOND
                    NORM = NOrdErode + NOrdErode + 1;
                    int R_2 = NOrdErode;
                    int Radius = 0;
                    NOrd_2 = 0;
                    for( y=0; y<NORM; y++ ){
                        for( x=0; x<NORM; x++ ){
                            Radius = ( abs(NOrdErode - x) + abs(NOrdErode - y) );
                            if( Radius<=R_2 ){
                                in_fft2d[x+y*W_N][0] = 1;
                                if( RgbStrl!=NULL ){
                                    RgbStrl[x+y*NORM].rgbRed     =
                                    RgbStrl[x+y*NORM].rgbGreen   =
                                    RgbStrl[x+y*NORM].rgbBlue    = 255;
                                }
                                NOrd_2++;
                            }
                        }
                    }
                }
                break;
            }
            ///
            if( W_P_H!=NULL ){
                W_P_H[0] = NORM;
            }
            ///
            ///
            ///
            ///
            ///
            ///
            ///
            plan2d = (fftw_plan)fftw_plan_dft( 2, rankWH, in_fft2d, out_NxN, FFTW_FORWARD, FFTW_ESTIMATE );
            fftw_execute( plan2d );
            fftw_destroy_plan( plan2d );
            ///
            ///
            ///
            ZeroMemory( in_fft2d, sizeof(fftw_complex) * (W_N*H_N + 15) );
            for( int y=0; y<H_PIC; y++ ){
                for( int x=0; x<W_PIC; x++ ){
                    in_fft2d[x+y*W_N][0] = (double)( Rgb_In_Out[x+y*W_PIC].rgbRed>250 );
                }
            }
            ///
            plan2d = (fftw_plan)fftw_plan_dft( 2, rankWH, in_fft2d, out_fft2d, FFTW_FORWARD, FFTW_ESTIMATE );
            fftw_execute( plan2d );
            ///
            ///
            ///
            int WH_N = W_N * H_N;
            ///
            double DVal_in_R = 0;
            double DVal_in_I = 0;
            ///
            double DVal_B_R = 0;
            double DVal_B_I = 0;
            ///
            for( int i=0; i<WH_N; i++ ){
                ///
                DVal_in_R = out_fft2d[i][0];
                DVal_in_I = out_fft2d[i][1];
                ///
                DVal_B_R = out_NxN[i][0];
                DVal_B_I = out_NxN[i][1];
                ///
                ///
                in_fft2d[i][0] = DVal_in_R * DVal_B_R - DVal_in_I * DVal_B_I;
                in_fft2d[i][1] = DVal_in_I * DVal_B_R + DVal_B_I  * DVal_in_R;
                ///
                ///
            }
            ///
            ///
            ///
            /// ZeroMemory( out_fft2d, sizeof(fftw_complex) * (W_N*H_N + 15) );
            ///
            plan2d = (fftw_plan)fftw_plan_dft( 2, rankWH, in_fft2d, out_fft2d, FFTW_BACKWARD, FFTW_ESTIMATE );
            fftw_execute( plan2d );
            fftw_destroy_plan( plan2d );
            ///
            int NAdv = NORM/2;
            ///
            ///
            ///
            fftw_complex*   ftwptrA = NULL;
        ///
            ///
            ///
            ZeroMemory(  in_fft2d, sizeof(fftw_complex) * (W_N*H_N) );
            double W_I = 1/((double)WH_N);
            for( y=0; y<H_PIC; y++ ){
                ftwptr = &in_fft2d[(W_1)+(H_1-y)*W_N];
                ftwptrA = &out_fft2d[(NAdv)+(y+NAdv)*W_N];
                for( x=0; x<W_PIC; x++ ){
                    /// in_fft2d[y][0] = (double)((int)round( out_fft2d[y][0]/WH_N )==NOrd_2);
                    ftwptr[0][0] = (double)((int)round( W_I * ftwptrA[0][0] )==NOrd_2);
                    ftwptrA++;
                    ftwptr--;
                }
            }
            ///
            ///
            ///
            ///
            ///
            plan2d = (fftw_plan)fftw_plan_dft( 2, rankWH, in_fft2d, out_fft2d, FFTW_FORWARD, FFTW_ESTIMATE );
            fftw_execute( plan2d );
            fftw_destroy_plan( plan2d );
            ///
            ///
            for( int i=0; i<WH_N; i++ ){
                ///
                DVal_in_R = out_fft2d[i][0];
                DVal_in_I = out_fft2d[i][1];
                ///
                DVal_B_R = out_NxN[i][0];
                DVal_B_I = out_NxN[i][1];
                ///
                ///
                in_fft2d[i][0] = DVal_in_R * DVal_B_R - DVal_in_I * DVal_B_I;
                in_fft2d[i][1] = DVal_in_I * DVal_B_R + DVal_B_I  * DVal_in_R;
                ///
                ///
            }
            ///
            fftw_free(out_NxN);
            ///
            plan2d = (fftw_plan)fftw_plan_dft( 2, rankWH, in_fft2d, out_fft2d, FFTW_BACKWARD, FFTW_ESTIMATE );
            fftw_execute( plan2d );
            fftw_destroy_plan( plan2d );
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
            ///
            ///
            fftw_free(in_fft2d);
            ///
            ///
            ///
            ///
            ZeroMemory( Hist, 256 * sizeof(double) );
            if( BFront==0 ){
                for( int y=0; y<H_PIC; y++ ){
                    rgbqptr = &Rgb_In_Out[(W_1)+(H_1-y)*W_PIC];
                    ftwptrA = &out_fft2d[(NAdv)+(y+NAdv)*W_N];
                    for( int x=0; x<W_PIC; x++ ){
                        IRes = ParseByte( 255*(int)( (int)( round( W_I * ftwptrA[0][0] ) )>0 ) );
                        rgbqptr->rgbRed    =
                        rgbqptr->rgbGreen  =
                        rgbqptr->rgbBlue   = IRes;
                        Hist[IRes] = Hist[IRes] + 1;
                        ftwptrA++;
                        rgbqptr--;
                    }
                }
            }else{
                for( int y=0; y<H_PIC; y++ ){
                    rgbqptr = &Rgb_In_Out[(W_1)+(H_1-y)*W_PIC];
                    ftwptrA = &out_fft2d[(NAdv)+(y+NAdv)*W_N];
                    for( int x=0; x<W_PIC; x++ ){
                        IRes = 255 * (int)( rgbqptr->rgbRed>0 && ( ( (int)round( W_I * ftwptrA[0][0] ) )<1 ) );
                        rgbqptr->rgbRed    =
                        rgbqptr->rgbGreen  =
                        rgbqptr->rgbBlue   = IRes;
                        Hist[IRes] = Hist[IRes] + 1;
                        ftwptrA++;
                        rgbqptr--;
                    }
                }
            }
        ///
        ///
        fftw_free(out_fft2d);
        ///
        return TRUE;
    }
    /// imopen
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
    ///
    ///
    /// imclose
    ///
    ///
    double imclose( RGBQUAD* Rgb_In_Out, int W_PIC, int H_PIC, double* Hist, int NOrdErode, BYTE OptErode,  BYTE BFront ){
        return imclose( Rgb_In_Out, W_PIC, H_PIC, Hist, NOrdErode, OptErode, BFront, NULL, NULL );
    }
    double imclose( RGBQUAD* Rgb_In_Out, int W_PIC, int H_PIC, double* Hist, int NOrdErode, BYTE OptErode, BYTE BFront,
                                                        RGBQUAD* RgbStrl, int* W_P_H ){
        ///
        fftw_plan       plan2d = NULL;
        fftw_complex*   in_fft2d = NULL;
        fftw_complex*   out_fft2d = NULL;
        fftw_complex*   out_NxN = NULL;
        ///
        ///
        ///
        ///
        ///
        int x = 0;
        int y = 0;
        ///
        const int W_1 = W_PIC - 1;
        const int H_1 = H_PIC - 1;
        ///
        fftw_complex*   ftwptr = NULL;
        RGBQUAD*        rgbqptr = NULL;
        ///
        ///
        ///
        ///
        ///
        int NOrd_2 = 0;
        int NORM = 0;
        ///
        int IRes = 0;
            int MaxofWH = max( W_PIC, H_PIC );
            MaxofWH = max( W_PIC+NOrdErode, H_PIC+NOrdErode );
            int W_N = 1<<nextpow2(MaxofWH);//1<<nextpow2(MaxofWH);
            int H_N = 1<<nextpow2(MaxofWH);
            ///
            int rankWH[2] = { W_N, H_N };///
            ///
            ///
            ///
            ///
            ///
            ///
            in_fft2d    = (fftw_complex*)fftw_malloc( sizeof(fftw_complex) * (W_N*H_N + W_N*NOrdErode) );
            ///
            if( in_fft2d==NULL ){
                exit( 1221 );
            }
            ZeroMemory(  in_fft2d, sizeof(fftw_complex) * (W_N*H_N + 15) );
            ///
            out_fft2d   = (fftw_complex*)fftw_malloc( sizeof(fftw_complex) * (W_N*H_N + W_N*NOrdErode) );
            ///
            if( out_fft2d==NULL ){
                exit( 1222 );
            }
            ZeroMemory( out_fft2d, sizeof(fftw_complex) * (W_N*H_N + 15) );
            ///
            out_NxN     = (fftw_complex*)fftw_malloc( sizeof(fftw_complex) * (W_N*H_N + W_N*NOrdErode) );
            ///
            if( out_NxN==NULL ){
                exit( 1223 );
            }
            ZeroMemory( out_NxN, sizeof(fftw_complex) * (W_N*H_N + 15) );
            ///
            ///
            ///
            ///
            ///
            ///
            ///
            if( RgbStrl!=NULL ){
                ZeroMemory( RgbStrl, 4 * 256 * 256 );
            }
            NORM = 0;
            switch( OptErode ){
                case 0:{/// BALL
                    NORM = NOrdErode + NOrdErode + 1;
                    int R_2 = NOrdErode * NOrdErode;
                    int Radius = 0;
                    NOrd_2 = 0;
                    for( y=0; y<NORM; y++ ){
                        for( x=0; x<NORM; x++ ){
                            Radius = ( (NOrdErode - x)*(NOrdErode - x) + (NOrdErode - y)*(NOrdErode - y) );
                            if( Radius<=R_2 ){
                                in_fft2d[x+y*W_N][0] = 1;
                                if( RgbStrl!=NULL ){
                                    RgbStrl[x+y*NORM].rgbRed     =
                                    RgbStrl[x+y*NORM].rgbGreen   =
                                    RgbStrl[x+y*NORM].rgbBlue    = 255;
                                }
                                NOrd_2++;
                            }
                        }
                    }
                }
                break;
                ///
                ///
                ///
                ///
                case 1:{/// SQUARE
                    NORM = NOrdErode;
                    NOrd_2 = NOrdErode * NOrdErode;
                    for( y=0; y<NORM; y++ ){
                        for( x=0; x<NORM; x++ ){
                            in_fft2d[x+y*W_N][0] = 1;
                            if( RgbStrl!=NULL ){
                                RgbStrl[x+y*NORM].rgbRed     =
                                RgbStrl[x+y*NORM].rgbGreen   =
                                RgbStrl[x+y*NORM].rgbBlue    = 255;
                            }
                        }
                    }
                }
                break;
                ///
                ///
                ///
                ///
                case 2:{/// DIAMOND
                    NORM = NOrdErode + NOrdErode + 1;
                    int R_2 = NOrdErode;
                    int Radius = 0;
                    NOrd_2 = 0;
                    for( y=0; y<NORM; y++ ){
                        for( x=0; x<NORM; x++ ){
                            Radius = ( abs(NOrdErode - x) + abs(NOrdErode - y) );
                            if( Radius<=R_2 ){
                                in_fft2d[x+y*W_N][0] = 1;
                                if( RgbStrl!=NULL ){
                                    RgbStrl[x+y*NORM].rgbRed     =
                                    RgbStrl[x+y*NORM].rgbGreen   =
                                    RgbStrl[x+y*NORM].rgbBlue    = 255;
                                }
                                NOrd_2++;
                            }
                        }
                    }
                }
                break;
            }
            ///
            if( W_P_H!=NULL ){
                W_P_H[0] = NORM;
            }
            ///
            ///
            ///
            ///
            ///
            ///
            ///
            plan2d = (fftw_plan)fftw_plan_dft( 2, rankWH, in_fft2d, out_NxN, FFTW_FORWARD, FFTW_ESTIMATE );
            fftw_execute( plan2d );
            fftw_destroy_plan( plan2d );
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
            ZeroMemory( in_fft2d, sizeof(fftw_complex) * (W_N*H_N + 15) );
            for( y=0; y<H_PIC; y++ ){
                rgbqptr = &Rgb_In_Out[W_1+(H_1-y)*W_PIC];
                ftwptr = &in_fft2d[y*W_N];
                for( x=0; x<W_PIC; x++ ){
                    ftwptr[0][0] = rgbqptr->rgbRed>250;
                    rgbqptr--;
                    ftwptr++;
                }
            }
            ///
            plan2d = (fftw_plan)fftw_plan_dft( 2, rankWH, in_fft2d, out_fft2d, FFTW_FORWARD, FFTW_ESTIMATE );
            fftw_execute( plan2d );
            ///
            ///
            ///
            int WH_N = W_N * H_N;
            ///
            double DVal_in_R = 0;
            double DVal_in_I = 0;
            ///
            double DVal_B_R = 0;
            double DVal_B_I = 0;
            ///
            for( int i=0; i<WH_N; i++ ){
                ///
                DVal_in_R = out_fft2d[i][0];
                DVal_in_I = out_fft2d[i][1];
                ///
                DVal_B_R = out_NxN[i][0];
                DVal_B_I = out_NxN[i][1];
                ///
                ///
                in_fft2d[i][0] = DVal_in_R * DVal_B_R - DVal_in_I * DVal_B_I;
                in_fft2d[i][1] = DVal_in_I * DVal_B_R + DVal_B_I  * DVal_in_R;
                ///
                ///
            }
            ///
            ///
            ///
            /// ZeroMemory( out_fft2d, sizeof(fftw_complex) * (W_N*H_N + 15) );
            ///
            plan2d = (fftw_plan)fftw_plan_dft( 2, rankWH, in_fft2d, out_fft2d, FFTW_BACKWARD, FFTW_ESTIMATE );
            fftw_execute( plan2d );
            fftw_destroy_plan( plan2d );
            ///
            int NAdv = NORM/2;
            ///
            fftw_complex* ftwptrA = NULL;
            ///
            double W_I = 1/((double)WH_N);
            ///
            ZeroMemory(  in_fft2d, sizeof(fftw_complex) * (W_N*H_N) );
            for( y=0; y<H_PIC; y++ ){
                ftwptr = &in_fft2d[W_1 + (H_1-y)*W_N];
                ftwptrA = &out_fft2d[(NAdv)+(y+NAdv)*W_N];
                for( x=0; x<W_PIC; x++ ){
                    ftwptr[0][0] = (double)((int)round( W_I * ftwptrA[0][0] )>0  );
                    ftwptrA++;
                    ftwptr--;
                }
            }
            ZeroMemory(  out_fft2d, sizeof(fftw_complex) * (W_N*H_N) );
            ///
            ///
            ///
            ///
            ///
            plan2d = (fftw_plan)fftw_plan_dft( 2, rankWH, in_fft2d, out_fft2d, FFTW_FORWARD, FFTW_ESTIMATE );
            fftw_execute( plan2d );
            fftw_destroy_plan( plan2d );
            ///
            ///
            for( int i=0; i<WH_N; i++ ){
                ///
                DVal_in_R = out_fft2d[i][0];
                DVal_in_I = out_fft2d[i][1];
                ///
                DVal_B_R = out_NxN[i][0];
                DVal_B_I = out_NxN[i][1];
                ///
                ///
                in_fft2d[i][0] = DVal_in_R * DVal_B_R - DVal_in_I * DVal_B_I;
                in_fft2d[i][1] = DVal_in_I * DVal_B_R + DVal_B_I  * DVal_in_R;
                ///
                ///
            }
            ///
            fftw_free(out_NxN);
            ///
            plan2d = (fftw_plan)fftw_plan_dft( 2, rankWH, in_fft2d, out_fft2d, FFTW_BACKWARD, FFTW_ESTIMATE );
            fftw_execute( plan2d );
            fftw_destroy_plan( plan2d );
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
            ///
            ///
            fftw_free(in_fft2d);
            ///
            ZeroMemory( Hist, 256 * sizeof(double) );
            if( BFront==0 ){
                for( y=0; y<H_PIC; y++ ){
                    rgbqptr = &Rgb_In_Out[y*W_PIC];
                    ftwptr = &out_fft2d[(NAdv)+(y+NAdv)*W_N];
                    for( x=0; x<W_PIC; x++ ){
                        IRes = ParseByte( 255*(int)( (int)( round( W_I * ftwptr[0][0] ) )==NOrd_2 ) );
                        rgbqptr->rgbRed    =
                        rgbqptr->rgbGreen  =
                        rgbqptr->rgbBlue   = IRes;
                        Hist[IRes] = Hist[IRes] + 1;
                        ftwptr++;
                        rgbqptr++;
                    }
                }
            }else{
                for( y=0; y<H_PIC; y++ ){
                    rgbqptr = &Rgb_In_Out[y*W_PIC];
                    ftwptr = &out_fft2d[(NAdv)+(y+NAdv)*W_N];
                    for( x=0; x<W_PIC; x++ ){
                        IRes = 255 * (int)( ( ( round( W_I * ftwptr[0][0] ) )==NOrd_2 ) & (rgbqptr->rgbRed<1) );
                        rgbqptr->rgbRed    =
                        rgbqptr->rgbGreen  =
                        rgbqptr->rgbBlue   = IRes;
                        Hist[IRes] = Hist[IRes] + 1;
                        ftwptr++;
                        rgbqptr++;
                    }
                }
            }
        ///
        ///
        fftw_free(out_fft2d);
        ///
        return TRUE;
    }
    /// imclose
    ///
    ///
    ///
    ///
    ///
    ///
    ///
    ///
    ///
};
DSP_LOAD  DSP_MNG;
///
///
///

/// /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL Dlg1D_PreloadMAT( HWND hwnd, UINT  message, WPARAM wParam, LPARAM lParam ){
    static VarsInfo* VINF = NULL;
    switch(message){
        case WM_INITDIALOG:{
            VINF = (VarsInfo*)lParam;
            int wwin = 0;
            int hwin = 0;
            WindowSize( hwnd, &wwin, &hwin );
            ///
            ///
            int HWIN = 24*VINF[0].NVARS;
            if( HWIN>400 ){
                HWIN = 400;
            }
            MoveWindow( hwnd, 1, 1, wwin+8, 72 + HWIN, TRUE );
            MoveWindow( GetDlgItem(hwnd,100), 1, 44, wwin-2, 12 + HWIN, TRUE );
            CenterWindow( hwnd, 0, 0, NULL );
            char T2T[256] = {0};
            for( int i=0; i<VINF[0].NVARS; i++ ){
                if( VINF[i].IsArrVect ){
                    SendDlgItemMessage( hwnd, 100, LB_ADDSTRING, 0, (LPARAM)VINF[i].VName );
                }else{
                    sprintf( T2T, "%s  (%g)", VINF[i].VName, VINF[i].FirstValue );
                    SendDlgItemMessage( hwnd, 100, LB_ADDSTRING, 0, (LPARAM)T2T );
                }
                ///MessageBoxA( hwnd, VINF[i].VName, "", 0 );
            }
            SendDlgItemMessage( hwnd, 100, LB_SETCURSEL, 0, 0 );
            SetWindowTextA( hwnd, "Señales de una dimensión." );
        }
        break;
        case WM_COMMAND:{
            if( LOWORD(wParam)==IDYES ){
                EndDialog( hwnd, SendDlgItemMessage( hwnd, 100, LB_GETCURSEL, 0, 0 ) );
            }
            if( LOWORD(wParam)==IDCANCEL ){
                EndDialog( hwnd, -1 );
            }
            if( HIWORD(wParam)==LBN_DBLCLK ){
                EndDialog( hwnd, SendDlgItemMessage( hwnd, 100, LB_GETCURSEL, 0, 0 ) );
            }
        }
        break;
    }
    return FALSE;
}
/// /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///#include "Types_Structs_DSP.cpp"
#include "FDESIGN\TYPES_FDESIGN.cpp"
///
///
#include "DIM1.cpp"
#include "DIM2.cpp"

