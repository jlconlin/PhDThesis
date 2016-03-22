__id__       = "$Id: ArnoldiMatrix.py 339 2008-07-29 19:17:18Z jlconlin $"
__author__   = "$Author: jlconlin $"
__version__ = " $Revision: 339 $"
__date__     = "$Date: 2008-07-29 13:17:18 -0600 (Tue, 29 Jul 2008) $"

"""
This is an implentation of Arnoldi's method where the linear operator is just a
matrix.
"""

import sys

import numpy

import Arnoldi

class ArnoldiMatrix(Arnoldi.Arnoldi):

    def __init__(self, A):
        self._A = A
        Arnoldi.Arnoldi.__init__(self)

    def Av(self):
        """
        """
        v = self._V[:,self._k]
        self._v = numpy.dot(self._A, v)

    def Restart(self):
        """
        """
        self._Restart()
        print "Restart #: %4i" %(self._r)

    def Iteration(self):
        """
        """
        self._Iteration()
        print "\tIteration#: %4i" %(self._k)

def Diagonal(n):
    A = numpy.diag(numpy.arange(1,n+1),0)
    return A

def Standard(n):
    A = Diagonal(n)
    A[2,1] = 1
    return A

if __name__ == "__main__":
    n = 5
    kI = 2
    jI = 2

    A = Diagonal(n)
    v = numpy.ones(n)
    print "A:\n%s\nv:%s" %(A, v)

    ArnMat = ArnoldiMatrix(A)
    ArnMat.arnoldi(v,4)
#   ArnMat.iram(v,2,4,2)
    print "V:\n%s\nH:\n%s" %(ArnMat._V, ArnMat._H)

