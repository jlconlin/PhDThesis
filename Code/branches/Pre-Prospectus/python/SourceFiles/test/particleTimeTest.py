

import time
import os
import math
import random

import particle


def main():

    N = int(1e6)
    start = time.localtime()
    for i in xrange(N):
        n = particle.neutron()
        del n
    end = time.localtime()
    etime = time.mktime(end) - time.mktime(start)
    print "Time to create and delete %i neutrons: %s" %(N, etime)
    print etime/N

    start = time.localtime()
    particles = []
    for i in xrange(N):
#       direction = (random.random(), random.random(), random.random())
#       position = (random.random(), random.random(), random.random())
        direction = (1.0,2.0,3.0)
        position = (1.0,2.0,3.0)
        particles.append((position, direction))
    end = time.localtime()
    etime = time.mktime(end) - time.mktime(start)
    print "Time to create pos and dirs for %i neutrons: %s" %(N, etime)

    start = time.localtime()
    n = particle.neutron()
    for i in xrange(N):
        n.position = particles[i][0]
        n.direction = particles[i][1]
        n.weight = 1.0
    del n
    end = time.localtime()
    etime = time.mktime(end) - time.mktime(start)
    print "Time to reassign %i neutrons: %s" %(N, etime)


main()
