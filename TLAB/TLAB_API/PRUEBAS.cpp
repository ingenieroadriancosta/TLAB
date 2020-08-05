BOOL TLAB::PRUEBAS( void ){
    return TRUE;
    char T2T[1024] = {0};
    mat_t*              matfp = NULL;
    matvar_t*           matvar = NULL;
    size_t              dims[2] = { 0, 0 };
    matfp = (mat_t*)DSP_MNG.Mat_Open( "E:\\AIIF\\SegAll\\CONOCIMIENTO\\UNIVERSIDAD\\TESIS\\P_NUEVO\\APLICACIONES\\TLAB\\DSP_LAB\\ARCHIVOS_DE_PRUEBA\\AAAAMATSALL.mat", 0 );
    if( matfp==NULL ){
        return FALSE;
    }
    ///
    dims[0] = (size_t)1;dims[1] = (size_t)1;
    for( int i=0; i<MaxProcs; i++ ){
        sprintf( T2T, "y%i", i );
        Dim1Procs[i].ReadDatas();
        dims[1] = Dim1Procs[i].NPosBuff;
        matvar = (matvar_t *)DSP_MNG.Mat_VarCreate( T2T, MAT_C_DOUBLE, MAT_T_DOUBLE, 2, dims, Dim1Procs[i].DBuff, 0 );
        DSP_MNG.Mat_VarWrite(matfp,matvar,MAT_COMPRESSION_NONE);
        DSP_MNG.Mat_VarFree(matvar);
        Dim1Procs[i].FreeBuff();
    }
    ///
    return TRUE;
}
