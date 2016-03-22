
__author__   = "$Author: jlconlin $"
__version__ = " $Revision: 83 $"
__date__     = "$Date: 2007-06-14 15:35:15 -0600 (Thu, 14 Jun 2007) $"
__id__       = "$Id: Transport.py 83 2007-06-14 21:35:15Z jlconlin $"

class Transport(object):
    """
    """
    def __init__(self, Throw, xs, Geometry):
        """
        Throw: Function defining how particles are transported
        xs: Cross section data
        Geometry: Geometry of problem (initially width of slab)
        """
        self.Throw = Throw
        self.xs = xs
        self.Geometry = Geometry
