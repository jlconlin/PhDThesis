__id__       = "$Id: Errors.py 92 2007-07-12 15:25:16Z jlconlin $"
__author__   = "$Author: jlconlin $"
__version__ = " $Revision: 92 $"
__date__     = "$Date: 2007-07-12 09:25:16 -0600 (Thu, 12 Jul 2007) $"

"""Custom Exceptions for my code."""

class Error(Exception):
    """Base class for exceptions in the module."""

    def __init__(self, message):
        """
        """
        self.message = message


    def __str__(self):
        return repr(self.message)


class GeometryError(Error):
    """
    Exception raised for errors in the Geometry.
    """

    def __init__(self, message):
        Error.__init__(self, message)
    

class NoiseShapeError(Error):
    """
    Exception raised when NoiseShape unknown.
    """
    def __init__(self, message):
        Error.__init__(self, message)
