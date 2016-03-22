#! /usr/local/bin/python2.4

import ArnoldiDtm
import numpy
import random
import Gnuplot

__author__   = "$Author: jlconlin $"
__version__ = " $Revision: 83 $"
__date__     = "$Date: 2007-06-14 15:35:15 -0600 (Thu, 14 Jun 2007) $"
__id__       = "$Id: ArnoldiDtmMain.py 83 2007-06-14 21:35:15Z jlconlin $"


def main():
    N = 10
    A = DiagonalMatrix(N)
#   A = DingDongMatrix(N)

#   print "A:\n%s" %A

    Restarts = 30
    Iterations = 10
    X = ArnoldiDtm.Arnoldi(A, Iterations, Restarts, debug=1)

    q0 = numpy.ones(A.shape[0]) # Uniform starting vector
#   q0 = numpy.zeros(A.shape[0]) # Uniform starting vector
#   q0[5] = 1
    q0 = q0/X.Norm(q0,2)
    Chart = Gnuplot.Gnuplot(debug=0)
    ChartTitle = "A.shape = (%i,%i), Restarts = %i, Iterations = %i, Uniform q0" %(
        A.shape[0], A.shape[1], Restarts, Iterations)

    DiagTitle = "Diagonal Matrix, " + ChartTitle
    Chart.title(DiagTitle)

    print "eValues: %s" %X.Values
    print "eVector: %s" %X.Vectors[-1]
    Pairs = X.Arnoldi(q0)
    print "eValues: %s" %Pairs[0]
    print "eVector: "
    X.PrintMatrix(Pairs[1])

#   X.ERAM(q0, Chart=Chart, debug=0)
#   X.PlotResidual(ChartTitle=DiagTitle)
#   X.PrintMatrix(X.H)

#   q0 = numpy.random.rand(N)
#   q0 = q0/X.Norm(q0,2)
#   Chart = Gnuplot.Gnuplot(debug=0)
#   Chart.title(DiagTitle)
#   X.ERAM(q0, Chart=Chart, debug=0)
#   X.PlotResidual(ChartTitle=DiagTitle)


#   A = DingDongMatrix(N)
#   Y = ArnoldiDtm.Arnoldi(A, Iterations, Restarts, debug=1)

#   q0 = numpy.ones(A.shape[0]) # Uniform starting vector
#   q0 = q0/X.Norm(q0,2)
#   Y.ERAM(q0, Plot=True, debug=0)
#   ChartTitle = "Diagonal Matrix, A.shape = (%i,%i), Restarts = %i, Iterations = %i, Random q0" %(
#       A.shape[0], A.shape[1], Restarts, Iterations)
    Chart.title("DingDong Matrix, " + ChartTitle)
#   Y.PlotResidual(ChartTitle=ChartTitle)

#   q0 = numpy.random.rand(N)
#   q0 = q0/X.Norm(q0,2)
#   Y.ERAM(q0, Plot=True, debug=0)
#   ChartTitle = "DingDong Matrix, A.shape = (%i,%i), Restarts = %i, Iterations = %i, Random q0" %(
#       A.shape[0], A.shape[1], Restarts, Iterations)
#   Y.PlotResidual(ChartTitle=ChartTitle)


def DiagonalMatrix(N, Direction=1):
    """
    DiagonalMatrix makes a diagonal matrix (clearly) of size N x N whose
    diagonal elements are integers, equal to the row index.

    N: Size of matrix
    Direction: Whether A[0,0] should be N or 1.  If Direction == 1, A[0,0] == N.
    """
    A = numpy.zeros((N,N))
    if Direction == 1:
        for i in xrange(N):
            A[i,i] = 1.0/(i + 1)
    else:
        for i in xrange(N):
            A[i,i] = N - i

    return A


def DingDongMatrix(N):
    """
    This function will generate an (N x N) matrix whose eigenvalues cluster around
    pi/2 and -pi/2.  See:
        http://math.nist.gov/MatrixMarket/deli/DingDong/

    References:
        This generator is adapted from Nicholas J. Higham's Test Matrix Toolbox.
        J.C. Nash, Compact Numerical Methods for Computers: Linear Algebra and Function 
        Minimisation, second edition, Adam Hilger, Bristol, 1990 (Appendix 1).
    """
    A = numpy.zeros((N,N))
    for i in xrange(N):
        for j in xrange(N):
            A[i,j] = 1.0/(2*(10-i-j+(3/2.0)))
    return A

main()
