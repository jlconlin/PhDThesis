#!/usr/bin/python
import numpy
import math

"""
This file is merely a prototype and a checker for my C++ code.
"""

def Diagonal(n):
    A = numpy.diag(numpy.arange(1,n+1),0)
    return A

def Standard(n):
    A = Diagonal(n)
    A[2,1] = 1
    return A

class Arnoldi():
    def __init__(self, A):
        self.A = A

    def IRAM(self, q):
        print "I'm in Arnoldi.IRAM\n"
        n,n = A.shape   # Assume square matrix
        self.H = numpy.zeros((n+1,n), dtype='complex')

        q = q/numpy.linalg.norm(q,2)
        self.Q = [q]

        # First Iteration
        self.k = 0
        new_q = numpy.dot(self.A, self.Q[0])
        self.H[0,self.k] = numpy.dot(new_q, self.Q[0])
        new_q = new_q - self.H[0, self.k]*self.Q[0]
        self.H[self.k+1, self.k] = numpy.linalg.norm(new_q,2)
        new_q = new_q/self.H[self.k+1, self.k]
        self.Q.append(new_q)
        print "k = %s, new_q: %s" %(self.k, new_q)

        # Second Iteration
        self.k += 1
        new_q = numpy.dot(self.A, self.Q[self.k])
        self.H[0,self.k] = numpy.dot(new_q, self.Q[0])
        self.H[1,self.k] = numpy.dot(new_q, self.Q[1])
        new_q = new_q - self.H[0, self.k]*self.Q[0]
        new_q = new_q - self.H[1, self.k]*self.Q[1]
        self.H[self.k+1, self.k] = numpy.linalg.norm(new_q,2)
        new_q = new_q/self.H[self.k+1, self.k]
        self.Q.append(new_q)
        print "k = %s, new_q: %s" %(self.k, new_q)

        # Third Iteration (First Inner Iteration)
        self.k += 1
        new_q = numpy.dot(self.A, self.Q[self.k])
        self.H[0,self.k] = numpy.dot(new_q, self.Q[0])
        self.H[1,self.k] = numpy.dot(new_q, self.Q[1])
        self.H[2,self.k] = numpy.dot(new_q, self.Q[2])
        new_q = new_q - self.H[0, self.k]*self.Q[0]
        new_q = new_q - self.H[1, self.k]*self.Q[1]
        new_q = new_q - self.H[2, self.k]*self.Q[2]
        self.H[self.k+1, self.k] = numpy.linalg.norm(new_q,2)
        new_q = new_q/self.H[self.k+1, self.k]
        self.Q.append(new_q)
        print "k = %s, new_q: %s" %(self.k, new_q)

        # Fourth Iteration (Second Inner Iteration)
        self.k += 1
        new_q = numpy.dot(self.A, self.Q[self.k])
        self.H[0,self.k] = numpy.dot(new_q, self.Q[0])
        self.H[1,self.k] = numpy.dot(new_q, self.Q[1])
        self.H[2,self.k] = numpy.dot(new_q, self.Q[2])
        self.H[3,self.k] = numpy.dot(new_q, self.Q[3])
        new_q = new_q - self.H[0, self.k]*self.Q[0]
        new_q = new_q - self.H[1, self.k]*self.Q[1]
        new_q = new_q - self.H[2, self.k]*self.Q[2]
        new_q = new_q - self.H[3, self.k]*self.Q[3]
        self.H[self.k+1, self.k] = numpy.linalg.norm(new_q,2)
        new_q = new_q/self.H[self.k+1, self.k]
        self.Q.append(new_q)
        print "k = %s, new_q: %s" %(self.k, new_q)

        # QR portion
        # Shifts
        self.Hs = self.H[:self.k+1,:self.k+1]
        self.values = numpy.linalg.eigvals(self.Hs)
        self.values.sort()
        self.shifts = self.values[:2]
        print "\nEigenvalues: %s, \nShifts: %s" %(self.values, self.shifts)

        # First Shift
        self.Hs = self.Hs - self.shifts[0]*numpy.eye(n-1)
        print "Hs:\n%s" %(self.Hs)
        v,r = numpy.linalg.qr(self.Hs)
        self.Hs = numpy.dot(r,v) + self.shifts[0]*numpy.eye(n-1)
        print "v:\n%s\nr:\n%s\nHs:\n%s" %(v,r,self.Hs)

        # Second Shift
        self.Hs = self.Hs - self.shifts[1]*numpy.eye(n-1)
        print "Hs:\n%s" %(self.Hs)
        v,r = numpy.linalg.qr(self.Hs)
        self.Hs = numpy.dot(r,v) + self.shifts[1]*numpy.eye(n-1)
        print "v:\n%s\nr:\n%s\nHs:\n%s" %(v,r,self.Hs)




if __name__ == "__main__":
    n = 5
    A = Standard(n)
    arn = Arnoldi(A)

    q = numpy.ones(n)
    arn.IRAM(q)
    
