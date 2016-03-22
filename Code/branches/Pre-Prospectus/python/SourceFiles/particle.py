import random
import math

__author__   = "$Author: jlconlin $"
__version__ = " $Revision: 29 $"
__date__     = "$Date: 2007-03-20 12:21:00 -0600 (Tue, 20 Mar 2007) $"
__id__       = "$Id: particle.py 29 2007-03-20 18:21:00Z jlconlin $"

class particle(object):
    def __init__(self, position=(0,0,0), direction=(1,1,1), weight=1):
        object.__init__(self)
        self.position = position    # Initial Position of Particle
        self.direction = direction  # Initial Direction of travel of Particle
        self.weight = weight        # Initial Weight of Particle


    def __repr__(self):
        """
        __repr__ is:
        Return the canonical string representation of the object.
        For most object types, eval(repr(object)) == object`.
        """
        return "(x,y,z)=(%7.4f,%8.4f,%8.4f)\t(u,v,w)=(%7.4f,%8.4f,%8.4f)\tw=%7.4f" %(
                self.position[0], self.position[1], self.position[2], 
                self.direction[0], self.direction[1], self.direction[2], 
                self.weight)


    def move(self, xT):
        """
        Move will move the Particle to a new random Position.  The
        distance the Particle will travel is is sampled from the mean free path
        of the neutorn in the medium (1/xT).

        xT: Total cross section of medium
        """
        # Sample distance to collision
        Distance = -(1.0/xT)*math.log(random.random())

        # Move Particle
        x = self.position[0] + Distance*self.direction[0]
        y = self.position[1] + Distance*self.direction[1]
        z = self.position[2] + Distance*self.direction[2]
        self.position = (x,y,z)


    def move1D(self, xT):
        """
        This performs the same calculation as method self.Move but only moves in
        one dimension.  The dimension it moves in is the first entry of the
        self.Position tuple.

        xT: Total cross section of medium
        """
        # Sample distance to collision
        Distance = (1/xT)*math.log(random.random())

        # Move particle
        x = self.position[0] + Distance*self.direction[0]
        self.position = (x, 0, 0)


    def collision(self):
        """
        This must be overridden by each subclass.
        """
        raise NotImplementedError


    def setRandDirection(self):
        """
        randDirection will set the direction of the particle to a random
        direction
        """
        phi = 2*math.pi*random.random()
        u = 2*random.random() - 1
        v = math.sqrt(1-u*u)*math.cos(phi)
        w = math.sqrt(1-u*u)*math.sin(phi)
        self.direction = (u,v,w)


    def setRandDirection1D(self):
        """
        """
        # Sample new direction
        Direction = random.choice([-1,1])   
        self.direction = (Direction, 0, 0)
        


class neutron(particle):
    """
    """

    def __init__(self, position=(0,0,0), direction=(1,1,1), weight=1):
        particle.__init__(self, position=position, direction=direction, weight=weight)


    def collision(self, newWeight):
        """
        collision will take care of things that happen at a collision.  Namely,
        it will change the weight and new direction of the neutron.

        newWeight: Neutron weight *after* collision.
        """
        self.setRandDirection()     # Sample new direction
        self.weight = newWeight     # Change neutron's weight
