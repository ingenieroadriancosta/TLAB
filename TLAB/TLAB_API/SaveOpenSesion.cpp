BOOL TLAB::SaveSesion1D( LPCSTR FileName ){
    ///
    hwndC = HW_MAIN;
    HWProgress = NULL;
    DWORD IdTh = 0;
    CreateThread( NULL, 0, ThreadCall_Progress, (PVOID)&IPtr, 0, &IdTh );
    Sleep( 100 );
    while( HWProgress==NULL ){
        Sleep( 10 );
    }
    SetWindowTextA( HWProgress, " Guardando Sesión " );
    ///
    mat_t*              matfp = NULL;
    matvar_t*           matvar = NULL;
    size_t              dims[2] = { 0, 0 };
    BYTE*               BPTR = NULL;
    ///
    char*               T2T = (char*)malloc( 1024 );
    ZeroMemory( T2T, 1023 );
    ///
    matfp = (mat_t*)DSP_MNG.Mat_Open(FileName, 0);
    if( matfp==NULL ){
        free( T2T );
        SendMessageA( HWProgress, WM_CLOSE, 0, 0 );
        return FALSE;
    }
    ///
    ///
    ///
    ///
    ///
    dims[0] = 1;
    for( int i=0; i<MaxProcs; i++ ){
        if( i!=0 ){
            Dim1Procs[i].IsLoaded = FALSE;
        }
        Dim1Procs[i].ClosePath();
        Dim1Procs[i].FreePicPlot();
        Dim1Procs[i].FreeAll();
        Dim1Procs[i].FreeBuff();
        Dim1Procs[i].FDS.Free();
        BPTR = (BYTE*)(&Dim1Procs[i]);
        dims[1] = sizeof(DIM1);
        sprintf( T2T, "DIM1_N_%i", i );
        matvar = (matvar_t *)DSP_MNG.Mat_VarCreate( T2T, MAT_C_UINT8, MAT_T_UINT8, 2, dims, BPTR, 0 );
        DSP_MNG.Mat_VarWrite(matfp,matvar,MAT_COMPRESSION_NONE);
        DSP_MNG.Mat_VarFree(matvar);
    }
    ///
    DSP_MNG.Mat_Close( matfp );
    free( T2T );
    ///
    ///
    ///
    for( int i=0; i<MaxProcs; i++ ){
        Dim1Procs[i].Ind_File = i;
        Dim1Procs[i].ClosePath();
        Dim1Procs[i].SetPath( DSP_MNG.GetDSPPath() );
    }
    ///
    Dim1Procs[0].LoadSignal( HW_MAIN );
    Dim1Procs[0].ReadDatas();
    Pltrs.SetMaxXLim(
                     Dim1Procs[0].NPosBuff/( (double)Dim1Procs[0].SLOAD.Fs ),
                     TRUE );
    Pltrs.PLOTDOUBLE_BIPOL( Dim1Procs[0].DBuff, Dim1Procs[0].NPosBuff, 0, TRUE );
    Dim1Procs[0].FreeBuff();
    Dim1Procs[0].SetPicPlot( Pltrs.GetBitmap() );
    Dim1Procs[0].FreeBuff();
    ///
    SendMessageA( HWProgress, WM_CLOSE, 0, 0 );
    ///
    return TRUE;
}
/// ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL TLAB::SaveSesion2D( LPCSTR FileName ){
    ///
    hwndC = HW_MAIN;
    HWProgress = NULL;
    DWORD IdTh = 0;
    CreateThread( NULL, 0, ThreadCall_Progress, (PVOID)&IPtr, 0, &IdTh );
    Sleep( 100 );
    while( HWProgress==NULL ){
        Sleep( 10 );
    }
    SetWindowTextA( HWProgress, " Guardando Sesión " );
    ///
    mat_t*              matfp = NULL;
    matvar_t*           matvar = NULL;
    size_t              dims[2] = { 0, 0 };
    BYTE*               BPTR = NULL;
    ///
    char*               T2T = (char*)malloc( 1024 );
    ZeroMemory( T2T, 1023 );
    ///
    matfp = (mat_t*)DSP_MNG.Mat_Open(FileName, 0);
    if( matfp==NULL ){
        free( T2T );
        SendMessageA( HWProgress, WM_CLOSE, 0, 0 );
        return FALSE;
    }
    ///
    dims[0] = 1;
    for( int i=0; i<MaxProcs; i++ ){
        if( i!=0 ){
            Dim2Procs[i].IsLoaded = FALSE;
        }
        Dim2Procs[i].ClosePath();
        Dim2Procs[i].FreeAll();
        BPTR = (BYTE*)(&Dim2Procs[i]);
        dims[1] = sizeof(DIM2);
        sprintf( T2T, "DIM2_N_%i", i );
        matvar = (matvar_t *)DSP_MNG.Mat_VarCreate( T2T, MAT_C_UINT8, MAT_T_UINT8, 2, dims, BPTR, 0 );
        DSP_MNG.Mat_VarWrite(matfp,matvar,MAT_COMPRESSION_NONE);
        DSP_MNG.Mat_VarFree(matvar);
    }
    ///
    DSP_MNG.Mat_Close( matfp );
    free( T2T );
    ///
    ///
    ///
    ///
    ///
    for( int i=0; i<MaxProcs; i++ ){
        Dim2Procs[i].Ind_File = i;
        Dim2Procs[i].HFileMod = NULL;
        Dim2Procs[i].SetPath( DSP_MNG.GetDSPPath() );
    }
    ///
    ///
    ///
    Dim2Procs[0].PreloadImage();
    if( Dim2Procs[0].IsGray ){
        Dim2Procs[0].IsBin  = FALSE;
        int NPosP = Dim2Procs[0].WPIC_IN * Dim2Procs[0].HPIC_IN;
        Dim2Procs[0].ReadDatas();
        for( int i=0; i<NPosP; i++ ){
            Dim2Procs[0].RgbBuff[i].rgbRed =
            Dim2Procs[0].RgbBuff[i].rgbGreen =
            Dim2Procs[0].RgbBuff[i].rgbBlue =
                (BYTE)round(
                            0.2989 * Dim2Procs[0].RgbBuff[i].rgbRed +
                            0.5870 * Dim2Procs[0].RgbBuff[i].rgbGreen +
                            0.1140 * Dim2Procs[0].RgbBuff[i].rgbBlue
                            );
        }
        Dim2Procs[0].WriteDatas();
        SetBitmapBits( Dim2Procs[0].HBMP_OUT, Dim2Procs[0].NBytesBuff, Dim2Procs[0].RgbBuff );
    }
    SendMessage( HW_IMSHOW, STM_SETIMAGE, IMAGE_BITMAP,
                        (LPARAM)Dim2Procs[0].SetPicPlot( W_BOUT, H_BOUT ) );
    Dim2Procs[0].FreeImageIn();
    Dim2Procs[0].FreeImage();
    Dim2Procs[0].FreeBuff();
    Dim2Procs[0].IsLoaded = TRUE;
    Dim2Procs[0].ProcType = -2;
    ///
    SendMessageA( HWProgress, WM_CLOSE, 0, 0 );
    ///
    return TRUE;
}
/// ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL TLAB::OpenSesion1D( LPCSTR FileName ){
    ///
    ///
    mat_t*              matfp = NULL;
    matvar_t*           matvar = NULL;
    BYTE*               BPTR = NULL;
    ///
    char*               T2T = (char*)malloc( 1024 );
    ZeroMemory( T2T, 1023 );
    BOOL IsSesion1D = TRUE;
    ///
    matfp = (mat_t*)DSP_MNG.Mat_Open(FileName, 1);
    if( matfp==NULL ){
        free( T2T );
        return FALSE;
    }
    ///
    ///
    ///
    DIM1* D1PTR = NULL;
    for( int i=0; i<MaxProcs; i++ ){
        sprintf( T2T, "DIM1_N_%i", i );
        matvar = (matvar_t*)DSP_MNG.Mat_VarRead( matfp, T2T );
        if( matvar==NULL ){
            IsSesion1D = FALSE;
            break;
        }

        if( matvar->nbytes<37240 ){
            DSP_MNG.Mat_VarFree(matvar);
            break;
        }
        D1PTR = (DIM1*)matvar->data;
        if( strcmp( D1PTR->DReserved, "D1R" )!=0 ){
            if( matvar->nbytes!=sizeof(DIM1) || matvar->class_type!=MAT_C_UINT8 ||
                                matvar->data_type!=MAT_T_UINT8 ){
                DSP_MNG.Mat_VarFree(matvar);
                IsSesion1D = FALSE;
                break;
            }
        }
        DSP_MNG.Mat_VarFree(matvar);
    }
    ///
    ///
    if( !IsSesion1D ){
        DSP_MNG.Mat_Close( matfp );
        free( T2T );
        return FALSE;
    }
    ///
    ///
    ///
    //*
    for( int i=0; i<MaxProcs; i++ ){
        sprintf( T2T, "DIM1_N_%i", i );
        matvar = (matvar_t*)DSP_MNG.Mat_VarRead( matfp, T2T );
        if( matvar==NULL ){
            break;
        }
        if( matvar->nbytes!=sizeof(DIM1) || matvar->class_type!=MAT_C_UINT8 ||
                            matvar->data_type!=MAT_T_UINT8 ){
            /// MsgBoxF( "data_size = %i", matvar->data_size );
            DSP_MNG.Mat_VarFree(matvar);
            break;
        }
        Dim1Procs[i].ClosePath();
        Dim1Procs[i].FreeAll();
        Dim1Procs[i].FreeBuff();
        Dim1Procs[i].FDS.Free();
        BPTR = (BYTE*)matvar->data;
        memcpy( &Dim1Procs[i], BPTR, matvar->nbytes );
        DSP_MNG.Mat_VarFree(matvar);
    }
    //*/
    ///
    ///
    DSP_MNG.Mat_Close( matfp );
    free( T2T );
    ///
    ///
    ///
    ///
    for( int i=0; i<MaxProcs; i++ ){
        if( i==0 ){
            Dim1Procs[i].IsLoaded = FALSE;
        }else{
            B_1D[i].SetPInit = Dim1Procs[i].FDS.GetBInput()>0;
            B_1D[i].SetPicOn( Dim1Procs[i].ProcType + PLOT_PNG, FILE_RES );
            ///
            B_2D[i].SetPInit = Dim2Procs[i].ImageInP>0;
            B_2D[i].SetPicOn( Dim2Procs[i].ProcType + IMSHOW, FILE_RES );
        }
    }
    ///
    ///
    for( int i=0; i<MaxProcs; i++ ){
        Dim1Procs[i].Ind_File = i;
        Dim1Procs[i].ClosePath();
        Dim1Procs[i].SetPath( DSP_MNG.GetDSPPath() );
        ///
        ///
        if( i==0 ){
            Dim1Procs[i].ProcType = -2;
            Dim1Procs[i].D1_IN = NULL;
            Dim1Procs[i].D1_INIT = NULL;
        }else{
            Dim1Procs[i].D1_IN   = &Dim1Procs[i-1];
            Dim1Procs[i].D1_INIT = &Dim1Procs[0];
            if( Dim1Procs[i].FDS.GetBInput()>0 ){
                B_1D[i].SetPInit = TRUE;
                B_1D[i].SetOn( NULL, FALSE );
            }
            B_1D[i].Enable( FALSE );
        }
        ///
    }
    ///
    ///
    ///
    ///
    return TRUE;
}

/// ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL TLAB::OpenSesion2D( LPCSTR FileName ){
    ///
    ///
    mat_t*              matfp = NULL;
    matvar_t*           matvar = NULL;
    BYTE*               BPTR = NULL;
    ///
    char*               T2T = (char*)malloc( 1024 );
    ZeroMemory( T2T, 1023 );
    BOOL IsSesion2D = TRUE;
    ///
    matfp = (mat_t*)DSP_MNG.Mat_Open(FileName, 1);
    if( matfp==NULL ){
        free( T2T );
        return FALSE;
    }
    ///
    ///
    ///
    DIM2* D2PTR = NULL;
    for( int i=0; i<MaxProcs; i++ ){
        sprintf( T2T, "DIM2_N_%i", i );
        matvar = (matvar_t*)DSP_MNG.Mat_VarRead( matfp, T2T );
        if( matvar==NULL ){
            IsSesion2D = FALSE;
            break;
        }
        ///
        if( matvar->nbytes<11544 ){
            DSP_MNG.Mat_VarFree(matvar);
            break;
        }
        ///
        D2PTR = (DIM2*)matvar->data;
        ///
        if( strcmp( D2PTR->DReserved, "D2R" )!=0 ){
            if( matvar->nbytes!=sizeof(DIM2) || matvar->class_type!=MAT_C_UINT8 ||
                                matvar->data_type!=MAT_T_UINT8 ){
                DSP_MNG.Mat_VarFree(matvar);
                IsSesion2D = FALSE;
                break;
            }
        }
        DSP_MNG.Mat_VarFree(matvar);
    }
    ///
    ///
    if( !IsSesion2D ){
        DSP_MNG.Mat_Close( matfp );
        free( T2T );
        return FALSE;
    }
    ///
    ///
    ///
    //*
    for( int i=0; i<MaxProcs; i++ ){
        sprintf( T2T, "DIM2_N_%i", i );
        matvar = (matvar_t*)DSP_MNG.Mat_VarRead( matfp, T2T );
        if( matvar==NULL ){
            break;
        }
        ///
        if( matvar->nbytes<11544 ){
            DSP_MNG.Mat_VarFree(matvar);
            break;
        }
        ///
        D2PTR = (DIM2*)matvar->data;
        ///
        if( strcmp( D2PTR->DReserved, "D2R" )!=0 ){
            if( matvar->nbytes!=sizeof(DIM2) || matvar->class_type!=MAT_C_UINT8 ||
                                matvar->data_type!=MAT_T_UINT8 ){
                DSP_MNG.Mat_VarFree(matvar);
                break;
            }
        }
        Dim2Procs[i].ClosePath();
        Dim2Procs[i].FreeAll();
        BPTR = (BYTE*)matvar->data;
        memcpy( &Dim2Procs[i], BPTR, matvar->nbytes );
        DSP_MNG.Mat_VarFree(matvar);
    }
    //*/
    ///
    ///
    DSP_MNG.Mat_Close( matfp );
    free( T2T );
    ///
    ///
    ///
    ///
    for( int i=0; i<MaxProcs; i++ ){
        ///
        ///
        ///
        Dim2Procs[i].Ind_File = i;
        Dim2Procs[i].HFileMod = NULL;
        Dim2Procs[i].SetPath( DSP_MNG.GetDSPPath() );
        ///
        Dim2Procs[i].D2_SOURCE = &Dim2Procs[0];
        if( i==0 ){
            Dim2Procs[i].Dim_In = NULL;
            Dim2Procs[i].D2_SOURCE = NULL;
            Dim2Procs[i].IsLoaded = FALSE;
        }else{
            B_2D[i].SetPInit = Dim2Procs[i].ImageInP>0;
            Dim2Procs[i].SetInput( &Dim2Procs[i-1] );
            Dim2Procs[i].D2_SOURCE = &Dim2Procs[0];
            B_2D[i].SetOn( NULL, FALSE );
            B_2D[i].Enable( FALSE );
            B_2D[i].SetPicOn( Dim2Procs[i].ProcType + IMSHOW, FILE_RES );
            ///
        }
        ///
    }
    ///
    ///
    ///
    ///
    ///
    return TRUE;
}
