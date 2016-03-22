__id__       = "$Id: test_objects.py 38 2007-04-03 16:53:35Z jlconlin $"
__author__   = "$Author: jlconlin $"
__version__ = " $Revision: 38 $"
__date__     = "$Date: 2007-04-03 10:53:35 -0600 (Tue, 03 Apr 2007) $"

import math
import time

import scipy

import Geometry
import fissionSource

def main():
    N = int(1E6)
    geo = Geometry.Geometry(10,[[-0.5,0,5]])
    uni = scipy.ones(10)
    uSource = fissionSource.histogramSource(uni, geo)

    start = time.time()
    uSource.sample(N)

    end = time.time()
    elapsedTime = end - start
    print "Elapsedtime to sample %i neutrons: %s" %(N, elapsedTime)




main()

