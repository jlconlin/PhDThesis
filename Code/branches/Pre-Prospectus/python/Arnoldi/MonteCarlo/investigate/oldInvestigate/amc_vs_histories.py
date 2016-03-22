__id__       = "$Id: amc_vs_histories.py 155 2007-08-31 20:11:54Z jlconlin $"
__author__   = "$Author: jlconlin $"
__version__  = "$Revision: 155 $"
__date__     = "$Date: 2007-08-31 14:11:54 -0600 (Fri, 31 Aug 2007) $"


"""This module will investigate how the error of an eigenvector/fission Source
changes as a function of histories per cycle.  The error calculated is the root 
mean square error compared with a deterministic code. """

import sys

import Gnuplot
import scipy

import CrossSection
import Geometry
import fissionSource
import arnoldiMC
import procOUTSN
import gnuplotFile

if __name__ == "__main__":
    Chart = Gnuplot.Gnuplot()
    Chart.xlabel("iteration")
    Chart.ylabel("RMS Error")
    Chart("set logscale y")
    Chart.title("3 Restarts, 10 Iterations each")

    histories = [1000, 5000, 10000, 50000, 100000]
    bins = 50
    halfwidth = 0.5
    iterations = 3
    restarts = 10

    geo = Geometry.Geometry(bins, [[0, 2*halfwidth]])
    xs = CrossSection.CrossSection(xS=0.5, nu=1.0, xF=0.5, xG=0)
    uSource = fissionSource.histogramSource(scipy.ones(bins), geo)
     
    # Deterministic
    sn3 = procOUTSN.procOutsn(sys.argv[1])
    csHeight, csCenters = sn3.coursenSNFS(geo)

    snSource = fissionSource.histogramSource(csHeight, geo)
    gnuData = {}
    for h in histories:
        print "\nHistories %i" %h
        amc = arnoldiMC.arnoldiMC(geo, xs, h, storeVectors=True)
#       Values, Vectors = amc.arnoldi(uSource, iterations)
        Values, Vectors = amc.ERAM(uSource, restarts, iterations)
#       Values, Vectors = amc.ERAM(snSource, restarts, iterations)
        
        vAMprime = []
        for i in xrange(1, len(amc.vectorStorage)+1):
            vAMprime.append(scipy.stats.mean(amc.vectorStorage[:i]))
        vAM = [abs(v)/sum(abs(v)) for v in vAMprime]
        rAM = [sum((v-csHeight)**2)/len(v) for v in vAM]
        rAMData = Gnuplot.Data(rAM, with='linespoints', 
                title='hist/iter: %i' %(h))
        Chart.replot(rAMData)
        gnuData[h] = (range(1, len(rAM)+1), rAM)

    gnuFile = gnuplotFile.gnuplotFile(sys.argv[2], gnuData)
