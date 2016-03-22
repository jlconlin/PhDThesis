_id__       = "$Id: arnoldiDTM.py 98 2007-07-18 19:41:04Z jlconlin $"
__author__   = "$Author: jlconlin $"
__version__ = " $Revision: 98 $"
__date__     = "$Date: 2007-07-18 15:41:04 -0400 (Wed, 18 Jul 2007) $"

import cmath
import time
import sys

import scipy.stats
import Gnuplot

import gnuplotFile

"""This module takes uses the formula of the eigenvalue from Mathematica and
applies it many times, finds the mean and standard deviation.  The formulas for
the eigenvalue are taken from an analytical analysis in Mathematica of a noisy
Arnoldi's method on a 2x2 matrix:
A = [[alpha  0]
     [ 0  beta]]
"""

def Value1(alpha, beta, Xi11, Xi12, Xi21, Xi22):
    """
    Value1 returns a list of eigenvalues estimates equal in length to the lists
    of random numbers.

    alpha: First diagonal element of A.
    beta: Second diagonal element of A.
    """
    eValues = scipy.zeros(len(Xi11), dtype=complex)

    for i, xi11, xi12, xi21, xi22 in zip(
            xrange(len(eValues)), Xi11, Xi12, Xi21, Xi22):
        eValues[i] = 1.0/2.0*(alpha - beta + xi11 + xi22 - cmath.sqrt(
            alpha*alpha + beta*beta - 2*beta*xi11 + xi11*xi11 + 4*xi12*xi21 + 
            2*beta*xi22 - 2*xi11*xi22 + xi22*xi22 - 2*alpha*(beta - xi11 + xi22)))

    return eValues


def Value2(alpha, beta, Xi11, Xi12, Xi21, Xi22):
    """
    Value2 returns a list of eigenvalues estimates equal in length to the lists
    of random numbers.

    alpha: First diagonal element of A.
    beta: Second diagonal element of A.
    """
    eValues = scipy.zeros(len(Xi11), dtype=complex)

    for i, xi11, xi12, xi21, xi22 in zip(
            xrange(len(eValues)), Xi11, Xi12, Xi21, Xi22):
        eValues[i] = 1.0/2.0*(-alpha + beta + xi11 + xi22 + cmath.sqrt(
            alpha*alpha + beta*beta - 2*beta*xi11 + xi11*xi11 + 4*xi12*xi21 + 
            2*beta*xi22 - 2*xi11*xi22 + xi22*xi22 - 2*alpha*(beta - xi11 + xi22)))

    return eValues

if __name__ == "__main__":

    mu = 0.0
    sd = [0.01, 0.1, 0.25, 0.5, 0.75, 1.0, 1.5, 2.0, 5.0, 7.5, 10.0]
    num = 1E3
    repeats=10000

    Chart = Gnuplot.Gnuplot()
    Chart.xlabel("Standard Deviation of Noise")
    Chart.ylabel("Eigenvalue bias")


    Means = []
    StdDev = []
    gnuData = {}
    start = time.clock()
    for dev in sd:
        means = []
        stddev = []
        sums = 0
        sumofSquares = 0
        for i in xrange(repeats):
            p = scipy.stats.norm(mu, dev)
            Xi11 = p.rvs(num); Xi12 = p.rvs(num); Xi21 = p.rvs(num); Xi22 = p.rvs(num)
#           x = Value1(10,9, Xi11, Xi12, Xi21, Xi22)
            x = Value2(10,9, Xi11, Xi12, Xi21, Xi22)
            sums += sum(x)
            sumofSquares += sum(x*x)
            end = time.clock()
            means.append(sums/(num*(i+1)))
            stddev.append(cmath.sqrt(1/(num*(i+1))*(sumofSquares/(num*(i+1)) - means[-1]**2)))
#           print "\tfinished %4.2E, time: %4.2f" %((i+1)*num, end-start)
        print "mu: %6.4f, std. dev.: %6.4f, time: %4.2f" %(mu,dev, (end-start))
        Means.append(means)
        StdDev.append(stddev)

    Means = scipy.transpose(Means)
    StdDev = scipy.transpose(StdDev)

    gnuData["%4.2E" %(num*1)] = (sd, Means[0], StdDev[0])
    gnuData["%4.2E" %(num*10)] = (sd, Means[9], StdDev[9])
    gnuData["%4.2E" %(num*100)] = (sd, Means[99], StdDev[99])
    gnuData["%4.2E" %(num*1000)] = (sd, Means[999], StdDev[999])
    gnuData["%4.2E" %(num*10000)] = (sd, Means[9999], StdDev[9999])
#   gnuData["%E" %(num*10000)] = (sd, Means[9999], StdDev[9999])
#   gnuData["%E" %(num*10)] = (sd, Means[9], StdDev[9])
#   gnuData["%E" %(num*50)] = (sd, Means[49], StdDev[49])
#   gnuData["%E" %(num*100)] = (sd, Means[99], StdDev[99])
#   gnuData["%E" %(num*200)] = (sd, Means[199], StdDev[199])
#   gnuData["%E" %(num*500)] = (sd, Means[499], StdDev[499])
    
    if len(sys.argv) > 1:
        gnuFile = gnuplotFile.gnuplotFile(sys.argv[1], gnuData)

#   print x

print "Time Elapsed: %.2f" %(end-start)

