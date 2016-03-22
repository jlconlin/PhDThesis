_id__       = "$Id: NBPseudoDiag.py 141 2007-08-23 12:25:42Z jlconlin $"
__author__   = "$Author: jlconlin $"
__version__ = " $Revision: 141 $"
__date__     = "$Date: 2007-08-23 06:25:42 -0600 (Thu, 23 Aug 2007) $"

"""NBPseudoDiag calculates the eigenvalue bias of a matrix B which is a similar
matrix to a diagonal matrix.  B is calculated using an orthogonal set of column
vectors from an Arnoldi's method calculation."""

import sys
import cmath
import time

import scipy
import scipy.linalg

import arnoldiDTM
import NumBias
import Matrix
import gnuplotFile

if __name__ == "__main__":

    numby = NumBias.NB(version=str(__version__))

    A = Matrix.Diagonal(scipy.arange(numby.options.size,0,-1))

    # Calculate similarDiag matrix
    adtm = arnoldiDTM.arnoldiDTM(A, Noise=0.0)
    q = scipy.ones(numby.options.size)
    adtm.arnoldi(q, numby.options.size)
    Q = scipy.array(adtm.Q)
    B = scipy.dot(scipy.dot(Q[:-1].transpose(),A),Q[:-1])
    # Double-check
    print "\n\nDouble-check:"
    print "Eigenvalues of A:\n%s\nEigenvalues of B:\n%s\n" %(
            scipy.linalg.eigvals(A), scipy.linalg.eigvals(B))

    print "A: \n%s" %B
    numby.printParameters()
    if numby.options.run:
        numby.run(A=B, q=q)
