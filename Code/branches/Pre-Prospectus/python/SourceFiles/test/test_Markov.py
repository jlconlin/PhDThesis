
import math
import random

import particle
import fissionBank
import Markov
import Geometry
import CrossSection

__author__   = "$Author: jlconlin $"
__version__ = " $Revision: 22 $"
__date__     = "$Date: 2007-03-15 10:27:58 -0600 (Thu, 15 Mar 2007) $"
__id__       = "$Id: test_Markov.py 22 2007-03-15 16:27:58Z jlconlin $"


def main():
    """
    """
#   random.seed(684984361)

#   testtransport()
#   testRoulette()
    testtrackHistory()


def testtrackHistory():
    """
    """
    geo = Geometry.Geometry(10,[[-1,1]])
    xs = CrossSection.CrossSection()
    mark = Markov.Markov(geo,xs, histories=1000)

    n = particle.neutron()
    bank = fissionBank.fissionBank()
    bank.append(n,5)
    newbank = fissionBank.fissionBank()

    print "xs: %s" %xs
    for neut in bank[:1]:
        neut.setRandDirection()
        mark.trackHistory(neut, newbank, 1.0)


def testtransport():
    """
    """
    geo = Geometry.Geometry(10,[[-1,1]])
    xs = CrossSection.CrossSection()
    mark = Markov.Markov(geo,xs, histories=1000)

    bank = fissionBank.fissionBank()
    n = particle.neutron()
    bank.append(n,10)

    for neut in bank:
        neut.setRandDirection()

    newbank = mark.transport(bank, 1)
    print "leak-left: %.4f, leak-right: %.4f, weight-killed: %.4f" %(
            mark.minLeakage, mark.maxLeakage, mark.wgtKilled)



def testRoulette():
    """
    """
    geo = Geometry.Geometry(10,[[-1,1]])
    xs = CrossSection.CrossSection()
    mark = Markov.Markov(geo,xs, histories=1000)

    n = particle.neutron()
    bank = fissionBank.fissionBank()
    bank.append(n,1000000)

    print "weight-in: %s" %(len(bank))
    for i in xrange(len(bank)):
        bank[i] = mark.russianRoulette(bank[i])

    print "\nweight-out: %s, +- %.4f" %(bankWeight(bank),math.sqrt(len(bank)))


def bankWeight(bank):
    """
    """
    weight = 0
    for par in bank:
        if par:
            weight += par.weight
    return weight


main()
