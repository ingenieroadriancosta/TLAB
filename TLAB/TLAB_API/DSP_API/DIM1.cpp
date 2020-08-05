BOOL Dlg1D_PreloadMAT( HWND hwnd, UINT  message, WPARAM wParam, LPARAM lParam );
#define Graph_Signal            0
#define Filt_Signal             1
#define Windowing_Signal        2
#define FFT_Signal              3
#define SPECTOGRAM_Signal       4
#define WIGNER_Signal           5
#define CHOI_WIL_Signal         6
#define PLAY_Signal             7
#define NO_PROC_Signal          8
struct SIGNAL_1_D_LOAD{
    int                     SType;
    int                     STypeT;
    char                    FName[1024];
    char                    FNameT[1024];
    char                    VarName[128];
    char                    VarNameT[128];
    int                     LOfVar;
    int                     LOfVarT;
    int                     Fs;
    int                     FsT;
    double                  f;
    double                  fT;
    BYTE                    NChannels;
    BYTE                    NChannelsT;
    BYTE                    ChannSel;
    BYTE                    ChannSelT;
    DWORD                   BitsPerSample;
    DWORD                   BitsPerSampleT;
    int                     TimeSignal;
    int                     TimeSignalT;
};
///
///
DWORD WINAPI ThreadCall_PlaySignal( LPVOID pv );
///
struct DIM1{
    int                                 IPtr;
    char                                DReserved[4];
    SIGNAL_1_D_LOAD                     SLOAD;
    int                                 ProcType;
    /// int                                 S_Input;
    BOOL                                IsLoaded;
    int                                 Ind_File;
    HANDLE                              HFileMod;
    DIM1*                               D1_IN;
    DIM1*                               D1_INIT;
    char                                CHAR_PATH[1024];
    DWORD                               NBytesBuff;
    DWORD                               NPosBuff;
    double*                             DBuff;
    HBITMAP                             HBMP2PLOT;
    double                              MaxValFFT;
    BOOL                                OnPlay;
    HWAVEOUT                            HWaveOut;
    WAVEHDR                             WHeader;
    WAVEFORMATEX                        WaveFormat;
    short*                              SPTR2PLAY;
    FDESIGN_CLASS                       FDS;
    BOOL                                PLAYSELECTED[MaxProcs];
    double                              PLAY_PERCENT[MaxProcs];
    BOOL                                PLAY_SET;
    DWORD                               NPosFFT;
    DIM1(){
        IPtr = 0;
        SLOAD.Fs = 2000;
        SLOAD.FsT = 2000;
        SLOAD.fT = 100;
        SLOAD.f = 100;
        D1_IN = NULL;
        D1_INIT = NULL;
        DBuff = NULL;
        SPTR2PLAY = NULL;
        ProcType = -1;
        NPosBuff = 0;
        OnPlay = 0;
        strcpy( DReserved, "D1R" );
        for( int i=0; i<MaxProcs; i++ ){
            PLAY_PERCENT[i] = 1;
        }
    }
    /// ///////////////////////////////////////////////////////
    /// ///////////////////////////////////////////////////////
    ~DIM1(){
        FreeBuff();
        FreeAll();
        if( HFileMod!=NULL ){
            CloseHandle(HFileMod);
            HFileMod = NULL;
        }
        DeleteFileA( CHAR_PATH );
        if( HBMP2PLOT!=NULL ){
            DeleteObject( HBMP2PLOT );
        }
    }
    /// ///////////////////////////////////////////////////////
    /// ///////////////////////////////////////////////////////
    HBITMAP	NewBitmap( int W_NEWHB, int H_NEWHB ){ return CreateCompatibleBitmap( GetDC( 0 ), W_NEWHB, H_NEWHB );}
    HBITMAP GetPicPlot(){
        return HBMP2PLOT;
    }
    /*
    BOOL SetPicPlot( HBITMAP HBMP_IN ){
        if( IsLoaded && HBMP_IN!=NULL ){
            if( HBMP2PLOT!=NULL ){
                DeleteObject( HBMP2PLOT );
            }
            BITMAP BMP = {0};
            GetObject( HBMP_IN, sizeof(BITMAP) , (LPSTR)&BMP );
            HBMP2PLOT = NewBitmap( BMP.bmWidth, BMP.bmHeight );
            RGBQUAD* RgbQ = (RGBQUAD*)malloc( 4 * BMP.bmWidth * BMP.bmHeight + 1024 );
            GetBitmapBits( HBMP_IN, 4 * BMP.bmWidth * BMP.bmHeight, RgbQ );
            SetBitmapBits( HBMP2PLOT, 4 * BMP.bmWidth * BMP.bmHeight, RgbQ );
            free( RgbQ );
            return TRUE;
        }
        return FALSE;
    }
    //*/
    //*
    ///
    BOOL FreePicPlot(){
        DeleteObject( HBMP2PLOT );
        HBMP2PLOT = NULL;
        return TRUE;
    }
    ///
    BOOL SetPicPlot( HBITMAP HBMP_IN ){
        if( HBMP_IN!=NULL ){
            BITMAP BMPOWN = {0};
            BITMAP BMP = {0};
            GetObject( HBMP_IN, sizeof(BITMAP) , (LPSTR)&BMP );
            if( HBMP2PLOT==NULL ){
                HBMP2PLOT = NewBitmap( BMP.bmWidth, BMP.bmHeight );
            }else{
                GetObject( HBMP2PLOT, sizeof(BITMAP) , (LPSTR)&BMPOWN );
                if( BMPOWN.bmWidth!=BMP.bmWidth || BMPOWN.bmHeight!=BMP.bmHeight ){
                    DeleteObject( HBMP2PLOT );
                    HBMP2PLOT = NewBitmap( BMP.bmWidth, BMP.bmHeight );
                }
            }
            RGBQUAD* RgbQ = (RGBQUAD*)malloc( 4 * BMP.bmWidth * BMP.bmHeight + 1024 );
            ZeroMemory( RgbQ, 4 * BMP.bmWidth * BMP.bmHeight );
            GetBitmapBits( HBMP_IN, 4 * BMP.bmWidth * BMP.bmHeight, RgbQ );
            SetBitmapBits( HBMP2PLOT, 4 * BMP.bmWidth * BMP.bmHeight, RgbQ );
            free( RgbQ );
            return TRUE;
        }
        return FALSE;
    }
    //*/
    /// ///////////////////////////////////////////////////////
    /// ///////////////////////////////////////////////////////
    BOOL SetInput( DIM1* DIN ){
        if( DIN!=NULL ){
            D1_IN = DIN;
        }
        return (DIN!=NULL);
    }
    /// ///////////////////////////////////////////////////////
    /// ///////////////////////////////////////////////////////
    BOOL MakeBuff(){
        FreeBuff();
        DBuff = (double*)malloc( NBytesBuff + 1024*1024 );
        ZeroMemory( DBuff, NBytesBuff + 1023 );
        return (DBuff!=NULL);
    }
    /// ///////////////////////////////////////////////////////
    /// ///////////////////////////////////////////////////////
    BOOL FreeBuff(){
        if( DBuff!=NULL ){
            free(DBuff);
            DBuff = NULL;
        }
        return (DBuff==NULL);
    }
    /// ///////////////////////////////////////////////////////
    /// ///////////////////////////////////////////////////////
    BOOL WriteDatas(){
        if( DBuff!=NULL ){
            DWORD BWRITTEN = 0;
            SetFilePointer( HFileMod, 0, NULL, FILE_BEGIN );
            WriteFile( HFileMod, DBuff, NBytesBuff, &BWRITTEN, NULL );
            return (BWRITTEN==NBytesBuff);
        }
        return FALSE;
    }
    /// ///////////////////////////////////////////////////////
    /// ///////////////////////////////////////////////////////
    BOOL WriteDatas( double* D_IN ){ return WriteDatas( D_IN, NBytesBuff ); }
    BOOL WriteDatas( double* D_IN, int NBytes2Sav ){
        if( D_IN!=NULL ){
            DWORD BWRITTEN = 0;
            SetFilePointer( HFileMod, 0, NULL, FILE_BEGIN );
            WriteFile( HFileMod, D_IN, NBytes2Sav, &BWRITTEN, NULL );
            return ((int)BWRITTEN==NBytes2Sav);
        }
        return FALSE;
    }
    /// ///////////////////////////////////////////////////////
    /// ///////////////////////////////////////////////////////
    BOOL ReadDatas(){
        MakeBuff();
        DWORD BREADED = 0;
        SetFilePointer( HFileMod, 0, NULL, FILE_BEGIN );
        ReadFile( HFileMod, DBuff, NBytesBuff, &BREADED, NULL );
        return (BREADED==NBytesBuff);
    }
    /// ///////////////////////////////////////////////////////
    /// ///////////////////////////////////////////////////////   NBytesBuff
    BOOL ReadDatas( double* D_OUT ){ return ReadDatas( D_OUT, NBytesBuff ); }
    BOOL ReadDatas( double* D_OUT, int NBytes2Read ){
        DWORD BREADED = 0;
        SetFilePointer( HFileMod, 0, NULL, FILE_BEGIN );
        ReadFile( HFileMod, D_OUT, NBytes2Read, &BREADED, NULL );
        return ((int)BREADED==NBytes2Read);
    }
    /// ///////////////////////////////////////////////////////
    /// ///////////////////////////////////////////////////////
    BOOL ClosePath(){
        if( HFileMod!=NULL ){
            CloseHandle(HFileMod);
            Sleep( 1 );
            DeleteFileA( CHAR_PATH );
            Sleep( 1 );
        }
        HFileMod = NULL;
        return TRUE;
    }
    /// ///////////////////////////////////////////////////////
    /// ///////////////////////////////////////////////////////
    /// ///////////////////////////////////////////////////////
    BOOL ClosePathOnly(){
        CloseHandle( HFileMod );
        HFileMod = NULL;
        return TRUE;
    }
    /// ///////////////////////////////////////////////////////
    /// ///////////////////////////////////////////////////////
    /// ///////////////////////////////////////////////////////
    BOOL OpenPathOnly(){
        ClosePathOnly();
        HFileMod = CreateFile( CHAR_PATH, GENERIC_WRITE|GENERIC_READ, 0, NULL, OPEN_EXISTING,
                               FILE_ATTRIBUTE_NORMAL | FILE_ATTRIBUTE_HIDDEN, NULL );
        return TRUE;
    }
    /// ///////////////////////////////////////////////////////
    /// ///////////////////////////////////////////////////////
    /// ///////////////////////////////////////////////////////
    BOOL SetPath( LPCSTR LPATH ){
        ClosePath();
        Sleep( 1 );
        sprintf( CHAR_PATH, "%szz%i.module", LPATH, Ind_File );
        HFileMod = CreateFile( CHAR_PATH, GENERIC_WRITE|GENERIC_READ, 0, NULL, CREATE_ALWAYS,
                               FILE_ATTRIBUTE_NORMAL | FILE_ATTRIBUTE_HIDDEN, NULL );
        /*/
        DWORD LE = GetLastError();
        if( LE!=183 && LE!=0 ){
            char T2T[1024];
            sprintf( T2T, "%i, %i\n\n\n%i\n\n\n", (int)0, (int)NBytesBuff, (int)LE );
            MessageBoxA( GetActiveWindow(), T2T, "", 0 );
        }
        //*/
        return TRUE;
    }
    /// ///////////////////////////////////////////////////////
    BOOL FreeAll(){
        if( SPTR2PLAY!=NULL ){
            StopPlay();
        }
        return TRUE;
    }
    /// ///////////////////////////////////////////////////////
    /// ///////////////////////////////////////////////////////
    /// ///////////////////////////////////////////////////////
    BOOL StopPlay(){
        Sleep( 10 );
        if( HWaveOut!=NULL ){
            waveOutRestart( HWaveOut );
            waveOutReset( HWaveOut );
            while( IsPlaying() ){
                Sleep( 1 );
            }
            Sleep( 10 );
            /*/
            waveOutUnprepareHeader( HWaveOut, &WHeader, sizeof(WAVEHDR) );
            waveOutClose( HWaveOut );
            Sleep( 10 );
            HWaveOut = NULL;
            Sleep( 1 );
            //*/
        }
        return TRUE;
    }
    /// ////////////////////////////////////////////////////////
    BOOL IsPlaying(){
        if( HWaveOut==NULL ){
            return FALSE;
        }else{
            return ( OnPlay );
        }
        return FALSE;
    }
    /// ////////////////////////////////////////////////////////
    /// ////////////////////////////////////////////////////////
    /// ////////////////////////////////////////////////////////
    /// ////////////////////////////////////////////////////////
    BOOL FillPlays( PLOTEOS* PltrPtr ){
        ///
        ///
        MakeBuff();
        ///
        /// DBuff
        int NDivsS = 0;
        double PPER = 1;
        for( int x=0; x<MaxProcs; x++ ){
            if( PLAYSELECTED[x] && (D1_INIT[x].ProcType==Windowing_Signal || D1_INIT[x].ProcType==Filt_Signal || x==0) ){
                NDivsS++;
                PPER = PLAY_PERCENT[x];
                D1_INIT[x].ReadDatas();
                ///
                for( int i=0; i<(int)NPosBuff; i++ ){
                    DBuff[i] = DBuff[i] + PPER * D1_INIT[x].DBuff[i];
                }
                ///
                D1_INIT[x].FreeBuff();
            }
        }
        if( NDivsS==0 ){
            NDivsS = 1;
        }
        for( int i=0; i<(int)NPosBuff; i++ ){
            // DBuff[i] = DBuff[i]/NDivsS;
        }
        for( int i=0; i<(int)SLOAD.Fs/80; i++ ){
            DBuff[i] = 0;
        }
        WriteDatas();
        /// PltrPtr->PLOTDOUBLE_BIPOL( DBuff, NPosBuff, 0, TRUE );SetPicPlot( PltrPtr->GetBitmap() );Sleep( 3000 );
        FreeBuff();
        return TRUE;
    }
    /// ////////////////////////////////////////////////////////
    /// ////////////////////////////////////////////////////////
    /// ////////////////////////////////////////////////////////
    /// ////////////////////////////////////////////////////////
    /// ////////////////////////////////////////////////////////
    BOOL PlaySignal(){
        if( OnPlay ){
            return FALSE;
        }
        //StopPlay();
        OnPlay = TRUE;
        ///
        ///
        ///
        SPTR2PLAY = (short*)malloc( NPosBuff * sizeof(short) + 1024 );
        ZeroMemory( SPTR2PLAY, NPosBuff * sizeof(short) + 1023 );
        //
        //
        double* DBLPTR = (double*)malloc( NBytesBuff + 1024 );
        ReadDatas( DBLPTR );
        ///
        /// char T2T[32];sprintf( T2T, "%i  --  %i", (int)NPosBuff, (int)NBytesBuff );MessageBoxA( NULL, T2T, "", 0 );
        ///
        /*
        double DVMax = -1e10;
        for( int i=0; i<(int)NPosBuff; i++ ){
            DVMax = fmax( DVMax, fabs(DBLPTR[i]) );
        }
        if( DVMax<1e-10 ){
            DVMax = 1;
        }
        //*/
        for( int i=0; i<(int)NPosBuff; i++ ){
            SPTR2PLAY[i] = (short)ParseShort( round( 32767 * DBLPTR[i] ) );
        }
        free( DBLPTR );
        ///
        ///
        memset(&WHeader , 0, sizeof( WAVEHDR ));
        WHeader.lpData = (LPSTR)&SPTR2PLAY[0];
        WHeader.dwBufferLength = (NPosBuff * sizeof(short));
        WHeader.dwBytesRecorded=0;
        WHeader.dwUser = 0L;
        WHeader.dwFlags = 0L;
        WHeader.dwLoops = 0L;
        ///
        memset(&WaveFormat , 0, sizeof( WAVEFORMAT ));
        WaveFormat.wFormatTag = WAVE_FORMAT_PCM;
        WaveFormat.nChannels = 1;
        WaveFormat.nSamplesPerSec = SLOAD.Fs;
        WaveFormat.wBitsPerSample = 16;
        WaveFormat.nBlockAlign = WaveFormat.nChannels * (WaveFormat.wBitsPerSample/8);
        WaveFormat.nAvgBytesPerSec = WaveFormat.nSamplesPerSec * WaveFormat.nBlockAlign;
        WaveFormat.cbSize = 0;
        waveOutOpen( &HWaveOut , WAVE_MAPPER , &WaveFormat , (DWORD)NULL , 0 , CALLBACK_NULL );
        waveOutPrepareHeader( HWaveOut , &WHeader , sizeof(WAVEHDR) );
        waveOutWrite( HWaveOut, &WHeader , sizeof(WAVEHDR) );
        Sleep( 1 );
        while( waveOutUnprepareHeader( HWaveOut, &WHeader, sizeof(WAVEHDR) ) == WAVERR_STILLPLAYING ){
            Sleep(2);
        }
        waveOutRestart( HWaveOut );
        waveOutReset( HWaveOut );
        waveOutUnprepareHeader( HWaveOut, &WHeader, sizeof(WAVEHDR) );
        waveOutClose( HWaveOut );
        HWaveOut = NULL;
        free( SPTR2PLAY );
        SPTR2PLAY = NULL;
        FreeAll();
        /// WHDR_DONE
        /// while( WHeader.dwFlags==0  ){
        /// MessageBoxA( GetActiveWindow(), "", "", 0 );
        ///
        OnPlay = FALSE;
        return TRUE;
    }
    /// ///////////////////////////////////////////////////////
    /// ///////////////////////////////////////////////////////
    /// ///////////////////////////////////////////////////////
    /// ///////////////////////////////////////////////////////
    /// ///////////////////////////////////////////////////////
    /// ///////////////////////////////////////////////////////
    BOOL LoadWAV( HWND hwnd ){ return LoadWAV( hwnd, TRUE ); }
    BOOL LoadWAV( HWND hwnd, BOOL ShowMsg ){
        DWORD FileSizeA = 0;
        DWORD Readed    = 0;
        char* ChBuffIn = NULL;
        HANDLE HFile = CreateFile( SLOAD.FNameT, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE,
                                   NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
        if( HFile==NULL ){
            if( ShowMsg ){
                MessageBoxA( hwnd, "El archivo no puede ser leido.", " Error de lectura ", 16 );
            }
            return FALSE;
        }
        FileSizeA = GetFileSize( HFile, NULL );
        /*
        if( FileSizeA>(DWORD)MaxFileSize ){
            if( ShowMsg ){
                MessageBoxA( hwnd, "El tamaño de archivo supera el límite permitido.", " Error de lectura ", 16 );
            }
            return FALSE;
        }
        //*/
        ///
        ///
        ///sprintf( ChBuffIn, "NPSB %i\nNCHA %i\nNBY %i\n", (int)round( NPSB/(1024.0*1024) ), NCHA, NBY );
        ///MessageBoxA( GetActiveWindow(), ChBuffIn, "", 0 );
        ///
        ///
        ChBuffIn = (char*)malloc( FileSizeA + 1024 );
        if( !ReadFile(HFile, ChBuffIn, FileSizeA, &Readed, NULL ) ){
            if( ShowMsg ){
                MessageBoxA( hwnd, "El archivo no puede ser leido.", " Error de lectura ", 16 );
            }
            CloseHandle( HFile );
            free( ChBuffIn );
            return FALSE;
        }
        if( Readed!=FileSizeA ){
            free( ChBuffIn );
            if( ShowMsg ){
                MessageBoxA( hwnd, "El archivo no puede ser leido.", " Error de lectura ", 16 );
            }
            CloseHandle( HFile );
            return FALSE;
        }
        CloseHandle( HFile );
        if( ChBuffIn[0]!='R' || ChBuffIn[1]!='I' || ChBuffIn[2]!='F' || ChBuffIn[3]!='F' ){
            free( ChBuffIn );
            if( ShowMsg ){
                MessageBoxA( hwnd, "Error en el formato WAV.", " Error de lectura ", 16 );
            }
            return FALSE;
        }
        ///
        memcpy( ChBuffIn, &ChBuffIn[44], FileSizeA - 44 );
        ///
        ///sprintf( ChBuffIn, "BitsPerSample = %i\nNChannels = %i", (int)SLOAD.BitsPerSample, (int)SLOAD.NChannels );MessageBoxA( hwnd, ChBuffIn, "  ", 0 );
        if( SLOAD.BitsPerSampleT==8 ){
            if( SLOAD.NChannelsT==1 ){
                NPosBuff = FileSizeA - 44;
                SLOAD.ChannSelT = 0;
            }else{
                NPosBuff = (FileSizeA - 44)/2;
            }
        }else{
            if( SLOAD.NChannelsT==1 ){
                NPosBuff = (FileSizeA - 44)/2;
                SLOAD.ChannSelT = 0;
            }else{
                NPosBuff = (FileSizeA - 44)/4;
            }
        }
        NBytesBuff = NPosBuff * sizeof(double);
        MakeBuff();
        if( SLOAD.BitsPerSample==8 ){
            unsigned char* UCBuff = (unsigned char*)&ChBuffIn[0];
            if( SLOAD.NChannels==1 ){
                for( int i=0; i<(int)NPosBuff; i++ ){
                    DBuff[i] = ((double)UCBuff[i])/255;
                    DBuff[i] = 2*( DBuff[i] - 0.5 );
                }
            }else{
                if( SLOAD.ChannSel==0 ){
                    for( int i=0; i<(int)(NPosBuff); i++ ){
                        DBuff[i] = ((double)UCBuff[2*i])/255;
                        DBuff[i] = 2*( DBuff[i] - 0.5 );
                    }
                }else{
                    for( int i=0; i<(int)(NPosBuff); i++ ){
                        DBuff[i] = ((double)UCBuff[2*i+1])/255;
                        DBuff[i] = 2*( DBuff[i] - 0.5 );
                    }
                }
            }
        }else{
            short* SHBuff = (short*)&ChBuffIn[0];
            if( SLOAD.NChannels==1 ){
                for( int i=0; i<(int)NPosBuff; i++ ){
                    DBuff[i] = ((double)SHBuff[i])/32768;
                }
            }else{
                if( SLOAD.ChannSel==0 ){
                    for( int i=0; i<(int)(NPosBuff); i++ ){
                        DBuff[i] = ((double)SHBuff[2*i])/32768;
                    }
                }else{
                    for( int i=0; i<(int)(NPosBuff); i++ ){
                        DBuff[i] = ((double)SHBuff[2*i+1])/32768;
                    }
                }
            }
        }
        ///
        MaxValFFT = 1;
        free( ChBuffIn );
        ///
        if( !WriteDatas() ){
            FreeBuff();
            return FALSE;
        }
        if( SLOAD.NChannelsT==1 ){
            SLOAD.ChannSelT = 0;
            SLOAD.ChannSel  = 0;
        }
        FreeBuff();
        ///
        return TRUE;
    }
    /// ///////////////////////////////////////////////////////
    /// ///////////////////////////////////////////////////////
    /// ///////////////////////////////////////////////////////
    /// ///////////////////////////////////////////////////////
    LPCSTR GetInfo( int NMod, BOOL ParseLoaded ){
        static char TT2T[1024] = {0};
        static char TT2T_T[1024] = {0};
        switch( ProcType ){
            ///
            case Graph_Signal:
                if( !IsLoaded && ParseLoaded ){
                    sprintf( TT2T, "Módulo %i, No procesado, tipo de procesamiento del módulo: Graficar.", NMod );
                }else{
                    sprintf( TT2T, "Módulo %i, tipo de procesamiento del módulo: Graficar", NMod );
                }
                break;
                ///
                ///
                ///
            case Filt_Signal:
                if( !IsLoaded && ParseLoaded ){
                    sprintf( TT2T,
                            "Módulo %i, No procesado, tipo de procesamiento del módulo: Filtrar.\n"
                            "Filtro %s %s",
                            NMod, FDS.GetNameFilter(),
                            FDS.GetFilterInfo(TT2T_T)
                             );
                }else{
                    sprintf( TT2T,
                            "Módulo %i, tipo de procesamiento del módulo: Filtrar.\n"
                            "Filtro %s %s",
                            NMod, FDS.GetNameFilter(),
                            FDS.GetFilterInfo(TT2T_T)
                             );
                }
                break;
                ///
                ///
                ///
            case Windowing_Signal:
                if( !IsLoaded && ParseLoaded ){
                    sprintf( TT2T,
                            "Módulo %i, No procesado, tipo de procesamiento del módulo: Enventanado.\n"
                            "Ventana %s",
                            NMod, FDS.WINDWN.GetWinInfo(TT2T_T)
                             );
                }else{
                    sprintf( TT2T,
                            "Módulo %i, tipo de procesamiento del módulo: Enventanado.\n"
                            "Ventana %s",
                            NMod, FDS.WINDWN.GetWinInfo(TT2T_T)
                             );
                }
                break;
                ///
                ///
                ///
            case FFT_Signal:
                if( !IsLoaded && ParseLoaded ){
                    sprintf( TT2T,
                            "Módulo %i, No procesado, tipo de procesamiento del módulo:\n"
                            "FFT( Transformada rápida de FOURIER )", NMod
                             );
                }else{
                    sprintf( TT2T,
                            "Módulo %i, tipo de procesamiento del módulo:\n"
                            "FFT( Transformada rápida de FOURIER )", NMod
                             );
                }
                break;
                ///
                ///
                ///
            case SPECTOGRAM_Signal:
                if( !IsLoaded && ParseLoaded ){
                    sprintf( TT2T,
                            "Módulo %i, No procesado, tipo de procesamiento del módulo:\n"
                            "ESPECTOGRAMA", NMod
                             );
                }else{
                    sprintf( TT2T,
                            "Módulo %i, tipo de procesamiento del módulo:\n"
                            "ESPECTOGRAMA", NMod
                             );
                }
                break;
                ///
                ///
                ///
            case WIGNER_Signal:
                if( !IsLoaded && ParseLoaded ){
                    sprintf( TT2T,
                            "Módulo %i, No procesado, tipo de procesamiento del módulo:\n"
                            "WINGER-VILLE", NMod
                             );
                }else{
                    sprintf( TT2T,
                            "Módulo %i, tipo de procesamiento del módulo:\n"
                            "WINGER-VILLE", NMod
                             );
                }
                break;
                ///
                ///
                ///
            case CHOI_WIL_Signal:
                if( !IsLoaded && ParseLoaded ){
                    sprintf( TT2T,
                            "Módulo %i, No procesado, tipo de procesamiento del módulo:\n"
                            "CHOI-WILLIAM", NMod
                             );
                }else{
                    sprintf( TT2T,
                            "Módulo %i, tipo de procesamiento del módulo:\n"
                            "CHOI-WILLIAM", NMod
                             );
                }
                break;
                ///
                ///
                ///
            case PLAY_Signal:{
                TT2T_T[0] = 0;
                for( int i=0; i<MaxProcs; i++ ){
                    if( PLAYSELECTED[i] &&
                            (D1_INIT[i].ProcType==Windowing_Signal ||
                             D1_INIT[i].ProcType==Filt_Signal || i==0 )
                                                                                ){
                        sprintf( TT2T, "%i, ", i );
                        strcat( TT2T_T, TT2T );
                    }
                }
                if( !IsLoaded && ParseLoaded ){
                    sprintf( TT2T,
                            "Módulo %i, No procesado, tipo de procesamiento del módulo:\n"
                            "Reproducir auido.\n"
                            "Módulos (%s)", NMod, TT2T_T
                             );
                }else{
                    sprintf( TT2T,
                            "Módulo %i, tipo de procesamiento del módulo:\n"
                            "Reproducir auido.\n"
                            "Módulos (%s)", NMod, TT2T_T
                             );
                }
            }
                break;
                ///
                ///
                ///
            default:
                if( !IsLoaded && ParseLoaded ){
                    sprintf( TT2T,
                            "Módulo %i, No procesado, tipo de procesamiento del módulo:\n"
                            "Lectura de señal", NMod
                             );
                }else{
                    sprintf( TT2T,
                            "Módulo %i, tipo de procesamiento del módulo:\n"
                            "Lectura de señal", NMod
                             );
                }
                break;
                ///
                ///
                ///
        }

        return TT2T;
    }
    /// ///////////////////////////////////////////////////////
#define NO_PROC_Signal          8
    /// ///////////////////////////////////////////////////////
    /// ///////////////////////////////////////////////////////
    /// ///////////////////////////////////////////////////////
    /// ///////////////////////////////////////////////////////
    /// ///////////////////////////////////////////////////////
    /// ///////////////////////////////////////////////////////
    /// ///////////////////////////////////////////////////////
    BOOL PreloadWAV( HWND hwnd ){ return PreloadWAV( hwnd, TRUE ); }
    BOOL PreloadWAV( HWND hwnd, BOOL ShowMsg ){
        DWORD FileSizeA = 0;
        DWORD Readed    = 0;
        DWORD ReaT    = 0;
        char ChBuffIn[128] = {0};
        HANDLE HFile = CreateFile( SLOAD.FNameT, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE,
                                   NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
        if( HFile==NULL ){
            if( ShowMsg ){
                MessageBoxA( hwnd, "El archivo no puede ser leido.", " Error de lectura ", 16 );
            }
            return FALSE;
        }
        FileSizeA = GetFileSize( HFile, NULL );
        ReaT = 128;
        if( ReaT>FileSizeA ){
            ReaT = FileSizeA;
        }
        if( !ReadFile(HFile, ChBuffIn, ReaT, &Readed, NULL ) ){
            if( ShowMsg ){
                MessageBoxA( hwnd, "El archivo no puede ser leido.", " Error de lectura ", 16 );
            }
            CloseHandle( HFile );
            return FALSE;
        }
        if( ReaT!=Readed ){
            if( ShowMsg ){
                MessageBoxA( hwnd, "El archivo no puede ser leido.", " Error de lectura ", 16 );
            }
            CloseHandle( HFile );
            return FALSE;
        }
        CloseHandle( HFile );
        if( ChBuffIn[0]!='R' || ChBuffIn[1]!='I' || ChBuffIn[2]!='F' || ChBuffIn[3]!='F' ){
            if( ShowMsg ){
                MessageBoxA( hwnd, "Error en el formato WAV.", " Error de lectura ", 16 );
            }
            return FALSE;
        }
        ///
        DWORD* DWPT = NULL;
        ///
        DWPT = ((DWORD*)&ChBuffIn[22]);
        int NCHA = 255&DWPT[0];
        DWPT = ((DWORD*)&ChBuffIn[34]);
        int NBY = 255&DWPT[0];
        /// if( FileSizeA>(DWORD)MaxFileSize && DWPT[0]==1 ){
        int NPSB = (int)round( FileSizeA/( double(NCHA) * double(NBY/8) ) );
        if( NPSB>MaxFileSize ){
            if( ShowMsg ){
                MessageBoxA( hwnd, "El tamaño de archivo supera el límite permitido.", " Error de lectura ", 16 );
            }
            return FALSE;
        }
        /*
        if( FileSizeA>(DWORD)MaxFileSize ){
            if( ShowMsg ){
                MessageBoxA( hwnd, "El tamaño de archivo supera el límite permitido.", " Error de lectura ", 16 );
            }
            return FALSE;
        }
        ///*/
        ///
        DWPT = ((DWORD*)&ChBuffIn[24]);
        SLOAD.FsT = DWPT[0];
        DWPT = ((DWORD*)&ChBuffIn[22]);
        SLOAD.NChannelsT = 255&DWPT[0];
        DWPT = ((DWORD*)&ChBuffIn[34]);
        SLOAD.BitsPerSampleT = 255&DWPT[0];
        ///
        ///
        ///
        ///
        if( SLOAD.BitsPerSampleT>16 ){
            if( ShowMsg ){
                MessageBoxA( hwnd, "El BitsPerSample del archivo es mayor a 16.", " Error de lectura ", 16 );
            }
            return FALSE;
        }
        ///
        ///
        ///
        ///
        ///
        return TRUE;
    }
    /// ///////////////////////////////////////////////////////
    /// ///////////////////////////////////////////////////////
    BOOL LoadMAT( HWND hwnd ){
        MakeBuff();
        BOOL B2Ret = DSP_MNG.LoadVar( SLOAD.FName, SLOAD.VarName, DBuff );
        if( !B2Ret ){
            FreeBuff();
            return FALSE;
        }
        for( int i=0; i<(int)NPosBuff; i++ ){
            MaxValFFT = fmax( MaxValFFT, fabs(DBuff[i]) );
        }
        WriteDatas();
        FreeBuff();
        return B2Ret;
    }
    /// ///////////////////////////////////////////////////////
    /// ///////////////////////////////////////////////////////
    BOOL PreloadMAT( HWND hwnd ){
        const int MaxPtrPos = 128;
        int NVarsMax = MaxPtrPos;
        VarsInfo* VINFOUT = (VarsInfo*)malloc( MaxPtrPos*sizeof(VarsInfo)+128 );
        ZeroMemory( VINFOUT, MaxPtrPos*sizeof(VarsInfo)+127 );
        DSP_MNG.GetVarsInfo1D( SLOAD.FNameT, VINFOUT, &NVarsMax, hwnd );
        ///
        int VSEL = DialogBoxParamA( GetModuleHandle(NULL), MAKEINTRESOURCE(DLG_LOAD_VAR_1D), hwnd, (DLGPROC)Dlg1D_PreloadMAT,
                                     (LPARAM)VINFOUT ); /// LenOfV
        ///
        if( VSEL>=0 ){
            strcpy( SLOAD.VarNameT, VINFOUT[VSEL].VName );
            SLOAD.LOfVarT = VINFOUT[VSEL].LenOfV;
        }
        ///
        free( VINFOUT );
        /// MessageBoxA( hwnd, "Fin", " ", 0 );
        return (VSEL>=0);
    }
    /// ///////////////////////////////////////////////////////
    /// ///////////////////////////////////////////////////////
    BOOL PreLoadInfo(){
        strcpy( SLOAD.FName, SLOAD.FNameT );
        SLOAD.Fs = SLOAD.FsT;
        SLOAD.NChannels = SLOAD.NChannelsT;
        SLOAD.SType = SLOAD.STypeT;
        SLOAD.TimeSignal = SLOAD.TimeSignalT;
        SLOAD.LOfVar = SLOAD.LOfVarT;
        strcpy( SLOAD.VarName, SLOAD.VarNameT );
        SLOAD.f = SLOAD.fT;
        SLOAD.BitsPerSample = SLOAD.BitsPerSampleT;
        SLOAD.ChannSel = SLOAD.ChannSelT;
        NPosBuff = SLOAD.LOfVar;
        NBytesBuff = NPosBuff * sizeof(double);
        return TRUE;
    }
    /// ///////////////////////////////////////////////////////
    /// ///////////////////////////////////////////////////////
    BOOL UnLoadInfo(){
        strcpy( SLOAD.FNameT, SLOAD.FName );
        SLOAD.FsT = SLOAD.Fs;
        SLOAD.NChannelsT = SLOAD.NChannels;
        SLOAD.STypeT = SLOAD.SType;
        SLOAD.TimeSignalT = SLOAD.TimeSignal;
        SLOAD.LOfVarT = SLOAD.LOfVar;
        strcpy( SLOAD.VarNameT, SLOAD.VarName );
        SLOAD.fT = SLOAD.f;
        SLOAD.BitsPerSampleT = SLOAD.BitsPerSample;
        SLOAD.ChannSelT = SLOAD.ChannSel;
        return TRUE;
    }
    /// ///////////////////////////////////////////////////////
    /// ///////////////////////////////////////////////////////
    BOOL LoadSignal( HWND hwnd ){ return LoadSignal( hwnd, TRUE ); }
    BOOL LoadSignal( HWND hwnd, BOOL ShowMsg ){
        switch( SLOAD.SType ){
            case 0:
                if( !LoadWAV( hwnd, ShowMsg ) ){
                    IsLoaded = FALSE;
                    return FALSE;
                }
                break;
            case 1:
                if( !LoadMAT( hwnd ) ){
                    IsLoaded = FALSE;
                    return FALSE;
                }
                break;
            case 2:{
                SLOAD.LOfVar = MaxFileSize/8;///SLOAD.Fs;
                SLOAD.LOfVar = 10*SLOAD.Fs;
                NPosBuff = SLOAD.LOfVar;
                NBytesBuff = NPosBuff * sizeof(double);
                MakeBuff();
                const double PI_2_f = ( 2 * 3.1415926535897932384626433832795 * SLOAD.f )/((double)SLOAD.Fs);
                MaxValFFT = -1e100;
                for( int i=0; i<(int)NPosBuff; i++ ){
                    DBuff[i] = sin( PI_2_f * i );
                    MaxValFFT = fmax( MaxValFFT, fabs(DBuff[i]) );
                }
                WriteDatas();
                FreeBuff();
            }
                break;
            default:
                IsLoaded = FALSE;
                return FALSE;
                break;
        }
        IsLoaded = TRUE;
        return TRUE;
    }
    /// ///////////////////////////////////////////////////////
    /// ///////////////////////////////////////////////////////
    /// ///////////////////////////////////////////////////////
    DIM1* GetInput(){
        DIM1*   D1_INPUT = D1_IN;
        if( FDS.GetBInput()!=0 ){
            D1_INPUT = D1_INIT;
        }
        return D1_INPUT;
    }
    /// ///////////////////////////////////////////////////////
    /// ///////////////////////////////////////////////////////
    BOOL SetProc( HWND hwnd, PLOTEOS* PltrPtr ){
        return SetProc( hwnd, PltrPtr, FALSE );
    }
    BOOL SetProc( HWND hwnd, PLOTEOS* PltrPtr, BOOL SaveOthers ){
        IsLoaded = TRUE;
        fftprepare fftp_in;
        ///
        if( ProcType==NO_PROC_Signal ){
            return FALSE;
        }
        DIM1*   D1_INPUT = GetInput();
        while( D1_INPUT->ProcType==NO_PROC_Signal ||
                    D1_INPUT->ProcType==Graph_Signal ||
                    D1_INPUT->ProcType==FFT_Signal ||
                    D1_INPUT->ProcType==SPECTOGRAM_Signal ||
                    D1_INPUT->ProcType==WIGNER_Signal ||
                    D1_INPUT->ProcType==CHOI_WIL_Signal ||
                    D1_INPUT->ProcType==PLAY_Signal
                                                    ){
            ///
            D1_INPUT = D1_INPUT->GetInput();
            if( D1_INPUT==NULL ){
                break;
            }
        }
        //
        if( D1_INPUT==NULL ){
            return FALSE;
        }
        ///
        StopPlay();
        SLOAD = D1_INPUT->SLOAD;
        NPosBuff = D1_INPUT->NPosBuff;
        NBytesBuff = D1_INPUT->NBytesBuff;
        ///
        //BOOL BTMP = FDS.IsIRR;
        //FDS.IsIRR = FALSE;
        ///FDS.SetFs( D1_INPUT->FDS.GetFs() );
        //FDS.IsIRR = TRUE;
        ///FDS.SetFs( D1_INPUT->FDS.GetFs() );
        //FDS.IsIRR = BTMP;
        ///
        switch( ProcType ){
            /// GRAFICAR:
            case Graph_Signal:{
                /// PreLoadInfo();
                /*
                D1_INPUT->ReadDatas();
                ///
                //WriteDatas( D1_INPUT->DBuff );
                ///
                ///
                MaxValFFT = -1e100;
                for( int i2=0; i2<(int)NPosBuff; i2++ ){
                    MaxValFFT = fmax( MaxValFFT, D1_INPUT->DBuff[i2] );
                }
                if( MaxValFFT<1 ){
                    MaxValFFT = 1;
                }
                ///
                ///
                PltrPtr->PLOTDOUBLE_BIPOL( D1_INPUT->DBuff, NPosBuff, 0, TRUE );
                D1_INPUT->FreeBuff();
                SetPicPlot( PltrPtr->GetBitmap() );
                //*/
                ///
                ///
                MaxValFFT = D1_INPUT->MaxValFFT;
                BITMAP BMP = {0};
                GetObject( D1_INPUT->HBMP2PLOT, sizeof(BITMAP), &BMP );
                if( HBMP2PLOT!=NULL ){
                    DeleteObject( HBMP2PLOT );
                }
                HBMP2PLOT = NewBitmap( BMP.bmWidth, BMP.bmHeight );
                RGBQUAD* RgbQ = (RGBQUAD*)malloc( 4 * BMP.bmWidth * BMP.bmHeight + 1024 );
                ZeroMemory( RgbQ, 4 * BMP.bmWidth * BMP.bmHeight );
                GetBitmapBits( D1_INPUT->HBMP2PLOT, 4 * BMP.bmWidth * BMP.bmHeight, RgbQ );
                SetBitmapBits( HBMP2PLOT, 4 * BMP.bmWidth * BMP.bmHeight, RgbQ );
                free( RgbQ );
                ///
                ///
            }
                break;
            ///
            ///
            ///
            ///
            case Filt_Signal:{
                /// DSP_MNG.tic( 0 );
                int MBFLEN = NPosBuff;///1<<nextpow2(NPosBuff);
                if( MBFLEN<192000 ){
                    MBFLEN = 192000;
                }
                /*
                if( !FDS.IsIRR ){
                    if( MBFLEN>8388608 ){
                        while( (MBFLEN%16)!=0 ){
                            MBFLEN = MBFLEN - 1;
                        }
                    }
                }
                //*/
                DSP_MNG.SetPlanFFT( ( 1<<nextpow2(MBFLEN) ) );
                fftp_in.Prepare( ( 1<<nextpow2(MBFLEN) ) );
                D1_INPUT->ReadDatas( fftp_in.InputV );
                /*
                if( FDS.IsIRR ){
                    FDS.IIRSet();
                }else{
                    FDS.Fir1Set();
                }
                //*/
                FDS.Apply_Filter( MBFLEN, &fftp_in );
                WriteDatas( fftp_in.InputV );
                ///
                PltrPtr->PLOTDOUBLE_BIPOL( fftp_in.InputV, NPosBuff, 0, TRUE );
                SetPicPlot( PltrPtr->GetBitmap() );
                ///
                MaxValFFT = -1;
                for( int i2=0; i2<(int)NPosBuff; i2++ ){
                    MaxValFFT = fmax( MaxValFFT, fabs(fftp_in.InputV[i2]) );
                }
                if( MaxValFFT<1 && MaxValFFT>0.099 ){
                    ///MaxValFFT = 1;
                }
                ///
                fftp_in.FreeAll();
                /// DSP_MNG.SetWintocText( "Filt_Signal ", 0 );
                ///
                ///
            }
                break;
            ///
            ///
            ///
            ///
            case Windowing_Signal:{
                D1_INPUT->ReadDatas();
                FDS.SetWindowing( NPosBuff, D1_INPUT->DBuff );
                WriteDatas( D1_INPUT->DBuff );
                ///
                ///
                MaxValFFT = -1e100;
                for( int i2=0; i2<(int)NPosBuff; i2++ ){
                    MaxValFFT = fmax( MaxValFFT, D1_INPUT->DBuff[i2] );
                }
                if( MaxValFFT<1 ){
                    MaxValFFT = 1;
                }
                ///
                ///
                PltrPtr->PLOTDOUBLE_BIPOL( D1_INPUT->DBuff, NPosBuff, 0, TRUE );
                SetPicPlot( PltrPtr->GetBitmap() );
                ///
                D1_INPUT->FreeBuff();
            }
                break;
            ///
            ///
            ///
            ///
            ///
            case FFT_Signal:{
                int PFs = 10 * FDS.GetFs();
                int NPos2Save = PFs;
                int NDatasB = PFs;///                 NPosBuff
                ///
                ///
                ///
                ///
                /// ///////////////////////////////////////////////////////////////////////////////
                if( PFs>(int)NPosBuff || TRUE ){
                    PFs = NPosBuff;
                    NPos2Save = NPosBuff;
                    NDatasB = NPosBuff;
                    fftp_in.Prepare( NPosBuff );
                    D1_INPUT->ReadDatas( fftp_in.InputV );
                }else{
                    fftp_in.Prepare( NPosBuff );
                    D1_INPUT->ReadDatas( fftp_in.InputV );
                    int NFs = (int)ceil( ((double)NPosBuff)/((double)PFs) );
                    double* BFSPTR = (double*)malloc( PFs * sizeof(double) + 1024 );
                    ZeroMemory( BFSPTR, PFs * sizeof(double) );
                    double* DblPtr = NULL;
                    for( int i=0; i<NFs; i = i + PFs ){
                        DblPtr = &fftp_in.InputV[i*PFs];
                        for( int x = 0; x<PFs; x++ ){
                            BFSPTR[x] = BFSPTR[x] + DblPtr[x];
                        }
                    }
                    memcpy( fftp_in.InputV, BFSPTR, PFs * sizeof(double) );
                    free( BFSPTR );
                }
                /// ///////////////////////////////////////////////////////////////////////////////
                ///
                NPosFFT = NPos2Save;
                ///
                ///
                ///
                DSP_MNG.FFT( NDatasB, fftp_in.InputV, fftp_in.RealV, fftp_in.ImagV, FALSE );
                ///
                ///
                double FNy_2_D = double(2)/( (double)(NDatasB) );
                ///
                MaxValFFT = -1e100;
                for( int i2=0; i2<(int)NDatasB/2; i2++ ){
                    fftp_in.RealV[i2] =
                        FNy_2_D*sqrt( fftp_in.RealV[i2] * fftp_in.RealV[i2] + fftp_in.ImagV[i2] * fftp_in.ImagV[i2] );
                    /// MaxValFFT = fmax( MaxValFFT, fftp_in.RealV[i2] );
                    if( MaxValFFT<fftp_in.RealV[i2] ){
                        MaxValFFT = fftp_in.RealV[i2];
                    }
                }
                if( MaxValFFT<1 && MaxValFFT>0.099 ){
                    MaxValFFT = 1;
                }
                ///
                ///
                ///
                ///
                ///
                PltrPtr->PLOT_DOUBLE_FFT( fftp_in.RealV, NDatasB/2, 0, TRUE );
                SetPicPlot( PltrPtr->GetBitmap() );
                ///
                WriteDatas( fftp_in.RealV, NPos2Save * sizeof(double) );
                ///
                ///
                ///
                ///
                fftp_in.FreeAll();
            }
                break;
            ///
            ///
            ///
            ///
            ///
            case SPECTOGRAM_Signal:{
                fftp_in.Prepare( NPosBuff + 2*SLOAD.Fs );
                D1_INPUT->ReadDatas( fftp_in.InputV );
                /// WriteDatas( fftp_in.InputV );
                FDS.SPecto.Spectogram( SLOAD.Fs, NPosBuff, &fftp_in );
                PltrPtr->PLOT_SPECTOGRAM( FDS.SPecto.Get_Spect_Out(), FDS.SPecto.Get_xs(), FDS.SPecto.Get_ys()/2, TRUE );
                PltrPtr->Redraw();
                SetPicPlot( PltrPtr->GetBitmap() );
                FDS.SPecto.Free();
                fftp_in.FreeAll();
                ///
            }
                break;
            ///
            ///
            ///
            ///
            ///
            case WIGNER_Signal:{
                fftp_in.Prepare( NPosBuff + 2*SLOAD.Fs  );
                D1_INPUT->ReadDatas( fftp_in.InputV );
                /// WriteDatas( fftp_in.InputV );
                FDS.SPecto.Wigner( SLOAD.Fs, NPosBuff, &fftp_in );
                PltrPtr->PLOT_SPECTOGRAM( FDS.SPecto.Get_Spect_Out(), FDS.SPecto.Get_xs(), FDS.SPecto.Get_ys()/2, TRUE );
                PltrPtr->Redraw();
                SetPicPlot( PltrPtr->GetBitmap() );
                FDS.SPecto.Free();
                fftp_in.FreeAll();
            }
                break;
            ///
            ///
            ///
            ///
            ///
            case CHOI_WIL_Signal:{
                fftp_in.Prepare( NPosBuff + 2*SLOAD.Fs  );
                D1_INPUT->ReadDatas( fftp_in.InputV );
                /// WriteDatas( fftp_in.InputV );
                FDS.SPecto.ChoiW( SLOAD.Fs, NPosBuff, &fftp_in, 1 );
                PltrPtr->PLOT_SPECTOGRAM( FDS.SPecto.Get_Spect_Out(), FDS.SPecto.Get_xs(), FDS.SPecto.Get_ys()/2, TRUE );
                /// MessageBoxA( hwnd, "", "", 0 );
                PltrPtr->Redraw();
                SetPicPlot( PltrPtr->GetBitmap() );
                FDS.SPecto.Free();
                fftp_in.FreeAll();
                /// MessageBoxA( hwnd, "", "", 0 );
                /// RGB_SPECT  HBSPECT
            }
                break;
            ///
            ///
            ///
            ///
            case PLAY_Signal:{
                break;
                D1_INPUT->ReadDatas();
                ///
                WriteDatas( D1_INPUT->DBuff );
                D1_INPUT->FreeBuff();
                //SetPicPlot( D1_INPUT->GetPicPlot() );
                ///DWORD IdTh = 0;CreateThread( NULL, 0, ThreadCall_PlaySignal, (PVOID)&IPtr, 0, &IdTh );
            }
                break;
            ///
            ///
            default:
                break;
                D1_INPUT->ReadDatas();
                WriteDatas( D1_INPUT->DBuff );
                D1_INPUT->FreeBuff();
                break;
            ///
            ///
            ///
        }
        return TRUE;
    }
    /// ///////////////////////////////////////////////////////
    /// ///////////////////////////////////////////////////////
    /// ///////////////////////////////////////////////////////
    /// ///////////////////////////////////////////////////////
    /// ///////////////////////////////////////////////////////
};
///
DWORD WINAPI ThreadCall_PlaySignal( LPVOID pv ){
    DIM1* D1PTR = (DIM1*)pv;
    D1PTR->PlaySignal();
    return 0;
}
///
