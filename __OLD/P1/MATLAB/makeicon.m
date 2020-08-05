% function makeicon
clc;clear all;
W_P = 480;
H_P = 480;
im = ones( H_P, W_P, 3 );
N = 10000;
wn = 0.9599 * nuttallwin( N ) + 0.0099;
R = 0.3989;     G = 0.3870;         B = 0.2140;
im( 1:2, :, : ) = 0;im( end:-1:end-1, :, : ) = 0;
im( :, 1:2, : ) = 0;im( :, end:-1:end-1, : ) = 0;
N_L_P = 12;
if N_L_P<5
    wn = 0.977 * nuttallwin( N ) + 0.0099;
end
for n = 0:length(wn)-1
    x = round( W_P * n/length(wn) );
    y = round( wn(n+1) * H_P );
    for inx=-N_L_P:N_L_P
        for iny=-N_L_P:N_L_P
            if ( (inx+x)<=W_P && (inx+x)>0 ) && ...
                ( (iny+y)<=H_P && (iny+y)>0 )
                im( y+iny, x+inx, 1 ) = R;
                im( y+iny, x+inx, 2 ) = G;
                im( y+iny, x+inx, 3 ) = B;
            end
        end
    end
    
end
im = im( end:-1:1, :, : );


imwrite( im, '0000im.bmp' )
imshow( im )

