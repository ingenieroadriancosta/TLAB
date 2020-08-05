const double pi_dbl = 3.1415926535897932384626433832795;
const double pi = 3.1415926535897932384626433832795;
double bessi0( double );
double tscheby( double, double );
double Get_Val_In_fc_FIR( int, double*, double );
double sinc( double DVal );
void Window_F( int, double*, int, double );
int fir1( int n, double* WnV, int FType, int Win, double alpha, double* b ){
    ZeroMemory( b, (n+1)*sizeof(double) );
    int N_Top = n + 1;
    double N_Top_Med = ((double)(N_Top-1))/2;
    double Mag0FNyq = 0;
    switch( FType ){
        /// Paso Altos !!! .
        case 1:{
            for( int i=0; i<N_Top; i++ ){
                b[i] = sinc( ((double)i - (double)N_Top_Med) ) -
                                    WnV[0] * sinc( WnV[0] * ((double)i - (double)N_Top_Med) );
            }
            Window_F( N_Top, b, Win, alpha );
            Mag0FNyq = 0;
            for( int i=0; i<N_Top; i++ ){
                Mag0FNyq = Mag0FNyq + abs(b[i]);
            }
            Mag0FNyq = Get_Val_In_fc_FIR( N_Top, b, 1 );
            for( int i=0; i<N_Top; i++ ){
                b[i] = b[i]/Mag0FNyq;
            }
        }
            break;
        /// Paso Bajos!!!.
        case 0:{
            if( N_Top==2 ){
                b[0] = 0.5;
                b[1] = 0.5;
                b[2] = 0;
                Window_F( N_Top, b, Win, alpha );
                Mag0FNyq = 0;
                for( int i=0; i<N_Top; i++ ){
                    Mag0FNyq = Mag0FNyq + b[i];
                }
                if( abs(Mag0FNyq)>1e-15){
                    for( int i=0; i<N_Top; i++ ){
                        b[i] = b[i]/Mag0FNyq;
                    }
                }
                break;
            }
            for( int i=0; i<N_Top; i++ ){
                b[i] = WnV[0] * sinc( WnV[0] * ((double)i - (double)N_Top_Med) );
            }
            Window_F( N_Top, b, Win, alpha );
            Mag0FNyq = 0;
            for( int i=0; i<N_Top; i++ ){
                Mag0FNyq = Mag0FNyq + b[i];
            }
            for( int i=0; i<N_Top; i++ ){
                b[i] = b[i]/Mag0FNyq;
            }
        }
            break;
        /// Paso Bandas!!!.
        case 2:{
            for( int i=0; i<N_Top; i++ ){
                b[i] = WnV[1] * sinc( WnV[1] * ((double)i - (double)N_Top_Med) )
                     - WnV[0] * sinc( WnV[0] * ((double)i - (double)N_Top_Med) );
            }
            Window_F( N_Top, b, Win, alpha );
            Mag0FNyq = 0;
            for( int i=0; i<N_Top; i++ ){
                Mag0FNyq = Mag0FNyq + b[i];
            }
        }
            break;
        /// Para Bandas!!!.
        case 3:{
            for( int i=0; i<N_Top; i++ ){
                b[i] = sinc( ((double)i - (double)N_Top_Med) ) - WnV[1] * sinc( WnV[1] * ((double)i - (double)N_Top_Med) )
                     + WnV[0] * sinc( WnV[0] * ((double)i - (double)N_Top_Med) );
            }
            Window_F( N_Top, b, Win, alpha );
            Mag0FNyq = 0;
            for( int i=0; i<N_Top; i++ ){
                Mag0FNyq = Mag0FNyq + b[i];
            }
        }
            break;
    }
    return N_Top;
}
/// |||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
/// /////////////////////////////////////////////////////////////////////////////////////////////
/// /////////////////////////////////////////////////////////////////////////////////////////////
/// |||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
int fir1( int n, double* WnV, int FType, double* b ){
    ZeroMemory( b, (n+1)*sizeof(double) );
    int N_Top = n + 1;
    double N_Top_Med = ((double)(N_Top-1))/2;
    double Mag0FNyq = 0;
    switch( FType ){
        /// Paso Altos !!! .
        case 1:{
            for( int i=0; i<N_Top; i++ ){
                b[i] = sinc( ((double)i - (double)N_Top_Med) ) -
                                    WnV[0] * sinc( WnV[0] * ((double)i - (double)N_Top_Med) );
            }
            Mag0FNyq = 0;
            for( int i=0; i<N_Top; i++ ){
                Mag0FNyq = Mag0FNyq + abs(b[i]);
            }
            Mag0FNyq = Get_Val_In_fc_FIR( N_Top, b, 1 );
            for( int i=0; i<N_Top; i++ ){
                b[i] = b[i]/Mag0FNyq;
            }
        }
            break;
        /// Paso Bajos!!!.
        case 0:{
            if( N_Top==2 ){
                b[0] = 0.5;
                b[1] = 0.5;
                b[2] = 0;
                Mag0FNyq = 0;
                for( int i=0; i<N_Top; i++ ){
                    Mag0FNyq = Mag0FNyq + b[i];
                }
                if( abs(Mag0FNyq)>1e-15){
                    for( int i=0; i<N_Top; i++ ){
                        b[i] = b[i]/Mag0FNyq;
                    }
                }
                break;
            }
            for( int i=0; i<N_Top; i++ ){
                b[i] = WnV[0] * sinc( WnV[0] * ((double)i - (double)N_Top_Med) );
            }
            Mag0FNyq = 0;
            for( int i=0; i<N_Top; i++ ){
                Mag0FNyq = Mag0FNyq + b[i];
            }
            for( int i=0; i<N_Top; i++ ){
                b[i] = b[i]/Mag0FNyq;
            }
        }
            break;
        /// Paso Bandas!!!.
        case 2:{
            for( int i=0; i<N_Top; i++ ){
                b[i] = WnV[1] * sinc( WnV[1] * ((double)i - (double)N_Top_Med) )
                     - WnV[0] * sinc( WnV[0] * ((double)i - (double)N_Top_Med) );
            }
            Mag0FNyq = 0;
            for( int i=0; i<N_Top; i++ ){
                Mag0FNyq = Mag0FNyq + b[i];
            }
        }
            break;
        /// Para Bandas!!!.
        case 3:{
            for( int i=0; i<N_Top; i++ ){
                b[i] = sinc( ((double)i - (double)N_Top_Med) ) - WnV[1] * sinc( WnV[1] * ((double)i - (double)N_Top_Med) )
                     + WnV[0] * sinc( WnV[0] * ((double)i - (double)N_Top_Med) );
            }
            Mag0FNyq = 0;
            for( int i=0; i<N_Top; i++ ){
                Mag0FNyq = Mag0FNyq + b[i];
            }
        }
            break;
    }
    return N_Top;
}
/// |||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
/// /////////////////////////////////////////////////////////////////////////////////////////////
/// /////////////////////////////////////////////////////////////////////////////////////////////
/// |||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
double sinc( double DVal ){
    static const double pi_v = 3.1415926535897932384626433832795;
    static double val_Zero = 1e-6;
    if( fabs(DVal)<=val_Zero ){
        return 1;
    }else{
        return ( sin(pi_v*DVal)/(pi_v*DVal) );
    }
}
/// |||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
/// /////////////////////////////////////////////////////////////////////////////////////////////
/// /////////////////////////////////////////////////////////////////////////////////////////////
/// |||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
double Get_Val_In_fc_FIR( int N, double* b, double fc ){
    const double pi2_NN = (fc*(pi_dbl));
    double ValbR = 0;
    double ValbI = 0;
    for( int n = 0; n<N; n++ ){
        ValbR = ValbR + b[n] * cos( pi2_NN*((double)n) );
        ValbI = ValbI + b[n] * sin( pi2_NN*((double)n) );
    }
    return ( sqrt( ValbR*ValbR + ValbI*ValbI ) );
}
/// |||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
/// /////////////////////////////////////////////////////////////////////////////////////////////
/// /////////////////////////////////////////////////////////////////////////////////////////////
/// |||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
void Window_F( int N, double* b, int WinType, double alphaV ){
    static const double pi_v = 3.1415926535897932384626433832795;
    static double V2B;
    static double ValT;
    static char T2T[128];
    static char TextF[1024];
    switch( WinType ){
        case 0:/// R E C T W I N
            break;
        case 1:{/// G A U S S W I N
            V2B = - (((double)1)/2) * alphaV * alphaV/( N/2.0 * N/2.0 );
            static int iN;
            for( int i=0; i<N; i++ ){
                iN = (i - N/2.0);
                b[i] = b[i] * exp( V2B * iN * iN );
            }
        }
            break;
        case 2:{/// N U T A L L
            const double a[4] = { 0.3635819, 0.4891775, 0.1365995, 0.0106411 };
            V2B = (2 * pi_v)/(N);
            for( int i=0; i<N; i++ ){
                b[i] = b[i] * ( a[0] - a[1] * cos( i * V2B ) + a[2] * cos( 2 * i * V2B )
                               -a[3] * cos( 3 * i * V2B ) );
            }
        }
            break;
        case 3:{/// H A N N
            V2B = (2 * pi_v)/(N-1);
            for( int i=0; i<N; i++ ){
                b[i] = b[i] * ( 0.5 * (1 - cos( i * V2B ) ) );
            }
        }
            break;
        case 4:{/// B L A C K M A N
            V2B = (2 * pi_v)/(N-1);
            for( int i=0; i<N; i++ ){
                b[i] = b[i] * ( 0.42 - 0.5 * cos( i * V2B ) + 0.08 * cos( 2 * i * V2B ) );
            }
        }
            break;
        case 5:{/// B L A C K M A N - H A R R I S
            const double a[4] = { 0.35875, 0.48829, 0.14128, 0.01168 };
            V2B = (2 * pi_v)/(N);
            for( int i=0; i<N; i++ ){
                b[i] = b[i] * ( a[0] - a[1] * cos( i * V2B ) + a[2] * cos( 2 * i * V2B )
                               -a[3] * cos( 3 * i * V2B ) );
            }
        }
            break;
        case 6:{/// F L A T   T O P
            const double a[5] = { 0.21557895, 0.41663158, 0.277263158, 0.083578947, 0.006947368 };
            V2B = (2 * pi_v)/(N-1);
            for( int i=0; i<N; i++ ){
                b[i] = b[i] * ( a[0] - a[1] * cos( i * V2B ) + a[2] * cos( 2 * i * V2B )
                               -a[3] * cos( 3 * i * V2B ) + + a[4] * cos( 4 * i * V2B ) );
            }
        }
            break;
        case 7:{/// B A R T L E T T
            const double NTop_M1 = N-1;
            const double NTop_M1_2 = ((double)(NTop_M1))/2;
            V2B = (2 * pi_v)/(N-1);
            for( int i=0; i<N; i++ ){
                if( i<=NTop_M1_2 ){
                    b[i] = b[i] * (2 * i)/((double)NTop_M1);
                }else{
                    b[i] = b[i] * ( 2 - (2*i)/NTop_M1 );
                }
            }
            // MessageBoxA( HW_WinT, "", "", 0 );
        }
            break;
/// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
/// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
/// !!!!!!!!!!!!!!!!!!!!!! N O   I M P L E M E N T A D O !!!!!!!!!!!!!!!!!!!!!!
        case 8:{/// T R I A N G U L A R
            TextF[0]++;TextF[0] = 0;
            if( ((N%2)==1) ){
                const double LMed = (double)((N+1))/2;
                for( int i=0; i<N; i++ ){
                    if( i<LMed ){
                        b[i] = b[i] * (((double)(i+1))/LMed);
                        //sprintf( T2T, "%f\n", ((double)(i+1))/LMed );
                    }else{
                        b[i] = b[i] * ( (double)(N) - (i+1) + 1 )/LMed;
                        //sprintf( T2T, "%f\n", ( (double)(N) - (i+1) + 1 )/LMed );
                    }
                    //strcat( TextF, T2T );
                }
            }else{
                const double LMed = ((double)(N))/2;
                for( int i=0; i<N; i++ ){
                    if( i<LMed ){
                        b[i] = b[i] * ((2*((double)i + 1) - 1 ) /N);
                        // sprintf( T2T, "%f\n", ( (2*((double)i + 1) - 1 )/N ) );
                        // strcat( TextF, T2T );
                    }else{
                        b[i] = b[i] * ( (2*( N - (double)i) - 1 )/N );
                        // sprintf( T2T, "%f\n", ( (2*( N - (double)i) - 1 )/N ) );
                        // strcat( TextF, T2T );
                    }
                }
                // MessageBoxA( HW_WinT, TextF, "", 0 );
            }
        }
            break;
        case 9:{/// H A M M I N G
            V2B = (2 * pi_v)/(N-1);
            for( int i=0; i<N; i++ ){
                b[i] = b[i] * ( 0.54 - 0.46 * cos( V2B * i ) );
            }
        }
            break;
        case 10:{/// K A I S E R
            if( alphaV<0.000001 ){
                break;
            }
            const double L_1_M = ((double)(N-1))/2;
            V2B = abs( bessi0( alphaV ) );
            for( int i=0; i<N; i++ ){
                    ValT = ( i - L_1_M )/L_1_M;
                    ValT = alphaV * sqrt( 1 - ValT * ValT );
                    ValT = bessi0( ValT );
                    b[i] = b[i] * ( ValT/V2B );
            }
        }
            break;
        case 11:{/// R I E S Z
            const double L_1_M = ((double)(N-1))/2;
            for( int i=0; i<N; i++ ){
                    ValT = ( i - L_1_M )/(L_1_M);
                    ValT = ValT * ValT;
                    ValT = 1 - ValT;
                    b[i] = b[i] * ( ValT );
            }
        }
            break;
        case 12:{/// R I E M A N N
            const double L_1_M = ((double)(N+1))/2;
            for( int i=0; i<N; i++ ){
                    ValT = sinc( ( i - L_1_M )/L_1_M );
                    b[i] = b[i] * ( ValT );
            }
        }
            break;
        case 13:{/// P A R Z E N
            const double N_M = ((double)(N))/2;
            const double L_1_M = ((double)(N-1))/2;
            const double N_4 = ((double)(N-1))/4;
            double k;
            for( int i=0; i<N; i++ ){
                k = i - L_1_M;
                if( (k)<-N_4 ){
                    ValT = ( 1 - fabs(k)/N_M );
                    ValT = 2 * ValT * ValT * ValT;
                }else{
                    if( fabs(k)>N_4 ){
                        ValT = ( 1 - fabs(k)/N_M );
                        ValT = 2 * ValT * ValT * ValT;
                    }else{
                        ValT = ( fabs(k)/N_M );
                        ValT = 1 - 6 * ValT * ValT * ( 1 - fabs(k)/N_M );
                    }
                }
                b[i] = b[i] * ( ValT );
            }
        }
            break;
        case 14:{/// B O H M A N
            const double N_M = ((double)(N))/2;
            const double L_1_M = ((double)(N-1))/2;
            for( int i=1; i<N-1; i++ ){
                    ValT = ( 1 - fabs(i - L_1_M)/N_M ) * cos( pi_v * fabs(i - L_1_M)/N_M ) +
                                1.0/pi_v * sin( pi_v * fabs(i - L_1_M)/N_M );
                    b[i] = b[i] * ( ValT );
            }
            b[0] = 0;
            b[N-1] = 0;
        }
            break;
        case 15:{/// T U K E Y
            //*
            if( alphaV>=1 ){
                V2B = (2 * pi_v)/(N-1);
                for( int i=0; i<N; i++ ){
                    b[i] = b[i] * ( 0.5 * (1 - cos( i * V2B ) ) );
                }
                break;
            }
            const double N_M = ((double)(N-1))/2;
            const double ALN_M = alphaV * N_M;
            const double ALN_N = (N - 1) * ( 1 - alphaV/2);
            for( int i=0; i<N; i++ ){
                if( i>=ALN_M && i<=ALN_N ){
                    continue;
                }
                if( i<ALN_M ){
                    ValT = pi_v * ( i/ALN_M - 1 );
                    ValT = 0.5 * ( 1 + cos(ValT) );
                }else{
                    ValT = pi_v * ( i/ALN_M - 2/alphaV + 1 );
                    ValT = 0.5 * ( 1 + cos(ValT) );
                }
                b[i] = b[i] * ( ValT );

            }
        }
            break;
        case 16:{/// P O I S S O N   Y   C A U C H Y
            const double L_1_M = ((double)(N))/2;
            for( int i=1; i<N-1; i++ ){
                    ValT = ( alphaV * ( i - L_1_M )/L_1_M );
                    ValT = 1/( 1 + ValT * ValT );
                    b[i] = b[i] * ( ValT );
            }
            b[0] = 0;
            b[N-1] = 0;
        }
            break;
        case 17:{/// D O L P H - C H E B Y S H E V
            const double N_1 = (double)( N - 1 );
            const double d = pow( 10, (-alphaV/20) );
            const double ys = pow( ( 1.0/d + sqrt( 1.0/pow( d, 2 ) - 1) ), (1.0/N_1) );
            const double xs = ( ys + 1.0/ys )/2;
            const double oms = 1.0/pi_v * acos( 1.0/xs );
            double* W = (double*)malloc( (N+10) * sizeof(double) );
            double MaxOf = -1e100;
            ///
            // printf("xs = %f\noms = %f\n", xs, oms );
            ///
            for( int k=0; k<N; k++ ){
                W[k] = 0;
                for( int i=0; i<N; i++ ){
                    W[k] = W[k] + tscheby( cos( (pi * i)/(N) )/cos(pi*oms), N_1 ) *
                                        cos( pi * (( 2 * k - N_1 ) * i)/(N) ) ;
                }
                MaxOf = max( MaxOf, W[k] );
            }
            for( int i=0; i<N; i++ ){
                b[i] = b[i] * ( W[i]/MaxOf );
            }
            // sprintf( T2T, "N O   I M P L E M E N T A D O\nOpción: %i", WinType );
            // MessageBoxA( HW_WinT, T2T, "", 0 );
            free( W );
        }
            break;
/// //////////////////////////////////////////////////////////////////////////////////////////
/// //////////////////////////////////////////////////////////////////////////////////////////
        default:
            sprintf( T2T, "N O   I M P L E M E N T A D O\nOpción: %i", WinType );
            MessageBoxA( NULL, T2T, "", 0 );
            break;
    }
}
/// /////////////////////////////////////////////////////////////////////////////////////////////
/// |||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
/// BESSEL MODIFICADA PARA KAISER.
double bessi0( double x ){
    // static long double* FactNi;
    double y = 0;
    /*
    double x_2;
    if( FactNi==NULL ){
        FactNi = (long double*)malloc( 100 *sizeof( long double ) );
        FactNi[0] = 1;
        FactNi[1] = 1;
        for( int i=2; i<100; i++ ){
            FactNi[i] = 1;
            for( int f = 2; f<=i; f++ ){
                FactNi[i] = ((long double)f) * FactNi[i];
            }
            FactNi[i] = FactNi[i] * FactNi[i];
        }
    }
    x_2 =  (x * x)/4;
    for( int k=0; k<20; k++ ){
        y = y + pow( x_2, k )/FactNi[k];
    }
    //*/
    //*
    double ax = fabs( x );
    if( ax < 3.75 ){
        y = (x*x)/14.0625;
        y = 1.0 + y * ( 3.5156229 + y * ( 3.0899424 + y * (1.2067492 + y * ( 0.2659732 + y * ( 0.360768e-1 + y * 0.45813e-2 )))));
    }else{
        y=3.75/ax;
        y=(exp(ax)/sqrt(ax))*(0.39894228+y*(0.1328592e-1
            +y*(0.225319e-2+y*(-0.157565e-2+y*(0.916281e-2
                +y*(-0.2057706e-1+y*(0.2635537e-1+y*(-0.1647633e-1
                +y*0.392377e-2))))))));
    }
    //*/
    return y;
}
/// /////////////////////////////////////////////////////////////////////////////////////////////
/// /////////////////////////////////////////////////////////////////////////////////////////////
/// |||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
/// |||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
double tscheby( double x, double n ){
    if( ( (int)(n)%2)==0 ){
        x = fabs(x); /// NO FUNCION IMPAR (N)
        if( x<1 ){
            return cos( n * acos(x) );
        }else{
            return cosh( n * acosh(x) );
        }
    }else{
        ///
        if( fabs(x)<1 ){
            return cos( n * acos(x) );
        }else{
            if( x<1 ){ /// PAR NO FUNCIONA (N)
                return -cosh( n * acosh(abs(x)) );
            }else{
                return cosh( n * acosh(x) );
            }
        }
    }
    return 0;
}
#include "conv.cpp"
#include "butter.cpp"

