__id__       = "$Id: test_fissionSource.py 59 2007-04-27 16:17:29Z jlconlin $"
__author__   = "$Author: jlconlin $"
__version__ = " $Revision: 59 $"
__date__     = "$Date: 2007-04-27 10:17:29 -0600 (Fri, 27 Apr 2007) $"

""" This file is a tester for fissionSource.py to see if the histogramSource
object will bin the fissionBank correctly."""

import random
import math

import scipy.linalg
import Gnuplot

import particle
import fissionBank
import fissionSource
import Geometry


def main():
    sampleTest()

def sampleTest():
    geo = Geometry.Geometry(10,[[0,1]])
    histories = 100000

    HSX = []
    HSErrors = []
    source = scipy.ones(10)
    source[3:7] = source[3:7]*-1
    source[2] = 3
    source[5] = -3
    HS = fissionSource.histogramSource(source, geo)
    HS = HS/scipy.linalg.norm(HS,2)
    for i in xrange(len(HS)):
        HSX.append(i)
        HSErrors.append(math.sqrt(abs(HS[i]))/math.sqrt(histories/geo.bins))
    F1 = HS.sample(histories)
    F2 = HS.sample(histories)
    F3 = HS.sample(histories)
    F4 = HS.sample(histories)
    F5 = HS.sample(histories)
    H1 = fissionSource.histogramSource(F1, geo)
    H2 = fissionSource.histogramSource(F2, geo)
    H3 = fissionSource.histogramSource(F3, geo)
    H4 = fissionSource.histogramSource(F4, geo)
    H5 = fissionSource.histogramSource(F5, geo)
    H1 = H1/scipy.linalg.norm(H1,2)
    H2 = H2/scipy.linalg.norm(H2,2)
    H3 = H3/scipy.linalg.norm(H3,2)
    H4 = H4/scipy.linalg.norm(H4,2)
    H5 = H5/scipy.linalg.norm(H5,2)

    Chart = prinths(HS,HSX)
    printsample(H1, Chart)
    printsample(H2, Chart)
    printsample(H3, Chart)
    printsample(H4, Chart)
    printsample(H5, Chart)
    printErrors(HS, HSX, HSErrors, Chart)
    

def prinths(hs,x):
    chart = Gnuplot.Gnuplot(debug=0)
    chart('set style fill solid')
#   chart('set yrange [0.08:0.125]')
#   chart('set ytics scale 0.01 0.005')
    chart('set grid')
    hsData = Gnuplot.Data( hs, with='histogram', title='Source')
    chart.plot(hsData)
    return chart

def printsample(h, chart):
    """
    """
    bankData = Gnuplot.Data(h, with='histograms')
    
    chart.replot(bankData)

    return chart


def printErrors(hs, x, errors, chart):
    errorData = Gnuplot.Data(x,hs,errors, with='errorbars linestyle -1')
    chart.replot(errorData)

    return chart

main()
