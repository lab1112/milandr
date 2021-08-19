set autoscale
set nokey
set grid 
plot 'odduc.bin' binary format="%int16%int16" using 1, \
     'odduc.bin' binary format="%int16%int16" using 2 with lines

pause -1

#set xrange [0:100]
set title "itest1.bin"
plot 'itest1.bin' binary format="%int16" using 1 with lines
pause -1

set xrange [0:500]
set title "itest2.bin"
plot 'itest2.bin' binary format="%int16" using 1 with lines
pause -1

set xrange [0:500]
set title "itest3.bin"
plot 'itest3.bin' binary format="%int16" using 1 with lines
pause -1

set title "itest4.bin"
plot 'itest4.bin' binary format="%int16" using 1 with lines
pause -1
