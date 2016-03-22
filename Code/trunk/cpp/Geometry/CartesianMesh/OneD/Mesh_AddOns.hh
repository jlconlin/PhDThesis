/*Mesh_AddOns.hh
 $Author: jlconlin $
 $Id: Mesh_AddOns.hh 418 2008-11-17 18:16:28Z jlconlin $
 $Revision: 418 $
 $Date: 2008-11-17 11:16:28 -0700 (Mon, 17 Nov 2008) $

 * This file provides extra functionality to the Geometry Mesh class that isn't
 * provided by the original.
 */

#ifndef MESH_ADDONS_HH
#define MESH_ADDONS_HH

#include <iostream>
#include <algorithm>
#include "boost/numeric/ublas/vector.hpp"
#include "CartesianMesh1D.hh"

using std::cout;
using std::endl;

namespace ublas = boost::numeric::ublas;

inline bool ZoneComp( const Zone<OneD>&, double);
inline const Zone<OneD>& getZone(CartesianMesh<OneD>&, double);

inline ublas::vector<double> NodePositions( const CartesianMesh<OneD>& M ){
    ublas::vector<double> ReturnMe( M.numNodes() );
    ublas::vector<double>::iterator vIter = ReturnMe.begin();
    CartesianMesh<OneD>::const_NodeIterator NIter = M.nodeBegin();
    
    for( ; NIter != M.nodeEnd(); ++NIter, ++vIter ){
        *vIter = NIter->x();
    }
    return ReturnMe;
}

inline ublas::vector<double> ZoneCenters( const CartesianMesh<OneD>& M ){
    ublas::vector<double> ReturnMe( M.numZones() );
    ublas::vector<double>::iterator vIter = ReturnMe.begin();
    CartesianMesh<OneD>::const_ZoneIterator ZIter = M.zoneBegin();

    for( ; ZIter != M.zoneEnd(); ++ZIter, ++vIter ){
        *vIter = ZIter->center().x();
    }

    return ReturnMe;
}

#endif


