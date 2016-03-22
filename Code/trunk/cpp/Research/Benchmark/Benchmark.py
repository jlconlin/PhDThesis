__id__       = "$Id: Benchmark.py 560 2009-05-27 15:21:00Z jlconlin $"
__author__   = "$Author: jlconlin $"
__version__ = " $Revision: 560 $"
__date__     = "$Date: 2009-05-27 09:21:00 -0600 (Wed, 27 May 2009) $"


"""
This file contains some utilities designed to help with calculating benchmark
quantities.  For example converting from c to k eigenvalues.
"""

import sys
import string

def kToc(k, nu = 5.0, xF = 0.2, xS = 0.8, xG = 0.0 ):
    """
    kToc will convert a k-eigenvalue to a c-eigenvalue given the cross sections
    for the problem.
    """
    xA = xG + xF
    xT = xA + xS
    
    return (nu*xF/k + xS)/xT

def cTok(c, nu = 5.0, xF = 0.2, xS = 0.8, xG = 0.0 ):
    """
    cTok will convert a c-eigenvalue to a k-eigenvalue given the cross sections
    for the problem.
    """
    xA = xG + xF
    xT = xA + xS
    return nu*xF/(xT*c - xS)


if __name__ == "__main__":
    if string.lower(sys.argv[1]) == 'c':
        # Convert c to k
        c = float(sys.argv[2])
        print "c-eigenvalue: %9.6f, k-eigenvalue: %9.6f" %(c, cTok(c))
    elif string.lower(sys.argv[1]) == 'k':
        # Convert k to c
        k = float(sys.argv[2])
        print "k-eigenvalue: %9.6f, c-eigenvalue: %9.6f" %(k, kToc(k))
    else:
        print "I don't know what to do."
