__id__       = "$Id: ProcessVectors.py 739 2009-07-27 20:51:11Z jlconlin $"
__author__   = "$Author: jlconlin $"
__version__ = " $Revision: 739 $"
__date__     = "$Date: 2009-07-27 14:51:11 -0600 (Mon, 27 Jul 2009) $"

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

    try:
        width = eval(sys.argv[1])
        aFilename = 'ArnoldiBasic-w%s.dat' %width
        pFilename = 'PowerBasic-w%s.dat' %width
        oFilename = 'OUTSN-w%s.dat' %width
        processFile = 'Processed-w%s.dat' %width

        print "aFile: %s\npFile: %s\noFile: %s" %(
                aFilename, pFilename, oFilename)
        aFile = gnuFile.gnuFile(aFilename)
        pFile = gnuFile.gnuFile(pFilename)
        oFile = gnuFile.gnuFile(oFilename)

        # Get eigenvectors
        a0 = aFile.Data['RAM eigenvector-0-Real']
        a1 = aFile.Data['RAM eigenvector-1-Real']
        a2 = aFile.Data['RAM eigenvector-2-Real']
        p0 = pFile.Data['Power vector']
        o0 = oFile.Data['FissionSource']

        # Normalize eigenvectors
        a0[:,1] /= max(a0[:,1])
        a1[:,1] /= max(a1[:,1])
        a2[:,1] /= max(a2[:,1])
        p0[:,1] /= max(p0[:,1])
        o0[:,1] /= max(o0[:,1])

        pyplot.figure()
        pyplot.plot(a0[:,0], a0[:,1], ls='steps',label='Arnoldi-0')
        pyplot.plot(a1[:,0], a1[:,1], ls='steps',label='Arnoldi-1')
        pyplot.plot(a2[:,0], a2[:,1], ls='steps',label='Arnoldi-2')
        pyplot.plot(p0[:,0], p0[:,1], ls='steps',label='Power')
        pyplot.plot(o0[:,0], o0[:,1], '-k',ls='steps',label='Reference') 
        pyplot.legend(loc=8)
        pyplot.title('Width = %s' %width)

        # Write Data to file
        Header = "This data was taken from the files: %s, %s, %s" %(
                aFilename, pFilename, oFilename)
        Data = {}
        Data['Arnoldi-0'] = (a0[:,0], a0[:,1])
        Data['Arnoldi-1'] = (a1[:,0], a1[:,1])
        Data['Arnoldi-2'] = (a2[:,0], a2[:,1])
        Data['Power'] = (p0[:,0], p0[:,1])
        Data['SN'] = (o0[:,0], o0[:,1])
        gnuFile.Write_gnuFile(processFile, Data, header=Header)


    except IndexError:
        print "You must give a width."


