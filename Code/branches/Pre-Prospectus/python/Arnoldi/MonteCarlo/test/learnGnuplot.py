__id__       = "$Id: learnGnuplot.py 32 2007-03-22 13:31:22Z jlconlin $"
__author__   = "$Author: jlconlin $"
__version__ = " $Revision: 32 $"
__date__     = "$Date: 2007-03-22 07:31:22 -0600 (Thu, 22 Mar 2007) $"

import scipy
import Gnuplot

def main():
#   x = scipy.arange(-35,35)/4.0
    y = scipy.arange(-70, 70)/20.0-1.5
#   x = scipy.arange(0,5)
#   xdat = scipy.ndarray((len(y),len(x)))
#   ydat = scipy.ndarray((len(x),len(y)))
#   m = (scipy.sin(xdat) + 0.1*xdat) - ydat**2
#   m = scipy.sin(scipy.sqrt(xdat**2+ydat**2))/scipy.sqrt(xdat**2+ydat**2)
    m = scipy.zeros((5,len(y)))
#   print "len(x): %s" %len(x)
#   print "len(y): %s" %len(y)
#   print "len(m): %s" %len(m)
#   print "len(m[0]): %s" %len(m[0])

#   for i in xrange(len(x)):
#       m[i] = scipy.sin(y)
#       ydat[i] = y
#   for i in xrange(len(y)):
#       xdat[i] = x

#   print "x: %s" %xdat
#   print "y: %s" %ydat
#   print "m: %s" %m

    Chart = Gnuplot.Gnuplot(debug=1)
#   Chart('set pm3d')
#   Chart('set parametric')
#   Chart('set data style lines')
#   Chart('set hidden')
#   Chart('set contour base')
#   dat = Gnuplot.GridData(m,x,y, binary=1, filename='filename1')
    sine = Gnuplot.Func('sin(x), 4', with='filledcurves')
    Chart.plot(sine)
#   Chart.splot(Gnuplot.File('filename1', binary=1))


main()


