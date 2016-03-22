__id__       = "$Id: test_BankSource.py 230 2008-01-02 15:12:50Z jlconlin $"
__author__   = "$Author: jlconlin $"
__version__ = " $Revision: 230 $"
__date__     = "$Date: 2008-01-02 08:12:50 -0700 (Wed, 02 Jan 2008) $"

"""
test_BankSource.py is designed to test the BankSource class.
"""

from Transport.Geometry import Field, Mesh
from Transport import Material
from Transport.FissionSource import BankSource

N = 100000   # Number of particles

numZones = 10
ZoneLengths = [1.0]*10
print "Here I create a Mesh of %s zones each of length 1.0" %(numZones)
myMesh = Mesh.Mesh(ZoneLengths)  # Mesh with specified non-uniform ZoneLengths

print "Here I create a Field of the same size as the Mesh."
print "And I fill it with the same material in each Zone."
Mat1 = Material.Material(0.5, 0.5, 0.0, 1.0)  # 'Standard' material
print "The Material I use is: %s" %(Mat1)
myMaterial = [Mat1 for i in xrange(numZones)]
vecMats = Material.vectormats(myMaterial)

print "Now I define source probabilities for each Zone."
vecProbabilities = Field.DVector([1.0 for i in xrange(numZones)])

F = Field.FieldZoneMat(myMesh, vecMats)
ProbField = Field.FieldZoneDouble(myMesh, vecProbabilities)

print "Creating (not so) random seed..."
seed = BankSource.ULongVector([1])

print "Now BankSource will sample %i particles" %(N)
uniBank = BankSource.BankSource(ProbField, myMesh, seed, N)

print "The discretized source is: ",
disc = uniBank.discretized(myMesh)
print "%s" %(disc)
