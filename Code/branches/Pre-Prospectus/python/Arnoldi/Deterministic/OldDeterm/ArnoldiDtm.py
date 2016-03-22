__id__       = "$Id: ArnoldiDtm.py 83 2007-06-14 21:35:15Z jlconlin $"
__author__   = "$Author: jlconlin $"
__version__ = " $Revision: 83 $"
__date__     = "$Date: 2007-06-14 15:35:15 -0600 (Thu, 14 Jun 2007) $"

import numpy
import math
import Gnuplot



class Arnoldi(object):
    """
    ArnoldiDtm will implement Arnoldi's method and contain all the supporting
    methods so Arnoldi's method will be self contained.
    """

    def __init__(self, A, I, R, debug=0):
        """
        A: Matrix to calculate eigenvalues of
        I: Number of Arnoldi Iterations
        R: Number of Arnoldi Restarts
        debug: Value to determine the verbosity of the output, for testing
        """
        self.A = A
        self.I = I
        self.R = R
        self.debug = debug
        self.Residual = []  # List of residuals after each iteration

        # Get and sort true eigenpairs
        truePairs = numpy.linalg.eig(A)
        truePairs = self.SortEigen(truePairs)
        self.Values = truePairs[0]
        self.Vectors = truePairs[1]     # Column vectors


    def Arnoldi(self, q, debug=1):
        """
        Arnoldi is an implementation of the iterative Arnoldi algorithm to
        calculate the eigenvalue(s) of a matrix.  
        Arnoldi returns a sorted estimate of the eigenpairs.  This algorithm is
        taken from Watkins (2002) pg. 436.

        q: A normalized starting vector
        """
        debug = self.SetDebug(debug)
        (n,m) = self.A.shape
        H = numpy.zeros((self.I+1,self.I))
        Q = numpy.zeros((n,0))
        Q = numpy.column_stack((Q,q))   # Append q as a column of Q
        print "q: %s" %q

        for k in xrange(1,self.I+1):
#           print "k = %i" %k
            q = numpy.dot(self.A, Q[:,-1])  # Columns of Q are the orthonormal vectors
            for j in xrange(1, (k+1)):
                Qj = Q[:, (j-1)]
                H[(j-1),(k-1)] = numpy.dot(Qj, q)
                q = q - Qj*H[(j-1),(k-1)]
            H[k,(k-1)] = self.Norm(q,2, debug=0)
            q = q/H[k,(k-1)]    # Normalize q
            Q = numpy.column_stack((Q,q))   # Append q as column of Q
            Pairs = self.Pairs_ResidualCalc(H[:k+1,:k], Q, debug=0)
            if self.Residual[-1] < 1e-10:
                print "\nI finished because my residual was too small."
                if debug > 0:
                    print "H:"
                    self.PrintMatrix(H[:k+1,:k])
                print "\nResidual: %s\n" %self.Residual
                break
        print "\nQ:"
        self.PrintMatrix(Q)
        return Pairs


    def ERAM(self, q, Chart=None, debug=-1):
        """
        ERAM---Explicitly Restarted Arnoldi's Method.  This method will
        repeatedly call self.Arnoldi with a "better" initial starting vector.
        It gets the starting vector from the dominant eigenvector from the
        previous call to Arnoldi.  Algorithm from Saad (1992) pg. 179.

        q: Initial vector or "guess"
        Plot: If true, plot restart vector
        """
        debug = self.SetDebug(debug)
        
        self.Residual = []
        for r in xrange(self.R):
            Pairs = self.Arnoldi(q)
            q = Pairs[1][:,-1]    # Dominant eigenvector
            if Chart:
                VectorTitle = "Restart #: %i" %r
                Chart = self.PlotVector(q, Chart=Chart, VectorTitle=VectorTitle)
            if self.Residual[-1] > 1e-10:
                if debug > 0:
                    print "r = %s, q: " %(r)
                    self.PrintVector(q)
            else:
                print "I am stopping restarting because the residual was too"
                print "small."
                break


    def GetSortedEigen(self, Mat):
        """
        GetSortedEigen will return the sorted eigenpairs of Mat.  It uses
        SortEigen to sort and return the eigenpairs

        Mat: 2-D numpy array
        """
        EigenPairs = numpy.linalg.eig(Mat)
        return self.SortEigen(EigenPairs)


    def Norm(self, v, dim, debug=-1):
        """
        Norm will return the dim-norm of vector v.  If dim == 2, Norm will
        return the Euclidean distance.

        v: Vector
        dim: Dimension of the Norm
        """
        debug = self.SetDebug(debug)

        norm = 0
        for element in v:
            norm = norm + math.pow(element, dim)
        norm = math.pow(norm, (1.0/dim))
        return norm


    def Pairs_ResidualCalc(self, H, Q, debug=-1):
        """
        Pairs_ResidualCalc will return the sorted estimates of the true eigenpairs by finding the
        eigenpairs of H.  It will also calculate the residual norm of the iteration.  See 
        Saad (1992) pp. 175-176

        H: The similar matrix to A
        Q: Orthonormal basis vectors (columns of Q)
        """
        debug = self.SetDebug(debug)
        (m, n) = H.shape
        (r, s) = Q.shape
        HValues, HVects = self.GetSortedEigen(H[:-1])    # HVects contains COLUMN vectors
        res = HVects[-1][-1]*H[-1,-1]   # See Saad (1992) pp. 175-176
        self.Residual.append(abs(res))
        if debug > 0:
            print "\n", "~"*100, "\n", "~"*100
            print "H:\t H.shape = %s" %((m,n),)
            self.PrintMatrix(H)
            print "\nQ:\t Q.shape = %s" %((r,s),)
            self.PrintMatrix(Q)
            print "HVects: "
            self.PrintMatrix(HVects)
            print
        EstVectors = numpy.zeros((r,0))
        for i in xrange(n):
            vector = numpy.zeros(r)
            for j in xrange(m-1):
                if debug > 0:
                    print "j = %s\tvector:\t" %(j),
                    self.PrintVector(vector)
                vector = vector + HVects[j][i]*Q[:,j]
            if debug > 0:
                print "j = %s\tvector:\t" %(j),
                self.PrintVector(vector)
                print 
            EstVectors = numpy.column_stack((EstVectors,vector))

        if debug > 0:
            print "\nHValues: ",
            self.PrintVector(HValues)
            print "EstVectors: "
            self.PrintMatrix(EstVectors)
            print "A:"
            self.PrintMatrix(self.A)
            print "\n", "~"*100, "\n", "~"*100

        return (HValues, EstVectors)


    def PlotResidual(self, Chart=None, ChartTitle=None, Title=None, Axes=None, debug=-1):
        """
        PlotResidual will plot (via Gunplot.py) the residual norm as a function
        of the Arnoldi Iteration number.  It returns a Gnuplot.Gnuplot object.

        Chart: The Gnuplot object to plot to.  If Chart=None, a chart will be
            created.
        ChartTitle: The Title for the Chart
        Title: The Gnuplot title for the data series (string object).
        Axes: The Gnuplot axes for the data series (string object).
        """
        debug = self.SetDebug(debug)

        if (not Chart):
            Chart = Gnuplot.Gnuplot(debug=0)
            Chart.xlabel("Arnoldi Iterations")
            Chart.ylabel("Residual Norm")
            Chart.title(ChartTitle)

        ResData = Gnuplot.Data(self.Residual, with="lines", title=Title,
                axes=Axes)
        Chart("set logscale y")
        Chart.replot(ResData)

        return Chart


    def PlotVector(self, vector, Chart=None, ChartTitle=None, VectorTitle=None, Axes=None, debug=-1):
        """
        PlotVector will plot a vector (1-D numpy.array object) to Chart.  If
        Chart doesn't exist, it will be created.  Plot Vector returns a
        Gnuplot.Gnuplot object.

        vector: The data to be plotted
        Chart: The Gnuplot object to plot to.  If Chart=None, a chart will be
            created.
        ChartTitle: The Title for the Chart
        VectorTitle: The Gnuplot title for the data series (string object).
        Axes: The Gnuplot axes for the data series (string object).
        """
        debug = self.SetDebug(debug)

        if (not Chart):
            Chart = Gnuplot.Gnuplot(debug=0)
            Chart.xlabel("Vector element")
            Chart.ylabel("Vector element value")
        if ChartTitle:
            Chart.title(ChartTitle)

        vecData = Gnuplot.Data(vector, with="lines", title=VectorTitle,
                axes=Axes)
        Chart.replot(vecData)

        return Chart

    
    def PrintMatrix(self, Mat):
        """
        PrintMatrix will print Mat in a more readable format than the default
        of the numpy module.  It uses the function Print Vector

        Mat: 2-D numpy array
        """
        for row in Mat:
            self.PrintVector(row)


    def PrintVector(self, v):
        """
        PrintVector will print to the screen the vector v, in a more readale
        format than the default of the numpy module.

        v: 1-D numpy array
        """
        print "[",
        for element in v:
            print "%11.3E" %element,
        print " ]"


    def Residual(self, eValue, eVector):
        """
        Residual will calculate the residual norm for use in Arnoldi's
        algorithm.  This is based on Saad pp 175-176.

        eValue: 
        """
        pass


    def RestartVector(self):
        """
        """
        pass
        # Use Matrix vector multiplication (B=Q*v)
        # Use r = numpy.sum(B, axis=1) to sum column vectors


    def SetDebug(self, debug):
        """
        SetDebug will determine wheter or not debug should be debug or
        self.debug.  This method was required because of a limitation in the
        Python implementation.
        """
        if debug < 0:
            return self.debug
        else:
            return debug


    def SortEigen(self, Pairs):
        """
        SortEigen will sort the eigenpairs (Pairs) by the dominant eigenvalue

        Pairs: Eigenpairs to sort.  Must be a tuple where Pairs[0] is the
            eigenvalues and Pairs[1] the eigenvectors.
        """
        Values = Pairs[0]
        Vectors = Pairs[1]
        m = len(Values)
        SortedIndex = Values.argsort()

        SortedValues = numpy.zeros(m)
        SortedVectors = []
        for i in xrange(m):
            SortedValues[i] = Values[SortedIndex[i]]
            SortedVectors.append(Vectors[:, SortedIndex[i]])
        SortedVectors = numpy.array(SortedVectors)
        SortedVectors = numpy.transpose(SortedVectors)
        return (SortedValues, SortedVectors)



