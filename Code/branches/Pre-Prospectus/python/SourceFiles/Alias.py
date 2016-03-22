__id__       = "$Id: Alias.py 59 2007-04-27 16:17:29Z jlconlin $"
__author__   = "$Author: jlconlin $"
__version__ = " $Revision: 59 $"
__date__     = "$Date: 2007-04-27 10:17:29 -0600 (Fri, 27 Apr 2007) $"

"""Implementation of Walker's Alias method.  Originally written by 
Duncan Smith duncan.g.smith@manchester.ac.uk"""

import random
import numpy


def _alias_table(probs):
    """
    Returns lists of aliases and cutoffs (see Kronmal and
    Peterson, 1979, I{The American Statistician}, Vol.33,
    No.4., pp. 214-218).

    @type  probs: sequence of I{floats}
    @param probs: sequence of probabilities
    @rtype:       C{tuple} of C{lists}
    @return:      a C{list} of aliases and a C{list}
                  of cutoffs
    """
    n = len(probs)
    probs = numpy.array(probs)
    r = probs*n # mean r is 1
    a = numpy.zeros(n)
    G = set()
    S = set()
    # partition r into values lower and higher than mean
    for i, val in enumerate(r):
        if val < 1:
            S.add(i)
        else:
            G.add(i)
    while True:
        try:
            j, k = S.pop(), G.pop()
        except KeyError:
            break
        a[j] = k
        r[k] += r[j] - 1
        if r[k] < 1:
            S.add(k)
        else:
            G.add(k)
    return a, r

def gen_discrete(probs, N):
    """
    Generates discrete (0, 1, ..., I{n}-1) variates from the
    mass function defined by I{probs} using the alias method.

    N: number of discrete variates desired
    @type   probs: sequence of C{floats}
    @param  probs: probabilities of corresponding values
    @rtype:        generator of I{ints}
    @return:       generator of independent discrete variates
    """
    a, r = _alias_table(probs)
    n = len(probs)
    for z in xrange(N):
#   while True:
        rnd_n = random.random() * n
        i = int(rnd_n)
        if rnd_n - i < r[i]:
            yield i
        else:
            yield a[i]


