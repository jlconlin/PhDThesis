__id__       = "$Id: NumBias.py 146 2007-08-24 14:56:05Z jlconlin $"
__author__   = "$Author: jlconlin $"
__version__  = "$Revision: 146 $"
__date__     = "$Date: 2007-08-24 08:56:05 -0600 (Fri, 24 Aug 2007) $"

"""This module calculates the bias in the eigenvalue from a noisy/inexct
Arnoldi's method.  It calculates the eigenvalue many times and takes an average."""

import sys
import cmath
import time
import optparse
import string

import scipy
import scipy.linalg

import arnoldiDTM
import Matrix
import gnuplotFile

class NumBias(arnoldiDTM.arnoldiDTM):
    """
    """

    def __init__(self, A, numValues, Noise, NoiseShape='normal', storeVectors=False,
            verbose=False):
        """
        A: Matrix
        numValues: How many eigenvalues to calculate the bias for.
        Noise: The amount of noise added to the matrix-vector product Aq
        NoiseShape: The shape of the pdf of the noise
        storeVectors: Whether the dominant eigenvector should be stored between
            iterations
        """
        arnoldiDTM.arnoldiDTM.__init__(self, A, Noise, NoiseShape,
                storeVectors, verbose)
        self.numValues = numValues
        self.trueValues, self.trueVectors = self._EigenPairs(self.A)

    
    def Mean(self, q, I, Trials=0, Tolerance=0.0):
        """
        Mean will perform Arnoldi's method many times and returns the mean and
        standard deviation of the calculated dominant eigenvalue.  It can be set to run a
        certain number of trials or until the standard deviation is within a
        specific fraction of the eigenvalue.  If both are given, it will perform
        trials until either the Trials are maxed out or until the Tolerance is
        met.

        q: Initial guess for Arnoldi's method.  If q = 'random', a random vector
            will be picked each time.
        I: Number of Arnoldi iterations for each trial.
        Trials: How many Arnoldi trials should be averaged together.
        Tolerance: Fraction of dominant eigenvalue.
        """

        self.m = scipy.zeros(self.numValues)
        self.sd = scipy.zeros(self.numValues)
        self.biasSum = scipy.zeros(self.numValues)
        self.biasSumSquared = scipy.zeros(self.numValues)
        self.startTime = time.clock()
        if Trials > 0:
            for self.i in xrange(int(Trials)):
                if self.i%1E3 == 0.0:
                    dt = time.clock()-self.startTime
                    print "Noise: %5.2f, Trials: %.1E, l-0: %6.4G, sd-0: %6.4G time: %.2f" %(
                            self.Noise, float(self.i), self.m[-1], self.sd[-1],
                            dt)
                self._Trial(q, I)
                if Tolerance > 0.0 and abs(self.sd).all() > 0.0:
                    if (abs(self.sd) < abs(Tolerance*self.m)).all():
                        print "\tTrials = %i" %self.i
                        break
        elif Tolerance > 0.0:
            self.i = 0
            while (abs(self.sd) >= abs(Tolerance*self.m)).any():
                self.i += 1
                self._Trial(q, I)
        else:
            raise ValueError, "Must specify either Trials and/or Tolerance"

        self.sE = self.sd*cmath.sqrt(1.0/(self.i+1))

    
    def _Trial(self, q, I):
        """
        _Trial will peform one trial of Arnoldi's method and calculate the new
        mean and standard deviation.

        q: Initial guess for Arnoldi's method.  If q = 'random', a random vector
            will be picked each time.
        I: Number of Arnoldi iterations for each trial.
        """

        if q == 'random':
            initial = scipy.rand(len(self.A))
        else:
            initial  = q
        self.Values, Vectors = self.arnoldi(initial, I)
        bias = self.Values[-self.numValues:] - self.trueValues[-self.numValues:]
#       print "val = %s, bias = %s" %(self.Values[-1], bias)
        self.biasSum += bias.real
        self.biasSumSquared += bias.real**2
        self.m = self.biasSum/(self.i+1)
        self.sd = scipy.sqrt(1.0/(self.i+1)*(self.biasSumSquared/(self.i+1) - self.m**2))
        self.endTime = time.clock()


class ManyBias(object):
    """
    ManyBias calculates the numerical bias (using NumBias) for various standard
    deviations.  
    """
    
    def __init__(self, filename, A, sd=0, T=1E5, Tol=0.1, values=1):
        """
        filename: Filename you want data written to
        A: Matrix
        sd: List of standard deviations to consider
        T: Number of Trials to perform
        Tol: Acceptable tolerance in eigenvalue estimate
        values: How many eigenvalue estimates to keep track of
        """
        if not filename:
            raise ValueError("""

            It's a waste of time to perform this calculation without writing the
            data to a file.""")
        self.filename = filename
        self.A = A
        self.T = T
        self.Tol= Tol
        self.values = values

        if sd == 0:
            self.sd = [0.01, 0.1, 0.25, 0.5, 0.75, 1.0, 1.5, 2.0, 5.0, 7.5, 10.0]
        else:
            self.sd = sd


    def CalcBias(self, q, I):
        """
        CalcBias will calculate the many bias' using NumBias

        q: Starting vector for Arnoldi's Method
        I: The number of Arnoldi Iterations
        """
        self.Means = []
        self.StdDev = []
        self.sigmaE = []
        start = time.time()
        for dev in self.sd:
            if q == 'random': initial = scipy.rand(len(A))
            else: initial = q

            self.nb = NumBias(self.A, self.values, Noise=dev, verbose=False)
            self.nb.Mean(initial, I, Trials=self.T, Tolerance=self.Tol)
            self.Means.append(self.nb.m.real)
            self.StdDev.append(self.nb.sd.real)
            self.sigmaE.append(self.nb.sE.real)
            end=time.time()
            print "sd: %s, Mean = %s, StdDev = %s, time = %6.2f sec" %(dev, 
                    self.Means[-1], self.StdDev[-1], (end-start))

        self.Means = scipy.array(self.Means)
        self.StdDev = scipy.array(self.StdDev)
        self.sigmaE = scipy.array(self.sigmaE)

    
    def WriteBias(self):
        """
        """
        self.gnuData = {}
        for i in xrange(len(self.Means[0,:])):
            self.gnuData['sigmaE-%i' %i] = (self.sd, self.sigmaE[:,i])
            self.gnuData['value-%i' %i] = (self.sd, self.Means[:,i], self.StdDev[:,i])

        gnuFile = gnuplotFile.gnuplotFile(self.filename, self.gnuData)


class NB(optparse.OptionParser):

    def __init__(self, usage=None, version=None):
        """
        __init__ will create all the necessary information you need to run the code.
        It parses the command line to understand all your options.
        """
        if not usage:
            usage = 'usage: %prog [options] args'
        optparse.OptionParser.__init__(self, usage=usage, version=version)
        self.add_option("-f", "--file", dest="filename", type='string', 
                help="gnuplot data filename")
        self.add_option("-A", "--matrix_form", dest='MatrixForm', type='string',
                default="diagonal",
                help="What kind of matrix.  Currently only supports diagonal.")
        self.add_option("-z", "--size", dest="size", type='int', default=10,
                help="Size of matrix.")
        self.add_option("-T", "--trials", dest="Trials", type="float", default=1E5,
                help="How many independent trials to calculate bias.")
        self.add_option("-s", "--sd", dest="sd", type='string', default='0',
                help="What noise standard deviations will be used.")
        self.add_option("-t", "--tol", dest="Tol", type="float", default=0.1,
                help="Accepted tolerance in eigenvalue bias standard deviation.")
        self.add_option("-v", "--values", dest="eigenvalues", type="int",
                default=1, help="How many eigenvalues calculated.")
        self.add_option("-I", "--iterations", dest="I", type="int",
                default=10, help="How many Arnoldi Iterations per trial.")
        self.add_option("-q", dest="q", type="string", 
                default="scipy.ones(self.options.size)", 
                help="Initial q.")
        self.add_option("-r", "--run", dest="run", action="store_true", default=False,
                help="Perform calculation.")

        # Parse command line options
        self.options, self.args = self.parse_args()

        if string.lower(self.options.MatrixForm) == 'diagonal':
            self.A = Matrix.Diagonal(scipy.arange(self.options.size, 0, -1))
        elif isinstance(eval(self.options.MatrixForm), scipy.ndarray):
            self.A = eval(self.options.MatrixForm)
            self.options.size = self.A.shape
        else:
            raise ValueError, """Only Diagonal matrices or scipy.ndarray objects
                are supported"""


    def printParameters(self):
        """
        printParameters will print the necessary information about the job to be
        submitted.
        """
        print "\nA: \n%s\nq:\n%s\nsd: %s" %(self.A,eval(self.options.q), 
                eval(self.options.sd))
        print "Size: %s, filename: %s" %(self.options.size, 
                self.options.filename)
        print "Trials = %.2E, Tolerance = %6.4f" %(self.options.Trials, self.options.Tol)
        print "Iterations = %i, NumValues = %i" %(self.options.I, self.options.eigenvalues)


    def run(self, A=None, q=None):
        """
        run will start the calculation.
        """
        try:
            if not A:
                A = self.A
            if not q:
                q = eval(self.options.q)
        except ValueError: print "A and q 'must' be a scipy arrays'"

        print "\n\nRunning job...\n\n"
        mb = ManyBias(self.options.filename, A, eval(self.options.sd), self.options.Trials,
                self.options.Tol, self.options.eigenvalues)
        mb.CalcBias(q, self.options.I)
        mb.WriteBias()


if __name__ == "__main__":
    usage = 'usage: %prog [options] args'
    runner = NB(usage=usage, version=str(__version__))
    runner.printParameters()
    if runner.options.run:
        runner.run()
    
    

