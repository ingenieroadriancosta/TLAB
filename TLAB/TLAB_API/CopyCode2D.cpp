BOOL TLAB::CopyCode2D( LPSTR T2T, BOOL* writekrish ){
    char* TextT = (char*)malloc( 1024 * 1024 );
    ZeroMemory( TextT, 1024 * 1024 - 1 );
    ///
    ///
    strcat( TextT, "clc;clear all;tic;\n" );
    strcat( T2T, TextT );
    sprintf( TextT, "y0 = imread('%s');\n", Dim2Procs[0].FName );
    strcat( T2T, TextT );
    if( Dim2Procs[0].IsGray ){
        sprintf( TextT, "if ndims(y0)==3\n"
                            "\ty0 = rgb2gray(y0);\n"
                        "end\n" );
        strcat( T2T, TextT );
    }
    int i_from = 0;
    ///
    writekrish[0] = FALSE;
    ///
    for( int i=1; i<MaxProcs; i++ ){
        i_from = i-1;
        while( Dim2Procs[i_from].ProcType==HIST_2DPROC ||
              Dim2Procs[i_from].ProcType==NO_PROC_Signal_2D || Dim2Procs[i_from].ProcType==SHOW_2D ){
            if( Dim2Procs[i_from].ImageInP>0 ){
                i_from = 0;
                break;
            }
            i_from--;
        }
        if( Dim2Procs[i].ImageInP>0 ){
            i_from = 0;
        }
        switch( Dim2Procs[i].ProcType ){
            case SHOW_2D:{
                sprintf( TextT, "%%Procesamiento del módulo %i(Mostrar imagen)\n", i );
                strcat( T2T, TextT );
                strcat( T2T, "figure;\n" );
                sprintf( TextT, "imshow(y%i);\n", i_from );
                strcat( T2T, TextT );
                sprintf( TextT, "title('Módulo %i');\ndrawnow;\n", i );
                strcat( T2T, TextT );
                /// strcat( T2T, "\n\n" );
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
                if( SetPlotimshow ){
                    strcat( T2T, "figure;\n" );
                    sprintf( TextT, "imshow(y%i);\n", i );
                    strcat( T2T, TextT );
                    sprintf( TextT, "title('Módulo %i');\ndrawnow;\n", i );
                    strcat( T2T, TextT );
                }
                /// strcat( T2T, "\n\n" );
            }
                break;
            /// //////////////////////////////////////////////////////////////////////////////////////////////////
            /// //////////////////////////////////////////////////////////////////////////////////////////////////
            /// //////////////////////////////////////////////////////////////////////////////////////////////////
            case BINARY_2DPROC:{
                sprintf( TextT, "%%Procesamiento del módulo %i(Binarizar imagen.)\n", i );
                strcat( T2T, TextT );
                if( Dim2Procs[i_from].IsBin || Dim2Procs[i_from].ProcType==BW_LABEL ){
                    sprintf( TextT, "y%i = y%i;%%La imagen ya es binaria.\n", i, i_from );
                }else{
                    switch( Dim2Procs[i].OffsetType ){
                        case 0:
                            sprintf( TextT, "y%i = im2bw( y%i, %i/255 );\n", i, i_from, Dim2Procs[i].LevelBIN );
                            break;
                        case 1:
                            sprintf( TextT, "y%i = im2bw( y%i, graythresh(uint8(y%i)) );\n", i, i_from, i_from );
                            break;
                        case 2:
                            /// sprintf( TextT, "y%i = im2bw( y%i, (sum(y%i(:))/numel(y%i))/255 );\n", i, i_from, i_from, i_from );
                            sprintf( TextT, "y%i = im2bw( y%i, mean(y%i(:))/255 );\n", i, i_from, i_from );
                            /// mean(y0(:))
                            break;
                        case 3:
                            sprintf( TextT, "y%i = im2bw( y%i, (sum(y%i(:))/sum(y%i(:)~=0))/255 );\n",
                                                        i, i_from, i_from, i_from );
                            break;
                        case 4:
                            sprintf( TextT, "y%i = im2bw( y%i, 1-graythresh(uint8(y%i)) );\n", i, i_from, i_from );
                            break;
                        case 5:
                            /// MsgBoxF( " " );
                            sprintf( TextT, "y%i = y%i;\nth%i = uint8(255 * graythresh(uint8(y%i)));\n", i, i_from, i, i );
                            strcat( T2T, TextT );
                            ///
                            sprintf( TextT,
                                    "for in=1:numel( y%i )\n"
                                        "\tif y%i(in)<th%i\n"
                                            "\t\ty%i(in) = th%i;\n"
                                        "\tend\n"
                                    "end\n"
                                    , i, i, i, i, i );
                            strcat( T2T, TextT );
                            sprintf( TextT, "y%i = im2bw( y%i, graythresh(uint8(y%i)) );\n", i, i, i );
                            break;
                    }
                }
                strcat( T2T, TextT );
                ///
                if( Dim2Procs[i].BW_OPTS>0 ){
                    sprintf( TextT, "y%i = ~y%i;\n", i, i );
                    strcat( T2T, TextT );
                }
                ///
                if( SetPlotimshow ){
                    strcat( T2T, "figure;\n" );
                    sprintf( TextT, "imshow(y%i);\n", i );
                    strcat( T2T, TextT );
                    sprintf( TextT, "title('Módulo %i');\ndrawnow;\n", i );
                    strcat( T2T, TextT );
                }
                /// strcat( T2T, "\n\n" );
            }
                break;
            /// //////////////////////////////////////////////////////////////////////////////////////////////////
            /// //////////////////////////////////////////////////////////////////////////////////////////////////
            /// //////////////////////////////////////////////////////////////////////////////////////////////////
            case CONV_3x3_2DPROC:{
                sprintf( TextT, "%%Procesamiento del módulo %i(Convolución 3x3.)\n", i );
                strcat( T2T, TextT );
                if( Dim2Procs[i_from].IsBin || Dim2Procs[i_from].ProcType==BW_LABEL ){
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
                        if( Dim2Procs[i].BSobel_Remove ){
                            sprintf( TextT, "y%i(1:3,1:end) = 0;\n", i );strcat( T2T, TextT );
                            sprintf( TextT, "y%i(end-2:end,1:end) = 0;\n", i );strcat( T2T, TextT );
                            sprintf( TextT, "y%i(1:end,1:3) = 0;\n", i );strcat( T2T, TextT );
                            sprintf( TextT, "y%i(1:end,end-2:end) = 0;\n", i );strcat( T2T, TextT );
                        }
                    }
                    if( Dim2Procs[i].Kernel3x3_Type==PREWITT ){
                        if( Dim2Procs[i].Kernel3x3_Offset[PREWITT]==-128 ){
                            sprintf( TextT, "y%i = edge(y%i, 'prewitt', 'nothinning' );\n", i, i );
                        }else{
                            sprintf( TextT, "y%i = edge(y%i, 'prewitt', 'nothinning', %g );\n", i, i,
                                    ((double)Dim2Procs[i].Kernel3x3_Offset[PREWITT]+128)/255 );
                        }
                        strcat( T2T, TextT );
                        if( Dim2Procs[i].BSobel_Remove ){
                            sprintf( TextT, "y%i(1:3,1:end) = 0;\n", i );strcat( T2T, TextT );
                            sprintf( TextT, "y%i(end-2:end,1:end) = 0;\n", i );strcat( T2T, TextT );
                            sprintf( TextT, "y%i(1:end,1:3) = 0;\n", i );strcat( T2T, TextT );
                            sprintf( TextT, "y%i(1:end,end-2:end) = 0;\n", i );strcat( T2T, TextT );
                        }
                    }
                    if( Dim2Procs[i].Kernel3x3_Type==KIRSH ){
                        if( writekrish!=NULL ){
                            writekrish[0] = TRUE;
                        }
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
                if( SetPlotimshow ){
                    strcat( T2T, "figure;\n" );
                    sprintf( TextT, "imshow(y%i);\n", i );
                    strcat( T2T, TextT );
                    sprintf( TextT, "title('Módulo %i');\ndrawnow;\n", i );
                    strcat( T2T, TextT );
                }
                /// strcat( T2T, "\n\n" );
            }
                break;
            /// //////////////////////////////////////////////////////////////////////////////////////////////////
            /// //////////////////////////////////////////////////////////////////////////////////////////////////
            /// //////////////////////////////////////////////////////////////////////////////////////////////////
            case HIST_2DPROC:{
                sprintf( TextT, "%%Procesamiento del módulo %i(Historigrama)\n", i );
                strcat( T2T, TextT );
                strcat( T2T, "figure;\n" );
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
                /// strcat( T2T, "\n\n" );
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
                if( SetPlotimshow ){
                    strcat( T2T, "figure;\n" );
                    sprintf( TextT, "imshow(y%i);\n", i );
                    strcat( T2T, TextT );
                    sprintf( TextT, "title('Módulo %i');\ndrawnow;\n", i );
                    strcat( T2T, TextT );
                }
                ///
                /// strcat( T2T, "\n\n" );
            }
                break;
            /// //////////////////////////////////////////////////////////////////////////////////////////////////
            /// //////////////////////////////////////////////////////////////////////////////////////////////////
            /// //////////////////////////////////////////////////////////////////////////////////////////////////
            case ARITM_2DPROC:{
                sprintf( TextT, "%%Procesamiento del módulo %i(Aritmética)\n", i );
                strcat( T2T, TextT );
                if( Dim2Procs[i_from].IsBin || Dim2Procs[i_from].ProcType==BW_LABEL ){
                    sprintf( TextT, "y%i = 255 * double(y%i);\n", i, i_from );
                }else{
                    if( Dim2Procs[i_from].IsGray ){
                        sprintf( TextT, "y%i = double(y%i);\n", i, i_from );
                    }else{
                        sprintf( TextT, "if ndims(y%i)==3\n"
                                  "\ty%i = rgb2gray(y%i);\n"
                                  "else\n"
                                  "\ty%i = y%i;\n"
                                  "end\n", i_from, i, i_from, i, i_from );
                        /// sprintf( TextT, "y%i = rgb2gray(y%i);\n", i, i_from );
                    }
                }
                strcat( T2T, TextT );
                switch( Dim2Procs[i].ArithType ){
                    case 0:
                        if( Dim2Procs[i].InvOut ){
                            sprintf( TextT, "y%i = 255 - (uint8( floor(y%i + %i) ));\n", i, i, Dim2Procs[i].SumOper );
                        }else{
                            sprintf( TextT, "y%i = uint8( floor(y%i + %i) );\n", i, i, Dim2Procs[i].SumOper );
                        }
                        strcat( T2T, TextT );
                        break;
                    case 1:
                        if( Dim2Procs[i].InvOut ){
                            sprintf( TextT, "y%i = 255 - (uint8( floor(y%i * %g) ));\n", i, i, Dim2Procs[i].ProdOper );
                        }else{
                            sprintf( TextT, "y%i = uint8( floor(y%i * %g) );\n", i, i, Dim2Procs[i].ProdOper );
                        }
                        strcat( T2T, TextT );
                        break;
                    case 2:
                        if( Dim2Procs[i].InvOut ){
                            if( Dim2Procs[i].LowValue>Dim2Procs[i].HiValue ){
                                sprintf( TextT, "y%i = ~(( y%i>%i | y%i<%i ));\n", i, i, Dim2Procs[i].LowValue, i,
                                        Dim2Procs[i].HiValue );
                            }else{
                                sprintf( TextT, "y%i = ~(( y%i>%i & y%i<%i ));\n", i, i, Dim2Procs[i].LowValue, i,
                                        Dim2Procs[i].HiValue );
                            }
                        }else{
                            if( Dim2Procs[i].LowValue>Dim2Procs[i].HiValue ){
                                sprintf( TextT, "y%i = ( y%i>%i | y%i<%i );\n", i, i, Dim2Procs[i].LowValue, i,
                                    Dim2Procs[i].HiValue );
                            }else{
                                sprintf( TextT, "y%i = ( y%i>%i & y%i<%i );\n", i, i, Dim2Procs[i].LowValue, i,
                                        Dim2Procs[i].HiValue );
                            }
                        }
                        strcat( T2T, TextT );
                        break;
                }
                ///
                if( SetPlotimshow ){
                    strcat( T2T, "figure;\n" );
                    sprintf( TextT, "imshow(y%i);\n", i );
                    strcat( T2T, TextT );
                    sprintf( TextT, "title('Módulo %i');\ndrawnow;\n", i );
                    strcat( T2T, TextT );
                }
                /// strcat( T2T, "\n\n" );
            }
                break;
            /// //////////////////////////////////////////////////////////////////////////////////////////////////
            /// //////////////////////////////////////////////////////////////////////////////////////////////////
            /// //////////////////////////////////////////////////////////////////////////////////////////////////
            case IMERODE:{
                sprintf( TextT, "%%Procesamiento del módulo %i(Erosión binaria)\n", i );
                strcat( T2T, TextT );
                if( Dim2Procs[i_from].IsBin || Dim2Procs[i_from].ProcType==BW_LABEL ){
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
                static char             Text20[8][16] = { "ball", "square", "diamond" };
                if( Dim2Procs[i].OptErode==0 ){
                    sprintf( TextT, "strel%i = strel('%s', %i, 0, 0 );\n", i, Text20[Dim2Procs[i].OptErode], Dim2Procs[i].NOrdErode );
                }else{
                    sprintf( TextT, "strel%i = strel('%s', %i );\n", i, Text20[Dim2Procs[i].OptErode], Dim2Procs[i].NOrdErode );
                }
                strcat( T2T, TextT );
                if( Dim2Procs[i].BFront==0 ){
                    sprintf( TextT, "y%i = imerode( y%i, strel%i );\n", i, i, i );
                }else{
                    sprintf( TextT, "y%i = y%i & (~imerode( y%i, strel%i ));\n", i, i, i, i );
                }
                strcat( T2T, TextT );
                if( SetPlotimshow ){
                    sprintf( TextT, "figure;\nimshow(y%i);\n", i );
                    strcat( T2T, TextT );
                    sprintf( TextT, "title('Módulo %i');\ndrawnow;\n", i );
                    strcat( T2T, TextT );
                }
            }
            break;
            /// //////////////////////////////////////////////////////////////////////////////////////////////////
            /// //////////////////////////////////////////////////////////////////////////////////////////////////
            /// //////////////////////////////////////////////////////////////////////////////////////////////////
            case IMDILATE:{
                sprintf( TextT, "%%Procesamiento del módulo %i(Erosión binaria)\n", i );
                strcat( T2T, TextT );
                if( Dim2Procs[i_from].IsBin || Dim2Procs[i_from].ProcType==BW_LABEL ){
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
                static char             Text20[8][16] = { "ball", "square", "diamond" };
                if( Dim2Procs[i].OptErode==0 ){
                    sprintf( TextT, "strel%i = strel('%s', %i, 0, 0 );\n", i, Text20[Dim2Procs[i].OptErode], Dim2Procs[i].NOrdErode );
                }else{
                    sprintf( TextT, "strel%i = strel('%s', %i );\n", i, Text20[Dim2Procs[i].OptErode], Dim2Procs[i].NOrdErode );
                }
                strcat( T2T, TextT );
                /// sprintf( TextT, "y%i = imdilate( y%i, strel%i );\n", i, i, i );
                if( Dim2Procs[i].BFront==0 ){
                    sprintf( TextT, "y%i = imdilate( y%i, strel%i );\n", i, i, i );
                }else{
                    sprintf( TextT, "y%i = (~y%i) & (imdilate( y%i, strel%i ));\n", i, i, i, i );
                }
                strcat( T2T, TextT );
                if( SetPlotimshow ){
                    sprintf( TextT, "figure;\nimshow(y%i);\n", i );
                    strcat( T2T, TextT );
                    sprintf( TextT, "title('Módulo %i');\ndrawnow;\n", i );
                    strcat( T2T, TextT );
                }
            }
            break;
            /// //////////////////////////////////////////////////////////////////////////////////////////////////
            /// //////////////////////////////////////////////////////////////////////////////////////////////////
            /// //////////////////////////////////////////////////////////////////////////////////////////////////
            case IMOPEN:{
                sprintf( TextT, "%%Procesamiento del módulo %i(Erosión binaria)\n", i );
                strcat( T2T, TextT );
                if( Dim2Procs[i_from].IsBin || Dim2Procs[i_from].ProcType==BW_LABEL ){
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
                static char             Text20[8][16] = { "ball", "square", "diamond" };
                if( Dim2Procs[i].OptErode==0 ){
                    sprintf( TextT, "strel%i = strel('%s', %i, 0, 0 );\n", i, Text20[Dim2Procs[i].OptErode], Dim2Procs[i].NOrdErode );
                }else{
                    sprintf( TextT, "strel%i = strel('%s', %i );\n", i, Text20[Dim2Procs[i].OptErode], Dim2Procs[i].NOrdErode );
                }
                strcat( T2T, TextT );
                if( Dim2Procs[i].BFront==0 ){
                    sprintf( TextT, "y%i = imopen( y%i, strel%i );\n", i, i, i );
                }else{
                    sprintf( TextT, "y%i = y%i & ~imopen( y%i, strel%i );\n", i, i, i, i );
                }
                strcat( T2T, TextT );
                if( SetPlotimshow ){
                    sprintf( TextT, "figure;\nimshow(y%i);\n", i );
                    strcat( T2T, TextT );
                    sprintf( TextT, "title('Módulo %i');\ndrawnow;\n", i );
                    strcat( T2T, TextT );
                }
            }
            break;
            /// //////////////////////////////////////////////////////////////////////////////////////////////////
            /// //////////////////////////////////////////////////////////////////////////////////////////////////
            /// //////////////////////////////////////////////////////////////////////////////////////////////////
            case IMCLOSE:{
                sprintf( TextT, "%%Procesamiento del módulo %i(Erosión binaria)\n", i );
                strcat( T2T, TextT );
                if( Dim2Procs[i_from].IsBin || Dim2Procs[i_from].ProcType==BW_LABEL ){
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
                static char             Text20[8][16] = { "ball", "square", "diamond" };
                if( Dim2Procs[i].OptErode==0 ){
                    sprintf( TextT, "strel%i = strel('%s', %i, 0, 0 );\n", i, Text20[Dim2Procs[i].OptErode], Dim2Procs[i].NOrdErode );
                }else{
                    sprintf( TextT, "strel%i = strel('%s', %i );\n", i, Text20[Dim2Procs[i].OptErode], Dim2Procs[i].NOrdErode );
                }
                strcat( T2T, TextT );
                if( Dim2Procs[i].BFront==0 ){
                    sprintf( TextT, "y%i = imclose( y%i, strel%i );\n", i, i, i );
                }else{
                    sprintf( TextT, "y%i = imclose( y%i, strel%i ) & (~y%i);\n", i, i, i, i );
                }
                strcat( T2T, TextT );
                if( SetPlotimshow ){
                    sprintf( TextT, "figure;\nimshow(y%i);\n", i );
                    strcat( T2T, TextT );
                    sprintf( TextT, "title('Módulo %i');\ndrawnow;\n", i );
                    strcat( T2T, TextT );
                }
            }
            break;
            /// //////////////////////////////////////////////////////////////////////////////////////////////////
            /// //////////////////////////////////////////////////////////////////////////////////////////////////
            /// //////////////////////////////////////////////////////////////////////////////////////////////////
            case BW_LABEL:{
                sprintf( TextT, "%%Procesamiento del módulo %i(Conteo binario)\n", i );
                strcat( T2T, TextT );
                if( Dim2Procs[i_from].IsBin || Dim2Procs[i_from].ProcType==BW_LABEL ){
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
                if( SetPlotimshow ){
                    sprintf( TextT, "figure;\nimshow(E%i);\n", i );
                    strcat( T2T, TextT );

                    /// sprintf( TextT, "y%i = E%i;\n", i, i );strcat( T2T, TextT );

                    sprintf( TextT, "str = sprintf( 'Módulo %i, \t%%i Objetos encontrados.', nobjetosbw%i );\n", i, i );
                    strcat( T2T, TextT );
                    sprintf( TextT, "title(str);\n" );
                    strcat( T2T, TextT );
                }
                ///
                /// strcat( T2T, "\n\n" );
                ///
            }
                break;
            /// //////////////////////////////////////////////////////////////////////////////////////////////////
            /// //////////////////////////////////////////////////////////////////////////////////////////////////
            /// //////////////////////////////////////////////////////////////////////////////////////////////////
            case IMFILL_OPT:{
                sprintf( TextT, "%%Procesamiento del módulo %i(Conteo binario)\n", i );
                strcat( T2T, TextT );
                if( Dim2Procs[i_from].IsBin ||
                   (Dim2Procs[i_from].ProcType==BW_LABEL) ||
                        (Dim2Procs[i_from].ProcType==CONV_3x3_2DPROC &&
                         (Dim2Procs[i_from].Kernel3x3_Type>=SOBEL &&
                          Dim2Procs[i_from].Kernel3x3_Type<=PREWITT ) ) ||
                            Dim2Procs[i_from].ProcType==IMERODE ||
                            Dim2Procs[i_from].ProcType==IMDILATE ||
                            Dim2Procs[i_from].ProcType==IMOPEN ||
                            Dim2Procs[i_from].ProcType==IMCLOSE ||
                            Dim2Procs[i_from].ProcType==BINARY_2DPROC
                             ){
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
                if( Dim2Procs[i].BOptFill==0 ){
                    sprintf( TextT, "y%i = imfill(y%i,8, 'holes' );\n", i, i );
                }else{
                    sprintf( TextT, "y%i = imfill(y%i,8, 'holes' ) & (~y%i);\n", i, i, i );
                }
                strcat( T2T, TextT );
                if( SetPlotimshow ){
                    sprintf( TextT, "figure;\nimshow(y%i);\n", i );
                    strcat( T2T, TextT );
                }
                ///
                /// strcat( T2T, "\n\n" );
                ///
            }
                break;
            /// //////////////////////////////////////////////////////////////////////////////////////////////////
            /// //////////////////////////////////////////////////////////////////////////////////////////////////
            /// //////////////////////////////////////////////////////////////////////////////////////////////////

        }
    }
    ///
    ///
    strcat( T2T, "\n\ntoc\n\n" );
    ///
    ///
    ///
    free( TextT );
    ///
    return TRUE;
}
/// ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL TLAB::CopyCode2D_2_Clipboard( void ){
    char* T2T = (char*)malloc( 1024 * 1024 );
    ZeroMemory( T2T, 1024 * 1024 - 1 );
    BOOL writekrish = FALSE;
    ///
    ///
    T2T[0] = 0;
    CopyCode2D( T2T, &writekrish );
    if( writekrish ){
        MessageBoxA( HW_MAIN,
                     "Se necesita la función (kirschedge) para que funcione el código, "
                     " asegúrese que está en el directorio de trabajo en el entorno matlab."
                            , "Advertencia", MB_ICONEXCLAMATION );
    }
    ///
    Copy2ClipBoard2( T2T );
    /// MsgBoxF( "Código copiado al portapapeles." );
    DialogBoxParam( GetModuleHandle(NULL), MAKEINTRESOURCEA(Dial_MSGBOX),
                            HW_MAIN, DLGMSGBOX, (LPARAM)"Código copiado al portapapeles." );
    Copy2ClipBoard2( T2T );
    ///
    free( T2T );
    ///
    return TRUE;
}





