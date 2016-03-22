
import numpy
import sys
import math
import matplotlib.pyplot as plt

""" 
This file is used to test how sampling of a linear function should happen.
"""

class LinearSampler(object):
    """
    LinearSampler is used to investigate different ways of sampling from a 
    linear function.
    """

    def __init__(self, xmin = 0, xmax = 1, slope = (2.0/3.0), intercept = 1.0):
        self._xmin = xmin
        self._xmax = xmax
        self._m = slope
        self._b = intercept
        self._slope = 0.0       # slope for scoring
        self._intercept = 0.0   # intercept for scoring
        object.__init__(self)

        self._Normalize()
        
    def __repr__(self):
        return "%6.4f*x + %6.4f" %(self._m, self._b)

    def _Normalize(self):
        """
        _Normalize will normalize the slope and the intercept such that the 
        absolute value of the area under the curve will be 1 between self._xmin
        and self._xmax.
        """
        self._a = self.area()

        self._b /= self._a
        self._m /= self._a

    def NewSampler(self):
        """
        NewSampler will return a new LinearSampler object with 
        slope = self._slope and intercept = self._intercept.  This is the 
        result of scoring multiple samples.  It sets self._slope and 
        self._intercept to zero.
        """

        NS = LinearSampler(self._xmin, self._xmax, self._slope, self._intercept)
        self._slope = 0.0
        self._intercept = 0.0
        return NS

    def Sample(self, N):
        """
        Sample is just syntax sugar for calling Forrest's sampling scheme
        """
        return self.Forrest(N)

    def Score(self, xS):
        """
        Score will 'score' for the linear function using a Legendre Expansion
        with n = 2.
        """
        pass

    def area(self):
        """
        area will return the absolute value of the area under the curve between
        self._xmin and self._xmax
        """
        ymin = self.f(self._xmin)
        ymax = self.f(self._xmax)

        if ymin*ymax < 0:
            a1 = (1.0/2.0)*((-self._b/self._m) - self._xmin)*(2.0*self._b + 
                self._m*((-self._b/self._m) + self._xmin) )
            a2 = (1.0/2.0)*(self._xmax - (-self._b/self._m))*(2.0*self._b + 
                self._m*(self._xmax + (-self._b/self._m)) )

            return abs(a1) + abs(a2)

        else: return abs( (1.0/2.0)*(self._xmax - self._xmin)*(2.0*self._b + 
                self._m*(self._xmax + self._xmin) ) )

    def f(self, x, m = None, b = None):
        if not m: m = self._m
        if not b: b = self._b

        return m*x + b

    def Plot(self, xmin = None, xmax = None):
        """
        Plot will plot the linear function on the interval [xmin, xmax].
        """
        if not xmin: xmin = self._xmin
        if not xmax: xmax = self._xmax

        bins = 100
        x = numpy.linspace(xmin, xmax, bins)
        y = numpy.zeros(bins)
        for i in xrange(bins):
            y[i] = self.f(x[i])

        y /= sum(abs(y))
        plt.plot(x,y, lw=2, label="y=%s" %self)
        
        return (x,y)

    def PlotWithSampled(self, xS, title, bins = 100):
        """
        PlotWithSampled will plot the function along with the sampled points
        """
        h, edges = PlotSampled(xS, title, bins)

        y = numpy.zeros(len(edges))
        for n in xrange(len(y)):
            y[n] = self.f(edges[n])
#       plt.plot(edges, y, label="y=%s" %self)
        plt.plot(edges, y)

    def Forrest(self, N):
        """
        This function samples according to Forrest's notes.  It returns an 
        array of sample points of length N.
        """
        ymin = self.f(self._xmin)
        ymax = self.f(self._xmax)
        yRatio = ymin/(ymax+ymin)

        # Sampling
        diff = self._xmax-self._xmin
        xS = numpy.zeros(N)
        for n in xrange(N):
            r = numpy.random.random()
            if r < yRatio:
                xS[n] = self._xmax - (self._xmax-self._xmin)* \
                        math.sqrt( numpy.random.random() )
            else:
                xS[n] = self._xmin + (self._xmax-self._xmin)* \
                        math.sqrt( numpy.random.random() )
        return xS

    def Holloway(self, N):
        """
        Holloway will sample from the linear function according to Holloway's 
        derivation.  It returns an array of sample points of length N.
        """
        ymin = self.f(self._xmin)
        ymax = self.f(self._xmax)

        # These variables are created to avoid recalculating them many times
        two_m = 2.0*self._m
        inner = self._b + self._m*self._xmin
        inner *= inner

        xS = numpy.zeros(N)
        if not self._m:     # do simple linear sampling
            for n in xrange(N):
                r = numpy.random.random()
                xS[n] = self._xmin + (self._xmax - self._xmin)*r
        else:
            for n in xrange(N):
                r = numpy.random.random()
                xS[n] = ( -self._b + math.sqrt(inner + two_m*r) )/self._m

        return xS

def PlotSampled(x, title, bins = 100):
    """
    PlotSampled will plot an array of sampled points by binning them into a 
    historgram
    """
#   h, edges, p = numpy.histogram(x, bins=bins, normed=True)
    h, e, p = plt.hist(x, bins = bins, label=title, histtype='step', 
            normed=True)
    
    return h, e

def SampleExamples(N):
    LS = LinearSampler(xmin = 0, xmax = 3, slope = 1, intercept = 10.0)
    plt.ylim(ymin=0)
    xS = LS.Forrest(N)
    LS.PlotWithSampled(xS, "F", bins=100)
    xS = LS.Holloway(N)
    LS.PlotWithSampled(xS, "H", bins=100)


if __name__ == "__main__":

    if len(sys.argv) < 2:
        N = 100000
    else:
        N = int(sys.argv[1])

    plt.clf()
    plt.title("N = %6.4G" %N)
    SampleExamples(N)
    plt.legend()
    


