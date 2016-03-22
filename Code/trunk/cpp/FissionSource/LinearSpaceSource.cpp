/*LinearSpaceSource.cpp

 This file is the implementation details for LinearSpaceSource and LinearSpace 
 classes.

*/

#include <iostream>
#include "Particle.h"
#include "LinearSpaceSource.h"

// --------------LinearSpace----------------------------------------------
double LinearSpace::sample( RandomLib::Random* r ){
    double xi = r->Fixed();
    if( _straddled ){
        if( xi < _pLeft ) { ++_cLeft; return _sLeft->sample(r); }
        else { ++_cRight; return _sRight->sample(r); }
    }
    else{
        double d = _width*std::sqrt( xi );
        if( r->Fixed() < _yRatio ) d = _xmax - d;
        else d = _xmin + d;

        return d;
    }
}

double LinearSpace::area() const{
    return 0.5*_width*(2*_b + _m*(_xmax+_xmin));
}

/*magnitude returns the total weight of scores in this space.*/
double LinearSpace::magnitude() const{
    return _alpha;
}

/*_magnitude returns the total area under the curve; the area of the positive 
 * part plus the area of the negative part.*/
double LinearSpace::_Magnitude() {

    if( y(_xmin)*y(_xmax) < -1E-14 ){    // Soft-equivalence
        if( not _straddled ){   // Have not created straddled sources.
            // Straddled 
            _straddled = true;
            double mid = -_b/_m;
            double slopeLeft, slopeRight;
            double interceptLeft, interceptRight;
            if( _m < 0 ){   // Left side is positive
                slopeLeft = _m;     slopeRight = -_m;
                interceptLeft = _b; interceptRight = -_b;
            }
            else{   // Right side is positive
                slopeLeft = -_m;     slopeRight = _m;
                interceptLeft = -_b; interceptRight = _b;
            }

            // Make two new spaces, both all positive
            _sLeft = new LinearSpace(slopeLeft, interceptLeft, _xmin, mid);
            _sRight = new LinearSpace(slopeRight, interceptRight, mid, _xmax);

            // Get areas/probibilites of picking from each side
            _pLeft = std::fabs(_sLeft->area());
            _pRight = std::fabs(_sRight->area());
        }
        return  _pLeft + _pRight;
    }
    else{   // Not straddled
        return std::fabs( area() );
    }
}

void LinearSpace::score( const Particle& P, double s){
//  _alpha(s);
//  _beta( s*(P.x() - _xmid) );
    _alpha += s;
    _value = s*(P.x() - _xmid);
    _beta += _value;
    _betaSquared += _value*_value;
//  ++_count;
}

void LinearSpace::PostScoring( int N ){

    // Calculate means and standard deviations
//  _m = 12.0/(_width*_width*_width)*accum::mean(_beta);
//  _b = (accum::mean(_alpha) - _m/2.0*(_xmax*_xmax - _xmin*_xmin))/_width;
    _beta /=N;
    _betaSquared /=N;
    double variance = _betaSquared - _beta*_beta;

    // If variance is bigger than the _beta, _beta is within the noise
//  if( abs(std::sqrt(variance)) > abs(_beta) ){
//      _beta = 0;
//      cout << "Noisy" << endl;
//  }

    _m = 12.0/(_width*_width*_width)*_beta;
    _b = (_alpha/N - _m*0.5*(_xmax*_xmax - _xmin*_xmin))/_width;

}

void LinearSpace::Normalize(){
    double a = _Magnitude();
    if( a != 0.0 ){
        _m /= a;
        _b /= a;
        _alpha = a;
    }

    _yRatio = y(_xmin)/(y(_xmin) + y(_xmax) );

    // Normalize internal stuff
    if( _straddled ){
        double pTotal = _pLeft + _pRight;
        _pLeft /= pTotal;
        _pRight /= pTotal;

        _sLeft->Normalize();
        _sRight->Normalize();
    }

}

// --------------LinearSpaceSource---------------------------------------------

LinearSpaceSource::LinearSpaceSource( const CartesianMesh<OneD>& M, 
            const std::vector<unsigned long>& seed )
    : _Space(0), _PDF(M.numZones()), _CDF(M.numZones()), FissionSource(seed)
    {
        std::vector<LinearSpace> Spaces;    Spaces.reserve( M.numZones() );

        CartesianMesh<OneD>::const_ZoneIterator zMIter = M.zoneBegin();

        double xmin, xmax;
        for( ; zMIter != M.zoneEnd(); ++zMIter ){
            xmin = zMIter->getLeftNode().x();
            xmax = zMIter->getRightNode().x();

            // LinearSpace with zero slope and intercept
            Spaces.push_back(LinearSpace(xmin, xmax) );
        }

        _Space = new field_t(M, Spaces);
    }

void LinearSpaceSource::sample( Particle& P ){
    ublas_vec_t::iterator cdfIter = _CDF.begin();
    field_t::iterator SIter = _Space->begin();

    double p = _r->Fixed();
//  cout << "p = " << p << endl;
    for( ; cdfIter != _CDF.end(), SIter != _Space->end(); ++cdfIter, ++SIter ){
//      cout << "CDF: " << *cdfIter << ", Space: " << *SIter << endl;
        if( p <= std::abs(*cdfIter) ) break;    // Found zone
    }
    
    P._x = SIter->sample(_r);
    P._y = 0.0;
    P._z = 0.0;
    P.setRandomDirection();
    double y_x = SIter->y(P._x);
    if( y_x < 0 ) P._weight = -1.0; // Negative weight particle
    else P._weight = 1.0;               // Positive weight particle
}

double LinearSpaceSource::magnitude() const{
    double mag(0.0);
    field_t::iterator SIter = _Space->begin();
    for( ; SIter != _Space->end(); ++SIter ){
        mag += SIter->magnitude();
    }

    return mag;
}

void LinearSpaceSource::score( const Particle& P, double s){
    (*_Space)[_Space->getZone(P._x)].score(P, s);
}

void LinearSpaceSource::Normalize(){
    _MakeDistributionFunctions();

    field_t::iterator SIter = _Space->begin();
    for( ; SIter != _Space->end(); ++SIter ){
        SIter->Normalize();
    }
}

void LinearSpaceSource::PostScoring( int N ){
    field_t::iterator SIter = _Space->begin();
    for( ; SIter != _Space->end(); ++SIter ) SIter->PostScoring( N );
}

/*PDF does *not* return _PDF.  It returns a vector of alternating slopes and
 * intercepts for the contained LinearSpaces.*/
LinearSpaceSource::ublas_vec_t LinearSpaceSource::PDF() const{
    ublas_vec_t PDF(_Space->size()*2);

    field_t::const_iterator SIter = _Space->begin();
    ublas_vec_t::iterator pdfIter = PDF.begin();
    for( ; SIter != _Space->end(); ++SIter, ++pdfIter ){
        *pdfIter = SIter->slope();
        ++pdfIter;
        *pdfIter = SIter->intercept();
    }
    return PDF;
}

void LinearSpaceSource::_MakeDistributionFunctions(){

    Assert(_CDF.size() == _Space->size() );
    Assert(_PDF.size() == _Space->size() );

    double mag(0.0);
    double magSum(0.0);     // Sum of magnitudes

    // Make PDF
    field_t::iterator SIter = _Space->begin();
    ublas_vec_t::iterator pdfIter = _PDF.begin();
    for( ; SIter != _Space->end(); ++SIter, ++pdfIter ){
        mag = SIter->_Magnitude();
        *pdfIter = mag;
        magSum += mag;      // absolute value has already been taken
    }

//  cout << "areas: " << _PDF << "\nmagSum = " << magSum << endl;
    _PDF /= magSum;     // Normalize
    _CDF[0] = _PDF[0];

    // Make CDF
    ublas_vec_t::iterator cdfIter = _CDF.begin() + 1;
    for( pdfIter = _PDF.begin() + 1; pdfIter != _PDF.end(); 
            ++pdfIter, ++cdfIter ){
        *cdfIter = *(cdfIter - 1) + std::fabs(*pdfIter);
    }
//  cout << "PDF: " << _PDF << "\nCDF: " << _CDF << endl;

}

