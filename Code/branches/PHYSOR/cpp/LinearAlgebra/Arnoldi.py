__id__       = "$Id: Arnoldi.py 246 2008-01-09 21:46:05Z jlconlin $"
__author__   = "$Author: jlconlin $"
__version__ = " $Revision: 246 $"
__date__     = "$Date: 2008-01-09 14:46:05 -0700 (Wed, 09 Jan 2008) $"

"""This module contains the base class for all Arnoldi classes."""

import time
import scipy.linalg

class Arnoldi(object):
    """
    Arnoldi is the base class for all Arnoldi based methods.  It will be 
    inherited at least by a Monte Carlo and a Matrix based classes.
    """
    def __init__(self, verbose = True, resTol = 1E-10, invTol = 1E-12):
        """
        verbose: If detailed information should be printed to screen
        resTol: Tolerance for residual
        invTol: Tolerance for determination of invariance
        """
        self.Verbose = verbose
        self.resTolerance = resTol  # Tolerance for residual
        self.invTolerance = invTol  # Tolerance for invariance
        object.__init__(self)


    def ERAM(self, q, R, I, NEig = 3):
        """
        ERAM is Explicitly Restarted Arnoldi's Method.  It returns the 
        estimated eigenvalues and eigenvectors.

        q: Initial vector
        R: Number of Arnoldi Restarts
        I: Number of Arnoldi Iterations per restart
        NEig: The number of Eigenpair estimates to store after each Restart
        """
        self.RestartedResidual = []
        self.Eigenvalues = []
        self.Eigenvectors = []
        for self.r in xrange(1, R + 1):
            Values, Vectors = self.Restart(q, I, NEig)
            q = Vectors[-1]
            
            # Determine if ERAM should stop
            if self.reachedTol: break

        return Values, Vectors


    def Restart(self, q, I, NEig):
        raise NotImplementedError


    def RestartBase(self, q, I, NEig):
        """
        Restart performs one Arnoldi Restart with a given number of iterations.
        It returns the eigenpair estimates.

        q: Initial vector
        I: Number of Arnoldi Iterations in this restart
        NEig: The number of Eigenpair estimates to store after each Restart
        """
        Values, Vectors = self.arnoldi(q, I)
        self.Eigenvalues.append(Values[-NEig:])
        self.Eigenvectors.append(Vectors[-NEig:])
        self.RestartedResidual.extend(self.Residual)
        
        return Values, Vectors


    def arnoldi(self, q, I):
        """
        arnoldi is the primary method.  arnoldi returns the estimated
        eigenvalues and eigenvectors of the matrix/linear operator A. 

        q: Initial vector/function
        I: Number of Arnoldi Iterations
        """
        self.H = scipy.zeros((I+1, I))
        q = q/scipy.linalg.norm(q,2)
        self.Q = [q]

        self.Residual = []
        self.reachedTol = False

        self.StartTime = time.time()
        for self.k in xrange(1, I+1):
            self.Iteration()
            Values, vectors = self._SortedPairs(self.H[:self.k, :self.k])
            Vectors = self._calcEVectors(self.Q, vectors)
            self._calcResidual(vectors)
            
            # Determine if we should stop
            if self.H[self.k, self.k-1] < self.invTolerance:
                if self.Verbose:
                    print "I finished because H[%i, %i] = %8.4E < %6.2E" %(
                            self.k, self.k-1, self.H[self.k, self.k-1], 
                            self.invTolerance)
                    self.reachedTol = True
                    break
            elif self.Residual[-1] < self.resTolerance:
                if self.Verbose:
                    print "I finished because the residual %8.4E < %8.4E" %(
                            self.Residual[-1], self.resTolerance)
                    self.reachedTol = True
                break
        return (Values, Vectors)


    def Iteration(self):
        raise NotImplementedError

    def IterationBase(self):
        """
        Iteration performs calculations required at each Arnoldi iteration.
        """
        q = self.Aq()
        q = self._Orthogonalize(q)
        q = self._Normalize(q)


    def Aq(self):
        """
        Aq peforms the matrix-vector product or the application of the linear
        operator A on the function q.  Since this is required for Arnoldi's 
        method, this function must be defined in the derived classes.
        """
        raise NotImplementedError


    def _calcEVectors(self, Q, V):
        """
        _calcEVectors will calculate the eigenvectors of the matrix (or linear
        operator) A from the eigenvectors of H and an orthonormal set of basis
        vectors for a Krylov subspace.

        Q: Orthonormal basis vectors
        V: Eigenvectors (row vectors)
        """
        r = len(V)  # Rows in Vectors
        c = len(Q[0]) # Columns in Vectors
        Vectors = scipy.zeros((r,c))
        for j in xrange(r):
            for i in xrange(r):
                Vectors[j] += V[j,i]*Q[i]

        return Vectors


    def _calcResidual(self, vectors):
        """
        _calcResidual will calculate the residual norm and append it to the 
        container for the Residual.
        See Saad (1992) pp. 173-174.

        vectors: Eigenvectors of self.H
        """
        # Calculate residual
        res = vectors[-1][-1]*self.H[self.k, self.k-1]
        self.Residual.append(res)


    def _Normalize(self, q):
        """
        _Normalize will normalize the vector q and set it's L2-norm in upper
        Hessenberg matrix at the right position.  _Normalize will return the 
        normalized vector.

        q: Arnoldi vector to normalize
        """
        self.H[self.k, self.k-1] = scipy.linalg.norm(q,2)
        q = q/self.H[self.k, self.k-1]
        self.Q.append(q)

        return q


    def _Orthogonalize(self, q):
        """
        _Orthogonalize will orthogonalize the vector q against all previously 
        calculated Arnoldi vectors.  The Arnoldi vectors must be in a list 
        called self.Q and the upper Hessenberg matrix must be self.H.  The 
        iteration number must be self.k.  _Orthogonalize will return the 
        orthogonalized vector.

        q: Arnoldi vector to be orthogonalize
        """
        for j in xrange(1, self.k+1):
            self.H[j-1, self.k-1] = scipy.dot(self.Q[j-1], q)
            q = q - self.H[j-1, self.k-1]*self.Q[j-1]

        return q


    def _SortedPairs(self, M):
        """
        (values, vectors) = _SortedPairs(M)

        _SortedPairs calculates and sorts the eigen pairs of the matrix M.  It
        returns the eigenvectors as row vectors so they can easily be accessed
        as 

        vector[i]

        to get the eigenvector associated witht he i-th smallest eigenvalue.

        M: 2-D array representing matrix
        """
        values, vectors = scipy.linalg.eig(M)   # Returns column vectors
        sortedIndex = values.argsort()
        sortedValues = values[sortedIndex]
        sortedVectors = vectors[:,sortedIndex]
        sortedVectors = scipy.transpose(sortedVectors)  # Makes row vectors

        return (sortedValues, sortedVectors)

    
