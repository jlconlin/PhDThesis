__id__       = "$Id: fissionSource.py 59 2007-04-27 16:17:29Z jlconlin $"
__author__   = "$Author: jlconlin $"
__version__ = " $Revision: 59 $"
__date__     = "$Date: 2007-04-27 10:17:29 -0600 (Fri, 27 Apr 2007) $"

import math
import copy
import random
import bisect

import scipy
import scipy.linalg
import numpy

import fissionBank
import particle
import Alias



class fissionSource(scipy.ndarray):
    """
    fissionSource differs from fissionBank in that it doesn't actually contain
    particles.  The elements of a fissionSource are the expansion coefficients
    of an FET.  One can think of a fissionSource object as a PDF that fission
    neutrons can be sampled from.  This class is meant to be inherited.
    """

    def sample(self):
        """
        sample is used to generate a fissionBank object containing neutrons
        which will be used in a Monte Carlo generation
        """
        raise NotImplementedError


class histogramSource(fissionSource):
    """
    historgramSource is a fissionSource object whose basis functions are step
    functions defining a histogram.
    """

    def __init__(self, source, geo=None):
        if isinstance(source, fissionBank.fissionBank):
            self.histogram(source)      # bin data from fissionBank object

    def __new__(self, source, geo=None):
        """
        source: A source, either a fissionBank object or a scipy.ndarray object
        geo: Geometry object optional if source is already a fissionSource
            object
        """
        fissionSource.__init__(self)

        pdf = source
        # Set object-dependent initialization
        if isinstance(source, fissionBank.fissionBank):
            pdf = scipy.zeros(self.geo.bins)

        self.geo = copy.deepcopy(geo)
        return pdf.view(self)


    def _get_pSource(self):
        """
        """
        prob = 0
        for bin in self:
            if bin > 0:
                prob += bin

        return prob


    def _get_nSource(self):
        """
        """
        prob = 0
        for bin in self:
            if bin < 0:
                prob += abs(bin)

        return prob


    posSource = property(fget=_get_pSource)
    negSource = property(fget=_get_nSource)


    def histogram(self, bank):
        """
        histogram will populate the histogram source from a fissionBank.  The
        pdf will be normalized so the total area is 1.  The pdf is returned.

        bank: fissionBank object which needs to be binned
        """
        binwidth = (self.geo.max - self.geo.min)/float(self.geo.bins)
        min = self.geo.min
        
        for par in bank:
            binNum = math.ceil((par.position[0] - min)/binwidth)
            if binNum >= self.geo.bins:
                binNum = self.geo.bins
            if par.weight > 0:
                self[binNum-1] = self[binNum-1] + 1
            else:
                self[binNum-1] = self[binNum-1] - 1


    def sample(self, histories):
        """
        sample will return a fissionBank object where the neutrons in the bank
        are sampled from self.

        histories: The number of neutron desired in the fissionBank
        """
        n = particle.neutron()
        pdf = self/sum(abs(self))

        fb = fissionBank.fissionBank()
        for j in Alias.gen_discrete(abs(pdf), histories):
            x = random.uniform(self.geo.edges[j], self.geo.edges[j+1])

            if pdf[j] < 0:
                nWeight = -1
            else:
                nWeight = 1
            n.weight = nWeight
            n.position = (x, 0, 0)
            n.setRandDirection()
            fb.append(n)

        return fb


    def score(self, weight, position):
        """
        score will add weight to the appropriate bin defined by position.  This
        function can be used to score on the fly rather than binning a
        fissionbank object.
        """
        binwidth = (self.geo.max - self.geo.min)/float(self.geo.bins)
        min = self.geo.min
        binNum = math.ceil((position[0] - min)/binwidth)

        self[binNum-1] += weight

