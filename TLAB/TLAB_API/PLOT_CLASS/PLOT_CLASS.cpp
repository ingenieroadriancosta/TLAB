#define RgbColor 204
char CreadoPor[32] = {0};
char AdrianCosta[16] = {0};
class	PLOTEOS{
public:
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	BOOL FillBuff( void );
	BOOL FillBuff_Only( void );
	BOOL FillGrid( void );
	BOOL FillGrid( BOOL OptRepint );
	LRESULT WProcCon( HWND, UINT, WPARAM, LPARAM );
	BOOL PLOT_U_SHORT( short*, int, int, BOOL );
	BOOL PLOT_U_SHORT_FFT( short*, int, int, BOOL );
	BOOL PLOT_DOUBLE_FFT( double*, int, int, BOOL );
	BOOL PLOT_SPECTOGRAM( BYTE*, int, int, BOOL );
	BOOL PLOT_SPECTOGRAM_OPT( BOOL );
	BOOL PLOTSHORT( short*, int, int, BOOL );
	BOOL PLOTDOUBLE( double*, int, int, BOOL );
	BOOL PLOTDOUBLE_BIPOL( double*, int, int, BOOL );
	BOOL PLOTDOUBLE_BIPOL( double*, int, int, BOOL, int );
	BOOL PLOTSHORT3( short*, short*, short*, int, int, int, BOOL );
	BOOL PLOT_HIST( double*, int, int, BOOL );
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
private:
int 									PtrClass;
char									PltClassP[32];
char*									Text2Show;
char*									Text2Title;
BOOL									BText2Show;
HWND									HWFatherP;

HBRUSH                                  HBrushC;

HWND									HWSText;
HWND									HWSTextInfoAdd;
HWND									HW_SPECTOPT;
HFONT									FuenteM;


HBITMAP									HBitmapP;

HWND									HWContainer;
int										xPosCon;
int										yPosCon;
int										WidthCon;
int										HeightCon;


HWND									HWGraph;
HFONT                                   HFntGraph;
HFONT                                   HFntNumberY;
int										xPosGraph;
int										yPosGraph;


BOOL                                    OnSpect;

BOOL									ReGrid;
int										WidthGraph;
int										HeightGraph;


//int										LastWidthGraph;
//int										LastHeightGraph;




BOOL									BVisible;
BOOL									RoundValues;

BOOL									DoubleBuffP;
BOOL									Grid_On;
BOOL									IsGrid_Inv;

RGBQUAD*								RGB_BUFF;
RGBQUAD*								OLD_RGB_BUFF;

HBITMAP								    HBSPECT;
int                                     SpectOpt;
int                                     WSPEC;
int                                     HSPEC;
RGBQUAD*								RGB_SPECT;




RGBQUAD									ColorBack;
RGBQUAD									ColorLineG;
int										Grid_Style;
int										PGrid_Style;
int										ActGrid_Style;
int										LineWidth;

double									MaxXLim;
public:
LPCSTR                                  TextMx;
LPCSTR                                  TextMy;
private:
double									MaxYLim;

double									MinXLim;
double									MinYLim;

BOOL									HoldV;

BOOL									IsDocked;
BOOL									IsDockedPermis;
BOOL									IsDockedMaxed;


LPVOID                                  PLT0;
LPVOID                                  PLT1;
LPVOID                                  PLT2;
LPVOID                                  PLTS;


LRESULT FMess( UINT  message, WPARAM wParam, LPARAM lParam ){
    return SendMessage( HWFatherP, message, wParam, lParam );
}

static LRESULT PltWProc( HWND hwnd, UINT  message, WPARAM wParam, LPARAM lParam ){
    // return DefWindowProc( hwnd, message, wParam, lParam );
    static int                              DirClass[32];
    static int                              CntClass;
    if( message==WM_GETTEXT ){
       return DefWindowProc( hwnd, message, wParam, lParam );
    }
    if( message==WM_CREATE ){
        char WTText[256];
		GetWindowText( hwnd, WTText, 200 );
	    int IPtr = atoi(WTText);
	    DirClass[CntClass] = IPtr;
	    CntClass++;
        return DefWindowProc( hwnd, message, wParam, lParam );
	}else{
		PLOTEOS* SPloteosCnt;
		int IClass = 0;
		for( IClass=0; IClass<CntClass; IClass++ ){
             SPloteosCnt = (PLOTEOS*)(DirClass[IClass]);
             if( SPloteosCnt[0].GetHandleCon()==hwnd ){
                return SPloteosCnt[0].WProcCon( hwnd, message, wParam, lParam );
             }
        }
        return DefWindowProc( hwnd, message, wParam, lParam );
		char WTText[256];
		GetWindowText( hwnd, WTText, 200 );
	    int IPtr = atoi(WTText);
	    SPloteosCnt = (PLOTEOS*)(IPtr);
	    return SPloteosCnt[0].WProcCon( hwnd, message, wParam, lParam );
	}
	return 0;
}
HBITMAP	NewBitmap( int W_NEWHB, int H_NEWHB ){ return CreateCompatibleBitmap( GetDC( 0 ), W_NEWHB, H_NEWHB );}


    public:
    BOOL FreeAll(){
        if( RGB_BUFF!=NULL ){
            free( RGB_BUFF );
            RGB_BUFF = NULL;
        }
        if( OLD_RGB_BUFF!=NULL ){
            free( OLD_RGB_BUFF );
            OLD_RGB_BUFF = NULL;
        }
        if( RGB_SPECT!=NULL ){
            free( RGB_SPECT );
            RGB_SPECT = NULL;
        }
        return TRUE;
    }

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    HWND GetContainMain(){ return HWContainer; }
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    int GetSpectOpt(){ return SpectOpt; }
    BOOL SetSpectOpt( int SpectOptI ){
        SpectOpt = SpectOptI;
        SendMessage( HW_SPECTOPT, CB_SETCURSEL, SpectOpt, 0 );
        return TRUE;
    }
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    BOOL SetOnSpect( BOOL BVal ){
        OnSpect = BVal;
        return OnSpect;
    }
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    HWND GetHWSpect(){
        return HW_SPECTOPT;
    }
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    HBITMAP GetBitmap(){
        return HBitmapP;
    }
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    HBITMAP GetBitmapSpect(){
        return HBSPECT;
    }
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    BOOL SetBitmap( HBITMAP HBMP_IN, BOOL IsSpec ){
        if( HBMP_IN!=NULL ){
            if( IsSpec ){
                if( WSPEC!=WidthGraph || HSPEC!=HeightGraph ){
                    DeleteObject( HBSPECT );
                    HBSPECT = NewBitmap( WidthGraph, HeightGraph );
                    if( RGB_SPECT!=NULL ){
                        free( RGB_SPECT );
                        RGB_SPECT = NULL;
                    }
                    RGB_SPECT = (RGBQUAD*)malloc( 4*WidthGraph*HeightGraph + 1024 );
                }
                WSPEC = WidthGraph;
                HSPEC = HeightGraph;
                GetBitmapBits( HBMP_IN, 4*WidthGraph*HeightGraph, &RGB_SPECT[0]);
                PLOT_SPECTOGRAM_OPT( TRUE );
                //SetBitmapBits( HBSPECT, 4*WidthGraph*HeightGraph, &RGB_SPECT[0]);
                //SetBitmapBits( HBitmapP, 4*WidthGraph*HeightGraph, &RGB_SPECT[0]);
                //SendMessage( HWGraph, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)HBMP_IN );
                return FALSE;
                GetBitmapBits( HBMP_IN, 4 * WSPEC * HSPEC, &RGB_SPECT[0] );
                SetBitmapBits( HBSPECT, 4 * WSPEC * HSPEC, &RGB_SPECT[0] );
                PLOT_SPECTOGRAM_OPT( TRUE );
            }else{
                GetBitmapBits( HBMP_IN, 4*WidthGraph*HeightGraph, &RGB_BUFF[0]);
                SetBitmapBits( HBitmapP, 4*WidthGraph*HeightGraph, &RGB_BUFF[0]);
                SendMessage( HWGraph, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)HBMP_IN );
            }
        }
        return TRUE;
    }
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	BOOL ReDrawLim(){
		if( HWContainer==NULL ){
		   return FALSE;
		}

        char TTex[64] = {0};
	    HDC HDcT = GetDC( HWContainer );
    	SetBkMode( HDcT, TRANSPARENT);
    	SetTextAlign( HDcT, TA_CENTER );


    	RECT Re = {0};

    	Re.top = yPosGraph + HeightGraph + 5;
    	Re.bottom = yPosGraph + HeightGraph + 24;

    	Re.left = xPosGraph - 30;
    	Re.right = xPosGraph + 30;


    	// TextOut( HDcT, xPosGraph + WidthGraph, yPosGraph + HeightGraph + 5, "______", 6 );
    	FillRect( HDcT, &Re, HBrushC );

    	Re.left = xPosGraph + WidthGraph - 100;
    	Re.right = xPosGraph + WidthGraph + 38;
    	FillRect( HDcT, &Re, HBrushC );

        ///
        SetTextAlign( HDcT, TA_RIGHT );
    	sprintf( TTex, "%g %s", MaxXLim, TextMx );
    	TextOut( HDcT, xPosGraph + WidthGraph + 30, yPosGraph + HeightGraph + 5, TTex, strlen(TTex) );


        SetTextAlign( HDcT, TA_CENTER );
    	TextOut( HDcT, xPosGraph, yPosGraph + HeightGraph + 5, "      ", 6 );
    	sprintf( TTex, "%g", MinXLim );
    	TextOut( HDcT, xPosGraph, yPosGraph + HeightGraph + 5, TTex, strlen(TTex) );

    	ReleaseDC( HWContainer, HDcT );
        return TRUE;
	}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  PUBLIC:
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
public:
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	HWND GetHandleCon(){return HWContainer;}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	HWND GetHandleGraph(){return HWGraph;}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	BOOL isDockF( void ){return (IsDocked);}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	BOOL Dock( void ){
		IsDocked = !IsDocked;
		if( IsDocked ){
            SetParent( HWContainer, HWND_DESKTOP );
            SetWindowLong( HWContainer , GWL_STYLE, WS_POPUP | WS_BORDER );
            ShowWindow( HWContainer, 3 );
        }else{
            IsDockedMaxed = FALSE;
            SetParent( HWContainer, HWFatherP );
            SetWindowLong( HWContainer , GWL_STYLE, WS_CHILD | WS_BORDER );
            ShowWindow( HWContainer, 1 );
        }
        return IsDocked;
	}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    BOOL RefactBuff(){
        FreeAll();
		RGB_BUFF = (RGBQUAD*)malloc( WidthCon*HeightCon*sizeof(RGBQUAD) + 1024 );
		OLD_RGB_BUFF = (RGBQUAD*)malloc( WidthCon*HeightCon*sizeof(RGBQUAD) + 1024 );
		for( int y=0; y<HeightCon; y++ ){
			for( int x=0; x<WidthCon; x++ ){
				int ToIndex = x + WidthCon*y;
				RGB_BUFF[ToIndex] = ColorBack;
				OLD_RGB_BUFF[ToIndex] = ColorBack;
			}
		}
        return TRUE;
    }
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    BOOL SetRoundValues( BOOL STRV ){ RoundValues = STRV; return RoundValues;}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void PLOTEOS_INIT(){
	    FreeAll();
	    RoundValues = FALSE;
	    HFntGraph = CreateFont( 28, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, DEFAULT_CHARSET,
                                OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, PROOF_QUALITY,
                                    DEFAULT_PITCH | FF_ROMAN, "Times" );
        ///
		int CXSc = GetSystemMetrics( SM_CXSCREEN ) + 256;
		int CYSc = GetSystemMetrics( SM_CYSCREEN ) + 256;
		///
		WidthCon = CXSc;
		HeightCon = CYSc;
		RGB_BUFF = (RGBQUAD*)malloc( WidthCon*HeightCon*sizeof(RGBQUAD) + 1024 );
		OLD_RGB_BUFF = (RGBQUAD*)malloc( WidthCon*HeightCon*sizeof(RGBQUAD) + 1024 );
		for( int y=0; y<HeightCon; y++ ){
			for( int x=0; x<WidthCon; x++ ){
				int ToIndex = x + WidthCon*y;
				RGB_BUFF[ToIndex] = ColorBack;
				OLD_RGB_BUFF[ToIndex] = ColorBack;
			}
		}
		///
	}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    BOOL Hold( BOOL HoldI ){
        HoldV = HoldI;
        return HoldV;
    }
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	PLOTEOS(){
        PtrClass = 0;
        PtrClass = (int)(&PtrClass);
        OnSpect = FALSE;
        HoldV = FALSE;
        //DirClass[CntClass] = (int)(&PtrClass);
        //CntClass++;
        IsDocked = FALSE;
        IsDockedPermis = FALSE;
        Text2Show = NULL;
        Text2Title = NULL;
        HBrushC = CreateSolidBrush( RGB( RgbColor , RgbColor , RgbColor ) );
        HBSPECT = NULL;
        ReGrid = TRUE;
		LineWidth = 1;
		MaxXLim = 60;
		MaxYLim = 5;

		MinXLim = 0;
		MinYLim = 0;
        SpectOpt = 0;

        BVisible = TRUE;
        IsGrid_Inv = FALSE;
		HWContainer = NULL;
		xPosCon = 100;
		yPosCon = 100;
		WidthCon = 300;
		HeightCon = 300;

		xPosGraph = 40;
		yPosGraph = 38;

		DoubleBuffP = FALSE;

		ColorBack.rgbRed = 255;
		ColorBack.rgbGreen = 255;
		ColorBack.rgbBlue = 255;

		ColorLineG.rgbRed = 0;
		ColorLineG.rgbGreen = 0;
		ColorLineG.rgbBlue = 0;
		Grid_On = TRUE;
		Grid_Style = 2;
		PGrid_Style = 2;
		BText2Show = TRUE;
		TextMx = "Seg ";



		static char								PltClass[32];
		PltClass[0] = ' ';  PltClass[1] = 'P';PltClass[2] = 'L'; PltClass[3] = 'O';
		PltClass[4] = 'T';  PltClass[5] = 'E';PltClass[6] = 'R'; PltClass[7] = '_';
		PltClass[8] = 'C';  PltClass[9] = 'L';PltClass[10] = 'A';PltClass[11] = 'S';
		PltClass[12] = 'S'; PltClass[13] = '_';PltClass[14] = 'U';  PltClass[15] = 0;
		PltClass[16] = 'C'; PltClass[17] = 'C';PltClass[18] = '1';  PltClass[19] = 0;

		static	BOOL	Created;
		if( !Created ){
			Created = TRUE;
			PltClass[0] = ' ';  PltClass[1] = 'P';PltClass[2] = 'L'; PltClass[3] = 'O';
			PltClass[4] = 'T';  PltClass[5] = 'E';PltClass[6] = 'R'; PltClass[7] = '_';
			PltClass[8] = 'C';  PltClass[9] = 'L';PltClass[10] = 'A';PltClass[11] = 'S';
			PltClass[12] = 'S'; PltClass[13] = '_';PltClass[14] = 'U';  PltClass[15] = 0;
			PltClass[16] = 'C'; PltClass[17] = 'C';PltClass[18] = '1';  PltClass[19] = 0;
			//
			WNDCLASSEX wincl;
			wincl.hInstance = GetModuleHandle(0);
			wincl.lpszClassName = (LPCSTR)(PltClass);
			wincl.lpfnWndProc = (WNDPROC)PltWProc;
			wincl.style = CS_DBLCLKS |  CS_HREDRAW | CS_VREDRAW;
			wincl.style = CS_DBLCLKS;
			wincl.cbSize = sizeof( WNDCLASSEX );
			wincl.hIcon = NULL;
			wincl.hIconSm = NULL;
			wincl.hCursor = LoadCursor( NULL, IDC_ARROW);
			wincl.lpszMenuName = NULL ;
			wincl.cbClsExtra = 0;
			wincl.cbWndExtra = 0;
			wincl.hbrBackground = (HBRUSH)COLOR_BACKGROUND;
			//#define RgbColor 238

			wincl.hbrBackground = (HBRUSH)CreateSolidBrush( RGB( RgbColor , RgbColor , RgbColor ) );
			if (!RegisterClassEx (&wincl)){
				MessageBox( NULL, "RegisterClassEx - PLOTEOS", " Error ", 16 );
				exit( -1 );
				return;
			}
		}
		for( int i=0; i<32; i++ ){ PltClassP[i] = PltClass[i];}


		FuenteM = CreateFont( 18, 0, 0, 0, FW_BOLD, TRUE, FALSE, FALSE,
							DEFAULT_CHARSET, OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS,
							PROOF_QUALITY, DEFAULT_PITCH | FF_ROMAN,
							"Times New Roman" );

	}
///////////////-///////////////////////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	~PLOTEOS(){
        FreeAll();
		DeleteObject( FuenteM );
		DeleteObject( HBSPECT );
		DeleteObject( HFntGraph );
		DeleteObject( HFntNumberY );
	}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	BOOL SetFatherA( HWND HWFather ){
		HWFatherP = HWFather;
		SetParent( HWContainer, HWFatherP );
		return ShowWindow( HWContainer, (1&((int)BVisible)) );
	}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	BOOL SetFatherPOP( HWND HWFather ){
		HWFatherP = HWFather;
		DestroyWindow( HWContainer );
		char WTitlePlt[64] = {0};
		sprintf( WTitlePlt, "%i", PtrClass );
		HWContainer = CreateWindowEx( 	WS_EX_TOOLWINDOW | WS_EX_TOPMOST, PltClassP, WTitlePlt,
										//WS_OVERLAPPEDWINDOW |
										WS_BORDER |
										WS_POPUP
										//WS_CHILD
										,
										xPosCon, yPosCon, WidthCon, HeightCon, HWFatherP, NULL,
										GetModuleHandle(0), NULL );
		HWGraph = CreateWindowEx( 	0, "STATIC", NULL,
									WS_CHILD | WS_VISIBLE |
									//WS_BORDER |
									SS_BITMAP,
									xPosGraph, yPosGraph, WidthGraph, HeightGraph, HWContainer, NULL,
									GetModuleHandle(0), NULL );
		HWSText = CreateWindowEx( 	0, "STATIC", "X:\nY:", WS_CHILD | WS_VISIBLE | WS_BORDER,
									xPosGraph, 1, 104, yPosGraph-2, HWContainer, NULL,
									//xPosGraph, 1, 290, yPosGraph-2, HWContainer, NULL,
									GetModuleHandle(0), NULL );

		SendMessage( HWSText, WM_SETFONT, (WPARAM)FuenteM, MAKELPARAM(TRUE, 0) );

		SetParent( HWContainer, HWFatherP );
		return ShowWindow( HWContainer, (1&((int)BVisible)) );
	}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	BOOL SetFather( HWND HWFather ){
		HWFatherP = HWFather;
		DestroyWindow( HWContainer );
		char WTitlePlt[64] = {0};
		sprintf( WTitlePlt, "%i", PtrClass );
		HWContainer = CreateWindowEx( 	WS_EX_TOOLWINDOW, PltClassP, WTitlePlt,
										//WS_OVERLAPPEDWINDOW |
										WS_BORDER |
										//WS_POPUP |
										WS_CHILD,
										xPosCon, yPosCon, WidthCon, HeightCon, HWFatherP, NULL,
										GetModuleHandle(0), NULL );
		HWGraph = CreateWindowEx( 	0, "STATIC", NULL,
									WS_CHILD | WS_VISIBLE |
									//WS_BORDER |
									SS_BITMAP,
									xPosGraph, yPosGraph, WidthGraph, HeightGraph, HWContainer, NULL,
									GetModuleHandle(0), NULL );
		HWSText = CreateWindowEx( 	0, "STATIC", "X:\nY:", WS_CHILD | WS_VISIBLE | WS_BORDER,
									xPosGraph, 1, 104, yPosGraph-2, HWContainer, NULL,
									//xPosGraph, 1, 290, yPosGraph-2, HWContainer, NULL,
									GetModuleHandle(0), NULL );

		SendMessage( HWSText, WM_SETFONT, (WPARAM)FuenteM, MAKELPARAM(TRUE, 0) );
        ///
        ///
        ///
        ///
        const int XOPT = xPosGraph + WidthGraph - 40;
        HW_SPECTOPT = CreateWindowEx( 0, "combobox", NULL,
                       (WS_CHILD | WS_BORDER | CBS_DROPDOWNLIST | WS_TABSTOP | WS_VSCROLL),
									XOPT,
									/// 1,
									2, 40, 120, HWContainer, (HMENU)1,
									GetModuleHandle(0), NULL );
        SendMessage( HW_SPECTOPT, CB_ADDSTRING, 0, (LPARAM)"0" );
        SendMessage( HW_SPECTOPT, CB_ADDSTRING, 0, (LPARAM)"1" );
        SendMessage( HW_SPECTOPT, CB_ADDSTRING, 0, (LPARAM)"2" );
        SendMessage( HW_SPECTOPT, CB_ADDSTRING, 0, (LPARAM)"3" );
        SendMessage( HW_SPECTOPT, CB_ADDSTRING, 0, (LPARAM)"4" );
        SpectOpt = 2;
		SendMessage( HW_SPECTOPT, CB_SETCURSEL, SpectOpt, 0 );
        ///
        ///
        ///
        ///ASDASD
        DestroyWindow( HWSTextInfoAdd );
		HWSTextInfoAdd = CreateWindowEx( 	0, "STATIC", "", WS_CHILD,// | WS_BORDER,
									xPosGraph, 1, 104, yPosGraph-2, HWContainer, NULL,
									//xPosGraph, 1, 290, yPosGraph-2, HWContainer, NULL,
									GetModuleHandle(0), NULL );

        HFONT HFNT = CreateFont( 15, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET,
                                    OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, PROOF_QUALITY,
                                        DEFAULT_PITCH | FF_ROMAN, "Times New Roman" );
        SendMessage( HWSTextInfoAdd, WM_SETFONT, (WPARAM)HFNT, MAKELPARAM(TRUE, 0) );
        ///
        ///
        ///
		SetParent( HWContainer, HWFatherP );
		return ShowWindow( HWContainer, (1&((int)BVisible)) );
	}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    BOOL SetTextInfoAddVisible( BOOL Set ){
        ShowWindow( HWSTextInfoAdd, Set );
        return TRUE;
    }
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    BOOL SetTextInfoAdd( LPCSTR LPS ){
        SetWindowText( HWSTextInfoAdd, LPS );
        return TRUE;
    }
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	BOOL SetTextVisible( BOOL Opt_Visible ){
		return ShowWindow( HWSText, (1&((int)Opt_Visible)) );
	}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	BOOL SetVisible( BOOL Opt_Visible ){
		BVisible = Opt_Visible;
		if( IsDocked ){
            return ShowWindow( HWContainer, (3*((int)BVisible)) );
        }
		return ShowWindow( HWContainer, (1&((int)BVisible)) );
	}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	BOOL TrySetSizeCon( int WidthC, int HeightC ){
		if( (WidthCon==WidthC && HeightCon==HeightC) ){return FALSE;}
		return TRUE;
	}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	BOOL SetReGrid( void ){
		ReGrid = TRUE;
		return ReGrid;
	}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	BOOL SetSizeConHard( int WidthC, int HeightC ){
		ReGrid = TRUE;
		WidthCon = 0;
		HeightCon = 0;
		if( WidthC<150 ){WidthC = 150;}
		if( HeightC<150 ){HeightC = 150;}
		return SetSizeCon( WidthC, HeightC );
	}
//
private:
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	BOOL SetSizeConP( void ){
		ReGrid = TRUE;
		xPosGraph = 40;
		//xPosGraph = 1;
		yPosGraph = 38;
		WidthGraph = WidthCon - xPosGraph - 5 - 25;
		//WidthGraph = WidthCon - xPosGraph - 5;
		HeightGraph = HeightCon - yPosGraph - 30;
		//HeightGraph = HeightCon - yPosGraph - 5;
		if( WidthGraph<1 ){WidthGraph=1;}
		if( HeightGraph<1 ){HeightGraph=1;}
		MoveWindow( HWGraph, xPosGraph, yPosGraph, WidthGraph, HeightGraph, TRUE );
		DeleteObject( HBitmapP );
		HBitmapP = NewBitmap( WidthGraph, HeightGraph );
		SendMessage( HWGraph, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)HBitmapP );
		return TRUE;//MoveWindow( HWContainer, xPosCon, yPosCon, WidthCon, HeightCon, TRUE );
	}
//
public:
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	BOOL SetSizeCon( int WidthC, int HeightC ){
		if( (WidthCon==WidthC && HeightCon==HeightC) || IsDocked ){return FALSE;}
		ReGrid = TRUE;
		///
		if( WidthC<150 ){WidthC = 150;}
		if( HeightC<150 ){HeightC = 150;}
		///
		if( (WidthC%2)!=0 ){WidthC--;}
		if( (HeightC%2)!=0 ){HeightC--;}
		///
		WidthCon = WidthC;
		HeightCon = HeightC;
        ///
        RefactBuff();
        ///
		xPosGraph = 40;
		//xPosGraph = 1;

		yPosGraph = 38;

		WidthGraph = WidthCon - xPosGraph - 5 - 25;
		//WidthGraph = WidthCon - xPosGraph - 5;

		HeightGraph = HeightCon - yPosGraph - 30;
		//HeightGraph = HeightCon - yPosGraph - 5;

		if( WidthGraph<1 ){WidthGraph=1;}
		if( HeightGraph<1 ){HeightGraph=1;}
        ///
        ///
        ///
        ///
        const int XOPT = xPosGraph + WidthGraph - 40;
		MoveWindow( HW_SPECTOPT, XOPT, yPosGraph - 26, 40, 120, TRUE );
		///
		///
		///ASDXY
		MoveWindow( HWSTextInfoAdd, WidthCon - 193, 1, 190, 35, TRUE );
		///
		///
		MoveWindow( HWGraph, xPosGraph, yPosGraph, WidthGraph, HeightGraph, TRUE );
		DeleteObject( HBitmapP );
		HBitmapP = NewBitmap( WidthGraph, HeightGraph );

		SendMessage( HWGraph, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)HBitmapP );

		return MoveWindow( HWContainer, xPosCon, yPosCon, WidthCon, HeightCon, TRUE );
	}
///////////////////////////////////////////////////////////////////////////////////////////////////////////
int GetXCon( void ){
	return xPosCon;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////
int GetYCon( void ){
	return yPosCon;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	BOOL SetLocationCon( int xPosC, int yPosC ){
		xPosCon = xPosC;
		yPosCon = yPosC;
		if( IsDocked ){
            return FALSE;
        }
		return MoveWindow( HWContainer, xPosCon, yPosCon, WidthCon, HeightCon, TRUE );
	}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	int GetGraphWidth( void ){
		return WidthGraph;
	}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	int GetGraphHeight( void ){
		return HeightGraph;
	}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	int GetConWidth( void ){
		return WidthCon;
	}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	int GetConHeight( void ){
		return HeightCon;
	}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	BOOL SetDoubleBuff( BOOL Opt2Buff ){
		DoubleBuffP = Opt2Buff;
		return DoubleBuffP;
	}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	inline BOOL setRGB( int x, int y, RGBQUAD RGBColor ){
		//if( x<0 || y<0 || x>=WidthGraph || y>=HeightGraph ){
			//MessageBox( HWContainer, "BOOL setRGB( int x, int y, RGBQUAD RGBColor );", " Error ", 16  );
			//exit( -10 );
			//return FALSE;
		//}
		if( x<0 ){x = 0;}
		if( x>=WidthGraph ){x = WidthGraph - 1;}

		if( y<0 ){y = 0;}
		if( y>=HeightGraph ){y = HeightGraph - 1;}

		int ToIndex = x + WidthGraph*y;
		RGB_BUFF[ToIndex] = RGBColor;
		return TRUE;
	}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	inline BOOL setRGB_Spec( int x, int y, RGBQUAD RGBColor ){
		/*
		if( x<0 || y<0 || x>=WidthGraph || y>=HeightGraph ){
			MessageBox( HWContainer, "BOOL setRGB( int x, int y, RGBQUAD RGBColor );", " Error ", 16  );
			exit( -10 );
			return FALSE;
		}
		//*/
		if( x<0 ){x = 0;}
		if( x>=WSPEC ){x = WSPEC - 1;}
		///
		if( y<0 ){y = 0;}
		if( y>=HSPEC ){y = HSPEC - 1;}
		///
		if( RGB_SPECT==NULL ){
            return FALSE;
		}
		RGB_SPECT[x + WSPEC*y] = RGBColor;
		return TRUE;
	}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    BOOL PLOTSHORT( short* SBUF, int NFrames ){
        return PLOTSHORT( SBUF, NFrames, 0, TRUE );
    }
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL SetBackG( BYTE R, BYTE G, BYTE B ){
	ColorBack.rgbRed = R;
	ColorBack.rgbGreen = G;
	ColorBack.rgbBlue = B;

	ColorLineG.rgbRed = ~R;
	ColorLineG.rgbGreen = ~G;
	ColorLineG.rgbBlue = ~B;
	return TRUE;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL SetBackG( BYTE Col ){
	ColorBack.rgbRed = Col;
	ColorBack.rgbGreen = Col;
	ColorBack.rgbBlue = Col;

	ColorLineG.rgbRed = ~Col;
	ColorLineG.rgbGreen = ~Col;
	ColorLineG.rgbBlue = ~Col;
	ReGrid = TRUE;
	return TRUE;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL SetGridStyle( BYTE GStyle ){
    GStyle = 3&GStyle;
    ActGrid_Style = GStyle;
    if( GStyle!=0 ){
        Grid_Style = (int)(1<<GStyle);
    }else{
        Grid_Style = GStyle;
    }
    PGrid_Style = Grid_Style;
    ReGrid = TRUE;
	return TRUE;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////
int GetGridStyle( void ){
	return ActGrid_Style;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL ActGridStyle( BOOL OptStyle ){
    ReGrid = TRUE;
    if( OptStyle ){Grid_Style = PGrid_Style;return TRUE;}
    Grid_Style = 1;
	return TRUE;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL IsGridInv( void ){
	return IsGrid_Inv;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL InvGridStyle( BOOL InvFlag ){
    IsGrid_Inv = InvFlag;
    if( IsGrid_Inv ){
    ColorBack.rgbRed = 0;
	ColorBack.rgbGreen = 0;
	ColorBack.rgbBlue = 0;

	ColorLineG.rgbRed = 255;
	ColorLineG.rgbGreen = 255;
	ColorLineG.rgbBlue = 255;
    }else{
          ColorBack.rgbRed = 255;
        	ColorBack.rgbGreen = 255;
        	ColorBack.rgbBlue = 255;

        	ColorLineG.rgbRed = 0;
        	ColorLineG.rgbGreen = 0;
        	ColorLineG.rgbBlue = 0;
    }

	ReGrid = TRUE;
	return TRUE;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////
RGBQUAD GetBkg( void ){
    return ColorBack;
}
/// ////////////////////////////////////////////////////////////////////////////////////////////////////////
/// BOOL SetMaxXLim( double MXLim ){ return SetMaxXLim( MXLim, TRUE ); }
BOOL SetMaxXLim( double MXLim, BOOL ForceRedraw ){
    if( MXLim!=MaxXLim ){
        MaxXLim = MXLim;
        if( BVisible || ForceRedraw ){
            ReDrawLim();
        }
    }
	return TRUE;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL SetMaxYLim( double MYLim ){
	MaxYLim = MYLim;
	/// ReDrawLim();
	return TRUE;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL SetMinXLim( double MXLim, BOOL ForceRedraw ){
    if( MXLim!=MinXLim ){
        MinXLim = MXLim;
        if( BVisible || ForceRedraw ){
            ReDrawLim();
        }
    }
	return TRUE;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL SetMinYLim( double MYLim ){
	MinYLim = MYLim;
	/// ReDrawLim();
	return TRUE;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL SetBText2Show( BOOL TBText2Show ){
	BText2Show = TBText2Show;
	InvalidateRect( HWContainer, NULL, TRUE );
	return BText2Show;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL Text2ShowF( const char* Tx2Sh ){
    if( Text2Show!=NULL ){free( Text2Show );}
    int LSTR = strlen( Tx2Sh );
    Text2Show = (char*)malloc( LSTR + 16 );
    //sprintf( Text2Show, " Usted está viendo la ultima adquisición realizada " );
	strcpy( Text2Show, Tx2Sh );
    //( Text2Show, " Usted está viendo la ultima adquisición realizada " );
    InvalidateRect( HWContainer, NULL, TRUE );
    UpdateWindow( HWContainer );
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL SetTitle( const char* Tx2Sh ){
    if( Text2Title!=NULL ){free( Text2Title );}
    int LSTR = strlen( Tx2Sh );
    Text2Title = (char*)malloc( LSTR + 16 );
    //sprintf( Text2Show, " Usted está viendo la ultima adquisición realizada " );
	strcpy( Text2Title, Tx2Sh );
    //( Text2Show, " Usted está viendo la ultima adquisición realizada " );
    InvalidateRect( HWContainer, NULL, TRUE );
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL Text2ShowF( int ITx2Sh ){
    if( Text2Show!=NULL ){free( Text2Show );}
    Text2Show = (char*)malloc( 256 + 16 );
    memset( &Text2Show[0], 0, 256 );
    LoadString( GetModuleHandle( NULL ), ITx2Sh, &Text2Show[0], 255 );
    InvalidateRect( HWContainer, NULL, TRUE );
	return TRUE;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL Inval( void ){
	InvalidateRect( HWContainer, NULL, TRUE );
	return TRUE;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	BOOL Redraw( void ){
		return InvalidateRect( HWContainer, NULL, TRUE );
	}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    BOOL SetLineColor( RGBQUAD Color ){
        ColorLineG = Color;
        return TRUE;
    }
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    BOOL SetLineColor( BYTE R, BYTE G, BYTE B ){
        ColorLineG.rgbRed   = R;
        ColorLineG.rgbGreen = G;
        ColorLineG.rgbBlue  = B;
        return TRUE;
    }
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    BOOL SetTextMiddle( LPCSTR LPS ){
        /// HBitmapP
        HDC OHDC = GetDC( HWGraph );
        HDC NHDC = CreateCompatibleDC( OHDC );
        SelectObject( NHDC, HBitmapP );
        SelectObject( NHDC, HFntGraph );
        ///
        ///SetBkMode( NHDC, TRANSPARENT);
        SetBkMode( NHDC, OPAQUE);
        SetBkColor( NHDC, RGB(255,255,255));
        SetBkColor( NHDC, RGB(0,0,0));
        SetTextAlign( NHDC, TA_CENTER );
        SetTextColor( NHDC, RGB(255,0,0) );
        TextOut( NHDC, WidthGraph/2, HeightGraph/2, LPS, strlen(LPS) );
        ///
        ReleaseDC( HWGraph, OHDC );
        DeleteDC( NHDC );
        return TRUE;
    }
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////
};

// PLOTEOS      Pltrs[2];
//PLOTEOS      Pltrs2;
