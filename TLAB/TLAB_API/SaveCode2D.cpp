BOOL TLAB::SaveCode2D( void ){
    static char FNAME[1024] = {0};
    static char PATH[1024] = {0};
    static char TFT[1024] = {0};
    if( !Dim2Procs[0].IsLoaded ){
        ///MsgBoxF( "\n\nNo hay una señal cargada!!!\n\nHaga doble click en el boton habilitado.\n\n" );return FALSE;
    }
    ///
    if( !SaveFileAs( FNAME, ".M Files (*.m*)\0*.m\0", HW_MAIN ) ){
        return FALSE;
    }
    if( FNAME[strlen(FNAME)-1]!='m' || FNAME[strlen(FNAME)-2]!='.' ){
        MessageBoxA( HW_MAIN, "El formato del archivo no es válido.", " Error ", 16 );
        return FALSE;
    }
    ///
    strcpy( PATH, FNAME );
    PathRemoveFileSpecA( PATH );
    strcat( PATH, "\\" );
    GetFileTitleA( FNAME, TFT, 1023 );
    ///
    BOOL SetRetu = FALSE;
    if( !( TFT[0]>=97 && TFT[0]<=122 ) && !( TFT[0]>=65 && TFT[0]<=90 ) ){
        SetRetu = TRUE;
    }
    for( int i=1; i<(int)strlen(TFT)-2; i++ ){
        if( !( TFT[i]>=97 && TFT[i]<=122 ) && !( TFT[i]>=65 && TFT[i]<=90 ) ){
            if( ( TFT[i]>=48 && TFT[i]<=57 ) ){
                continue;
            }
            if( TFT[i]!='_' || i==0 ){
                SetRetu = TRUE;
                break;
            }
        }
    }
    ///
    if( SetRetu ){
        MessageBoxA( HW_MAIN,
                     "\nEl nombre del archivo no es válido.\n"
                     "El nombre del archivo debe comenzar con letras mayúsculas ó minúsculas simples.\n\n",
                     " Error ", 16 );
        return FALSE;
    }
    ///
    char* T2T = (char*)malloc( 1024 * 1024 );
    ZeroMemory( T2T, 1024 * 1024 - 1 );
    char* TextT = (char*)malloc( 1024 * 1024 );
    ZeroMemory( TextT, 1024 * 1024 - 1 );
    ///
    const char* T2DLG =
                    "\nAdvetencia:\n\n"
                    "Se escribirá el código de procesamiento de cada módulo en lenguaje de programación MATLAB "
                    //" y otros archivos de funciones con extensión (.m) adicionales, "
                    "elija una carpeta vacia para que no pierda trabajos anteriores.\n\n"
                    "Los archivos que se crearán son:\n"
                    ///
                    "%s\n"
                    ""
                    "\n\n¿Desea continuar?\n\n";
    sprintf( T2T, T2DLG, TFT );
    if( MessageBoxA( HW_MAIN, T2T, " ", MB_ICONEXCLAMATION | MB_YESNO )!=IDYES ){
        free( T2T );
        free( TextT );
        return FALSE;
    }
    ///
    HANDLE HFile = CreateFile( FNAME, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL );
    DWORD LERR = GetLastError();
    if( HFile==NULL  || (LERR!=0 && LERR!=183) ){
        free( T2T );
        free( TextT );
        if( LERR==32 ){
            MessageBoxA( HW_MAIN, "El archivo está siendo usado por otra aplicación.", " Error ", 16 );
        }else{
            MessageBoxA( HW_MAIN, "Error al crear el archivo.", " Error ", 16 );
        }
        return FALSE;
    }
    ///
    ///
    ///
    ///
    TFT[strlen(TFT)-2] = 0;
    ///
    T2T[0] = 0;
    sprintf( T2T, "function %s\n\n", TFT );
    BOOL writekrish = FALSE;
    #define compcpy2d 0
    #ifdef compcpy2d
    CopyCode2D( T2T, &writekrish );
    #else
    ///strcat( T2T, TextT );
    strcat( TextT, "clc;clear all;tic;\n\n" );
    strcat( T2T, TextT );
    sprintf( TextT, "y0 = imread('%s');\n\n", Dim2Procs[0].FName );
    strcat( T2T, TextT );
    if( Dim2Procs[0].IsGray ){
        sprintf( TextT, "if ndims(y0)==3\n"
                            "\ty0 = rgb2gray(y0);\n"
                        "end\n" );
        strcat( T2T, TextT );
    }
    int i_from = 0;
    ///
    ///
    for( int i=1; i<MaxProcs; i++ ){
        i_from = i-1;
        while( Dim2Procs[i_from].ProcType==HIST_2DPROC ||
              Dim2Procs[i_from].ProcType==NO_PROC_Signal_2D || Dim2Procs[i_from].ProcType==SHOW_2D ){
            i_from--;
        }
        if( Dim2Procs[i].ImageInP>0 ){
            i_from = 0;
        }
        switch( Dim2Procs[i].ProcType ){
            case SHOW_2D:{
                sprintf( TextT, "%%Procesamiento del módulo %i(Mostrar imagen)\n", i );
                strcat( T2T, TextT );
                strcat( T2T, "figure\n" );
                sprintf( TextT, "imshow(y%i);\n", i_from );
                strcat( T2T, TextT );
                sprintf( TextT, "title('Módulo %i');\ndrawnow;\n", i );
                strcat( T2T, TextT );
                strcat( T2T, "\n\n" );
            }
                break;
            /// //////////////////////////////////////////////////////////////////////////////////////////////////
            /// //////////////////////////////////////////////////////////////////////////////////////////////////
            /// //////////////////////////////////////////////////////////////////////////////////////////////////
            case RGB2GRAY_2DPROC:{
                sprintf( TextT, "%%Procesamiento del módulo %i(Convertir a escala de grises.)\n", i );
                strcat( T2T, TextT );
                if( Dim2Procs[i_from].IsGray ){
                    sprintf( TextT, "y%i = y%i; %%La imagen está en escala de grises.)\n", i, i_from );
                    strcat( T2T, TextT );
                }else{
                    sprintf( TextT, "if ndims(y%i)==3\n"
                                  "\ty%i = rgb2gray(y%i);\n"
                                  "else\n"
                                  "\ty%i = y%i;\n"
                                  "end\n", i_from, i, i_from, i, i_from );
                    strcat( T2T, TextT );
                }
                strcat( T2T, "figure\n" );
                sprintf( TextT, "imshow(y%i);\n", i );
                strcat( T2T, TextT );
                sprintf( TextT, "title('Módulo %i');\ndrawnow;\n", i );
                strcat( T2T, TextT );
                strcat( T2T, "\n\n" );
            }
                break;
            /// //////////////////////////////////////////////////////////////////////////////////////////////////
            /// //////////////////////////////////////////////////////////////////////////////////////////////////
            /// //////////////////////////////////////////////////////////////////////////////////////////////////
            case BINARY_2DPROC:{
                sprintf( TextT, "%%Procesamiento del módulo %i(Binarizar imagen.)\n", i );
                strcat( T2T, TextT );
                if( Dim2Procs[i_from].IsBin ){
                    sprintf( TextT, "y%i = y%i;%%La imagen ya es binaria.\n", i, i_from );
                }else{
                    sprintf( TextT, "y%i = im2bw( y%i, %g );\n", i, i_from, ((double)Dim2Procs[i].LevelBIN)/255 );
                }
                strcat( T2T, TextT );
                ///
                strcat( T2T, "figure\n" );
                sprintf( TextT, "imshow(y%i);\n", i );
                strcat( T2T, TextT );
                sprintf( TextT, "title('Módulo %i');\ndrawnow;\n", i );
                strcat( T2T, TextT );
                strcat( T2T, "\n\n" );
            }
                break;
            /// //////////////////////////////////////////////////////////////////////////////////////////////////
            /// //////////////////////////////////////////////////////////////////////////////////////////////////
            /// //////////////////////////////////////////////////////////////////////////////////////////////////
            case CONV_3x3_2DPROC:{
                sprintf( TextT, "%%Procesamiento del módulo %i(Convolución 3x3.)\n", i );
                strcat( T2T, TextT );
                if( Dim2Procs[i_from].IsBin ){
                    sprintf( TextT, "y%i = 255 * double(y%i);\n", i, i_from );
                }else{
                    if( Dim2Procs[i_from].IsGray ){
                        sprintf( TextT, "y%i = uint8(y%i);\n", i, i_from );
                    }else{
                        sprintf( TextT, "y%i = rgb2gray(y%i);\n", i, i_from );
                    }
                }
                strcat( T2T, TextT );
                if( Dim2Procs[i].Kernel3x3_Type==BLUR_K || Dim2Procs[i].Kernel3x3_Type==EMBOSS ||
                        Dim2Procs[i].Kernel3x3_Type==SHARPEN || Dim2Procs[i].Kernel3x3_Type==SMOOTH ||
                        Dim2Procs[i].Kernel3x3_Type==MEANRENOVAL || Dim2Procs[i].Kernel3x3_Type==LP_MEDIA ||
                        Dim2Procs[i].Kernel3x3_Type==LP_POND || Dim2Procs[i].Kernel3x3_Type==HP_MEDIA ||
                        Dim2Procs[i].Kernel3x3_Type==HP_LAPLACE || Dim2Procs[i].Kernel3x3_Type==HP_LAPLACE_MIN ||
                                        Dim2Procs[i].Kernel3x3_Type==USUARIO ){
                    int KerSum = 0;
                    for( int y = 0; y<9; y++ ){
                        KerSum = KerSum + Dim2Procs[i].Kernel3x3H[y];
                    }
                    if( KerSum==0 ){
                        KerSum = 1;
                    }
                    sprintf( TextT,
                             "ker%i = [%i, %i, %i; %i, %i, %i; %i, %i, %i]/%i;\n", i,
                             Dim2Procs[i].Kernel3x3H[0], Dim2Procs[i].Kernel3x3H[1], Dim2Procs[i].Kernel3x3H[2],
                             Dim2Procs[i].Kernel3x3H[3], Dim2Procs[i].Kernel3x3H[4], Dim2Procs[i].Kernel3x3H[5],
                             Dim2Procs[i].Kernel3x3H[6], Dim2Procs[i].Kernel3x3H[7], Dim2Procs[i].Kernel3x3H[8],
                             KerSum
                             );
                    strcat( T2T, TextT );
                    sprintf( TextT, "y%i = uint8( imfilter(y%i, ker%i ) );\n", i, i, i );
                    strcat( T2T, TextT );
                }else{
                    if( Dim2Procs[i].Kernel3x3_Type==SOBEL ){
                        if( Dim2Procs[i].Kernel3x3_Offset[SOBEL]==-128 ){
                            sprintf( TextT, "y%i = edge(y%i, 'sobel', 'nothinning' );\n", i, i );
                        }else{
                            sprintf( TextT, "y%i = edge(y%i, 'sobel', 'nothinning', %g );\n", i, i,
                                    ((double)Dim2Procs[i].Kernel3x3_Offset[SOBEL]+128)/255 );
                        }
                        strcat( T2T, TextT );
                    }
                    if( Dim2Procs[i].Kernel3x3_Type==PREWITT ){
                        if( Dim2Procs[i].Kernel3x3_Offset[PREWITT]==-128 ){
                            sprintf( TextT, "y%i = edge(y%i, 'prewitt', 'nothinning' );\n", i, i );
                        }else{
                            sprintf( TextT, "y%i = edge(y%i, 'prewitt', 'nothinning', %g );\n", i, i,
                                    ((double)Dim2Procs[i].Kernel3x3_Offset[PREWITT]+128)/255 );
                        }
                        strcat( T2T, TextT );
                    }
                    if( Dim2Procs[i].Kernel3x3_Type==KIRSH ){
                        writekrish = TRUE;
                        sprintf( TextT, "y%i = uint8( kirschedge( y%i ) + %i );\n", i, i,
                                Dim2Procs[i].Kernel3x3_Offset[KIRSH] );
                        /*
                        if( Dim2Procs[i].Kernel3x3_Offset[10]==-128 ){
                            sprintf( TextT, "y%i = edge(y%i, 'prewitt', 'nothinning' );\n", i, i );
                        }else{
                            sprintf( TextT, "y%i = edge(y%i, 'prewitt', 'nothinning', %g );\n", i, i,
                                    ((double)Dim2Procs[i].Kernel3x3_Offset[10]+128)/255 );
                        }
                        //*/
                        strcat( T2T, TextT );
                    }
                    if( Dim2Procs[i].Kernel3x3_Type==AVERAGE ){
                        sprintf( TextT,
                                "h = fspecial( 'average', %i );\n"
                                "y%i = uint8( imfilter( y%i, h ) + %i );\n"
                                ,
                                Dim2Procs[i].NOrd,
                                i, i,
                                Dim2Procs[i].Kernel3x3_Offset[AVERAGE] );
                        strcat( T2T, TextT );
                    }
                }
                ///
                strcat( T2T, "figure\n" );
                sprintf( TextT, "imshow(y%i);\n", i );
                strcat( T2T, TextT );
                sprintf( TextT, "title('Módulo %i');\ndrawnow;\n", i );
                strcat( T2T, TextT );
                strcat( T2T, "\n\n" );
            }
                break;
            /// //////////////////////////////////////////////////////////////////////////////////////////////////
            /// //////////////////////////////////////////////////////////////////////////////////////////////////
            /// //////////////////////////////////////////////////////////////////////////////////////////////////
            case HIST_2DPROC:{
                sprintf( TextT, "%%Procesamiento del módulo %i(Historigrama)\n", i );
                strcat( T2T, TextT );
                strcat( T2T, "figure\n" );
                ///
                sprintf( TextT,
                        "if ismatrix(y%i)==1\n"
                        "\tplot( imhist(uint8(y%i)) );\n"
                        "else\n"
                        "\tR = uint8(y%i(:,:,1));\n"
                        "\tG = uint8(y%i(:,:,2));\n"
                        "\tB = uint8(y%i(:,:,3));\n"
                        "\tplot( 0:255, imhist(R), 'r' );hold on;\n"
                        "\tplot( 0:255, imhist(G), 'g' );\n"
                        "\tplot( 0:255, imhist(B), 'b' );hold off;\n"
                        "end"
                        "\n", i_from, i_from, i_from, i_from, i_from );
                strcat( T2T, TextT );
                sprintf( TextT, "title('Módulo %i');\ndrawnow;\n", i );
                strcat( T2T, TextT );
                strcat( T2T, "\n\n" );
                ///
            }
                break;
            /// //////////////////////////////////////////////////////////////////////////////////////////////////
            /// //////////////////////////////////////////////////////////////////////////////////////////////////
            /// //////////////////////////////////////////////////////////////////////////////////////////////////
            case REDIM_2DPROC:{
                sprintf( TextT, "%%Procesamiento del módulo %i(Redimensionar)\n", i );
                strcat( T2T, TextT );
                sprintf( TextT, "y%i = imresize( y%i, [%i, %i] );\n", i, i_from,
                        Dim2Procs[i].HPIC_REDIM, Dim2Procs[i].WPIC_REDIM );
                strcat( T2T, TextT );
                ///
                strcat( T2T, "figure\n" );
                sprintf( TextT, "imshow(y%i);\n", i );
                strcat( T2T, TextT );
                sprintf( TextT, "title('Módulo %i');\ndrawnow;\n", i );
                strcat( T2T, TextT );
                ///
                strcat( T2T, "\n\n" );
            }
                break;
            /// //////////////////////////////////////////////////////////////////////////////////////////////////
            /// //////////////////////////////////////////////////////////////////////////////////////////////////
            /// //////////////////////////////////////////////////////////////////////////////////////////////////
            case ARITM_2DPROC:{
                sprintf( TextT, "%%Procesamiento del módulo %i(Aritmética)\n", i );
                strcat( T2T, TextT );
                if( Dim2Procs[i_from].IsBin ){
                    sprintf( TextT, "y%i = 255 * double(y%i);\n", i, i_from );
                }else{
                    sprintf( TextT, "y%i = double(y%i);\n", i, i_from );
                }
                strcat( T2T, TextT );
                switch( Dim2Procs[i].ArithType ){
                    case 0:
                        if( Dim2Procs[i].InvOut ){
                            sprintf( TextT, "y%i = 255 - (uint8( y%i + %i ));\n", i, i, Dim2Procs[i].SumOper );
                        }else{
                            sprintf( TextT, "y%i = uint8( y%i + %i );\n", i, i, Dim2Procs[i].SumOper );
                        }
                        strcat( T2T, TextT );
                        break;
                    case 1:
                        if( Dim2Procs[i].InvOut ){
                            sprintf( TextT, "y%i = 255 - (uint8( y%i * %g ));\n", i, i, Dim2Procs[i].ProdOper );
                        }else{
                            sprintf( TextT, "y%i = uint8( y%i * %g );\n", i, i, Dim2Procs[i].ProdOper );
                        }
                        strcat( T2T, TextT );
                        break;
                    case 2:
                        if( Dim2Procs[i].InvOut ){
                            sprintf( TextT, "y%i = 255 - (255*uint8( y%i>%i & y%i<%i ));\n", i, i, Dim2Procs[i].LowValue, i,
                                    Dim2Procs[i].HiValue );
                        }else{
                            sprintf( TextT, "y%i = 255*uint8( y%i>%i & y%i<%i );\n", i, i, Dim2Procs[i].LowValue, i,
                                    Dim2Procs[i].HiValue );
                        }
                        strcat( T2T, TextT );
                        break;
                }
                ///
                strcat( T2T, "figure\n" );
                sprintf( TextT, "imshow(y%i);\n", i );
                strcat( T2T, TextT );
                sprintf( TextT, "title('Módulo %i');\ndrawnow;\n", i );
                strcat( T2T, TextT );
                strcat( T2T, "\n\n" );
            }
                break;
            /// //////////////////////////////////////////////////////////////////////////////////////////////////
            /// //////////////////////////////////////////////////////////////////////////////////////////////////
            /// //////////////////////////////////////////////////////////////////////////////////////////////////
            case BW_LABEL:{
                sprintf( TextT, "%%Procesamiento del módulo %i(Conteo binario)\n", i );
                strcat( T2T, TextT );
                if( Dim2Procs[i_from].IsBin ){
                    sprintf( TextT, "y%i = y%i;\n", i, i_from );
                    strcat( T2T, TextT );
                }else{
                    if( Dim2Procs[i_from].IsGray ){
                        sprintf( TextT, "y%i = im2bw(y%i, graythresh(y%i) );\n", i, i_from, i_from );
                        strcat( T2T, TextT );
                    }else{
                        sprintf( TextT, "y%i = y%i;\n", i, i_from );
                        strcat( T2T, TextT );
                        sprintf( TextT, "if ndims(y%i)==3\n"
                                            "\ty%i = rgb2gray(y%i);\n"
                                            "\ty%i = im2bw(y%i, graythresh(y%i) );\n"
                                        "else\n"
                                            "\tif islogical(y%i)==0\n"
                                            "\t\ty%i = im2bw(y%i, graythresh(y%i) );\n"
                                            "\tend\n"
                                        "end\n", i, i, i, i, i, i, i, i, i, i );
                        strcat( T2T, TextT );
                    }
                }
                sprintf( TextT, "[etiquetas%i, nobjetosbw%i] = bwlabel(y%i,8);\n", i, i, i );
                strcat( T2T, TextT );
                sprintf( TextT, "E%i = label2rgb(etiquetas%i);\n", i, i );
                strcat( T2T, TextT );
                sprintf( TextT, "imshow(E%i);\n", i );
                strcat( T2T, TextT );
                sprintf( TextT, "str = sprintf( 'Módulo %i, \t%%i Objetos encontrados.', nobjetosbw%i );\n", i, i );
                strcat( T2T, TextT );
                sprintf( TextT, "title(str);\n" );
                strcat( T2T, TextT );
                ///
                strcat( T2T, "\n\n" );
                ///
            }
                break;
            /// //////////////////////////////////////////////////////////////////////////////////////////////////
            /// //////////////////////////////////////////////////////////////////////////////////////////////////
            /// //////////////////////////////////////////////////////////////////////////////////////////////////

        }
    }
    ///
    strcat( T2T, "\n\ntoc\n\n" );
    #endif // compcpy2d
    ///
    if( writekrish ){
        DSP_MNG.Write_Resource( "kirschedge.m", PATH, KIRSH_M, FILE_RES );
    }
    ///
    ///
    ///
    DWORD D_W = 0;
    DWORD SLEN = strlen(T2T);
    WriteFile( HFile, T2T, SLEN, &D_W, NULL );
    if( D_W!=SLEN ){
        MessageBoxA( HW_MAIN, "Error al escribir el archivo.", " Error ", 16 );
        CloseHandle( HFile );
        free( T2T );
        free( TextT );
        return FALSE;
    }
    ///
    ///
    CloseHandle( HFile );
    free( T2T );
    free( TextT );
    MsgBoxF( "Archivos escritos correctamente." );
    return TRUE;
}

