__id__       = "$Id: test_arnoldiMC.py 76 2007-05-31 20:14:36Z jlconlin $"
__author__   = "$Author: jlconlin $"
__version__ = " $Revision: 76 $"
__date__     = "$Date: 2007-05-31 14:14:36 -0600 (Thu, 31 May 2007) $"

"""This file is a test for the arnoldiMC module"""

import random
import math
import sys

import scipy.linalg
import Gnuplot

import fissionBank
import fissionSource
import Markov
import Geometry
import CrossSection
import arnoldiMC
import powerMC
import gnuplotFile


def arnoldiMCGeneral(geo, xs, histories, iterations, Chart=None):
    """
    """
    uni = scipy.ones(geo.bins)
    point = scipy.zeros(geo.bins)
    point[2] = 1
    uSource = fissionSource.histogramSource(uni,geo)
    pSource = fissionSource.histogramSource(point,geo)
    uSource = uSource/scipy.linalg.norm(uSource,2)
    print "uSource: %s" %uSource

#   mark = Markov.Markov(geo,xs, histories)

    amc = arnoldiMC.arnoldiMC(geo, xs, histories)
    Values, Vectors = amc.arnoldi(uSource, iterations)

    data = {}
    n = len(Vectors[0])-1
    for i in xrange(5):
        vectData = Gnuplot.Data(Vectors[:,n-i], with='lines',
                title="vector %i" %i)
        Chart.replot(vectData)
        data['Vector%i' %i] = (amc.geo.centers,Vectors[:,n-i])
    Chart.title('Eigenvector, %i particles per iteration' %histories)
#   Chart.ylabel('Residual')
#   Chart.xlabel('Iteration')
#   Chart('set logscale y')
#   print "Histories = %s, eValue = %s" %(amc.histories, amc.eValue)

    if len(sys.argv) > 1:
        gFile = gnuplotFile.gnuplotFile(sys.argv[1], data)

    return Chart

    
if __name__ == "__main__":
#   histories = [1000, 10000, 100000]
    histories = 10000
    iterations = 11
    halfWidth = [0.5, 1.0, 5.0, 10.0, 30.0]
    c = [1.615379, 1.277102, 1.024879, 1.007135, 1.0]
    xs = CrossSection.CrossSection(xS=0.5, nu=1.0, xF=0.5, xG=0)
    print "Cross Section: %s" %xs
    Chart = Gnuplot.Gnuplot()
#   Chart.title("histories = %i" %histories)

#   xs = CrossSection.CrossSection(xS=0.0)
#   geo = Geometry.Geometry(10,[[-300.0, 300.0]])
#   print "geo: %s" %geo
#   Chart = arnoldiMCGeneral(geo, xs, histories, iterations, Chart)


    for i in xrange(len(halfWidth[:1])):
        geo = Geometry.Geometry(50,[[-halfWidth[i], halfWidth[i]]])
        print "geo: %s" %geo
        print "Histories = %i" %histories
        
        Chart = arnoldiMCGeneral(geo, xs, histories, iterations, Chart)

