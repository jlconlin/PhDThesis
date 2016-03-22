/*test_Geometry.cpp
 $Author: jlconlin $
 $Id: test_Geometry.cpp 480 2009-03-10 16:56:40Z jlconlin $
 $Revision: 480 $
 $Date: 2009-03-10 10:56:40 -0600 (Tue, 10 Mar 2009) $

 This file is used for testing/learning about the new Geometry libraries by
 Greg Davidson.

 */

#include<iostream>
#include<vector>
#include<utility>
#include "boost/numeric/ublas/vector.hpp"
// #include "boost/tuple/tuple.hpp"
#include "CartesianMesh1D.hh"
#include "Field.hh"

using std::cout;
using std::endl;
using std::vector;

namespace ublas = boost::numeric::ublas;

void MeshTest();
void FieldTest();

int main(int argc, char* argv[] ){
    cout << "I'm testing/learning the new Geometry." << endl;

    MeshTest();
    FieldTest();

    return 0;
}

void MeshTest(){
    cout << "\n====================================\n"
         << "Testing/learning the new Mesh class." 
         << "\n====================================\n" << endl;

    cout << "Creating a Mesh with 10 zones and total length of 7.0." << endl;
    CartesianMesh<OneD> MyMesh(7.0, 1.0, 10.0);
    cout << "Mesh length (should be 7) = " << MyMesh.length()
         << "\nMesh area (should be 1) = " << MyMesh.area()
         << "\nMesh volume (should be 7) = " << MyMesh.volume() 
         << "\nNumber of zones (should be 10) = " 
         << MyMesh.numElements< Zone<OneD> >() 
         << endl;

    cout << "\nLet's iterate over the Mesh to see the Node locations." << endl;
    CartesianMesh<OneD>::NodeIterator NIter = MyMesh.nodeBegin();
    int i = 0;
    for( ; NIter != MyMesh.nodeEnd(); ++NIter, ++i ){
        cout << "Node #: " << i << ", x = " << NIter->x();
        if( NIter->onBoundary() ){
            if( NIter->onLeftBoundary() ) cout << ", on LeftBoundary." << endl;
            if( NIter->onRightBoundary() ) cout << ", on RightBoundary." << endl;
        }
        else cout << ", not on Boundary." << endl;

    }

    cout << "\nLet's iterate over the Mesh to see the Zone locations." << endl;
    CartesianMesh<OneD>::ZoneIterator ZIter = MyMesh.zoneBegin();
    i = 0;
    for( ; ZIter != MyMesh.zoneEnd(); ++ZIter, ++i ){
        cout << "Zone #: " << i << ", length = " << ZIter->length() 
             << ", center = " << ZIter->center().x() << endl;
        const std::pair<const Node<OneD>*, 
              const Node<OneD>*>& ZoneNodes = ZIter->getNodes();
//      const Node<OneD>& Left = ZIter->getLeftNode();
//      const Node<OneD>& Right = ZIter->getRightNode();
        const Node<OneD>& Left = *(ZoneNodes.first);
        const Node<OneD>& Right = *(ZoneNodes.second);
        cout << "\tLeft Node pos = " << Left.x() 
             << ", Right Node pos = " << Right.x() << endl;
    }

    vector<double> ZoneLengths(5);
    ZoneLengths[0] = 1.0;
    ZoneLengths[1] = 2.0;
    ZoneLengths[2] = 3.0;
    ZoneLengths[3] = 2.0;
    ZoneLengths[4] = 1.0;
    cout << "\n==================================================\n"
         << "Creating a Mesh with 5 zones and total length of 9.0."
         << "\nThe widths of the zones are 1,2,3,2,1 respectively." << endl;

    CartesianMesh<OneD> UnequalMesh(ZoneLengths, 1.0);
    cout << "Mesh length (should be 9) = " << UnequalMesh.length()
         << "\nMesh area (should be 1) = " << UnequalMesh.area()
         << "\nMesh volume (should be 9) = " << UnequalMesh.volume() 
         << "\nNumber of zones (should be 5) = " 
         << UnequalMesh.numElements< Zone<OneD> >() 
         << endl;

    cout << "\nLet's iterate over the Mesh to see the Node locations." << endl;
     NIter = UnequalMesh.nodeBegin();
    i = 0;
    for( ; NIter != UnequalMesh.nodeEnd(); ++NIter, ++i ){
        cout << "Node #: " << i << ", x = " << NIter->x();
        if( NIter->onBoundary() ){
            if( NIter->onLeftBoundary() ) cout << ", on LeftBoundary." << endl;
            if( NIter->onRightBoundary() ) cout << ", on RightBoundary." << endl;
        }
        else cout << ", not on Boundary." << endl;
    }

    cout << "\nLet's iterate over the Mesh to see the Zone locations." << endl;
    ZIter = UnequalMesh.zoneBegin();
    i = 0;
    for( ; ZIter != UnequalMesh.zoneEnd(); ++ZIter, ++i ){
        cout << "Zone #: " << i << ", length = " << ZIter->length() << endl;
        const Node<OneD>& Left = ZIter->getLeftNode();
        const Node<OneD>& Right = ZIter->getRightNode();
        cout << "\tLeft Node pos = " << Left.x() 
             << ", center = " << ZIter->center().x()
             << ", Right Node pos = " << Right.x() << endl;
    }
}

void FieldTest(){
    cout << "\n====================================\n"
         << "Testing/learning the new Field class." 
         << "\n====================================\n" << endl;

    typedef ublas::vector<double> vec_t;
    typedef Field<Zone1D, double, vec_t> Field_t;
    // Initialize ZoneLengths and Field 'Probabilities'
    vector<double> ZoneLengths(7);      vec_t Probabilities(7);
    ZoneLengths[0] = 1.0;               Probabilities[0] = 4.0;
    ZoneLengths[1] = 2.0;               Probabilities[1] = 3.0;
    ZoneLengths[2] = 3.0;               Probabilities[2] = 2.0;
    ZoneLengths[3] = 4.0;               Probabilities[3] = 1.0;
    ZoneLengths[4] = 3.0;               Probabilities[4] = 2.0;
    ZoneLengths[5] = 2.0;               Probabilities[5] = 3.0;
    ZoneLengths[6] = 1.0;               Probabilities[6] = 4.0;
    CartesianMesh<OneD> FieldMesh(ZoneLengths, 1.0);

    cout << "Creating a Field with inputs of ZoneLengths and 'Probabilities'." 
         << endl;
    Field_t MyField(FieldMesh, Probabilities);

    cout << "\nLet's iterate over the Field and extract the information." << endl;
    Field_t::iterator FIter = MyField.begin();
    int i = 0;
    for( ; FIter != MyField.end(); ++FIter, ++i ){
        cout << "Zone #: " << i << ", probability = " << *FIter << endl;
    }
    
    
    

}
