__id__       = "$Id: amc_vs_bins.py 155 2007-08-31 20:11:54Z jlconlin $"
__author__   = "$Author: jlconlin $"
__version__  = "$Revision: 155 $"
__date__     = "$Date: 2007-08-31 14:11:54 -0600 (Fri, 31 Aug 2007) $"


"""This module will investiage how the error of an eigenvector/fission source
changes as a function of the number of discretized bins.  The error calculated
is the root mean square error compared with a deterministic code. """

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
    Chart.xlabel("bins")
    Chart.ylabel("RMS Error")
    Chart("set logscale y")
    
    histories = [1000, 5000, 10000]
    bins = [3, 5, 10, 20, 35, 50, 100]
    iterations = 3
    restarts = 10
    halfwidth = 10.0


    gnuData = {}
    for h in histories:
        print "\nHistories %i" %h
        Error = []
        for b in bins:
            print "\nBins %i" %b
            geo = Geometry.Geometry(b, [[0, 2*halfwidth]])
            xs = CrossSection.CrossSection(xS=0.5, nu=1.0, xF=0.5, xG=0)
            uSource = fissionSource.histogramSource(scipy.ones(b), geo)
            amc = arnoldiMC.arnoldiMC(geo, xs, h, storeVectors=True)
            Values, Vectors = amc.ERAM(uSource, restarts, iterations)
     
            # Deterministic
            sn3 = procOUTSN.procOutsn("LarsenComparison/HW%.1f.OUTSN" %halfwidth)
            csHeight, csCenters = sn3.coursenSNFS(geo)

            meanVector = scipy.stats.mean(amc.vectorStorage)
            vAM = abs(meanVector)/sum(abs(meanVector))
            rAM = sum((vAM-csHeight)**2)/len(vAM)
            Error.append(rAM)
        ErrorData = Gnuplot.Data(bins, Error, with='linespoints', 
                title='hist: %i' %h)
        Chart.replot(ErrorData)
        gnuData[h] = (bins, Error)

    gnuFile = gnuplotFile.gnuplotFile(sys.argv[1], gnuData)

            
