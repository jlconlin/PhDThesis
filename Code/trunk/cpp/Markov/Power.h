/*Power.h
 $Author: jlconlin $
 $Id: Power.h 688 2009-07-08 18:11:45Z jlconlin $
 $Revision: 688 $
 $Date: 2009-07-08 12:11:45 -0600 (Wed, 08 Jul 2009) $

 This file contains header information for the Power class.
 The Power class is an implementation of the Monte Carlo power method.

 */

#ifndef POWER_H
#define POWER_H

#include<iostream>
#include<iomanip>
#include<cmath>
#include<ctime>
#include<numeric>
#include "boost/numeric/ublas/vector.hpp"
#include "boost/format.hpp"
#include "Markov.h"
#include "CartesianMesh1D.hh"
#include "Field.hh"
#include "BankSource.h"
#include "HistSource.h"
#include "Utilities.h"
#include "boost/date_time/posix_time/posix_time.hpp"
#include "boost/date_time/microsec_time_clock.hpp"

using std::cout;
using std::endl;
using boost::format;
using boost::posix_time::ptime;

template<typename S>
class Power : public Markov<S> {
    public:
        Power(const vector<unsigned long>& seed, 
              const typename Markov<S>::field_t& field, CartesianMesh<OneD>& M)
            : Markov<S>(seed, field), _k(1), _eigEstimates(), _active(false),
              _meanEigActive(), _stddevEig(), _Tracks(), _Tracks_inactive(),
              _DiscMesh(M), _InactiveEstimates(), _Ent(0.0), _Entropy(), 
              _Entropy_inactive(), _PDF()
        {   }

        ~Power() {  }
        void power(S*, unsigned int, unsigned int, unsigned int);

        //Gettr function
        double k() const { return _k; }
        /*Returns the active eigenvalue estimates.*/
        std::vector<double> EigEstimates(bool inactive=true) const{ 
            if( inactive and not _InactiveEstimates.empty() ){
                std::vector<double> E( _InactiveEstimates );
                E.insert(E.end(), _eigEstimates.begin(), _eigEstimates.end() );
                return E;
            }
            return _eigEstimates; 
        }
        std::vector<double> MeanVector() const { return _MeanVector; }
        std::vector<double> StdDevVector() const { return _StdDevVector; }
        std::vector<double> FOM() const { return _FOM; }
        std::vector<double> Time() const {
            std::vector<double> T( _Time_inactive );
            T.insert(T.end(), _Time.begin(), _Time.end() );
            return T;
        }
        std::vector<double> Entropy() const {
            std::vector<double> H( _Entropy_inactive );
            H.insert(H.end(), _Entropy.begin(), _Entropy.end() );
            return H;
        }
        std::vector<double> Tracks(bool inactive = true) const {
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
                if(not _Tracks_inactive.empty()) inT = _Tracks_inactive.back();

                std::vector<double>::iterator TIter = T.begin();
                for( ; TIter != T.end(); ++TIter ){
                    *TIter += inT;
                }

                return T;
            }
        }

        /*Returns the eigenvalue estimates at each iteration.  Includes the 
         * estimates from the inactive iterations as well as the mean of the 
         * active iterations.*/
        std::vector<double> MeanValues() const { 
            std::vector<double> tmp(_InactiveEstimates);

            tmp.insert(tmp.end(), _meanEigActive.begin(), _meanEigActive.end());

            return tmp;
        }

        /*StdDevValues returns the standard deviation of the mean of the 
         * eigenvalue estimates at each iteration.  For inactive cycles the 
         * standard deviation is zero.*/
        std::vector<double> StdDevValues() const { 
            std::vector<double> tmp(_InactiveEstimates.size(), 0.0);

            tmp.insert( tmp.end(), _stddevEig.begin(), _stddevEig.end() );

            return tmp; 
        }

    private:
        void _power(S*, int, int, int); 
        CartesianMesh<OneD>& _DiscMesh;

        double _k;  //Eigenvalue estimate
        vector<double> _eigEstimates;   // Active Eigenvalue estimates
        vector<double> _InactiveEstimates; // Inactive Eigenvalue estimates
        vector<double> _meanEigActive;
        vector<double> _stddevEig;
        
        // Estimates of eigenvector
        vector< typename Markov<S>::ublas_vec_t > _CycleVectors;   
        vector<double> _MeanVector;
        vector<double> _StdDevVector;
        vector<double> _Tracks;
        vector<double> _Tracks_inactive;

        typename Markov<S>::ublas_vec_t _PDF;
        // Store time
        std::vector<double> _Time;
        std::vector<double> _Time_inactive;

        // Store Figure of Merit 1/(variance*time)
        std::vector<double> _FOM;
        
        // Entropy
        double _Ent;
        std::vector<double> _Entropy;
        std::vector<double> _Entropy_inactive;

        bool _active;       // Whether or not we are in active cycles
        unsigned int _active_Iteration;
        unsigned int _inactive_Iteration;
        double _histories;  // Number of histories to track in each iteration
        ptime _start;
        ptime _now;

        S* _currentSource;
        S* _nextSource;

        // _Iteration is called for each power method cycle
        void _Iteration();

        void _score(S& Source, Particle& P);
        void _calcStatistics();
        void _VectorStats();
        void _CalcMeanVector();
        void _CalcEntropy();
};

/* power is the workhorse function of this class.  It performs the Power 
 * Method calculation.
 *
 * source: Source where Particles are sampled from
 * active: Number of active iterations
 * inactive: Number of inactive iterations
 * histories: Number of histories per iteration*/
template<typename S>
void Power<S>::power(S* currentSource, unsigned int active, 
        unsigned int inactive, unsigned int histories){

    _histories = histories;

    // Initialize stuff
    _currentSource = currentSource;
    _nextSource = new BankSource( currentSource->seed() );
    _active = false;
    _Time.clear();
    _Time_inactive.clear();
    _FOM.clear();

    // Start timing
    _start = boost::date_time::microsec_clock<ptime>::local_time();
    for( _inactive_Iteration = 0; _inactive_Iteration < inactive; 
            _inactive_Iteration++){
        _Iteration();
    }

    _active = true;

    std::cout << "--------------ACTIVE ITERATIONS--------------------------\n";
    for(  _active_Iteration = 0; _active_Iteration < active; 
            _active_Iteration++){
        _Iteration();
    }
}

// _Iteration is called for each power method cycle
template<typename S>
void Power<S>::_Iteration(){
    S* tmpSource;
    Markov_Transport(*_currentSource, *_nextSource, _histories);

    //Update eigenvalue estimate
    _k = _k*(_nextSource->magnitude()/_histories);    

    _now = boost::date_time::microsec_clock<ptime>::local_time();
    double duration = (_now-_start).total_microseconds()/1E6;
    _CalcEntropy();
    if( _active ){
        _calcStatistics();

        // Store Time
        _Time.push_back(duration);
        // Store Figure of Merit
        if( _FOM.empty() ) _FOM.push_back(0.0);
        else _FOM.push_back(1.0/(_stddevEig.back()*_stddevEig.back()*duration));

        // Store number of histories tracked
        if( _Tracks.empty() ) _Tracks.push_back(_histories);
        else  _Tracks.push_back(_Tracks.back() + _histories); 

        cout << format( "active: %4i, H = %5.4G, k = %6.4G, mean k = %6.4G" )
                %_active_Iteration %_Entropy.back() %_k %_meanEigActive.back()
             << format( ", std.dev. = %7.3G, time: %6.4G, FOM = %6.4G" )
                %_stddevEig.back() %duration %_FOM.back()
             << endl;
    }
    else{   // inactive iterations
        // Store Time
        _Time_inactive.push_back(duration);

        // Store number of histories tracked
        if( _Tracks_inactive.empty() ) _Tracks_inactive.push_back(_histories);
        else  _Tracks_inactive.push_back(_Tracks_inactive.back() + _histories); 

        _InactiveEstimates.push_back(_k);
        cout << format( "inactive: %3i, H = %5.4G, k = %6.4G, time = %6.4G" )
                %_inactive_Iteration %_Entropy_inactive.back() %_k %duration
             << endl;
    }

    // Update Sources
    tmpSource = _currentSource; _currentSource = _nextSource;
    _nextSource = tmpSource; _nextSource->reset();

}

/*Power<BankSource> score will add a random number of Particles to the source.*/
template<>
void Power<BankSource>::_score(BankSource& source, Particle& P) {
    double xi = _r.Fixed();
    const Material& mat = _field[*(P.zone())];
    int N = std::abs(P.weight())*(1.0/_k)*(mat.nu()*mat.xF()/mat.xT()) + xi;
    source.score(P, N);
//  for( int i = 0; i< N; i++ ){ source.push_back(&P); }
}

/*Power<HistSource> will increase the size of a bin a random integer amount.*/
template<>
void Power<HistSource>::_score(HistSource& source, Particle& P) {
    double xi = _r.Fixed();
    const Material& mat = _field[*(P.zone())];
    int N = std::abs(P.weight())*(1.0/_k)*(mat.nu()*mat.xF()/mat.xT()) + xi;
    source.score(P, N);
//  source[source.getZone(P.x())] += N;
}

/* _calcStatistics will calulate the mean and variance of the eigenvalue
 * estimates so far and append their results to _mean_k_active and 
 * _var_mean_k_active respectively.*/
template<typename S>
void Power<S>::_calcStatistics(){
    _eigEstimates.push_back(_k);
    double mean; double stddev;
    StdDevMean(_eigEstimates, mean, stddev);
    _meanEigActive.push_back(mean);
    _stddevEig.push_back(stddev);

    _VectorStats();
}

template<>
void Power<BankSource>::_VectorStats(){

//  _PDF = _nextSource->discretized(_DiscMesh);

    // Normalize
    _PDF /= boost::numeric::ublas::norm_2(_PDF);

    _CycleVectors.push_back(_PDF);
    _CalcMeanVector();
}

template<>
void Power<HistSource>::_VectorStats(){
//  _PDF = _nextSource->PDF();

    // Normalize
    _PDF /= boost::numeric::ublas::norm_2(_PDF);

    _CycleVectors.push_back(_PDF);
    _CalcMeanVector();
}

template<typename S>
void Power<S>::_CalcMeanVector(){
    int N = _CycleVectors.size();
    int n = _CycleVectors[0].size();

    _MeanVector = vector<double>(n, 0.0);
    _StdDevVector = vector<double>(n, 0.0);

    typename Markov<S>::ublas_vec_t sum(n, 0.0);
    typename Markov<S>::ublas_vec_t sum2(n, 0.0);
    typename Markov<S>::ublas_vec_t::iterator sumIter;
    typename Markov<S>::ublas_vec_t::iterator sum2Iter;

    double value;
    // Iterator over _CycleVectors
    typename vector< typename Markov<S>::ublas_vec_t >::iterator CVIter;
    CVIter = _CycleVectors.begin();
    vector<double>::iterator subIter;   // Iterator for one CycleVector

    // Calculate sums and sum of squares
    for( ; CVIter != _CycleVectors.end(); ++CVIter ){
        typename Markov<S>::ublas_vec_t& v = *CVIter;
        sum += v;
        sum2 += element_prod(v,v);
    }

    // Calculate means and standard deviations
    double mean = 0.0;
    double mean2 = 0.0;
    vector<double>::iterator MeanIter = _MeanVector.begin();
    vector<double>::iterator SDIter = _StdDevVector.begin();
    sumIter = sum.begin();
    sum2Iter = sum2.begin();
    for( ; MeanIter != _MeanVector.end();
            ++MeanIter, ++SDIter, ++sumIter, ++sum2Iter ){
        mean = *sumIter/static_cast<double>(N);
        *MeanIter = mean;
        mean2 = *sum2Iter/static_cast<double>(N);
//      cout << setprecision(5);
//      cout << "\n(1.0/N) = " << (1.0/N) << endl;
//      cout << "mean2 = " << mean2 << endl;
//      cout << "mean = " << mean << endl;
//      cout << "mean*mean = " << mean*mean << endl;
//      cout << "std::sqrt( (1.0/N)*( mean2 - mean*mean)) = " 
//           << std::sqrt( (1.0/N)*( mean2 - mean*mean)) << endl;
        *SDIter = std::sqrt( (1.0/N)*( mean2 - mean*mean));
//        cout << "*SDIter = " << *SDIter << endl;
    }
//  std::cout << "\nStdDevVector:\n" << _StdDevVector << endl;
//  std::cout << "\nStdDevVector():\n" << StdDevVector() << endl;
}

template<>
void Power<BankSource>::_CalcEntropy(){
    _PDF = _nextSource->discretized(_DiscMesh);
    if( _active ){
        _Entropy.push_back( ShannonEntropy( _PDF ) );
    }
    else{
        _Entropy_inactive.push_back( ShannonEntropy( _PDF ) );
    }
}

template<>
void Power<HistSource>::_CalcEntropy(){
    _PDF = _nextSource->PDF();

    if( _active ) _Entropy.push_back( ShannonEntropy( _PDF ) );
    else _Entropy_inactive.push_back( ShannonEntropy( _PDF ) );
}

#endif
