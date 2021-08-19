%-------------------------------------------------------------------------------
function [xi,xq]=ddc(x,step,n)

N=65536 ;
h=ones(1,n) ;
i=mod(step*[0:length(x)-1],N) ;

ct = round(32767*cos(2*pi/N*[0:N-1])) ;
st = round(32767*sin(2*pi/N*[0:N-1])) ;

xi = round(x.*ct(i+1)/2^15) ;
xq = round(x.*st(i+1)/2^15) ;

% lpf
xi=filter(h,1,xi) ;
xq=filter(h,1,xq) ;
xi=filter(h,1,xi) ;
xq=filter(h,1,xq) ;
xi=filter(h,1,xi) ;
xq=filter(h,1,xq) ;
% decimator
xi=xi(1:n:end) ;
xq=xq(1:n:end) ;

%line approximation
%N = 1024 ;
%c = round(32767*cos(2*pi*[0:N-1]/N)) ;
%s = round(32767*sin(2*pi*[0:N-1]/N)) ;
%dc = round(diff(c)/64) ;
%ds = round(diff(s)/64) ;
%cc=[0:63]'*dc + ones(64,1)*c(1:N-1) ;
%ss=[0:63]'*ds + ones(64,1)*s(1:N-1) ;
%ct=cc(1:end) ;
%st=ss(1:end) ;
