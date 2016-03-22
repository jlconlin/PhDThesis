"""Unit test for Geometry subsystem"""

__author__  = "$Author: wollaber $"
__version__ = "$Revision: 81 $"
__date__    = "$Date: 2007-03-04 12:55:47 -0500 (Sun, 04 Mar 2007) $"
__id__      = "$Id: TestGeometry.py 91 2007-03-04 17:55:47Z wollaber $"

from Transport.Geometry import Mesh
from Transport.Geometry import Field
from Transport.Material import Material
import unittest

m = Mesh.Mesh(1,1,10);
f = Field.FieldZoneDouble(m)

# Use this to test how the Material classes updates other quantities 
class KnownValues(unittest.TestCase):
	mesh_length = 1.0; mesh_area=1.0; num_zones=10;
	len_vector = [1.0, 2.0, 3.0];
	
	def  testInstantiation(self):
		"""A Mesh and a Field should instantiate in python"""
		m = Mesh.Mesh(self.mesh_length, self.mesh_area, self.num_zones);
		fzd = Field.FieldZoneDouble(m);
		fzn = Field.FieldNodeDouble(m);
		fzm = Field.FieldZoneMat(m);
		self.assertEqual(m.numZones(), self.num_zones) 
		self.assertEqual(m.length(), self.mesh_length) 
		self.assertEqual(m.area(), self.mesh_area) 
		m2 = Mesh.Mesh(self.len_vector, 1.0)
		self.assertEqual(m2.numZones(), len(self.len_vector))
		fzd2 = Field.FieldZoneDouble(m2, self.len_vector)
		self.assertEqual(m2.numZones(), fzd2.size())

	def testFieldIteration(self):
		"""We should be able to iterate over a Field to retrieve our values."""
		#  First a zone-centered field of doubles
		m = Mesh.Mesh(self.len_vector);
		vds = [11,22,33];
		fzd = Field.FieldZoneDouble(m, vds);
		for i,j in zip(vds, fzd):
			self.assertEqual(i,j);
		# Now check a field of three Materials
		m1 = Field.Material(vds[0], 1,1,1,1);
		m2 = Field.Material(vds[1], 1,1,1,1);
		m3 = Field.Material(vds[2], 1,1,1,1);
		vms = Material.vectormats( [m1,m2, m3]);
		fms = Field.FieldZoneMat(m, vms);
		for i,j in zip(vds, fms):
			self.assertEqual(i,j.sigt());

	def testMeshIteration(self):
		"""We should be able to iterate over a Mesh to get Zone, Node data."""
		vds = [11,22,33];
		nds = [0, 11, 33, 66];
		lpos = nds[0:-1];
		rpos = nds[1:];
		isonbnd = [True, False, False, True];
		m = Mesh.Mesh(vds)
		for l,z in zip(vds, m.Zones()):
			self.assertEqual(l, z.length())
		for x,n in zip(nds, m.Nodes()):
			self.assertAlmostEqual(x, n.x() );
		for b,n in zip(isonbnd, m.Nodes()):
			self.assertEqual(b, n.onBoundary() );
		for x,z in zip(lpos, m.Zones()):
			self.assertAlmostEqual(x, z.getNodeLeft().x() );
		for x,z in zip(rpos, m.Zones()):
			self.assertAlmostEqual(x, z.getNodeRight().x() );
		
	def testFieldMeshIterators(self):
		"""Field should take a GeomElem and return its data."""
		vds = [11, 22, 33];
		zdata = [10,20,30];
		ndata = [5,15,25,35];
		m = Mesh.Mesh(vds);
		fz = Field.FieldZoneDouble(m, zdata);
		fn = Field.FieldNodeDouble(m, ndata);
		for zd, z in zip(zdata, m.Zones() ):
			self.assertEqual(zd, fz[z] )
		for nd, n in zip(ndata, m.Nodes() ):
			self.assertEqual(nd, fn[n])
		
		
		

# Run all tests if this script is called	
if __name__ == "__main__":
	unittest.main()
