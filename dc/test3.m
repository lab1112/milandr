fid = fopen('adcmod.bin','rb') ; x=fread(fid,inf,'uint16') ; fclose(fid);
plot(x);grid on ;
pause

[xi,xq]=ddc(x',23593,32) ;
plot([xi;xq]');grid on ;
