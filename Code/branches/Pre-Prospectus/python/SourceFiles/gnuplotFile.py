__id__       = "$Id: gnuplotFile.py 160 2007-09-24 13:43:04Z jlconlin $"
__author__   = "$Author: jlconlin $"
__version__  = "$Revision: 160 $"
__date__     = "$Date: 2007-09-24 07:43:04 -0600 (Mon, 24 Sep 2007) $"

"""This module will write data to a data file suitable for plotting with 
Gnuplot.  It doesn't do the plotting."""

import time
import sys
import os
import string

class gnuplotFile(object):

    def __init__ (self, filename, data, header=None):
        """
        filename:  Name of file you want to write to
        data: dictionary of data you want written.  dictionary values must be
            a tuple of x and y values.
        header: String of extra information to add to Header section
        """
        self.data = data
        if not header:
            header = ''
        self._prepareLines(header)
        self._writeFile(filename)
        object.__init__(self)


    def _prepareLines(self, header):
        """
        _prepareLines will prepare the lines which will be written to the file.

        data: Data that will be written to a file.
        header: String of extra information to add to Header section
        """
        self.Lines = []
        Header = """# This file was created by gnuplotFile.py version %s on %s.  
# This file is written with the intent of being used to plot the data with
# Gnuplot.  It contains no information regarding how the plot should be
# formatted.
# =============================================================================
#
# %s$ %s
# %s
# =============================================================================
# """  %(__version__, 
        time.asctime(), os.uname()[1], string.join(sys.argv), header)
        self.Lines.append(Header)
        
        if not isinstance(self.data, dict):
            raise TypeError, "Data must be in dictionary type."
        
        keys = self.data.keys()
        keys.sort(cmp=self.sortKeys)
        datalines = []
        nline = '# '
        tline = '# '
        cline = '# '
        counter = 1

        # Prepare Column Headers
        for key in keys:
            if len(self.data[key]) == 3:
                nline = nline + '  %-12i %-12i %-12i' %(counter, counter+1,
                        counter+2)
                tline = tline + "|%s|" %self.pad(key)
                cline = cline + ' %-12s %-12s %-12s ' %('x-coord', 'y-coord', 'error')
                counter +=3
            elif len(self.data[key]) == 2:
                nline = nline + '   %-12i %-12i' %(counter, counter+1)
                tline = tline + "|%s|" %self.pad(key)
                cline = cline + ' %-12s %-12s ' %('x-coord', 'y-coord')
                counter +=2
            else:
                raise TypeError("Data must be tuples of length 2 or 3.")

        # Prepare Data lines
        for i in xrange(self.Longest()):
            line = "  "
            for key in keys:
                if len(self.data[key][0]) > i:
                    if len(self.data[key]) == 3:
                        line += ' %-12.5G %-12.5G %-12.5G ' %(
                                self.data[key][0][i], self.data[key][1][i], self.data[key][2][i])
                    elif len(self.data[key]) == 2:
                        line += ' %-12.5G %-12.5G ' %(
                                self.data[key][0][i], self.data[key][1][i])
                else:       # Data list doesn't contain this many elements
                    line += ' '*(12*len(self.data[key]) + len(self.data[key]) + 1)
            datalines.append(line)


        self.Lines.append(nline)
        self.Lines.append(tline)
        self.Lines.append(cline)
        self.Lines.extend(datalines)

    
    def sortKeys(self, k1, k2):
        """
        sortKeys is used to sort the data elements.
        """
        if len(self.data[k1][0]) < len(self.data[k2][0]):
            return 1
        elif len(self.data[k2][0]) < len(self.data[k1][0]):
            return -1
        else:
            return 0


    def Longest(self):
        """
        Longest will determine which data element has the largest length.  It
        returns this length.
        """
        longest = 0
        for key in self.data:
            if len(self.data[key][0]) > longest:
                longest = len(self.data[key][0])
        return longest

    
    def pad(self, key):
        """
        pad will return a padded string for column headers
        """
        l = 12*len(self.data[key]) + len(self.data[key]) - 1
        sl = len(key)
        diff = l-sl
        left = '-'*(diff/2)
        right = '-'*(diff/2 + diff%2)
        return '%s%s%s' %(left, key, right)


    def _writeFile(self, filename):
        """
        _writeFile will do the actual writing of the data to the file.

        filename: Name of file to be written.
        """
        outfile = open(filename, 'w')
        for line in self.Lines:
            outfile.write(line + '\n')
        outfile.close()



if __name__ == "__main__":
    import scipy
    import sys

    data = {}
    x1 = scipy.rand(10)
    y1 = scipy.rand(10)
    e1 = scipy.arange(10)
    data['first'] = (x1,y1,e1)

    x2 = scipy.rand(15)
    y2 = scipy.rand(15)
    data['second'] = (x2,y2)

    x3 = scipy.arange(20)
    y3 = scipy.rand(20)
    e3 = scipy.rand(20)
    data['third'] = (x3,y3,e3)

    x4 = scipy.arange(10)
    y4 = scipy.arange(10,0,-1)
    e4 = scipy.ones(10)
    data['fourth'] = (x4, y4, e4)

    gnuFile = gnuplotFile(sys.argv[1], data)

