__id__       = "$Id: Arnoldi.py 349 2008-08-07 18:32:50Z jlconlin $"
__author__   = "$Author: jlconlin $"
__version__ = " $Revision: 349 $"
__date__     = "$Date: 2008-08-07 12:32:50 -0600 (Thu, 07 Aug 2008) $"

"""
This is an implementation of Implicitly Restarted Arnoldi's Method.  It is a
prototype used to check the C++ implementation.
"""

import numpy

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
            self._Eigenvalues, self._Eigenvectors = numpy.linalg.eig(
                    self._H[:self._k+1, :self._k+1])
            self._Eigenvalues.sort()
            print "Restart #: %i, Eigenvalues %s" %(
                    self._r, self._Eigenvalues[-1])
            self.MakeRitzPairs()

            self._Shifts = self._Eigenvalues[:self._n]
            
            Q = self.QRSteps()
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
        self._k = self._n-1

        return V

    def MakeRitzPairs(self):
        """
        """
        self._Ritzvectors = numpy.dot(self._V, self._Eigenvectors)


if __name__ == "__main__":
    n = 5
    kI = 2
    jI = 2

    A = numpy.diag( xrange(1,n+1) )
    A[2,1] = 1.0
    v = numpy.ones(n)
    print "A:\n%s\nv:%s" %(A, v)

    Arn = Arnoldi(A)
#   Arn.arnoldi(v, 4)
    Arn.iram(v,2,4,10)
    print "V:\n%s\nH:\n%s" %(Arn._V, Arn._H)

