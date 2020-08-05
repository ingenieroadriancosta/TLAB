% function pruebas
clc;clear all;
W = 705;
H = 397;
W = 80;
H = 80;

im = 255 * ones( H, W, 3 );
t = (0:2.3*1024-1)/1024;
sig = 0.94*sin( 2 * pi * (t.*t) );sig = ( sig + 1 )/2;
% sig = 0.01 + 0.98*nuttallwin( 1024 );
% sig = abs(fft( sig ));sig = sig(1:end/2);sig = sig/max(sig);
% spectrogram( sig, rectwin(1024), 1, 1024, 1024, 'yaxis' );
% asd
%%%%
adv = 0;
for in=1:length(sig)
    x = round( (W - 1) * in/length(sig) + 1 );
    y = round( (H - 1) * (1-sig(in)) + 1 );
    
    
    for y_2 = y-adv:y+adv
        for x_2 = x-adv:x+adv
            if( x_2>W || y_2>H )
                continue;
            end
            try %#ok<TRYNC>
                im( y_2, x_2, 1 ) = 0;
                im( y_2, x_2, 2 ) = 0;
                im( y_2, x_2, 3 ) = 0;
            end
        end
    end
    
end
imshow( uint8(im) )
imwrite( uint8(im), 'plot.png' )
clc;size( im )
%%
Fs = 8000;
t = (0:100*Fs-1)/Fs;
f = 0.5*Fs/2 * (t/max(t)) .* (t/max(t));
sig = 0.94*sin( 2 * pi * f.*t );
ap = audioplayer( sig, Fs );
% play( ap )
plot( sig )
spectrogram( sig, rectwin(Fs), 16, 20*Fs, Fs, 'yaxis' );
%%
clc;clear all;
%%%
Fs = 2000;
mt = 220;
t = (0:mt*Fs-1)/Fs;
f = (Fs/4-10) * (t/max(t));
chirp_signal = sin( 2 * pi * t.*f );
clear f t mt Fs
%%%
%%%
MaxSize = 30 * (1024*1024);
MaxSize = MaxSize/8;
Fs = 2000;
t = (0:MaxSize-1)/Fs;
f = (Fs/4-10) * (t/max(t));
chirp_signalMax = sin( 2 * pi * t.*f );
audiowrite( 'chirp_signalMax.wav', chirp_signalMax, Fs )
clear f t Fs MaxSize
%%%
signaltry = (2^18-1) * sin( 2 * pi * (0:1999)/2000 );
%%%
im0 = imread( '00.jpg' );
im1 = imread( 'Lena_C.jpg' );
[signal0, Fs0 ] = audioread( '1KHz.wav' );

signal0 = signal0( :, 1 );
[signal1, Fs1 ] = audioread( '50Hz.wav' );
signal1 = signal1( :, 1 );

[signal2, Fs2 ] = audioread( 'chirpsig.wav' );
signal2 = signal2( :, 1 );

Fs3 = 192000;
y = sin( 2 * pi * (0:5*Fs3-1)/Fs3 );

Fs_module = 2000;
module = 0.7 * sin( 2 * pi * (0:Fs_module-1)/Fs_module ) +...
            0.3 * sin( 25 * 2 * pi * (0:Fs_module-1)/Fs_module );
%%%
mod100M = sin( 1000*2* pi * (0:(100*1024*1024)/8-1)/((100*1024*1024)/8) );
%%%
%%%
% plot( chirp_signal )
save vars.mat






%%
clc;clear all;
Fs = 48000;
mt = 5;
t = (0:mt*Fs-1)/Fs;
f = Fs/4 * (t/max(t));
y = sin( 2 * pi * t.*f );
plot( t, y )
audiowrite( 'chirpsig.wav', y, Fs )

aup = audioplayer(y,Fs);
play( aup )
%%
clc;clear all;
Fs = 44100;
mt = 1;
t = (0:mt*Fs-1)/Fs;
f = Fs/4 * (t/max(t));

y = 0.5 + 0.4 * sin( 2 * pi * t.*f ) + 0.1 * sin( 2 * pi * t * 1000 );

plot( t, y )
audiowrite( 'chirpsig_1S.wav', y, Fs )

aup = audioplayer(y,Fs);
play( aup )
close all;


%%
clc;clear all;
Fs = 22050;
mt = 5;
t = (0:mt*Fs-1)/Fs;
t = (0:1023)/Fs;
f = Fs/4 * (t/max(t));
y = uint8( 255 * (sin( 2 * pi * t.*f )+1)/2 );
plot( t, y )
audiowrite( 'signal_8bits.wav', y, Fs, 'BitsPerSample', 8 )

aup = audioplayer(y,Fs);
play( aup )
%%
clc;clear all;
Fs = 2000;
t = (0:127)/128;
y = uint8( 255 * ((sin( 2 * pi * t )+1)/2) );
plot( t, y )
audiowrite( 'signal_128_8bits.wav', y, Fs, 'BitsPerSample', 8 )

aup = audioplayer(y,Fs);
play( aup )

%%
clc;clear all;
Fs = 2000;
t = (0:127)/128;
y = ( 0.5 * sin( 2 * pi * t ) );
plot( t, y )
audiowrite( 'signal_128_16bits.wav', y, Fs )

aup = audioplayer(y,Fs);
play( aup )
%%
clc;

%Coeficientes del programa.
b = [ 9.8695605517465310000e-012, 0.0000000000000000000e+000, -1.9739121103493062000e-011, 0.0000000000000000000e+000, 9.8695605517465310000e-012 ]; 
a = [ 1.0000000000000000000000, -3.9291402745596105000000, 5.8595269385447626000000, -3.9291228178881021000000, 0.9999911142736017700000 ]; 

Fs = 2000;							%Frecuencia de muestreo.
FNyq = Fs/2;						%Frecuencia de Nyquist.
[Hbut f] = freqz( b, a, FNyq, Fs ); %Respuesta en frecuencia del filtro(fft(b)/fft(a)).
Hbut = abs(Hbut);					%Magnitud de la repuesta.



n = 2;								%Orden del filtro a implementar.
fc = [59.999 60.001];						%Frecuencia de corte.
[b0 a0] = butter( n, fc/FNyq, 'bandpass' );%Filtro Butterworth
H = freqz( b0, a0, FNyq, Fs );		%Respuesta en frecuencia del filtro(fft(b0)/fft(a0)).
H = abs(H);							%Magnitud de la repuesta.
plot( f, H, 'LineWidth', 4 ); %Gráfixa del filtro(Matlab)
hold on;
plot( f, Hbut, 'r', 'LineWidth', 2 );%Gráfixa del filtro(TLAB)
hold off;
ylim( [0 max(max(Hbut), max(H))+0.1] );
xlim( [0 1000] )



%%
clc;clear all;
Fs = 1000;
n = 0:Fs-1;
t = n/Fs;
y = (sin( 2 * pi * t * 100 ) + 1)/2;
subplot( 3, 1, 1 )
plot( y )
ylim( [-1 1] )
N = 32;
b = fir1( N, 2 * 50/Fs, 'high', nuttallwin(N+1) );
yn = real( ifft( fft(y) .* fft(b,length(y)) ) );
yn = zeros( size( y ) );
yn = y;
for in=1:length(y)
    s = 0;
    for bp = 1:length(b)
        try
            s = s + yn( in - bp + 1 ) * b(bp);
        end
    end
    yn( in ) = s;
end
subplot( 3, 1, 2 )
plot( yn )
ylim( [-1 1] )












%%
clc;clear all;
[y, Fs] = audioread( 'Perico_Ripiao.wav' );
save Perico_Ripiao.mat
y = y( :, 1 );
plot( y )
size(y)
[b, a] = butter( 2, 1/Fs, 'high' );
B = zeros( size(y) );
A = zeros( size(y) );
B(1:length(b)) = b;
A(1:length(a)) = a;
tic
B = fft( B );
A = fft( A );
Y = fft( y );
toc
%
Y = Y .* (abs(B)./abs(A));
clear A 
clear B
tic
y = real(ifft( Y ));
plot( y )
toc
%%
clc;clear all;
FName = 'NO MORIRA.wav';
[y, Fs] = audioread( FName );
y = y( :, 1 );
audiowrite( FName, y, Fs );
asd
%%
clc;clear all;
tic
im = imread( '02.jpg' );
imshow( im )
toc

im2 = rgb2gray(im);
im2 = rgb2gray(im2);
% im2(:,:,1) = 255 * uint8(im2bw(im(:,:,1),0.5));
% im2(:,:,2) = 255 * uint8(im2bw(im(:,:,2),0.5));
% im2(:,:,3) = 255 * uint8(im2bw(im(:,:,3),0.5));




imshow( im2 )

% imhist( im(:,:,1) )



%%
clc;clear all;
X = imread( '00.jpg' );
X = rgb2gray( X );
size( X )
imshow(X)
%
tic
Y = fft2(X);
toc
size( Y )
imagesc( abs(fftshift(Y)) )


kernel = [ 1  2 1; ...
           2  0 1; ....
           1  2 1
           ];
kernel = [ 2   1   0; ...
           0  -1   0; ....
           0   0  -1
           ];
kernel = [  0  -1   0; ...
           -1   100  -1; ....
            0  -1   0
           ];
K = fft2( kernel, size( X,1 ), size( X,2 ) );

% K = 1;

XI = real( ifft2( Y .* K ) );
imagesc( XI );
% Y = fft2(X, 2^nextpow2(size( X,1 )), 2^nextpow2(size( X,2 )) );
% imagesc(abs(fftshift(Y)));
% size( Y )


%%
clc;clear all;
X0 = imread( '00.jpg' );
X0 = rgb2gray( X0 );
X1 = imread( '01.jpg' );
X1 = rgb2gray( X1 );
% X1 = ones( size(X0,1), size(X0,2) );
%
N = 7;
% X1 = ones( N, N );
% 
Y0 = fft2(X0);
Y1 = fft2(X1, size(X0,1), size(X0,2) );

Y = Y0 .* Y1;
Y = Y0;
XI = real( ifft2( Y, size(X0,1), size(X0,2) ) );
% XI = real( ifft2( Y, 2*size(X0,1)+1, 2*size(X0,2)+1 ) );
imagesc( XI );


XxY = size(X0,1) * size(X0,2);
Y = ( abs(Y)/XxY )/169.7071;
max( max(Y) )
imagesc( Y );


%%
clc;clear all;
W = 128;
H = 128;
X = zeros( W, H );
X = rgb2gray(imread( '00.jpg' )); W = size(X,2);H = size(X,1);imshow( X );
NFIL = 1;
b = zeros( NFIL, NFIL );
HG = 0;
DivF = 1*(size(b,1)^2 + size(b,2)^2);
ND = 1;
for y = 1:size(b,1)
    ym = ND*(size(b,1)/2 - y);
    for x = 1:size(b,2)
        xm = ND*(size(b,2)/2 - x);
        b( y, x ) = exp( (-(xm^2 + ym^2))/DivF );
        %%%
%         b( y, x ) = sin( 2 * pi * 100 * (x/size(b,2)+y/size(b,1)) );
        %%%
%         b( y, x ) = sin( 2 * pi * 128 * x/size(b,2) ) +...
%                         sin( 2 * pi * 200 * y/size(b,1) );
        %%%
        %%%
        %%%
    end
end
b = [ 1 2 1; 2 4 2; 1 2 1];
imshow( b )

%     t = (0:size(X,1)-1)/size(X,1);
%     X( 1:end, 1 ) = sin( 2 * pi * t * 20 );
%     for in=2:size(X,2)
%         X( 1:end, in ) = X( 1:end, 1 );
%     end
% 
Y = fft2(X, size(X,1), size(X,2));
B = fft2(b, size(X,1), size(X,2));
imshow(abs(B))
%
YO = Y .* B;
%
IMO = abs(B);
imo = real(ifft2( YO, size(X,1), size(X,2) ));
imshow( imo )
imshow( imo/max(max(imo)) )
% imshow( X )
return


B2P = B;
% B2P = B2P( 1:size(B2P,1)/2, 1:size(B2P,2)/2 );
t = 0:size(B2P,2)-1;
f = 0:size(B2P,1)-1;
% B2P = abs(fftshift(B));
subplot( 1, 1, 1 );surf( t, f, abs(B2P), 'EdgeColor', 'none' );
axis xy; 
axis tight; 
colormap(jet);
xlabel( 'Ancho' );
ylabel( 'Alto' );
% view(0,90);

