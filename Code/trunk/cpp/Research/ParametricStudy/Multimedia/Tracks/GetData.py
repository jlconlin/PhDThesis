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
    EntropyFile = "EntropyConvergence.dat"
    ValuesFile = "ValuesConvergence.dat"

    # Get files in directory
    Files = os.listdir( os.getcwd() )
    ScreenFiles = []
    ArnoldiDatFiles = []
    PowerDatFiles = []
    for f in Files:
        root, ext = os.path.splitext(f) # Get root and extension of file
        if ext == '.screen':
            ScreenFiles.append(f)
        elif ext == '.dat': 
            parts = string.split(f, '.')
            if( parts[0] == 'ArnoldiEntropy'):
                ArnoldiDatFiles.append(f)
            elif( parts[0] == 'PowerEntropy'):
                PowerDatFiles.append(f)

    EntropyData = {}
    ValuesData = {}
    PowerEigen = {}
    ArnoldiEigen = {}
    # Process Arnoldi files
    for f in ArnoldiDatFiles:
        gF = gnuFile.gnuFile(f)
        parts = string.split(f, '.')
        tracks = float(parts[1][1:])   # parts[1] looks like 't1000'
        t = '%.1G' %tracks

        EI = gF.Data['Entropy-Iterations']
        ER = gF.Data['Entropy-Restarts']
        RE = gF.Data['raw value-00-Real']
        ME = gF.Data['RAM eigenvalue-0-Real']
        EntropyData['Ent-Arn-Iter-%s' %t] = (EI[:,0], EI[:,1])
        EntropyData['Ent-Arn-Rest-%s' %t] = (ER[:,0], ER[:,1])
        ValuesData['Arn Raw Value-%s' %t] = (RE[:,0], RE[:,1])
        ValuesData['Arn Mean Value-%s' %t] = (ME[:,0], ME[:,1], ME[:,2])

        ArnoldiEigen[tracks] = ME[-1]

    for f in PowerDatFiles:
        gF = gnuFile.gnuFile(f)
        parts = string.split(f, '.')
        tracks = float(parts[1][1:])   # parts[1] looks like 't1000'
        t = '%.1G' %tracks

        E = gF.Data['Entropy']
        V = gF.Data['Power values']
        R = gF.Data['Power raw values']
        EntropyData['Ent-Pow-%s' %t] = (E[:,0],E[:,1])
        ValuesData['Pow Mean Value-%s' %t] = (V[:,0],V[:,1], V[:,2])
        ValuesData['Pow Raw Value-%s' %t] = (R[:,0],R[:,1])

        PowerEigen[tracks] = V[-1]

    Powerkeys = PowerEigen.keys()
    Arnoldikeys = ArnoldiEigen.keys()
    Powerkeys.sort()
    Arnoldikeys.sort()

    ArnoldiValues = []
    PowerValues = []
    ArnoldiSD = []
    PowerSD = []
    for a,p in zip(Arnoldikeys, Powerkeys):
        ArnoldiValues.append(ArnoldiEigen[a][1])
        ArnoldiSD.append(ArnoldiEigen[a][2])
        PowerValues.append(PowerEigen[a][1])
        PowerSD.append(PowerEigen[a][2])

    ValuesData['Arnoldi Values'] = (Arnoldikeys, ArnoldiValues, ArnoldiSD)
    ValuesData['Power Values'] = (Powerkeys, PowerValues, PowerSD)


    gnuFile.Write_gnuFile(EntropyFile, EntropyData)
    gnuFile.Write_gnuFile(ValuesFile, ValuesData)

if __name__ == "__main__":
    main()

