__id__       = "$Id: Matrix.py 98 2007-07-18 19:41:04Z jlconlin $"
__author__   = "$Author: jlconlin $"
__version__ = " $Revision: 98 $"
__date__     = "$Date: 2007-07-18 13:41:04 -0600 (Wed, 18 Jul 2007) $"

import scipy


"""Matrix contains several methods that return a 2-D scipy.array with various 
properties.  Most of these come from the matrix market deli."""

def Diagonal(diag):
    """
    DiagonalMatrix makes a diagonal matrix (clearly) with elements equal to the
    elements of diag.

    diag: array of elements for diagonal
    """
    return scipy.diagflat(diag)


def DingDong(N):
    """
    This function will generate an (N x N) matrix whose eigenvalues cluster around
    pi/2 and -pi/2.  See:
        http://math.nist.gov/MatrixMarket/deli/DingDong/

    References:
        This generator is adapted from Nicholas J. Higham's Test Matrix Toolbox.
        J.C. Nash, Compact Numerical Methods for Computers: Linear Algebra and Function 
        Minimisation, second edition, Adam Hilger, Bristol, 1990 (Appendix 1).
    """
    A = scipy.zeros((N,N))
    for i in xrange(N):
        for j in xrange(N):
            A[i,j] = 1.0/(2*(10-i-j+(3/2.0)))
    return A



