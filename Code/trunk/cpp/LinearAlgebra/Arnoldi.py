__id__       = "$Id: Arnoldi.py 531 2009-05-11 14:04:08Z jlconlin $"
__author__   = "$Author: jlconlin $"
__version__ = " $Revision: 531 $"
__date__     = "$Date: 2009-05-11 08:04:08 -0600 (Mon, 11 May 2009) $"

"""
This is an implementation of Implicitly Restarted Arnoldi's Method.  It is a
prototype used to check the C++ implementation.
"""

import sys
import string
import numpy
import QRAlgo

class Arnoldi(object):

    def __init__(self, A):
        self._k = 0.0
        self._A = A
        object.__init__(self)

    def arnoldi(self, f, m):
        """
        arnoldi is the main function in this class to perform Arnoldi's method.

        f: Starting vector
        m: Number of Arnoldi Iterations to perform
        """
        self._length = f.size
        if not self._k: # First call of Arnoldi
            self._H = numpy.zeros((m,m))
            self._V = numpy.zeros( (self._length, m) )

            beta = numpy.linalg.norm(f, 2)
            v = f/beta    # Normalize
            self._V[:,0] = v    # Add v as first column of _V

            w = numpy.dot(self._A, v)
            alpha = numpy.dot(v, w)
            f = w - v*alpha
            self._H[0,0] = alpha

        for self._k in xrange(self._k+1, m):
            beta = numpy.linalg.norm(f,2)
            v = f/beta  # Normalize

            self._H[self._k, self._k-1] = beta
            self._V[:,self._k] = v

            print "\t Iteration #: %i" %self._k
            w = numpy.dot(A, v)
            # Orthogonalize
            h = numpy.dot(self._V[:,:self._k+1].transpose(), w) 
            f = w - numpy.dot(self._V[:,:self._k+1], h) 
            self._H[:self._k+1,self._k] = h
            # Re-orthogonalize
            h = numpy.dot(self._V[:,:self._k+1].transpose(), f) 
            f -= numpy.dot(self._V[:,:self._k+1], h)
            self._H[:self._k+1,self._k] += h

        return f
        
    def iram(self, f, n, m, r = 2):
        """
        iram performs Implicitly Restarted Arnoldi's Method.  It utilizes the
        arnoldi method to do its work.

        f: Starting vector
        n: Number of desired eigenvalues
        m: total number of Arnoldi Iterations
        r: Number of restarts
        """
        
        self._n = n
        self._m = m
        for self._r in xrange(r):
            f = self.arnoldi(f, m)
            self._Eigenvalues = numpy.linalg.eigvals(
                    self._H[:self._k+1, :self._k+1])
            self._Eigenvalues.sort()
            print "Restart #: %i, Eigenvalues %s" %(self._r, 
                    self._Eigenvalues[-1])

            print "Orthogonal?\n%s" %numpy.dot(self._V[:,:self._k].transpose(),
                    self._V[:,:self._k])
            self._Shifts = self._Eigenvalues[:-self._n]
            
            Q = numpy.diag(numpy.ones(self._m))
            Q, self._H[:self._k+1, :self._k+1] = QRAlgo.QRAlgShift(
                    self._H[:self._k+1, :self._k+1], self._Shifts, Q)
            self._k = self._n-1
            tmp = numpy.dot(self._V, Q[:, self._k+2])
            f = tmp*self._H[self._k+2,self._k] + \
                    f*Q[self._m-1, self._k]
            self._V[:,:self._k+1] = numpy.dot(self._V, Q[:, :self._k+1]) 

    def QRSteps(self):
        """
        QRSteps takes care of the implicit QR steps of IRAM.  It returns the
        cumulative orthogonal matrix created over all the shifted QR steps.
        """
        V = numpy.eye(self._m)
        for shift in self._Shifts:
            Q, R = numpy.linalg.qr(
                self._H[:self._k+1, :self._k+1] - shift*numpy.eye(self._k+1) )

            self._H[:self._k+1,:] = numpy.dot(Q.transpose(), self._H[:self._k+1,:])
            self._H[:,:self._k+1] = numpy.dot(self._H[:, :self._k+1], Q)
            V[:, :self._k+1] = numpy.dot(V[:, :self._k+1], Q)

        # Clean up rounding error below subdiagonal
        for i in xrange(len(self._H)):
            for j in xrange(i-1):
                self._H[i,j] = 0

        return V

def Hessenberg(n):
    H = numpy.zeros((n,n))
    k = 0;
    for i in xrange(len(H)):
        for j in xrange(len(H[0])):
            if i <= j+1:
                k += 1
                H[i,j] = k
    return H

def Diagonal(n):
    A = numpy.diag(numpy.arange(1,n+1),0)
    return A

def Standard(n):
    A = Diagonal(n)
    A[2,1] = 1
    return A

def Full(n):
    F = numpy.array(range(1, n*n+1))
    F.resize((n,n))
    return F


if __name__ == "__main__":
    n = 5
    numValues = 2
    print "numValues = %i" %numValues
    numIters = 4
    numRestarts = 10

    if len(sys.argv) > 1 and string.lower(sys.argv[1]) == 'h':
        A = Hessenberg(n)
    else:
        A = Diagonal(n)

    v = numpy.ones(n)
    print "A:\n%s\nv:%s" %(A, v)

    Arn = Arnoldi(A)
#   Arn.arnoldi(v, 4)
    Arn.iram(v,numValues, numIters, numRestarts)

