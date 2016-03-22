
import scipy



import fissionBank
import fissionSource
import Markov
import Geometry
import CrossSection
import arnoldiMC
import powerMC

def main():
    histories = 10000
    iterations = 10
    xs = CrossSection.CrossSection(xS=0.5, nu=1.0, xF=0.5, xG=0)
    geo = Geometry.Geometry(10,[[-0.5, 0.5]])
    uni = scipy.ones(geo.bins)
    point = scipy.zeros(geo.bins)
    point[0] = 1
    uSource = fissionSource.histogramSource(uni,geo)
    pSource = fissionSource.histogramSource(point,geo)
    uSource = uSource/scipy.linalg.norm(uSource,2)

    mark = Markov.Markov(geo,xs, histories)
    amc = arnoldiMC.arnoldiMC(mark)

#   calc_eVector(amc)
    EigenPairs(amc)

def EigenPairs(amc):
#   A = InverseDiag(10)
    A = DiagonalMatrix(10)
#   A[0,1] = 1
#   A[1,1] = 20
    print "A: \n%s" %A

    values, vectors = amc._EigenPairs(A)
    print "values: %s" %values
    print "vectors: "
    amc.printM(vectors)



def calc_eVector(amc):
    Q = [scipy.zeros(10)]
    for i in xrange(10):
        q = scipy.zeros(10)
        q[i] = i+1
        Q.append(q)
    v = scipy.zeros(10)
    v[1:3] = 5
    v[-1] = 3
    print "length = %s, Q: \n" %(len(Q)),
    amc.printM(Q)
    print "\nlength = %s, v: " %(len(v)),
    amc.printVector(v)
    result = amc._calc_eVector(Q,v)
    print "result: %s" %result


def DiagonalMatrix(N):
    A = scipy.zeros((N,N))
    for i in xrange(N):
        A[i,i] = i+1

    return A


def InverseDiag(N):
    A = scipy.zeros((N,N))
    for i in xrange(N):
        A[i,i] = 1.0/(i+1)

    return A

main()
