__id__       = "$Id: kvsIterationNoisy.py 94 2007-07-13 19:48:51Z jlconlin $"
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
    iterations = 50
    q = scipy.ones(M)
    Noise = float(sys.argv[1])
    print "Noise: %s" %Noise

    Chart = Gnuplot.Gnuplot()
    Chart.xlabel('Iteration')
    Chart.ylabel('Estimated eigenvalue')
    Chart.title('Noise = %4G' %(Noise))
    Chart('set yrange [3.0:]')
    Chart('set pointsize 0.5')
    Chart('set key right bottom')
    Chart('set y2tics')
#   Chart('set no key')
    
    # Not Noisy
    Chart.replot(Gnuplot.Data(range(iterations), scipy.ones(iterations)*scipy.pi,  with='lines',
        title='True eValue'))
#   adtm = arnoldiDTM.arnoldiDTM(A, Noise=0.0, NoiseShape='uniform')
#   ePairs = adtm.arnoldi(q, iterations)
#   conData = Gnuplot.Data(adtm.convergence, with='linespoints', 
#           title='NotNoisy')
#   Chart.replot(conData)
    
    eValues = []
    for n in xrange(20):
        adtm = arnoldiDTM.arnoldiDTM(A, Noise=Noise, NoiseShape='normal')
        ePairs = adtm.arnoldi(q, iterations)
        eValues.append(ePairs[0][-1])   
        conData = Gnuplot.Data(adtm.convergence, with='linespoints', 
                title=None)
        Chart.replot(conData)

    print "Averaage eigenvalue: %.4f" %(scipy.mean(eValues))
