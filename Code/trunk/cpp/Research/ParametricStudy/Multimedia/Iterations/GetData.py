__id__       = "$Id: GetData.py 729 2009-07-21 03:41:17Z jlconlin $"
__author__   = "$Author: jlconlin $"
__version__ = " $Revision: 729 $"
__date__     = "$Date: 2009-07-20 21:41:17 -0600 (Mon, 20 Jul 2009) $"

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
    WriteFile = "Iterations.dat"

    # Get files in directory
    Files = os.listdir( os.getcwd() )
    ScreenFiles = []
    DatFiles = []
    for f in Files:
        root, ext = os.path.splitext(f) # Get root and extension of file
        if ext == '.screen':
            ScreenFiles.append(f)
        elif ext == '.dat': 
            DatFiles.append(f)

    # Remove any file we don't like
    try:
        DatFiles.pop( DatFiles.index(WriteFile) )
    except ValueError: pass
        
    Eigen0 = {}
    Eigen1 = {}
    for f in DatFiles:
        parts = string.split(f, '.')
        parameter = float(parts[1][1:])
        gF = gnuFile.gnuFile(f)

        Eigen0[parameter] = gF.Data['RAM eigenvalue-0-Real'][-1]
        Eigen1[parameter] = gF.Data['RAM eigenvalue-1-Real'][-1]

    # Sort data by parameter value
    keys = Eigen0.keys()    # A key looks like '.I10'
    keys.sort()

    # Extract data
    Parameter = []
    Histories = [ ]
    Eigenvalues = [ [], [], [] ]
    EigenvaluesSD = [ [], [], [] ]
    for key in keys:
        Parameter.append(float(key))
        n,v,sd = Eigen0[key]
        Histories.append(n)
        Eigenvalues[0].append(v)
        EigenvaluesSD[0].append(sd)
        n,v,sd = Eigen1[key]
        Eigenvalues[1].append(v)
        EigenvaluesSD[1].append(sd)


    Data = {}
    Data['Tracks'] = (Parameter, Histories)
    Data['Eigenvalue-0'] = (Parameter, Eigenvalues[0], EigenvaluesSD[0])
    Data['Eigenvalue-1'] = (Parameter, Eigenvalues[1], EigenvaluesSD[1])

    gnuFile.Write_gnuFile(WriteFile, Data)

if __name__ == "__main__":
    main()

