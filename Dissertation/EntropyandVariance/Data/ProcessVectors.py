__id__       = "$Id: ProcessVectors.py 769 2009-08-19 05:10:17Z jlconlin $"
__author__   = "$Author: jlconlin $"
__version__ = " $Revision: 769 $"
__date__     = "$Date: 2009-08-18 23:10:17 -0600 (Tue, 18 Aug 2009) $"

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

    SPFilename = 'Symmetric.Power-2.dat'
    SAFilename = 'Symmetric.Arnoldi.dat'
    SA2Filename = 'Symmetric.Arnoldi-2.dat'
    AsPFilename = 'Asymmetric.Power.dat'
    AsAFilename = 'Asymmetric.Arnoldi.dat'
    processFile = 'ProcessedVectors.dat'

    SPFile = gnuFile.gnuFile(SPFilename)
    SAFile = gnuFile.gnuFile(SAFilename)
    SA2File = gnuFile.gnuFile(SA2Filename)
    AsPFile = gnuFile.gnuFile(AsPFilename)
    AsAFile = gnuFile.gnuFile(AsAFilename)

    # Get eigenvectors
    Sp0 = SPFile.Data['Power vector']
    Sa0 = SAFile.Data['RAM eigenvector-0-Real']
    Sa1 = SAFile.Data['RAM eigenvector-1-Real']
    Sa20 = SA2File.Data['RAM eigenvector-0-Real']
    Sa21 = SA2File.Data['RAM eigenvector-1-Real']
    Asp0 = AsPFile.Data['Power vector']
    Asa0 = AsAFile.Data['RAM eigenvector-0-Real']
    Asa1 = AsAFile.Data['RAM eigenvector-1-Real']

    # Normalize eigenvectors
    Sp0[:,1] /= max(Sp0[:,1])
    Sa0[:,1] /= max(Sa0[:,1])
    Sa1[:,1] /= max(Sa1[:,1])
    Sa20[:,1] /= min(Sa20[:,1])
    Sa21[:,1] /= max(Sa21[:,1])

    Asp0[:,1] /= max(Asp0[:,1])
    Asa0[:,1] /= max(Asa0[:,1])
    Asa1[:,1] /= max(Asa1[:,1])

    pyplot.figure()
    pyplot.plot(Sp0[:,0], Sp0[:,1], ls='steps', label='Sy-Power')
    pyplot.plot(Sa0[:,0], Sa0[:,1], ls='steps', label='Sy-Arnoldi-0')
    pyplot.plot(Sa1[:,0], Sa1[:,1], ls='steps', label='Sy-Arnoldi-1')
    pyplot.legend()

    pyplot.figure()
    pyplot.plot(Asp0[:,0], Asp0[:,1], ls='steps', label='Asy-Power')
    pyplot.plot(Asa0[:,0], Asa0[:,1], ls='steps', label='Asy-Arnoldi-0')
    pyplot.plot(Asa1[:,0], Asa1[:,1], ls='steps', label='Asy-Arnoldi-1')
    pyplot.legend()

    # Write Data to file
    Header = "This data was taken from the file: %s, and %s" %(
            SPFilename, SAFilename)
    Data = {}
    Data['Sym-Power'] = (Sp0[:,0], Sp0[:,1])
    Data['Sym-Arnoldi-0'] = (Sa0[:,0], Sa0[:,1])
    Data['Sym-Arnoldi-1'] = (Sa1[:,0], Sa1[:,1])
    Data['Sym2-Arnoldi-0'] = (Sa20[:,0], Sa20[:,1])
    Data['Sym2-Arnoldi-1'] = (Sa21[:,0], Sa21[:,1])

    Data['Asym-Power'] = (Asp0[:,0], Asp0[:,1])
    Data['Asym-Arnoldi-0'] = (Asa0[:,0], Asa0[:,1])
    Data['Asym-Arnoldi-1'] = (Asa1[:,0], Asa1[:,1])
    gnuFile.Write_gnuFile(processFile, Data, header=Header)


