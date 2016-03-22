__id__       = "$Id: mc_vs_SN3.py 159 2007-09-14 14:07:06Z jlconlin $"
__author__   = "$Author: jlconlin $"
__version__  = "$Revision: 159 $"
__date__     = "$Date: 2007-09-14 08:07:06 -0600 (Fri, 14 Sep 2007) $"


""" This module will compare the Monte Carlo Arnoldi's method to the
deterministic SN3 code."""

import sys
import bisect

import scipy.linalg
import scipy.stats
import Gnuplot

import CrossSection
import Geometry
import fissionSource
import arnoldiMC
import powerMC
import procOUTSN
import gnuplotFile


if __name__ == "__main__":
    VectorChart = Gnuplot.Gnuplot()
#   VectorChart("set yrange [:1.1]")

    amchistories = 1000
    pmchistories = 1000
    amciterations = 5
    amcrestarts = 40
    pmciterations = 50
    bins = 10
    halfwidth = 10

    geo = Geometry.Geometry(bins, [[0, 2*halfwidth]])
    xs = CrossSection.CrossSection(xS=0.5, nu=1.0, xF=0.5, xG=0)
    uSource = fissionSource.histogramSource(scipy.ones(bins), geo)

    # Arnoldi's Method
    amc = arnoldiMC.arnoldiMC(geo, xs, amchistories, storeVectors=True,
            verbose=True)
#   Values, Vectors = amc.arnoldi(uSource, amciterations)
    Values, Vectors = amc.ERAM(uSource, amcrestarts, amciterations)

    # Power Method
    inactive = 5
    pmc = powerMC.powerMC(geo, xs, inactive, pmciterations, pmchistories)
    pmc.power(uSource)

    # Deterministic
    sn3 = procOUTSN.procOutsn(sys.argv[1])
    csHeight, csCenters = sn3.coursenSNFS(amc.geo)

    # Plot
#   courseSN = Gnuplot.Data(csCenters, csHeight, title="courseSN",
#           with='histeps')
#   VectorChart.replot(courseSN)

    amcplotvalues = scipy.mean(amc.vectorStorage,0)
    amcData = Gnuplot.Data(amc.geo.centers, amcplotvalues, title="AMC",
            with='histeps')

    pmcVector = scipy.stats.mean(pmc.vectorStorage)
    pmcplotvalues = abs(pmcVector)/sum(abs(pmcVector))
    pmcData = Gnuplot.Data(pmc.geo.centers, pmcplotvalues, title='PMC',
            with='histeps')

    VectorChart.replot(amcData, pmcData)

    rChart = Gnuplot.Gnuplot()
    rChart.xlabel("Histories")
    rChart.ylabel("RMS Error")
    rChart("set logscale y")
    rChart('set yrange [1E-7:1E-3]')
    
    data = {}

    vAMprime = []
    for i in xrange(1, len(amc.vectorStorage)+1):
        vAMprime.append(scipy.stats.mean(amc.vectorStorage[:i]))
#   vAM = [abs(v)/sum(abs(v)) for v in amc.vectorStorage]
    vAM = [abs(v)/sum(abs(v)) for v in vAMprime]
    rAM = [sum((v-csHeight)**2)/len(v) for v in vAM]
    hAM = [(i+1)*amchistories*amciterations for i in xrange(len(rAM))]
    rAMData = Gnuplot.Data(hAM, rAM, with='linespoints', 
            title='AM %iH, %iI' %(amchistories, amciterations))
    data['Arnoldi'] = (hAM, rAM)

    vPMprime = []
    for i in xrange(1, len(pmc.vectorStorage)+1):
        vPMprime.append(scipy.stats.mean(pmc.vectorStorage[:i]))
    vPM = [abs(v)/sum(abs(v)) for v in vPMprime]
    rPM = [sum((v-csHeight)**2)/len(v) for v in vPM]
    hPM = [(i+1)*pmchistories for i in xrange(pmciterations)]
    rPMData = Gnuplot.Data(hPM, rPM, with='linespoints',
            title='PM %iH, %iI' %(pmchistories, pmciterations))
    data['Power'] = (hPM, rPM)

    rChart.plot(rAMData, rPMData)

    if len(sys.argv) > 2:
        gnuFile = gnuplotFile.gnuplotFile(sys.argv[2], data)
