#include <math.h>
#include <ctype.h>
/// ///////////////////////////////////////////////////////////////////
/// ///////////////////////////////////////////////////////////////////
unsigned char FType( LPSTR Type ){
    char t2t[strlen(Type)+2];
    for( int i=0; i<(int)strlen(Type)+1; i++ ){
        t2t[i] = tolower(Type[i]);
    }
    if( strcmp(t2t, "low")==0 ){
        return 0;
    }
    if( strcmp(t2t, "high")==0 ){
        return 1;
    }
    if( strcmp(t2t, "bandpass")==0 ){
        return 2;
    }
    if( strcmp(t2t, "stop")==0 ){
        return 3;
    }
    return 0;
}
/// ///////////////////////////////////////////////////////////////////
/// ///////////////////////////////////////////////////////////////////
double Get_Val_In_fc( int N, double* b, double* a, double fc ){
    const double pi2_NN = (fc*(pi_dbl));
    double ValbR = 0;
    double ValbI = 0;
    double ValaR = 0;
    double ValaI = 0;
    for( int n = 0; n<N; n++ ){
        ValbR = ValbR + b[n] * cos( pi2_NN*((double)n) );
        ValbI = ValbI + b[n] * sin( pi2_NN*((double)n) );
        ///
        ValaR = ValaR + a[n] * cos( pi2_NN*((double)n) );
        ValaI = ValaI + a[n] * sin( pi2_NN*((double)n) );
    }
    return ( sqrt( ValbR*ValbR + ValbI*ValbI )/sqrt( ValaR*ValaR + ValaI*ValaI ) );
}
/// ///////////////////////////////////////////////////////////////////
/// ///////////////////////////////////////////////////////////////////
int buttap( int n, double* preal, double* pimag, double* k ){
    const double Pi_Arg = pi_dbl/(2*n);
    const double Pi_2 = pi_dbl/2;
    int iri = 0;
    for( int i=1; i<=(n-1); i = i + 2){
        preal[iri] = cos( (Pi_Arg * i) + Pi_2 );
        pimag[iri] = sin( (Pi_Arg * i) + Pi_2 );
        iri++;
    }
    if( n%2==1 ){
        preal[iri] = -1;
        pimag[iri] = 0;
        iri++;
    }
    k[0] = 1;
    return (iri);
}
/// ///////////////////////////////////////////////////////////////////
/// ///////////////////////////////////////////////////////////////////
int butter( int n, double* WnV, LPSTR Type, double* b, double* a ){
    if( n==0 ){ b[0] = 1;a[0] = 1;return 1; }
    ZeroMemory( b, (2*n+500) * sizeof(double) );ZeroMemory( a, (2*n+500) * sizeof(double) );
    //const double Mfc = ((double)1)/sqrt(2);
    const double Fs = 2;
    const double Fs_2 = 4;
    const double Fs_3 = 8;
    const double Fs_4 = 16;
    const int NB = 2*n + 10;
	double u0 = 2 * Fs * tan( pi_dbl * WnV[0]/Fs );
	double u1 = 2 * Fs * tan( pi_dbl * WnV[1]/Fs );
    double preal[NB+1];
    double pimag[NB+1];
    double k = 0;
    /// //
    const int BLen = buttap( n, preal, pimag, &k );
    /// //
    const double W_0 = sqrt( u1 * u0 );
    const double B_w = u1 - u0;
    const double B_w_2 = B_w * B_w;
    const double W_0_2 = W_0 * W_0;
    const double W_0_4 = W_0_2 * W_0_2;
    const double B_w_2__Fs_2__by4 = (4 * B_w_2 * Fs_2);
    ///
    double ap2u = 0;
    double ap4 = 0;
    double ap3 = 0;
    double ap2 = 0;
    double ap1 = 0;
    double ap0 = 0;
    double u0_2 = 0;
    double preal_2;
    double pimag_2;
    int LConvb = 0;
    int LConva = 0;
    const int BLC = BLen - (n%2);
    static double polysb[ 4096 ];
    static double polysa[ 4096 ];
    ZeroMemory( polysb, 4096 * sizeof(double) );
    ZeroMemory( polysa, 4096 * sizeof(double) );
    const unsigned char IType = FType( Type );
    switch( IType ){
/// H I G H ////////////////////////////////////////////////////////////////
        case 1:
            u0 = 2 * Fs * tan( pi_dbl * (1 - WnV[0])/Fs );
/// L O W //////////////////////////////////////////////////////////////////
        case 0:{
            if( n==1 ){
                b[0] = b[1] = 1/(2*Fs/u0 + 1);
                a[0] = 1;
                a[1] = (1 - 2*Fs/u0) / (2*Fs/u0 + 1);
                if( IType==1 ){
                    b[1] = -b[1];
                    a[1] = -a[1];
                }
                return 2;
            }
            ///
            u0_2 = u0 * u0;
            preal_2 = preal[0] * preal[0];
            pimag_2 = pimag[0] * pimag[0];
            ap2u = 16 - 8 * preal[0] * u0 + u0_2 * ( preal_2 + pimag_2 );
            ap2  = ap2u/(u0_2);
            ap1  = (2 * u0_2 * (preal_2 + pimag_2) - 32)/(ap2u);
            ap0  = (16 + 8 * preal[0] * u0 + preal_2 * u0_2 + pimag_2 * u0_2)/(ap2u);
            ///
            polysa[0] = 1;
            polysa[1] = ap1;
            polysa[2] = ap0;
            ///
            ///polysb[0] = 1/ap2;
            ///polysb[1] = 2/ap2;
            ///polysb[2] = 1/ap2;
            ///
            b[0] = 1;a[0] = 1;
            ///LConvb = conv( polysb, 3, b, 1, b );
            LConva = conv( polysa, 3, a, 1, a );
            ///
            // printf( "%5.4f + %5.4fi\n", preal[0], pimag[0] );
            for( int i=1; i<BLC; i++ ){
                preal_2 = preal[i] * preal[i];
                pimag_2 = pimag[i] * pimag[i];
                ap2u = 16 - 8 * preal[i] * u0 + u0_2 * ( preal_2 + pimag_2 );
                ap2  = ap2u/(u0_2);
                ap1  = (2 * u0_2 * (preal_2 + pimag_2) - 32)/(ap2u);
                ap0  = (16 + 8 * preal[i] * u0 + preal_2 * u0_2 + pimag_2 * u0_2)/(ap2u);
                ///
                ///
                polysa[0] = 1;
                polysa[1] = ap1;
                polysa[2] = ap0;
                ///
                /// polysb[0] = 1/ap2;
                /// polysb[1] = 2/ap2;
                polysb[2] = 1/ap2;
                ///
                /// LConvb = conv( polysb, 3, b, LConvb, b );
                LConva = conv( polysa, 3, a, LConva, a );
            }
            //*/
            if( (n%2)==1 ){
                ///polysb[0] = 1/(4/u0 + 1);
                ///polysb[1] = 1/(4/u0 + 1);
                //
                polysa[0] = 1;
                polysa[1] = (1 - 4/u0)/(4/u0 + 1);
                ///LConvb = conv( polysb, 2, b, LConvb, b );
                LConva = conv( polysa, 2, a, LConva, a );
            }
            ///
            ZeroMemory( b, (2*n+10)*sizeof(double) );
            ZeroMemory( polysb, (2*n+10)*sizeof(double) );
            polysb[0] = 1;polysb[1] = 1;b[0] = 1;
            LConvb = conv( polysb, 2, b, 1, b );
            for( int i=1; i<n; i++ ){
                LConvb = conv( polysb, 2, b, LConvb, b );
            }
            {
                double S2a = sum( a, n+1 );
                double S2b = sum( b, n+1 );
                for( int i=0; i<n+1; i++ ){
                    b[i] = S2a * b[i]/S2b;
                }
            }
            if( IType==1 ){
                for( int i=1; i<(n+1); i = i + 2 ){
                    b[i] = -b[i];
                    a[i] = -a[i];
                }
            }
            ///
            ///
        }
            break;
///
///
///
/// B A N D P A S S  |/|/|/|/|/|/|/|/|/|/|/|/|/|/|/|/|/|/|/|/|/|/|/|/|/|/|/|/|/|/|/|/
        case 2:{
            ///
            if( n==1 ){
                b[0] = (2*B_w*Fs)/(4*Fs_2 + 2*B_w*Fs + W_0_2);
                b[1] = 0;
                b[2] = -b[0];
                ///
                a[0] = 1;
                a[1] = -(8*Fs_2 - 2*W_0_2)/(4*Fs_2 + 2*B_w*Fs + W_0_2);
                a[2] = ( 4*Fs_2 + W_0_2 - 2*B_w*Fs )/(4*Fs_2 + 2*B_w*Fs + W_0_2);
                return 3;
            }
            ///
            ///
            preal_2 = preal[0] * preal[0];
            pimag_2 = pimag[0] * pimag[0];
            ap4  = (4*B_w_2*Fs_2*pimag_2 + 4*B_w_2*Fs_2*preal_2 - 16*B_w*Fs_3*preal[0] - 4*B_w*Fs*W_0_2*preal[0] + 16*Fs_4 + 8*Fs_2*W_0_2 + W_0_4);
            ap3  = -(64*Fs_4 - 32*B_w*preal[0]*Fs_3 + 8*B_w*preal[0]*Fs*W_0_2 - 4*W_0_4)/ap4;
            ap2  = -(8*B_w_2*Fs_2*pimag_2 + 8*B_w_2*Fs_2*preal_2 - 96*Fs_4 + 16*Fs_2*W_0_2 - 6*W_0_4)/ap4;
            ap1  = -(64*Fs_4 + 32*B_w*preal[0]*Fs_3 - 8*B_w*preal[0]*Fs*W_0_2 - 4*W_0_4)/(ap4);
            ap0  = (16*Fs_4 + W_0_4 + 8*Fs_2*W_0_2 + 4*B_w_2*Fs_2*pimag_2 + 4*B_w_2*Fs_2*preal_2 + 16*B_w*Fs_3*preal[0]
                     + 4*B_w*Fs*W_0_2*preal[0])/(ap4);
            ///
            ///
            polysa[4] = ap0;
            polysa[3] = ap1;
            polysa[2] = ap2;
            polysa[1] = ap3;
            polysa[0] = 1;
            ///
            polysb[0] = B_w_2__Fs_2__by4/ap4;
            polysb[1] = 0;
            polysb[2] = -2*polysb[0];
            polysb[3] = 0;
            polysb[4] = polysb[0];
            ///
            b[0] = 1;a[0] = 1;
            LConvb = conv( polysb, 6, b, 1, b );
            LConva = conv( polysa, 6, a, 1, a );
            ///
            // printf( "%5.4f + %5.4fi\n", preal[0], pimag[0] );
            for( int i=1; i<BLC; i++ ){
                preal_2 = preal[i] * preal[i];
                pimag_2 = pimag[i] * pimag[i];
                ap4  = (4*B_w_2*Fs_2*pimag_2 + 4*B_w_2*Fs_2*preal_2 - 16*B_w*Fs_3*preal[i] - 4*B_w*Fs*W_0_2*preal[i] + 16*Fs_4 + 8*Fs_2*W_0_2 + W_0_4);
                ap3  = -(64*Fs_4 - 32*B_w*preal[i]*Fs_3 + 8*B_w*preal[i]*Fs*W_0_2 - 4*W_0_4)/ap4;
                ap2  = -(8*B_w_2*Fs_2*pimag_2 + 8*B_w_2*Fs_2*preal_2 - 96*Fs_4 + 16*Fs_2*W_0_2 - 6*W_0_4)/ap4;
                ap1  = -(64*Fs_4 + 32*B_w*preal[i]*Fs_3 - 8*B_w*preal[i]*Fs*W_0_2 - 4*W_0_4)/(ap4);
                ap0  = (16*Fs_4 + W_0_4 + 8*Fs_2*W_0_2 + 4*B_w_2*Fs_2*pimag_2 + 4*B_w_2*Fs_2*preal_2 + 16*B_w*Fs_3*preal[i]
                         + 4*B_w*Fs*W_0_2*preal[i])/(ap4);
                ///
                ///
                polysa[4] = ap0;
                polysa[3] = ap1;
                polysa[2] = ap2;
                polysa[1] = ap3;
                polysa[0] = 1;
                ///
                polysb[0] = B_w_2__Fs_2__by4/ap4;
                polysb[1] = 0;
                polysb[2] = -2*polysb[0];
                polysb[3] = 0;
                polysb[4] = polysb[0];
                ///
                LConvb = conv( polysb, 6, b, LConvb, b );
                LConva = conv( polysa, 6, a, LConvb, a );
            }
            ///
            if( (n%2)==1 ){
                polysb[0] = (2*B_w*Fs)/(4*Fs_2 + 2*B_w*Fs + W_0_2);
                polysb[1] = 0;
                polysb[2] = -polysb[0];
                ///
                polysa[0] = 1;
                polysa[1] = -(8*Fs_2 - 2*W_0_2)/(4*Fs_2 + 2*B_w*Fs + W_0_2);
                polysa[2] = ( 4*Fs_2 + W_0_2 - 2*B_w*Fs )/(4*Fs_2 + 2*B_w*Fs + W_0_2);
                LConvb = conv( polysb, 3, b, LConvb, b );
                LConva = conv( polysa, 3, a, LConvb, a );
            }
            //double ValFc = Get_Val_In_fc( LConvb, b, a, WnV[0] );
            for( int i=0; i<LConvb; i++ ){
                //b[i] = Mfc * b[i]/ValFc;
            }
            ///
            ///
        }
            break;
///
///
///
/// B A N D S T O P  |/|/|/|/|/|/|/|/|/|/|/|/|/|/|/|/|/|/|/|/|/|/|/|/|/|/|/|/|/|/|/|/
        case 3:{
            if( n==1 ){
                b[0] = (4*Fs_2 + W_0_2)/(4*Fs_2 + 2*B_w*Fs + W_0_2);
                b[1] = -(8*Fs_2 - 2*W_0_2)/(4*Fs_2 + 2*B_w*Fs + W_0_2);
                b[2] = (4*Fs_2 + W_0_2)/(4*Fs_2 + 2*B_w*Fs + W_0_2);
                ///
                a[0] = 1;
                a[1] = -(8*Fs_2 - 2*W_0_2)/(4*Fs_2 + 2*B_w*Fs + W_0_2);
                a[2] = ( 4*Fs_2 + W_0_2 - 2*B_w*Fs )/(4*Fs_2 + 2*B_w*Fs + W_0_2);
                return 3;
            }
            ///
            preal_2 = preal[0] * preal[0];
            pimag_2 = pimag[0] * pimag[0];
            ap4  = (4*B_w_2*Fs_2 - 16*B_w*Fs_3*preal[0] - 4*B_w*Fs*W_0_2*preal[0] + 16*Fs_4*pimag_2 + 16*Fs_4*preal_2 + 8*Fs_2*W_0_2*pimag_2 + 8*Fs_2*W_0_2*preal_2 + W_0_4*pimag_2 + W_0_4*preal_2);
            ap3  = -(64*Fs_4*pimag_2 + 64*Fs_4*preal_2 - 32*B_w*Fs_3*preal[0] + 8*B_w*Fs*W_0_2*preal[0] - 4*W_0_4*pimag_2 - 4*W_0_4*preal_2) /ap4;
            ap2  = (96*Fs_4*pimag_2 - 8*B_w_2*Fs_2 + 96*Fs_4*preal_2 - 16*Fs_2*W_0_2*pimag_2 - 16*Fs_2*W_0_2*preal_2 + 6*W_0_4*pimag_2 + 6*W_0_4*preal_2)/ap4;
            ap1  = -(64*Fs_4*pimag_2 + 64*Fs_4*preal_2 + 32*B_w*Fs_3*preal[0] - 8*B_w*Fs*W_0_2*preal[0] - 4*W_0_4*pimag_2 - 4*W_0_4*preal_2)/(ap4);
            ap0  = (4*B_w_2*Fs_2 + 16*Fs_4*pimag_2 + 16*Fs_4*preal_2 + W_0_4*pimag_2 + W_0_4*preal_2 + 8*Fs_2*W_0_2*pimag_2 + 8*Fs_2*W_0_2*preal_2 + 16*B_w*Fs_3*preal[0] + 4*B_w*Fs*W_0_2*preal[0])/(ap4);
            ///
            ///
            polysa[4] = ap0;
            polysa[3] = ap1;
            polysa[2] = ap2;
            polysa[1] = ap3;
            polysa[0] = 1;
            ///
            polysb[0] = (16*Fs_4 + 8*Fs_2*W_0_2 + W_0_4)/ap4;
            polysb[1] = -(64*Fs_4 - 4*W_0_4)/ap4;
            polysb[2] = (96*Fs_4 - 16*Fs_2*W_0_2 + 6*W_0_4)/ap4;
            polysb[3] = -(64*Fs_4 - 4*W_0_4)/ap4;
            polysb[4] = (16*Fs_4 + W_0_4 + 8*Fs_2*W_0_2)/ap4;
            ///
            b[0] = 1;a[0] = 1;
            LConvb = conv( polysb, 6, b, 1, b );
            LConva = conv( polysa, 6, a, 1, a );
            for( int i=1; i<BLC; i++ ){
                preal_2 = preal[i] * preal[i];
                pimag_2 = pimag[i] * pimag[i];
                ap4  = (4*B_w_2*Fs_2 - 16*B_w*Fs_3*preal[i] - 4*B_w*Fs*W_0_2*preal[i] + 16*Fs_4*pimag_2 + 16*Fs_4*preal_2 + 8*Fs_2*W_0_2*pimag_2 + 8*Fs_2*W_0_2*preal_2 + W_0_4*pimag_2 + W_0_4*preal_2);
                ap3  = -(64*Fs_4*pimag_2 + 64*Fs_4*preal_2 - 32*B_w*Fs_3*preal[i] + 8*B_w*Fs*W_0_2*preal[i] - 4*W_0_4*pimag_2 - 4*W_0_4*preal_2)/ap4;
                ap2  = (96*Fs_4*pimag_2 - 8*B_w_2*Fs_2 + 96*Fs_4*preal_2 - 16*Fs_2*W_0_2*pimag_2 - 16*Fs_2*W_0_2*preal_2 + 6*W_0_4*pimag_2 + 6*W_0_4*preal_2)/ap4;
                ap1  = -(64*Fs_4*pimag_2 + 64*Fs_4*preal_2 + 32*B_w*Fs_3*preal[i] - 8*B_w*Fs*W_0_2*preal[i] - 4*W_0_4*pimag_2 - 4*W_0_4*preal_2)/ap4;
                ap0  = (4*B_w_2*Fs_2 + 16*Fs_4*pimag_2 + 16*Fs_4*preal_2 + W_0_4*pimag_2 + W_0_4*preal_2 + 8*Fs_2*W_0_2*pimag_2 + 8*Fs_2*W_0_2*preal_2 + 16*B_w*Fs_3*preal[i] + 4*B_w*Fs*W_0_2*preal[i])/ap4;
                ///
                ///
                polysa[4] = ap0;
                polysa[3] = ap1;
                polysa[2] = ap2;
                polysa[1] = ap3;
                polysa[0] = 1;
                ///
                polysb[0] = (16*Fs_4 + 8*Fs_2*W_0_2 + W_0_4)/ap4;
                polysb[1] = -(64*Fs_4 - 4*W_0_4)/ap4;
                polysb[2] = (96*Fs_4 - 16*Fs_2*W_0_2 + 6*W_0_4)/ap4;
                polysb[3] = -(64*Fs_4 - 4*W_0_4)/ap4;
                polysb[4] = (16*Fs_4 + W_0_4 + 8*Fs_2*W_0_2)/ap4;
                ///
                LConvb = conv( polysb, 6, b, LConvb, b );
                LConva = conv( polysa, 6, a, LConvb, a );
            }
            ///
            if( (n%2)==1 ){
                polysb[0] = (4*Fs_2 + W_0_2)/(4*Fs_2 + 2*B_w*Fs + W_0_2);
                polysb[1] = -(8*Fs_2 - 2*W_0_2)/(4*Fs_2 + 2*B_w*Fs + W_0_2);
                polysb[2] = (4*Fs_2 + W_0_2)/(4*Fs_2 + 2*B_w*Fs + W_0_2);
                ///
                polysa[0] = 1;
                polysa[1] = -(8*Fs_2 - 2*W_0_2)/(4*Fs_2 + 2*B_w*Fs + W_0_2);
                polysa[2] = ( 4*Fs_2 + W_0_2 - 2*B_w*Fs )/(4*Fs_2 + 2*B_w*Fs + W_0_2);
                ///
                LConvb = conv( polysb, 3, b, LConvb, b );
                LConva = conv( polysa, 3, a, LConvb, a );
            }
            //double ValFc = Get_Val_In_fc( LConvb, b, a, WnV[0] );
            for( int i=0; i<LConvb; i++ ){
                //b[i] = Mfc * b[i]/ValFc;
            }
        }
            break;

    }
    //char T2T[128];sprintf( T2T, "%5.19f", Get_Val_In_fc( LConvb, b, a, WnV[0] ) );
    //MessageBoxA( VENTANA_PADRE, T2T, "", 0 );
    return LConvb;
}
/// ///////////////////////////////////////////////////////////////////
/// ///////////////////////////////////////////////////////////////////

