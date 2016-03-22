/*HistSource.cpp
 $Author: jlconlin $
 $Id: HistSource.cpp 238 2008-01-05 22:05:09Z jlconlin $
 $Revision: 238 $
 $Date: 2008-01-05 15:05:09 -0700 (Sat, 05 Jan 2008) $

This file provides the implementation for the HistSource class.

 */

#include<numeric>
#include "HistSource.h"
#include "boost/utility.hpp"

using boost::compressed_pair;


/*_makeDF will create a PDF and CDF for the Source.  This should only be 
called from the constructors.  The HistSource object should be a true PDF 
(i.e. should sum to 1), this function will also ensure the sum of the absolute
value is 1.*/
void HistSource::makeDF(){
    typedef vector<double>::iterator Vd_Iter;
    double sum = 0;
    for( Vd_Iter vIter = mData.begin(); vIter != mData.end(); ++vIter ){
        sum += std::abs(*vIter);
    }

    if( sum ){  // Sum is greater than 0
        // Make into PDF
        Vd_Iter dataIter = mData.begin();
        Vd_Iter cdfIter = _CDF.begin();
        for( ; dataIter != mData.end(); ++dataIter, ++cdfIter ){
            *dataIter = *dataIter/sum;
            *cdfIter = *dataIter;

        }

        // Make CDF from PDF
        cdfIter = _CDF.begin() + 1;  // Start loop at second element
        for( ; cdfIter != _CDF.end(); ++cdfIter ){
            // Add all previous elements to current element
            *cdfIter += *(cdfIter - 1);
        }
    }
//    std::cout << "Printing data for debugging\n"
//              << "\tPDF\t\tCDF\t\t\n\t---\t\t---\n";
//    for( int i = 0; i < _CDF.size(); ++i ){
//        std::cout << "\t" << mData[i] << "\t\t" << _CDF[i] << "\n";
//    }

}


/*sample will change the properties of Particle P.  It samples a Particle
 * from the underlying distribution and makes P nearly identical to it.  It 
 * will randomize the direction of the Particle.*/
void HistSource::sample( Particle& P ){
//    assert(size() > 0);
    double probPDF = _r->Fixed();     //Probability from PDF
    const Zone* Z = 0;

    vector<double>::const_iterator cdfIter = _CDF.begin();
    Mesh::const_ZoneIterator zMIter = mMesh.zoneBegin();

    // Find zone from where a Particle should be sampled
    for( ; cdfIter != _CDF.end(); ++cdfIter, ++zMIter ){
        if( probPDF <= std::abs(*cdfIter) ) 
            { Z = &(*zMIter); break; } //Ugly!
    }
    
    compressed_pair<const Node*, const Node*> zN;      //Nodes around a Zone
    zN = Z->getNodes();
    double min = zN.first()->x();
    double max = zN.second()->x();
    P._x = (max - min)*_r->Fixed() + min;  // Sample position
    P._y = 0.0;
    P._z = 0.0;
    P.setRandomDirection();
    P._geoZone = Z;
    if( *cdfIter <= 0 ) { P._weight = -1.0; }
    else { P._weight = 1.0; }
}

/**/
int HistSource::size() const {
    typedef vector<double>::const_iterator Vd_Iter;
    double sum = 0;
    for( Vd_Iter vIter = mData.begin(); vIter != mData.end(); ++vIter ){
        sum += std::abs(*vIter);
    }
    return sum;
}
