/*miniLS.hpp

*/

#ifndef MINILS_H
#define MINILS_H

#include<iostream>
#include<vector>
#include<cmath>
#include<algorithm>
#include "boost/tr1/random.hpp"
#include "boost/numeric/ublas/vector.hpp"
#include "boost/math/special_functions/legendre.hpp"
#include "boost/format.hpp"

#include "boost/accumulators/accumulators.hpp"
#include "boost/accumulators/statistics/stats.hpp"
#include "boost/accumulators/statistics/mean.hpp"
#include "boost/accumulators/statistics/variance.hpp"

#include "libs/numeric/bindings/lapack/test/utils.h"
#include "Assert.hh"
#include "CartesianMesh1D.hh"
#include "Field.hh"
#include "FissionSource.h"

using std::cout;
using std::endl;

using boost::format;
using namespace std::tr1;
namespace accum = boost::accumulators;
namespace ublas = boost::numeric::ublas;

template<unsigned int O> class LinearS;

template<unsigned int O>
class LinearSS : FissionSource {
public:
    typedef std::vector<LinearS<O> > vLS;
    typedef Field<Zone<OneD>, LinearS<O> , vLS > field_t;

    /*Coeff is the coefficients for the contained LinearSs.  They must be 
     * in order; i.e. the coefficients for the first LinearS followed by 
     * the coefficients for the second LinearS and so forth.*/
    LinearSS( const std::vector<unsigned long>& seed, 
        const CartesianMesh<OneD>& M, const std::vector<double>& Coeff )
    : _field(0), FissionSource(seed)
    {
        double xmin, xmax;
        vLS Spaces;     Spaces.reserve( M.numZones() );

        std::vector<double> C(O,0.0);   // Coefficients for LinearS

        CartesianMesh<OneD>::const_ZoneIterator zIter = M.zoneBegin();
        // Iterate over all Linear Spaces
        unsigned int i(0);
        for( ; zIter != M.zoneEnd(); ++zIter, ++i ){
            xmin = zIter->getLeftNode().x();
            xmax = zIter->getRightNode().x();

            // Get Coefficients for LinearS
            std::copy((Coeff.begin()+i*O), (Coeff.begin()+(i+1)*O), C.begin());

            Spaces.push_back( LinearS<O>(C, xmin, xmax) );
        }

        _field = new field_t(M, Spaces);
    }

    void sample( Particle& P) { }
    double magnitude() const{   }
    void score(const Particle& P, double s){  }
    void PostScoring( int ) {    }

    friend std::ostream& operator<<(std::ostream& os, 
            const LinearSS<O>& LSS ){

        typename LinearSS<O>::field_t::const_iterator fIter;
        for( fIter = LSS._field->begin(); fIter != LSS._field->end(); ++fIter ){
            os << *fIter << endl;
        }
        return os;
    }

private:
    field_t* _field;

};

template<unsigned int O>
class LinearS {
public:
    typedef accum::accumulator_set<double, accum::stats<accum::tag::mean,
        accum::tag::variance> > accum_t;
    typedef ublas::vector<double, std::vector<double> > ublas_vec_t;

    LinearS( double xmin, double xmax )
    : _xmin(xmin), _xmax(xmax), _width(xmax - xmin), _yRatio(0), _accCoeff(),
      _ExCoeff(O, 0.0), _ExCoeffSD(O, 0.0), _straddled(false), 
      _sLeft(0), _sRight(0), _pLeft(0.0), _pRight(0.0)
    {   }

    template<typename V>
    LinearS( const V& C, double xmin, double xmax )
    : _xmin(xmin), _xmax(xmax), _width(xmax - xmin), _yRatio(0), _accCoeff(),
      _ExCoeff(O, 0.0), _ExCoeffSD(O, 0.0), _straddled(false), 
      _sLeft(0), _sRight(0), _pLeft(0.0), _pRight(0.0)
    {
        Assert(C.size() == O);

        // Copy coefficients
        std::copy( C.begin(), C.end(), _ExCoeff.begin() );
    }

    /*CoeffFromMB will calculate the coefficients given the slope, intercept and
     * xrange.*/
    std::vector<double> CoeffFromMB(double m, double b, double xmin, 
            double xmax);

    double y(double) const;
    double area() const;
    double slope() const;
    double intercept() const;

    double magnitude();
    void Normalize();

    friend std::ostream& operator<<(std::ostream& os, const LinearS<2> LS ){

        os << format( "f(x) = %6.4f x + %6.4f,\t%4.2f <= x <= %4.2f" ) 
             % LS.slope() % LS.intercept() % LS._xmin % LS._xmax;
        return os;
    }

private:

    double _xmin;
    double _xmax;
    double _width;

    double _yRatio;

    std::vector<accum_t> _accCoeff;     // Accumulator for expansion coeff
    ublas_vec_t _ExCoeff;               // Expansion Coefficients
    ublas_vec_t _ExCoeffSD;             // Expansion Coefficients standard dev.

    bool _straddled;
    LinearS* _sLeft;                // Left side of straddled LinearS
    LinearS* _sRight;               // Right side of straddled LinearS
    double _pLeft;                      // Probability of picking left side
    double _pRight;                     // Probability of picking right side

    double _xScaled(double x) const { return 2*(x-_xmin)/_width - 1.0; }

};

template<unsigned int O>
inline double LinearS<O>::y(double x) const{
    double f(0.0);

    typename ublas_vec_t::const_iterator coefIter = _ExCoeff.begin();
    unsigned int n(0);
    for( ; coefIter != _ExCoeff.end(); ++coefIter, ++n ){
        f += *coefIter*boost::math::legendre_p(n, _xScaled(x) );
    }

}

template<>
inline double LinearS<2>::slope() const { 
    return _ExCoeff[1]*2.0/_width;
}

template<>
inline double LinearS<2>::intercept() const { 
    return _ExCoeff[0] - _ExCoeff[1]*(_xmax + _xmin)/_width; 
}

template<>
inline double LinearS<2>::area() const{
    return 0.5*_width*(2.0*intercept() + slope()*(_xmax + _xmin) );
}


/*CoeffFromMB will calculate the coefficients given the slope, intercept and
 * xrange.*/
template<>
std::vector<double> LinearS<2>::CoeffFromMB(double m, double b, 
    double xmin, double xmax){

    std::vector<double> Coefficients(2, 0.0);
    Coefficients[0] = b + (xmax + xmin)/2.0;
    Coefficients[1] = m*(xmax - xmin)/2.0;

    return Coefficients;
}

/*magnitude will return the absolute value of the area under the curve.*/
template<>
double LinearS<2>::magnitude(){

    double ymin( y(_xmin) );
    double ymax( y(_xmax) );

    if( ymin*ymax < 0 ){    // Straddled
        _straddled = true;

        double b = intercept();
        double m = slope();
        double xmid = -b/m;

        if( slope() < 0 ){  // Left side is originally positive
            _sLeft = new LinearS<2>( CoeffFromMB(m, b, _xmin, xmid), 
                    _xmin, xmid );
            _sRight = new LinearS<2>(CoeffFromMB(-m, -b, xmid, _xmax),
                    xmid, _xmax );
        }
        else{               // Right side is originally positive
            _sLeft = new LinearS<2>( CoeffFromMB(-m, -b, _xmin, xmid), 
                    _xmin, xmid );
            _sRight = new LinearS<2>(CoeffFromMB(m, b, xmid, _xmax),
                    xmid, _xmax );
        }

        double aLeft = std::fabs(_sLeft->area());
        double aRight = std::fabs(_sRight->area());

        // Probability of picking left and right side
        _pLeft = aLeft/(aLeft + aRight);
        _pRight = aRight/(aLeft + aRight);

        return aLeft + aRight;
    }
    else return std::fabs( area() );

}

template<unsigned int O>
void LinearS<O>::Normalize(){

}


#endif

