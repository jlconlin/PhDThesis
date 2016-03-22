/*LinearSpaceSource.h

 This file contains header information for the LinearSpaceSource and LinearSpace
 classes.

*/
#ifndef LINEARSPACESOURCE_H
#define LINEARSPACESOURCE_H

#include <iostream>
#include <sstream>
#include <vector>
#include <numeric>
#include <algorithm>
#include <cmath>
#include "boost/format.hpp"
#include "boost/math/special_functions/legendre.hpp"
#include "boost/numeric/ublas/io.hpp"
#include "boost/numeric/ublas/vector.hpp"
#include "boost/accumulators/accumulators.hpp"
#include "boost/accumulators/statistics/stats.hpp"
#include "boost/accumulators/statistics/mean.hpp"
#include "boost/accumulators/statistics/variance.hpp"
#include "Field.hh"
#include "Field_AddOns.hh"
#include "Mesh_AddOns.hh"
#include "CartesianMesh1D.hh"
#include "FissionSource.h"

using std::cout;
using std::endl;
using boost::format;

namespace ublas = boost::numeric::ublas;
namespace accum = boost::accumulators;

class LinearSpace{
public:
    typedef accum::accumulator_set<double, accum::stats<accum::tag::mean,
        accum::tag::variance> > accum_t;

    LinearSpace(double xmin, double xmax)
    : _m(0.0), _b(0.0), _xmin(xmin), _xmax(xmax), 
      _xmid( (_xmax+_xmin)/2.0 ), _width( _xmax - _xmin ), _straddled(false),
      _sLeft(0), _sRight(0), _pLeft(0), _pRight(0),
      _alpha(0.0), _beta(0.0),_betaSquared(0.0), _count(0), _value(0.0),
      _cLeft(0), _cRight(0)
    {   }

    LinearSpace(double slope, double intercept, double xmin, double xmax)
    : _m(slope), _b(intercept), _xmin(xmin), _xmax(xmax), 
      _xmid( (_xmax+_xmin)/2.0 ), _width( _xmax - _xmin ), _straddled(false),
      _sLeft(0), _sRight(0), _pLeft(0), _pRight(0),
      _alpha(intercept), _beta(slope),_betaSquared(0.0), _count(0), _value(0.0),
      _cLeft(0), _cRight(0)
    {   }

    ~LinearSpace() {
        if( _sLeft != 0 ) delete _sLeft;
        if( _sRight != 0 ) delete _sRight;
    }

    double sample( RandomLib::Random* );
    double magnitude() const;
    void score( const Particle& P, double s);
    void Normalize();
    void PostScoring( int N );


    double xmin() const { return _xmin; }
    double xmax() const { return _xmax; }
    double xmid() const { return _xmid; }

    double slope() const { return _m; }
    double intercept() const { return _b; }
    inline double y(double x) const { return _m*x + _b; }
    double area() const;

    double pLeft() const{ return _pLeft; }
    double pRight() const{ return _pRight; }
    double cLeft() const{ return _cLeft; }
    double cRight() const{ return _cRight; }
    LinearSpace sLeft() const{ return *_sLeft; }
    LinearSpace sRight() const{ return *_sRight; }


    friend class LinearSpaceSource;
    friend std::ostream& operator<<(std::ostream& os, const LinearSpace& LS ){
        os << format( "%6.4f*x + %6.4f\t %5.3d <= x <= %5.3d")
            %LS._m %LS._b %LS._xmin %LS._xmax;
        return os;
    }
private:
    double _m;   // Expansion coefficients
    double _b;  // f(x)  = _m(x-xmid) + _b
    double _xmin;
    double _xmax;
    double _xmid;
    double _width;

    double _yRatio;

    // Data needed if source is straddled---crosses x-axis
    bool _straddled;
    LinearSpace* _sLeft;
    LinearSpace* _sRight;
    double _pLeft;
    double _pRight;
    unsigned int _cLeft;
    unsigned int _cRight;

    double _alpha;
    double _beta;
    double _betaSquared;    // For calculating variance
    unsigned int _count;
    double _value;          // For whatever I want---use sparingly

    double _Magnitude();
    inline double _Scaled(double x) const { return 2*(x-_xmin)/_width - 1; }
    inline double _k(unsigned int n) const{ return _width/(2.0*n + 1.0); }


};

class LinearSpaceSource : FissionSource{
public:
    typedef Field<Zone<OneD>, LinearSpace, std::vector<LinearSpace> > field_t;
    typedef ublas::vector<double, std::vector<double> > ublas_vec_t;

    LinearSpaceSource( const CartesianMesh<OneD>& M, 
            const std::vector<unsigned long>& seed );

    template<typename S, typename I>
    LinearSpaceSource( const S& Slopes, const I& Intercepts, 
            const CartesianMesh<OneD>& M, const vector<unsigned long>& seed );

    template<typename C>
    LinearSpaceSource( const vector<unsigned long>& seed, 
            const CartesianMesh<OneD>& M, const C& Coefficients );

    ~LinearSpaceSource() { delete _Space; }

    void sample( Particle& P );
    double magnitude() const;
    void score( const Particle& P, double s);
    void Normalize(); 
    void PostScoring( int );
    std::vector<double> getMagnitudes() const{
        std::vector<double> M(_Space->size(), 0.0);

        field_t::iterator sIter = _Space->begin();
        std::vector<double>::iterator mIter = M.begin();
        for( ; sIter != _Space->end(); ++sIter, ++mIter ){
            *mIter = std::fabs( sIter->area() );
        }

        return M;
    }

    void Samples() {
        field_t::iterator SIter = _Space->begin();
        for( ; SIter != _Space->end(); ++SIter ){
            cout << "Left: " << SIter->cLeft() << ", pLeft: " << SIter->pLeft()
                 <<"\t" << SIter->sLeft() << "\nRight: " << SIter->cRight() 
                 << ", pRight: " << SIter->pRight() << "\t" << SIter->sRight()
                 << endl;
        }
    }

    /*PDF does *not* return _PDF.  It returns a vector of alternating slopes and
     * intercepts for the contained LinearSpaces.*/
    ublas_vec_t PDF() const;

    /*PlotPoints will calculate the x and y points at the edges for plotting.*/
    template<typename X, typename Y>
    void PlotPoints(X&, Y&) const;

    friend std::ostream& operator<<(std::ostream& os, 
            const LinearSpaceSource& L){

        field_t::const_iterator SIter = L._Space->begin();
        for( ; SIter != L._Space->end(); ++SIter ){
            os << format("\tf(x) = %s") %*SIter << std::endl;
        }
        return os;
    }
private:
    field_t* _Space;

    ublas_vec_t _PDF;
    ublas_vec_t _CDF;

    void _MakeDistributionFunctions();
};

// Constructor
template<typename S, typename I>
LinearSpaceSource::LinearSpaceSource( const S& Slopes, const I& Intercepts, 
            const CartesianMesh<OneD>& M, const vector<unsigned long>& seed )
: _Space(0), _PDF(M.numZones()), _CDF(M.numZones()), FissionSource(seed)
{
    Assert(Slopes.size() == Intercepts.size() );
    Assert( Slopes.size() == M.numZones() );

    std::vector<LinearSpace> Spaces;    Spaces.reserve( Slopes.size() );

    typename S::const_iterator sIter = Slopes.begin();
    typename I::const_iterator iIter = Intercepts.begin();
    CartesianMesh<OneD>::const_ZoneIterator zMIter = M.zoneBegin();

    double xmin, xmax;
    for( ; zMIter != M.zoneEnd(); ++zMIter, ++sIter, ++iIter ){
        xmin = zMIter->getLeftNode().x();
        xmax = zMIter->getRightNode().x();
        Spaces.push_back(LinearSpace(*sIter, *iIter, xmin, xmax) );
    }

    _Space = new field_t(M, Spaces);
}

template<typename C>
LinearSpaceSource::LinearSpaceSource( const vector<unsigned long>& seed, 
            const CartesianMesh<OneD>& M, const C& Coefficients )
: _Space(0), _PDF(M.numZones()), _CDF(M.numZones()), FissionSource(seed)
{
    std::vector<LinearSpace> Spaces;    Spaces.reserve( Coefficients.size() );

    typename C::const_iterator cIter = Coefficients.begin();
    CartesianMesh<OneD>::const_ZoneIterator zMIter = M.zoneBegin();

    double xmin, xmax;
    double slope, intercept;
    for( ; zMIter != M.zoneEnd(); ++zMIter ){
        xmin = zMIter->getLeftNode().x();
        xmax = zMIter->getRightNode().x();
        slope = *cIter; ++cIter;
        intercept = *cIter; ++cIter;
        Spaces.push_back(LinearSpace(slope, intercept, xmin, xmax) );
    }

    _Space = new field_t(M, Spaces);
}

/*PlotPoints will calculate the x and y points used for plotting.*/
template<typename X, typename Y>
void LinearSpaceSource::PlotPoints(X& x, Y& y) const{
    double size = _Space->size()*2;
    x = X(size);
    y = Y(size);

    double xmin;
    double xmax;
    typename field_t::iterator SIter = _Space->begin();
    for( int index = 0; SIter != _Space->end(); ++SIter ){
        xmin = SIter->xmin();
        xmax = SIter->xmax();

        x[index] = xmin;
        y[index] = SIter->y(xmin);
        ++index;

        x[index] = xmax;
        y[index] = SIter->y(xmax);
        ++index;
    }

}

#endif

