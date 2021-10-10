# usage : gnuplot draw_plots.p

set terminal pngcairo
set logscale xy
set format x "10^{%L}"
set format y "10^{%L}"
set xlabel "Step size"
set ylabel "Calculation error at x0 = 5"
set key left top

set output "plots/x_3_05.png"
set title "(x + 3)^0^.^5"
plot "rawdata/sqrtx3.dat" using 1:2 with lines title "1st method",\
"rawdata/sqrtx3.dat" using 1:3 with lines title "2nd method",\
 "rawdata/sqrtx3.dat" using 1:4 with lines title "3rd method",\
  "rawdata/sqrtx3.dat" using 1:5 with lines title "4th method",\
   "rawdata/sqrtx3.dat" using 1:6 with lines title "5th method"

set output "plots/cos_sin_x.png"
set title "cos(sin(x))"
plot "rawdata/cossinx.dat" using 1:2 with lines title "1st method",\
"rawdata/cossinx.dat" using 1:3 with lines title "2nd method",\
 "rawdata/cossinx.dat" using 1:4 with lines title "3rd method",\
  "rawdata/cossinx.dat" using 1:5 with lines title "4th method",\
   "rawdata/cossinx.dat" using 1:6 with lines title "5th method"

set output "plots/exp_sin_cos_x.png"
set title "e^s^i^n^(^c^o^s^(^x^)^)"
plot "rawdata/cossinx.dat" using 1:2 with lines title "1st method",\
"rawdata/cossinx.dat" using 1:3 with lines title "2nd method",\
 "rawdata/cossinx.dat" using 1:4 with lines title "3rd method",\
  "rawdata/cossinx.dat" using 1:5 with lines title "4th method",\
   "rawdata/cossinx.dat" using 1:6 with lines title "5th method"


set output "plots/ln_x_3.png"
set title "ln(x + 3)"
plot "rawdata/lnx3.dat" using 1:2 with lines title "1st method",\
"rawdata/lnx3.dat" using 1:3 with lines title "2nd method",\
 "rawdata/lnx3.dat" using 1:4 with lines title "3rd method",\
  "rawdata/lnx3.dat" using 1:5 with lines title "4th method",\
   "rawdata/lnx3.dat" using 1:6 with lines title "5th method"

set output "plots/sin_x_2.png"
set title "sin(x^2)"
plot "rawdata/sinx2.dat" using 1:2 with lines title "1st method",\
"rawdata/sinx2.dat" using 1:3 with lines title "2nd method",\
 "rawdata/sinx2.dat" using 1:4 with lines title "3rd method",\
  "rawdata/sinx2.dat" using 1:5 with lines title "4th method",\
   "rawdata/sinx2.dat" using 1:6 with lines title "5th method"
