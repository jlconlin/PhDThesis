
import sys
import string
import math
import numpy
import matplotlib.pyplot as pyplot

def PlotSampled(x, title, bins = 100):
    """
    PlotSampled will plot an array of sampled points by binning them into a 
    historgram
    """
    h, e, p = pyplot.hist(x, bins = bins, label=title, histtype='step', 
            normed=True)
    
    return h, e

class f(object):
    """
    f represents the linear function f(x) = m*x + b
    """
    def __init__(self, m, b, xmin, xmax):
        self.m = m
        self.b = b
        self.xmin = xmin
        self.xmax = xmax
        self.xmid = (self.xmax + self.xmin)/2.0

        self.width = self.xmax - self.xmin
        object.__init__(self)

    def __call__(self, x):
#       print "I'm inside of call with x = %s" %(x)
        return self.f(x)

    def __repr__(self):
        return "%6.4f*x + %6.4f,\t%5.3f <= x <= %5.3f" %(
                self.m, self.b, self.xmin, self.xmax)

    def __div__(self, x):
        self.b /= x
        self.m /= x

    def Normalize(self):
        """
        """
#       constant = ( math.pow(self.b + self.m*self.xmax,3)-
#                   math.pow(self.b + self.m*self.xmin,3) )/(3.0*self.m)
#       self / math.sqrt(constant)
        constant = 0.5*self.width*(2*self.b + self.m*(self.xmax + self.xmin))
        self / constant

    def f(self, x): 
        return self.m*x + self.b

    def w(self, x):
        """
        w is the weighted function f(x)/U(x) where U(x) is the uniform
        distribution over the range [self.xmin, self.xmax]
        """
        return self.width*self.f(x)

    def Plot(self, title, xmin = None, xmax = None):
        """
        Plot will plot the linear function on the interval [xmin, xmax].
        """
        if not xmin: xmin = self.xmin
        if not xmax: xmax = self.xmax

        x = [xmin, xmax]
        y = [self(a) for a in x]

        pyplot.plot(x,y, lw=2, label=title)


class Griesheimer(object):
    """
    Griesheimer will perform sampling according to David Griesheimer's work.
    """
    def __init__(self, f, N):
        """
        f is a linear function object
        N is the number of times to score
        """
        self.N = N
        self.f = f
        print "\nGriesheimer Sampling\nf(x) = %s" %(self.f)
        object.__init__(self)

    def k(self, n):
        """
        k will return the normalization constant.
        """
        return (self.f.xmax - self.f.xmin)/(2*n + 1)

    def xScaled(self, x):
        """
        xScaled will scale the input x so it lies within the range [-1,1].
        """
        return 2*(x-self.f.xmin)/self.f.width - 1

    def score(self):
        k0 = self.k(0)
        k1 = self.k(1)

        print "\nk0 = %6.4f, k1 = %6.4f" %(k0, k1)
        x = numpy.random.rand(self.N)
        X = (self.f.xmax - self.f.xmin)*x + self.f.xmin
        X = self.xScaled(X)

        print "min: %6.4f, max: %6.4f, mean: %6.4f" %(
                X.min(), X.max(), X.mean() )

        a0 = self.f.w(X)
        a1 = a0*X

        print "a0: %6.4f +- %6.4f" %(a0.mean(), a0.std()/math.sqrt(self.N) )
        print "a1: %6.4f +- %6.4f" %(a1.mean(), a1.std()/math.sqrt(self.N) )
        print "a0/k0: %6.4f" %(a0.mean()/k0)
        print "a1/k1: %6.4f" %(a1.mean()/k1)

    def newF(self, a0, a1):

        A = a0*self.k(0) - self.k(1)*(self.f.xmax+self.f.xmin)/self.f.width
        B = a1*self.k(1)*2.0/(self.f.width)

        print "\nA: %6.4f, B: %6.4f" %(A, B)


class Centered(object):
    """
    Centered is a linearspace object that is already centered about 0
    """
    def __init__(self, f, N):
        self.f = f
        self.N = N

        print "\nCentered Scoring\nf(x) = %s" %self.f
        object.__init__(self)

    def score(self):
        k0 = self.f.width
        k1 = (math.pow(self.f.xmax,3) - math.pow(self.f.xmin,3))/3.0

        print "\nk0 = %6.4f, k1 = %6.4f" %(k0, k1)

        # Pick random numbers
        x = numpy.random.rand(self.N)
        X = (self.f.xmax - self.f.xmin)*x + self.f.xmin
#       X = xScaledJ(X)  #no need to do this since it is already centered

        a0 = self.f.w(X)
        a1 = a0*X

        print "a0: %6.4f +- %6.4f" %(a0.mean(), a0.std()/math.sqrt(self.N) )
        print "a1: %6.4f +- %6.4f" %(a1.mean(), a1.std()/math.sqrt(self.N) )
        print "a0/k0: %6.4f" %(a0.mean()/k0)
        print "a1/k1: %6.4f" %(a1.mean()/k1)


class Shifted(object):
    """
    Shifted is a LinearSpace oject that is shifted so that it is centered about
    0.
    """
    def __init__(self, f, N):
        self.f = f
        self.N = N

        print "\nShifted Scoring\nf(x) = %s" %self.f
        object.__init__(self)

    def score(self):
        k0 = self.f.width
        k1 = math.pow(self.f.width,3)/12.0
        print "\nk0 = %6.4f, k1 = %6.4f" %(k0, k1)

        x = numpy.random.rand(self.N)
        X = (self.f.xmax - self.f.xmin)*x + self.f.xmin
        X -= self.f.xmid

        print "min: %6.4f, max: %6.4f, mean: %6.4f" %(
                X.min(), X.max(), X.mean() )

        a0 = self.f.w(X)
        a1 = a0*X

        print "a0: %6.4f +- %6.4f" %(a0.mean(), a0.std()/math.sqrt(self.N) )
        print "a1: %6.4f +- %6.4f" %(a1.mean(), a1.std()/math.sqrt(self.N) )
        print "a0/k0: %6.4f" %(a0.mean()/k0)
        print "a1/k1: %6.4f" %(a1.mean()/k1)


class LinearSpace(object):
    """
    LinearSpace will simulate the sampling and scoring in a linear over space
    approximated source.
    """
    def __init__(self, f):
        """
        f is a function object
        """
        self.f = f
        object.__init__(self)

    def xScaled(self, x):
        """
        xScaled will scale the input x so it lies within the range [-1,1].
        """
        return 2.0*(x-self.f.xmin)/self.f.width - 1.0

    def SampleAndScore(self, N):
        """
        SampleAndScore will sample N times and score N times to see if we
        understand what we are doing.
        """
        print "\nSampling and Scoring on:\n\tf(x) = %s" %self.f

        X = self.Sample(N)
        PlotSampled(X, "Sampled")
        self.Score(X)

    def k(self, n):
        return self.f.width/(2*n+1)

    def Score(self, x):
        """
        Score will score in the appropriate manner given the sampled points x.
        """
        x = self.xScaled(x)
        print "min: %6.4f, max: %6.4f, mean: %6.4f" %(x.min(),x.max(),x.mean())

        a0 = numpy.ones(len(x)) # Nothing special from the 0-th order Legendre
        a1 = a0*x

        A0 = a0.mean()/self.k(0)
        A1 = a1.mean()/self.k(1)
        print "\nk0 =  %6.4f, k1 = %6.4f" %(self.k(0), self.k(1))
        print "A0 = %6.4f, A1 = %6.4f" %(A0,A1)

        b = A0 - A1*(self.f.xmax + self.f.xmin)/self.f.width
        m = A1*2.0/self.f.width
        scoredF = f(m, b, self.f.xmin, self.f.xmax)

        scoredF.Normalize()
        print "scoredF f(x) = %s" %scoredF

        scoredF.Plot("Scored")

    def Sample(self, N):
        """
        This function samples according to Forrest's notes.  It returns an 
        array of sample points of length N.
        """
        ymin = self.f(self.f.xmin)
        ymax = self.f(self.f.xmax)
        yRatio = ymin/(ymax+ymin)

        # Sampling
        xS = numpy.zeros(N)
        for n in xrange(int(N)):
            r = numpy.random.random()
            if r < yRatio:
                xS[n] = self.f.xmax - self.f.width* \
                        math.sqrt( numpy.random.random() )
            else:
                xS[n] = self.f.xmin + self.f.width* \
                        math.sqrt( numpy.random.random() )
        return xS


if __name__ == "__main__":
    xmin = -1.0
    xmax = 1.0
    xmid = (xmax+xmin)/2.0
    N = 1E6
    pyplot.clf()
    fx = f(m=1.0, b=-1.0, xmin = 1.0, xmax = 5.0)
#   fx = f(m=2.0, b=-1.0, xmin = 1.0, xmax = 5.0)
#   fx = f(m=2.0, b=-1.0, xmin = 1.0, xmax = 10.0)
#   fx = f(m=1.0, b=-1.0, xmin = 1.0, xmax = 10.0)
#   fx = f(m=1.0, b=1.0, xmin = -1.0, xmax = 1.0)
    fx.Normalize()
    try:
        if string.upper(sys.argv[1]) == 'G':  # Griesheimer
            G = Griesheimer(fx, N)
            G.score()
        elif string.upper(sys.argv[1]) == 'C':  # Centered
            C = Centered(fx, N)
            C.score()
        elif string.upper(sys.argv[1]) == 'S':  # Shifted
            S = Shifted(fx, N)
            S.score()
        elif string.upper(sys.argv[1]) == 'L':  # Shifted
            fx.Normalize()
            fx.Plot("fx")
            L = LinearSpace(fx)
            L.SampleAndScore(N)
            pyplot.legend(loc=2)
            pyplot.title("%s" %fx)
    except IndexError: 
        print "I don't know what to do!"

