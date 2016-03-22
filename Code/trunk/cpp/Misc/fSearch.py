
import re
import sys
import math
import numpy

class bcolors:
   """allows colorful text on terminal"""
   HEADER = '\033[95m'
   OKBLUE = '\033[94m'
   OKGREEN = '\033[92m'
   WARNING = '\033[93m'
   FAIL = '\033[91m'
   ENDC = '\033[0m'

   def disable(self):
       self.HEADER = ''
       self.OKBLUE = ''
       self.OKGREEN = ''
       self.WARNING = ''
       self.FAIL = ''
       self.ENDC = ''

class fSearch:
    """General postprocessing class. Made to post process output files.

    Argument: filename

    Methods:
    reset - resets the iterative file, seeks to 0
    fsearch(pattern, msg) - searches file for pattern, returns line, prints msg

    Constants:
    scipat - scientific number
    """
    scipat = '[+-]?\d*\.\d+[eEdD][+-]\d+' # Scientific number
    number = '[+-]?\d+\.*\d*'         # matches 1, 100, 1.0, -1.2, +12.2342

    def __init__(self,fname):
        try:
            self.f=open(fname)
        except IOError:
            print "ERROR: cannot open %s " % fname
            raise
        self.fname=fname

    def __repr__(self):
        line = 'Post Processing wrapper for file: ' + self.f.name
        return line

    def reset(self):
        """Resets file"""
        self.f.seek(0)

    def close(self):
        self.f.close()

    def fsearch(self,pattern,msg=None, killon=None ):
        """Searches file f for pattern and displays msg when found. Returns line 
        in which pattern is found or FALSE if no pattern is found."""
        import re
        current=0
        cpat=re.compile(pattern)

        for num,line in enumerate(self.f):
            if killon:
                kill = re.search(killon,line)
                if kill:
                    # the kill phrase was found first, so die. 
                    return False
            current=re.search(cpat,line)
            if current:
                if msg:
                    print msg
                break
        if not current:
#           print 'ERROR: Requested pattern ('+pattern+') not found in file: <'+self.f.name+ '>. Check file for correct structure. Exiting...'
            return False

        return line


if __name__ == "__main__":

    filename = sys.argv[1]

    F = fSearch(filename)
    print "%s" %F

    expression = '^Restart \#'
    found = True
    while found:
        found = F.fsearch(expression)
        print found
