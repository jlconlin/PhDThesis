__id__       = "$Id: GetData.py 609 2009-06-14 02:46:04Z jlconlin $"
__author__   = "$Author: jlconlin $"
__version__ = " $Revision: 609 $"
__date__     = "$Date: 2009-06-13 20:46:04 -0600 (Sat, 13 Jun 2009) $"

"""
This module is used to extract the data from the output files used in this
parameter study.
"""

import os
import sys
import numpy
p=os.path.join(os.path.expanduser('~'), 'Code/Misc')
try:
    sys.path.index(p)
except ValueError:
    sys.path.append(p)
import gnuFile


def TeXWrite(NumBins, BinWidth, Eigenvalue, SD, Bias, FOM):
    n = len(NumBins)
    for i in xrange(n):
        print "%3s & %.2f & %.4f & %.1e & %.1e & %.1f \\\\" %(
                NumBins[i], BinWidth[i], Eigenvalue[i], SD[i], Bias[i], FOM[i])

def main():
    # Needed variables
    WriteFile = 'BiasLinear.dat'
    lambda0 = 4.827752
    lambda1 = 4.383101
    lambda2 = 3.817469
    Width = 20.0

    # Get files in directory
    Files = os.listdir( os.getcwd() )
    ScreenFiles = []
    DatFiles = []
    for f in Files:
        root, ext = os.path.splitext(f) # Get root and extension of file
    #   print "file: %s, root = %s, ext = %s" %(f, root, ext)
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
    Eigen2 = {}
    FOM = {}
    for f in DatFiles:
        root, ext = os.path.splitext(f) # Remove extension from file
        root, parameter = os.path.splitext(root) # Get parameter information
        gF = gnuFile.gnuFile(f)

        Eigen0[parameter] = gF.Data['RAM eigenvalue-0-Real'][-1]
        Eigen1[parameter] = gF.Data['RAM eigenvalue-1-Real'][-1]
        Eigen2[parameter] = gF.Data['RAM eigenvalue-2-Real'][-1]
        FOM[parameter] = gF.Data['FOM-Arnoldi'][-1]

    # Sort data by parameter value
    keys = Eigen0.keys()    # A key looks like '.b10'
    keys.sort(lambda x, y: int(x[2:])-int(y[2:]))

    # Extract data
    Parameter = []
    Histories = [ [], [], [] ]
    Eigenvalues = [ [], [], [] ]
    EigenvaluesSD = [ [], [], [] ]
    F = []
    Widths = []
    for key in keys:
        Parameter.append(int(key[2:]))
        n,v,sd = Eigen0[key]
        Histories[0].append(n)
        Eigenvalues[0].append(v)
        EigenvaluesSD[0].append(sd)
        n,v,sd = Eigen1[key]
        Histories[1].append(n)
        Eigenvalues[1].append(v)
        EigenvaluesSD[1].append(sd)
        n,v,sd = Eigen2[key]
        Histories[2].append(n)
        Eigenvalues[2].append(v)
        EigenvaluesSD[2].append(sd)

        F.append(FOM[key][-1])
        Widths.append(Width/int(key[2:]))

    Bias0 = abs(lambda0 - numpy.array(Eigenvalues[0]))
    Bias1 = abs(lambda1 - numpy.array(Eigenvalues[1]))
    Bias2 = abs(lambda2 - numpy.array(Eigenvalues[2]))

    Data = {}
    Data['Bias0'] = (Widths, Bias0, EigenvaluesSD[0])
    Data['Bias1'] = (Widths, Bias1, EigenvaluesSD[1])
    Data['Bias2'] = (Widths, Bias2, EigenvaluesSD[2])
    Data['Eigenvalue-0'] = (Widths, Eigenvalues[0], EigenvaluesSD[0])
    Data['Eigenvalue-1'] = (Widths, Eigenvalues[1], EigenvaluesSD[1])
    Data['Eigenvalue-2'] = (Widths, Eigenvalues[2], EigenvaluesSD[2])
    Data['FOM'] = (Widths, F)

    gnuFile.Write_gnuFile(WriteFile, Data)

    print "\nTex Table output for Fundamental:\n"
    TeXWrite(Parameter, Widths, Eigenvalues[0], EigenvaluesSD[0], Bias0, F)
    print "\nTex Table output for first higher order:\n"
    TeXWrite(Parameter, Widths, Eigenvalues[1], EigenvaluesSD[1], Bias0, F)
    print "\nTex Table output for second higher order:\n"
    TeXWrite(Parameter, Widths, Eigenvalues[2], EigenvaluesSD[2], Bias0, F)

if __name__ == "__main__":
    main()

