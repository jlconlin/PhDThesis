__id__       = "$Id: Arguments.py 684 2009-07-07 20:05:36Z jlconlin $"
__author__   = "$Author: jlconlin $"
__version__ = " $Revision: 684 $"
__date__     = "$Date: 2009-07-07 14:05:36 -0600 (Tue, 07 Jul 2009) $"

"""
This module will mimic the arguments used in the C++ version.  It is provided
for creating simple scripts that will call the C++ programs.
"""

import optparse

class ArgParser(optparse.OptionParser):
    """
    """

    def __init__(self, usage=None):
        if not usage:
            usage = 'usage: %prog [options] args'
        optparse.OptionParser.__init__(self,usage=usage)

        # Add a bunch of options
        self.add_option('-t', '--tracks', help='# Histories to track', 
                dest='tracks', default='50000')
        self.add_option('-i', '--inactive', help='# Inactive Cycles',
                dest='inactive', default='0')
        self.add_option('-a', '--active', help='# Active Cycles', dest='active',
                default='20')
        self.add_option('-b', '--bins', help='# spatial bins for discretization', 
                dest='bins', default='50')
        self.add_option('-w', '--width', help='Width of slab', dest='width', 
                default='20')
        self.add_option('-s', '--seed', help='Random seed', dest='seed',
                default='0')
        self.add_option('-v', '--verbose', help='The verbosity of the output',
                dest='verbose', default='2', type=str)
#       self.add_option('--tol', help='Tolerance', dest='tolerance')
        self.add_option('-f', '--filename', help='Filename for data',
                dest='filename', default='data.dat')
        self.add_option('--run', help='Run simulation', action='store_true', 
                default=False)
        self.add_option('-m', '--multimedia', action='store_false',
                default=True, help='Multi Media problem')


if __name__ == "__main__":
    print "Testing Arguments.py."

    AP = ArgParser()

    op, arg = AP.parse_args()
    print "\noptions entered:"
    for key in op.__dict__.keys():
        print "\t%s: %s" %(key, op.__dict__[key])
    

