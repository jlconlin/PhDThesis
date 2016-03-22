__id__       = "$Id: kvsNoise.py 98 2007-07-18 19:41:04Z jlconlin $"
__author__   = "$Author: jlconlin $"
__version__ = " $Revision: 98 $"
__date__     = "$Date: 2007-07-18 13:41:04 -0600 (Wed, 18 Jul 2007) $"

"""This module is meant to complement the Mathematica notes included in 
Thesis/Notes.  It is a numerical investigation in the bias of the estimated
eigenvalue due to noise."""

import sys

import scipy
import Gnuplot

import arnoldiDTM
import Matrix


if __name__ == "__main__":
    M = 2
    A = Matrix.Diagonal(scipy.arange(M,0,-1))
    I = M+1
    q = scipy.zeros(M)
    q[0] = 1

    Chart = Gnuplot.Gnuplot()
    Chart.xlabel('Standard Deviation of Noise')
    Chart.ylabel('Mean Eigenvalues')

    Means = []
    Noises = scipy.arange(1,500,10)
    for N in Noises:
        eValues = []
        for i in xrange(100):
            adtm = arnoldiDTM.arnoldiDTM(A, Noise=N, NoiseShape='normal')
            Values, Vectors = adtm.arnoldi(q, I)
            eValues.append(Values[-1])

        Means.append(scipy.mean(eValues))

    meanData = Gnuplot.Data(Noises, Means, with='lines')
    Chart.replot(meanData)

