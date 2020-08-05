BOOL PLOTEOS::FillBuff(){
     SetBitmapBits( HBitmapP, 4*WidthGraph*HeightGraph, &RGB_BUFF[0]);
     SendMessage( HWGraph, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)HBitmapP );
     return TRUE;
     if( DoubleBuffP ){
         HDC OHDC = GetDC( HWGraph );
         HDC NHDC = CreateCompatibleDC( OHDC );
         SelectObject( NHDC, HBitmapP );
         BitBlt( OHDC, 0, 0, WidthGraph, HeightGraph, NHDC, 0, 0, SRCCOPY );
         ReleaseDC( HWGraph, OHDC );
         DeleteDC( NHDC );
     }else{
         SendMessage( HWGraph, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)HBitmapP );
     }
     return TRUE;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL PLOTEOS::FillBuff_Only(){
    SetBitmapBits( HBitmapP, 4*WidthGraph*HeightGraph, &RGB_BUFF[0]);
    return TRUE;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL PLOTEOS::FillGrid(){ return FillGrid(TRUE);}
///////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL	PLOTEOS::FillGrid( BOOL OptRepint ){
	if( RGB_BUFF==NULL || OLD_RGB_BUFF==NULL ){
		MessageBox( HWGraph , "RGB_BUFF==NULL || OLD_RGB_BUFF==NULL", " Error ", 16 );
        exit( -12345 );
	}
	ShowWindow( HW_SPECTOPT, 0 );
	int W_Grid = WidthGraph;
	int H_Grid = HeightGraph;
	if( !ReGrid ){
	   memcpy( RGB_BUFF, OLD_RGB_BUFF, 4*W_Grid*H_Grid );
       if( OptRepint ){
           FillBuff();
       }
       return TRUE;
	}
	int IndDivY = 0;
	RGBQUAD RgbLG = ColorBack;
	RgbLG.rgbRed    = ~RgbLG.rgbRed;
	RgbLG.rgbGreen  = ~RgbLG.rgbGreen;
	RgbLG.rgbBlue   = ~RgbLG.rgbBlue;
	ZeroMemory( RGB_BUFF, 4*W_Grid*H_Grid );
    for( int y=0; y<H_Grid; y++ ){
		int IndDivX = 0;
		for( int x=0; x<W_Grid; x++ ){
			//int FromIndex = x + W_Grid*y;
			//RGB_BUFF[ToIndex] = RGBColor;
			setRGB( x, y, ColorBack );
			//RGB_BUFF[FromIndex] = ColorBack;
			if( (IndDivY*H_Grid)/10==y ){
				setRGB( x, y, RgbLG );
				//ColorRGB[FromIndex] = ColorLineG;
            }else{
                if( (IndDivX*W_Grid)/10==x ){
                    setRGB( x, y, RgbLG );
                    //ColorRGB[FromIndex] = ColorLineG;
                    IndDivX++;
                }else{
                    if( x==W_Grid-1 || y==H_Grid-1 ){
                        setRGB( x, y, RgbLG );
                    }
				//ColorRGB[FromIndex] = ColorLineG;
                }
            }
		}/// LOOP X
		if ((IndDivY*H_Grid)/10==y) {
		   IndDivY++;
		}
    }




        int AdvGrid = 0;
        if( Grid_On ){
           for( int y=W_Grid/100; y<H_Grid-W_Grid/100; y = y + 1 ){
               for( int x=W_Grid/100; x<W_Grid-W_Grid/100; x = x + 1 ){
                   int FromIndex = x + W_Grid*y;
                   if( RGB_BUFF[FromIndex].rgbBlue==ColorLineG.rgbBlue ){
                      AdvGrid++;
                      if( AdvGrid==Grid_Style ){
                         RGB_BUFF[FromIndex] = ColorBack;
                         AdvGrid = 0;
                      }
                   }
               }
           }
        }else{
           for( int y=W_Grid/100; y<H_Grid-W_Grid/100; y = y + 1 ){
               for( int x=W_Grid/100; x<W_Grid-W_Grid/100; x = x + 1 ){
                   int FromIndex = x + W_Grid*y;
                   RGB_BUFF[FromIndex] = ColorBack;
               }
           }
        }

        memcpy( OLD_RGB_BUFF, RGB_BUFF, 4*W_Grid*H_Grid );

        if( OptRepint ){
           FillBuff();
        }
        ReGrid = FALSE;
        return TRUE;


}

