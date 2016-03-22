__id__       = "$Id: ArnoldiArgs.py 584 2009-06-04 18:07:25Z jlconlin $"
__author__   = "$Author: jlconlin $"
__version__ = " $Revision: 584 $"
__date__     = "$Date: 2009-06-04 12:07:25 -0600 (Thu, 04 Jun 2009) $"

"""
This module will mimic the arguments used in the C++ version.  It is provided
for creating simple scripts that will call the C++ ArnoldiResearch program
"""

import sys
import os
import optparse

# Add path so Python can find my ArgParser class
p = os.path.join(os.path.expanduser('~'),'Code/Misc')
try:
    sys.path.index(p)
except ValueError:
    sys.path.append(p)
import Arguments


class ArnoldiParser(Arguments.ArgParser):
    """
    ArnoldiParser contains commands to obtain arguements specific to running
    ArnoldiResearch
    """

    def __init__(self, usage=None):
        if not usage:
            usage = 'usage: %prog [options] args'

        Arguments.ArgParser.__init__(self, usage)

        # Add arguments specific to Arnoldi's method
        ArnoldiGroup = optparse.OptionGroup(self, "Arnoldi Options",
                "These options are specific to Arnoldi's method")
        ArnoldiGroup.add_option('-x', '--relaxed', help='Relaxation parameter',
                dest='relaxed', default='0')
        ArnoldiGroup.add_option('-n', '--eigenvalues', 
                help='# Desired eigenvalues', dest='eigenvalues', default='2')
        ArnoldiGroup.add_option('-I', '--Iterations', 
                help='# Iterations per Restart', dest='Iterations', default='10')
        ArnoldiGroup.add_option('-c', '--source', help='Fission Source Type',
                dest='source', default='Histogram')
        self.add_option_group(ArnoldiGroup)

if __name__ == "__main__":
    print "Testing ArnoldiArgs.py."

    AP = ArnoldiParser()

    op, arg = AP.parse_args()
    print "\noptions entered:"
    for key in op.__dict__.keys():
        print "\t%s: %s" %(key, op.__dict__[key])
    
