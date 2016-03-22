#! /usr/local/bin/python
"""
This code will calculate the dominant eigenvalue of a Monte Carlo
Simulation using Arnoldi's iteration method.  
"""

import sys
import os
import math
import random
import optparse

__author__   = "$Author: jlconlin $"
__version__ = " $Revision: 83 $"
__date__     = "$Date: 2007-06-14 15:35:15 -0600 (Thu, 14 Jun 2007) $"
__id__       = "$Id: MonteArnoldi.py 83 2007-06-14 21:35:15Z jlconlin $"


def main():
    AddSystemPath('/Users/jlconlin/Documents/Education/Convergence/Code/python/SourceFiles')
    import Particle
    import Transport
    xs, Histories, Width = Parse()
    Neutron = Particle.Particle(Position = (1,1,1))
#   print "Neutron Position: %r" %(Neutron.Position,)


    import Arnoldi
    Transporter = Transport.Transport(Throw, xs, Width)
    first = Arnoldi.Arnoldi(Histories, Transporter, 3)
#   first.Transport(Neutron,7)
    first.Iterate()
#   first.SolveBoltzman(3)

        
def Throw(Particle, Location=25):
    print "I am in Throw with Position = %s" %(Particle.Position,)

def Parse():
    """
    This function will use the optparse module to identify the command line
    arguments.  
    """
    # Version number and last modification
    Version = '0.1'
    Modified = 'November 1, 2006'

    # Prepare parser
    usage = 'usage: %prog [options]'
    parser = optparse.OptionParser(usage=usage, version = 'Version #: %s' %Version)
    parser.add_option("-H", "--Histories", action="store", default=10,
            dest="Histories", help="Number of histories to run")
    parser.add_option("-S", "--Scatter", action="store", default=1.0, 
            dest="xS", help="Scattering cross section")
    parser.add_option("-F", "--Fission", action="store", default=2.5,
            dest="xF", help="Fission cross section")
    parser.add_option("-n", "--nu", action="store", default=2.5,
            dest="nu", help="mean number of fission neutrons")
    parser.add_option("-G", "--Capture", action="store", default=2.5,
            dest="xG", help="Capture cross section")
    parser.add_option("-X", "--Width", action="store", default=10.0,
            dest="Width", help="Slab geometry width")

    # Parse output to see what the user wants to do
    (options, args) = parser.parse_args()

    # Cross section dictionary
    xs = {}
    xs['S'] = options.xS
    xs['F'] = options.xF
    xs['G'] = options.xG
    xs['nu'] = options.nu

    return xs, int(options.Histories), options.Width


def AddSystemPath(Directory):
    if os.path.isdir(Directory):
        sys.path.append(Directory)


main()
