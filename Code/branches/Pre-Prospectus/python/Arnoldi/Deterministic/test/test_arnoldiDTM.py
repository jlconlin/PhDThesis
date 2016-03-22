__id__       = "$Id: test_arnoldiDTM.py 159 2007-09-14 14:07:06Z jlconlin $"
__author__   = "$Author: jlconlin $"
__version__ = " $Revision: 159 $"
__date__     = "$Date: 2007-09-14 08:07:06 -0600 (Fri, 14 Sep 2007) $"


import scipy.linalg

import Gnuplot

import arnoldiDTM
import Matrix

if __name__ == "__main__":

    N = 10
#   A = Matrix.DingDong(N)
    A = Matrix.Diagonal(scipy.arange(1,N+1))
    print A

    I = 11     # Arnoldi Iterations
    R = 10      # Arnoldi Restarts
    q = scipy.random.random(N)
    adtm = arnoldiDTM.arnoldiDTM(A)
    values, vectors = adtm.arnoldi(q, I)

    print "Estimated Eigenvalues: %s" %(values)

    realvalues, realvectors = scipy.linalg.eig(A)
    realvalues.sort()
    print "Real values: %i, Estimated values %i" %(len(realvalues), len(values))
#   print "Real Eigenvalues: %s" %(realvalues)
#   print "Difference: %s" %(abs(realvalues-values))

    Chart = Gnuplot.Gnuplot()
    plotData = Gnuplot.Data(vectors[:,-1], with='histeps')
    Chart.plot(plotData)
