__id__       = "$Id: powerMC.py 163 2007-10-05 12:35:38Z jlconlin $"
__author__   = "$Author: jlconlin $"
__version__ = " $Revision: 163 $"
__date__     = "$Date: 2007-10-05 06:35:38 -0600 (Fri, 05 Oct 2007) $"

import random
import math
import time

import Gnuplot
import scipy.stats

import Markov
import fissionBank
import fissionSource


class powerMC(Markov.Markov):
    """
    powerMC performs a simple Monte Carlo Power Method to find the dominant
    eigenvalue.
    """

    def __init__(self, geo, xs, inactive, active, histories=1000):
        """
        geo: Geometry of the simulation
        xs: Cross sections for the simulation
        inactive: Number of Monte Carlo generations to skip
        active: Number of active Monte Carlo generations
        histories: Number of histories to run in each cycle
        storeVectors: Whether the dominant eigenvector should be stored between
            iterations
        """
        self.active = active
        self.inactive = inactive

        Markov.Markov.__init__(self, geo, xs, histories)


    def power(self, source):
        """
        power is the main method for this algorithm

        source: Initial guess of fission source
        """
        # Initialize
        self.k = 1
        self.cycle_k = []         # list of eigenvalues per iteration
        self.convergence = []     
        self.sd = []              # list of standard deviaiton per iterations
        self.k_inactive = []
        self.vectorStorage = []

        self.source = source
        start = time.time()
        elapsed = 0
        totaltime = 0

        for i in xrange(1, self.inactive+1):
            self.nextBank = fissionBank.fissionBank()
            self.transport(self.source)
            self.k = self.k*len(self.nextBank)/float(self.histories)
            self.k_inactive.append(self.k)
            totaltime = time.time()-start
            print "iteration: %5i, eigenvalue = %8.6f," %(i, self.k),
            print " time: %8.3f sec" %(totaltime)
            self.source = self.nextBank

        print "------- Starting active cycles -------"
        for self.i in xrange(1, self.active+1):
            self.nextBank = fissionBank.fissionBank()
            self.transport(self.source)
            self.k = (self.k*len(self.nextBank)/float(self.histories))
            self.cycle_k.append(self.k)
            self.convergence.append(scipy.mean(self.cycle_k))
            self.sd.append((1/math.sqrt(self.i))*scipy.std(self.cycle_k))
            totaltime = time.time()-start
            print "iteration: %5i, eigenvalue = %8.6f," %(self.i, self.k),
            print " std.dev = %6.4f, time: %8.3f sec" %(
                    scipy.std(self.convergence), totaltime)
            self.source = self.nextBank
            Y = fissionSource.histogramSource(self.source,self.geo)
            Y = Y/sum(Y)
            self.vectorStorage.append(Y)


    def _estVar(self):
        """
        """
        if self.i > 1:
            self.vark = scipy.stats.var(self.convergence)
        else:
            self.vark = 0.0


    def score(self, history):
        """
        score provides the functionality for scoring tallies in a Markov 
        process.  This is meant to be overridden by subclasses.

        history: Particle to be tracked.
        bank: fissionBank where particles are added for next generation
        k: Estimate of Multiplication factor
        """
        ran = random.random()
        N = math.floor(history.weight*((1.0/self.k)*(self.xs.nu*self.xs.xF/self.xs.xT)) + ran)
        self.nextBank.append(history, N)     # Contribute to fission source
