__id__       = "$Id: ArnoldiMC.py 248 2008-01-18 14:18:25Z jlconlin $"
__author__   = "$Author: jlconlin $"
__version__ = " $Revision: 248 $"
__date__     = "$Date: 2008-01-18 07:18:25 -0700 (Fri, 18 Jan 2008) $"

import time
import scipy

import Transport.LinearAlgebra.Arnoldi 
import Transport.Markov.ArnoldiMCBase
import Transport.FissionSource.HistSource

class ArnoldiMC(Transport.LinearAlgebra.Arnoldi.Arnoldi, 
        Transport.Markov.ArnoldiMCBase.ArnoldiMCBase):
    """
    ArnoldiMC is the Monte Carlo particle transport implementation of Arnoldi's
    method for solving eigenvalues.
    """
    
    def __init__(self, seed, F, verbose = True, resTol = 1E-10, invTol = 1E-12):
        """
        seed: Random number generator seed
        F: Field.FieldZoneMat where we are transporting Particles
        """
        self._Mesh = F.getMesh()
        self._seed = seed
        Transport.LinearAlgebra.Arnoldi.Arnoldi.__init__(
                self, verbose = verbose, resTol = resTol, invTol = invTol)
        Transport.Markov.ArnoldiMCBase.ArnoldiMCBase.__init__(self, seed, F)


    def arnoldi(self, q, I, N):
        """
        N: Number of Histories
        """
        self.histories = N
        Transport.LinearAlgebra.Arnoldi.Arnoldi.arnoldi(self, q, I)


    def Aq(self):
        """
        """
        tmp = Transport.Markov.ArnoldiMCBase.DVector(self.Q[-1])
        newHistSource = Transport.FissionSource.HistSource.HistSource(
                self._seed, self._Mesh, tmp)
        nextSource = self.Markov_Transport(newHistSource, self.histories)
        print "PDF: %s" %(nextSource.getPDF(),)
        return nextSource.getPDF()


    def Restart(self, q, I, NEig):
        """
        """
        pass


    def Iteration(self):
        """
        """
        self.IterationBase()
        self.TotalTime = time.time() - self.StartTime
        print "\tIteration #: %3i, time, %7.3f sec" %(self.k, self.TotalTime)



if __name__ == "__main__":
    import sys
    N = int(sys.argv[1]) # Number of particles
    I = int(sys.argv[2]) # Number of Iterations
    
    numZones = 10
    ZoneLengths = [1.0]*10
    # Mesh with specified non-uniform ZoneLengths
    myMesh = Transport.Markov.ArnoldiMCBase.Mesh(ZoneLengths)  

    Mat1 = Transport.Markov.ArnoldiMCBase.Material(0.5, 0.5, 0.0, 1.0)  # 'Standard' material
    Mat2 = Transport.Markov.ArnoldiMCBase.Material(0.1, 0.9, 0.0, 3.0)  # Highly fissioning material
    
    myMaterial = [Mat1 for i in xrange(numZones)]
#   myMaterial[2] = Mat2    # Heterogeneous
#   myMaterial[3] = Mat2
#   myMaterial[7] = Mat2
#   myMaterial[6] = Mat2
    vecMats = Transport.Markov.ArnoldiMCBase.vectormats(myMaterial)

    print "Testing Monte Carlo Arnoldi's method."
    # print "Now I define source probabilities for each Zone."
    vecProbabilities = Transport.Markov.ArnoldiMCBase.DVector([1.0 for i in xrange(numZones)])

    F = Transport.Markov.ArnoldiMCBase.FieldZoneMat(myMesh, vecMats)
    ProbField = Transport.Markov.ArnoldiMCBase.FieldZoneDouble(myMesh, vecProbabilities)

    s = 1
    seed = Transport.Markov.ArnoldiMCBase.ULongVector(s)
    print "Not so random seed: %s" %(s)

    q = scipy.ones(numZones)    # Starting vector
    print "Starting vector:\n%s\n" %(q)

    arnMC = ArnoldiMC(seed, F, verbose = True)
    Values, Vectors = arnMC.arnoldi(q, I, N)

