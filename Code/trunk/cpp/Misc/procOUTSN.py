__id__       = "$Id: procOUTSN.py 572 2009-06-01 19:25:52Z jlconlin $"
__author__   = "$Author: jlconlin $"
__version__  = "$Revision: 572 $"
__date__     = "$Date: 2009-06-01 13:25:52 -0600 (Mon, 01 Jun 2009) $"

"""This module will read an OUTSN3 file from Professor Larsen's SN3 code and 
will extract the important information"""

import sys
import os
import numpy

# Add path so Python can find my gnuFile class
p = os.path.join(os.path.expanduser('~'),'Code/Misc')
try:
    sys.path.index(p)
except ValueError:
    sys.path.append(p)
import gnuFile

class procOutsn(object):

    def __init__(self, infile='OUTSN3'):
        self.infile = infile
        self.__process()
        object.__init__(self)


    def writeFile(self, outfile):
        """
        writeFile will put the data into the dictionary format required by the
            gnuFile module.

        outfile: Filename of output file to write.
        """
        data = {}
        data['Flux1'] = (self.X, self.F1)
        data['Flux2'] = (self.X, self.F2)
        data['Flux3'] = (self.X, self.F3)
        data['FissionSource'] = (self.X, 2*self.FS/sum(self.FS))
        gFile = gnuFile.Write_gnuFile(outfile, data)


    def coursenSNFS(self, geo):
        """
        coursenSNFS will average the fissionSource bin values in the bin range defined in the 
            geo bins so the two will have the same bin structure.  It returns 
            an array of bin centers and bin heights.

        geo: Geometry object
        """
        snbins = len(self.FS)
        mcbins = geo.bins
        centers = geo.centers
        heights = numpy.zeros(mcbins)
        
        binCombo = float(snbins)/mcbins
        for i in xrange(mcbins):
            heights[i] =numpy.mean( self.FS[binCombo*i:binCombo*(i+1)])

        heights = heights/sum(abs(heights)) # Normalize

        return heights, centers


    def __process(self):
        """
        __process will process the OUTSN3 file and retrieve the necessary 
        information.
        """
        sn3file = open(self.infile)
        try:
            self.outsn3 = sn3file.read().splitlines()
        finally:
            sn3file.close()

        self.regions, self.quadrature, self.bc = [
                int(w) for w in self.outsn3[4].split()]
        if self.bc:
            self.bc = 'Reflecting'
        else:
            self.bc = 'Vacuum'

        self.Regions = []
        for i in xrange(self.regions):
            region = {}
            line1 = self.outsn3[7+i]
            line2 = self.outsn3[7+self.regions+2]
            words = line1.split() + line2.split()
            region['cells'] = int(words[1])
            region['deltaX'] = float(words[2])
            region['xT1'] = float(words[3])
            region['xT2'] = float(words[4])
            region['xT3'] = float(words[5])
            region['xS1'] = float(words[6])
            region['xS12'] = float(words[7])
            region['xS13'] = float(words[8])
            region['xS2'] = float(words[9])
            region['xS23'] = float(words[10])
            region['xS3'] = float(words[11])
            region['xF1'] = float(words[12])
            region['xF2'] = float(words[13])
            region['xF3'] = float(words[14])
            self.Regions.append(region)

        self.fluxline = '       X       F1(X)       F2(X)       F3(X)       FS(X)'
        self.fluxListBegins = self.outsn3.index(self.fluxline)+2
        k = self.outsn3[self.fluxListBegins-4].split()[-1]
        self.k = self.eigenvalue = float(k)

        
        # Get flux data
        X = []
        F1 = []
        F2 = []
        F3 = []
        FS = []
        for line in self.outsn3[self.fluxListBegins-1:]:
            if line == '':
                break
            else:
                x, f1x, f2x,f3x, fsx = [float(n) for n in line.split()]
                X.append(x)
                F1.append(f1x)
                F2.append(f2x)
                F3.append(f3x)
                FS.append(fsx)

        # Add other half of flux data
        lengthX = len(X)
        x = (X[-1] - X[-2])
        print "length = %i" %lengthX
        for i in xrange(lengthX-1):
            dx = X[lengthX-1] - X[lengthX-(i+2)]
            X.append(X[lengthX-1] + dx + x)
            F1.append(F1[lengthX-(i+2)])
            F2.append(F2[lengthX-(i+2)])
            F3.append(F3[lengthX-(i+2)])
            FS.append(FS[lengthX-(i+2)])

        self.X = numpy.array(X)
        self.F1 = numpy.array(F1)
        self.F2 = numpy.array(F2)
        self.F3 = numpy.array(F3)
        self.FS = numpy.array(FS)


if __name__ == "__main__":
    outsn = procOutsn()
    print "Regions = %i, Quadrature = %i, Boundary Condition: %s\n"%(
            outsn.regions, outsn.quadrature, outsn.bc)

    # Print Region Data
    for region, i in zip(outsn.Regions, xrange(len(outsn.Regions))):
        print "Region %i\n\tCells: %i, delta-x = %6.4f" %(
               i+1, region['cells'], region['deltaX'])
        print "\txT1: %6.4f, vxF1: %6.4f" %(region['xT1'], region['xF1']), 
        print "xS1: %6.4f, xS12: %6.4f, xS13: %6.4f" %(
                region['xS1'], region['xS12'], region['xS13'])
        print "\txT2: %6.4f, vxF2: %6.4f" %(region['xT2'], region['xF2']), 
        print "xS2: %6.4f, xS23: %6.4f" %(region['xS2'], region['xS23'])
        print "\txT3: %6.4f, vxF3: %6.4f" %(region['xT3'], region['xF3']), 
        print "xS3: %6.4f" %(region['xS3'])

    print "Eigenvalue = %8.6f\nFlux begins at line # %i" %(
            outsn.eigenvalue, outsn.fluxListBegins)

    if len(sys.argv) > 1:
        outsn.writeFile(sys.argv[1])
    else:
        outsn.writeFile('OUTSN.dat')


