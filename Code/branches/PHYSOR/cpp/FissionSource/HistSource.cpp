/*HistSource.cpp
 $Author: jlconlin $
 $Id: HistSource.cpp 278 2008-03-28 14:56:02Z jlconlin $
 $Revision: 278 $
 $Date: 2008-03-28 08:56:02 -0600 (Fri, 28 Mar 2008) $

This file provides the implementation for the HistSource class.

 */

#include<numeric>
#include "HistSource.h"
#include "boost/utility.hpp"
#include "OneDCartMesh.hh"

using boost::compressed_pair;


/*_makeDF will create a PDF and CDF for the Source.  This should only be 
called from the constructors.  The HistSource object should be a true PDF 
(i.e. should sum to 1), this function will also ensure the sum of the absolute
value is 1.*/
void HistSource::makeCDF(){
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
            *cdfIter = std::abs(*dataIter/sum);
        }

        // Make CDF from PDF
        cdfIter = _CDF.begin() + 1;  // Start loop at second element
        for( ; cdfIter != _CDF.end(); ++cdfIter ){
            // Add all previous elements to current element
            *cdfIter += *(cdfIter - 1);
        }
    }
}


/*sample will change the properties of Particle P.  It samples a Particle
 * from the underlying distribution and makes P nearly identical to it.  It 
 * will randomize the direction of the Particle.*/
void HistSource::sample( Particle& P ){
    makeCDF();
//    assert(size() > 0);
    double probPDF = _r->Fixed();     //Probability from PDF
    const Zone* Z = 0;

    vector<double>::const_iterator cdfIter = _CDF.begin();
    vector<double>::const_iterator pdfIter = mData.begin();
    Mesh::const_ZoneIterator zMIter = mMesh.zoneBegin();

    // Find zone from where a Particle should be sampled
    for( ; cdfIter != _CDF.end(); ++cdfIter, ++zMIter, ++pdfIter ){
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
    if( *pdfIter <= 0 ) { P._weight = -1.0; }
    else { P._weight = 1.0; }
}

/*size returns the absolute value of the sum of the bins*/
double HistSource::size() const {
    double sum = 0;
    typedef vector<double>::const_iterator Vd_Iter;
    for( Vd_Iter vIter = mData.begin(); vIter != mData.end(); ++vIter ){
        sum += std::abs(*vIter);
    }
    return sum;
}
