#! /usr/local/bin/python

import time
import os 
import math
import random

class Neutron(object):
    def __init__(self, Location, Direction):
        self.Direction = Direction
        self.Location = Location

def getFreq():
    input, output = os.popen2('sysctl -n hw.cpufrequency', 1)
    input.close()
    speed = float(output.readline())
    output.close()
    return speed

def Transport(Width, xS, xA, xT, Location):
    """
    This function will transport a particle. The function returns three things
    in a tuple; the location of the particle, whether the particle escaped the
    geometry and whether or not the particle was absorbed. 

    Width: The width of the multiplying slab
    xS, xA, xT: Scattering, Absorption, and Total cross sections
    Location: The location of the particle
    Direction: The direction of the particle
    """
    for i in xrange(1000):
        # Move particle
#       Location =  Location - Direction*(xT*math.log(random.random()))
        Location = Move(Location, xT)
        if not Escaped(Location, Width):  # Particle still in system
            Interaction = random.random()
            if Interaction*xT > xA:   # Scatter
                Direction = random.choice([-1,1])   # Pick new direction
            else:   # Absorption
#               print "cycle length: %s" %i
                return (Location, False, True)
        else:   # Particle has "left the building"
#           print "cycle length: %s" %i
            return (Location, True, False)
    return (Location, False, False)  # Particle remained in system, but scatter

def Move(Location, xT):
    """
    This function will return the new location (as a tuple) of a particle after it has
    isotropically scattered

    Location: Location of particle before scattering
    xT: Total cross section of medium
    """
    # Sample distance to collision
    Distance = (1/xT)*math.log(random.random())

    # Sample new direction
    phi = 2*math.pi*random.random()
    u = 2*random.random() - 1
    v = math.sqrt(1-u*u)*math.cos(phi)
    w = math.sqrt(1-u*u)*math.sin(phi)

    # Move particle
    x = Location[0] + Distance*u
    y = Location[1] + Distance*v
    z = Location[2] + Distance*w

    return (x, y, z)

def Escaped(Location, Width):
    """
    This function will return a true if the particle has esacped the cubic
    geometry defined by 
    Width: The width of the cube
    Location: A tuple defining where the particle is located
    """
    for coordinate in Location:
        if coordinate < 0 or coordinate > Width:
            return True
    
    return False    # Return this if particle is still in cube

def main():
    freq = getFreq()
    print "CPU speed: ", freq, 'Hz' 
    N = int(1e6)
    start = time.localtime()
    for i in xrange(N):
        Location = (10.0,3.0,2.0)
        Direction = (3.5,1.0,0.93)
        positron = Neutron(Location,Direction)
        del positron 
        # math.sin(1.0)
    end = time.localtime()
    etime = time.mktime(end) - time.mktime(start)
    print "time elapsed: %s seconds" % etime
    print "%g sec per object" % (etime/N)
    print "%g cycles per object" % ((etime/N)*freq)

    start = time.localtime()
    for i in xrange(N):
        x = random.uniform(0,10)
        y = random.uniform(0,10)
        z = random.uniform(0,10)
        Location = (x,y,z)
        Direction = (3.5,1.0,0.93)
        positron = Neutron(Location,Direction)
        Transport(10, 99.0, 1.0, 100.0, Location)
        del positron
    end = time.localtime()
    etime = time.mktime(end) - time.mktime(start)
    print "time elapsed: %s seconds" % etime
    print "%g seconds per transport" % (etime/N)
    print "%g cycles per transport" % ((etime/N)*freq)
main()
