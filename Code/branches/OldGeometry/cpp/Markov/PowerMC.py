__id__       = "$Id: PowerMC.py 244 2008-01-09 16:40:31Z jlconlin $"
__author__   = "$Author: jlconlin $"
__version__ = " $Revision: 244 $"
__date__     = "$Date: 2008-01-09 09:40:31 -0700 (Wed, 09 Jan 2008) $"

import time
import math
import scipy

import Transport.Markov.PowerMCBase

class Power(Transport.Markov.PowerMCBase.PowerMCBase):
    """
    Power is the base class for power method algorithms.  Eventually it will 
    be inherited to provide functionality for both Monte Carlo and 
    Determinstic calculations
    """

    def __init__(self, seed, F):
        """
        seed: Random number generator seed
        F: Field.FieldZoneMat
        """
        Transport.Markov.PowerMCBase.PowerMCBase.__init__(self, seed, F)

    def power(self, currentSource, active, inactive, histories, discMesh):
        """
        power is the main method for the power method

        currentSource: Initial source for power method
        active: Number of active iterations
        inactive: Number of inactive iterations
        histories: Number histories per iteration
        discMesh: Mesh for discretization of FissionSource
        """
        self.k = 1

        self.eigEstI = []   # Estimate of eigenvalue from inactive iterations
        self.meanEigI = []  # Mean of the eigenvalues from inactive iterations
        self.varEigI = []   # Variance of the eigenvalues from inactive iterations
        self.eigEst = []    # Estimate of eigenvalue from active iterations
        self.meanEig = []   # Mean of the eigenvalues from active iterations
        self.varEig = []    # Variance of the eigenvalues from active iterations
        
        self.eigVector = [] # Eigenvector estimate for active iterations

        start = time.time()
        for i in xrange(inactive):
            nextSource = self.Markov_Transport(currentSource, histories)
            self.k = self.k*(len(currentSource)/float(histories))

            self.eigEstI.append(self.k)
            self.meanEigI.append(scipy.mean(self.eigEst))    # Mean eigenvalue
            self.varEigI.append(scipy.var(self.eigEst))  # Variance eigenvalue

            print "I: %5i, eigenvalue = %8.6f," %(i, self.k),
            print " time: %8.3f sec" %(time.time() - start)

            currentSource = nextSource

        print "------------------ACTIVE ITERATIONS------------------"
        for self.i in xrange(active):
            nextSource = self.Markov_Transport(currentSource, histories)
            self.k = self.k*(len(currentSource)/float(histories))

            self.eigEst.append(self.k)
            self.meanEig.append(scipy.mean(self.eigEst))    # Mean eigenvalue
            self.varEig.append(scipy.var(self.meanEig))  # Variance eigenvalue

            print "A: %5i, eigenvalue = %8.6f," %(self.i, self.k),
            print " mean = %6.4f, std.dev = %6.4f, time: %8.3f sec" %(
                    self.meanEig[-1], math.sqrt(self.varEig[-1]),
                    (time.time() - start))

            # Discretized fissionSource
            discSource = nextSource.discretized(discMesh)
            discSource = discSource/sum(discSource)
            self.eigVector.append(discSource)

            currentSource = nextSource

