LRESULT PLOTEOS::WProcCon( HWND hwnd, UINT  message, WPARAM wParam, LPARAM lParam ){
switch( message ){
    case( WM_COMMAND ):
        if( LOWORD(wParam)==1 ){
            if( HIWORD(wParam)==CBN_SELCHANGE ){
                SpectOpt = SendMessage( HW_SPECTOPT, CB_GETCURSEL, 0, 0 );
                if( OnSpect==FALSE ){
                    PLOT_SPECTOGRAM_OPT( TRUE );
                }
                /// char T2T[128] = {0};sprintf( T2T, "%i", SpectOpt );MessageBoxA( hwnd, T2T, "", 0 );
            }
            break;
        }
        ///
		/// FMess( message, wParam, lParam );
		break;
    case( WM_LBUTTONDBLCLK ):
        FMess( WM_COMMAND, CMD_PIC_2D, 0 );
        break;
        if( IsDocked ){
           if( wParam!=12345 || lParam!=12345 ){
              break;
           }
           IsDockedPermis = TRUE;
           if( IsDockedMaxed && lParam!=12345 ){
              ShowWindow( HWContainer, 1 );
              IsDockedMaxed = FALSE;
           }else{
              ShowWindow( HWContainer, 3 );
              IsDockedMaxed = TRUE;
           }
           SetSizeConP();
           IsDockedPermis = FALSE;
           /// FMess( WM_COMMAND, 12345, 12345 );
        }else{
           /// FMess( message, wParam, lParam );
        }
        break;
	case( WM_LBUTTONDOWN ):
		SetFocus( hwnd );
        /// FMess( message, 12345, lParam );
        break;
	case( WM_RBUTTONDOWN ):
		//SetFocus( hwnd );
        /// FMess( message, 12345, lParam );
        break;
    case( WM_MOUSEWHEEL ):
        /// FMess( WM_MOUSEWHEEL, wParam, 12345 );
        break;
    case( WM_SIZE ):
		if( IsDocked ){
           xPosCon = 0;
           yPosCon = 0;
           WidthGraph = 0;
           HeightGraph = 0;

           WidthCon = GetSystemMetrics( SM_CXSCREEN );
		   HeightCon = GetSystemMetrics( SM_CYSCREEN );
		   WidthGraph = WidthCon - xPosGraph - 5 - 25;
           HeightGraph = HeightCon - yPosGraph - 30;
		}
        // FMess( message, wParam, lParam );
        // msgbox();
    case( WM_KEYDOWN ):
        /// FMess( WM_KEYDOWN, wParam, lParam );
        break;
	case( WM_MOUSEMOVE ):{
		char TText[256] = {0};
		int XMPos = LOWORD(lParam) - xPosGraph;
		int YMPos = HIWORD(lParam) - yPosGraph - 1;
		YMPos = HeightGraph - YMPos - 1;
		if( XMPos<0 || XMPos>=WidthGraph ){
            sprintf( TText, "X:\nY:" );
            SetWindowText( HWSText, TText );
			break;
		}
		//
		if( YMPos<0 || YMPos>=HeightGraph ){
		    sprintf( TText, "X:\nY:" );
            SetWindowText( HWSText, TText );
			break;
		}
		//
		double DiffIEX = MaxXLim - MinXLim;
		double DXL = (XMPos);
		DXL = DXL/(WidthGraph-1);
		DXL = (MinXLim + DXL*DiffIEX );
        ///
        ///
        ///
        ///
		/// double DYL = (MaxYLim*YMPos);DYL = DYL/(HeightGraph-1);
		double DYL = 0;
		///
		///
		///
		if( MinYLim>-0.0001 ){
            DYL = (MaxYLim*YMPos);
            DYL = DYL/(HeightGraph-1);
		}else{
            DYL = 2*( ( YMPos/( (double)(HeightGraph-1) ) ) - 0.5 );
            ///DYL = MaxYLim * DYL;
		}
        ///
        ///
        ///
        ///
        ///
		//sprintf( TText, "X:%5.3f - %g - %g\nY:%5.5f", DXL, MaxXLim, MinXLim, DYL );
		sprintf( TText, "X:%5.3f\nY:%5.5f", DXL, DYL );
		sprintf( TText, "X:%g\nY:%g", DXL, DYL );
		if( RoundValues ){
            sprintf( TText, "X:%g\nY:%g", round(DXL), round(DYL) );
		}
		SetWindowText( HWSText, TText );

		/// FMess( message, 12345, lParam );
	}
        break;
case( WM_PAINT ):{
    //
    //return DefWindowProc (hwnd, message, wParam, lParam);
    HFONT HFontT = NULL;
    ///
    PAINTSTRUCT         ps;
    HDC HDc = BeginPaint( hwnd, &ps );
    SetBkMode( HDc, TRANSPARENT);
	SetTextAlign( HDc, TA_CENTER );
	///
    BOOL Permission = TRUE;
	if( Text2Show!=NULL && BText2Show ){
	   if( Permission ){
	      TextOut( HDc, WidthCon/2, HeightCon - 24, Text2Show, strlen(Text2Show) );
       }else{
	      TextOut( HDc, WidthCon/2, 8, Text2Show, strlen(Text2Show) );
       }
	}
	if( Text2Title!=NULL ){
        TextOut( HDc, WidthCon/2, 14, Text2Title, strlen(Text2Title) );
	}
	/*
    TextOut( HDc, WidthCon - 180, 2, CreadoPor, strlen(CreadoPor) );
    SetTextColor( HDc, (COLORREF)(0x00FF0000) );
    TextOut( HDc, WidthCon - 180, 2+16, AdrianCosta, strlen(AdrianCosta) );
    SetTextColor( HDc, (COLORREF)(0x00000000) );
    //*/
	/*
	RECT re;
    re.bottom = WidthCon;
    re.left = 0;
    re.right = xPosGraph;
    re.top = 0;
	FillRect( HDc, &re, CreateSolidBrush( RGB( RgbColor , RgbColor , RgbColor ) )  );
	//*/
	static char TTex[256] = {0};
	double Val2P = MaxYLim;
	Val2P = round(10*Val2P);
	Val2P = Val2P/10;
	sprintf( TTex, "%g", Val2P );
	//TextOut( HDc, xPosGraph/2, yPosGraph-6, TTex, strlen(TTex) );
	for( int i=1; i<6; i++ ){
		Val2P = ((5-i)*MaxYLim)/5;
		Val2P = round(10*Val2P);
		Val2P = Val2P/10;
        ///
        sprintf( TTex, "%g", Val2P );
		/// TextOut( HDc, xPosGraph/2, yPosGraph-8 + (i*HeightGraph)/5, TTex, strlen(TTex) );
	}
	sprintf( TTex, "%g", MinYLim );
    /// TextOut( HDc, xPosGraph/2, yPosGraph-8 + (5*HeightGraph)/5, TTex, strlen(TTex) );
	///
	sprintf( TTex, "%g", MaxXLim );
	TextOut( HDc, xPosGraph + WidthGraph, yPosGraph + HeightGraph + 5, TTex, strlen(TTex) );

	sprintf( TTex, "%g", MinXLim );
	TextOut( HDc, xPosGraph, yPosGraph + HeightGraph + 5, TTex, strlen(TTex) );
	//*/
	///
	/// MaxValFFT
	///
	if( TextMy!=NULL ){
        if( TextMy[0]!=0 ){
            HFontT = CreateFont( 16, 0, 900, 0, FW_BOLD, FALSE, FALSE, FALSE, DEFAULT_CHARSET,
                                OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, PROOF_QUALITY,
                                DEFAULT_PITCH | FF_ROMAN, "Times");
            SetBkMode( HDc, TRANSPARENT);
            SetTextAlign( HDc, TA_CENTER );
            SelectObject( HDc, HFontT );
            TextOut( HDc, 10, HeightCon/2, TextMy, strlen(TextMy) );
        }
	}
	///
	///
	///
	///
    HFntNumberY = CreateFont( 16, 0, 900, 0, FW_BOLD, FALSE, FALSE, FALSE, DEFAULT_CHARSET,
                            OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, PROOF_QUALITY,
                                DEFAULT_PITCH | FF_ROMAN, "Times" );
	SelectObject( HDc, HFntNumberY );
	///
	Val2P = MaxYLim;
	Val2P = floor(100*Val2P);Val2P = Val2P/100;
	sprintf( TTex, "%g", Val2P );
	if( abs(MaxYLim)<0.01 ){
        sprintf( TTex, "%g", MaxYLim );
	}
	TextOut( HDc, xPosGraph/2 - 8, yPosGraph-6, TTex, strlen(TTex) );
	///
	Val2P = MinYLim;
	Val2P = floor(100*Val2P);Val2P = Val2P/100;
	sprintf( TTex, "%g", Val2P );
	if( abs(MinYLim)<0.01 ){
        sprintf( TTex, "%g", MinYLim );
	}
    TextOut( HDc, xPosGraph/2 - 8, yPosGraph-8 + (5*HeightGraph)/5, TTex, strlen(TTex) );
	///
	///
	///
	///
	///
    EndPaint( hwnd, &ps);
    DeleteObject( HFontT );
    DeleteObject( HFntNumberY );
}
    break;
    //*
    case WM_CTLCOLORSTATIC:{
        #define fndsttc 200
        static HBRUSH pincel = NULL;
        if( pincel==NULL ){
            pincel = CreateSolidBrush( RGB( fndsttc, fndsttc, fndsttc) );
        }
        HDC hdcStatic = (HDC)wParam; // or obtain the static handle in some other way
        SetTextColor( hdcStatic, RGB( 0, 0, 0)); // text color
        SetBkColor( hdcStatic, RGB( fndsttc, fndsttc, fndsttc ) );
        /// return (LRESULT)GetStockObject( NULL_BRUSH );
        return (LRESULT)pincel;
    }
    break;
    //*/
case( WM_CLOSE ):
    return 0;
default:
     return DefWindowProc (hwnd, message, wParam, lParam);
}
return 0;
}

// floricienta 2005 cap 47 3/5
