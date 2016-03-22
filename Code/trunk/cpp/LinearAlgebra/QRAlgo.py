import numpy
import numpy.linalg
import sys
import string
import math

def Hessenberg(r,c):
    """
    Hessenberg will create a 2-D numpy array of shape (r,c).  It will be made in
    upper Hessenberg form.
    """
    H = numpy.zeros((r,c))
    k = 0;
    for i in xrange(len(H)):
        for j in xrange(len(H[0])):
            if i <= j+1:
                k += 1
                H[i,j] = k
    return H

def Full(n):
    F = numpy.array(range(1, n*n+1))
    F.resize((n,n))
    return F


def QRAlgShift(A, Shifts, Q):
    """
    QRAlg will perform the shifted QR algorithm using exact shifts.  It will 
    determine if the shifts are complex, and if so, will use a double-shift.
    It assumes A is upper hessenberg.
    """
    n = A.shape[0]
    print "Shifts: %s" %Shifts

    l = len(Shifts)
    k = 0
    while k < l:
#   for s in Shifts:
        S = Shifts[k]
        print "\n-------------------------------------------\n"
        print "k = %i, Shift: %s" %(k,S)
        try:
            if S.imag != 0:
                s = -2*S.real
                t = abs(S.real*S.real + S.imag*S.imag)
#               t = math.pow(S.real,2) + math.pow(S.imag, 2)
                m = numpy.dot(A, A) + s*A + t*numpy.eye(n)
                k += 2
            else:
                m = A-S.real*numpy.eye(n)
                k +=1
        except AttributeError:
            m = A-S*numpy.eye(n)
            k += 1

        q,r = numpy.linalg.qr(m)

        A = numpy.dot(q.transpose(), A)
        A = numpy.dot(A, q)
        Q = numpy.dot(Q,q)

        for i in xrange(n):
            for j in xrange(n):
                if i > j+1: A[i,j] = 0
        print "Q:\n%s\nA:\n%s" %(Q,A)

    return Q, A


def QRAlg(A, n, shift = 0):
    """
    QRAlg will perform the QR algorithm on A for n steps.
    """
    for i in xrange(n):
        if shift:
            shape = A.shape[0]
            A = A - shift*numpy.eye(shape)
        q,r = numpy.linalg.qr(A)
        A = numpy.dot(r,numpy.transpose(q) )
        if shift:
            A += shift+ numpy.eye(shape)
    return A, q, r


if __name__ == "__main__":
    print "Testing Shifted QR algorithm."

    if len(sys.argv) > 1 and string.lower(sys.argv[1]) == 'h':
#      A = numpy.array([[1,2,3,4],[5,6,7,8],[0,9,10,11],[0,0,-12,13]])
       A = Hessenberg(5,5)
       Shifts = numpy.linalg.eigvals(A)
    elif len(sys.argv) > 1 and string.lower(sys.argv[1]) == 'f':
       A = Full(5)
       Shifts = numpy.linalg.eigvals(A)
    else:
        A = numpy.array([[8,2],[2,5]])
        Shifts = [5.0, 4.0769,4.00001907341357, 4.0]

    print "Starting with\nA:\n%s\n" %A
    Q = numpy.diag(numpy.ones(A.shape[0]))
    QRAlgShift(A, Shifts, Q)

