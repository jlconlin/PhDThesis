#! /usr/local/bin/python2.4
"""
This code will calculate the eigenvalue(s) of a matrix using Arnoldi's method.
Arnoldi's method will be performed deterministically.
"""

import numpy, math, random
import Gnuplot
import ArnoldiDetermVerify

__author__   = "$Author: jlconlin $"
__version__ = " $Revision: 83 $"
__date__     = "$Date: 2007-06-14 15:35:15 -0600 (Thu, 14 Jun 2007) $"
__id__       = "$Id: DetermArnoldi.py 83 2007-06-14 21:35:15Z jlconlin $"




def main():
    """
    """
    N = 10     # The shape of the matrix A
    I = 9      # Number of Arnoldi Iterations
    R = 10     # Number of Arnoldi Repeats
    A = DiagonalMatrix(N)

    # Create starting vector
    q0 = numpy.ones(A.shape[0])
    q0 = q0/Vector2Norm(q0)

    Residual = []   # Residual after each iteration
    for i in xrange(R):
        print i
        Pairs, Q, resid = Arnoldi(A, I, q0) # Pairs are already sorted
        Residual.extend(resid)
        if not (abs(Residual[-1]) < 1e-10):
#           sPairs = EigenSort(Pairs[0], Pairs[-1])
            q0 = RestartVector(Pairs[1][-1], Q[:,:-1])
        else:
            break
    print "Residual: %s" %Residual
    ResidPlot = Gnuplot.Gnuplot(debug=0)
    ResidPlot.xlabel("Iterations")
    ResidPlot.ylabel("Residual Norm")
    ResidData = Gnuplot.Data(Residual, with="lines")
    ResidPlot.plot(ResidData)


#   X = ArnoldiDetermVerify.ArnoldiDetermVerify(Arnoldi, A, RestartVector, EigenSort, Vector2Norm)
#   X.IterationConvergence(50)
#   X.RestartConvergence(3, 10)
#   X.IterRestartConvergence(50, 10)
#   
#   N = 1000
#   A = DiagonalMatrix(N)
#   Y = ArnoldiDetermVerify.ArnoldiDetermVerify(Arnoldi, A, RestartVector, EigenSort, Vector2Norm)
#   Y.IterationConvergence(20)


def IRAM(A, I, R, q0):
    """
    IRAM is Implicitly Restarted Arnoldi Method.  It will call Arnoldi the
    appropriate number of times.

    A: an (m x n) matrix
    I: The number of Arnoldi Iterations
    R: The number of implicit restarts
    q0: The initial guess of the eigenvector
    """
    for i in xrange(R):
        Pairs, Q = Arnoldi(A, I, q0)
        sPairs = EigenSort(Pairs[0], Pairs[-1])
        q0 = RestartVector(sPairs[1][-1], Q[:,:-1])

        print "\nQ:"
        for vect in Q:
            PrintVector(vect)
        print '\n"Largest" eigenvector: ',
        PrintVector(sPairs[1][-1])
        print "\nRestart vector: ",
        PrintVector(q0)


def Arnoldi(A, N, q0):
    """
    This function performs Arnoldi iteration.  It returns estimates of the first
    N eigenpairs as well as the orthonormal basis vectors, Q.  If, during the
    iteration, the set of basis vectors becomes invariant, the iteration will
    stop.  A list of the residual norm is also returned.

    A: an (m x n) matrix
    N: The number of times to iterate
    q0: The initial guess of the eigenvector
    """
    Invariant = False
    m,n = A.shape
    Q = []
    Q.append(q0) # Initially the elements of Q are the orthormal basis vectors

    # Initialilze the Hessenberg matrix we will fill later
    H = numpy.zeros((N+1,N))

    # Take First iteration.  This is necessary because of Python's for loops
    q = VectorMatrixProduct(Q[0],A)
    H[0,0] = VectorDotProduct(Q[0],q)
    q = q-H[0,0]*Q[0]
    H[1,0] = Vector2Norm(q)
    q = q/H[1,0]
    Q.append(q)
    
    Residual = [q[-1]*H[1,0]]   # To hold residuals calculated after each iteration

    # Arnoldi's iteration
    for k in xrange(1,N):
        q = VectorMatrixProduct(Q[k],A)
        for j in xrange(k+1):
            H[j,k] = VectorDotProduct(Q[j],q)
            q = q - H[j,k]*Q[j]
        H[k+1,k] = Vector2Norm(q)
        Q.append(numpy.array(q/H[k+1,k]))

        # Calculate residual
        EigenPairs = numpy.linalg.eig(H[:k+1,:k+1]) 
        sPairs = EigenSort(EigenPairs[0], EigenPairs[-1])
        DominantVector = sPairs[-1][-1]
        DominantVector = DominantVector/Vector2Norm(DominantVector) # Normalize
        Residual.append(abs(DominantVector[-1]*H[k+1,k]))

    # Make Q a numpy array who's columns are the orthonormal basis vectors
    Q = numpy.array(Q)
    Q = numpy.transpose(Q)
    return EigenPairs, Q, Residual


def DiagonalMatrix(N):
    """
    DiagonalMatrix makes a diagonal matrix (clearly) of size N x N whose
    diagonal elements are integers, equal to the row index.

    N: Size of matrix
    """
    A = numpy.zeros((N,N))
    for i in xrange(N):
        A[i,i] = i+1

    return A


def DingDongMatrix(N):
    """
    This function will generate an (N x N) matrix whose eigenvalues cluster around
    pi/2 and -pi/2.  See:
        http://math.nist.gov/MatrixMarket/deli/DingDong/

    References:
        This generator is adapted from Nicholas J. Higham's Test Matrix Toolbox.
        J.C. Nash, Compact Numerical Methods for Computers: Linear Algebra and Function 
        Minimisation, second edition, Adam Hilger, Bristol, 1990 (Appendix 1).
    """
    A = numpy.zeros((N,N))
    for i in xrange(N):
        for j in xrange(N):
            A[i,j] = 1.0/(2*(10-i-j+(3/2.0)))
    return A


def EigenSort(Values, Vectors):
    """
    EigenSort will sort the eigenpairs according to the eigenvector.  It returns
    the sorted eigenpairs.

    Values: The eigenvalues
    Vectors: The eigenvectors
    """
    m = len(Values)     # Number of eigenpairs
    SortedIndex = Values.argsort()

    SortedValues = numpy.zeros(m)
    SortedVectors = []
    for i in xrange(m):
        SortedValues[i] = Values[SortedIndex[i]]
        SortedVectors.append(Vectors[:, SortedIndex[i]])
    SortedVectors = numpy.array(SortedVectors)
    SortedVectors = numpy.transpose(SortedVectors)
    return (SortedValues, SortedVectors)


def RestartVector(v, Q):
    """
    RestartVector will calculate the restart vector, an estimate of the dominant
        eigenvector, for Implicitly Restarted Arnoldi.  The restart vector is a
        linear combination of the orthonormal basis vectors (the columns of Q)
        and the elements of v.  This function returns q0, an estimate of the
        dominant eigenvector of A.

    v: Dominant eigenvector of H
    Q: Matrix who's columns are orthonormal 
    """
    m, n = Q.shape
    q0 = numpy.zeros(m)
    for i in xrange(n):
        q0 = q0 + v[i]*Q[:,i]
    
    q0Norm = Vector2Norm(q0)
    q0 = q0/q0Norm
    return q0


def Vector2Norm(v):
    """
    This function will return the 2-norm of a vector v.
    """
    norm = 0
    for element in v:
        norm = norm + element*element
    norm = math.sqrt(norm)
    return norm


def VectorMatrixProduct(q, A):
    """
    This function will return a 1-d array that is the product of a vector and a
    matrix.

    q: Vector of length n
    A: Matrix (array) with A.shape = (m,n)
    """
    m,n = A.shape
    c = numpy.zeros(n)

    for i in xrange(n):
        for j in xrange(m):
            c[i] = c[i] + q[j]*A[i,j]

    return c


def MatrixDotProduct(A, B):
    """
    This function will return an (m x n) array which is the Matrix Dot Product
    of A and B.  
        m: A.shape[0]
        n: B.shape[-1] = B.shape[1]

    A: Matrix (array) with A.shape = (m,p)
    B: Matrix(array) with B.shape = (p,n)
    """
    m = A.shape[0]  # rows of A
    n = B.shape[-1]  # columns of B
    C = numpy.zeros((m,n))

    for i in xrange(m):
        for j in xrange(n):
            C[i,j] = VectorDotProduct(A[i], B[:,j])
    return C


def VectorDotProduct(a,b):
    """
    This function will return a scaler which is the dot product of a.b
    a,b: vectors of the same length.  This function does not check the length.
    """
    c = 0.0
    for i in xrange(len(a)):
        c = c + a[i]*b[i]
    return c


def PrintMatrix(A):
    """
    PrintMatrix will print to the screen a matrix in a format that I like better
    than the built in method.

    A: Matrix to be printed
    """
    for row in A:
        PrintVector(row)


def PrintVector(v):
    """
    PrintVector will print to the screen a vector in a format that I like better
    than the built in method.

    v: Vector to be printed
    """
    print "[",
    for element in v:
        print "%14.6E" %element,
    print " ]"


def MatheMatrix(A):
    """
    MatheMatrix will write to the screen in a format that is compatible with
    Mathematica.
    """
    m,n = A.shape
    print "{",
    for row in A:
        print "{",
        for column in row:
            print "%f" %column, ",",
        print "},"
    print "}"

    
def mainOrig():
    N = 5   # The shape of the matrix A
    ArnoldiIterations = 5   # How many times to iterate within Arnoldi
    Restarts = 1            # How many times to restart Arnoldi
    A = DiagonalMatrix(N)
    print "A: \n%s " %A
    print "Arnoldi Iterations: %s\nArnoldi Restarts: %s" %(ArnoldiIterations, Restarts)

    AValues, AVectors = numpy.linalg.eig(A)

    sAValues, sAVectors = EigenSort(AValues, AVectors) # Sorted eigenpairs of A
    DominanceRatio = sAValues[-2]/sAValues[-1]
    print "Dominance Ratio = %.14E" %DominanceRatio
#   TrueChart = Gnuplot.Gnuplot(debug=0)    # Prepare for plotting eigenvectors
#   TrueChart("set xrange[0:20]")
#   TrueChart('set title "True eigenvectors"')
#   for i in xrange(5):
#       plot = Gnuplot.Data(sAVectors[:,-(i+1)], with="lines", title="eigVector%s" %i)
#       TrueChart.replot(plot)
#   Chart = Gnuplot.Gnuplot(debug=0)    # Prepare for plotting eigenvectors
#   Chart('set title "Estimated eigenvectors"')
    q0 = numpy.ones(N)
    q0 = q0/Vector2Norm(q0)
    domEigenEstimate = []               # Estimate of the dominant eigenvalue
#   Aplot = Gnuplot.Data(sAVectors[:,-1], with="lines", title="True eVector")
#   Vplot = Gnuplot.Data(q0, with="lines", title="q0")
#   Chart.plot(Aplot, Vplot)
    for i in xrange(Restarts):
        eigPairsH, Q = Arnoldi(A, ArnoldiIterations, q0)
        sHValues, sHVectors = EigenSort(eigPairsH[0], eigPairsH[-1])
#       print "\nsHValues: %s\nsHVectors: %s" %(sHValues, sHVectors)
        domEigenEstimate.append(sHValues[-1])
        q0 = RestartVector(sHVectors[-1], Q[:,:-1])
        q0 = q0/Vector2Norm(q0)
#       if i%10 == 0:
#           Chart.clear()
#           Chart.plot(Aplot)
#       Vplot = Gnuplot.Data(q0, with="lines", title="q%i" %(i+1))
#       Chart.replot(Vplot)
#       Chart("set xrange[0:20]")
    print "Eigenvalue Estimates: %s" %domEigenEstimate
    print "Eigenvalue convergence:\n"
    for value in domEigenEstimate:
        print "\t%E" %(1-value/sAValues[-1])


def mainTest():
    """
    mainTest is written to test the functionality of Arnoldi's method.  It
    contains much of the same stuff as main, but I didn't want to change what I
    already had.
    """
    N = 5   # The shape of the matrix A
    ArnoldiIterations = 5   # How many times to iterate within Arnoldi
    Restarts = 1            # How many times to restart Arnoldi
    A = DiagonalMatrix(N)

    AValues, AVectors = numpy.linalg.eig(A)
    sAValues, sAVectors = EigenSort(AValues, AVectors) # Sorted eigenpairs of A
    DominanceRatio = sAValues[-2]/sAValues[-1]
    print "A: \n%s " %A
    print "Arnoldi Iterations: %s\nArnoldi Restarts: %s" %(ArnoldiIterations, Restarts)
    print "Dominance Ratio = %.14E" %DominanceRatio

    # Create starting vector
    q0 = numpy.ones(N)
    q0 = q0/Vector2Norm(q0)

    eigPairsH, Q = ArnoldiTest(A, ArnoldiIterations, q0)
    sHValues, sHVectors = EigenSort(eigPairsH[0], eigPairsH[-1])
    print "eigenvalue estimates:\n%s" %sHValues

    # Get eigenvectors of A
    VectorsA = []
    print "len(sHVectors): %s" %len(sHVectors)
    print
    for i in xrange(len(sHVectors)):
        vect = sHVectors[:,i]
        vect = RestartVector(vect, Q[:,:-1])
        PrintVector(vect)
        VectorsA.append(vect)
    print

    print "\nQ:"
    for vect in Q:
        PrintVector(vect)


def Anothermain():
    """
    """
    N = 20   # The shape of the matrix A
    ArnoldiIterations = 9   # How many times to iterate within Arnoldi
    Restarts = 8            # How many times to restart Arnoldi
    A = DiagonalMatrix(N)

    AValues, AVectors = numpy.linalg.eig(A)
    sAValues, sAVectors = EigenSort(AValues, AVectors) # Sorted eigenpairs of A
    DominanceRatio = sAValues[-2]/sAValues[-1]
    print "A: \n%s " %A
    print "Arnoldi Iterations: %s\tArnoldi Restarts: %s" %(ArnoldiIterations, Restarts)
    print "Dominance Ratio = %.14E" %DominanceRatio


    # Print real eigenpairs
    print "\neigenvalues:\n%s" %sAValues
    print "\neigenvectors:"
    for vect in sAVectors:
        PrintVector(vect)

    # Create starting vector
    q0 = numpy.ones(N)
    q0 = q0/Vector2Norm(q0)

    for i in xrange(Restarts):
        Pairs, Q = Arnoldi(A, ArnoldiIterations, q0)
        sPairs = EigenSort(Pairs[0], Pairs[-1])
        q0 = RestartVector(sPairs[1][-1], Q[:,:-1])

    # Print Orthonormal basis
    print "\nQ:"
    for vect in Q:
        PrintVector(vect)

    # Print eigenpair estimates
    print "\neigenvalue estimates:\n%s" %sPairs[0]
    print "\neigenvector estimates:"
    for i in xrange(len(sPairs[1])):
        vect = sPairs[1][:,i]
        vect = RestartVector(vect, Q[:,:-1])
        PrintVector(vect)
    

def ArnoldiTest(A, N, q0):
    """
    ArnoldiTest is the same as Arnoldi, but it print more to the screen as I am
    trying to debug it.
    This function performs Arnoldi iteration.  It returns estimates of the first
    N eigenpairs as well as the orthonormal basis vectors, Q.  

    A: an (m x n) matrix
    N: The number of times to iterate
    q0: The initial guess of the eigenvector
    """
    m,n = A.shape
    Q = []
    Q.append(q0) # Initially the elements of Q are the orthormal basis vectors
    print "q0:\t\t",
    PrintVector(q0)

    # Initialilze the Hessenberg matrix we will fill later
    H = numpy.zeros((N+1,N))

    q = VectorMatrixProduct(Q[0],A)
    print "V.M Prod q:\t",
    PrintVector(q)
    H[0,0] = VectorDotProduct(Q[0],q)
    q = q - H[0,0]*Q[0]
    print "q:\t\t",
    PrintVector(q)
    H[1,0] = Vector2Norm(q)
    q = q/H[1,0]
    print "Normalized q:\t",
    PrintVector(q)
    Q.append(q)
    # Arnoldi's iteration
    for k in xrange(1,N):
        print "\nk = %s" %k
        q = VectorMatrixProduct(Q[k],A)
        print "V.M Prod q:\t",
        PrintVector(q)
        for j in xrange(k+1):
            H[j,k] = VectorDotProduct(Q[j],q)
            q = q - H[j,k]*Q[j]
            print "j = %s q:\t" %j,
            PrintVector(q)
        H[k+1,k] = Vector2Norm(q)
        q = q/H[k+1,k]
        print "Normalized q:\t",
        PrintVector(q)
        Q.append(numpy.array(q))

    # Make Q a matrix who's columns are the orthonormal basis vectors
    Q = numpy.array(Q)
    Q = numpy.transpose(Q)

    print "\nQ:"
    for vector in Q:
        PrintVector(vector)
    print "\nH:"
    for vector in H:
        PrintVector(vector)

    # Get eigenvalues (l) and eigenvectors (v) of H
    EigenPairs = numpy.linalg.eig(H[:-1]) 
    return EigenPairs, Q


main()
# mainTest()
