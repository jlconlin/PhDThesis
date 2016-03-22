#! /usr/local/bin/python2.4

import ArnoldiDtm
import numpy

__author__   = "$Author: jlconlin $"
__version__ = " $Revision: 83 $"
__date__     = "$Date: 2007-06-14 15:35:15 -0600 (Thu, 14 Jun 2007) $"
__id__       = "$Id: Test.py 83 2007-06-14 21:35:15Z jlconlin $"


N = 1000
A = numpy.zeros((N,N))

for i in xrange(N):
#   A[i,i] = i + 1
    A[i,i] = N - i

# Create starting vector
q0 = numpy.ones(A.shape[0])
X = ArnoldiDtm.Arnoldi(A, debug=1)
q0 = q0/X.Norm(q0,2)
#print "A: \n%s" %A
#print "q0: %s" %q0
#Pairs = X.Arnoldi(N+5, q0, debug=1)
Restarts = 10
Iterations = 20
X.ERAM(Iterations, Restarts, q0, debug=0)
ChartTitle = "A.shape = (%i,%i), Restarts = %i, Iterations = %i" %(A.shape[0], A.shape[1], Restarts, Iterations)
X.PlotResidual(ChartTitle=ChartTitle)
