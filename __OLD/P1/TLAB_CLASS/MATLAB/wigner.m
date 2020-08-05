function wigner( sinp, Fs, window_in )
sinp = sinp(:)';
window_in = window_in(:)';
l = length(sinp);
l = l - mod( l, Fs );
lf = l/Fs;
yact = zeros( 1, Fs );
wig = zeros( Fs/2+1, (lf) );
for x = 0:size( wig, 2 )-1
    n = Fs * (x);
    for y = 0:size( wig, 1 )-1
        if( (n-y+1)<1 )
            yact(y+1) = 0;
            continue;
        end
        yact(y+1) = sinp( n-y+1 ) * sinp( n+y+1 );
    end
    t_o = fft(yact .* window_in, Fs);
    wig( :, x+1 ) = 2*abs( t_o(1:end/2+1) )/Fs;
end
% 
if nargout==0
    wig = wig/max( max(wig) );
    t = 0:length(sinp)/Fs - 1;
    f = 0:Fs/2;
    surf( t, f, 100*wig, 'EdgeColor', 'none' );   
    axis xy; axis tight; colormap(jet); view(0,90);
    xlabel( 'Time (s)' );
    ylabel( 'Frequency (Hz)' );
end

