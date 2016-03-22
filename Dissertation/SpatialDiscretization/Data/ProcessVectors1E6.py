__id__       = "$Id: ProcessVectors1E6.py 761 2009-08-12 22:07:08Z jlconlin $"
__author__   = "$Author: jlconlin $"
__version__ = " $Revision: 761 $"
__date__     = "$Date: 2009-08-12 16:07:08 -0600 (Wed, 12 Aug 2009) $"

"""
ProcessVectors will process the eigenvectors output from my C++ code to prepare
them for using in my Thesis.
"""
import sys
import os
import re
import matplotlib.pyplot as pyplot
import numpy

# Add path so Python can find my gnuFile class
p = os.path.join(os.path.expanduser('~'),'Code/Misc')
try:
    sys.path.index(p)
except ValueError:
    sys.path.append(p)
import gnuFile


if __name__ == "__main__":
    """
    Write this in thesis: 'The vectors are scaled differently for clarity.'
    """
    print "Processing Eigenvectors."

    LFilename = 'Linear1E6.b60.dat'
    HFilename = 'Histogram1E6.b60.dat'
    OFilename = 'OUTSN-w20.dat'
    processFile = 'ProcessedVectors1E6.dat'

    print "HFile: %s\nHFile: %s\nOFile: %s" %(LFilename, HFilename, OFilename)

    LFile = gnuFile.gnuFile(LFilename)
    HFile = gnuFile.gnuFile(HFilename)
    OFile = gnuFile.gnuFile(OFilename)

    # Get eigenvectors
    l0 = LFile.Data['RAM eigenvector-0']
    h0 = HFile.Data['RAM eigenvector-0-Real']
    l1 = LFile.Data['RAM eigenvector-1']
    h1 = HFile.Data['RAM eigenvector-1-Real']
    l2 = LFile.Data['RAM eigenvector-2']
    h2 = HFile.Data['RAM eigenvector-2-Real']
    o0 = OFile.Data['FissionSource']

    # Normalize eigenvectors
    l0[:,1] /= max(l0[:,1])
    l1[:,1] /= 2*max(l1[:,1])
    l2[:,1] /= 2*max(l2[:,1])
    h0[:,1] /= min(h0[:,1])
    h1[:,1] /= 2*max(h1[:,1])
    h2[:,1] /= 2*min(h2[:,1])
    o0[:,1] /= max(o0[:,1])

    pyplot.figure()
    pyplot.plot(l0[:,0], l0[:,1], ls='-', label='Linear-0')
    pyplot.plot(h0[:,0], h0[:,1], ls='steps', label='Histogram-0')
    pyplot.plot(l1[:,0], l1[:,1], ls='-', label='Linear-1')
    pyplot.plot(h1[:,0], h1[:,1], ls='steps', label='Histogram-1')
    pyplot.plot(l2[:,0], l2[:,1], ls='-', label='Linear-2')
    pyplot.plot(h2[:,0], h2[:,1], ls='steps', label='Histogram-2')
    pyplot.plot(o0[:,0], o0[:,1], ls='steps', label='S_N')
    pyplot.legend()

    # Write Data to file
    Header = "This data was taken from the file: %s, %s, and %s" %(
            LFilename, HFilename, OFilename)
    Data = {}
    Data['Linear-0'] = (l0[:,0], l0[:,1])
    Data['Linear-1'] = (l1[:,0], l1[:,1])
    Data['Linear-2'] = (l2[:,0], l2[:,1])
    Data['Histogram-0'] = (h0[:,0], h0[:,1])
    Data['Histogram-1'] = (h1[:,0], h1[:,1])
    Data['Histogram-2'] = (h2[:,0], h2[:,1])
    Data['SN'] = (o0[:,0], o0[:,1])
    gnuFile.Write_gnuFile(processFile, Data, header=Header)

