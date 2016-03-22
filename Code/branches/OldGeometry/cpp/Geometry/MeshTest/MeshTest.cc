// -------------------------------------------- # C++ # ------------------------------------
// Programmer: Greg Davidson
// Project: Summer 2005 LANL Diffusion - Monte Carlo Hybrid
// June 8, 2005
//
// FILE: MeshTest.cc

// The purpose of this file is to test all of the OneDCartMesh package's
// functionality

#include <iostream>
#include <iomanip>

#include "OneDCartMesh.hh"

using std::cout;
using std::endl;
using std::setw;
using std::left;
using std::right;
using std::fixed;
using std::showpoint;
		
void print_info(const Mesh& mesh);

int main(void)
{
	cout << endl << endl
		 << "Welcome to the One-Dimensional Cartesian Mesh Test Program" 
		 << endl << endl;
	
	cout << "Test 1: Uniform mesh of length 15, area 2, with 15 zones" << endl;
	
	Mesh mesh1(15.0, 2.0, 15);
	print_info(mesh1);
	
	cout << endl << endl
		 << "Test 2: Zone sizes grow geomtrically, area 3.5, with 25 zones"
		 << endl;
	vector<double> zone_lengths(25);
	zone_lengths[0] = 1.0;
	unsigned int zone_counter;
	vector<double>::iterator iter = zone_lengths.begin();
	++iter;
	for(; iter != zone_lengths.end(); ++zone_counter, ++iter)
	{
		*iter = *(iter-1)*2;
	}

	Mesh mesh2(zone_lengths, 3.5);
	print_info(mesh2);
	
	cout << endl << "Test the Zone Iterator on a non-uniform mesh with length 10, area 1, with 5 zones" << endl;
	vector<double> zone_lengths2(5);
	zone_lengths[0] = 1.0;
	zone_lengths[1] = 2.0;
	zone_lengths[2] = 3.0;
	zone_lengths[3] = 2.5;
	zone_lengths[4] = 1.5;
	
	Mesh mesh3(zone_lengths2, 1.0);
	
	// Iterate over the zones
	for(Mesh::ZoneIterator iter = mesh3.zoneBegin(); iter != mesh3.zoneEnd(); ++iter)
	{
		cout << "Zone " << iter->id() << endl;
	}
	
	return 0;
}


void print_info(const Mesh& mesh)
{
	cout << "Mesh information:" << endl;
	cout << "  Length = " << mesh.length() << endl;
	cout << "  Area = " << mesh.area() << endl;
	cout << "  Volume = " << mesh.volume() << endl;
	cout << "  Number of zones = " << mesh.numZones() << endl;
	cout << "  Number of nodes = " << mesh.numNodes() << endl << endl;
	
	cout << "Zone information: " << endl;
	
	for(unsigned int i=0; i<mesh.numZones(); ++i)
	{
		const Zone::Id id(i);
		const Zone& z = mesh.getZone(id);
		cout.precision(3);		
		cout << setw(2) << left << "  ";
		cout << setw(5) << left << "Zone ";
		cout << setw(4) << right << z.id(); 
		cout << setw(3) << ":  ";
		cout << setw(9) << left << "Length = ";
		cout << fixed << showpoint << setw(5) << left << z.length();
		cout << setw(9) << right << "Area = ";
		cout << setw(7) << left << z.area();
		cout << setw(9) << right << "Volume = ";
		cout << setw(5) << left << z.volume();
		cout << setw(21) << right << "Connected to nodes ";
		cout << right;
		cout << setw(3) << z.getNodeLeft().id();
		cout << setw(1) << " ";
		cout << setw(3)	<< z.getNodeRight().id() << endl;
	}
	
	cout << "Node information: " << endl;
	for(unsigned int i=0; i<mesh.numNodes(); ++i)
	{
		const Node::Id id(i);
		const Node& n = mesh.getNode(id);
		cout.precision(3);		
		cout << setw(2) << left << "  ";
		cout << setw(5) << left << "Node ";
		cout << setw(4) << right << n.id(); 
		cout << setw(3) << ":  ";
		cout << setw(11) << left << "Position = ";
		cout << fixed << showpoint << setw(7) << right << n.x();
		cout << setw(19) << right << "Connected to zone";
		
		
		// Get the left zone
		Zone const* zleft;
		
		try
		{
			zleft = &n.getZoneLeft();
		}
		catch(NoZoneConnected)
		{
			zleft = 0;
		}
		
		Zone const* zright;
		try
		{
			zright = &n.getZoneRight();
		}
		catch(NoZoneConnected)
		{
			zright = 0;
		}
		//cout << "Got l and r" << endl;
		if(zleft != 0 && zright != 0)
		{
			cout << "s";
			cout << setw(4) << right << n.getZoneLeft().id();
			cout << setw(4) << right << n.getZoneRight().id() << endl;
		}
		else if(zleft != 0)
		{
			cout << " ";
			cout << setw(4) << right << n.getZoneLeft().id() << endl;
		}
		else
		{
			cout << " ";
			cout << setw(4) << right << n.getZoneRight().id() << endl;
		}
	}
}
