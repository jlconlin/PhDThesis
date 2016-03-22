__id__       = "$Id: orthogonality.py 155 2007-08-31 20:11:54Z jlconlin $"
__author__   = "$Author: jlconlin $"
__version__ = " $Revision: 155 $"
__date__     = "$Date: 2007-08-31 14:11:54 -0600 (Fri, 31 Aug 2007) $"

"""This script will investigate how orthogonal my basis vectors really are. """

import random
import math
import time

import scipy
import Gnuplot

import fissionBank
import fissionSource
import Markov
import Geometry
import CrossSection
import arnoldiMC
import powerMC


class Ortho(object):

    def __init__(self, iterations, histories):
        self.halfwidth = 0.5
        self.iterations = iterations
        self.histories = histories
        self.xs = CrossSection.CrossSection(xS=0.5, nu=1.0, xF=0.5, xG=0.0)


    def Orthogonal(self, hw, bins):
        """
        Orthogonal will run Arnoldi's method and determine if the basis vectors 
        are orthogonal.

        hw: halfwidth of geometry
        bins: number of spatial bins
        """
        geo = Geometry.Geometry(bins, [[-hw,hw]])
        mark = Markov.Markov(geo, self.xs, self.histories)
        amc = arnoldiMC.arnoldiMC(mark)
        uSource = fissionSource.histogramSource(scipy.ones(bins))
        Values, Vectors = amc.ERAM(uSource, 5, self.iterations)
#       Values, Vectors = amc.arnoldi(uSource, self.iterations)

        n = len(amc.Q)
        O = scipy.zeros((n,n))
        for i in xrange(n):
            for j in xrange(n):
                O[i,j] = scipy.dot(amc.Q[i], amc.Q[j])
        print "Orthogonality:"
        amc.printM(O)



if __name__ == "__main__":
    bins = 100
    iterations = 15
    histories = 1000

    Chart = Gnuplot.Gnuplot()
    Chart.xlabel('Iteration')
    Chart.ylabel('Estimated dominant eigenvalue')
    Chart.title('Histories = %i' %(histories))
    Chart('set logscale x')
    Chart('set yrange [0.4:]')

    Ort = Ortho(iterations, histories)
    Ort.Orthogonal(0.5, bins)


