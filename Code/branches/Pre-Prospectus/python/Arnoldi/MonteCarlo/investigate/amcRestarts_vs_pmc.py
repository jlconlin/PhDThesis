__id__       = "$Id: amcRestarts_vs_pmc.py 208 2007-12-04 02:59:34Z jlconlin $"
__author__   = "$Author: jlconlin $"
__version__ = " $Revision: 208 $"
__date__     = "$Date: 2007-12-03 19:59:34 -0700 (Mon, 03 Dec 2007) $"

"""This module will investigate how the estimate of the eigenvalue converges to
the true eigenvalue.  It compares Explicitly Restarted Arnoldi's Method to the 
Power Method."""

import time
import optparse
import copy
import sys
import os
import string

import scipy
import pylab
import Gnuplot

import arnoldiMC
import powerMC
import Geometry
import CrossSection
import fissionSource
import gnuplotFile
import procOUTSN


def Movie():
    repeat = True
    while repeat:
        try:
            os.mkdir(options.movie)
            os.chdir(options.movie)
            repeat = False
        except OSError:
            print "Unable to make movie in that directory"
            again = raw_input("Would you like to try again? [y/n]")
            if again == "y":
                options.movie = raw_input("Please enter a new folder name:")
                repeat = True
            else:
                print "Stopping this script without creating movie."
                return

    movieData = {}
    k = 0
    for i in xrange(len(amc.vectorStorage)-1):
        aY = scipy.real(scipy.mean(amc.vectorStorage[:i+1],0))
        aE = scipy.real(scipy.std(amc.vectorStorage[:i+1],0))
        movieData['Arnoldi-%i' %(i+1)] = (vectX, aY, aE)
        for j in xrange(options.I):
            pylab.clf()
            pylab.xlabel("Slab Width (mfp)")
            pylab.title("Iteration%i" %k)
            pY = scipy.real(scipy.mean(pmc.vectorStorage[:k+1],0))
            pE = scipy.real(scipy.std(pmc.vectorStorage[:k+1],0))
            movieData['Power-%i' %k] = (vectX, pY, pE)
            pylab.plot(csCenters, csHeight, 'k--', linestyle='steps', label='SN')
            pylab.plot(vectX, aY, 'r--', linestyle='steps', label='Arnoldi')
            pylab.plot(vectX, pY, 'b--', linestyle='steps', label='Power')
            pylab.legend()
            filename = "Iteration%i.png" %k
            print "Saving %s" %filename
            pylab.savefig(filename, dpi=150, orientation='landscape')
            k += 1
    gnuFile = gnuplotFile.gnuplotFile("VectorData.dat", movieData)
    os.chdir("..")


usage = 'usage: %prog [options] args'
parser = optparse.OptionParser()

parser.add_option("-f", "--file", dest="filename", type='string', 
        default=None, help="gnuplot data filename")
parser.add_option("-b", "--bins", dest="bins", type="int", default="50",
        help="Number of spatial bins.")
parser.add_option("-w", "--width", dest="width", type="float", default="20",
        help="Width of slab.")
#   parser.add_option("--inactive", dest="inactive", type="int",
#           default=5, help="Inactive Power Method cycles.")
parser.add_option("-I", "--iterations", dest="I", type="int",
        default="5", help="How many Arnoldi Iterations per Restart.")
parser.add_option("-R", "--Restarts", dest="R", type="int",
        default="20", help="How many Arnoldi Restarts.")
parser.add_option("-i", "--inactiveR", dest="inactiveR", type="int",
        default="2", help="How many inactive Arnoldi Restarts.")
parser.add_option("--histories", dest="H", type="int",
        default="1000", help="How many hisories per iteration")
parser.add_option("-s", "--source", dest="source", type="string", 
        default='uniform',
        help="""Defines source distribution.  Available sources:
                'uniform' --- uniform distrbution
                'random' --- random distribution
                'pleft' --- point source in left most bin
                'pright' --- point source in right most bin
                'pcenter' --- point source in center bin""")
parser.add_option("-r", "--run", dest="run", action="store_true", 
        default=False, help="Perform calculation.")
parser.add_option("-v", "--verbose", dest="verbose", action="store_true", 
        default=False, help="Verbosity of ArnoldiMC output.")
parser.add_option("-x", "--Relaxed", dest="relaxed", action="store_true", 
        default=False, help="Relaxed or normal Arnoldi's method.")
parser.add_option("-M", "--movie", dest="movie", type="string",
        default=None, help="Directory for convergence of eigenvector.")

options, args = parser.parse_args()

geo = Geometry.Geometry(options.bins, [[0,options.width]])
xs = CrossSection.CrossSection(xS=0.5, nu=1.0, xF=0.5, xG=0)
if options.source == 'uniform': 
    s = scipy.ones(options.bins)
elif options.source == 'random':
    s = scipy.rand(options.bins)
elif options.source == 'pleft':
    s = scipy.zeros(options.bins)
    s[0] = 1
elif options.source == 'pright':
    s = scipy.zeros(options.bins)
    s[-1] = 1
elif options.source == 'pcenter':
    mid = int(options.bins/2.0)
    s = scipy.zeros(options.bins)
    s[mid] = 1
else:
    s = eval(options.source)

try:
    source = fissionSource.histogramSource(s, geo)
except:
    raise ValueError, "Unsupported source distribution: %s" %options.source

for key in options.__dict__:
    print "%10s: %s" %(key, options.__dict__[key])

print "source %s" %source

if options.run:
    # SN3
    sn3 = procOUTSN.procOutsn("LarsenComparison/HW%.0f.0.OUTSN"
            %(options.width/2))
    csHeight, csCenters = sn3.coursenSNFS(geo)
    courseSN = Gnuplot.Data(csCenters, csHeight, title="SN3",
            with='histeps')

    if not options.filename:
        raise ValueError, "\n\nCalculation is worthless without data file."
    start = time.clock()
    Chart = Gnuplot.Gnuplot()
    Chart.xlabel("Histories")
    Chart.ylabel("Eigenvalue Estimate")
    Chart.title(string.join(sys.argv))

    amc = arnoldiMC.arnoldiMC(geo, xs, options.H, verbose=options.verbose, 
            relaxed=options.relaxed, storeVectors=True)
    Values, Vectors = amc.ERAM(source, options.R, options.inactiveR, 
            options.I)
#   amcX = [(i+1)*options.H*options.I for i in xrange(len(amc.mean))]
    amcX = [sum(amc.histList[:i]) for i in xrange(len(amc.histList))]
    amcData = Gnuplot.Data(amcX, amc.mean, amc.std, with='errorlines', 
            title='Arnoldi')

    inactive = options.inactiveR*options.I
    pmc = powerMC.powerMC(geo, xs, inactive, options.R*options.I, options.H)
    pmc.power(source)
    pmcX = [(i+1)*options.H for i in xrange(len(pmc.convergence))]
    pmcData = Gnuplot.Data(pmcX, pmc.convergence, pmc.sd, with='errorlines',
            title='Power')

    # SN3
    f = Gnuplot.Func(str(sn3.k))
    Chart.plot(f, amcData, pmcData)

    # Data File
    gnuData = {}
    gnuData['Arnoldi'] = (amcX, amc.mean, amc.std)
    gnuData['Power'] = (pmcX, pmc.convergence, pmc.sd)
    vectX = csCenters

    gnuData['Arnoldi-vect'] = (vectX, 
            scipy.real(scipy.mean(amc.vectorStorage,0)),
            scipy.real(scipy.std(amc.vectorStorage,0)))
    gnuData['Power-vect'] = (vectX, scipy.mean(pmc.vectorStorage,0),
            scipy.std(pmc.vectorStorage,0))
    gnuData['SN3-vect'] = (csCenters, csHeight)
    gnuFile = gnuplotFile.gnuplotFile(options.filename, gnuData)

    newChart = Gnuplot.Gnuplot()
    newChart.title(string.join(sys.argv))
    amcvecData = Gnuplot.Data(vectX, scipy.mean(amc.vectorStorage,0),
            with='histeps', title='Arnoldi')
    amcvecEData = Gnuplot.Data(vectX, scipy.mean(amc.vectorStorage, 0), 
            scipy.std(amc.vectorStorage, 0), with='errorbars')
    pmcvecData = Gnuplot.Data(vectX, scipy.mean(pmc.vectorStorage,0),
            with='histeps', title='Power')
    newChart.plot(courseSN, pmcvecData, amcvecData,)

    # Make movie and save vector data
    if options.movie:
        Movie()


