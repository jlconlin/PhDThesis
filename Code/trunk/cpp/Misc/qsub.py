__id__       = "$Id: qsub.py 666 2009-07-01 14:34:38Z jlconlin $"
__author__   = "$Author: jlconlin $"
__version__ = " $Revision: 666 $"
__date__     = "$Date: 2009-07-01 08:34:38 -0600 (Wed, 01 Jul 2009) $"

"""
qsub.py will facilitate the creation of qsub job files and submit those jobs.
"""

import os
import optparse

class qsub(object):

    def __init__(self, Filename, Jobname, path, cmd, time=10, memory='1000mb'):
        """
        name: The name of the job as well as the output file name
        path: Directory from which to begin command
        cmd: Command to run 
        time: How many hours should be reserved
        """
        FileLines = """#! /bin/sh
# Name of job
#PBS -N %s
# Resource list
#PBS -l nodes=1:ppn=1:opt2356
#PBS -l walltime=%s:00:00
#PBS -l qos=preempt
#PBS -l pmem=%s
# Is batch requeued if terminated prematurely
#PBS -r y

# Job working directory leave---blank for current directory
#PBS -d %s
# Output file path
#PBS -o %s

# Define a queue
#PBS -q route

# email communication to:
#PBS -M jlconlin@umich.edu

# email options
#PBS -m abe

# Join output and error streams
#PBS -joe

# Export qsub environment variables to batch job
#PBS -V

#this is for helping you if a problem happens always include it
echo "I ran on: " $PBS_NODEFILE

#run job 
./%s
        """
        self.filename = os.path.abspath(Filename)
        self.path = os.path.abspath(path)

        # Need to make sure directory exists
        directory = os.path.dirname(self.filename)
        if directory == '':
            directory = self.path

        if not os.path.isdir(directory):
            os.mkdir(directory)

        Screenname = os.path.splitext(self.filename)[0] + '.screen'
        FileContents = FileLines %(Jobname, time, memory, self.path, Screenname, 
                cmd)

        self.queFile = open(self.filename, 'w')
        self.queFile.write(FileContents)
        self.queFile.close()

    def submit(self):
        """
        submit job for queuing.
        """
        os.system('qsub %s' %self.filename)


if __name__ == '__main__':
    print "q-subbing"

    parser = optparse.OptionParser()

    parser.add_option('-f', '--filename', dest='filename',
            default='data.q', help='Filename for job submission file')
    parser.add_option('--name', dest='name', default='Test', help='Jobname')
    parser.add_option('-p', dest='path', default=os.getcwd(), 
            help='Directory from whcih to begin command')
    parser.add_option('--cmd', dest='cmd', default='ls', 
            help='Which command to run')
    parser.add_option('-t', '--time', dest='jobtime', default=10,
            help='How many hours for job')
    parser.add_option('-m', '--mem', dest='memory', default='1000mb',
            help='How much memory should I request')
    parser.add_option('--sub', dest='submit', action='store_true',
            help='Submit jobs to qsub', default=False)

    options, args = parser.parse_args()
    Qsub = qsub(options.filename, options.name, options.path, 
            options.cmd, time=options.jobtime, memory=options.memory)
    if options.submit:
        Qsub.submit()
