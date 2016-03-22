__id__       = "$Id: Many.py 682 2009-07-07 13:32:51Z jlconlin $"
__author__   = "$Author: jlconlin $"
__version__ = " $Revision: 682 $"
__date__     = "$Date: 2009-07-07 07:32:51 -0600 (Tue, 07 Jul 2009) $"

"""
This module facilitates running ArnoldivsPower many times and combining the
results.
"""

import re
import sys
import os
import string
import subprocess
import numpy
import optparse

# Add path so Python can find my qsub class
p = os.path.join(os.path.expanduser('~'),'Code/Misc')
try:
    sys.path.index(p)
except ValueError:
    sys.path.append(p)
import qsub



class Many(object):
    def __init__(self, command):
        """
        N: number of times to run 
        command: what system command to run
        """
        self.command = command
        object.__init__(self)

    def run(self, args):
        if args.qsub:   # Make job files
            p = re.compile('-f\ (.*?)\ ', re.VERBOSE)

            # Get output filename
            found = p.search(args.cmd)  
            datFile = os.path.abspath(found.group(1))   
            root, ext = os.path.splitext(datFile)

            qFiles = []
            for n in xrange(int(args.N)):
                dFile = root + '-%i' %n + ext
                qFile = root + '-%i' %n + '.q'
                jobname = args.name + '-%i' %n
                qFiles.append(qFile)

                # Find and replace command to update output filename
                cmd = p.sub('-f %s ' %dFile, args.cmd)

                Q = qsub.qsub(qFile, jobname, os.getcwd(), cmd, args.jobtime,
                        args.memory)
                if args.submit:     # Submit jobs
                    Q.submit()


if __name__ == "__main__":
    
    parser = optparse.OptionParser()
    parser.add_option('-N', '--number', dest='N',
            default=100, help='Number of independent trials.')
    parser.add_option('--name', dest='name', default='Test', help='Jobname')
    parser.add_option('-p', dest='path', default=os.getcwd(), 
            help='Directory from whcih to begin command')
    parser.add_option('--cmd', dest='cmd', default='ls', 
            help='Which command to run')
    parser.add_option('-t', '--time', dest='jobtime', default=10,
            help='How many hours for job')
    parser.add_option('-m', '--mem', dest='memory', default='1000mb',
            help='How much memory should I request')
    parser.add_option('-q', '--qsub', dest='qsub', action='store_true', 
            help='Create files for submitting to qsub', default=False)
    parser.add_option('--sub', dest='submit', action='store_true',
            help='Submit jobs to qsub', default=False)

    options, args = parser.parse_args()
    print "\noptions entered:"
    for key in options.__dict__.keys():
        print "\t%s: %s" %(key, options.__dict__[key])
    print

    M = Many(options.cmd)
    M.run(options)
