BOOL WriteError( LPCSTR LPS );
LONG WINAPI OurCrashHandler(EXCEPTION_POINTERS * exceptionInfo ){
    char T2T_T[1024] = {0};
    sprintf( T2T_T, "An exception occurred which wasn't handled!\nCode: 0x%08X\nAddress: 0x%08X",
             (int)exceptionInfo->ExceptionRecord->ExceptionCode,
             (int)exceptionInfo->ExceptionRecord->ExceptionAddress);
    MessageBoxA( NULL, T2T_T, " Error inesperado ", 16 );
    exit(0);
    return EXCEPTION_CONTINUE_EXECUTION;//g_showCrashDialog ? EXCEPTION_CONTINUE_SEARCH : EXCEPTION_EXECUTE_HANDLER;
}//EXCEPTION_CONTINUE_EXECUTION
/// ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
VOID* GetNull(){
    static BOOL ExcSeted = FALSE;
    if( !ExcSeted ){
        //SetUnhandledExceptionFilter(OurCrashHandler);
        ExcSeted = TRUE;
        /*
        const size_t sizeT = (size_t)( 1.8*(1024 * 1024) * 1024 );
        HANDLE mapping = CreateFileMapping(INVALID_HANDLE_VALUE,
                          NULL, PAGE_READWRITE,
                          0u, -1, NULL);
        if (mapping == NULL){
            exit( -1 );
        }
        const size_t length =  sizeT;
        void* region = MapViewOfFile(mapping, FILE_MAP_ALL_ACCESS, 0u, 0u, length);
        if (region == NULL){
            exit( -2 );
        }
        UnmapViewOfFile(region);
        ///
        SYSTEM_INFO si;
 GetSystemInfo(&si);
 DWORD chunkOffset = offset % si.dwAllocationGranularity;
 DWORD chunkStart = offset - chunkOffset;
 LPBYTE pb = (LPBYTE*)MapViewOfFile(h, FILE_MAP_READ, 0,
      chunkStart, chunkOffset + sizeof(BYTE));
 BYTE b = pb[chunkOffset];
 UnmapViewOfFile(pb);
        ///
        //*/
    }
    return NULL;
}
