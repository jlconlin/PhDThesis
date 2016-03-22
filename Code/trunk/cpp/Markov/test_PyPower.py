__id__       = "$Id: test_PyPower.py 242 2008-01-08 19:08:09Z jlconlin $"
__author__   = "$Author: jlconlin $"
__version__ = " $Revision: 242 $"
__date__     = "$Date: 2008-01-08 12:08:09 -0700 (Tue, 08 Jan 2008) $"

"""
test_PyPower.py is designed to merely test the functionality of Power.py
"""
import scipy
import pylab

from Transport.Geometry import Field
from Transport.Geometry import Mesh
from Transport import Material
from Transport.FissionSource import BankSource
#from Transport.Markov import Power

import PowerMC

N = 5000   # Number of particles
active = 100
inactive = 20

numZones = 10
ZoneLengths = [1.0]*10
# print "Here I create a Mesh of %s zones each of length 1.0" %(numZones)
myMesh = Mesh.Mesh(ZoneLengths)  # Mesh with specified non-uniform ZoneLengths

# print "Here I create a Field of the same size as the Mesh."
# print "And I fill it with the same material in each Zone."
Mat1 = Material.Material(0.5, 0.5, 0.0, 1.0)  # 'Standard' material
Mat2 = Material.Material(0.1, 0.9, 0.0, 3.0)  # Highly fissioning material
# print "The Material I use is: %s" %(Mat1)
myMaterial = [Mat1 for i in xrange(numZones)]
myMaterial[2] = Mat2
myMaterial[3] = Mat2
myMaterial[7] = Mat2
myMaterial[6] = Mat2
vecMats = Material.vectormats(myMaterial)

# print "Now I define source probabilities for each Zone."
vecProbabilities = Field.DVector([1.0 for i in xrange(numZones)])

F = Field.FieldZoneMat(myMesh, vecMats)
ProbField = Field.FieldZoneDouble(myMesh, vecProbabilities)

# print "Creating (not so) random seed..."
seed = BankSource.ULongVector([1])

# print "Now BankSource will sample %i particles" %(N)
uniBank = BankSource.BankSource(ProbField, myMesh, seed, N)

# print "The discretized source is: ",
disc = uniBank.discretized(myMesh)
# print "%s" %(disc)

print "\nNow running power method with %i particles per iteration," %(N)
print "%i inactive iterations, and %i active iterations.\n" %(inactive, active)
discMesh = Mesh.Mesh([0.2]*50)  # Mesh for discretizing eigenvector
pmc = PowerMC.Power(seed, F)
pmc.power(uniBank, active, inactive, N, discMesh)

print "\nPlotting resulting eigenvector."
meanVector = scipy.mean(pmc.eigVector, 0)
pylab.plot(meanVector, linestyle='steps')
pylab.show()

