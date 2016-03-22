_id__       = "$Id: NoisyBias10.py 116 2007-08-13 20:22:38Z jlconlin $"
__author__   = "$Author: jlconlin $"
__version__ = " $Revision: 116 $"
__date__     = "$Date: 2007-08-13 14:22:38 -0600 (Mon, 13 Aug 2007) $"

import cmath
import time
import sys

import scipy.stats

import gnuplotFile

"""This module takes uses the formula of the eigenvalue from Mathematica and
applies it many times, finds the mean and standard deviation.  The formulas for
the eigenvalue are taken from an analytical analysis in Mathematica of a noisy
Arnoldi's method on a 2x2 matrix:
A = [[alpha  0]
     [ 0  beta]]

This module will repeat until the standard deviation is < 10% % of the
eigenvalue bias.
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
    num = 1E5

    Means = [0.0]
    StdDev = [0.0]
    start = time.clock()
    for dev in sd:
        print "dev: %6.4f" %(dev)
        repeat=0
        m = 0.0
        stddev = 0.0
        sums = 0.0
        sumofSquares = 0.0
        while abs(stddev) >= abs(0.1*m):
            repeat += 1 # Count how many times I have repeated
            p = scipy.stats.norm(mu, dev)
            Xi11 = p.rvs(num); Xi12 = p.rvs(num); Xi21 = p.rvs(num); Xi22 = p.rvs(num)
#           x = Value1(10,9, Xi11, Xi12, Xi21, Xi22)
            x = Value2(10,9, Xi11, Xi12, Xi21, Xi22)
            sums += sum(x)
            sumofSquares += sum(x*x)
            m = sums/(num*repeat)
            stddev = cmath.sqrt(1/(num*repeat)*(sumofSquares/(num*repeat) -
                m**2))
            end = time.clock()
            print "\trepeat #: %i, mean = %6.4G, stddev = %6.4G, time: %4.2f" %(
		repeat, m.real, stddev.real,(end-start))
        Means.append(m.real)
        StdDev.append(stddev.real)

        gnuData = {"Data":(sd, Means, StdDev)}

        if len(sys.argv) > 1:
            gnuFile = gnuplotFile.gnuplotFile(sys.argv[1], gnuData)


