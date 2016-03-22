__id__       = "$Id: pmc_vs_histories.py 67 2007-05-09 15:03:43Z jlconlin $"
__author__   = "$Author: jlconlin $"
__version__  = "$Revision: 67 $"
__date__     = "$Date: 2007-05-09 09:03:43 -0600 (Wed, 09 May 2007) $"


"""This module will investigate how the error of an eigenvector/fission Source
changes as a function of histories per cycle.  The error calculated is the root 
mean square error compared with a deterministic code. """

import sys

import Gnuplot
import scipy

import CrossSection
import Geometry
import fissionSource
import powerMC
import procOUTSN
import gnuplotFile

if __name__ == "__main__":
    Chart = Gnuplot.Gnuplot()
    Chart.xlabel("iteration")
    Chart.ylabel("RMS Error")
    Chart("set logscale y")
    Chart.title("Point Source")

    histories = [1000, 5000, 10000, 50000]
    bins = 50
    halfwidth = 0.5
    inactive = 20
    active = 50

    geo = Geometry.Geometry(bins, [[0, 2*halfwidth]])
    xs = CrossSection.CrossSection(xS=0.5, nu=1.0, xF=0.5, xG=0)
    uSource = fissionSource.histogramSource(scipy.ones(bins), geo)
    pSource = fissionSource.histogramSource(scipy.zeros(bins), geo)
    pSource[bins/2] = 1

    # Deterministic
    sn3 = procOUTSN.procOutsn(sys.argv[1])
    csHeight, csCenters = sn3.coursenSNFS(geo)
#   courseSN = Gnuplot.Data(csCenters, csHeight, title="courseSN",
#           with='histeps')
#   Chart.replot(courseSN)

    gnuData = {}
    for h in histories:
        print "\nHistories: %i" %h
        pmc = powerMC.powerMC(geo, xs, inactive, active, h)
#       pmc.power(uSource)
        pmc.power(pSource)

        vprime = []
        for i in xrange(1, len(pmc.vectorStorage)+1):
            vprime.append(scipy.stats.mean(pmc.vectorStorage[:i]))
        vPM = [abs(v)/sum(abs(v)) for v in vprime]
        rPM = [sum((v-csHeight)**2)/len(v) for v in vPM]
        hPM = [(i+1)*h for i in xrange(active)]
        rPMData = Gnuplot.Data(rPM, with='linespoints',
                title='his/iter: %i' %(h))
        Chart.replot(rPMData)
        gnuData[h] = (range(1, len(rPM)+1), rPM)

    gnuFile = gnuplotFile.gnuplotFile(sys.argv[2], gnuData)



