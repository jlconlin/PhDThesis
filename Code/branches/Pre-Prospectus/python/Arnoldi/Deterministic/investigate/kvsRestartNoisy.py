__id__       = "$Id: kvsRestartNoisy.py 94 2007-07-13 19:48:51Z jlconlin $"
__author__   = "$Author: jlconlin $"
__version__ = " $Revision: 94 $"
__date__     = "$Date: 2007-07-13 13:48:51 -0600 (Fri, 13 Jul 2007) $"

"""This module will investiage how the eigenvalue converges."""

import copy
import sys

import scipy
import Gnuplot

import arnoldiDTM
import Matrix

if __name__ == "__main__":
    M = 100
    A = Matrix.DingDong(M) + scipy.identity(M)*scipy.pi/2
    I = 20
    R = 5
    q = scipy.ones(M)
    Noise = float(sys.argv[1])

    Chart = Gnuplot.Gnuplot()
    Chart.xlabel('Iteration')
    Chart.ylabel('Estimated eigenvalue')
    Chart.title('Iterations = %i, Restarts = %i, Noise = %.1E' %(I, R, Noise))
#   Chart('set yrange [2.9:]')
    Chart('set pointsize 0.5')
    Chart('set key right bottom')
    
    # Not Noisy
    Chart.replot(Gnuplot.Data(range(I*R), scipy.ones(I*R)*scipy.pi,  with='lines',
        title='True eValue'))
#   adtm = arnoldiDTM.arnoldiDTM(A, Noise=0.0)
#   ePairs = adtm.ERAM(q, R, I)
#   conData = Gnuplot.Data(adtm.convergence, with='linespoints', 
#           title='NotNoisy')
#   Chart.replot(Gnuplot.Data(adtm.residual, with='linespoints',
#       title='residual', axes='x1y2'))
    Chart('set y2tics')
#   Chart('set logscale y2')
#   Chart('set tics nomirror')
#   Chart.replot(conData)
    
    for n in xrange(10):
        adtm = arnoldiDTM.arnoldiDTM(A, Noise=Noise, NoiseShape='normal')
        ePairs = adtm.ERAM(q, R, I)
        conData = Gnuplot.Data(adtm.convergence, with='linespoints', 
                title=None)

#       Chart.replot(Gnuplot.Data(adtm.residual, with='linespoints',
#           title='residual', axes='x1y2'))
        Chart.replot(conData)
