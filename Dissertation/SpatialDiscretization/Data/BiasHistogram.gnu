#!/usr/local/bin/gnuplot -persist
#
#    
#    	G N U P L O T
#    	Version 4.2 patchlevel 5 
#    	last modified Mar 2009
#    	System: Darwin 9.7.0
#    
#    	Copyright (C) 1986 - 1993, 1998, 2004, 2007 - 2009
#    	Thomas Williams, Colin Kelley and many others
#    
#    	Type `help` to access the on-line reference manual.
#    	The gnuplot FAQ is available from http://www.gnuplot.info/faq/
#    
#    	Send bug reports and suggestions to <http://sourceforge.net/projects/gnuplot>
#    
 set terminal aqua 0 title "Figure 0" size 846 594 font "Times-Roman,12" enhanced solid
 set terminal pslatex color solid rotate size 6in, 4.5in
unset clip points
set clip one
unset clip two
set bar 1.000000
set border 31 front linetype -1 linewidth 1.000
set xdata
set ydata
set zdata
set x2data
set y2data
set timefmt x "%d/%m/%y,%H:%M"
set timefmt y "%d/%m/%y,%H:%M"
set timefmt z "%d/%m/%y,%H:%M"
set timefmt x2 "%d/%m/%y,%H:%M"
set timefmt y2 "%d/%m/%y,%H:%M"
set timefmt cb "%d/%m/%y,%H:%M"
set boxwidth
set style fill  empty border
set style rectangle back fc lt -3 fillstyle  solid 1.00 border -1
set dummy x,y
set format x "% g"
set format y "% g"
set format x2 "% g"
set format y2 "% g"
set format z "% g"
set format cb "% g"
set angles radians
unset grid
set key title ""
set key inside right bottom vertical Right noreverse enhanced autotitles box
set key noinvert samplen 4 spacing 1 width -12 height 1 
unset label
unset arrow
set style increment default
unset style line
unset style arrow
set style histogram clustered gap 2 title  offset character 0, 0, 0
unset logscale
set logscale x 10
set logscale y 10
set offsets 0, 0, 0, 0
set pointsize 0.5
set encoding default
unset polar
unset parametric
unset decimalsign
set view 60, 30, 1, 1
set view 
set samples 100, 100
set isosamples 10, 10
set surface
unset contour
set clabel '%8.3g'
set mapping cartesian
set datafile separator whitespace
unset hidden3d
set cntrparam order 4
set cntrparam linear
set cntrparam levels auto 5
set cntrparam points 5
set size ratio 0 1,1
set origin 0,0
set style data points
set style function lines
set xzeroaxis linetype -2 linewidth 1.000
set yzeroaxis linetype -2 linewidth 1.000
set zzeroaxis linetype -2 linewidth 1.000
set x2zeroaxis linetype -2 linewidth 1.000
set y2zeroaxis linetype -2 linewidth 1.000
set ticslevel 0.5
set mxtics default
set mytics default
set mztics default
set mx2tics default
set my2tics default
set mcbtics default
set xtics border in scale 1,0.5 mirror norotate  offset character 0, 0, 0
set xtics autofreq  norangelimit
set ytics border in scale 1,0.5 mirror norotate  offset character 0, 0, 0
set ytics autofreq  norangelimit
set ztics border in scale 1,0.5 nomirror norotate  offset character 0, 0, 0
set ztics autofreq  norangelimit
set nox2tics
set noy2tics
set cbtics border in scale 1,0.5 mirror norotate  offset character 0, 0, 0
set cbtics autofreq  norangelimit
set title "" 
set title  offset character 0, 0, 0 font "" norotate
set timestamp bottom 
set timestamp "" 
set timestamp  offset character 0, 0, 0 font "" norotate
set rrange [ * : * ] noreverse nowriteback  # (currently [0.00000:10.0000] )
set trange [ * : * ] noreverse nowriteback  # (currently [-5.00000:5.00000] )
set urange [ * : * ] noreverse nowriteback  # (currently [-5.00000:5.00000] )
set vrange [ * : * ] noreverse nowriteback  # (currently [-5.00000:5.00000] )
set xlabel "Bin Width (mfp)" 
set xlabel  offset character 0, 0, 0 font "" textcolor lt -1 norotate
set x2label "" 
set x2label  offset character 0, 0, 0 font "" textcolor lt -1 norotate
set xrange [ * : 2.5 ] noreverse nowriteback  # (currently [-10.0000:] )
set x2range [ * : * ] noreverse nowriteback  # (currently [-10.0000:10.0000] )
set ylabel "Eigenvalue Estimate Error" 
set ylabel  offset character 0, 0, 0 font "" textcolor lt -1 rotate by 90
set y2label "" 
set y2label  offset character 0, 0, 0 font "" textcolor lt -1 rotate by 90
set yrange [ * : * ] noreverse nowriteback  # (currently [-10.0000:10.0000] )
set y2range [ * : * ] noreverse nowriteback  # (currently [-10.0000:10.0000] )
set zlabel "" 
set zlabel  offset character 0, 0, 0 font "" textcolor lt -1 norotate
set zrange [ * : * ] noreverse nowriteback  # (currently [-10.0000:10.0000] )
set cblabel "" 
set cblabel  offset character 0, 0, 0 font "" textcolor lt -1 rotate by 90
set cbrange [ * : * ] noreverse nowriteback  # (currently [-10.0000:10.0000] )
set zero 1e-08
set lmargin  -1
set bmargin  -1
set rmargin  -1
set tmargin  -1
set locale "C"
set pm3d explicit at s
set pm3d scansautomatic
set pm3d interpolate 1,1 flush begin noftriangles nohidden3d corners2color mean
set palette positive nops_allcF maxcolors 0 gamma 1.5 color model RGB 
set palette rgbformulae 7, 5, 15
set colorbox default
set colorbox vertical origin screen 0.9, 0.2, 0 size screen 0.05, 0.6, 0 front bdefault
set loadpath 
set fontpath 
set fit noerrorvariables
GNUTERM = "aqua"

# Get Data for making labels
set table
plot "BiasHistogram.dat" using 0:($0==1?(xf=$1):$1)
# This means:
#   0:          Get entry numbers, not column
#   ($0==0?     If entry number is zero
#   (xf=$1)         set zeroth entry of column 1 to the variable xf
#   :$1             else plot entry from column 1
unset table
set output "BiasHistogram.tex"

# Fit data
set fit errorvariables  # Get uncertainty values for fit parameters
l0(x) = x**m0*10**b0   # fit line to data for log-log plot
l1(x) = x**m1*10**b1   # fit line to data for log-log plot
l2(x) = x**m2*10**b2   # fit line to data for log-log plot
fit l0(x) "BiasHistogram.dat" using 1:2:3 via m0,b0
fit l1(x) "BiasHistogram.dat" using 4:5:6 via m1,b1
fit l2(x) "BiasHistogram.dat" using 7:8:9 via m2,b2

# Rotation angles
set angles degrees
r0 = atan(m0)
r1 = atan(m1)
r2 = atan(m2)

# display slopes
fmt = 'slope  = %.2f +- %.2G'
m0fit = sprintf(fmt, m0, m0_err)
m1fit = sprintf(fmt, m1, m1_err)
m2fit = sprintf(fmt, m2, m2_err)
set label m0fit at xf-0.005, l0(xf) center front rotate by r0/3 offset 0,1 tc lt 1
set label m1fit at xf-0.005, l1(xf) center front rotate by r1/3 offset 0,0.75 tc lt 2
set label m2fit at xf-0.005, l2(xf) center front rotate by r2/3 offset 0,1 tc lt 3

plot "BiasHistogram.dat" using 1:2:3 with errorbars ls 1 title "$\\mathcal{B}_0$" axes x1y1, "BiasHistogram.dat" using 4:5:6 with errorbars ls 2 title "$\\mathcal{B}_1$" axes x1y1, "BiasHistogram.dat" using 7:8:9 with errorbars ls 3 title "$\\mathcal{B}_2$" axes x1y1, l0(x) lt 1 lw 2 notitle axes x1y1, l1(x) lt 2 lw 3 notitle axes x1y1, l2(x) lt 3 lw 2 notitle 
# replot
#    EOF
