__id__       = "$Id: test_powerMC.py 93 2007-07-12 19:25:50Z jlconlin $"
__author__   = "$Author: jlconlin $"
__version__ = " $Revision: 93 $"
__date__     = "$Date: 2007-07-12 13:25:50 -0600 (Thu, 12 Jul 2007) $"

import math

import scipy
import Gnuplot

import Markov
import fissionSource
import powerMC
import Geometry
import CrossSection
import particle
import fissionBank



def main():
    xs = CrossSection.CrossSection(xS=0.5, nu=1.0, xF=0.5, xG=0)
    halfWidth = [0.5, 1.0, 5.0, 10.0, 30.0]
#   xs = CrossSection.CrossSection(xS=0)
#   halfWidth=[300]
    c = [1.615379, 1.277102, 1.024879, 1.007135, 1.0]
    histories = 1000
    in_cycles = 5
    a_cycles = 50

#   n = particle.neutron()
#   bank = fissionBank.fissionBank()
#   bank.append(n,histories)
#   for part in bank:
#       part.setRandDirection()

    chart = Gnuplot.Gnuplot()
    chart.xlabel("Cycles")
    chart.ylabel("k-effective")
    chart.title("Histories = %i, Inactive Cycles = %i, Active Cycles = %i" 
            %(histories, in_cycles, a_cycles))
    chart("set key right bottom")
    chart("set yrange [0:*]")

    for i in xrange(len(halfWidth)):
        geo = Geometry.Geometry(10,[[-halfWidth[i], halfWidth[i]]])
        print "xs: %s" %xs
        print "geo: %s" %geo
        print "Histories = %i" %histories

        source = makeFBank(histories)
        source = makeFSource(geo)

        pmc = powerMC.powerMC(geo, xs, in_cycles, a_cycles, histories)
        pmc.power(source)

        kbench = 1/(2*c[i] - 1)
        print "halfWidth: %s, k-benchmark: %8.6f" %(halfWidth[i], kbench)
        print "k-estimate = %8.6f, std.dev-est = %8.6E" %(pmc.k, math.sqrt(pmc.vark))
        print "left-leakage: %i, right-leakage: %i, weight-killed: %i" %(
                pmc.minLeakage, pmc.maxLeakage, pmc.wgtKilled)
        data = Gnuplot.Data((pmc.k_inactive+pmc.convergence), with="lines",
                title="kest=%.6f, kvarest=%.2E, kcalc=%.6f" %(pmc.k,
                    pmc.vark, kbench))
        cycleData = Gnuplot.Data((pmc.k_inactive+pmc.cycle_k), with="points",
                title="cycle estimates")
        chart.replot(data, cycleData)

        srcChart = Gnuplot.Gnuplot()
        hs = fissionSource.histogramSource(pmc.source, pmc.geo)
        sourceData = Gnuplot.Data(hs, with='lines')
        srcChart.title('Power Method Source, histories = %i' %histories)
#       srcChart.plot(sourceData)


def makeFBank(parts):
    """
    """
    n = particle.neutron()
    bank = fissionBank.fissionBank()
    bank.append(n,parts)

    return bank


def makeFSource(geo):
    """
    """
    pSource = scipy.zeros(geo.bins)
    mid = int(geo.bins/2)
    pSource[mid] = 1
    hs = fissionSource.histogramSource(pSource, geo)

    return hs


main()
