__id__       = "$Id: Geometry.py 51 2007-04-25 20:43:07Z jlconlin $"
__author__   = "$Author: jlconlin $"
__version__ = " $Revision: 51 $"
__date__     = "$Date: 2007-04-25 14:43:07 -0600 (Wed, 25 Apr 2007) $"


import scipy

import Errors


class Geometry(object):
    """
    Geometry is a class to hold information about the geometry of the problem.
    """

    def __init__(self, bins, range):
        """
        bins: A tuple each number is how many spatial bins in each dimension (up
            to 3)
        range: A list of [min, max] pairs; the limits of the spatial geometry in
            each dimension.
        """
        try:
            self.dimension = len(bins)
        except TypeError:
            self.dimension = 1

        if self.dimension != 1: 
            raise Errors.GeometryError(
                "Geometry currently only suppors 1-D geometry")
        elif self.dimension != len(range):
            raise Errors.GeometryError(
                    "Bins and Range must have same degree")
        else:
            self.bins = bins
            self.range = range
            self.edges = scipy.zeros(self.bins+1)
            self.centers = scipy.zeros(self.bins)   # Bin centers
            width = self.max - self.min
            for i in xrange(self.bins+1):
                edge = self.min + i*(width/float(self.bins))
                self.edges[i] = edge
            for i in xrange(len(self.centers)):
                self.centers[i] = self.edges[i] + (self.edges[i+1] - self.edges[i])/2.0


    def __repr__(self):
        """
        """
        return "bins: %s, range: %s" %(self.bins, self.range)


    def _getMinX(self):
        return min(self.range[0])
    def _getMaxX(self):
        return max(self.range[0])

    min = property(fget=_getMinX)
    max = property(fget=_getMaxX)
