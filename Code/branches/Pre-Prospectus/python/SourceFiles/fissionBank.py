
import random
import copy

__author__   = "$Author: jlconlin $"
__version__ = " $Revision: 56 $"
__date__     = "$Date: 2007-04-26 14:29:43 -0600 (Thu, 26 Apr 2007) $"
__id__       = "$Id: fissionBank.py 56 2007-04-26 20:29:43Z jlconlin $"


class fissionBank(list):
    """
    fissionBank contains all the required information to contain and retrieve
    particles from a fission bank.
    """

    def __init__(self, debug=0):
        """
        """
#       self.particles = []
        list.__init__(self)
#       self.length = 0     # length is the total weight of the bank


    def _get_length(self):
        """
        """
        length = 0
        for item in self:
            length += item.weight

        return length


    length = property(fget=_get_length)


    def __repr__(self):
        """
        """
        printed = ''
        for i in xrange(len(self)):
            printed = printed + "#%4i  %s\n" %(i,self[i])

        return printed
            

    def append(self, particle, number=1):
        """
        append will add one more particle to the fission bank.

        particle: The particle to be added to the bank
        number: How many particles to add to the bank
        """
        for i in xrange(int(number)):
            list.append(self, copy.copy(particle))


    def printBank(self, start=0, end=0):
        """
        print will print the position, direction, and weight of the particles in
        the fission bank.

        start: First particle to print
        end: Last particle to print
        """
        if end == 0:
            end = len(self)

        for i in xrange(end-start):
            print "#%4i  %s" %(i+start, self[i])


    def sample(self, particles):
        """
        sample will randomly sample from the bank and return a new bank.

        bank: fissionBank to sample from
        particles: How many particles to sample from self and add to new bank.
        """

        length = self.length
        newbank = self.__new__(fissionBank)
        newbank.__init__()

        difference = particles - len(self)
        if difference > 0:
            for i in xrange(difference):
                n = random.choice(self)
                if n.weight < 0:
                    n.weight = -1
                else:
                    n.weight = 1
                n.setRandDirection()
                newbank.append(n)

        for i in xrange(particles - len(newbank)):
            j = random.randint(1, len(self))
            n = self.pop(j-1)
            if n.weight < 0:
                n.weight = -1
            else:
                n.weight = 1
            n.setRandDirection()
            newbank.append(n)

        return newbank

