% function pic2freq
clc;clear all;
im = imread( 'lena1024.png' );
im = ( rgb2gray( im ) );
% imshow( im )
im = double(im)/max( max(double(im)) );
%%% 
Fs = 8000;
tend = size( im, 2 );
t = 0:tend*Fs-1;
tFs = (0:Fs-1)/Fs;
%%%
yo = zeros( size(t) );
%%%
fvar = 10;
f = fvar:fvar:fvar*size( im, 1 );
size( f )
for y = 1:size( im, 2 )
    for x = 1:size( im, 1 )
        yt = im( y, x ) .* sin( 2 * pi * t( (x-1)*Fs+1:((x)*Fs) ) ...
            * f(y) );
        yo( (x-1)*Fs+1:((x)*Fs) ) = yo( (x-1)*Fs+1:((x)*Fs) ) + yt;
    end
    y
end
yo = yo/max( abs(yo) );
play( audioplayer(yo,Fs) )
audiowrite( '0aaa.wav', yo, Fs )

