import Gnuplot
import math
import numpy

__author__   = "$Author: jlconlin $"
__version__ = " $Revision: 83 $"
__date__     = "$Date: 2007-06-14 15:35:15 -0600 (Thu, 14 Jun 2007) $"
__id__       = "$Id: ArnoldiDetermVerify.py 83 2007-06-14 21:35:15Z jlconlin $"


class ArnoldiDetermVerify(object):
    """
    ArnoldiDetermVerify contains some methods to investigate the convergence of the
    Implicitly Restarted Arnoldi's Method.  
    """
    
    def __init__(self, Arnoldi, A,  RestartVector, EigenSort, Vector2Norm):
        """
        Arnoldi: The Arnoldi Method
        A: Matrix for testing
        I: Number of Arnoldi Iterations
        R: Number of Arnoldi Restarts
        EigenSort: Function to sort eigenpairs
        Vector2Norm: Function to calculate 2-Norm of a vector
        """
        object.__init__(self)
        self.Arnoldi = Arnoldi
        self.A = A
        self.RestartVector = RestartVector
        self.EigenSort = EigenSort
        self.Vector2Norm = Vector2Norm
        self.shape = self.A.shape

        self.Values, self.Vectors = numpy.linalg.eig(self.A)
        self.Values, self.Vector = EigenSort(self.Values, self.Vectors)


    def IterRestartConvergence(self, I, R):
        """
        IterRestartConvergence will see how the eigenpair of a matrix A converges as a
        function of the number of Arnoldi Iterations and the number of Arnoldi
        Restarts.

        I: Number of Arnoldi Iterations
        R: Number of Arnoldi Restarts
        """
        print """\nIterRestartConvergence.  Testing how the number of Arnoldi
            Iterations and Restarts affects the convergence of eigenvalues."""

        Chart = Gnuplot.Gnuplot(debug=0)
        Chart.xlabel("Arnoldi Restarts")
        Chart.ylabel("eigenvalue/(true eigenvalue")
        title = "Convergence of Dominant Eigenvalue A = (%i x %i)" %(self.shape)
        Chart.title(title)
        Chart("set yrange [0.78:1.01]")
        Chart("set key right bottom")
        for i in xrange(25, I):
            ConvergValues = []
#           q0 = numpy.ones(self.shape[0])  # Create starting vector
#           q0 = q0/self.Vector2Norm(q0)
            self.RestartConvergence(i, R, Chart)
        

    def RestartConvergence(self, I, R, Chart=None):
        """
        RestartConvergence will see how the eigenpair of a matrix A converges as a
        function of the number of restarts.

        I: Number of Arnoldi Iterations
        R: Number of Arnoldi Restarts
        Chart: Gnuplot object to draw plots on
        """
        print """\nRestartConvergence.  Testing how the number of restarts affects
            the convergence of eigenvalues."""
        # Create starting vector
        q0 = numpy.ones(self.shape[0])
        q0 = q0/self.Vector2Norm(q0)

        ConvergValues = []
        if not Chart:
            Chart = Gnuplot.Gnuplot(debug=0)
            Chart.xlabel("Arnoldi Restarts")
            Chart.ylabel("eigenvalue/(true eigenvalue)")
            title = "Convergence of Dominant Eigenvalue A = (%i x %i)" %(self.shape)
            Chart.title(title)
            Chart("set yrange [*:1.001]")

        for i in xrange(1, R): # Iterate over all number of restarts
            for r in xrange(i):
                Pairs, Q, Invariance = self.Arnoldi(self.A, I, q0)
                if not Invariance:
                    sPairs = self.EigenSort(Pairs[0], Pairs[-1])
                    q0 = self.RestartVector(sPairs[1][-1], Q[:,:-1])
                else:
                    break
            ConvergValues.append(sPairs[0][-1]/self.Values[-1])
            if Invariance:
                break

        Values = Gnuplot.Data(ConvergValues, axes="x1y1", with="lines", 
                title="Iters: %i, Restarts: %i" %(I, i))
        Chart.replot(Values)
        print "\nEigenvalue Convergence (log(eigenvalue/(true eigenvalue))):"
        for number in ConvergValues:
            print "%6.3E" %math.log(number, 10),


    def IterationConvergence(self, I):
        """
        IterationConvergence will see how the eigenpair of a matrix A converges as a
        function of the number of Arnoldi Iterations.

        I: Number of Arnoldi Iterations
        """
        print """\nIterationConvergence.  Testing how the number of Arnoldi
            Iterations affects the convergence of eigenvalues."""

        # Create starting vector
        q0 = numpy.ones(self.shape[0])
        q0 = q0/self.Vector2Norm(q0)

        ConvergValues = []    # List of eigenvalues for number of Arnoldi iterations
        Chart = Gnuplot.Gnuplot(debug=0)
        for i in xrange(1, I):
            Pairs, Q, Invariance = self.Arnoldi(self.A, i, q0)
            if not Invariance:
                sPairs = self.EigenSort(Pairs[0], Pairs[-1])    # Sorted eigenpairs
                q0 = self.RestartVector(sPairs[1][-1], Q[:,:-1])
                ConvergValues.append(sPairs[0][-1]/self.Values[-1])
            else:
                break
            
        Values = Gnuplot.Data(ConvergValues, axes="x1y1", with="lines", 
                title="Eigenvalue")
        print "\nEigenvalue Convergence (log(eigenvalue/(true eigenvalue))):"
        for number in ConvergValues:
            print "%6.3E" %math.log(number, 10),
        Chart.xlabel("Arnoldi Iterations")
        Chart.ylabel("eigenvalue/(true eigenvalue)")
        title = "Convergence of Dominant Eigenvalue A = (%i x %i)" %(self.shape)
        Chart.title(title)
        Chart("set yrange [*:1.05]")
        Chart.replot(Values)


