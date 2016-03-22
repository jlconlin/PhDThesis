

__author__   = "$Author: jlconlin $"
__version__ = " $Revision: 22 $"
__date__     = "$Date: 2007-03-15 10:27:58 -0600 (Thu, 15 Mar 2007) $"
__id__       = "$Id: CrossSection.py 22 2007-03-15 16:27:58Z jlconlin $"


class CrossSection(object): 
    """ 
    CrossSection is a container object to hold information about cross sections. 
    """ 

    def __init__(self, xS=0.5, xF=2.0, xG=1.0, nu=2.5, debug=0): 
        """ 
        """ 
        self.xS = xS 
        self.xF = xF 
        self.nu = nu 
        self.xG = xG 

    def __repr__(self):
        return "xG: %.4f, xF: %.4f, nu: %.2f, xA: %.4f, xS: %.4f, xT: %.4f" %(
                self.xG, self.xF, self.nu, self.xA, self.xS, self.xT)


    def _get_xA(self):
        return self.xG + self.xF

    def _set_xA(self, dummy):
        raise AttributeError("%s.xT is read-only" %self.__class__.__name__)

    xA = property(fset=_set_xA, fget=_get_xA)

    def _get_xT(self):
        return self.xA + self.xS

    def _set_xT(self, dummy):
        raise AttributeError("%s.xT is read-only" %self.__class__.__name__)

    xT = property(fset=_set_xT, fget=_get_xT)


    def __setitem__(self, key, value):
        setattr(self, key, float(value))

