__id__       = "$Id: Study.py 686 2009-07-07 20:46:11Z jlconlin $"
__author__   = "$Author: jlconlin $"
__version__ = " $Revision: 686 $"
__date__     = "$Date: 2009-07-07 14:46:11 -0600 (Tue, 07 Jul 2009) $"

"""
Study.py will facilitate the study of the effects of changing various 
parameters.
"""

import sys
import os
import subprocess
import optparse
import time

# Add path so Python can find my qsub class
p = os.path.join(os.path.expanduser('~'),'Code/Misc')
try:
    sys.path.index(p)
except ValueError:
    sys.path.append(p)
import qsub

# Add path so Python can find my ArgParser class
p = os.path.join(os.path.expanduser('~'),'Code/Research')
try:
    sys.path.index(p)
except ValueError:
    sys.path.append(p)
import ArnoldiArgs

def ParametricStudy(options, args):
    """
    ParametricStudy is the main function for this module.  It accepts the 
    options and arguments passed from the command-line (and the defaults) and 
    will create the necessary calls to ArnoldivsPower.
    """
    optD = options.__dict__
    run = optD.pop('run')
    submit = optD.pop('submit')
    queuesub = optD.pop('qsub')
    jobtime = optD.pop('jobtime')
    memory = optD.pop('memory')
    command = optD.pop('cmd')

    # Remove options if power method
    if command == '../PowerResearch ':
        optD.pop('Iterations')
        optD.pop('eigenvalues')
        optD.pop('relaxed')
        optD.pop('source')

    # Need to make sure Directory exists
    Directory = optD.pop('Directory')
    if not os.path.isdir(Directory):
        os.mkdir(Directory)
    filename = optD.pop('filename')
    filepath = os.path.splitext(os.path.join(Directory, filename))


    for key in optD.keys():
        try:
            print key, optD[key]
            s = eval(str(optD[key]))
            if isinstance(s, list): # See if it's a list
                optD.pop(key)
                parameters = (key, s)
            elif isinstance(s, bool):   
                # All my boolean options are off by default
                # Need special handling
                if not s:
                    command += '--%s ' %key
            else:
                # Add option to command
                command += '--%s %s ' %(key, optD[key])
        except NameError: command += '--%s %s ' %(key, optD[key])


    # Run commands
    for param, i in zip(parameters[1], xrange(len(parameters[1]))):
        # Make filename
        f = '%s.%s%s%s' %(filepath[0], parameters[0][0], param, filepath[1])
        cmd = command + '--run --%s %s -f %s' %(parameters[0], param, f)

        sys.stdout.write("\n\x1b[31m%s\x1b[m\n" %time.asctime())
        print "\t%s" %cmd
        
        outFilename = os.path.splitext(filename)[0] + '.screen'
        outFilepath = os.path.join(Directory, outFilename)

        # Prepare files for submission
        jobname = '%s%s' %(parameters[0][0], param)
        queueFilename = os.path.splitext(outFilename)[0] + '.%s.q' %jobname
        queueFilepath = os.path.join(os.path.abspath(Directory), queueFilename)
        if queuesub:
#           WriteJobFile(cmd, queueFilepath, jobname, outFilename, jobtime)
            Qsub = qsub.qsub(queueFilepath, jobname, os.getcwd(), cmd, jobtime,
                    memory)
            if submit:
                print 
                Qsub.submit()

        if run:
            p = subprocess.Popen(cmd , shell=True, 
                    stdout=subprocess.PIPE, stderr = subprocess.STDOUT)
            s = p.communicate()
            # Open file called f.screen, f is output filename without extension
            oScreen = open(outFilepath, 'w')
            oScreen.writelines(s[0])
            oScreen.close()


if __name__ == '__main__':
    print "Studying parameters."

    AP = ArnoldiArgs.ArnoldiParser()
    ParameterGroup = optparse.OptionGroup(AP, "Parameter Study",
            "These options are specific to the parameter study.")
    ParameterGroup.add_option('--cmd', dest='cmd', default='../ArnoldiResearch ',
            help='Which command to run')
    ParameterGroup.add_option('-d', '--directory', dest='Directory', 
            default='tmp', help='Which directory used to store results.')
    ParameterGroup.add_option('--collect', dest='collect', action='store_true',
            default=False)
    AP.add_option_group(ParameterGroup)

    QS = optparse.OptionGroup(AP, "Parameter Study",
            "These options are specific to running/submitting on the cluster.")
    QS.add_option('--mem', dest='memory', default='1000mb',
            help='How much memory should I request')
    QS.add_option('-q', '--qsub', dest='qsub', action='store_true', 
            help='Create files for submitting to qsub', default=False)
    QS.add_option('--sub', dest='submit', action='store_true',
            help='Submit jobs to qsub', default=False)
    QS.add_option('--time', dest='jobtime', default='5',
            help='How much time (in hours) to give to batch jobs')
    AP.add_option_group(QS)

    options, args = AP.parse_args()
    print "\noptions entered:"
    for key in options.__dict__.keys():
        print "\t%s: %s" %(key, options.__dict__[key])
    print

    if options.collect:     # Collect results
        print "\nCollecting..."
    else:
        options.__dict__.pop('collect')     # Don't need this anymore
        ParametricStudy(options, args)

