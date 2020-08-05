BOOL TLAB::SaveRestFile( void ){
    ///
    mat_t*              matfp = NULL;
    matvar_t*           matvar = NULL;
    size_t              dims[2] = { 0, 0 };
    BYTE*               BPTR = NULL;
    char                T2T[1024];
    ZeroMemory( T2T, 1023 );
    sprintf( T2T, "%sTLABFRES", DSP_MNG.GetDSPPath() );
    /// MsgBoxF( T2T );
    matfp = (mat_t*)DSP_MNG.Mat_Open(T2T, 0);
    if( matfp==NULL ){
        return FALSE;
    }
    ///
    dims[0] = (size_t)1;dims[1] = (size_t)1;
    BPTR = (BYTE*)(&AlwOnTop);
    matvar = (matvar_t *)DSP_MNG.Mat_VarCreate( "AlwOnTop", MAT_C_INT32, MAT_T_INT32, 2, dims, BPTR, 0 );
    DSP_MNG.Mat_VarWrite(matfp,matvar,MAT_COMPRESSION_NONE);
    DSP_MNG.Mat_VarFree(matvar);
    ///
    ///
    ///
    ///
    ///
    ///
    ///
    dims[0] = (size_t)1;dims[1] = (size_t)1;
    BPTR = (BYTE*)(&TabPage);
    matvar = (matvar_t *)DSP_MNG.Mat_VarCreate( "TabPage", MAT_C_INT32, MAT_T_INT32, 2, dims, BPTR, 0 );
    DSP_MNG.Mat_VarWrite(matfp,matvar,MAT_COMPRESSION_NONE);
    DSP_MNG.Mat_VarFree(matvar);
    ///
    ///
    ///
    ///
    ///
    ///
    ///
    dims[0] = (size_t)1;dims[1] = (size_t)1;
    BPTR = (BYTE*)(&SetPlotimshow);
    matvar = (matvar_t *)DSP_MNG.Mat_VarCreate( "SetPlotimshow", MAT_C_INT32, MAT_T_INT32, 2, dims, BPTR, 0 );
    DSP_MNG.Mat_VarWrite(matfp,matvar,MAT_COMPRESSION_NONE);
    DSP_MNG.Mat_VarFree(matvar);
    ///
    ///
    ///
    ///
    dims[0] = 1;
    for( int i=0; i<MaxProcs; i++ ){
        Dim1Procs[i].IsLoaded = FALSE;
        Dim1Procs[i].FreeBuff();
        Dim1Procs[i].ClosePath();
        Dim1Procs[i].FreeAll();
        Dim1Procs[i].FDS.Free();
        BPTR = (BYTE*)(&Dim1Procs[i]);
        dims[1] = sizeof(DIM1);
        sprintf( T2T, "DIM1_N_%i", i );
        matvar = (matvar_t *)DSP_MNG.Mat_VarCreate( T2T, MAT_C_UINT8, MAT_T_UINT8, 2, dims, BPTR, 0 );
        DSP_MNG.Mat_VarWrite(matfp,matvar,MAT_COMPRESSION_NONE);
        DSP_MNG.Mat_VarFree(matvar);
    }
    ///
    ///
    dims[0] = 1;
    for( int i=0; i<MaxProcs; i++ ){
        Dim2Procs[i].IsLoaded = FALSE;
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
    ///
    DSP_MNG.Mat_Close( matfp );
    ///
    return TRUE;
}
/// ////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// ////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// ////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// ////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// ////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// ////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL TLAB::ReadRestFile( void ){
    ///
    ///
    mat_t*              matfp = NULL;
    matvar_t*           matvar = NULL;
    BYTE*               BPTR = NULL;
    char*               T2T = (char*)malloc( 10 * 1024 );
    ZeroMemory( T2T, 10 * 1024 - 1 );
    SetCurrentDirectoryA( DSP_MNG.GetDSPPath() );
    sprintf( T2T, "%sTLABFRES", DSP_MNG.GetDSPPath() );
    matfp = (mat_t*)DSP_MNG.Mat_Open( T2T, 1 );// MAT_ACC_RDWR
    if( matfp==NULL ){
        return FALSE;
    }
    //MsgBoxF( "1" );
    ///
    ///
    matvar = (matvar_t *)DSP_MNG.Mat_VarRead( matfp, "AlwOnTop" );
    if( matvar->data_size!=4 || matvar->nbytes!=4 ||
        matvar->dims[0]!=1 || matvar->dims[1]!=1 ||
                    matvar->data_type!=MAT_T_INT32 || matvar->class_type!=MAT_C_INT32 ){
        DSP_MNG.Mat_Close( matfp );
        DSP_MNG.Mat_VarFree(matvar);
        return FALSE;
    }
    BPTR = (BYTE*)matvar->data;
    AlwOnTop = ((BOOL*)BPTR)[0];
    DSP_MNG.Mat_VarFree(matvar);
    ///
    ///
    ///
    ///
    ///
    matvar = (matvar_t *)DSP_MNG.Mat_VarRead( matfp, "SetPlotimshow" );
    if( matvar->data_size!=4 || matvar->nbytes!=4 ||
        matvar->dims[0]!=1 || matvar->dims[1]!=1 ||
                    matvar->data_type!=MAT_T_INT32 || matvar->class_type!=MAT_C_INT32 ){
        DSP_MNG.Mat_Close( matfp );
        DSP_MNG.Mat_VarFree(matvar);
        return FALSE;
    }
    BPTR = (BYTE*)matvar->data;
    SetPlotimshow = ((BOOL*)BPTR)[0];
    DSP_MNG.Mat_VarFree(matvar);
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
    DIM1* D1PTR = NULL;
    for( int i=0; i<MaxProcs; i++ ){
        sprintf( T2T, "DIM1_N_%i", i );
        matvar = (matvar_t*)DSP_MNG.Mat_VarRead( matfp, T2T );
        if( matvar==NULL ){
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
                break;
            }
        }
        BPTR = (BYTE*)matvar->data;
        memcpy( &Dim1Procs[i], BPTR, matvar->nbytes );
        DSP_MNG.Mat_VarFree(matvar);
    }
    ///
    ///
    ///
    DIM2* D2PTR = NULL;
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
        BPTR = (BYTE*)matvar->data;
        memcpy( &Dim2Procs[i], BPTR, matvar->nbytes );
        DSP_MNG.Mat_VarFree(matvar);
    }
    ///
    ///
    ///
    ///
    matvar = (matvar_t *)DSP_MNG.Mat_VarRead( matfp, "TabPage" );
    if( matvar==NULL ){
        DSP_MNG.Mat_Close( matfp );
        return FALSE;
    }
    if( matvar->data_size!=4 || matvar->nbytes!=4 ||
        matvar->dims[0]!=1 || matvar->dims[1]!=1 ||
                    matvar->data_type!=MAT_T_INT32 || matvar->class_type!=MAT_C_INT32 ){
        DSP_MNG.Mat_Close( matfp );
        DSP_MNG.Mat_VarFree(matvar);
        return FALSE;
    }
    BPTR = (BYTE*)matvar->data;
    TabPage = ((int*)BPTR)[0];
    DSP_MNG.Mat_VarFree(matvar);
    ///
    ///
    ///
    ///
    ///
    DSP_MNG.Mat_Close( matfp );
    ///
    return TRUE;
}
