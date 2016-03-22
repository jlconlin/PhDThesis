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
#include "Field.hh"


using std::cout;
using std::endl;


int main(void)
{
	cout << endl << endl
		 << "Welcome to the One-Dimensional Field Test Program" 
		 << endl << endl;
	
	cout << "First, we'll create a uniform mesh of length 15, area 2, with 15 zones" << endl;
	
	Mesh mesh(15.0, 2.0, 15);

	cout << "Now, we'll create a zone-centered field of doubles with all zeroes" << endl;
	Field<Zone, double> zcfield(mesh);
	cout << "The size of this Field is " << zcfield.size() << endl;
	
	cout << "Field1: " << endl;
        for (Mesh::ZoneIterator iter = mesh.zoneBegin(); iter != mesh.zoneEnd(); ++iter)
	{
		//cout << "Zone " << iter->id() << ": " << zcfield[mesh[iter->id()] ] << endl;
		cout << "Zone " << iter->id() << ": " << zcfield[*iter] << endl;
	}
	cout << endl;
	
	cout << "Now, we'll create a node-centered field of vector<double> of length 4, with values equal to 0 1 2 3" << endl;
	Field< Node, vector<double> > ncfield(mesh, vector<double>(4));
	cout << "The size of this Field is " << ncfield.size() << endl;
	// Fill the field
	typedef Field<Node, vector<double> >::iterator ncfiter;
	for(ncfiter iter = ncfield.begin(); iter != ncfield.end(); ++iter)
	{
		for (int j=0; j<4; ++j)
		{
			(*iter)[j] = j;   // two ways of doing this: first is here
		} 
	}
	
	cout << "Field2: " << endl;
	for(Mesh::NodeIterator iter = mesh.nodeBegin(); iter != mesh.nodeEnd(); ++iter)
	{
		cout << "Node " << iter->id() << ": ";
		for(int j=0; j<4; ++j)
		{
			cout << ncfield[mesh.getNode( iter->id() )][j] << "  "; // second is here
		}
		cout << endl;
	}

	cout << "Now we'll try creating a (deep) copy of a Field." << endl;
	Field<Zone, double> fieldcopy(zcfield);	

	cout << "Changing the first element of the COPY to 22."  << endl;
	typedef Field<Zone, double >::iterator zcfiter;
	zcfiter firstinold = zcfield.begin();
	zcfiter firstinnew = fieldcopy.begin();
	*firstinnew = 22.0;	

	cout << "The first two elements the old field are: " << *firstinold  <<
	 "  " << *(++firstinold) << "." << endl;
	cout <<" The first two elements in the copied field are now: " <<
		*firstinnew << "   " << *(++firstinnew) << endl;
	return 0;
}
