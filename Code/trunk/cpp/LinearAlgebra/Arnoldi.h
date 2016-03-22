/*Arnoldi.h
 $Author: jlconlin $
 $Id: Arnoldi.h 648 2009-06-25 23:28:39Z jlconlin $
 $Revision: 648 $
 $Date: 2009-06-25 17:28:39 -0600 (Thu, 25 Jun 2009) $

 The Arnoldi class is the C++ implementation of Arnoldi's method.  It is the 
 base class from which other Arnoldi's method implentations will inherit.
 */

#ifndef ARNOLDI_H
#define ARNOLDI_H

#include<iostream>
#include<complex>
#include<vector>
#include<utility>
#include<limits>
#include "Utilities.h"
#include "QRAlgo.h"
#include "boost/format.hpp"
#include "boost/compressed_pair.hpp"
#include "boost/numeric/ublas/vector.hpp"
#include "boost/numeric/ublas/matrix.hpp"
#include "boost/numeric/ublas/matrix_proxy.hpp"
#include "boost/numeric/ublas/vector_proxy.hpp"
#include "boost/numeric/bindings/lapack/geev.hpp"
#include "boost/numeric/bindings/traits/ublas_matrix.hpp"
#include "boost/numeric/bindings/traits/ublas_vector.hpp"
#include "boost/date_time/posix_time/posix_time.hpp"
#include "boost/date_time/microsec_time_clock.hpp"

#include "libs/numeric/bindings/lapack/test/utils.h"

using std::complex;
using std::cout;
using std::endl;

using boost::posix_time::ptime;
using boost::format;

namespace ublas = boost::numeric::ublas;
namespace lapack = boost::numeric::bindings::lapack;

enum RestartMethod { EXPLICIT, IMPLICIT };
typedef std::pair<complex<double>, unsigned int> ePair;

bool _PairCompare(const ePair&, const ePair&);

template<typename T>
class Arnoldi {
    public:
        typedef T                                                   value_t;
        typedef ublas::vector<T, std::vector<T> >                   T_vec_t;
        typedef ublas::matrix<T, ublas::column_major>               T_mat_t;
        typedef ublas::matrix_column<T_mat_t>                       T_column_t;

        // Complex typedefs
        typedef ublas::vector<complex<double> >                     c_vec_t;
        typedef ublas::matrix<complex<double>, ublas::column_major> c_mat_t;
        typedef ublas::matrix_column<c_mat_t>                       c_column_t;

        /*arnoldi is the main method for the class.  It performs Arnoldi's 
         * method for starting vector q, and I iterations.*/
        void arnoldi(T_vec_t& v, unsigned int I);

        /*RAM is Restarted Arnoldi's Method.  RestartMethod indicates what kind
         * of Restart (explicit or implicit) will be done.*/
        void RAM(T_vec_t v, unsigned int n, unsigned int I, unsigned int R,
                RestartMethod M = IMPLICIT);

        const T_mat_t& H() const { return _H; }
        const T_mat_t& Q() const { return _V; }
        const c_vec_t& Eigenvalues() const { return _Eigenvalues; }
        const c_vec_t& Ritzvalues() const { return _Eigenvalues; }
        const c_mat_t& Eigenvectors() const { return _Eigenvectors; }
        const c_mat_t& Ritzvectors() const { return _Ritzvectors; }

        bool ePairComp(ePair&, ePair&);

        friend class ArnoldiTest;

    protected:
        /*Tolerance is the the smallest possible value.*/
        Arnoldi(int v = 1, double Tol = std::numeric_limits<double>::epsilon() )
            : _I(0), _n(0), _k(0), _R(0), _H(), _V(), _Verbose(v), 
              _Tolerance(Tol), _stoppedI(false), _stoppedR(false), _start(), 
              _startR(), _Eigenvalues(), _Eigenvectors(), _Ritzvectors(), 
              _residual(0.0), _RMethod()
        {   }

        unsigned int _I;    // Total number of iterations
        unsigned int _n;    // Number of desired eigenvalues
        unsigned int _k;    // Current iteration number
        unsigned int _R;    // Current restart number
        T_mat_t _H;
        T_mat_t _V;
        c_vec_t _Eigenvalues;
        c_mat_t _Eigenvectors;
        c_mat_t _Ritzvectors;
        complex<double> _residual;
        RestartMethod _RMethod;

        // Functions
        virtual T_vec_t _Av() = 0;
        virtual void _Iteration(T_vec_t&);
        virtual void _Restart(T_vec_t&);    // Implicit Restart
        void _IRestart(T_vec_t&);    // Implicit Restart
        void _ERestart(T_vec_t&);    // Explicit Restart
        void _CalcEigenPairs();
        void _MakeRitzPairs();

        // Supporting variables
        int _Verbose;
        double _Tolerance;
        unsigned int _length;
        bool _stoppedI;     // Stop iterating
        bool _stoppedR;     // Stop restarting
        ptime _start;    // Start of Arnoldi
        ptime _startR;   // Start of a restart

#ifdef JY_DEBUG
        void _CheckOrthogonality();
#endif
        
    private:
        void _Orthogonalize(T_vec_t&);
        void _ReOrthogonalize(T_vec_t&);
        void _Normalize(T_vec_t&);
        void _SortEigenPairs(c_vec_t&, c_mat_t&);

        /*_addColumn is a utility function to help with minimizing repeating 
         * errors with template specializing.*/
        void _addColumn(T_vec_t&, int, int);
        Arnoldi(Arnoldi&) { }

};

/*RAM is Restarted Arnoldi's Method.  
 * 
 *
 * q: Starting vector
 * n: Number of desired eigenvalues
 * I: Total number of Arnoldi iterations
 * R: Number of Arnoldi Restarts
 * M: RestartMethod  IMPLICIT means to restart following Danny Sorensen
 *          EXPLICIT means to restart following Yousef Saad*/
template<typename T>
void Arnoldi<T>::RAM(T_vec_t q, unsigned int n, unsigned int I, unsigned int R,
        RestartMethod M){

    if( I <= n ){
        std::cerr << "Number if iterations per restart must be smaller than\n"
                  << "the desired number of eigenvalues.\n" << endl;
        return;
    }

    _I = I;
    _n = n;
    _RMethod = M;
    T_vec_t v(q);

    // Start timing
    _start = boost::date_time::microsec_clock<ptime>::local_time();

    // Do Restarts
    _stoppedR = false;
    _k = 0;
    _R = 0;
    while( not _stoppedR ){
        _Restart(v);

        ++_R;
        if( _R >= R ) _stoppedR = true;
    }
}

/*arnoldi is the main method of this class.  This is just a basic function.  It
 * doesn't do anything other than create the Arnoldi factorization given the 
 * starting vector v and a matrix-vector product defined in _Av()
 * v: Initial starting vector
 * I: Number of Arnoldi Iterations to perform.*/
template<typename T>
void Arnoldi<T>::arnoldi(T_vec_t& v, unsigned int I){
    if( not _k ){
        _length = v.size();
        _H.resize( I, I );    _H *= 0.0;
        _V.resize( _length, I );  _V *= 0.0;

        v /= ublas::norm_2(v);  // Normalize v
        T_column_t q = T_column_t(_V, 0);
        q = v;   // Set first column of _V to be starting vector

        // Apply Linear operator
        v = _Av();

        // Orthogonalize first vector
        T alpha = inner_prod(q,v);
        v -= q*alpha;
        _H(0,0) = alpha;

        _k = 1;
    }

    _stoppedI = false;
    while( not _stoppedI ){
        _Iteration(v);

        if( _k == I-1 ) _stoppedI = true;
        else ++_k;
    }   // while( not_stopped )
}

/*_Restart performs some operations common to both explicit and implicit 
 * restarts and decides what kind of restart needs to be performed depending on 
 * the RestartMethod.*/
template<typename T>
void Arnoldi<T>::_Restart(T_vec_t& v){
    // Start timing restart
    _startR = boost::date_time::microsec_clock<ptime>::local_time();
    
    arnoldi(v, _I);
    _CalcEigenPairs();
    _MakeRitzPairs();

    // Calculate residual
    _residual = abs(_Eigenvectors(_k, _k))*ublas::norm_2(v);

    // Call Implicit or Explicit restarts
    switch( _RMethod ){
        case IMPLICIT: _IRestart(v); break;
        case EXPLICIT: _ERestart(v); break;
    }

}

/*_IRestart manages all the things that much happen at each Restart.*/
template<typename T>
void Arnoldi<T>::_IRestart(T_vec_t& v){
//  print_v(v, "v beginning of _IRestart");
    // Do shifted QR algorithm
    ublas::identity_matrix<T, ublas::column_major> Id(_I, _I);
    T_mat_t Q( Id );
    c_vec_t Shifts( ublas::subrange( _Eigenvalues, 0, (_I-_n) ) );
    T_mat_t Hm( ublas::subrange( _H, 0,_I, 0,_I ) );
//  print_v(_Eigenvalues, "Eigenvalues");
//  print_v(Shifts, "Shifts");
//  print_m(Hm, "Hm");
    QRShifted( Hm, Q, Shifts);

    // Update _H and clean up rounding error below subdiagonal
    ublas::subrange( _H, 0,_I, 0,_I ) = Hm;
    for( int i = 0; i < _H.size1(); ++i ){
        for( int j = 0; j < i-1; ++j ) _H(i,j) = 0.0;
    }

    // Update _k, _V
    _k = _n-1;
    ublas::subrange( _V, 0,_length, 0,_n ) = prod( 
        ublas::subrange( _V, 0,_length, 0,_I ), 
        ublas::subrange( Q, 0,_I, 0,_n ) );

    // Update v for next restart
    v = v*Q(_I-1, _n-1);
}

/*_ERestart manages all the things that must happen at each explicit Restart.*/
template<typename T>
void Arnoldi<T>::_ERestart(T_vec_t& v){
    // Update v for next restart
    v *= 0;
    int n = _Ritzvectors.size2();
    for( int i  = 0; i < _n; ++i ) _addColumn(v, n, i);
    _k = 0;
}

template<>
inline void Arnoldi<complex<double> >::_addColumn(T_vec_t& v, int n, int i){
    v += c_column_t( _Ritzvectors, (n-1-i) );
}

template<>
inline void Arnoldi<double>::_addColumn(T_vec_t& v, int n, int i){
    v += real(c_column_t( _Ritzvectors, (n-1-i) ) );
}

/*_Iteration manages all the things that must happen at each iteration.*/
template<typename T>
void Arnoldi<T>::_Iteration(T_vec_t& v){
    _Normalize(v);
    T_column_t(_V, _k) = v;    // Add v to _V

    v = _Av();
    _Orthogonalize(v);

#ifdef JY_DEBUG
    _CheckOrthogonality();
#endif
}

/*_Orthogonalize will orthogonalize vector q against all previously calculated
 * Arnoldi vectors.*/
template<typename T>
void Arnoldi<T>::_Orthogonalize(T_vec_t& v){

    T_mat_t sub_V( ublas::subrange( _V, 0,_length , 0, _k+1) );
    T_vec_t h = prod( trans(sub_V), v);
      
    // Assign h to column of _H (this looks nasty but that's just the way it is)
    ublas::matrix_vector_slice<T_mat_t>( _H, 
            ublas::slice(0,1, _k+1), ublas::slice(_k, 0, _k+1) ) = h;
    v = v - prod(sub_V, h);
}

/*_ReOrthogonalize will reorthogonalize vector q against all previously 
 * calculated Arnoldi vectors.*/
template<typename T>
void Arnoldi<T>::_ReOrthogonalize(T_vec_t& v){

    T_mat_t sub_V( ublas::subrange( _V, 0,_length , 0, _k+1) );
    // Do dot product between vectors
    T_vec_t h = prod( trans(sub_V), v);

    // Add information
    ublas::matrix_vector_slice<T_mat_t>( _H, 
            ublas::slice(0,1, _k+1), ublas::slice(_k, 0, _k+1) ) += h;
    v = v - prod(sub_V, h);
}

template<typename T>
void Arnoldi<T>::_Normalize(T_vec_t& q){
    T NormFactor = ublas::norm_2(q);
    _H(_k,_k-1) = NormFactor;
    q /= NormFactor;
}

/*_CalcEigenPairs will calculate the Eigenpairs of the upper Hessenberg 
 * matrix.*/
template<typename T>
void Arnoldi<T>::_CalcEigenPairs(){
    c_vec_t Values(_k+1);
    c_mat_t Vectors(_k+1, _k+1);

    T_mat_t H_tmp = ublas::subrange(_H, 0,_k+1, 0,_k+1);   // Submatrix of _H
    c_mat_t* LeftVectors = 0;

    lapack::geev(H_tmp, Values, LeftVectors, &Vectors, 
            lapack::optimal_workspace() );  // Calculate Eigenpairs

    NormColumns(Vectors); // Normalize Vectors

    _SortEigenPairs(Values, Vectors);
}

/*_MakeRitzPairs will create the Ritzpairs of the linear operator.
 * This one is easy :D.*/
template<typename T>
void Arnoldi<T>::_MakeRitzPairs(){
    T_mat_t sub_V(ublas::subrange(_V, 0,_length, 0,_k+1));

    _Ritzvectors = ublas::prod(sub_V, _Eigenvectors);
}

/*Compare will return whether x is less than y defined by the real parts of the
 * first elements of the pair.  If the real part is equal the imaginary parts
 * are compared.*/
inline bool _PairCompare(const ePair& x, const ePair& y){
    if( x.first.real() == y.first.real() ){
       return x.first.imag() < y.first.imag();
    }
    else  return x.first.real() < y.first.real();
}

/*_SortEigenPairs will sort _Eigenvalues and associated eigenvectors by size
 * of the eigenvalue.
 * Vectors: Eigenvectors associated with _Eigenvalues.  Assumed to be in same
 *  order as eigenvalues.*/
template<typename T>
void Arnoldi<T>::_SortEigenPairs(c_vec_t& Values, c_mat_t& Vectors){

    std::vector<ePair> Pairs( Values.size() );
    std::vector<ePair>::iterator pairIter = Pairs.begin();

    // Make vector of eigenvalues and indices
    unsigned int index = 0;
    c_vec_t::iterator valIter = Values.begin();
    for( ; valIter != Values.end(); ++valIter, ++pairIter, ++index ){
        *pairIter = ePair(*valIter, index);
    }

    std::stable_sort(Pairs.begin(), Pairs.end(), &_PairCompare);

    _Eigenvalues.resize(Values.size());
    _Eigenvectors.resize(Vectors.size1(),Vectors.size2());
    valIter = _Eigenvalues.begin();
    pairIter = Pairs.begin();
    int i = 0;
    for( ; pairIter != Pairs.end(); ++pairIter, ++valIter, ++i ){
        index = pairIter->second;
        *valIter = Values(index);
        c_column_t(_Eigenvectors, i) = c_column_t(Vectors, index);
    }
}
    
    
#ifdef JY_DEBUG
template<typename T>
void Arnoldi<T>::_CheckOrthogonality(){
    static bool checked = true;
    if( checked ){
        cout << "Checking Orthogonality." << endl;
//      checked = false;
        T_mat_t sub_V( ublas::subrange( _V, 0,_length, 0,_k+1 ) );
        CheckOrthogonality(sub_V);
    }
}

#endif
#endif

