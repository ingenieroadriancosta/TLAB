BOOL TLAB::WRITE_ERROR_REPORT(){
    char FNAME[1024] = {0};
    DWORD BWri = 0;
    sprintf( FNAME, "%sErrorReport.txt", DSP_MNG.GetDSPPath() );
    HANDLE HFile = CreateFile( FNAME, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL );
    ///
    /// MsgBoxF( FNAME );
    ///
    ///
    sprintf( FNAME, "Error report:\n\n" );
    WriteFile( HFile, FNAME, strlen(FNAME), &BWri, NULL );
    ///
    sprintf( FNAME, "TabPage = %i\n\n", TabPage );
    WriteFile( HFile, FNAME, strlen(FNAME), &BWri, NULL );
    ///
    sprintf( FNAME, "\n1D\nFNAME %s\n", Dim1Procs[0].SLOAD.FNameT );
    WriteFile( HFile, FNAME, strlen(FNAME), &BWri, NULL );
    ///
    sprintf( FNAME, "NPos = %i\n", Dim1Procs[0].SLOAD.LOfVarT );
    WriteFile( HFile, FNAME, strlen(FNAME), &BWri, NULL );
    ///
    sprintf( FNAME, "ProcTypes \n" );
    WriteFile( HFile, FNAME, strlen(FNAME), &BWri, NULL );
    for( int i=0; i<MaxProcs; i++ ){
        sprintf( FNAME, "%i ", Dim1Procs[i].ProcType );
        WriteFile( HFile, FNAME, strlen(FNAME), &BWri, NULL );
    }
    ///
    sprintf( FNAME, "\n\n\n\n2D\n" );
    WriteFile( HFile, FNAME, strlen(FNAME), &BWri, NULL );
    ///
    ///
    ///
    ///
    ///
    sprintf( FNAME, "\n\nFNAME %s\n", Dim2Procs[0].FNameT );
    WriteFile( HFile, FNAME, strlen(FNAME), &BWri, NULL );
    ///
    sprintf( FNAME, "WxH = %ix%i\n", Dim2Procs[0].WPIC_IN, Dim2Procs[0].HPIC_IN );
    WriteFile( HFile, FNAME, strlen(FNAME), &BWri, NULL );
    ///
    sprintf( FNAME, "ProcTypes \n" );
    WriteFile( HFile, FNAME, strlen(FNAME), &BWri, NULL );
    for( int i=0; i<MaxProcs; i++ ){
        sprintf( FNAME, "%i ", Dim2Procs[i].ProcType );
        WriteFile( HFile, FNAME, strlen(FNAME), &BWri, NULL );
    }
    ///
    ///
    ///
    sprintf( FNAME, "\n\n\n\n\n" );
    WriteFile( HFile, FNAME, strlen(FNAME), &BWri, NULL );
    ///
    ///
    CloseHandle( HFile );
    ///
    return TRUE;
}
