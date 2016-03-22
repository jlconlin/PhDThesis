__id__       = "$Id: arnoldiMC.py 208 2007-12-04 02:59:34Z jlconlin $"
__author__   = "$Author: jlconlin $"
__version__ = " $Revision: 208 $"
__date__     = "$Date: 2007-12-03 19:59:34 -0700 (Mon, 03 Dec 2007) $"

import math
import copy
import time
import os

import scipy
import scipy.linalg

import Markov
import fissionBank
import fissionSource
import particle
import random



class arnoldiMC(Markov.Markov):
    """
    """

    def __init__(self, geo, xs, histories=1000, storeVectors=False,
           relaxed=False,  verbose=False):
        """
        geo: Geometry of the simulation
        xs: Cross sections for the simulation
        histories: Number of histories to run in each cycle
        source: fissionSource object defining initial "guess"
        storeVectors: Whether the dominant eigenvector should be stored between
            iterations
        """
        self.origHistories = histories
        self.histList = []
        self.verbose = verbose
        if storeVectors:
            self.vectorStorage = []
        else:
            self.vectorStorage = None
        self.residual = []
        self.convergence = []   # List of eigenvalues at each iteration
        self.iterVectors = []
        self.relaxed = relaxed

        Markov.Markov.__init__(self, geo, xs, histories)


    def ERAM(self, q, R, inactiveR, Iterations):
        """
        ERAM is Explicitly Restarted Arnoldi's Method.  It is similar to
        self.arnoldi() except it calculates a new starting source after a few
        iterations.  ERAM returns the estimated eigenvalues and eigenvectors.

        q: fissionSource object, starting source vector
        R: How many Arnoldi Restarts
        inactiveR: The number of Restarts to throw away
        Iterations: How many Arnoldi Iterations between restarts
        """
        self.eValues = []
        self.mean = []
        self.std = []
        start = time.time()

        # Inactive Restarts
        for i in xrange(inactiveR):
            Values, Vectors = self.arnoldi(q, Iterations)
            q = fissionSource.histogramSource(abs(Vectors[:,-1]), self.geo)

        for self.r in xrange(1, R+1):
            Values, Vectors = self.arnoldi(q, Iterations)
            self.eValues.append(Values)
            self.mean.append(scipy.mean(self.eValues,0)[-1])
            self.std.append((1/math.sqrt(self.r))*scipy.std(self.eValues,0)[-1])
            self.histList.append(self.resHist)

            if isinstance(self.vectorStorage, list):
                v = Vectors[:,-1]/sum(Vectors[:,-1])
                self.vectorStorage.append(v)
            totaltime = time.time()-start
            if self.verbose:
                print "Restart #: %4i, eValue = %8.5f, std.dev. = %6.4f," %(
                        self.r, self.mean[-1], self.std[-1]),
                print " residual = %8.4E, time: %8.3f sec" %(
                        self.residual[-1], totaltime)

            if self.residual[-1] < 1e-12:
                print "I'm stopping restarting because residual is too small"
                break
            elif self.H[self.k, self.k-1] < 1e-12:
                print "I finished because H[%i,%i] = %8.4E < 1E-12" %(self.k+1,
                        self.k, self.H[self.k,self.k-1])
                break
            else:
                q = fissionSource.histogramSource(abs(Vectors[:,-1]), self.geo)

        return Values, Vectors
                

    def arnoldi(self, q, iterations):
        """
        arnoldi is the master method, controlling the methods required to
        perform the actual method.  arnoldi returns the estimated eigenvalues
        and eigenvectors of the linear operator.

        q: fissionSource object, starting source vector
        Iterations: The number of Arnoldi iterations.  In Monte Carlo parlance
            this could be called "cycles" or "generations".
        """
        # Store space for upper Hessenberg Matrix
        self.H = scipy.zeros((iterations+1, iterations))
        q = q/scipy.linalg.norm(q,2)
        self.Q = [q]    # List of orthonoormal q's/basis vectors
        self.oQ = [q]   # List of Original q's (pre-orthogonalization)

        start = time.time()
        self.histories = self.origHistories
        self.resHist = 0
        for self.k in xrange(1, iterations + 1):
            self.resHist += self.histories
            self.iteration()
            totaltime = time.time()-start
            if self.relaxed:
                self.histories = 0.9*self.histories
            if self.verbose:
                print "\titeration #: %3i, time: %7.3f sec" %(self.k, totaltime)
            if self.H[self.k,self.k-1] < 1e-12:
                print "I finished because H[%i,%i] = %8.4E < 1E-12" %(self.k+1,
                        self.k, self.H[self.k,self.k-1])
                break

            Values, vectors = self._EigenPairs(self.H[:self.k,:self.k])
            self.convergence.append(Values[-1])

            # Calculate Residual
            res = vectors[-1][-1]*self.H[self.k, self.k-1]
            self.residual.append(abs(res))
            Vectors = self._calc_eVectors(self.Q, vectors)
        
        return Values, Vectors


    def iteration(self):
        """
        Iteration performs one Arnoldi iteration.  This step includes the Monte
        Carlo (Aq), orthogonalization, and normalization.

        """
        q = self.Aq_OneSource(self.Q[self.k-1])  # Linearly operate

        # Orthogonlize
        for j in xrange(1, self.k+1):
            self.H[j-1,self.k-1] = scipy.dot(self.Q[j-1],q)
            q = q - self.H[j-1,self.k-1]*self.Q[j-1]

        # Normalize
        self.H[self.k,self.k-1] = scipy.linalg.norm(q, 2)
        q = q/self.H[self.k,self.k-1]
        self.Q.append(q)

        # Save vector for convergence study
        Values, vectors = self._EigenPairs(self.H[:self.k,:self.k])
        Vectors = self._calc_eVectors(self.Q, vectors)
        v = Vectors[:,-1]/sum(Vectors[:,-1])
        self.iterVectors.append(v)
        

    def Aq(self, Q):
        """
        Aq will operate on Q by the linear operator A.  
        In Monte Carlo, this is just solving the transport equation.

        Q: Fission source
        """
        # Transport Bank
        qPos = copy.deepcopy(Q)
        qNeg = copy.deepcopy(Q)
        for i in xrange(len(qPos)):
            if qPos[i] < 0:
                qPos[i] = 0
        qNeg = qPos - qNeg

        if sum(qPos) > 0:
            self.Bank = fissionSource.histogramSource(
                    scipy.zeros(self.geo.bins), self.geo)
            self.transport(qPos)
            nextPos = self.Bank*sum(qPos)/self.histories
        else:
            nextPos = fissionSource.histogramSource(
                    scipy.zeros(self.geo.bins), self.geo)

        if sum(qNeg) > 0:
            self.Bank = fissionSource.histogramSource(
                    scipy.zeros(self.geo.bins), self.geo)
            self.transport(qNeg)
            nextNeg = self.Bank*sum(qNeg)/self.histories
        else:
            print "No negative sources."
            nextNeg = fissionSource.histogramSource(
                    scipy.zeros(self.geo.bins), self.geo)

        q = (nextPos - nextNeg)
        self.oQ.append(q)

        return q


    def Aq_OneSource(self, Q):
        """
        This is just like Aq, but it passes one source (both positive and 
        negative to be transported, instead of doing two sepaate runs.
        Aq will operate on Q  by the linear operator A.  
        In Monte Carlo, this is just solving the transport equation.

        Q: Fission source
        """
        # Transport Bank
        self.Bank = fissionSource.histogramSource(
                scipy.zeros(self.geo.bins), self.geo)
        self.transport(Q)

        # Make fissionSource
        Qsum = sum(abs(Q))
        q = self.Bank*(Qsum/self.histories)

        self.oQ.append(q)
        return q


    def score(self, history):
        """
        """
        w = history.weight*(self.xs.nu*self.xs.xF/self.xs.xT)
        self.Bank.score(w, history.position)


    def _EigenPairs(self, M):
        """
        _EigenPairs calculates the eigenpairs of M.
        It returns the sorted eigenpairs with the first element 
        being the smallest eigenvalue.  The eigenvectors are column vectors.

        M: Matrix whose eigenpairs, _EigenPairs will sort
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


    def printM(self, M):
        for row in M:
            self.printVector(row)


    def printVector(self, row):
        print "[",
        for element in row:
#           if abs(element) < 1E-12:
#               print '0 ',
#           elif abs(element) == 1.0:
#               print '1 ',
#           else:
#               print '# ',
#               print "%6.2E " %abs(element),
            print "%11.4E " %element,
        print "]"


