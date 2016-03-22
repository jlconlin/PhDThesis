/*test_Particle.cpp
 $Author: jlconlin $
 $Id: test_Field.cpp 218 2007-12-13 16:30:57Z jlconlin $
 $Revision: 218 $
 $Date: 2007-12-13 09:30:57 -0700 (Thu, 13 Dec 2007) $

 This file is used to begin to understand how to use the Field class
 and to experiment how to fill a Field with some Materials.

 */

#include<iostream>
#include<vector>
#include<utility>

#include "OneDCartMesh.hh"
#include "Field.hh"
#include "Assert.hh"

#include "Material.h"

using std::cout;
using std::endl;
using std::vector;
using std::pair;

using boost::compressed_pair;

template <typename T> void printVector(vector<T>);
Field<Zone, Material> makeAndFill(vector<Material>, vector<int>, const Mesh&);

int main(void){
    cout << "I will create a mesh of length 12 with 5 zones." << endl;

    vector<double> widths(5);
    widths[0] = 1.0;
    widths[1] = 2.0;
    widths[2] = 5.0;
    widths[3] = 3.0;
    widths[4] = 1.0;
    cout << "Widths of bins:\n";
    printVector<double>(widths);

    //Create mesh with given widths
    Mesh mesh(widths, 1.0);
    cout << "\nNode positions:\n";
    for (int i = 0; i < mesh.numZones(); i++){
        const Zone::Id zId(i);
        const Zone& z = mesh.getZone(zId);
        const compressed_pair<const Node*, const Node*> zoneNodes = z.getNodes();
        cout << "[" << zoneNodes.first()->x()
        << " | " << z.center() << " | "
        << zoneNodes.second()->x() << "]"
        << " -- width = " << widths[i] << "\n";

    }

    vector<Material> myMaterials(3);
    myMaterials[0] = Material(0.3, 0.3, 0.4, 1.0);
    myMaterials[1] = Material(1.0, 1.0, 0.0, 2.0);
    myMaterials[2] = Material(1.0, 1.0, 1.0, 3.0);
    cout << "\nMaterials used in problem:\n";
    printVector<Material>(myMaterials);

    vector<int> materialOrder(5);
    materialOrder[0] = 0;
    materialOrder[1] = 2;
    materialOrder[2] = 2;
    materialOrder[3] = 1;
    materialOrder[4] = 1;
    cout << "\nOrder of materials used:\n";
    printVector<int>(materialOrder);

    cout << "\nNow I'm creating my Field.\n";
    Field<Zone, Material> myField = makeAndFill(myMaterials, materialOrder, mesh);
    cout << "This is what my Field looks like:\n";
    typedef Field<Zone, Material>::iterator zcfiter;
    for(zcfiter iter = myField.begin(); iter != myField.end(); iter++){
        cout << *iter << "\n";
    }

//  cout << "Can I access the members of the Field through indexes?\n";
//  for ( int i = 0; i < myField.size(); i++){
//      cout << "(" << i << "): " << myField[i] << "\n";
//  }
//  cout << "Anser: Of course, after I modify the class :D\n";


    return 0;
}

/*Function to print out elements of the vector.*/
template <typename T> void printVector(vector<T> v){
    for( int i = 0; i < v.size(); i++ ){
        cout << "(" << i << "): " << v[i] << "\n";
    }
}

/*Function to fill Field Zones with materials and return a Field.
 *
 * Mats: Materials used in this geometry
 * matNums: Order of Mats elements in geometry
 * widths: width of bins.*/
Field<Zone, Material> makeAndFill(const vector<Material> Mats, 
        const vector<int> matNums, const Mesh& myMesh)
{
    int size = matNums.size();
    Assert(size == myMesh.numZones());
    
    vector<Material> materialData(size);

    int materialNumber;
    for (int i = 0; i < size; i++){
        materialNumber = matNums[i];
        materialData[i] = Mats[materialNumber];
    }

    Field<Zone, Material> returnField (myMesh, materialData);
    return returnField;
}
