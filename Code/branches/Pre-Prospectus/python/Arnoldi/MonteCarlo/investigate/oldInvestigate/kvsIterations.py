__id__       = "$Id: kvsIterations.py 156 2007-08-31 20:48:45Z jlconlin $"
__author__   = "$Author: jlconlin $"
__version__ = " $Revision: 156 $"
__date__     = "$Date: 2007-08-31 14:48:45 -0600 (Fri, 31 Aug 2007) $"

"""This module will investigate the effect on the eigenvalue estimate for various
particles thrown per iteration."""

import time
import optparse

import scipy

import arnoldiMC
import Geometry
import CrossSection
import fissionSource
import gnuplotFile


class kI(optparse.OptionParser):

    def __init__(self, usage=None, version=None):
        """
        __init__ will create all the necessary information you need to run the code.
        It parses the command line to understand all your options.
        """
        if not usage:
            usage = 'usage: %prog [options] args'
        optparse.OptionParser.__init__(self, usage=usage, version=version)
        self.add_option("-f", "--file", dest="filename", type='string', 
                default=None, help="gnuplot data filename")
        self.add_option("-b", "--bins", dest="bins", type="int", default="50",
                help="Number of spatial bins.")
        self.add_option("-w", "--width", dest="width", type="float", default="20",
                help="Width of slab.")
        self.add_option("-I", "--iterations", dest="I", type="string",
                default='[3, 5, 7, 10, 15, 20]',
                help="How many Arnoldi Iterations per trial.")
        self.add_option("-T", "--trials", dest="Trials", type="float", default=100,
                help="How many independent trials.")
        self.add_option("-r", "--run", dest="run", action="store_true", default=False,
                help="Perform calculation.")
        self.add_option("-s", "--source", dest="source", type="string", 
                default='uniform',
                help="""Defines source distribution.  Available sources:
    'uniform' --- uniform distrbution
    'pleft' --- point source in left most bin
    'pright' --- point source in right most bin
    'pcenter' --- point source in center bin""")

        self.options, self.args = self.parse_args()

        self.geo = Geometry.Geometry(self.options.bins, [[0,self.options.width]])
        self.xs = CrossSection.CrossSection(xS=0.5, nu=1.0, xF=0.5, xG=0)
        if self.options.source == 'uniform': 
            s = scipy.ones(self.options.bins)
        elif self.options.source == 'pleft':
            s = scipy.zeros(self.options.bins)
            s[0] = 1
        elif self.options.source == 'pright':
            s = scipy.zeros(self.options.bins)
            s[-1] = 1
        elif self.options.source == 'pcenter':
            mid = int(self.options.bins/2.0)
            s = scipy.zeros(self.options.bins)
            s[mid] = 1
        else:
            s = eval(self.options.source)

        try:
            self.source = fissionSource.histogramSource(s, self.geo)
        except:
            raise ValueError, "Unsupported source distribution: %s" %self.options.source

        self.options.I = eval(self.options.I)


    def printParameters(self):
        """
        printParameters will print the necessary information about the job to be
        submitted.
        """
        print "Source Distribution: %s" %self.source
        print "Bins: %i, Width: %5.3f" %(self.options.bins, self.options.width)
        print "Trials: %.2G, Iterations: %s" %(self.options.Trials, self.options.I)
        print "Filename: %s" %(self.options.filename)


    def run(self, h):
        """
        run will start the calculation.  It returns the expected value of the 
        estimates of the eigenvalues and their standard deviation.

        h: The number of histories per iteration
        """
        if not self.options.filename:
            raise ValueError, "\n\nCalculation is worthless without data file."
        
        start = time.clock()
        eigenvalues = []
        sd = []
        print "\tIterations: "
        for I in self.options.I:
            print "\t\t%i... " %I
            estimates = scipy.zeros(self.options.Trials)
            for t in xrange(int(self.options.Trials)):
                amc = arnoldiMC.arnoldiMC(self.geo, self.xs, histories=h)
                Values, Vectors = amc.arnoldi(self.source, I)
                estimates[t] = Values[-1]
            eigenvalues.append(estimates.mean())
            sd.append(estimates.std())
        print "\telapsed time = %.3f seconds" %(time.clock()-start)
        return (eigenvalues, sd)


if __name__ =="__main__":
    usage = 'usage: %prog [options] args'
    runner = kI(usage=usage, version=str(__version__))
    runner.printParameters()

    if runner.options.run:
        histories = [500, 1000, 5000, 10000]
        gnuData = {}
        for h in histories:
            print "\nHistories = ----------------- %i" %h 
            ev, sd = runner.run(h)
            gnuData['part=%i' %h] = (runner.options.I, ev, sd)
        gnuFile = gnuplotFile.gnuplotFile(runner.options.filename, gnuData)
