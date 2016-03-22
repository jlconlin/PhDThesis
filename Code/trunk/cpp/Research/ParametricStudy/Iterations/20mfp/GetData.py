__id__       = "$Id: GetData.py 576 2009-06-02 02:20:26Z jlconlin $"
__author__   = "$Author: jlconlin $"
__version__ = " $Revision: 576 $"
__date__     = "$Date: 2009-06-01 20:20:26 -0600 (Mon, 01 Jun 2009) $"

"""
This module is used to extract the data from the output files used in this
parameter study.
"""

import os
import sys
p=os.path.join(os.path.expanduser('~'), 'Code/Misc')
try:
    sys.path.index(p)
except ValueError:
    sys.path.append(p)
import gnuFile


WriteFile = 'Data.dat'
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
keys = Eigen0.keys()    # A key looks like '.I10'
keys.sort(lambda x, y: int(x[2:])-int(y[2:]))

# Extract data
Parameter = []
Histories = [ [], [], [] ]
Eigenvalues = [ [], [], [] ]
EigenvaluesSD = [ [], [], [] ]
F = []
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


Data = {}
Data['FOM'] = (Parameter, F)
Data['Eigenvalue-0'] = (Parameter, Eigenvalues[0], EigenvaluesSD[0])
Data['Eigenvalue-1'] = (Parameter, Eigenvalues[1], EigenvaluesSD[1])
Data['Eigenvalue-2'] = (Parameter, Eigenvalues[2], EigenvaluesSD[2])

gnuFile.Write_gnuFile(WriteFile, Data)
