__id__       = "$Id: test_arnoldiMC.py 23 2007-03-15 20:32:26Z jlconlin $"
__author__   = "$Author: jlconlin $"
__version__ = " $Revision: 23 $"
__date__     = "$Date: 2007-03-15 16:32:26 -0400 (Thu, 15 Mar 2007) $"

""" """

import random
import math

import scipy
import Gnuplot

import fissionBank
import fissionSource
import Markov
import Geometry
import CrossSection
import arnoldiMC
import powerMC


def main():
    halfwidth = 0.5
    iterations = 100
    xs = CrossSection.CrossSection(xS=0.5, nu=1.0, xF=0.5, xG=0)
    print "Cross Section: %s" %xs
    Chart = Gnuplot.Gnuplot()
    title = "100 iterations max"
    Chart.title(title)
    Chart.xlabel("Number of Bins (length of vectors)")
    Chart.ylabel("Dominant eigenvalue")
    Chart("set yrange[0.35:]")

    Histories = [500, 1000, 5000, 10000]
    Zones = [5, 10, 20, 30, 50, 75, 100, 200, 500]

    for h in Histories:
        eValues = []
        for z in Zones:
            geo = Geometry.Geometry(z, [[-halfwidth, halfwidth]])
            print "geoL %s\nHistories per iteration  = %i" %(geo, h)
            uni = scipy.ones(geo.bins)
            uSource = fissionSource.histogramSource(uni, geo)
            
            mark = Markov.Markov(geo, xs, h)

            amc = arnoldiMC.arnoldiMC(mark)
            amc.arnoldi(uSource, iterations)

            eValues.append(amc.eValue)

        eData = Gnuplot.Data(Zones, eValues, with="lines", title="# histories: %i" %h)
        Chart.replot(eData)



def Arnoldo(geo, xs, histories, iterations):
    """
    """


main()
