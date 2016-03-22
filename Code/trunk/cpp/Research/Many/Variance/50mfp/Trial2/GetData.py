__id__       = "$Id: GetData.py 704 2009-07-13 19:14:28Z jlconlin $"
__author__   = "$Author: jlconlin $"
__version__ = " $Revision: 704 $"
__date__     = "$Date: 2009-07-13 13:14:28 -0600 (Mon, 13 Jul 2009) $"

"""
This module is used to extract the data from the output files used in this
parameter study.
"""

import os
import sys
import numpy
import string
import re

p=os.path.join(os.path.expanduser('~'), 'Code/Misc')
try:
    sys.path.index(p)
except ValueError:
    sys.path.append(p)
import gnuFile

def main():
    WriteFile = "Variance.dat"

    # Get files in directory
    Files = os.listdir( os.getcwd() )
    ArnoldiDatFiles = []
    PowerDatFiles = []
    for f in Files:
        root, ext = os.path.splitext(f) # Get root and extension of file
        if ext == '.dat': 
            parts = string.split(f, '.')
            if( parts[0][0] == 'A'):
                ArnoldiDatFiles.append(f)
            elif( parts[0][0] == 'P'):
                PowerDatFiles.append(f)

    ArnoldiMean = []
    ArnoldiSD = []
    for f in ArnoldiDatFiles:
        gF = gnuFile.gnuFile(f)
        data = gF.Data['RAM eigenvalue-0-Real'][-1]
        ArnoldiMean.append(data[1])
        ArnoldiSD.append(data[2])

    PowerMean = []
    PowerSD = []
    for f in PowerDatFiles:
        gF = gnuFile.gnuFile(f)
        data = gF.Data['Power values'][-1]
        PowerMean.append(data[1])
        PowerSD.append(data[2])

    Data = {}
    Data['Arnoldi Values'] = (ArnoldiMean, ArnoldiSD)
    Data['Power Values'] = (PowerMean, PowerSD)
    gnuFile.Write_gnuFile(WriteFile, Data)
    return ArnoldiMean, ArnoldiSD, PowerMean, PowerSD


if __name__ == "__main__":
    print "Collecting Data."

    Am, Asd, Pm, Psd = main()

    print "Power   & %8.5f & %6.1E & %6.1E \\\\" %(
            numpy.mean(Pm), numpy.mean(Psd), numpy.std(Pm))
    print "Arnoldi & %8.5f & %6.1E & %6.1E \\\\" %(
            numpy.mean(Am), numpy.mean(Asd), numpy.std(Am))


