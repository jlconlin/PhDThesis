__id__       = "$Id: kVariance.py 155 2007-08-31 20:11:54Z jlconlin $"
__author__   = "$Author: jlconlin $"
__version__ = " $Revision: 155 $"
__date__     = "$Date: 2007-08-31 14:11:54 -0600 (Fri, 31 Aug 2007) $"

"""This module will investigate the variance of the eigenvalue."""

import random
import math
import sys
import copy

import scipy
import Gnuplot

import fissionBank
import fissionSource
import Markov
import Geometry
import CrossSection
import arnoldiMC
import powerMC
import gnuplotFile

if __name__ == "__main__":
    bins = 50
    hw = 0.5
    histories = [1000, 5000, 10000]
    amciterations = 3
    amcrestarts = 20
    pmcactive = amciterations*amcrestarts
    pmcinactive = 3

    Chart = Gnuplot.Gnuplot()
    Chart.xlabel("Histories")
    Chart.ylabel("Standard Deviation")

    geo = Geometry.Geometry(bins, [[0, 2*hw]])
    xs = CrossSection.CrossSection(xS=0.5, nu=1.0, xF=0.5, xG=0)
    uSource = fissionSource.histogramSource(scipy.ones(bins), geo)

    if len(sys.argv) < 2:
        raise IndexError
    gnuData = {}
    for h in histories:
        print "\nHistories = ------------ %i\n" %h
        amc = arnoldiMC.arnoldiMC(geo, xs, h, storeVectors=True)
        Values, Vectors = amc.ERAM(uSource, amcrestarts, amciterations)

        y = scipy.array(amc.eValues)
        yMeans = [scipy.mean(y[:i,-1]) for i in xrange(1,len(y))]
        yVar = [scipy.var(y[:i,-1]) for i in xrange(1,len(y))]
        yStd = scipy.sqrt(yVar)
        yH = [(i+2)*h*amciterations for i in xrange(len(yMeans))]

        stdData = Gnuplot.Data(yH, yStd, with='linespoints',
                title='%iAM' %h)
        Chart.replot(stdData)
        gnuData['%iAM' %h] = (copy.copy(yH), copy.copy(yStd))

        pmc = powerMC.powerMC(geo, xs, pmcinactive, pmcactive, h)
        pmc.power(uSource)

        yPM = scipy.array(pmc.convergence)
        yPMMeans = [scipy.mean(yPM[:i]) for i in xrange(1,len(yPM))]
        yPMVar = [scipy.var(yPM[:i]) for i in xrange(1,len(yPM))]
        yPMStd = scipy.sqrt(yPMVar)
        yPMH = [(i+2)*h for i in xrange(len(yPMMeans))]

        stdPMData = Gnuplot.Data(yPMH, yPMStd, with='linespoints',
                title='%iPM' %h)
        Chart.replot(stdPMData)
        gnuData['%iPM' %h] = (copy.copy(yPMH), copy.copy(yPMStd))

    gnuFile = gnuplotFile.gnuplotFile(sys.argv[1], gnuData)
