__id__       = "$Id: test_Matrix.py 35 2007-03-27 15:31:57Z jlconlin $"
__author__   = "$Author: jlconlin $"
__version__ = " $Revision: 35 $"
__date__     = "$Date: 2007-03-27 09:31:57 -0600 (Tue, 27 Mar 2007) $"

import random
import math

import scipy
import scipy.linalg
import Gnuplot

import fissionSource
import fissionBank
import Markov
import Geometry
import CrossSection
import arnoldiMC
import ArnoldiDtm


class Investigator(object):
    def main(self):
        bins = 50
        halfwidth=0.5
        histories = 10000
        iterations = 50
        restarts = 5
        geo = Geometry.Geometry(bins, [[-halfwidth,halfwidth]])
        xs = CrossSection.CrossSection(xS=0.5,nu=1.0,xF=0.5,xG=0)
        self.mark = Markov.Markov(geo, xs, histories)
        self.mark.score = self.score
        self.Q = scipy.zeros((bins,0))

        for i in xrange(bins):
            print "I am at %i" %i
            point = scipy.zeros((bins))
            point[i] = 1
            pSource = fissionSource.histogramSource(point,geo)

            self.response = fissionBank.fissionBank()
            self.mark.transport(pSource)
            q = fissionSource.histogramSource(self.response,geo)
            q = q*(1.0/self.mark.histories)
            self.printVector(q)
            self.Q = scipy.column_stack((self.Q,q))

        
        q = scipy.ones(bins)
        q = q/scipy.linalg.norm(q,2)

        print "Calling Deterministic Arnoldi"
        adtm = ArnoldiDtm.Arnoldi(self.Q, iterations, restarts)
        eValues, eVectors = adtm.Arnoldi(q)
        print "Eigenvalues: "
        self.printVector(eValues)
        print "Dominant eigenvector: "
        self.printVector(eVectors[:,-1])
        print "\nAll eigenvectors: "
        self.printM(eVectors)

        Chart = Gnuplot.Gnuplot()
        Chart.title("Histories per 'vector': %i, bins = %i" %(histories, bins))
        
        length = len(eValues)-1
        for i in xrange(5):
            data = Gnuplot.Data(eVectors[:,length-i],with='lines', 
                    title='vector %i' %i)
            Chart.replot(data)

        

    def score(self, history):
        ran = random.random()
        N = math.floor(history.weight*(self.mark.xs.nu*self.mark.xs.xF/self.mark.xs.xT) + ran)
        self.response.append(history, N)     # Contribute to fission source

    def printM(self, M):
        for row in M:
            self.printVector(row)


    def printVector(self, row):
        print "[",
        for element in row:
            print "%7.4f " %element,
        print "]"

I = Investigator()
I.main()
