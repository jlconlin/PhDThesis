_id__       = "$Id: arnoldiDTM.py 397 2008-10-13 21:06:07Z jlconlin $"
__author__   = "$Author: jlconlin $"
__version__ = " $Revision: 397 $"
__date__     = "$Date: 2008-10-13 15:06:07 -0600 (Mon, 13 Oct 2008) $"

import math
import copy
import time
import os

import scipy
import scipy.linalg
import scipy.stats

import Errors

"""This module is a deterministic implementation of Arnoldi's method.  It can be 
used as comparison to the Monte Carlo implementation."""


class arnoldiDTM(object):
    """
    This class is a demonstration of a deterministic Arnoldi's method.
    """

    def __init__(self, A, Noise = 0.0, NoiseShape = 'normal',
            storeVectors=False, verbose = False):
        """
        A: Matrix
        Noise: The amount of noise added to the matrix-vector product Aq
        NoiseShape: The shape of the pdf of the noise
        storeVectors: Whether the dominant eigenvector should be stored between
            iterations
        """
        self.A = A
        if storeVectors:
            self.vectorStorage = []
        else:
            self.vectorStorage = None
        self.residual = []
        self.Noise = Noise
        self.NoiseShape = NoiseShape
        self.verbose = verbose
        self.convergence = []   # List of eigenvalues at each iteration
        object.__init__(self)


    def ERAM(self, q, R, I):
        """
        ERAM is Explicitly Restarted Arnoldi's Method.  ERAM returns the 
        estimated eigenvalues and eigenvector.

        q: Initial vector.
        R: Number of Arnoldi Restarts
        I: Number of Arnoldi Iterations per restart
        """
        self.eValues = []
        self.cycleVectors = []
        for self.r in xrange(1, R + 1):
            if self.verbose:
                print "Restart #: %4i" %(self.r)
            Values, Vectors = self.arnoldi(q, I)
            self.eValues.append(Values)
            self.cycleVectors.append(Vectors[:,-1])
            q = Vectors[:,-1]
            if self.H[self.k,self.k-1] < 1e-6 or self.residual[-1] < 1e-6:
                break

        return Values, Vectors


    def arnoldi(self, q, I):
        """
        arnoldi is the primary method.  arnoldi returns the estimated 
        eigenvalues and eigenvectors of the linear operator A.

        q: Initial vector.
        I: Number of Arnoldi Iterations per restart
        """
        self.H = scipy.zeros((I+1, I))
        q = q/scipy.linalg.norm(q,2)
        self.Q = [q]

        for self.k in xrange(1, I + 1):
            self.iteration()
            if self.verbose:
                print "\titeration #: %3i" %(self.k)

            Values, vectors = self._EigenPairs(self.H[:self.k, :self.k])
            self.convergence.append(Values[-1])
            # Calculate Residual
            res = vectors[-1][-1]*self.H[self.k, self.k-1]
            self.residual.append(abs(res))
            Vectors = self._calc_eVectors(self.Q, vectors)
                
            if self.H[self.k,self.k-1] < 1e-12:
                if self.verbose:
                    print "I finished because H[%i,%i] = %8.4E < 1E-12" %(
                            self.k+1, self.k, self.H[self.k,self.k-1])
                break
            elif self.residual[-1] < 1e-12:
                if self.verbose:
                    print "I finished because residual = %8.4E < 1E-12" %(
                        self.residual[-1])
                break

        return Values, Vectors


    def iteration(self):
        """
        iteration peforms one Arnoldi iteration.  This step includes the
        matrix-vector product (Aq), orthogonalization, and normalization.
        """

        q = self.Aq(self.A, self.Q[-1])

        # Orthogonalize
        for j in xrange(1, self.k+1):
            self.H[j-1, self.k-1] = scipy.dot(self.Q[j-1], q)
            q = q - self.H[j-1, self.k-1]*self.Q[j-1]

        # Normalize
        self.H[self.k, self.k-1] = scipy.linalg.norm(q,2)
        q = q/self.H[self.k, self.k-1]
        self.Q.append(q)

    
    def Aq(self, A, q):
        """
        Aq simply returns the product of A on q.  This method is included to 
        simplify customizing this feature.  It currently adds a noise 
        proportional to self.Noise.
        """
        if self.Noise != 0:
            if self.NoiseShape == 'normal':
                NoiseVector = scipy.stats.norm(0,self.Noise).rvs(len(q))
            elif self.NoiseShape == 'uniform':
                NoiseVector = scipy.stats.uniform(-self.Noise/2.0, self.Noise).rvs(len(q))
            else: 
                raise Errors.NoiseShapeError(
                        "NoiseShape unknown.  Please add NoiseShape to code.")
        else:
            NoiseVector = 0.0
        return scipy.inner(A, q) + NoiseVector


    def _EigenPairs(self, M):
        """
        _EigenPairs calculates the eigenpairs of M.
        It returns the sorted eigenpairs with the first element 
        being the smallest eigenvalue.  The eigenvectors are column vectors.

        M: Matrix whose eigenpairs _EigenPairs will sort
        """
        values, vectors = scipy.linalg.eig(M)
        sortedIndex = values.argsort()
        sortedValues = values[sortedIndex]
        sortedVectors = vectors[:,sortedIndex]

        return (sortedValues, sortedVectors)

    
    def _calc_eVectors(self, Q, V):
        """
        calc_eVectors will calculate the eigenvectors.  The 
        eigevector is a linear combination of the vectors of Q with the elements
        of an eigenvector of H as the expansion coefficients.  calc_eVecctors 
        returns a matrix who's columns are the eigenvectors.

        Q: List of orthornormal basis vectors
        V: Matrix of column vectors
        """
        n = len(V)
        m = len(Q[0])
        Vectors = scipy.zeros((m,0))
        for j in xrange(n):
            Vector = scipy.zeros(m)
            for i in xrange(n):
                Vector = Vector + V[i,j]*Q[i]
            Vectors = scipy.column_stack((Vectors,Vector))

        return Vectors

if __name__ == "__main__":
    n = 5
    R = 5
    I = 4

    q = scipy.ones(n)
#   A = scipy.diag(scipy.arange(1,n+1))
#   A[1,0] = 1.0
    A = scipy.zeros((n,n))
    k = 0
    for i in xrange(n):
        for j in xrange(i-1, n):
            A[i,j] = k
            k += 1

    print "A: \n%s" %A
    print "q: %s" %q
    arn = arnoldiDTM(A, verbose=True)
    arn.ERAM(q, R, I)

