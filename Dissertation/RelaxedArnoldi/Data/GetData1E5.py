__id__       = "$Id: GetData1E5.py 718 2009-07-17 21:23:07Z jlconlin $"
__author__   = "$Author: jlconlin $"
__version__ = " $Revision: 718 $"
__date__     = "$Date: 2009-07-17 15:23:07 -0600 (Fri, 17 Jul 2009) $"

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

def TeXWrite(eta, active, Tracks, Eigenvalue, SD, FOM, Time):
    """
    TeXWrite will write the data for the screen suitable to be copied and pasted
    for LaTeX.
    """
    n = len(eta)
    for i in xrange(n):
        print "%8.2G & %5i & %9.3e & %.4f $\\pm$  %.1e & %8.1f & %6.1f \\\\" %(
                eta[i], active[i], Tracks[i], Eigenvalue[i], SD[i], FOM[i],
                Time[i] )

def GetActiveRestarts(filename):
    """
    """
    f = open(filename, 'r')
    lines = f.readlines()
    f.close()
    line = string.join(lines)

    s = re.search('--active (\d*)', line)
    return int(s.groups()[0])

def main():
    WriteFile = "Relaxed1E5.dat"
    lambda0 = 0.997520

    # Get files in directory
    fileDir = os.path.join(os.getcwd(), 'MoreAgg/1E5')
    Files = os.listdir( fileDir )
    ScreenFiles = []
    DatFiles = []
    for f in Files:
        root, ext = os.path.splitext(f) # Get root and extension of file
        if ext == '.screen':
            ScreenFiles.append(f)
        elif ext == '.dat': 
            DatFiles.append(os.path.join(fileDir,f) )

    # Remove any file we don't like
    try:
        DatFiles.pop( DatFiles.index(
            os.path.join(fileDir, "50mfp.Power.1E5.dat" ) ) )
    except ValueError: pass
        
    Eigen0 = {}
    Eigen1 = {}
    Eigen2 = {}
    Time = {}
    FOM = {}
    Active = {}     # active iterations
    for f in DatFiles:
        filename = os.path.basename(f)
        root, ext = os.path.splitext(filename) # Remove extension from file
        parameter = float(root[7:])    # root looks like '50mfp.r0.01'
        gF = gnuFile.gnuFile(f)

        Eigen0[parameter] = gF.Data['RAM eigenvalue-0-Real'][-1]
        Eigen1[parameter] = gF.Data['RAM eigenvalue-1-Real'][-1]
        Eigen2[parameter] = gF.Data['RAM eigenvalue-2-Real'][-1]
        Time[parameter] = gF.Data['Arnoldi-Time'][-1]
        FOM[parameter] = gF.Data['FOM-Arnoldi'][-1]
        Active[parameter] = GetActiveRestarts(f)

    # Sort data by parameter value
    keys = Eigen0.keys()    # A key looks like '.r10'
    keys.sort()

    # Extract data
    Parameter = []
    Histories = [ ]
    Eigenvalues = [ [], [], [] ]
    EigenvaluesSD = [ [], [], [] ]
    F = [ [], [], [] ]
    totalTime = []
    A = []
    for key in keys:
        Parameter.append(float(key))
        n,v,sd = Eigen0[key]
        Histories.append(n)
        Eigenvalues[0].append(v)
        EigenvaluesSD[0].append(sd)

        # Get time information
        F[0].append(FOM[key][-1])
        T = float(Time[key][-1])
        totalTime.append(T)

        n,v,sd = Eigen1[key]
        Eigenvalues[1].append(v)
        EigenvaluesSD[1].append(sd)
        F[1].append(1.0/(T*sd*sd))

        n,v,sd = Eigen2[key]
        Eigenvalues[2].append(v)
        EigenvaluesSD[2].append(sd)
        F[2].append(1.0/(T*sd*sd))

        A.append(Active[key])

    Data = {}
    Data['Tracks'] = (Parameter, Histories)
    Data['Eigenvalue-0'] = (Parameter, Eigenvalues[0], EigenvaluesSD[0])
    Data['Eigenvalue-1'] = (Parameter, Eigenvalues[1], EigenvaluesSD[1])
    Data['Eigenvalue-2'] = (Parameter, Eigenvalues[2], EigenvaluesSD[2])
    Data['Time'] = (Parameter, totalTime)
    Data['FOM-0'] = (Parameter, F[0])
    Data['FOM-1'] = (Parameter, F[1])
    Data['FOM-2'] = (Parameter, F[2])

    gnuFile.Write_gnuFile(WriteFile, Data)

    print "\nTeX Table for Fundamental:\n"
    TeXWrite(Parameter,A, Histories, Eigenvalues[0], EigenvaluesSD[0],F[0],
            totalTime)
    print "\nTeX Table for first higher order mode\n"
    TeXWrite(Parameter,A, Histories, Eigenvalues[1], EigenvaluesSD[1],F[1],
            totalTime)
    print "\nTeX Table for second higher order mode\n"
    TeXWrite(Parameter,A, Histories, Eigenvalues[2], EigenvaluesSD[2],F[2],
            totalTime)

    return Parameter, A, Histories
    

if __name__ == "__main__":
    eta, A, N = main()

    filenames = []
    # output results so I know how many active restarts to run
    TN = N[0]
    print "\neta\t\tN\t\tcycles\t  New\t  diff"
    for e, a, n in zip(eta, A, N):
        n_per_a = n/(a+100)
        TI = int(TN/n_per_a)
        new_a = TI-100
        diff_a = a-new_a

        if abs(diff_a) > 10:
            sys.stdout.write("\x1b[31m")
            filenames.append("50mfp.r%.2g.q" %e)
        print "%8.2g\t %9.3E\t %5i\t %5i\t %5i " %(e, n, a, new_a, diff_a)
        if abs(diff_a) > 10:
            sys.stdout.write("\x1b[m")

    print "\n%s" %string.join(filenames)

