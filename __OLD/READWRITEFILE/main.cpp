#include <windows.h>
#include <stdio.h>
int main(){
    BYTE BBuff[1024] = {0};
    DWORD BWR = 0;
    /// FILE_ATTRIBUTE_HIDDEN
    HANDLE HFile = CreateFile( "00AA.dsm", GENERIC_WRITE|GENERIC_READ, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL );
    if( HFile==NULL ){
        printf( "\n\nHFile==NULL\n\n\n" );
        return -1;
    }
    BWR = 0;
    for( int i=0; i<9; i++ ){
        BBuff[i] = i;
    }
    if( !WriteFile( HFile, BBuff, 9, &BWR, NULL ) ){
        return -2;
    }
    ZeroMemory( BBuff, 1024 );
    SetFilePointer( HFile, 0, NULL, FILE_BEGIN );
    BWR = 0;
    if( !ReadFile( HFile, BBuff, 128, &BWR, NULL ) ){
        return -2;
    }
    printf( "BWR = %i\n\n", (int)BWR );
    for( int i=0; i<10; i++ ){
        printf( "%i\t", BBuff[i] );
    }
    ///
    ///
    ///
    ///
    printf( "\n\n\nFin de programa!!!." );
    system( "pause>nul" );
    return 0;
}









