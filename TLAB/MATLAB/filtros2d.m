% function filtros2d
clear all;clc;
im = ( rgb2gray(imread( 'Coins.jpg' )) );
imd = double( im );
ker = [ 2 0 0; 0 -1 0; 0 0 -1];
NxN = 3;
kerl = ones( NxN );
kerl( round(NxN/2) + 1, round(NxN/2) + 1 ) = 0;
kerl = kerl/8;
imo = uint8( imfilter( imd, kerl, 'replicate') );
imshow( imo )
% asd
%
% imo = 255 * imo/max( max(abs(imo)) );
% imo = ( imo>68 & imo<180 );
% imo = imo>140;
% imo = imo/max( max(imo) );
subplot( 1, 3, 2 )
% imshow( imo );
imshow( im .* uint8( ~(imo>21 & imo<98) ) )
title( 'Analizando historigrama' )
%
subplot( 1, 3, 1 )
imshow( im );



% clc;clear all;
im = ( rgb2gray(imread( 'Coins.jpg' )) );
imshow( im>48 & im<78 )
subplot( 1, 3, 3 )
imshow( im .* uint8(im2bw(im)) )
title( 'Binarizando solamente' )

subplot( 1, 3, 1 )
imshow( im );
title( 'Original' )


%%
at = 255;
subplot( 1, 2, 1 )
% imshow( imo );
imshow( at .* uint8( ~(imo>30 & imo<85) ) )
title( 'Analizando historigrama' )


subplot( 1, 2, 2 )
imshow( at .* uint8(im2bw(im)) )
title( 'Binarizando solamente' )


%%

x=double(im);


g1=[5,5,5; -3,0,-3; -3,-3,-3];
g2=[5,5,-3; 5,0,-3; -3,-3,-3];
g3=[5,-3,-3; 5,0,-3; 5,-3,-3];
g4=[-3,-3,-3; 5,0,-3; 5,5,-3];
g5=[-3,-3,-3; -3,0,-3; 5,5,5];
g6=[-3,-3,-3; -3,0,5;-3,5,5];
g7=[-3,-3,5; -3,0,5;-3,-3,5];
g8=[-3,5,5; -3,0,5;-3,-3,-3];


x1=imfilter(x,g1,'replicate');
x2=imfilter(x,g2,'replicate');
x3=imfilter(x,g3,'replicate');
x4=imfilter(x,g4,'replicate');
x5=imfilter(x,g5,'replicate');
x6=imfilter(x,g6,'replicate');
x7=imfilter(x,g7,'replicate');
x8=imfilter(x,g8,'replicate');

y1=max(x1,x2);
y2=max(y1,x3);
y3=max(y2,x4);
y4=max(y3,x5);
y5=max(y4,x6);
y6=max(y5,x7);
y7=max(y6,x8);
y=y7;
m2 = 2*mean2(y);
y = uint8(y)>m2;
imshow( y )


%%
clc;clear all;

sobH = [  1,   0, -1; 2,   0, -2;  1,   0, -1 ];
sobV = [  1,   2,  1;  0,   0,  0; -1,  -2, -1];

H = fft2( sobH, 3, 3 );
V = fft2( sobV, 3, 3 );
HV = H .* V;
real( ifft2( HV, 3, 3 ) )




















