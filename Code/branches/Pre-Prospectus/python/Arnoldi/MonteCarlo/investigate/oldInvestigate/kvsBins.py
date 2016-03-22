__id__       = "$Id: kvsBins.py 155 2007-08-31 20:11:54Z jlconlin $"
__author__   = "$Author: jlconlin $"
__version__ = " $Revision: 155 $"
__date__     = "$Date: 2007-08-31 14:11:54 -0600 (Fri, 31 Aug 2007) $"

"""This script will investigate the convergence of the dominant eigenvalue as
the number of bins change"""

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


class kvsBins(object):

    def __init__(self, iterations, histories):
        self.halfWidth = [0.5, 1.0, 5.0, 10.0]
        self.Bins = [3, 10, 100, 200, 300, 500, 750, 1000, 2000, 5000, 10000]
        self.xs = CrossSection.CrossSection(xS=0.5, nu=1.0, xF=0.5, xG=0.0)
        self.iterations = iterations
        self.histories = histories
#       self.uSource = fissionSource.histogramSource(scipy.ones(self.bins))
        object.__init__(self)


    def dummy(self, hw, Chart, title='None'):
        """
        hw: Halfwidth of geometry
        Chart: Gnuplot object where plotting occurs.
        title: Title of plot, if None, title will be eigenvalue
        """
        valuesvsBins = []
        for bin in self.Bins:
            print "Bins = %i" %bin
            geo = Geometry.Geometry(bin, [[-hw,hw]])
            mark = Markov.Markov(geo, self.xs, self.histories)
            amc = arnoldiMC.arnoldiMC(mark)
            uSource = fissionSource.histogramSource(scipy.ones(bin))
            Values, Vectors = amc.arnoldi(uSource, self.iterations)
            valuesvsBins.append(Values[-1])

        title = 'hw = %.2f' %hw
        kData = Gnuplot.Data(self.Bins, valuesvsBins, with='lines',
                title=title)
        Chart.replot(kData)
        return Chart


    def plotter(self, data, Chart, title):
        """
        plotter will plot the convergence of the eigenvalue

        data: What to plot
        Chart: Gnuplot object
        title: title of plot
        """
        kData = Gnuplot.Data(data, with='lines', title=title)
        Chart.replot(kData)

        return Chart


if __name__ == "__main__":
    bins = 100
    iterations = 100
    histories = 1000
    in_cycles = 5
    a_cycles = iterations-in_cycles

    Chart = Gnuplot.Gnuplot()
    Chart.xlabel('Iteration')
    Chart.ylabel('Estimated dominant eigenvalue')
    Chart.title('Histories = %i' %(histories))
    Chart('set logscale x')
    Chart('set yrange [0.4:]')

    Bk = kvsBins(iterations, histories)
    Chart = Bk.dummy(Bk.halfWidth[0], Chart)


