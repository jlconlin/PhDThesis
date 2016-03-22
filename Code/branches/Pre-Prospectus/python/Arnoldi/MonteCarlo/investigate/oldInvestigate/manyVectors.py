__id__       = "$Id: manyVectors.py 155 2007-08-31 20:11:54Z jlconlin $"
__author__   = "$Author: jlconlin $"
__version__  = "$Revision: 155 $"
__date__     = "$Date: 2007-08-31 14:11:54 -0600 (Fri, 31 Aug 2007) $"


"""This module is to simply calculate the first 5 eigenvectors using Arnoldi's
Method.  """

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

    histories  = 10000
    bins = 50
    iterations = 3
    restarts = 10
    halfwidth = 0.5

    gnuData = {}
    geo = Geometry.Geometry(bins, [[0, 2*halfwidth]])
    xs = CrossSection.CrossSection(xS=0.5, nu=1.0, xF=0.5, xG=0)
    uSource = fissionSource.histogramSource(scipy.ones(bins), geo)
    amc = arnoldiMC.arnoldiMC(geo, xs, histories, storeVectors=True)
    Values, Vectors = amc.ERAM(uSource, restarts, iterations)
    meanVector = scipy.stats.mean(amc.vectorStorage)

    vAM = abs(meanVector)/sum(abs(meanVector))
    vData = Gnuplot.Data(amc.geo.centers, vAM, with='histeps', 
            title='Arnoldi')
    v1Vect = abs(Vectors[:,-1])/sum(abs(Vectors[:,-1]))
    v1Data = Gnuplot.Data(amc.geo.centers, v1Vect, with='histeps',
            title = '1st Vector')
#   Chart.replot(vData)
    gnuData['meanV1'] = (amc.geo.centers, vAM)
     
    # Deterministic
    sn3 = procOUTSN.procOutsn("LarsenComparison/HW%.1f.OUTSN" %halfwidth)
    csHeight, csCenters = sn3.coursenSNFS(geo)
    snData = Gnuplot.Data(csCenters, csHeight, with='histeps',
            title='SN3')
#   Chart.replot(snData)
    gnuData['SN'] = (csCenters, csHeight)

    for i in xrange(3):
        gplotData = Gnuplot.Data(amc.geo.centers, Vectors[:,-(i+1)],
                with='histeps', title='Vector%i' %i)
        Chart.replot(gplotData)
        gnuData['Vector%i' %i] = (amc.geo.centers, Vectors[:,-(i+1)])

    gnuFile = gnuplotFile.gnuplotFile(sys.argv[1], gnuData)

