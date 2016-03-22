__id__       = "$Id: amcNoRestartConvergence.py 168 2007-10-25 19:25:39Z jlconlin $"
__author__   = "$Author: jlconlin $"
__version__ = " $Revision: 168 $"
__date__     = "$Date: 2007-10-25 13:25:39 -0600 (Thu, 25 Oct 2007) $"

"""This module will investigate how the eigenvector converges without restarts. """

import optparse
import os
import sys
import string

import scipy
import pylab

import arnoldiMC
import Geometry
import CrossSection
import fissionSource
import gnuplotFile
import procOUTSN



def Movie():
    """
    Movie will generate images for making a movie from the convergent eigenvectors
    """
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
    gnuData = {}
    evalueErrors = [scipy.std(amc.convergence[:i+1]) for i in
        xrange(len(amc.convergence))]
    gnuData['Eigenvalue'] = (scipy.arange(len(amc.convergence)), 
        amc.convergence, evalueErrors)

    k = [sn3.k for i in xrange(len(amc.convergence))]
    pylab.clf()
    pylab.xlabel("Iteration")
    pylab.ylabel("Eigenvalue estimate")
    pylab.title("%s" %(string.join(sys.argv)), size=10)
    pylab.errorbar(range(len(amc.convergence)), amc.convergence, evalueErrors)
    pylab.plot(k, 'k-')
    pylab.savefig('../%s.png' %options.filename)

    for vec, i in zip(amc.iterVectors, xrange(len(amc.iterVectors))):
        pylab.clf()
        pylab.xlabel("Slab Width (mfp)")
        pylab.plot(geo.edges[:-1], csHeight, 'k--', linestyle='steps')
        pylab.plot(geo.edges[:-1], vec, 'r--', linestyle='steps')
        pylab.title("Iteration %i" %i)
        fname = "Iteration%i.png" %i
        print "Saving %s" %fname
        pylab.savefig(fname, dpi=150, orientation='landscape')
        gnuData['Iter-%i' %i] = (geo.edges[:-1], vec)

    os.chdir("..")
    g = gnuplotFile.gnuplotFile(options.filename, gnuData)




usage = 'usage: %prog [options] args'
parser = optparse.OptionParser()

parser.add_option("-f", "--file", dest="filename", type='string', 
        default=None, help="gnuplot data filename")
parser.add_option("-b", "--bins", dest="bins", type="int", default="50",
        help="Number of spatial bins.")
parser.add_option("-w", "--width", dest="width", type="float", default="20",
        help="Width of slab.")
parser.add_option("-I", "--iterations", dest="I", type="int",
        default="53", help="How many Arnoldi Iterations.")
parser.add_option("--histories", dest="H", type="int",
        default="1000", help="How many histories per iteration")
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
parser.add_option("-M", "--movie", dest="movie", type="string",
        default=None, help="Directory for convergence of eigenvector.")

options, args = parser.parse_args()

geo = Geometry.Geometry(options.bins, [[0,options.width]])
xs = CrossSection.CrossSection(xS=0.5, nu=1.0, xF=0.5, xG=0)
if options.source == 'uniform': 
    s = scipy.ones(options.bins)
elif options.source == 'random':
    s = scipy.rand(options.bin)
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
    sn3 = procOUTSN.procOutsn("LarsenComparison/HW%.0f.0.OUTSN" %(options.width/2))
    csHeight, csCenters = sn3.coursenSNFS(geo)

    amc = arnoldiMC.arnoldiMC(geo, xs, options.H, verbose=options.verbose, 
            storeVectors=True)
    Values, Vectors = amc.arnoldi(source, options.I)

    Movie()
