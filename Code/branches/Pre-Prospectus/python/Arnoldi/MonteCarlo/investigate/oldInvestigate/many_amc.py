__id__       = "$Id: many_amc.py 155 2007-08-31 20:11:54Z jlconlin $"
__author__   = "$Author: jlconlin $"
__version__ = " $Revision: 155 $"
__date__     = "$Date: 2007-08-31 14:11:54 -0600 (Fri, 31 Aug 2007) $"

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
    histories = 1000
    iterations = 50
    trials = 50
    halfWidth = 0.5
    xs = CrossSection.CrossSection(xS=0.5, nu=1.0, xF=0.5, xG=0)
    print "Cross Section: %s" %xs
    geo = Geometry.Geometry(100, [[-halfWidth, halfWidth]])
    mark = Markov.Markov(geo,xs, histories)
    uni = scipy.ones(geo.bins)
    uSource = fissionSource.histogramSource(uni, geo)

    Chart = Gnuplot.Gnuplot()
    Chart.xlabel('Iteration')
    Chart.ylabel('Eigenvalue estimate')
    Chart.title('histories per Arnoldi iteration: %i' %histories)

    for j in xrange(5):
        eValues = []
        for i in xrange(trials):
            amc = arnoldiMC.arnoldiMC(mark)
            amc.arnoldi(uSource, iterations)
            print "eValue = %s" %amc.eValue

            eValues.append(amc.eValue)
     
        average_eValue = sum(eValues)/trials
        # Calculate Variance
        var_eValue = 0
        for value in eValues:
            var_eValue += math.pow((value - average_eValue),2)
        var_eValue = var_eValue/(trials - 1)

        eData = Gnuplot.Data(eValues, with='lines', 
                title="%6.4f, %6.4f" %(average_eValue, math.sqrt(var_eValue)))
        Chart.replot(eData)


main()
