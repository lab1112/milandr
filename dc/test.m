fif=45 ;
fs=125 ;

step = round(65536*fif/fs) ;

% const
x=100*ones(1,1000) ;
[xi,xq]=ddc(x,23593,1) ;
plot([xi;xq]');grid on ;
pause
fid = fopen('itest1.bin','wb') ; fwrite(fid,x,'int16','ieee-le') ; fclose(fid);

% if carrier
N=2^20;
x=round(1000*cos(2*pi*45000000/125000000*[0:N-1]));
[xi,xq]=ddc(x,23593,32) ;
plot([xi;xq]');grid on ;
pause
fid = fopen('itest2.bin','wb') ; fwrite(fid,x,'int16','ieee-le') ; fclose(fid);

% 2us meander modulation if carrier
x = ones(1,500) ;
x(2:2:end) = -1 ;
x = 1000*ones(250,1)*x ;
x = x(1:end) ;
x=round(x.*cos(2*pi*fif/fs*[0:length(x)-1]));
[xi,xq]=ddc(x,23593,32) ;
plot([xi;xq]');grid on ;
pause ;
fid = fopen('itest3.bin','wb') ; fwrite(fid,x,'int16','ieee-le') ; fclose(fid);

% if pulse carrier
x = [1 -1 1 1 -1 1 -1 -1 1 1 1 1 1 -1 1 -1] ;
x = [zeros(1,100),x,zeros(1,100)] ;
x = 1000*ones(250,1)*x ;
x = x(1:end) ;
x=round(x.*cos(2*pi*fif/fs*[0:length(x)-1]));
[xi,xq]=ddc(x,23594,32) ;
plot([xi;xq]');grid on ;
fid = fopen('itest4.bin','wb') ; fwrite(fid,x,'int16','ieee-le') ; fclose(fid);

