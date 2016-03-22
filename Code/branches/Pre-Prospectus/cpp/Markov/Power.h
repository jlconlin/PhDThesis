/*Power.h
 $Author: jlconlin $
 $Id: Power.h 238 2008-01-05 22:05:09Z jlconlin $
 $Revision: 238 $
 $Date: 2008-01-05 15:05:09 -0700 (Sat, 05 Jan 2008) $

 This file contains header information for the Power class.
 The Power class is an implementation of the Monte Carlo power method.

 */

#include<iostream>
#include<iomanip>
#include<cmath>
#include "Markov.h"
#include "Field.hh"
#include "BankSource.h"
#include "HistSource.h"

#ifndef POWER_H
#define POWER_H

using std::setw;
using std::setprecision;
using std::fixed;
using std::cout;

template<typename S>
class Power : public Markov<S> {
    public:
        Power(const vector<unsigned long>& seed, 
              const Field<Zone, Material>& field)
            : Markov<S>(seed, field)
        { _k = 1; }

        ~Power() {  }
        void power(S*, int, int, int); 

        //Gettr function
        double k() const { return _k; }
        void score(S& Source, Particle& P);

    private:
        double _k;  //Eigenvalue estimate
        vector<double> _eigEstimates;   //Estimates of eigenvalue at each iteration
        vector<double> _meanEigActive;
        vector<double> _varEig;

        void _iteration(BankSource&, int);
        void _calcStatistics();
};

/* power is the workhorse function of this class.  It performs the Power 
 * Method calculation.
 *
 * source: Source where Particles are sampled from
 * active: Number of active iterations
 * inactive: Number of inactive iterations
 * histories: Number of histories per iteration*/
template<typename S>
void Power<S>::power(S* currentSource, int active, int inactive, 
        int histories){

    clock_t start = clock();     //Start clock
    clock_t now;
    S* nextSource;
    for( int i = 0; i < inactive; i++){
        nextSource = Markov_Transport(currentSource, histories);

        //Update eigenvalue estimate
        _k = _k*(nextSource->size()/static_cast<double>(histories));    
        delete currentSource;
        currentSource = nextSource;
        now = clock();
        cout.setf(std::ios_base::fixed, std::ios_base::floatfield);
        cout << "inactive:" << setw(5) << (i+1) 
            << ", k =" << setprecision(4) << setw(7) << _k 
            << ", time:" << setprecision(2) << setw(7) 
            << double(now-start)/CLOCKS_PER_SEC << " sec." << std::endl;
    }

    std::cout << "--------------ACTIVE ITERATIONS--------------------------\n";
    for( int i = 0; i < active; i++){
        nextSource = Markov_Transport(currentSource, histories);
        //Update eigenvalue estimate
        _k = _k*(nextSource->size()/static_cast<double>(histories));    
        _calcStatistics();
        delete currentSource;
        currentSource = nextSource;
        cout << "active:" << setw(5) << (i+1) 
            << ", k =" << setprecision(4) << setw(7) << _k 
            << ", mean k =" << setprecision(4) << setw(7) 
            << _meanEigActive.back() 
            << ", std. dev. =" << setprecision(4) << setw(7) 
            << std::sqrt(_varEig.back())
            << ", time:" << setprecision(2) << setw(7) 
            << double(now-start)/CLOCKS_PER_SEC << " sec." << std::endl;
        now = clock();
/*        std::cout << "active: " << (i+1) << ", k = " << _k 
                  << ", mean k = " << _meanEigActive.back() 
                  << ", std. dev. = " << std::sqrt(_varEig.back())
                  << ", time: " << double(now - start)/CLOCKS_PER_SEC << std::endl;*/
    }
    delete nextSource;
}

/*Power<BankSource> score will add a random number of Particles to the source.*/
template<>
void Power<BankSource>::score(BankSource& source, Particle& P) {
    double xi = _r.Fixed();
    const Material& mat = _field[*(P.zone())];
    int N = std::abs(P.weight())*(1.0/_k)*(mat.nu()*mat.xF()/mat.xT()) + xi;
    for( int i = 0; i< N; i++ ){ source.push_back(new Particle(P)); }
}

/*Power<HistSource> will increase the size of a bin a random integer amount.*/
template<>
void Power<HistSource>::score(HistSource& source, Particle& P) {
    double xi = _r.Fixed();
    const Material& mat = _field[*(P.zone())];
    int N = std::abs(P.weight())*(1.0/_k)*(mat.nu()*mat.xF()/mat.xT()) + xi;
//    std::cout << "*P.zone(): " << source[*P.zone()] << ", N = " << N << "\n";
    source[*P.zone()] += N;
//    std::cout << "*P.zone(): " << source[*P.zone()] << ", N = " << N << "\n";
}


/* _calcStatistics will calulate the mean and variance of the eigenvalue
 * estimates so far and append their results to _mean_k_active and 
 * _var_mean_k_active respectively.*/
template<typename S>
void Power<S>::_calcStatistics(){
    _eigEstimates.push_back(_k);
    double sumEig = 0;  //Sum of eigenvalue estimates
    double sum2Eig = 0; //Sum of squares of eigenvalues estimates
    int N = _eigEstimates.size();
    for ( int i = 0; i< _eigEstimates.size(); i++){
        sumEig += _eigEstimates[i];
        sum2Eig += _eigEstimates[i]*_eigEstimates[i];
    }
    double mean  = sumEig/N;
    _meanEigActive.push_back(mean);
    _varEig.push_back(sum2Eig/N - mean*mean);
}
#endif
