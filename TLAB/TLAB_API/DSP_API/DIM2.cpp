#define SHOW_2D                     0
#define RGB2GRAY_2DPROC             1
#define BINARY_2DPROC               2
#define CONV_3x3_2DPROC             3
#define HIST_2DPROC                 4
#define REDIM_2DPROC                5
#define ARITM_2DPROC                6
#define BW_LABEL                    7
#define NO_PROC_Signal_2D           8
#define IMERODE                     9
#define IMDILATE                   10
#define IMOPEN                     11
#define IMCLOSE                    12
///
#define IMFILL_OPT                 13
///
#define GRAY_2_JET                 14
///
#define BLUR_K                      0
#define EMBOSS                      1
#define SHARPEN                     2
#define SMOOTH                      3
#define MEANRENOVAL                 4
#define LP_MEDIA                    5
#define LP_POND                     6
#define HP_MEDIA                    7
#define HP_LAPLACE                  8
#define HP_LAPLACE_MIN              9
#define SOBEL                      10
#define PREWITT                    11
#define KIRSH                      12
#define USUARIO                    13
#define AVERAGE                    14
///
#include "Kernels_3x3.cpp"
struct DIM2{
    int                                 IPtr;
    char                                FName[1024];
    char                                FNameT[1024];
    char                                CHAR_PATH[1024];
    int                                 Ind_File;
    HANDLE                              HFileMod;
    BYTE                                ImageInP;
    int                                 ProcType;
    BOOL                                IsLoaded;
    ///
    HBITMAP                             HBMP_IN;
    int                                 WPIC_IN;
    int                                 HPIC_IN;
    ///
    HBITMAP                             HBMP_OUT;
    int                                 WPIC_OUT;
    int                                 HPIC_OUT;
    ///
    HBITMAP                             HBMP_PLOT;
    int                                 WPIC_PLOT;
    int                                 HPIC_PLOT;
    ///
    ///
    DIM2*                               Dim_In;
    DIM2*                               D2_SOURCE;
    ///
    ///
    RGBQUAD*                            RgbBuff;
    DWORD                               NPosBuff;
    DWORD                               NBytesBuff;
    BYTE                                LevelBIN;
    BOOL                                IsGray;
    BOOL                                IsBin;
    double                              Hist[256];
    double                              Hist_R[256];
    double                              Hist_G[256];
    double                              Hist_B[256];
    BYTE                                Kernel3x3_Type;
    char                                Kernel3x3_Offset[16];
    int                                 Kernel3x3H[9];
    int                                 Kernel3x3V[9];
    int                                 Kernel3x3User[9];
    ///
    int                                 WPIC_REDIM;
    int                                 HPIC_REDIM;
    BOOL                                RELASP;
    ///
    ///
    int                                 ArithType;
    int                                 SumOper;
    double                              ProdOper;
    int                                 LowValue;
    int                                 HiValue;
    BOOL                                BSetPix;
    BOOL                                InvOut;
    char                                DReserved[4];
    double                              P_Prom_Zeros;
    double                              P_Prom_Ones;
    double                              P_PromR;
    double                              P_PromG;
    double                              P_PromB;
    ///
    int                                 NOrd;
    double                              GrayValue;
    int                                 NObjBWL;
    int                                 NOrdErode;
    int                                 OptErode;
    ///
    int                                 OffsetType;
    double                              OffSProm;
    double                              OffS0;
    double                              OffS255;
    ///
    BYTE                                BFront;
    BYTE                                BW_OPTS;
    BYTE                                BOptFill;
    BYTE                                BSobel_Remove;
    ///
    DIM2(){
        IPtr = 0;
        ImageInP = 0;
        BFront   = 0;
        Ind_File = 0;
        NObjBWL  = 0;
        BW_OPTS  = 0;
        ProcType = NO_PROC_Signal_2D;
        IsLoaded = FALSE;
        HFileMod = NULL;
        RgbBuff = NULL;
        /// HBMP_IN = NULL;
        HBMP_OUT = NULL;
        HBMP_PLOT = NULL;
        Dim_In = NULL;
        D2_SOURCE = NULL;
        WPIC_OUT = 0;
        HPIC_OUT = 0;
        WPIC_PLOT = 0;
        HPIC_PLOT = 0;
        NBytesBuff = 4  * WPIC_OUT * HPIC_OUT;
        ZeroMemory( FName, 1023 );
        ZeroMemory( FNameT, 1023 );
        ZeroMemory( CHAR_PATH, 1023 );
        LevelBIN = 128;
        IsGray = FALSE;
        IsBin = FALSE;
        RELASP = TRUE;
        Kernel3x3_Type = 0;
        ZeroMemory( Kernel3x3_Offset, 16 );
        Kernel3x3_Offset[10] = -128;
        Kernel3x3_Offset[11] = -128;
        ZeroMemory( Kernel3x3H, 9*sizeof(int) );
        ZeroMemory( Kernel3x3V, 9*sizeof(int) );
        ZeroMemory( Kernel3x3User, 9*sizeof(int) );
        ZeroMemory( Hist_R, 256*sizeof(double) );
        ZeroMemory( Hist_G, 256*sizeof(double) );
        ZeroMemory( Hist_B, 256*sizeof(double) );
        SumOper = 0;
        ProdOper = 1;
        LowValue = 0;
        HiValue = 256;
        strcpy( DReserved, "D2R" );
    }
    /// ///////////////////////////////////////////////////////
    /// ///////////////////////////////////////////////////////
    ~DIM2(){
        FreeAll();
        if( HFileMod!=NULL ){
            CloseHandle(HFileMod);
        }
        DeleteFileA( CHAR_PATH );
        if( HBMP_PLOT!=NULL ){
            DeleteObject( HBMP_PLOT );
        }
        if( HBMP_PLOT!=NULL ){
            DeleteObject( HBMP_PLOT );
        }
    }
    /// ///////////////////////////////////////////////////////
    /// ///////////////////////////////////////////////////////
    /// ///////////////////////////////////////////////////////
    /// ///////////////////////////////////////////////////////
    /// ///////////////////////////////////////////////////////
    /// ///////////////////////////////////////////////////////
    /// ///////////////////////////////////////////////////////
    BOOL FreeAll(){
        FreeBuff();
        FreeImage();
        FreeImageIn();
        FreeImagePlot();
        return TRUE;
    }
    /// ///////////////////////////////////////////////////////
    /// ///////////////////////////////////////////////////////
    BOOL FreeBuff(){
        if( RgbBuff!=NULL ){
            free( RgbBuff );
        }
        RgbBuff = NULL;
        return TRUE;
    }
    /// ///////////////////////////////////////////////////////
    /// ///////////////////////////////////////////////////////
    BOOL FreeImage(){
        if( HBMP_OUT!=NULL ){
            DeleteObject( HBMP_OUT );
        }
        HBMP_OUT = NULL;
        return TRUE;
    }
    /// ///////////////////////////////////////////////////////
    /// ///////////////////////////////////////////////////////
    BOOL FreeImagePlot(){
        if( HBMP_PLOT!=NULL ){
            DeleteObject( HBMP_PLOT );
        }
        HBMP_PLOT = NULL;
        return TRUE;
    }
    /// ///////////////////////////////////////////////////////
    /// ///////////////////////////////////////////////////////
    BOOL FreeImageIn(){
        if( HBMP_IN!=NULL ){
            DeleteObject( HBMP_IN );
        }
        HBMP_IN = NULL;
        return TRUE;
    }
    /// ///////////////////////////////////////////////////////
    /// ///////////////////////////////////////////////////////
    BOOL ClosePath(){
        if( HFileMod!=NULL ){
            CloseHandle(HFileMod);
            DeleteFileA( CHAR_PATH );
        }
        HFileMod = NULL;
        return TRUE;
    }
    /// ///////////////////////////////////////////////////////
    /// ///////////////////////////////////////////////////////
    BOOL SetPath( LPCSTR LPATH ){
        if( HFileMod!=NULL ){
            CloseHandle(HFileMod);
            DeleteFileA( CHAR_PATH );
            HFileMod = NULL;
        }
        strcpy( CHAR_PATH, LPATH );
        sprintf( CHAR_PATH, "%szzim%i.module", LPATH, Ind_File );
        HFileMod = CreateFile( CHAR_PATH, GENERIC_WRITE|GENERIC_READ, 0, NULL, CREATE_ALWAYS,
                               FILE_ATTRIBUTE_NORMAL | FILE_ATTRIBUTE_HIDDEN, NULL );
        return TRUE;
    }
    ///
    ///
    LPCSTR GetInfo( int NMod ){
        return GetInfo( NMod, TRUE );
    }
    //
    LPCSTR GetInfo( int NMod, BOOL ParseLoaded ){
        static char T2T[1204];
        ZeroMemory( T2T, 1023 );
        switch( ProcType ){
            case SHOW_2D:
                if( !IsLoaded && ParseLoaded ){
                    sprintf( T2T, "Módulo %i sin procesar, presione Ejecutar ó Ctrl+R", NMod );
                }else{
                    sprintf( T2T, "Módulo %i, Muestra la imagen.", NMod );
                }
                break;
            case RGB2GRAY_2DPROC:{
                if( !IsLoaded && ParseLoaded ){
                    sprintf( T2T, "Módulo %i sin procesar, presione Ejecutar ó Ctrl+R", NMod );
                }else{
                    sprintf( T2T, "Módulo %i, convierte la imagen a escala de grises.", NMod );
                }
                }
                break;
            case BINARY_2DPROC:{
                if( !IsLoaded && ParseLoaded ){
                    sprintf( T2T, "Módulo %i sin procesar, presione Ejecutar ó Ctrl+R", NMod );
                }else{
                    sprintf( T2T, "Módulo %i, binariza la imagen con un umbral de %g(%i).", NMod,
                            ((double)LevelBIN)/255, LevelBIN   );
                }
                }
                /// MessageBoxA( *HWN_MAIN, T2T, "", 0 );
                break;
            case CONV_3x3_2DPROC:{
                if( !IsLoaded && ParseLoaded ){
                    sprintf( T2T, "Módulo %i sin procesar, presione Ejecutar ó Ctrl+R", NMod );
                }else{
                    switch( Kernel3x3_Type ){
                        case BLUR_K:
                            sprintf( T2T, "Módulo %i, convolución con un kernel Blur.", NMod );
                            break;
                        case EMBOSS:
                            sprintf( T2T, "Módulo %i, convolución con un kernel Emboss.", NMod );
                            break;
                        case SHARPEN:
                            sprintf( T2T, "Módulo %i, convolución con un kernel Sharpen.", NMod );
                            break;
                        case SMOOTH:
                            sprintf( T2T, "Módulo %i, convolución con un kernel Smooth.", NMod );
                            break;
                        case MEANRENOVAL:
                            sprintf( T2T, "Módulo %i, convolución con un kernel MeanRenoval.", NMod );
                            break;
                        case LP_MEDIA:
                            sprintf( T2T, "Módulo %i, convolución kernel pasa bajos(Media).", NMod );
                            break;
                        case LP_POND:
                            sprintf( T2T, "Módulo %i, convolución kernel pasa bajos(Ponderada).", NMod );
                            break;
                        case HP_MEDIA:
                            sprintf( T2T, "Módulo %i, convolución kernel pasa altos(Media).", NMod );
                            break;
                        case HP_LAPLACE:
                            sprintf( T2T, "Módulo %i, convolución kernel pasa altos(Laplace).", NMod );
                            break;
                        case HP_LAPLACE_MIN:
                            sprintf( T2T, "Módulo %i, convolución kernel pasa altos(Laplace Min).", NMod );
                            break;
                        case SOBEL:
                            sprintf( T2T, "Módulo %i, Detección de bordes Sobel.", NMod );
                            break;
                        case PREWITT:
                            sprintf( T2T, "Módulo %i, Detección de bordes Prewitt.", NMod );
                            break;
                        case KIRSH:
                            sprintf( T2T, "Módulo %i, Detección de bordes Kirsh.", NMod );
                            break;
                        case USUARIO:
                            sprintf( T2T, "Módulo %i, Convolución con kernel de usuario.", NMod );
                            break;
                        case AVERAGE:
                            sprintf( T2T, "Módulo %i, Filtro promedio de orden %i.", NMod, NOrd );
                            break;
                    }
                }
                }
                /// MessageBoxA( *HWN_MAIN, T2T, "", 0 );
                break;
            case HIST_2DPROC:{
                if( !IsLoaded && ParseLoaded ){
                    sprintf( T2T, "Módulo %i sin procesar, presione Ejecutar ó Ctrl+R", NMod );
                }else{
                    sprintf( T2T, "Módulo %i, Historigrama de la imagen.", NMod );
                }
                }
                /// MessageBoxA( *HWN_MAIN, T2T, "", 0 );
                break;
            case REDIM_2DPROC:{
                if( !IsLoaded && ParseLoaded ){
                    sprintf( T2T, "Módulo %i sin procesar, presione Ejecutar ó Ctrl+R", NMod );
                }else{
                    sprintf( T2T, "Módulo %i, Redimensionar imagen.", NMod );
                }
                }
                break;
            case ARITM_2DPROC:{
                if( !IsLoaded && ParseLoaded ){
                    sprintf( T2T, "Módulo %i sin procesar, presione Ejecutar ó Ctrl+R", NMod );
                }else{
                    switch( ArithType ){
                        case 0:
                            if( InvOut ){
                                sprintf( T2T, "Módulo %i, Suma(%i) Negado.", NMod, SumOper );
                            }else{
                                sprintf( T2T, "Módulo %i, Suma(%i).", NMod, SumOper );
                            }
                            break;
                        case 1:
                            if( InvOut ){
                                sprintf( T2T, "Módulo %i, Producto(%g) Negado.", NMod, ProdOper );
                            }else{
                                sprintf( T2T, "Módulo %i, Producto(%g).", NMod, ProdOper );
                            }
                            break;
                        case 2:
                            if( InvOut ){
                                sprintf( T2T, "Módulo %i, Comparación (%i<im<%i) Negado.", NMod, LowValue, HiValue );
                            }else{
                                sprintf( T2T, "Módulo %i, Comparación (%i<im<%i).", NMod, LowValue, HiValue );
                            }
                            break;
                    }
                    ///sprintf( T2T, "Módulo %i, Redimensionar imagen.", NMod );
                }
                }
                break;
                ///
                ///
                ///
            case BW_LABEL:{
                if( !IsLoaded && ParseLoaded ){
                    sprintf( T2T, "Módulo %i sin procesar, presione Ejecutar ó Ctrl+R", NMod );
                }else{
                    sprintf( T2T, "Módulo %i, (Conteo binario), Objetos encontrados %i.", NMod, NObjBWL );
                }
                }
                break;
                ///
                ///
                ///
            case IMERODE:{
                if( !IsLoaded && ParseLoaded ){
                    sprintf( T2T, "Módulo %i sin procesar, presione Ejecutar ó Ctrl+R", NMod );
                }else{
                    char Typestrel[8][16] = { "BALL", "SQUARE", "DIAMOND" };
                    switch( OptErode ){
                        case 0:
                            sprintf( T2T, "Módulo %i, Erosión binaria, %s Radio %i", NMod, Typestrel[OptErode], NOrdErode );
                            break;
                        case 1:
                            sprintf( T2T, "Módulo %i, Erosión binaria, %s Lado %i", NMod, Typestrel[OptErode], NOrdErode );
                            break;
                        case 2:
                            sprintf( T2T, "Módulo %i, Erosión binaria, %s Radio %i", NMod, Typestrel[OptErode], NOrdErode );
                            break;
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
            case IMDILATE:{
                if( !IsLoaded && ParseLoaded ){
                    sprintf( T2T, "Módulo %i sin procesar, presione Ejecutar ó Ctrl+R", NMod );
                }else{
                    char Typestrel[8][16] = { "BALL", "SQUARE", "DIAMOND" };
                    switch( OptErode ){
                        case 0:
                            sprintf( T2T, "Módulo %i, Dilatación binaria, %s Radio %i", NMod, Typestrel[OptErode], NOrdErode );
                            break;
                        case 1:
                            sprintf( T2T, "Módulo %i, Dilatación binaria, %s Lado %i", NMod, Typestrel[OptErode], NOrdErode );
                            break;
                        case 2:
                            sprintf( T2T, "Módulo %i, Dilatación binaria, %s Radio %i", NMod, Typestrel[OptErode], NOrdErode );
                            break;
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
            case IMOPEN:{
                if( !IsLoaded && ParseLoaded ){
                    sprintf( T2T, "Módulo %i sin procesar, presione Ejecutar ó Ctrl+R", NMod );
                }else{
                    char Typestrel[8][16] = { "BALL", "SQUARE", "DIAMOND" };
                    switch( OptErode ){
                        case 0:
                            sprintf( T2T, "Módulo %i, Apertura binaria, %s Radio %i", NMod, Typestrel[OptErode], NOrdErode );
                            break;
                        case 1:
                            sprintf( T2T, "Módulo %i, Apertura binaria, %s Lado %i", NMod, Typestrel[OptErode], NOrdErode );
                            break;
                        case 2:
                            sprintf( T2T, "Módulo %i, Apertura binaria, %s Radio %i", NMod, Typestrel[OptErode], NOrdErode );
                            break;
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
            case IMCLOSE:{
                if( !IsLoaded && ParseLoaded ){
                    sprintf( T2T, "Módulo %i sin procesar, presione Ejecutar ó Ctrl+R", NMod );
                }else{
                    char Typestrel[8][16] = { "BALL", "SQUARE", "DIAMOND" };
                    switch( OptErode ){
                        case 0:
                            sprintf( T2T, "Módulo %i, Cierre binario, %s Radio %i", NMod, Typestrel[OptErode], NOrdErode );
                            break;
                        case 1:
                            sprintf( T2T, "Módulo %i, Cierre binario, %s Lado %i", NMod, Typestrel[OptErode], NOrdErode );
                            break;
                        case 2:
                            sprintf( T2T, "Módulo %i, Cierre binario, %s Radio %i", NMod, Typestrel[OptErode], NOrdErode );
                            break;
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
            case NO_PROC_Signal_2D:{
                sprintf( T2T, "Módulo %i sin procedimiento, doble click para modificar.", NMod );
                }
                break;
                ///
                ///
                ///
            default:{
                if( !IsLoaded && ParseLoaded ){
                    sprintf( T2T, "Módulo %i sin procesar, presione Ejecutar ó Ctrl+R", NMod );
                }else{
                    sprintf( T2T, "Módulo %i, Carga de la imagen.", NMod );
                }
                }
                break;
        }
        return (LPCSTR)(T2T);
    }
    ///
    ///
    BOOL PreloadImage(){
        HBITMAP HBMT = LoadImageF( FNameT );
        if( HBMT==NULL ){
            strcpy( FNameT, FName );
            return FALSE;
        }
        strcpy( FName, FNameT );
        FreeImage();
        FreeImageIn();
        FreeBuff();
        HBMP_OUT = HBMT;
        BITMAP BMPT = {0};
        GetObjectA( HBMP_OUT, sizeof(BITMAP), &BMPT );
        WPIC_OUT = BMPT.bmWidth;
        HPIC_OUT = BMPT.bmHeight;
        WPIC_IN = WPIC_OUT;
        HPIC_IN = HPIC_OUT;
        NBytesBuff = 4  * WPIC_OUT * HPIC_OUT;
        RgbBuff = (RGBQUAD*)malloc( NBytesBuff + 1024 );
        ZeroMemory( RgbBuff, NBytesBuff );
        GetBitmapBits( HBMP_OUT, NBytesBuff, RgbBuff );
        WriteDatas();
        FreeBuff();
        return TRUE;
    }
    ///
    ///
    /// ///////////////////////////////////////////////////////
    /// ///////////////////////////////////////////////////////
    BOOL ReadDatas( RGBQUAD* RgbQOut ){
        if( RgbQOut!=NULL ){
            DWORD BWRITTEN = 0;
            SetFilePointer( HFileMod, 0, NULL, FILE_BEGIN );
            ReadFile( HFileMod, RgbQOut, NBytesBuff, &BWRITTEN, NULL );
            return (BWRITTEN==NBytesBuff);
        }
        return FALSE;
    }
    /// ///////////////////////////////////////////////////////
    /// ///////////////////////////////////////////////////////
    BOOL ReadDatas(){
        FreeBuff();
        RgbBuff = (RGBQUAD*)malloc( NBytesBuff + 1024 );
        if( RgbBuff!=NULL ){
            DWORD BWRITTEN = 0;
            SetFilePointer( HFileMod, 0, NULL, FILE_BEGIN );
            ReadFile( HFileMod, RgbBuff, NBytesBuff, &BWRITTEN, NULL );
            return (BWRITTEN==NBytesBuff);
        }
        return FALSE;
    }
    /// ///////////////////////////////////////////////////////
    /// ///////////////////////////////////////////////////////
    BOOL WriteDatas(){
        if( RgbBuff!=NULL ){
            DWORD BWRITTEN = 0;
            SetFilePointer( HFileMod, 0, NULL, FILE_BEGIN );
            WriteFile( HFileMod, RgbBuff, NBytesBuff, &BWRITTEN, NULL );
            return (BWRITTEN==NBytesBuff);
        }
        return FALSE;
    }
    /// ///////////////////////////////////////////////////////
    /// ///////////////////////////////////////////////////////
    BOOL WriteDatas( RGBQUAD* RgbB_in, int WPC, int HPC ){
        WPIC_OUT = WPC;
        HPIC_OUT = HPC;
        NBytesBuff = 4  * WPIC_OUT * HPIC_OUT;
        if( HFileMod!=NULL ){
            DWORD BWRITTEN = 0;
            SetFilePointer( HFileMod, 0, NULL, FILE_BEGIN );
            WriteFile( HFileMod, RgbB_in, NBytesBuff, &BWRITTEN, NULL );
            return (BWRITTEN==NBytesBuff);
        }
        return FALSE;
    }
    /// ///////////////////////////////////////////////////////
    /// ///////////////////////////////////////////////////////
    /// ///////////////////////////////////////////////////////
    /// ///////////////////////////////////////////////////////
    HBITMAP	NewBitmap( int W_NEWHB, int H_NEWHB ){
        HDC HDCN = GetDC( NULL );
        HBITMAP HBP2RET = NULL;
        HBP2RET = CreateCompatibleBitmap( HDCN, W_NEWHB, H_NEWHB );
        /*
        DWORD GLE = GetLastError();
        if( GLE!=0 ){
            exit( GLE );
            ReleaseDC( NULL, HDCN );
            return NULL;
        }
        //*/
        ReleaseDC( NULL, HDCN );
        return HBP2RET;
    }
    /// ////////////////////
    HBITMAP GetPicPlot(){ return HBMP_PLOT; }
    HBITMAP GetPicPlot( int WP_OUT, int HP_OUT ){ return SetPicPlot( WP_OUT, HP_OUT); }
    /// ////////////////////
    HBITMAP SetPicPlot( int WP_OUT, int HP_OUT ){
        if( HBMP_OUT==NULL ){ return NULL; }
        ///
        ///
        int W_OUT = WP_OUT;
        int H_OUT = (int)( round( (WP_OUT * HPIC_OUT)/((double)WPIC_OUT) ) );
        if( H_OUT>HP_OUT ){
            H_OUT = HP_OUT;
            W_OUT = (int)( round( (H_OUT * WPIC_OUT)/((double)HPIC_OUT) ) );
        }
        ///
        ///
        if( W_OUT!=WPIC_PLOT || H_OUT!=HPIC_PLOT || HBMP_PLOT==NULL ){
            if( HBMP_PLOT!=NULL ){
                DeleteObject( HBMP_PLOT );
            }
            WPIC_PLOT = W_OUT;
            HPIC_PLOT = H_OUT;
            HBMP_PLOT = NewBitmap( WPIC_PLOT, HPIC_PLOT );
        }
        HDC HDCN = GetDC( 0 );
        HDC hdc = CreateCompatibleDC( HDCN );
        HDC memDC = CreateCompatibleDC( HDCN );
        ReleaseDC( NULL, HDCN );
        ///
        SelectObject( hdc , HBMP_OUT );/// HBPLOT
        SelectObject( memDC , HBMP_PLOT );/// HB_I
        SetStretchBltMode( memDC , HALFTONE ); /// COLORONCOLOR     HALFTONE
        StretchBlt( memDC, 0, 0, WPIC_PLOT, HPIC_PLOT, hdc, 0, 0, WPIC_OUT, HPIC_OUT, SRCCOPY );
        ///
        DeleteDC( hdc );
        DeleteDC( memDC );
        ///
        ///
        if( IsBin ){
            RGBQUAD* RgbQ = (RGBQUAD*)malloc( 4 * WPIC_PLOT * HPIC_PLOT );
            GetBitmapBits( HBMP_PLOT, 4 * WPIC_PLOT * HPIC_PLOT, RgbQ );
            for( int i=0; i<(WPIC_PLOT * HPIC_PLOT); i++ ){
                RgbQ[i].rgbRed   =
                RgbQ[i].rgbGreen =
                RgbQ[i].rgbBlue  = 255*(RgbQ[i].rgbRed>32);
            }
            SetBitmapBits( HBMP_PLOT, 4 * WPIC_PLOT * HPIC_PLOT, RgbQ );
            free( RgbQ );
        }
        ///
        ///
        return HBMP_PLOT;
        ///
    }
    /// ///////////////////////////////////////////////////////
    BOOL SetInput( DIM2* DIN ){
        Dim_In = DIN;
        return TRUE;
    }
    /// ///////////////////////////////////////////////////////
    /// ///////////////////////////////////////////////////////
    /// ///////////////////////////////////////////////////////
    DIM2* GetInput_Try(){
        DIM2*   D2_INPUT = Dim_In;
        if( ImageInP>0 || Dim_In->ProcType==NO_PROC_Signal_2D || Dim_In->ProcType==HIST_2DPROC ){
            D2_INPUT = D2_SOURCE;
        }
        return D2_INPUT;
    }
    /// ///////////////////////////////////////////////////////
    /// ///////////////////////////////////////////////////////
    /// ///////////////////////////////////////////////////////
    DIM2* GetInput(){
        DIM2*   D2_INPUT = Dim_In;
        if( ImageInP>0 ){
            D2_INPUT = D2_SOURCE;
        }
        return D2_INPUT;
    }
    /// ///////////////////////////////////////////////////////
    /// ///////////////////////////////////////////////////////
    /// ///////////////////////////////////////////////////////
    HBITMAP Redim_PIC( HBITMAP HBMI, int W_OUT, int H_OUT ){
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
        return HBMP_T;
    }
    /// ///////////////////////////////////////////////////////
    /// ///////////////////////////////////////////////////////
    /// ///////////////////////////////////////////////////////
    LPCSTR GetImageType(){
        if( IsBin ){
            return "Escala Binaria";
        }
        if( IsGray ){
            return "Escala de grises";
        }
        return "Color RGB";
    }
    /// ///////////////////////////////////////////////////////
    /// ///////////////////////////////////////////////////////
    /// ///////////////////////////////////////////////////////
    /// ///////////////////////////////////////////////////////
    /// ///////////////////////////////////////////////////////
    /// ///////////////////////////////////////////////////////
    /// ///////////////////////////////////////////////////////
    BOOL            CONV3x3                                     ( RGBQUAD*, int, int, int*, int*, int, int );
    BOOL            CONV3x3_TRY                                 ( RGBQUAD*, int, int, int*, int*, int, int, BOOL );
    BOOL            ARITH_PROCS                                 ( RGBQUAD*, int, int, int, double, int, int, BOOL, BOOL );
    ///
    int             BWLABEL                                     ( RGBQUAD*, int, int );
    int             BWLABEL_NEW                                 ( RGBQUAD*, int, int );
    int             BWLABEL_NEW2                                ( RGBQUAD*, int, int );
    int             BWLABEL_NEW3                                ( RGBQUAD*, int, int );
    int             BWLABEL_NEW4                                ( RGBQUAD*, int, int );
    int             IMFILL                                      ( RGBQUAD*, int, int, BYTE );
    ///
    BOOL            AVERAGEPROC                                 ( DIM2* );
    BOOL            ERODEPROC                                   ( DIM2* );
    BOOL            DILATEPROC                                  ( DIM2* );
    BOOL            OPENPROC                                    ( DIM2* );
    BOOL            CLOSEPROC                                   ( DIM2* );
    ///
    BOOL            IMFILLPROC                                  ( DIM2* );
    /// ///////////////////////////////////////////////////////NO_PROC_Signal_2D
    BOOL SetProc( HWND HW2PIC ){
        ///
        ///
        if( ProcType==NO_PROC_Signal_2D ){
            return FALSE;
        }
        DIM2* D2PTR = GetInput();
        while( D2PTR->ProcType==NO_PROC_Signal_2D ){
            D2PTR = D2PTR->GetInput();
            if( D2PTR==NULL ){
                break;
            }
        }
        //
        if( D2PTR==NULL ){
            return FALSE;
        }
        ///
        ///
        WPIC_IN = D2PTR->WPIC_OUT;
        HPIC_IN = D2PTR->HPIC_OUT;
        IsBin = D2PTR->IsBin || (D2PTR->ProcType==BW_LABEL);
        IsGray = (D2PTR->IsGray || IsBin)  || (D2PTR->ProcType==BW_LABEL);
        switch( ProcType ){
            ///
            case SHOW_2D:{
                WPIC_OUT = WPIC_IN;
                HPIC_OUT = HPIC_IN;
                NPosBuff = WPIC_OUT * HPIC_OUT;
                NBytesBuff = 4  * WPIC_OUT * HPIC_OUT;
                D2PTR->ReadDatas();
                ///
                if( D2PTR->ProcType==BW_LABEL ){
                    for( int i=0; i<(int)NPosBuff; i++ ){
                        D2PTR->RgbBuff[i].rgbRed   =
                        D2PTR->RgbBuff[i].rgbGreen =
                        D2PTR->RgbBuff[i].rgbBlue  = 255 * (int)((D2PTR->RgbBuff[i].rgbRed!=255) || (D2PTR->RgbBuff[i].rgbGreen!=255) ||
                                                        (D2PTR->RgbBuff[i].rgbBlue!=255));
                    }
                }
                ///
                FreeImage();
                HBMP_OUT = NewBitmap( WPIC_OUT, HPIC_OUT );
                SetBitmapBits( HBMP_OUT, NBytesBuff, D2PTR->RgbBuff );
                WriteDatas( D2PTR->RgbBuff, WPIC_OUT, HPIC_OUT );
                D2PTR->FreeBuff();
                memcpy( D2PTR->Hist, Hist, 256 * sizeof(double) );
            }
                break;
            ///
            case RGB2GRAY_2DPROC:{
                ZeroMemory( Hist_R, 256 * sizeof(long long) );
                ZeroMemory( Hist_G, 256 * sizeof(long long) );
                ZeroMemory( Hist_B, 256 * sizeof(long long) );
                WPIC_OUT = WPIC_IN;
                HPIC_OUT = HPIC_IN;
                NPosBuff = WPIC_OUT * HPIC_OUT;
                NBytesBuff = 4  * WPIC_OUT * HPIC_OUT;
                D2PTR->ReadDatas();
                if( D2PTR->ProcType==BW_LABEL ){
                    for( int i=0; i<(int)NPosBuff; i++ ){
                        D2PTR->RgbBuff[i].rgbRed   =
                        D2PTR->RgbBuff[i].rgbGreen =
                        D2PTR->RgbBuff[i].rgbBlue  = 255 * (int)((D2PTR->RgbBuff[i].rgbRed!=255) || (D2PTR->RgbBuff[i].rgbGreen!=255) ||
                                                        (D2PTR->RgbBuff[i].rgbBlue!=255));
                    }
                }
                BYTE VGRAY = 0;
                ZeroMemory( Hist, 256 * sizeof(double));
                for( int i=0; i<(int)NPosBuff; i++ ){ /// 0.2989 * R + 0.5870 * G + 0.1140 * B
                    VGRAY =
                    D2PTR->RgbBuff[i].rgbRed =
                    D2PTR->RgbBuff[i].rgbGreen =
                    D2PTR->RgbBuff[i].rgbBlue =
                        (BYTE)round(
                                    0.2989 * D2PTR->RgbBuff[i].rgbRed +
                                    0.5870 * D2PTR->RgbBuff[i].rgbGreen +
                                    0.1140 * D2PTR->RgbBuff[i].rgbBlue
                                    );
                    Hist[VGRAY] = Hist[VGRAY] + 1;
                }
                ///
                graythresh();
                ///
                FreeImage();
                HBMP_OUT = NewBitmap( WPIC_OUT, HPIC_OUT );
                SetBitmapBits( HBMP_OUT, NBytesBuff, D2PTR->RgbBuff );
                WriteDatas( D2PTR->RgbBuff, WPIC_OUT, HPIC_OUT );
                D2PTR->FreeBuff();
                IsGray = TRUE;
            }
            break;
            ///
            case BINARY_2DPROC:{
                WPIC_OUT = WPIC_IN;
                HPIC_OUT = HPIC_IN;
                NPosBuff = WPIC_OUT * HPIC_OUT;
                NBytesBuff = 4  * WPIC_OUT * HPIC_OUT;
                D2PTR->ReadDatas();
                if( D2PTR->ProcType==BW_LABEL ){
                    for( int i=0; i<(int)NPosBuff; i++ ){
                        D2PTR->RgbBuff[i].rgbRed   =
                        D2PTR->RgbBuff[i].rgbGreen =
                        D2PTR->RgbBuff[i].rgbBlue  = 255 * (int)((D2PTR->RgbBuff[i].rgbRed!=255) || (D2PTR->RgbBuff[i].rgbGreen!=255) ||
                                                        (D2PTR->RgbBuff[i].rgbBlue!=255));
                    }
                }
                BYTE VGRAY = 0;
                RGBQUAD* BPTR = NULL;
                ZeroMemory( Hist, 256 * sizeof(double));
                ///
                /// OffsetType
                ///
                OffSProm = 0;
                OffS0    = 0;
                OffS255  = 0;
                /// -----------------------------------------------------
                /// OffsetType = 0;
                /// -----------------------------------------------------
                if( !IsGray ){
                    IsGray = TRUE;
                    switch( OffsetType ){
                        case 0:
                            if( BW_OPTS==0 ){
                                for( int i=0; i<(int)NPosBuff; i++ ){ /// 0.2989 * R + 0.5870 * G + 0.1140 * B
                                    VGRAY =
                                    D2PTR->RgbBuff[i].rgbRed   =
                                    D2PTR->RgbBuff[i].rgbGreen =
                                    D2PTR->RgbBuff[i].rgbBlue  =
                                        255*(( (BYTE)round(
                                                    0.2989 * D2PTR->RgbBuff[i].rgbRed +
                                                    0.5870 * D2PTR->RgbBuff[i].rgbGreen +
                                                    0.1140 * D2PTR->RgbBuff[i].rgbBlue
                                                    ))>LevelBIN);
                                    Hist[VGRAY] = Hist[VGRAY] + 1;
                                }
                            }else{
                                for( int i=0; i<(int)NPosBuff; i++ ){ /// 0.2989 * R + 0.5870 * G + 0.1140 * B
                                    VGRAY =
                                    D2PTR->RgbBuff[i].rgbRed   =
                                    D2PTR->RgbBuff[i].rgbGreen =
                                    D2PTR->RgbBuff[i].rgbBlue  =
                                        255*(( (BYTE)round(
                                                    0.2989 * D2PTR->RgbBuff[i].rgbRed +
                                                    0.5870 * D2PTR->RgbBuff[i].rgbGreen +
                                                    0.1140 * D2PTR->RgbBuff[i].rgbBlue
                                                    ))<=LevelBIN);
                                    Hist[VGRAY] = Hist[VGRAY] + 1;
                                }
                            }
                            graythresh();
                            break;
                        case 2:
                        case 3:
                        case 4:
                        case 5:
                        case 1:{
                            BPTR = (RGBQUAD*)&D2PTR->RgbBuff[0];
                            for( int i=0; i<(int)NPosBuff; i++ ){ /// 0.2989 * R + 0.5870 * G + 0.1140 * B
                                VGRAY =
                                BPTR->rgbRed   =
                                BPTR->rgbGreen =
                                BPTR->rgbBlue  =
                                    (( (BYTE)round(
                                                0.2989 * BPTR->rgbRed +
                                                0.5870 * BPTR->rgbGreen +
                                                0.1140 * BPTR->rgbBlue
                                                )));
                                Hist[VGRAY] = Hist[VGRAY] + 1;
                                OffSProm = OffSProm + VGRAY;
                                OffS0    = OffS0    + (double)(VGRAY!=0);
                                /// OffS255  = OffS255  + (double)(VGRAY!=255);
                                BPTR++;
                            }
                            OffS0    = OffSProm/OffS0;
                            /// OffS255  = OffSProm/OffS255;
                            OffSProm = OffSProm/NPosBuff;
                            ///
                            ///
                            graythresh();
                            BPTR = (RGBQUAD*)&D2PTR->RgbBuff[0];
                            if( OffsetType==1 ){
                                VGRAY = (BYTE)( round(255 * GrayValue) );
                                if( BW_OPTS==0 ){
                                    for( int i=0; i<(int)NPosBuff; i++ ){
                                        BPTR->rgbRed   =
                                        BPTR->rgbGreen =
                                        BPTR->rgbBlue  = 255 * (BYTE)(BPTR->rgbRed>VGRAY);
                                        BPTR++;
                                    }
                                }else{
                                    for( int i=0; i<(int)NPosBuff; i++ ){
                                        BPTR->rgbRed   =
                                        BPTR->rgbGreen =
                                        BPTR->rgbBlue  = 255 * (BYTE)(BPTR->rgbRed<=VGRAY);
                                        BPTR++;
                                    }
                                }
                            }
                            if( OffsetType==2 ){
                                VGRAY = (BYTE)( round(OffSProm) );
                                if( BW_OPTS==0 ){
                                    for( int i=0; i<(int)NPosBuff; i++ ){
                                        BPTR->rgbRed   =
                                        BPTR->rgbGreen =
                                        BPTR->rgbBlue  = 255 * (BYTE)(BPTR->rgbRed>VGRAY);
                                        BPTR++;
                                    }
                                }else{
                                    for( int i=0; i<(int)NPosBuff; i++ ){
                                        BPTR->rgbRed   =
                                        BPTR->rgbGreen =
                                        BPTR->rgbBlue  = 255 * (BYTE)(BPTR->rgbRed<=VGRAY);
                                        BPTR++;
                                    }
                                }
                            }
                            if( OffsetType==3 ){
                                VGRAY = (BYTE)( round(OffS0) );
                                if( BW_OPTS==0 ){
                                    for( int i=0; i<(int)NPosBuff; i++ ){
                                        BPTR->rgbRed   =
                                        BPTR->rgbGreen =
                                        BPTR->rgbBlue  = 255 * (BYTE)(BPTR->rgbRed>VGRAY);
                                        BPTR++;
                                    }
                                }else{
                                    for( int i=0; i<(int)NPosBuff; i++ ){
                                        BPTR->rgbRed   =
                                        BPTR->rgbGreen =
                                        BPTR->rgbBlue  = 255 * (BYTE)(BPTR->rgbRed<=VGRAY);
                                        BPTR++;
                                    }
                                }
                            }
                            if( OffsetType==4 ){
                                VGRAY = (BYTE)( round(255 * (1-GrayValue) ) );
                                if( BW_OPTS==0 ){
                                    for( int i=0; i<(int)NPosBuff; i++ ){
                                        BPTR->rgbRed   =
                                        BPTR->rgbGreen =
                                        BPTR->rgbBlue  = 255 * (BYTE)(BPTR->rgbRed>VGRAY);
                                        BPTR++;
                                    }
                                }else{
                                    for( int i=0; i<(int)NPosBuff; i++ ){
                                        BPTR->rgbRed   =
                                        BPTR->rgbGreen =
                                        BPTR->rgbBlue  = 255 * (BYTE)(BPTR->rgbRed<=VGRAY);
                                        BPTR++;
                                    }
                                }
                            }
                            if( OffsetType==5 ){
                                double HistT[256] = {0};
                                VGRAY = (BYTE)( round(255 * GrayValue ) );
                                BPTR = (RGBQUAD*)&D2PTR->RgbBuff[0];
                                for( int i=0; i<(int)NPosBuff; i++ ){
                                    if( BPTR->rgbRed<=VGRAY ){
                                        HistT[VGRAY] = HistT[VGRAY] + 1;
                                    }else{
                                        HistT[BPTR->rgbRed] = HistT[BPTR->rgbRed] + 1;
                                    }
                                    BPTR++;
                                }
                                VGRAY = ParceBYTE( round( 255 * D2PTR->graythreshHist( HistT ) ) );
                                BPTR = (RGBQUAD*)&D2PTR->RgbBuff[0];
                                if( BW_OPTS==0 ){
                                    for( int i=0; i<(int)NPosBuff; i++ ){
                                        BPTR->rgbRed   =
                                        BPTR->rgbGreen =
                                        BPTR->rgbBlue  = 255 * (BYTE)(BPTR->rgbRed>VGRAY);
                                        BPTR++;
                                    }
                                }else{
                                    for( int i=0; i<(int)NPosBuff; i++ ){
                                        BPTR->rgbRed   =
                                        BPTR->rgbGreen =
                                        BPTR->rgbBlue  = 255 * (BYTE)(BPTR->rgbRed<=VGRAY);
                                        BPTR++;
                                    }
                                }
                            }
                            LevelBIN = VGRAY;
                            /// char T2T[64] = {0};sprintf( T2T, "\n\n%f\n%f\n%f\n%i\n\n", OffSProm, OffS0, OffS255, VGRAY );MessageBox( NULL, T2T, " ", 0 );
                        }
                            break;
                    }
                    ///
                    ///
                }else{
                    /// ISGRAY
                    /// ISGRAY
                    /// ISGRAY
                    /// ISGRAY
                    /// ISGRAY
                    /// ISGRAY
                    /// ISGRAY
                    switch( OffsetType ){
                        case 0:
                            if( BW_OPTS==0 ){
                                for( int i=0; i<(int)NPosBuff; i++ ){ /// 0.2989 * R + 0.5870 * G + 0.1140 * B
                                    VGRAY =
                                    D2PTR->RgbBuff[i].rgbRed =
                                    D2PTR->RgbBuff[i].rgbGreen =
                                    D2PTR->RgbBuff[i].rgbBlue = 255 * (D2PTR->RgbBuff[i].rgbRed>LevelBIN);
                                    Hist[VGRAY] = Hist[VGRAY] + 1;
                                }
                            }else{
                                for( int i=0; i<(int)NPosBuff; i++ ){ /// 0.2989 * R + 0.5870 * G + 0.1140 * B
                                    VGRAY =
                                    D2PTR->RgbBuff[i].rgbRed =
                                    D2PTR->RgbBuff[i].rgbGreen =
                                    D2PTR->RgbBuff[i].rgbBlue = 255 * (D2PTR->RgbBuff[i].rgbRed<=LevelBIN);
                                    Hist[VGRAY] = Hist[VGRAY] + 1;
                                }
                            }
                            graythresh();
                            break;
                        case 2:
                        case 3:
                        case 4:
                        case 5:
                        case 1:{
                            BPTR = (RGBQUAD*)&D2PTR->RgbBuff[0];
                            for( int i=0; i<(int)NPosBuff; i++ ){
                                VGRAY = BPTR->rgbRed;
                                Hist[VGRAY] = Hist[VGRAY] + 1;
                                OffSProm = OffSProm + VGRAY;
                                OffS0    = OffS0    + (double)(VGRAY!=0);
                                BPTR++;
                            }
                            OffS0    = OffSProm/OffS0;
                            OffSProm = OffSProm/NPosBuff;
                            graythresh();
                            if( OffsetType==1 ){
                                VGRAY = (BYTE)( round(255 * GrayValue) );
                                BPTR = (RGBQUAD*)&D2PTR->RgbBuff[0];
                                if( BW_OPTS==0 ){
                                    for( int i=0; i<(int)NPosBuff; i++ ){
                                        BPTR->rgbRed   =
                                        BPTR->rgbGreen =
                                        BPTR->rgbBlue  = 255 * (BPTR->rgbRed>VGRAY);
                                        BPTR++;
                                    }
                                }else{
                                    for( int i=0; i<(int)NPosBuff; i++ ){
                                        BPTR->rgbRed   =
                                        BPTR->rgbGreen =
                                        BPTR->rgbBlue  = 255 * (BPTR->rgbRed<=VGRAY);
                                        BPTR++;
                                    }
                                }
                            }
                            if( OffsetType==2 ){
                                VGRAY = (BYTE)( round(OffSProm) );
                                BPTR = (RGBQUAD*)&D2PTR->RgbBuff[0];
                                if( BW_OPTS==0 ){
                                    for( int i=0; i<(int)NPosBuff; i++ ){
                                        BPTR->rgbRed   =
                                        BPTR->rgbGreen =
                                        BPTR->rgbBlue  = 255 * (BPTR->rgbRed>VGRAY);
                                        BPTR++;
                                    }
                                }else{
                                    for( int i=0; i<(int)NPosBuff; i++ ){
                                        BPTR->rgbRed   =
                                        BPTR->rgbGreen =
                                        BPTR->rgbBlue  = 255 * (BPTR->rgbRed<=VGRAY);
                                        BPTR++;
                                    }
                                }
                            }
                            if( OffsetType==3 ){
                                VGRAY = (BYTE)( round(OffS0) );
                                BPTR = (RGBQUAD*)&D2PTR->RgbBuff[0];
                                if( BW_OPTS==0 ){
                                    for( int i=0; i<(int)NPosBuff; i++ ){
                                        BPTR->rgbRed   =
                                        BPTR->rgbGreen =
                                        BPTR->rgbBlue  = 255 * (BPTR->rgbRed>VGRAY);
                                        BPTR++;
                                    }
                                }else{
                                    for( int i=0; i<(int)NPosBuff; i++ ){
                                        BPTR->rgbRed   =
                                        BPTR->rgbGreen =
                                        BPTR->rgbBlue  = 255 * (BPTR->rgbRed<=VGRAY);
                                        BPTR++;
                                    }
                                }
                            }
                            if( OffsetType==4 ){
                                VGRAY = (BYTE)( round(255 * (1-GrayValue) ) );
                                BPTR = (RGBQUAD*)&D2PTR->RgbBuff[0];
                                if( BW_OPTS==0 ){
                                    for( int i=0; i<(int)NPosBuff; i++ ){
                                        BPTR->rgbRed   =
                                        BPTR->rgbGreen =
                                        BPTR->rgbBlue  = 255 * (BPTR->rgbRed>VGRAY);
                                        BPTR++;
                                    }
                                }else{
                                    for( int i=0; i<(int)NPosBuff; i++ ){
                                        BPTR->rgbRed   =
                                        BPTR->rgbGreen =
                                        BPTR->rgbBlue  = 255 * (BPTR->rgbRed<=VGRAY);
                                        BPTR++;
                                    }
                                }
                            }
                            if( OffsetType==5 ){
                                double HistT[256] = {0};
                                VGRAY = (BYTE)( round(255 * GrayValue ) );
                                BPTR = (RGBQUAD*)&D2PTR->RgbBuff[0];
                                for( int i=0; i<(int)NPosBuff; i++ ){
                                    if( BPTR->rgbRed<=VGRAY ){
                                        HistT[VGRAY] = HistT[VGRAY] + 1;
                                    }else{
                                        HistT[BPTR->rgbRed] = HistT[BPTR->rgbRed] + 1;
                                    }
                                    BPTR++;
                                }
                                VGRAY = ParceBYTE( round( 255 * D2PTR->graythreshHist( HistT ) ) );
                                BPTR = (RGBQUAD*)&D2PTR->RgbBuff[0];
                                if( BW_OPTS==0 ){
                                    for( int i=0; i<(int)NPosBuff; i++ ){
                                        BPTR->rgbRed   =
                                        BPTR->rgbGreen =
                                        BPTR->rgbBlue  = 255 * (BYTE)(BPTR->rgbRed>VGRAY);
                                        BPTR++;
                                    }
                                }else{
                                    for( int i=0; i<(int)NPosBuff; i++ ){
                                        BPTR->rgbRed   =
                                        BPTR->rgbGreen =
                                        BPTR->rgbBlue  = 255 * (BYTE)(BPTR->rgbRed<=VGRAY);
                                        BPTR++;
                                    }
                                }
                            }
                            LevelBIN = VGRAY;
                        }
                            break;
                    }
                }
                ///
                ///
                ///
                ///
                ///
                ///
                ///
                FreeImage();
                HBMP_OUT = NewBitmap( WPIC_OUT, HPIC_OUT );
                SetBitmapBits( HBMP_OUT, NBytesBuff, D2PTR->RgbBuff );
                WriteDatas( D2PTR->RgbBuff, WPIC_OUT, HPIC_OUT );
                D2PTR->FreeBuff();
                IsGray = TRUE;
                IsBin = TRUE;
            }
            break;
            ///
            case CONV_3x3_2DPROC:{
                ZeroMemory( Hist_R, 256 * sizeof(long long) );
                ZeroMemory( Hist_G, 256 * sizeof(long long) );
                ZeroMemory( Hist_B, 256 * sizeof(long long) );
                WPIC_OUT = WPIC_IN;
                HPIC_OUT = HPIC_IN;
                NPosBuff = WPIC_OUT * HPIC_OUT;
                NBytesBuff = 4  * WPIC_OUT * HPIC_OUT;
                D2PTR->ReadDatas();
                if( D2PTR->ProcType==BW_LABEL ){
                    for( int i=0; i<(int)NPosBuff; i++ ){
                        D2PTR->RgbBuff[i].rgbRed   =
                        D2PTR->RgbBuff[i].rgbGreen =
                        D2PTR->RgbBuff[i].rgbBlue  = 255 * (int)((D2PTR->RgbBuff[i].rgbRed!=255) || (D2PTR->RgbBuff[i].rgbGreen!=255) ||
                                                        (D2PTR->RgbBuff[i].rgbBlue!=255));
                    }
                }
                if( !IsGray ){
                    IsGray = TRUE;
                    for( int i=0; i<(int)NPosBuff; i++ ){ /// 0.2989 * R + 0.5870 * G + 0.1140 * B
                        D2PTR->RgbBuff[i].rgbRed =
                        D2PTR->RgbBuff[i].rgbGreen =
                        D2PTR->RgbBuff[i].rgbBlue =
                            (BYTE)round(
                                        0.2989 * D2PTR->RgbBuff[i].rgbRed +
                                        0.5870 * D2PTR->RgbBuff[i].rgbGreen +
                                        0.1140 * D2PTR->RgbBuff[i].rgbBlue
                                        );
                        Hist_R[D2PTR->RgbBuff[i].rgbRed]++;
                        Hist_G[D2PTR->RgbBuff[i].rgbRed]++;
                        Hist_B[D2PTR->RgbBuff[i].rgbRed]++;
                    }
                }
                ///
                ///
                ///
                /// #define WCONV2   0
                if( Kernel3x3_Type==AVERAGE ){
                    AVERAGEPROC( D2PTR );
                }else{
                    CONV3x3_TRY( D2PTR->RgbBuff, WPIC_OUT, HPIC_OUT, Kernel3x3H, Kernel3x3V, Kernel3x3_Type,
                                        Kernel3x3_Offset[Kernel3x3_Type], BSobel_Remove );
                    ///
                    FreeImage();
                    HBMP_OUT = NewBitmap( WPIC_OUT, HPIC_OUT );
                    SetBitmapBits( HBMP_OUT, NBytesBuff, D2PTR->RgbBuff );
                }
                WriteDatas( D2PTR->RgbBuff, WPIC_OUT, HPIC_OUT );
                ///
                ///
                ZeroMemory( Hist, 256 * sizeof(double));
                for( int i=0; i<(int)NPosBuff; i++ ){
                    Hist[D2PTR->RgbBuff[i].rgbRed] = Hist[D2PTR->RgbBuff[i].rgbRed] + 1;
                }
                graythresh();
                ///
                ///
                D2PTR->FreeBuff();
                IsGray = TRUE;
                IsBin = FALSE;
                if( Kernel3x3_Type==SOBEL || Kernel3x3_Type==PREWITT ){
                    IsBin = TRUE;
                }
            }
            break;
            ///
            ///
            case HIST_2DPROC:{ ///                   H I S T O R IG R A M A(IMHIST)
                WPIC_OUT = WPIC_IN;
                HPIC_OUT = HPIC_IN;
                NPosBuff = WPIC_OUT * HPIC_OUT;
                NBytesBuff = 4  * WPIC_OUT * HPIC_OUT;
                D2PTR->ReadDatas();
                if( D2PTR->ProcType==BW_LABEL ){
                    for( int i=0; i<(int)NPosBuff; i++ ){
                        D2PTR->RgbBuff[i].rgbRed   =
                        D2PTR->RgbBuff[i].rgbGreen =
                        D2PTR->RgbBuff[i].rgbBlue  = 255 * (int)((D2PTR->RgbBuff[i].rgbRed!=255) || (D2PTR->RgbBuff[i].rgbGreen!=255) ||
                                                        (D2PTR->RgbBuff[i].rgbBlue!=255));
                    }
                }
                ZeroMemory( Hist, 256 * sizeof(double) );
                ZeroMemory( Hist_R, 256 * sizeof(double) );
                ZeroMemory( Hist_G, 256 * sizeof(double) );
                ZeroMemory( Hist_B, 256 * sizeof(double) );
                if( !IsGray ){
                    P_PromR = 0;
                    P_PromG = 0;
                    P_PromB = 0;
                    for( int i=0; i<(int)NPosBuff; i++ ){ /// 0.2989 * R + 0.5870 * G + 0.1140 * B
                        ///
                        Hist_R[D2PTR->RgbBuff[i].rgbRed]    = Hist_R[D2PTR->RgbBuff[i].rgbRed]      + 1;
                        Hist_G[D2PTR->RgbBuff[i].rgbGreen]  = Hist_G[D2PTR->RgbBuff[i].rgbGreen]    + 1;
                        Hist_B[D2PTR->RgbBuff[i].rgbBlue]   = Hist_B[D2PTR->RgbBuff[i].rgbBlue]     + 1;
                        P_PromR = P_PromR + D2PTR->RgbBuff[i].rgbRed;
                        P_PromG = P_PromG + D2PTR->RgbBuff[i].rgbGreen;
                        P_PromB = P_PromB + D2PTR->RgbBuff[i].rgbBlue;

                    }
                    P_PromR      = P_PromR/NPosBuff;
                    P_PromG      = P_PromG/NPosBuff;
                    P_PromB      = P_PromB/NPosBuff;
                }else{
                    int NZeros = 0;
                    int NOnes  = 0;
                    for( int i=0; i<(int)NPosBuff; i++ ){
                        Hist[D2PTR->RgbBuff[i].rgbRed] = Hist[D2PTR->RgbBuff[i].rgbRed] + 1;
                        ///
                        P_PromR = P_PromR + D2PTR->RgbBuff[i].rgbRed;
                        NZeros  = NZeros + (int)(D2PTR->RgbBuff[i].rgbRed!=0);
                        NOnes   = NOnes  + (int)(D2PTR->RgbBuff[i].rgbRed!=255);
                        ///
                    }
                    ///
                    if( NZeros==0 ){
                        NZeros = 1;
                    }
                    if( NOnes==0 ){
                        NOnes = 1;
                    }
                    P_Prom_Zeros = P_PromR/NZeros;
                    P_Prom_Ones  = P_PromR/NOnes;
                    P_PromR      = P_PromR/NPosBuff;
                }
                graythresh();
                FreeImage();
                HBMP_OUT = NewBitmap( WPIC_OUT, HPIC_OUT );
                SetBitmapBits( HBMP_OUT, NBytesBuff, D2PTR->RgbBuff );
                WriteDatas( D2PTR->RgbBuff, WPIC_OUT, HPIC_OUT );
                D2PTR->FreeBuff();
            }
            break;
            ///
            ///
            ///
            ///
            case REDIM_2DPROC:{
                if( RELASP ){
                    WPIC_OUT = WPIC_REDIM;
                    HPIC_OUT = (WPIC_OUT * HPIC_IN)/WPIC_IN;
                    HPIC_REDIM = HPIC_OUT;
                }else{
                    WPIC_OUT = WPIC_REDIM;
                    HPIC_OUT = HPIC_REDIM;
                }
                NPosBuff = WPIC_OUT * HPIC_OUT;
                NBytesBuff = 4  * WPIC_OUT * HPIC_OUT;
                ///
                D2PTR->ReadDatas();
                if( D2PTR->ProcType==BW_LABEL ){
                    for( int i=0; i<(int)D2PTR->NPosBuff; i++ ){
                        D2PTR->RgbBuff[i].rgbRed   =
                        D2PTR->RgbBuff[i].rgbGreen =
                        D2PTR->RgbBuff[i].rgbBlue  = 255 * (int)((D2PTR->RgbBuff[i].rgbRed!=255) || (D2PTR->RgbBuff[i].rgbGreen!=255) ||
                                                        (D2PTR->RgbBuff[i].rgbBlue!=255));
                    }
                }
                ///
                HBITMAP HBMP_O = NULL;
                /// HBMP_O = NewBitmap( D2PTR->WPIC_OUT, D2PTR->HPIC_OUT );
                HDC HDCN = GetDC( NULL );
                HBMP_O = CreateCompatibleBitmap( HDCN, D2PTR->WPIC_OUT, D2PTR->HPIC_OUT );
                ReleaseDC( NULL, HDCN );
                ///
                SetBitmapBits( HBMP_O, 4 * D2PTR->WPIC_OUT * D2PTR->HPIC_OUT, D2PTR->RgbBuff );
                D2PTR->FreeBuff();
                FreeBuff();
                RgbBuff = (RGBQUAD*)malloc( NBytesBuff + 1024 );
                ZeroMemory( RgbBuff, NBytesBuff );
                FreeImage();
                ///
                HBMP_OUT = Redim_PIC( HBMP_O, WPIC_REDIM, HPIC_REDIM );
                ///
                DeleteObject( HBMP_O );
                GetBitmapBits( HBMP_OUT, NBytesBuff, RgbBuff );
                WriteDatas( RgbBuff, WPIC_OUT, HPIC_OUT );
                ///
                ///
                ///
                ZeroMemory( Hist, 256 * sizeof(double) );
                if( IsGray ){
                    for( int i=0; i<(int)NPosBuff; i++ ){ /// 0.2989 * R + 0.5870 * G + 0.1140 * B
                        Hist[RgbBuff[i].rgbRed] = Hist[RgbBuff[i].rgbRed] + 1;
                    }
                }else{
                    BYTE BValue = 0;
                    for( int i=0; i<(int)NPosBuff; i++ ){ /// 0.2989 * R + 0.5870 * G + 0.1140 * B
                        BValue = (BYTE)round(
                                        0.2989 * RgbBuff[i].rgbRed +
                                        0.5870 * RgbBuff[i].rgbGreen +
                                        0.1140 * RgbBuff[i].rgbBlue
                                        );
                        Hist[BValue] = Hist[BValue] + 1;
                    }
                }
                graythresh();
                ///
                ///
                ///
                FreeBuff();
                IsBin = FALSE;
                IsGray = D2PTR->IsGray;
                ///
            }
                break;
            ///
            ///
            ///
            case ARITM_2DPROC:{
                WPIC_OUT = WPIC_IN;
                HPIC_OUT = HPIC_IN;
                NPosBuff = WPIC_OUT * HPIC_OUT;
                NBytesBuff = 4  * WPIC_OUT * HPIC_OUT;
                D2PTR->ReadDatas();
                if( D2PTR->ProcType==BW_LABEL ){
                    for( int i=0; i<(int)NPosBuff; i++ ){
                        D2PTR->RgbBuff[i].rgbRed   =
                        D2PTR->RgbBuff[i].rgbGreen =
                        D2PTR->RgbBuff[i].rgbBlue  = 255 * (int)((D2PTR->RgbBuff[i].rgbRed!=255) || (D2PTR->RgbBuff[i].rgbGreen!=255) ||
                                                        (D2PTR->RgbBuff[i].rgbBlue!=255));
                    }
                }
                if( !IsGray ){
                    IsGray = TRUE;
                    BYTE BValue = 0;
                    for( int i=0; i<(int)NPosBuff; i++ ){
                        BValue = (BYTE)round(
                                                0.2989 * D2PTR->RgbBuff[i].rgbRed +
                                                0.5870 * D2PTR->RgbBuff[i].rgbGreen +
                                                0.1140 * D2PTR->RgbBuff[i].rgbBlue
                                                );
                        D2PTR->RgbBuff[i].rgbRed    =
                        D2PTR->RgbBuff[i].rgbGreen  =
                        D2PTR->RgbBuff[i].rgbBlue   = BValue;
                    }
                }
                ARITH_PROCS( D2PTR->RgbBuff, NPosBuff,
                                    ArithType,
                                    SumOper,
                                    ProdOper,
                                    LowValue,
                                    HiValue,
                                    BSetPix,
                                    InvOut
                   );
                FreeImage();
                HBMP_OUT = NewBitmap( WPIC_OUT, HPIC_OUT );
                SetBitmapBits( HBMP_OUT, NBytesBuff, D2PTR->RgbBuff );
                WriteDatas( D2PTR->RgbBuff, WPIC_OUT, HPIC_OUT );
                ///
                ///
                ///
                IsGray = TRUE;
                IsBin = ArithType==2;
                ///
                ZeroMemory( Hist, 256 * sizeof(double) );
                if( IsGray ){
                    for( int i=0; i<(int)NPosBuff; i++ ){ /// 0.2989 * R + 0.5870 * G + 0.1140 * B
                        Hist[D2PTR->RgbBuff[i].rgbRed] = Hist[D2PTR->RgbBuff[i].rgbRed] + 1;
                    }
                }else{
                    BYTE BValue = 0;
                    for( int i=0; i<(int)NPosBuff; i++ ){ /// 0.2989 * R + 0.5870 * G + 0.1140 * B
                        BValue = (BYTE)round(
                                        0.2989 * D2PTR->RgbBuff[i].rgbRed +
                                        0.5870 * D2PTR->RgbBuff[i].rgbGreen +
                                        0.1140 * D2PTR->RgbBuff[i].rgbBlue
                                        );
                        Hist[BValue] = Hist[BValue] + 1;
                    }
                }
                graythresh();
                ///
                ///
                ///
                ///
                D2PTR->FreeBuff();
            }
                break;

            ///
            ///
            ///
            case BW_LABEL:{
                WPIC_OUT = WPIC_IN;
                HPIC_OUT = HPIC_IN;
                NPosBuff = WPIC_OUT * HPIC_OUT;
                NBytesBuff = 4  * WPIC_OUT * HPIC_OUT;
                D2PTR->ReadDatas();
                if( D2PTR->ProcType==BW_LABEL ){
                    for( int i=0; i<(int)NPosBuff; i++ ){
                        D2PTR->RgbBuff[i].rgbRed   =
                        D2PTR->RgbBuff[i].rgbGreen =
                        D2PTR->RgbBuff[i].rgbBlue  = 255 * (int)((D2PTR->RgbBuff[i].rgbRed!=255) || (D2PTR->RgbBuff[i].rgbGreen!=255) ||
                                                        (D2PTR->RgbBuff[i].rgbBlue!=255));
                    }
                }
                for( int i=0; i<256; i++ ){
                    Hist[i] = 0;
                }
                if( !D2PTR->IsBin && D2PTR->IsGray ){
                    for( int i=0; i<(int)NPosBuff; i++ ){
                        Hist[D2PTR->RgbBuff[i].rgbRed] = Hist[D2PTR->RgbBuff[i].rgbRed] + 1;
                    }
                    graythresh();
                    int GVAL = (int)round( 255*GrayValue );
                    for( int i=0; i<(int)NPosBuff; i++ ){
                        D2PTR->RgbBuff[i].rgbRed    =
                        D2PTR->RgbBuff[i].rgbGreen  =
                        D2PTR->RgbBuff[i].rgbBlue   = 255*( (int)( D2PTR->RgbBuff[i].rgbRed>GVAL ) );
                    }
                }else{
                    if( !D2PTR->IsGray ){
                        for( int i=0; i<(int)NPosBuff; i++ ){
                            D2PTR->RgbBuff[i].rgbRed    =
                            D2PTR->RgbBuff[i].rgbGreen  =
                            D2PTR->RgbBuff[i].rgbBlue   = (( (BYTE)round(
                                        0.2989 * D2PTR->RgbBuff[i].rgbRed +
                                        0.5870 * D2PTR->RgbBuff[i].rgbGreen +
                                        0.1140 * D2PTR->RgbBuff[i].rgbBlue
                                        )));
                            Hist[D2PTR->RgbBuff[i].rgbRed] = Hist[D2PTR->RgbBuff[i].rgbRed] + 1;
                        }
                        graythresh();
                        int GVAL = (int)round( 255*GrayValue );
                        for( int i=0; i<(int)NPosBuff; i++ ){
                            D2PTR->RgbBuff[i].rgbRed    =
                            D2PTR->RgbBuff[i].rgbGreen  =
                            D2PTR->RgbBuff[i].rgbBlue   = 255*( (int)(D2PTR->RgbBuff[i].rgbRed>GVAL) );
                        }
                    }
                }
                NObjBWL = BWLABEL( D2PTR->RgbBuff, WPIC_OUT, HPIC_OUT );
                FreeImage();
                HBMP_OUT = NewBitmap( WPIC_OUT, HPIC_OUT );
                SetBitmapBits( HBMP_OUT, NBytesBuff, D2PTR->RgbBuff );
                WriteDatas( D2PTR->RgbBuff, WPIC_OUT, HPIC_OUT );
                ///
                ///
                ///
                ///
                ///
                ///
                ZeroMemory( Hist, 256 * sizeof(double) );
                if( IsGray ){
                    for( int i=0; i<(int)NPosBuff; i++ ){ /// 0.2989 * R + 0.5870 * G + 0.1140 * B
                        Hist[D2PTR->RgbBuff[i].rgbRed] = Hist[D2PTR->RgbBuff[i].rgbRed] + 1;
                    }
                }else{
                    BYTE BValue = 0;
                    for( int i=0; i<(int)NPosBuff; i++ ){ /// 0.2989 * R + 0.5870 * G + 0.1140 * B
                        BValue = (BYTE)round(
                                        0.2989 * D2PTR->RgbBuff[i].rgbRed +
                                        0.5870 * D2PTR->RgbBuff[i].rgbGreen +
                                        0.1140 * D2PTR->RgbBuff[i].rgbBlue
                                        );
                        Hist[BValue] = Hist[BValue] + 1;
                    }
                }
                graythresh();
                ///
                ///
                D2PTR->FreeBuff();
                IsGray = FALSE;
                IsBin  = FALSE;
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
            case IMERODE:{
                ERODEPROC( D2PTR );
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
            case IMDILATE:{
                DILATEPROC( D2PTR );
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
            case IMOPEN:{
                OPENPROC( D2PTR );
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
            case IMCLOSE:{
                CLOSEPROC( D2PTR );
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
            case IMFILL_OPT:{
                IMFILLPROC( D2PTR );
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
            ///
            ///
            ///
            ///
            ///
            ///
            case GRAY_2_JET:{
                WPIC_OUT = WPIC_IN;
                HPIC_OUT = HPIC_IN;
                NPosBuff = WPIC_OUT * HPIC_OUT;
                NBytesBuff = 4  * WPIC_OUT * HPIC_OUT;
                D2PTR->ReadDatas();
                if( D2PTR->ProcType==BW_LABEL ){
                    for( int i=0; i<(int)NPosBuff; i++ ){
                        D2PTR->RgbBuff[i].rgbRed   =
                        D2PTR->RgbBuff[i].rgbGreen =
                        D2PTR->RgbBuff[i].rgbBlue  = 255 * (int)((D2PTR->RgbBuff[i].rgbRed!=255) || (D2PTR->RgbBuff[i].rgbGreen!=255) ||
                                                        (D2PTR->RgbBuff[i].rgbBlue!=255));
                    }
                }
                if( !D2PTR->IsGray && !D2PTR->IsBin ){
                    for( int i=0; i<(int)NPosBuff; i++ ){
                        D2PTR->RgbBuff[i].rgbRed    = jet[D2PTR->RgbBuff[i].rgbRed].rgbRed;
                        D2PTR->RgbBuff[i].rgbGreen  = jet[D2PTR->RgbBuff[i].rgbGreen].rgbGreen;
                        D2PTR->RgbBuff[i].rgbBlue   = jet[D2PTR->RgbBuff[i].rgbBlue].rgbBlue;
                    }
                }else{
                    for( int i=0; i<(int)NPosBuff; i++ ){
                        D2PTR->RgbBuff[i].rgbRed    = (( (BYTE)round(
                                    0.2989 * D2PTR->RgbBuff[i].rgbRed +
                                    0.5870 * D2PTR->RgbBuff[i].rgbGreen +
                                    0.1140 * D2PTR->RgbBuff[i].rgbBlue
                                    )));
                        D2PTR->RgbBuff[i] = jet[D2PTR->RgbBuff[i].rgbRed];
                    }
                }
                FreeImage();
                HBMP_OUT = NewBitmap( WPIC_OUT, HPIC_OUT );
                SetBitmapBits( HBMP_OUT, NBytesBuff, D2PTR->RgbBuff );
                WriteDatas( D2PTR->RgbBuff, WPIC_OUT, HPIC_OUT );
                D2PTR->FreeBuff();
                IsGray = FALSE;
                IsBin  = FALSE;
            }
                break;
            ///
            ///
            ///
            ///
            ///
            default:
                break;
                WPIC_OUT = WPIC_IN;
                HPIC_OUT = HPIC_IN;
                D2PTR->ReadDatas();
                if( D2PTR->ProcType==BW_LABEL ){
                    for( int i=0; i<(int)NPosBuff; i++ ){
                        D2PTR->RgbBuff[i].rgbRed   =
                        D2PTR->RgbBuff[i].rgbGreen =
                        D2PTR->RgbBuff[i].rgbBlue  = 255 * (int)((D2PTR->RgbBuff[i].rgbRed!=255) || (D2PTR->RgbBuff[i].rgbGreen!=255) ||
                                                        (D2PTR->RgbBuff[i].rgbBlue!=255));
                    }
                }
                WriteDatas( D2PTR->RgbBuff, WPIC_OUT, HPIC_OUT );
                D2PTR->FreeBuff();
                break;
        }
        return TRUE;
    }
    /// ///////////////////////////////////////////////////////
    /// ///////////////////////////////////////////////////////
    /// ///////////////////////////////////////////////////////
    /// ///////////////////////////////////////////////////////
    /// ///////////////////////////////////////////////////////
    /// ///////////////////////////////////////////////////////
    /// ///////////////////////////////////////////////////////
    BOOL graythresh(){
        double S_H = 0;
        double counts[256] = {0};
        double p[256] = {0};
        double omega[256] = {0};
        double mu[256] = {0};
        double sigma_b_squared[256] = {0};
        double mu_t = 0;
        double maxval = -1;
        double idx = -1;
        BOOL isfinite_maxval = FALSE;
        //
        /// counts = imhist(I,num_bins);
        memcpy( counts, Hist, 256 * sizeof(double) );
        //
        /// S_H = sum(counts)
        for( int i=0; i<256; i++ ){
            /// counts[i] = counts[i]/255;
            S_H = S_H + counts[i];
        }
        //
        ///  p = counts / sum(counts);
        for( int i=0; i<256; i++ ){
            p[i] = counts[i]/S_H;
        }
        //
        /// omega = cumsum(p);
        omega[0] = p[0];
        for( int i=1; i<256; i++ ){
            omega[i] = p[i] + omega[i-1];
        }
        //
        /// p = p .* (1:num_bins)'
        for( int i=0; i<256; i++ ){
            p[i] = p[i] * (i+1);
        }
        //
        /// mu = cumsum(p)
        mu[0] = p[0];
        for( int i=1; i<256; i++ ){
            mu[i] = p[i] + mu[i-1];
        }
        mu_t = mu[255];
        //
        //
        /// sigma_b_squared = (mu_t * omega - mu).^2 ./ (omega .* (1 - omega));
        for( int i=0; i<256; i++ ){
            sigma_b_squared[i] = pow((mu_t * omega[i] - mu[i]), 2) / (omega[i] * (1 - omega[i]));
            maxval = fmax( maxval, sigma_b_squared[i] );
            if( maxval==sigma_b_squared[i] ){
                idx = i;
            }
        }
        isfinite_maxval = isfinite(maxval);
        //
        //
        if( isfinite_maxval && maxval>=0 ){
            /// idx = mean(find(sigma_b_squared == maxval));
            // Normalize the threshold to the range [0, 1].
            GrayValue = (idx) / (255);
        }else{
            GrayValue = 0.0;
        }
        //
        //
        //
        /// GrayValue;
        return TRUE;
    }
    /// ///////////////////////////////////////////////////////
    /// ///////////////////////////////////////////////////////
    /// ///////////////////////////////////////////////////////
    /// ///////////////////////////////////////////////////////
    /// ///////////////////////////////////////////////////////
    /// ///////////////////////////////////////////////////////
    /// ///////////////////////////////////////////////////////
    double graythreshHist( double Histgt[256] ){
        double S_H = 0;
        double counts[256] = {0};
        double p[256] = {0};
        double omega[256] = {0};
        double mu[256] = {0};
        double sigma_b_squared[256] = {0};
        double mu_t = 0;
        double maxval = -1;
        double idx = -1;
        BOOL isfinite_maxval = FALSE;
        //
        /// counts = imhist(I,num_bins);
        memcpy( counts, Histgt, 256 * sizeof(double) );
        //
        /// S_H = sum(counts)
        for( int i=0; i<256; i++ ){
            /// counts[i] = counts[i]/255;
            S_H = S_H + counts[i];
        }
        //
        ///  p = counts / sum(counts);
        for( int i=0; i<256; i++ ){
            p[i] = counts[i]/S_H;
        }
        //
        /// omega = cumsum(p);
        omega[0] = p[0];
        for( int i=1; i<256; i++ ){
            omega[i] = p[i] + omega[i-1];
        }
        //
        /// p = p .* (1:num_bins)'
        for( int i=0; i<256; i++ ){
            p[i] = p[i] * (i+1);
        }
        //
        /// mu = cumsum(p)
        mu[0] = p[0];
        for( int i=1; i<256; i++ ){
            mu[i] = p[i] + mu[i-1];
        }
        mu_t = mu[255];
        //
        //
        /// sigma_b_squared = (mu_t * omega - mu).^2 ./ (omega .* (1 - omega));
        for( int i=0; i<256; i++ ){
            sigma_b_squared[i] = pow((mu_t * omega[i] - mu[i]), 2) / (omega[i] * (1 - omega[i]));
            maxval = fmax( maxval, sigma_b_squared[i] );
            if( maxval==sigma_b_squared[i] ){
                idx = i;
            }
        }
        isfinite_maxval = isfinite(maxval);
        //
        //
        double GrayValueF = 0;
        if( isfinite_maxval && maxval>=0 ){
            /// idx = mean(find(sigma_b_squared == maxval));
            // Normalize the threshold to the range [0, 1].
            GrayValueF = (idx) / (255);
        }else{
            GrayValueF = 0.0;
        }
        //
        //
        //
        /// GrayValueF;
        return GrayValueF;
    }
    /// ///////////////////////////////////////////////////////
    /// ///////////////////////////////////////////////////////
    /// ///////////////////////////////////////////////////////
    /// ///////////////////////////////////////////////////////
    /// ///////////////////////////////////////////////////////
    /// ///////////////////////////////////////////////////////
    /// ///////////////////////////////////////////////////////
    BOOL graythreshConvert( RGBQUAD* RgbIO, int LBuffIO, BOOL isgray, BOOL isbin ){
        if( isbin ){
            for( int i=0; i<LBuffIO; i++ ){
                RgbIO[i].rgbRed     =
                RgbIO[i].rgbGreen   =
                RgbIO[i].rgbBlue    = 255 * ( RgbIO[i].rgbRed>64 );
            }
            return FALSE;
        }
        double S_H = 0;
        double counts[256] = {0};
        double p[256] = {0};
        double omega[256] = {0};
        double mu[256] = {0};
        double sigma_b_squared[256] = {0};
        double mu_t = 0;
        double maxval = -1;
        double idx = -1;
        double GrayValueC = 0;
        BOOL isfinite_maxval = FALSE;
        ///
        ///
        double HistC[256] = {0};
        ///
        BYTE BValue = 0;
        if( !isgray ){
            ZeroMemory( HistC, 256 * sizeof(double) );
            for( int i=0; i<LBuffIO; i++ ){
                BValue = (BYTE)round(
                                        0.2989 * RgbIO[i].rgbRed +
                                        0.5870 * RgbIO[i].rgbGreen +
                                        0.1140 * RgbIO[i].rgbBlue
                                        );
                HistC[BValue] = HistC[BValue] + 1;
                RgbIO[i].rgbRed = BValue;
            }
        }else{
            for( int i=0; i<LBuffIO; i++ ){
                BValue = RgbIO[i].rgbRed;
                HistC[BValue] = HistC[BValue] + 1;
            }
        }
        ///
        ///
        /// counts = imhist(I,num_bins);
        memcpy( counts, HistC, 256 * sizeof(double) );
        //
        /// S_H = sum(counts)
        for( int i=0; i<256; i++ ){
            /// counts[i] = counts[i]/255;
            S_H = S_H + counts[i];
        }
        //
        ///  p = counts / sum(counts);
        for( int i=0; i<256; i++ ){
            p[i] = counts[i]/S_H;
        }
        //
        /// omega = cumsum(p);
        omega[0] = p[0];
        for( int i=1; i<256; i++ ){
            omega[i] = p[i] + omega[i-1];
        }
        //
        /// p = p .* (1:num_bins)'
        for( int i=0; i<256; i++ ){
            p[i] = p[i] * (i+1);
        }
        //
        /// mu = cumsum(p)
        mu[0] = p[0];
        for( int i=1; i<256; i++ ){
            mu[i] = p[i] + mu[i-1];
        }
        mu_t = mu[255];
        //
        //
        /// sigma_b_squared = (mu_t * omega - mu).^2 ./ (omega .* (1 - omega));
        for( int i=0; i<256; i++ ){
            sigma_b_squared[i] = pow((mu_t * omega[i] - mu[i]), 2) / (omega[i] * (1 - omega[i]));
            maxval = fmax( maxval, sigma_b_squared[i] );
            if( maxval==sigma_b_squared[i] ){
                idx = i;
            }
        }
        isfinite_maxval = isfinite(maxval);
        //
        //
        if( isfinite_maxval && maxval>=0 ){
            /// idx = mean(find(sigma_b_squared == maxval));
            // Normalize the threshold to the range [0, 1].
            GrayValueC = (idx) / (255);
        }else{
            GrayValueC = 0.0;
        }
        ///
        ///
        int BTH = (int)round( 255 * GrayValueC );
        for( int i=0; i<LBuffIO; i++ ){
            RgbIO[i].rgbRed     =
            RgbIO[i].rgbGreen   =
            RgbIO[i].rgbBlue    = 255 * ( RgbIO[i].rgbRed>BTH );
        }
        ///
        ///
        //
        //
        ///
        return TRUE;
    }
    /// ///////////////////////////////////////////////////////
    /// ///////////////////////////////////////////////////////
    /// ///////////////////////////////////////////////////////
    /// ///////////////////////////////////////////////////////
    /// ///////////////////////////////////////////////////////
    /// ///////////////////////////////////////////////////////
    ///                                                     ///
    /// ///////////////////////////////////////////////////////
    static inline BYTE ParseByte( int I_B ){
        if( I_B>255 ){
            return 255;
        }
        if( I_B<0 ){
            return 0;
        }
        return I_B;
    }
    /// ///////////////////////////////////////////////////////
    ///                                                     ///
    /// ///////////////////////////////////////////////////////
    static inline BYTE ParseByteD( double D_B ){
        if( D_B>255 ){
            return 255;
        }
        if( D_B<0 ){
            return 0;
        }
        return ( (BYTE)round(D_B) );
    }
    /// ///////////////////////////////////////////////////////
    /// ///////////////////////////////////////////////////////
    BOOL GetKernel( int* K2RetH, int* K2RetV, int KTYPE ){
        switch( KTYPE ){
            case BLUR_K:
                memcpy( K2RetH, &ConvKernelBlur[0], 9*sizeof(int) );
                break;
            case EMBOSS:
                memcpy( K2RetH, &ConvKernelEmboss[0], 9*sizeof(int) );
                break;
            case SHARPEN:
                memcpy( K2RetH, &ConvKernelSharpen[0], 9*sizeof(int) );
                break;
            case SMOOTH:
                memcpy( K2RetH, &ConvKernelSmooth[0], 9*sizeof(int) );
                break;
            case MEANRENOVAL:
                memcpy( K2RetH, &ConvKernelMEANRENOVAL[0], 9*sizeof(int) );
                break;
            case LP_MEDIA:
                memcpy( K2RetH, &ConvKernelMediaLow[0], 9*sizeof(int) );
                break;
            case LP_POND:
                memcpy( K2RetH, &ConvKernelMediaLowPon[0], 9*sizeof(int) );
                break;
            case HP_MEDIA:
                memcpy( K2RetH, &ConvKernelMediaHigh[0], 9*sizeof(int) );
                break;
            case HP_LAPLACE:
                memcpy( K2RetH, &ConvKernelLaplacHigh[0], 9*sizeof(int) );
                break;
            case HP_LAPLACE_MIN:
                memcpy( K2RetH, &ConvKernelLaplacHighMin[0], 9*sizeof(int) );
                break;
            case SOBEL:
                memcpy( K2RetH, &ConvKernelSobelH[0], 9*sizeof(int) );
                if( K2RetV!=NULL ){
                    memcpy( K2RetV, &ConvKernelSobelV[0], 9*sizeof(int) );
                }
                break;
            case PREWITT:
                memcpy( K2RetH, &ConvKernelPrewittH[0], 9*sizeof(int) );
                if( K2RetV!=NULL ){
                    memcpy( K2RetV, &ConvKernelPrewittV[0], 9*sizeof(int) );
                }
                break;
            case KIRSH:
                memcpy( K2RetH, &ConvKernelKirshH[0], 9*sizeof(int) );
                if( K2RetV!=NULL ){
                    memcpy( K2RetV, &ConvKernelKirshV[0], 9*sizeof(int) );
                }
                break;
            case USUARIO:
                memcpy( K2RetH, &Kernel3x3User[0], 9*sizeof(int) );
                break;
        }
        return TRUE;
    }
    /// ///////////////////////////////////////////////////////
    /// ///////////////////////////////////////////////////////
    /// ///////////////////////////////////////////////////////
    ///
};
#include "CONVS_2D.cpp"
#include "ARITH_PROCS.cpp"
#include "BWLABEL.cpp"
#include "BWLABEL_NEW.cpp"
#include "BWLABEL_NEW2.cpp"
#include "BWLABEL_NEW3.cpp"
#include "BWLABEL_NEW4.cpp"
///
#include "IMFILL.cpp"
///
///
///
///
///
#include "AVERAGEPROC.cpp"
///
#include "ERODEPROC.cpp"
#include "DILATEPROC.cpp"
#include "OPENPROC.cpp"
#include "CLOSEPROC.cpp"
///
#include "IMFILLPROC.cpp"
///
///
///
///




