
import random
import math

import particle
import fissionBank
import fissionSource

__author__   = "$Author: jlconlin $"
__version__ = " $Revision: 29 $"
__date__     = "$Date: 2007-03-20 12:21:00 -0600 (Tue, 20 Mar 2007) $"
__id__       = "$Id: Markov.py 29 2007-03-20 18:21:00Z jlconlin $"


class Markov(object):
    """
    Markov objects will define how particles move in their Markov chains.
    Markov can be thought of as one Monte Carlo generation or cycle.  A new
    Markov object does not have to be created for each generation (although you
    could).
    """

    def __init__(self, geo, xs, histories=1000):
        """
        geo: Geometry of the simulation
        xs: Cross sections for the simulation
        histories: Number of histories to run in each cycle
        """
        self.geo = geo
        self.xs = xs
        self.histories = histories
        self.minLeakage = 0     # number of particlees leaking at x-min
        self.maxLeakage = 0     # number of particlees leaking at x-max
        self.wgtKilled = 0      # numper of particles killed due to weight
        object.__init__(self)


    def transport(self, source):
        """
        transport will take a fission source and transport it to create a
        new fission source.  transport doesn't return anything.  It is up the 
        calling object to implent a scoring function based on the particle data
        passed to it.
        """
        bank = source.sample(self.histories)
        for par in bank:
            self.trackHistory(par)
        

    def trackHistory(self, history):
        """
        trackHistory will follow a particle in its Monte Carlo trajectory.  

        history: Particle to be tracked.
        """
        weightCutoff = 0.2

        while history:
            history.move(self.xs.xT)              # Move neutron
             
            # Check if still inside geometry
            if history.position[0] > self.geo.range[0][1]:  
                # Particle leaked out right side
                self.maxLeakage += 1
                del(history)
                break
            elif history.position[0] < self.geo.range[0][0]:  
                # Particle leaked out left side
                self.minLeakage += 1
                del(history)
                break
            else:   # Collide neutron
                self.score(history)
                weight = history.weight*(self.xs.xS/self.xs.xT)
                history.collision(weight)

                # Check weight of neutron and play Russion Roulette if necessary
                if abs(history.weight) <= 0:
                    history = None
                elif abs(history.weight) < weightCutoff:
                    history = self.russianRoulette(history)


    def score(self, *args, **kwargs):
        """
        score provides the functionality for scoring tallies in a Markov 
        process.  This is meant to be overridden by subclasses.

        history: Particle to be tracked.
        bank: fissionBank where particles are added for next generation
        k: Estimate of Multiplication factor
        """
        raise NotImplementedError


    def russianRoulette(self, history, pKill=0.2):
        """
        russianRoulette will determine if a history needs to be deleted or not.
        If the history is killed, this routine will 

        history: 
        pKill: Kill probability
        """
        if random.random() < pKill:
#           print "Rouletted"
            self.wgtKilled += 1
            return None
        else:
#           print "saved"
            history.weight = history.weight/(1-pKill)
            return history



