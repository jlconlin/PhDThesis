/*ArnoldiMCTest.cpp
 $Author: jlconlin $
 $Id: ArnoldiMCTest.cpp 284 2008-04-09 13:33:27Z jlconlin $
 $Revision: 284 $
 $Date: 2008-04-09 07:33:27 -0600 (Wed, 09 Apr 2008) $

 This is used to test the ArnoldiMC class
 */

#include<iostream>
#include "ArnoldiMCTest.h"


using std::cout;
using std::endl;

int main( int argc, char* argv[] ){
    cout << "I'm testing ArnoldiMC..." << endl;
    vector<unsigned long> seed(2, 1);
    cout << "Master seed: " << seed[0] << endl;
    
    double width = 100.0;
    int zones = 1;
    int bins = 50;

    // Geometry
    Mesh geoMesh(width, 1.0, zones);
    vector<Material> Mat(zones, Material(0.5,0.5,0.0,1.0) );
    Field<Zone, Material> Geometry(geoMesh, Mat);

    Mesh DiscMesh(width, 1.0, bins);    // Discretization Mesh

    ArnoldiMCTest AMCTest(seed, Geometry, DiscMesh);
    AMCTest.ValueStats();
    AMCTest.VectorStats();
    AMCTest.Scale();

    return 0;
}

