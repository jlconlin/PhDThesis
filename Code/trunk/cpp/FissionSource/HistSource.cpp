/*HistSource.cpp
 $Author: jlconlin $
 $Id: HistSource.cpp 447 2009-01-21 21:29:49Z jlconlin $
 $Revision: 447 $
 $Date: 2009-01-21 14:29:49 -0700 (Wed, 21 Jan 2009) $

This file provides the implementation for the HistSource class.

 */

#include<numeric>
#include "boost/tuple/tuple.hpp"
#include "Assert.hh"
#include "HistSource.h"
#include "boost/utility.hpp"
#include "CartesianMesh1D.hh"
#include "Mesh_AddOns.hh"

using boost::compressed_pair;


/*_makeDF will create a PDF and CDF for the Source.  This should only be 
called from the constructors.  The HistSource object should be a true PDF 
(i.e. should sum to 1), this function will also ensure the sum of the absolute
value is 1.*/
void HistSource::makeCDF(){
    ublas_vec_t::iterator vIter = _field.mData.begin();
    double sum = 0;
    for( ; vIter != _field.mData.end(); ++vIter ){
        sum += std::abs(*vIter);
    }

    if( sum ){  // Sum is greater than 0
        // Make into PDF
        ublas_vec_t::iterator dataIter = _field.mData.begin();
        ublas_vec_t::iterator cdfIter = _CDF.begin();
        for( ; dataIter != _field.mData.end(); ++dataIter, ++cdfIter ){
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
 * will randomize the direction of the Particle.  sample assumes that that the
 * data has been made into a CDF already.*/
void HistSource::sample( Particle& P ){
//  makeCDF();
//    assert(size() > 0);
    double probPDF = _r->Fixed();     //Probability from PDF
    const Zone<OneD>* Z = 0;

    ublas_vec_t::const_iterator cdfIter = _CDF.begin();
    ublas_vec_t::const_iterator pdfIter = _field.mData.begin();
    CartesianMesh<OneD>::const_ZoneIterator zMIter = _field.mMesh.zoneBegin();

    // Find zone from where a Particle should be sampled
    for( ; cdfIter != _CDF.end(); ++cdfIter, ++zMIter, ++pdfIter ){
        if( probPDF <= std::abs(*cdfIter) ) break;  // Found zone
    }
    
    double min = zMIter->getLeftNode().x();
    double max = zMIter->getRightNode().x();
    P._x = (max - min)*_r->Fixed() + min;  // Sample position
    P._y = 0.0;
    P._z = 0.0;
    P.setRandomDirection();

    if( *pdfIter <= 0 )  P._weight = -1.0;  // Negative weight particle
    else P._weight = 1.0;   // Positive weight particle
}

void HistSource::score( const Particle& P, double s ){
    _field[getZone(P.x())] += s;
}

/*magnitude returns the absolute value of the sum of the bins*/
double HistSource::magnitude() const {
    double sum = 0;
    ublas_vec_t::const_iterator vIter = _field.mData.begin();
    for( ; vIter != _field.mData.end(); ++vIter ){
        sum += std::abs(*vIter);
    }
    return sum;
}

/*Edges returns a vector of the positions of the Edges of the bins.*/
HistSource::ublas_vec_t HistSource::Edges() const {
    return NodePositions( _field.getMesh() );
}

/*Centers returns a vector of the center locations of each bin.*/
HistSource::ublas_vec_t HistSource::Centers() const {
    return ZoneCenters( _field.getMesh() );
}

