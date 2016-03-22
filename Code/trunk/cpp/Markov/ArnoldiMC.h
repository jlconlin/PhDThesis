/*ArnoldiMC.h

 The ArnoldiMC class is the C++ implementation of Monte Carlo Arnoldi's method.
 
*/

#ifndef ARNOLDIMC_H
#define ARNOLDIMC_H

#include<vector>
#include<complex>
#include "boost/numeric/ublas/vector.hpp"
#include "boost/format.hpp"
#include "boost/numeric/ublas/io.hpp"
#include "boost/numeric/ublas/vector.hpp"
#include "boost/numeric/ublas/vector_proxy.hpp"
#include "boost/numeric/ublas/matrix_proxy.hpp"
#include "Assert.hh"
#include "Arnoldi.h"
#include "Field.hh"
#include "CartesianMesh1D.hh"
#include "HistSource.h"
#include "Particle.h"
#include "Utilities.h"
#include "LinearSpaceSource.h"

#include "libs/numeric/bindings/lapack/test/utils.h"

template<class S> class Markov;
#include "Markov.h"

template<typename T> class Arnoldi;
#include "Arnoldi.h"

using std::complex;
using std::vector;
using boost::format;

enum Method { ARNOLDI, POWER };

template<class S>   // S should be some Fission Source type object
class ArnoldiMC: public Markov<S>, public Arnoldi<double> {

    // Using declarations required to be able to access protected variables
    using Markov<S>::_r;
    using typename Markov<S>::_field;

    public:
        /*seed: Seed for random number generator
          geometry: Geometry of problem for scoring
          ScoringMesh: Spatial discretization for scoring
          v: Verbose true/false
          rT: Tolerance for Residual
          iT: Tolerance for Invariance*/
        ArnoldiMC(const vector<unsigned long>& seed, 
                const typename Markov<S>::field_t& geometry,
                CartesianMesh<OneD>& ScoringMesh, double relaxed = 0.0,
                int v = 2, double Tol = std::numeric_limits<double>::epsilon() )
            : Arnoldi<double>(v, Tol), Markov<S>(seed, geometry), 
              _histories(), _relaxed(relaxed), _rHistories(0), _tracks(0), 
              _Tracks(), _Tracks_inactive(), _TracksIteration(),
              _Tracks_inactiveIteration(), _TimeRestart(), _TimeIteration(),
              _TimeRestart_inactive(), _TimeIteration_inactive(), 
              _FOMRestart(), _FOMIteration(), _Ent(0.0), _Entropy(), 
              _Entropy_inactive(), _ScoringMesh(ScoringMesh), _EigenvaluesEst(), 
              _MeanValue(), _StdDevValue() { }

        /*
         * source: Starting source
         * n: Number of desired eigenvalues
         * I: Number of Arnoldi Iterations/Size of Krylov subspace
         * active: Number of active restarts
         * inactive: Number of inactive restarts
         * histories: How many histories to track in each iteration
         * M: Whether IMPLICIT or EXPLICIT restart
         * relaxed: Relaxed matrix-vector product parameter. 
         *          relaxed = 0 means no relaxing.*/
        void RAM( S& source, unsigned int n, unsigned I, 
                unsigned int active, unsigned int inactive, 
                unsigned int histories, RestartMethod M = IMPLICIT){

            unsigned int R = active + inactive;

            // Initialize data
            _inactive = inactive;
            _histories = histories;

            _Tracks.clear(); 
            _Tracks.reserve( active );
            _Tracks_inactive.clear(); 
            _Tracks_inactive.reserve( inactive );
            _TracksIteration.clear(); 
            _TracksIteration.reserve( active*I );
            _Tracks_inactiveIteration.clear(); 
            _Tracks_inactiveIteration.reserve( inactive*I );

            _Entropy.clear(); 
            _Entropy.reserve( active*I );
            _Entropy_inactive.clear(); 
            _Entropy_inactive.reserve( inactive*I );

            _Restarts.clear(); _Restarts.reserve( inactive + active );

            _Residual.clear(); _Residual.reserve( active );
            _Residual_inactive.clear(); _Residual_inactive.reserve( active );

            _EigenvaluesEst.resize(I, active, false);
            _EigenvaluesEst *= 0.0;
            _EigenvaluesEst_inactive.resize(I, inactive, false);
            _EigenvaluesEst_inactive *= 0.0;
            _RitzvectorsEst.clear();
            _RitzvectorsEst.reserve(active);
            _RitzvectorsEst_inactive.clear();
            _RitzvectorsEst_inactive.reserve(inactive);

            _TimeRestart.clear();
            _TimeRestart.reserve( active );
            _TimeIteration.clear();
            _TimeIteration.reserve( active*I );
            _TimeRestart_inactive.clear();
            _TimeRestart_inactive.reserve( inactive );
            _TimeIteration_inactive.clear();
            _TimeIteration_inactive.reserve( inactive*I );

            _FOMRestart.clear();
            _FOMIteration.clear();

            _sum = complex<double>(0.0,0.0);
            _sum2 = complex<double>(0.0,0.0);

            ublas::vector<double> v( source.PDF() );

            Arnoldi<double>::RAM(v, n, I, R, M);
        }

        void arnoldi(S& source, int I, int histories){
            _histories = histories;

            T_vec_t v = source.PDF();
            Arnoldi<double>::arnoldi(v, I);
        }


        // Gettr functions
        std::vector<double> Tracks( bool inactive = true ) const;
        std::vector<double> TracksbyIteration( bool inactive = true ) const;
        std::vector<double> Entropy(bool inactive = true ) const;
        std::vector<double> EntropyRestart(bool inactive = true ) const;
        std::vector<double> Time(bool inactive = true ) const;
        std::vector<double> TimebyIteration(bool inactive = true ) const;

        /*Return the Figure of Merit at each Restart*/
        std::vector<double> FOM() const { return _FOMRestart; }
        /*Return the Figure of Merit at each Iteration.  Only valid if
         * performing relaxed Arnoldi.*/
        std::vector<double> FOMbyIteration() const { return _FOMIteration; }
        std::vector<int> Restarts () const { return _Restarts; }

        void MeanVector(unsigned int, c_vec_t&, c_vec_t&) ;
        void MeanRestartVector(unsigned int, unsigned int, c_vec_t&, 
                c_vec_t&) ;
        std::vector<c_vec_t> VectorEst(unsigned int index, bool inactive=true);
        std::vector<complex<double> > Residual ( bool inactive=true ) const;
        void MeanValues (std::vector<complex<double> >& Mean, 
                std::vector<complex<double> >& SD, unsigned int index = 0, 
                bool inactive = true ) const;
        std::vector<complex<double> > Values(unsigned int index = 0, 
                bool inactive = true) const;

        ~ArnoldiMC() {  }

        friend class ArnoldiMCTest;

    private:
        unsigned int _histories;    // Original number of histories

        // Keeping track of number of histories
        double _relaxed;
        unsigned int _rHistories;   // Relaxed Histories
        unsigned int _tracks;   // Histories tracked in a restart

        // Store number of histories tracked by Restart
        std::vector<double> _Tracks;
        std::vector<double> _Tracks_inactive;

        // Store number of histories tracked by Iteration
        std::vector<double> _TracksIteration;   
        std::vector<double> _Tracks_inactiveIteration;
        void _RelaxHistories(T_vec_t&);

        // Store time
        std::vector<double> _TimeRestart;
        std::vector<double> _TimeIteration;
        std::vector<double> _TimeRestart_inactive;
        std::vector<double> _TimeIteration_inactive;

        // Store Figure of Merit 1/(variance*time)
        std::vector<double> _FOMRestart;
        std::vector<double> _FOMIteration;

        // Entropy
        double _Ent;
        std::vector<double> _EntropyRestart;
        std::vector<double> _EntropyRestart_inactive;
        std::vector<double> _Entropy;
        std::vector<double> _Entropy_inactive;

        unsigned int _inactive; // Number of inactive restarts to perform
        CartesianMesh<OneD>& _ScoringMesh;
        std::vector<int> _Restarts; // List of Restart numbers

        std::vector<complex<double> > _Residual;
        std::vector<complex<double> > _Residual_inactive;

        // Estimates of Eigenvalues at each restart.  Each column is one set 
        // of estimates.
        c_mat_t _EigenvaluesEst;     
        c_mat_t _EigenvaluesEst_inactive;     
        // Estimates of Ritzvectors at each restart. 
        vector<c_mat_t> _RitzvectorsEst;
        vector<c_mat_t> _RitzvectorsEst_inactive;
        complex<double> _MeanValue;     // Mean of estimates of largest value
        complex<double> _StdDevValue;   // StdDev of _MeanValue

        // Methods for Arnoldi's method
        T_vec_t _Av();
        T_vec_t _ScaleSource(S&, S&);
        void _Iteration(T_vec_t&);
        void _Restart(T_vec_t&);
        void _CalcEntropy();

        // Methods and members for statistical computations
        void _ValueStats();
        complex<double> _sum;    // Sum of dominant eigenvalue estimates
        complex<double> _sum2;   // Sum of dominant eigenvalue estimates square


        /*score will add nuSigmaF/SigmaT to the zone where the Particle lives.
         *
         * source: source where Particle scores
         * P: Particle being tracked*/
        void _score(S& source, Particle& P){
            const Material& mat  = _field[*(P.zone())];
            source.score(P, P.weight()*( mat.nu()*mat.xF()/mat.xT() ) );
        }

};

template<class S>
typename ArnoldiMC<S>::T_vec_t ArnoldiMC<S>::_Av(){
    T_vec_t Probabilities( T_column_t(_V, _k) );

    // Convert to fission source
    S CurrentSource(_r.Seed(), _ScoringMesh, Probabilities);
    S NextSource( _ScoringMesh, _r.Seed() );

    Markov_Transport(CurrentSource, NextSource, _rHistories);

    // Keep track how many histories were tracked
    _tracks += _rHistories;

    return _ScaleSource(CurrentSource, NextSource);
}

/* Scale returning vector to keep magnitude the same for each iteration
 * v = v*(W/N) 
 *      where W is sum of all the weights of previous source and
 *      N is number of histories tracked */
template<class S>
typename ArnoldiMC<S>::T_vec_t ArnoldiMC<S>::_ScaleSource(S& CurrentSource, 
        S& NextSource){
    T_vec_t v( NextSource.PDF() );
    if( _rHistories ) v *= CurrentSource.magnitude()/_rHistories;
    else v *= 0;

    return v;
}

template<>
ArnoldiMC<LinearSpaceSource>::T_vec_t 
ArnoldiMC<LinearSpaceSource>::_ScaleSource(LinearSpaceSource& CurrentSource, 
        LinearSpaceSource& NextSource){
    T_vec_t v( NextSource.PDF() );
    if( _rHistories ) v *= CurrentSource.magnitude();
    else v *= 0;

    return v;
}

template<class S>
void ArnoldiMC<S>::_Iteration(T_vec_t& v){

    ptime now( boost::date_time::microsec_clock<ptime>::local_time() );

    Arnoldi<double>::_Iteration(v);

    _CalcEigenPairs();
    _MakeRitzPairs();

    // Normalize Ritzvectors
    NormColumns(_Ritzvectors);

    _CalcEntropy();
    // Time since beginning of restart
    double durationR = (now-_startR).total_microseconds()/1E6;   
    // Time since beginning of Arnoldi
    double durationA = (now-_start).total_microseconds()/1E6;

    if( _Verbose > 1 ){
        cout << format( "\tIteration #: %2i,") %_k;
    }

    if( _R >= _inactive ){  // Active restarts

        // Store time 
        _TimeIteration.push_back(durationA);

        // Store number of histories tracked
        if( _TracksIteration.empty() ){
            _TracksIteration.push_back(_rHistories);
        }
        else{
            _TracksIteration.push_back(_TracksIteration.back() + _rHistories);
        }
//      _Entropy_inactive.push_back( _Ent );
    }
    else{                   // Inactive restarts
        //
        // Store time 
        _TimeIteration_inactive.push_back(durationA);

        // Store number of histories tracked
        if( _Tracks_inactiveIteration.empty() ){
            _Tracks_inactiveIteration.push_back(_rHistories);
        }
        else{
            _Tracks_inactiveIteration.push_back(
                    _Tracks_inactiveIteration.back() + _rHistories);
        }
//      _Entropy.push_back( _Ent );
    }

    if( _relaxed > 0.0 ){// Relax number of histories in the next Iteration
        // Store Figure of Merit only for active restarts
        if( _R >= _inactive ) { // Active restarts
            if( _FOMIteration.empty() ) _FOMIteration.push_back(0.0);
            else{
                _FOMIteration.push_back(1.0/(
                    _StdDevValue.real()*_StdDevValue.real()*durationA) );
            }
        }

        if( _Verbose > 1 ){
            cout << format( " Residual = %10.5G, N = %i,") 
                %real(_residual) %_rHistories;
        }
        _RelaxHistories(v);

    }
    
    if( _Verbose > 1 ){
        cout << format(" H = %6.4G, time: %6.4G sec.") %_Ent %durationR << endl;
    }

#if 0
    if( fabs( _H(_k,_k-1) ) < _Tolerance ){
//      _stoppedI = true;
        if( _Verbose ){
            cout << format("Stopping because H[%i,%i] = %8.2E < %8.2E.") 
                %_k %(_k-1) %_H(_k,_k-1) %_Tolerance << endl;
        }
    }
#endif

}

/*_RelaxHistories will update _rHistories to make the matrix-vector product 
 * faster.*/
template<class S>
void ArnoldiMC<S>::_RelaxHistories(T_vec_t& v){

    // Calculate residual
    _residual = abs(_Eigenvectors(_k, _k))*ublas::norm_2(v);
    if( real(_residual) < _relaxed ){
//      _rHistories = _histories*( real(_residual)/_relaxed );  // M&C Method
        _rHistories = _histories*(                              // Bouras Method
                real(_residual)*real(_residual)/(_relaxed*_relaxed) );
    }
    else _rHistories = _histories;

}

template<class S>
void ArnoldiMC<S>::_CalcEntropy(){
    // Get Fundamental eigenvector
    c_vec_t Fundamental(_length);
    Fundamental = c_column_t(_Ritzvectors, 0 );

    _Ent = ShannonEntropy( real(Fundamental) );

    if( _R >= _inactive ) _Entropy.push_back( _Ent );   // Active iterations
    else _Entropy_inactive.push_back( _Ent );   // Inactive iterations

}

template<>
void ArnoldiMC<LinearSpaceSource>::_CalcEntropy(){
    _CalcEigenPairs();
    _MakeRitzPairs();

    // Normalize Ritzvectors
    NormColumns(_Ritzvectors);

    // Get Fundamental eigenvector
    c_vec_t Fundamental(_length);
    Fundamental = c_column_t(_Ritzvectors, 0 );
    LinearSpaceSource Fund( _r.Seed(), _ScoringMesh, real(Fundamental) );

    Fund.Normalize();
    _Ent = ShannonEntropy( Fund.getMagnitudes() );

    if( _R >= _inactive ) _Entropy.push_back( _Ent );   // Active iterations
    else _Entropy_inactive.push_back( _Ent );   // Inactive iterations

}

/*_Restart is a restart for Restarted Arnoldi's Method. */
template<class S>
void ArnoldiMC<S>::_Restart(T_vec_t& v){

    _rHistories = _histories;
    _tracks = 0;

    // Take real part of vector
    v = real(v);
    Arnoldi<double>::_Restart(v);

    // Timing
    ptime now( boost::date_time::microsec_clock<ptime>::local_time() );
    // Time since beginning of restart
    double duration = (now-_start).total_microseconds()/1E6;

    // Normalize Ritzvectors
    NormColumns(_Ritzvectors);

    // Calculate Entropy
    c_vec_t Fundamental(_length);
    Fundamental = c_column_t(_Ritzvectors, 0 );
    _Ent = ShannonEntropy( real(Fundamental) );

    if( _R < _inactive ){ // Still inactive

        // Store time
        _TimeRestart_inactive.push_back(duration);

        // Store histories tracked
        if( _Tracks_inactive.empty() ){   // First restart
            _Tracks_inactive.push_back(_tracks);    
        }
        else _Tracks_inactive.push_back( _Tracks_inactive.back() + _tracks );

        _Residual_inactive.push_back( _residual );  // Store residual

        // Store Eigenvalue estimates
        c_column_t(_EigenvaluesEst_inactive, _R) = _Eigenvalues;

        // Mean is just current dominant eigenvalue
        _MeanValue = _Eigenvalues( _Eigenvalues.size()-1 );
        _StdDevValue = 0.0;

        // Store _Ritzvector estimates
        _RitzvectorsEst_inactive.push_back( _Ritzvectors );

        _EntropyRestart_inactive.push_back(_Ent);

    }
    else{   // Active restarts

        // Store time
        _TimeRestart.push_back(duration);

        // Store histories tracked
        if( _Tracks.empty() ){   // First active restart
            _Tracks.push_back( _tracks );
        }
        else _Tracks.push_back( _Tracks.back() + _tracks );  

        _Residual.push_back( _residual );
        _ValueStats();

        // Store Ritzvectors for statistical calculations later
        _RitzvectorsEst.push_back( _Ritzvectors );

        // Store Figure of Merit
        if( _FOMRestart.empty() ) _FOMRestart.push_back(0.0);
        else{
            _FOMRestart.push_back(1.0/(
                _StdDevValue.real()*_StdDevValue.real()*duration) );
        }

        _EntropyRestart.push_back(_Ent);
    }
    
    _Restarts.push_back(_R-_inactive);
    if( _Verbose > 0 ){
        cout << format( "Restart #: %i, H = %6.4G, mean = (%8.5G,%6.1G), ") 
             %_Restarts.back() %_Ent %real(_MeanValue) %imag(_MeanValue)
             << format( "std.dev. = (%7.3G,%6.1G),") 
             %real(_StdDevValue) %imag(_StdDevValue)
             << format( " res = %9.3G, time: %6.4G sec")
             %real(_residual) %duration;

        // Add FOM tally if active restart
        if( _FOMRestart.empty() ) cout << endl;     // Inactive restart
        else cout << format( ", FOM: %8.4G") %_FOMRestart.back() << endl;
    }

    if( std::fabs(_residual.real()) < _Tolerance ){
//      _stoppedR = true;
        if( _Verbose ){
            cout << format( "Stopping because residual = %8.4G < %8.4G" )
                %_residual %_Tolerance << endl;
        }
    }
}

/*_ValueStats will calculate the mean and standard deviation of the eigenvalue
 * estimates calculated during each restart.*/
template<class S>
void ArnoldiMC<S>::_ValueStats(){
    c_column_t(_EigenvaluesEst, (_R-_inactive) ) = _Eigenvalues;
//      ublas::subrange(_Eigenvalues, _Eigenvalues.size()-_n,
//      _Eigenvalues.size());

    complex<double> Dominant = _Eigenvalues( _Eigenvalues.size()-1 );

    _sum += Dominant;
    _sum2 += Dominant*Dominant;

    double N = (_R-_inactive) + 1;
    if( (_R-_inactive) > 0 ){   // Active cycles

        _MeanValue = _sum/N;    // mean = (1/N)*Sum(x_i)
        // std. dev. = sqrt( (1/N)(<x^2> - <x>^2 )
        _StdDevValue = std::sqrt( (1.0/N)*( _sum2/N - _MeanValue*_MeanValue) );
    }
    else{
        _MeanValue = Dominant;
        _StdDevValue = 0.0;
    }
#ifdef JY_DEBUG
    cout << format( "R = %3i, N = %3i, Dominant %14.5f, " ) %_R %N %Dominant
         << format( "Mean = %10.5f, SD = %14.5G, sum = %10.5f, sum2 = %10.5f" )
        %_MeanValue %_StdDevValue %_sum %_sum2 << endl;
#endif

}

/*Tracks will return the total number of histories tracked through each 
 * Restart.
 *
 * inactive: If true (by default), also include residual from inactive cycles.*/
template<class S>
std::vector<double> ArnoldiMC<S>::Tracks(bool inactive) const{

    if( inactive and not _Tracks_inactive.empty() ){
        std::vector<double> T( _Tracks_inactive );
        T.reserve( _Tracks_inactive.size() + _Tracks.size() );

        // Need to add total of inactive tracks to active tracks
        std::vector<double>::const_iterator TIter = _Tracks.begin();
        for( ; TIter != _Tracks.end(); ++TIter ){
            T.push_back( _Tracks_inactive.back() + *TIter );
        }
        return T;
    }
    else{
        std::vector<double> T(_Tracks);

        // Get number of inactive tracks
        double inT(0.0);
        if( not _Tracks_inactive.empty() ) inT = _Tracks_inactive.back();

        std::vector<double>::iterator TIter = T.begin();
        for( ; TIter != T.end(); ++TIter ){
            *TIter += inT;
        }

        return T;
    }
}

/*Tracks will return the total number of histories tracked through each 
 * Iteration.
 *
 * inactive: If true (by default), also include residual from inactive cycles.*/
template<class S>
std::vector<double> ArnoldiMC<S>::TracksbyIteration(bool inactive) const{
    if( inactive and not _Tracks_inactiveIteration.empty() ){
        std::vector<double> T( _Tracks_inactiveIteration );
        T.reserve( _Tracks_inactiveIteration.size() + _TracksIteration.size() );

        // Need to add total of inactive tracks to active tracks
        std::vector<double>::const_iterator TIter = _TracksIteration.begin();
        for( ; TIter != _TracksIteration.end(); ++TIter ){
            T.push_back( _Tracks_inactiveIteration.back() + *TIter );
        }
        return T;
    }
    else return _TracksIteration;
}

/*Time will return the elapsed computer time at each Restart
 *
 * inactive: If true (by default), also include elapsed time from inactive
 * restarts.*/
template<class S>
std::vector<double> ArnoldiMC<S>::Time(bool inactive ) const{

    if( inactive and not _TimeRestart_inactive.empty() ){
        // Get inactive times
        std::vector<double> T( _TimeRestart_inactive );
        // Append active times
        T.insert(T.end(), _TimeRestart.begin(), _TimeRestart.end() );
        return T;
    }
    else return _TimeRestart;
}

/*TimebyIteration will return the elapsed computer time at each iteration
 *
 * inactive: If true (by default), also include elapsed time from inactive
 * iterations.*/
template<class S>
std::vector<double> ArnoldiMC<S>::TimebyIteration(bool inactive ) const{

    if( inactive and not _TimeIteration_inactive.empty() ){
        // Get inactive times
        std::vector<double> T( _TimeIteration_inactive );
        // Append active times
        T.insert(T.end(), _TimeIteration.begin(), _TimeIteration.end() );
        return T;
    }
    else return _TimeIteration;
}

template<class S>
std::vector<double> ArnoldiMC<S>::Entropy(bool inactive ) const{

    if( inactive and not _Entropy_inactive.empty() ){
        std::vector<double> H( _Entropy_inactive );
        H.reserve( _Entropy_inactive.size() + _Entropy.size() );

        H.insert(H.end(), _Entropy.begin(), _Entropy.end() );

        return H;
    }
    else return _Entropy;
}

template<class S>
std::vector<double> ArnoldiMC<S>::EntropyRestart(bool inactive ) const{

    if( inactive and not _EntropyRestart_inactive.empty() ){
        std::vector<double> H( _EntropyRestart_inactive );
        H.reserve( _EntropyRestart_inactive.size() + _EntropyRestart.size() );

        H.insert(H.end(), _EntropyRestart.begin(), _EntropyRestart.end() );

        return H;
    }
    else return _EntropyRestart;
}

/*MeanVector will calculate the mean and standard deviation of the Ritzvector
 * estimates calculated and stored at each restart.
 *
 * index: Which Ritzvector to calculate mean of; 0 = largest eigenvalue.*/
template<class S>
void ArnoldiMC<S>::MeanVector(unsigned int index, c_vec_t& Mean, c_vec_t& StdDev) {
    MeanRestartVector(index, _R, Mean, StdDev);
}

/*MeanRestartVector will calculate the mean and standard deviation of the
 * Ritzcector estimates up to the restart desiginated by R.
 *
 * index: Which Ritzvector to calculate mean of; 0 = largest eigenvalue.*/
template<class S>
void ArnoldiMC<S>::MeanRestartVector(unsigned int index, unsigned int R, 
        c_vec_t& Mean, c_vec_t& StdDev) {

    // Initialize
    Mean.resize(_length); Mean *= 0.0;
    StdDev.resize(_length); StdDev *= 0.0;
    c_vec_t sum2(_length, 0.0);      // Sum of squares
    double N = _RitzvectorsEst.size();

    c_vec_t Vector(_length);
    // Iterate over Ritzvector estimates
    vector<c_mat_t>::iterator RVIter = _RitzvectorsEst.begin();
    for(unsigned int r = 0; r <= R, RVIter != _RitzvectorsEst.end(); 
            ++r, ++RVIter ){
        int ind = RVIter->size2()-1 - index;
        Vector = c_column_t(*RVIter, ind);
        // Fix "upside-down" vectors
        complex<double> DotProd = ublas::inner_prod(Vector, Mean);
        if( DotProd.real() < 0.0 ) Vector *= -1.0;

        Mean += Vector;
        sum2 += element_prod(Vector, Vector);
    }

    Mean /= N;
    StdDev = (1.0/N)*( sum2/N - element_prod(Mean, Mean) );
    for( c_vec_t::iterator iter = StdDev.begin(); iter != StdDev.end(); 
            ++iter ){
        *iter = std::sqrt(*iter);
    }

}

/*VectorEst will return a std::vector of the eigenvector estimates at each 
 * restart.
 *
 * index: Which eigenvector is desired, index = 0 is fundamental
 * inactive: include estimates from inactive cycles*/
template<class S>
std::vector<typename ArnoldiMC<S>::c_vec_t> ArnoldiMC<S>::VectorEst(unsigned int index, 
        bool inactive) {

    c_vec_t Reference(_length);
    c_vec_t Vector(_length);
    MeanVector(index, Reference, Vector);
    complex<double> dot;

    index = _RitzvectorsEst[0].size2()-1-index;
    unsigned int size = _RitzvectorsEst.size();
    if( inactive and _inactive ) size += _RitzvectorsEst_inactive.size();
    std::vector<c_vec_t> eigenvectors( size );

    typename std::vector<c_mat_t>::iterator rIter;
    typename std::vector<c_vec_t>::iterator eIter = eigenvectors.begin();
    // Iterate over inactive vector estimates
    if( inactive and _inactive ){
        rIter = _RitzvectorsEst_inactive.begin();
        for( ; rIter != _RitzvectorsEst_inactive.end(); ++rIter, ++eIter ){
            Vector = c_column_t(*rIter, index);
            dot = inner_prod(Vector, Reference);
            // Make sure we are not getting "upside-down" vectors
            if( dot.real() < 0 ) Vector *= -1;
            *eIter = Vector;
        }
    }

    // Iterate over active vector estimates
    rIter = _RitzvectorsEst.begin();
    for( ; eIter != eigenvectors.end(); ++eIter, ++rIter ){
        Vector = c_column_t(*rIter, index);
        dot = inner_prod(Vector, Reference);
        // Make sure we are not getting "upside-down" vectors
        if( dot.real() < 0 ) Vector *= -1;
        *eIter = Vector;
    }
    return eigenvectors;
}

/*Residual returns the vector of residual values calculated during the Arnoldi
 * process.  
 *
 * inactive: If true (by default), also include residual from inactive cycles.*/
template<class S>
std::vector<complex<double> > ArnoldiMC<S>::Residual ( bool inactive ) const {
    if( inactive ){
        std::vector<complex<double> > r(_Residual_inactive);
        r.insert( r.end(), _Residual.begin(), _Residual.end() );
        return r;
    }
    else return _Residual;

}

/*MeanValues will calculate (if needed) the mean and standard deviation of the
 * eigenvalue estimates.
 *
 * Mean: Return vector
 * SD: Return vector
 * index: Which Eigenvalue of which to calculate mean; 0 = dominant
 * inactive: If true (by default), also include residual from inactive cycles.*/
template<class S>
void ArnoldiMC<S>::MeanValues (std::vector<complex<double> >& Mean, 
        std::vector<complex<double> >& SD, unsigned int ind, 
        bool inactive ) const{

    unsigned int index = _EigenvaluesEst.size1()-1-ind;

    // Eigenvalue estimates from active restarts
    // Take first _R elements
    c_vec_t in( ublas::subrange(row(_EigenvaluesEst, (index)), 0,(_R-_inactive) ) );

    Mean.clear(); Mean.reserve(in.size());
    SD.clear(); SD.reserve(in.size());
    complex<double> m(0.0);
    complex<double> sum; complex<double> sum2; complex<double> element;

    // Calculate means and standard deviations
    double N = 1.0;
    c_vec_t::iterator inIter = in.begin();
    for( ; inIter != in.end(); ++inIter, N+=1.0 ){
        element = *inIter;
        sum += element; sum2 += element*element;
        m = sum/N;
        Mean.push_back(m);
        SD.push_back(std::sqrt( (1.0/N)*( sum2/N - m*m ) ) );
    }

    if( inactive and _inactive ){
        index = _EigenvaluesEst_inactive.size1()-1-ind;
        // Eigenvalue estimates from inactive restarts
        c_vec_t in( row( _EigenvaluesEst_inactive, index ) );
        Mean.insert( Mean.begin(), in.begin(), in.end() );
        SD.insert( SD.begin(), in.size(), 0.0 );
    }
    cout << endl;
}

/*Values returns the raw Eigenvalue estimates
 * index: Which Eigenvalue of which to calculate mean; 0 = dominant
 * inactive: If true (by default), also include residual from inactive cycles.*/
template<class S>
std::vector<complex<double> > ArnoldiMC<S>::Values(unsigned int index, 
        bool inactive) const{
    index = _EigenvaluesEst.size1()-1-index;
    c_vec_t r(row(_EigenvaluesEst, index));
    std::vector<complex<double> > v(r.begin(), r.begin()+(_R-_inactive));

    if( inactive ){
        r = row(_EigenvaluesEst_inactive, index);
        v.insert(v.begin(), r.begin(), r.end() );
    }

    return v;
}

#endif

