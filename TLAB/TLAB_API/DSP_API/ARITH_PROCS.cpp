BOOL DIM2::ARITH_PROCS( RGBQUAD* RgbIO, int Len_Buff,
                            int ArithType, int SumOper,
                                double ProdOper, int LowValue,
                                    int HiValue, BOOL BSetPix, BOOL InvOut ){
    ///
    ///
    ///
    ///
    ///
    if( BSetPix ){
        switch( ArithType ){
            case 0:
                if( InvOut ){
                    for( int i=0; i<Len_Buff; i++ ){
                        RgbIO[i].rgbRed   = ~ParseByte( RgbIO[i].rgbRed      + SumOper );
                        RgbIO[i].rgbGreen = ~ParseByte( RgbIO[i].rgbGreen    + SumOper );
                        RgbIO[i].rgbBlue  = ~ParseByte( RgbIO[i].rgbBlue     + SumOper );
                    }
                }else{
                    for( int i=0; i<Len_Buff; i++ ){
                        RgbIO[i].rgbRed   = ParseByte( RgbIO[i].rgbRed      + SumOper );
                        RgbIO[i].rgbGreen = ParseByte( RgbIO[i].rgbGreen    + SumOper );
                        RgbIO[i].rgbBlue  = ParseByte( RgbIO[i].rgbBlue     + SumOper );
                    }
                }
                IsBin = FALSE;
                break;
            case 1:
                if( InvOut ){
                    for( int i=0; i<Len_Buff; i++ ){
                        RgbIO[i].rgbRed   = ~ParseByte( ProdOper * RgbIO[i].rgbRed );
                        RgbIO[i].rgbGreen = ~ParseByte( ProdOper * RgbIO[i].rgbGreen );
                        RgbIO[i].rgbBlue  = ~ParseByte( ProdOper * RgbIO[i].rgbBlue );
                    }
                }else{
                    for( int i=0; i<Len_Buff; i++ ){
                        RgbIO[i].rgbRed   = ParseByte( ProdOper * RgbIO[i].rgbRed );
                        RgbIO[i].rgbGreen = ParseByte( ProdOper * RgbIO[i].rgbGreen );
                        RgbIO[i].rgbBlue  = ParseByte( ProdOper * RgbIO[i].rgbBlue );
                    }
                }
                IsBin = FALSE;
                break;
            case 2:
                if( InvOut ){
                    for( int i=0; i<Len_Buff; i++ ){
                        RgbIO[i].rgbRed   = ~( RgbIO[i].rgbRed*( RgbIO[i].rgbRed>LowValue      && RgbIO[i].rgbRed<HiValue ) );
                        RgbIO[i].rgbGreen = ~( RgbIO[i].rgbGreen*( RgbIO[i].rgbGreen>LowValue    && RgbIO[i].rgbGreen<HiValue ) );
                        RgbIO[i].rgbBlue  = ~( RgbIO[i].rgbBlue*( RgbIO[i].rgbBlue>LowValue     && RgbIO[i].rgbBlue<HiValue ) );
                    }
                    IsGray = TRUE;
                    IsBin = TRUE;
                }else{
                    for( int i=0; i<Len_Buff; i++ ){
                        RgbIO[i].rgbRed   = RgbIO[i].rgbRed*( RgbIO[i].rgbRed>LowValue      && RgbIO[i].rgbRed<HiValue );
                        RgbIO[i].rgbGreen = RgbIO[i].rgbGreen*( RgbIO[i].rgbGreen>LowValue    && RgbIO[i].rgbGreen<HiValue );
                        RgbIO[i].rgbBlue  = RgbIO[i].rgbBlue*( RgbIO[i].rgbBlue>LowValue     && RgbIO[i].rgbBlue<HiValue );
                    }
                    IsGray = TRUE;
                    IsBin = TRUE;
                }
                break;
        }
    }else{
        switch( ArithType ){
            case 0:
                if( InvOut ){
                    for( int i=0; i<Len_Buff; i++ ){
                        RgbIO[i].rgbRed   = ~ParseByte( RgbIO[i].rgbRed      + SumOper );
                        RgbIO[i].rgbGreen = ~ParseByte( RgbIO[i].rgbGreen    + SumOper );
                        RgbIO[i].rgbBlue  = ~ParseByte( RgbIO[i].rgbBlue     + SumOper );
                    }
                }else{
                    for( int i=0; i<Len_Buff; i++ ){
                        RgbIO[i].rgbRed   = ParseByte( RgbIO[i].rgbRed      + SumOper );
                        RgbIO[i].rgbGreen = ParseByte( RgbIO[i].rgbGreen    + SumOper );
                        RgbIO[i].rgbBlue  = ParseByte( RgbIO[i].rgbBlue     + SumOper );
                    }
                }
                break;
            case 1:
                if( InvOut ){
                    for( int i=0; i<Len_Buff; i++ ){
                        RgbIO[i].rgbRed   = ~ParseByte( ProdOper * RgbIO[i].rgbRed );
                        RgbIO[i].rgbGreen = ~ParseByte( ProdOper * RgbIO[i].rgbGreen );
                        RgbIO[i].rgbBlue  = ~ParseByte( ProdOper * RgbIO[i].rgbBlue );
                    }
                }else{
                    for( int i=0; i<Len_Buff; i++ ){
                        RgbIO[i].rgbRed   = ParseByte( ProdOper * RgbIO[i].rgbRed );
                        RgbIO[i].rgbGreen = ParseByte( ProdOper * RgbIO[i].rgbGreen );
                        RgbIO[i].rgbBlue  = ParseByte( ProdOper * RgbIO[i].rgbBlue );
                    }
                }
                break;
            case 2:
                if( InvOut ){
                    if( LowValue>HiValue ){
                        for( int i=0; i<Len_Buff; i++ ){
                            RgbIO[i].rgbRed   = ~( 255*(int)( RgbIO[i].rgbRed>LowValue      || RgbIO[i].rgbRed<HiValue ) );
                            RgbIO[i].rgbGreen = ~( 255*(int)( RgbIO[i].rgbGreen>LowValue    || RgbIO[i].rgbGreen<HiValue ) );
                            RgbIO[i].rgbBlue  = ~( 255*(int)( RgbIO[i].rgbBlue>LowValue     || RgbIO[i].rgbBlue<HiValue ) );
                        }
                    }else{
                        for( int i=0; i<Len_Buff; i++ ){
                            RgbIO[i].rgbRed   = ~( 255*(int)( RgbIO[i].rgbRed>LowValue      && RgbIO[i].rgbRed<HiValue ) );
                            RgbIO[i].rgbGreen = ~( 255*(int)( RgbIO[i].rgbGreen>LowValue    && RgbIO[i].rgbGreen<HiValue ) );
                            RgbIO[i].rgbBlue  = ~( 255*(int)( RgbIO[i].rgbBlue>LowValue     && RgbIO[i].rgbBlue<HiValue ) );
                        }
                    }
                }else{
                    /// char T2T[1024] = {0};sprintf( T2T, "%i\n%i", LowValue, HiValue );MessageBoxA( NULL, T2T, "", 0 );
                    if( LowValue>HiValue ){
                        for( int i=0; i<Len_Buff; i++ ){
                            RgbIO[i].rgbRed   = 255*(int)( RgbIO[i].rgbRed>LowValue      || RgbIO[i].rgbRed<HiValue );
                            RgbIO[i].rgbGreen = 255*(int)( RgbIO[i].rgbGreen>LowValue    || RgbIO[i].rgbGreen<HiValue );
                            RgbIO[i].rgbBlue  = 255*(int)( RgbIO[i].rgbBlue>LowValue     || RgbIO[i].rgbBlue<HiValue );
                        }
                    }else{
                        for( int i=0; i<Len_Buff; i++ ){
                            RgbIO[i].rgbRed   = 255*(int)( RgbIO[i].rgbRed>LowValue      && RgbIO[i].rgbRed<HiValue );
                            RgbIO[i].rgbGreen = 255*(int)( RgbIO[i].rgbGreen>LowValue    && RgbIO[i].rgbGreen<HiValue );
                            RgbIO[i].rgbBlue  = 255*(int)( RgbIO[i].rgbBlue>LowValue     && RgbIO[i].rgbBlue<HiValue );
                        }
                    }
                }
                IsGray = TRUE;
                IsBin = TRUE;
                break;
        }
    }
    ///
    ///
    ///
    return TRUE;
}
