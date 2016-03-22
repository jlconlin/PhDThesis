/*Power.h
 $Author: jlconlin $
 $Id: Power.h 298 2008-05-12 17:44:56Z jlconlin $
 $Revision: 298 $
 $Date: 2008-05-12 11:44:56 -0600 (Mon, 12 May 2008) $

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
#include "Markov.h"
#include "Field.hh"
#include "BankSource.h"
#include "HistSource.h"
#include "Utilities.h"

using std::setw;
using std::setprecision;
using std::fixed;
using std::cout;
using std::endl;

template<typename S>
class Power : public Markov<S> {
    public:
        Power(const vector<unsigned long>& seed, 
              const Field<Zone, Material>& field, Mesh& M)
            : Markov<S>(seed, field), _k(1), _eigEstimates(), _active(false),
              _meanEigActive(), _stddevEig(), _CycleHistories(), _DiscMesh(M),
              _InactiveHistories(0), _InactiveEstimates()
        {   }

        ~Power() {  }
        void power(S*, int, int, unsigned int);

        //Gettr function
        double k() const { return _k; }
        /*Returns the active eigenvalue estimates.*/
        vector<double> EigEstimates() const{ return _eigEstimates; }
        vector<double> MeanVector() const { return _MeanVector; }
        vector<double> StdDevVector() const { return _StdDevVector; }
        vector<double> HperI() const { return _CycleHistories; }
        double InactiveHistories() const { return _InactiveHistories; }

        /*Returns the eigenvalue estimates at each iteration.  Includes the 
         * estimates from the inactive iterations as well as the mean of the 
         * active iterations.*/
        vector<double> MeanValues() const { 
            vector<double> tmp(_InactiveEstimates);
            tmp.reserve(_CycleHistories.size());

            tmp.insert(tmp.end(), _meanEigActive.begin(), _meanEigActive.end());

            return tmp;
        }

        /*StdDevValues returns the standard deviation of the mean of the 
         * eigenvalue estimates at each iteration.  For inactive cycles the 
         * standard deviation is zero.*/
        vector<double> StdDevValues() const { 
            vector<double> tmp(_InactiveEstimates.size(), 0.0);
            tmp.reserve(_CycleHistories.size());

            tmp.insert( tmp.end(), _stddevEig.begin(), _stddevEig.end() );

            return tmp; 
        }

    private:
        void _power(S*, int, int, int); 
        Mesh& _DiscMesh;

        double _k;  //Eigenvalue estimate
        vector<double> _eigEstimates;   // Active Eigenvalue estimates
        vector<double> _InactiveEstimates; // Inactive Eigenvalue estimates
        vector<double> _meanEigActive;
        vector<double> _stddevEig;
        vector<vector<double> > _CycleVectors;   // Estimates of eigenvector
        vector<double> _MeanVector;
        vector<double> _StdDevVector;
        vector<double> _CycleHistories;

        bool _active;       // Whether or not we are in active cycles
        double _InactiveHistories;    // Number of inactive histories

        void _score(S& Source, Particle& P);
        void _calcStatistics(S&);
        void _VectorStats(S&);
        void _CalcMeanVector();
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
        unsigned int histories){

    _active = false;
    _InactiveHistories = 0;

    clock_t start = clock();     //Start clock
    clock_t now;
    S* nextSource = new BankSource( currentSource->seed() );
    S* tmpSource;
    for( int i = 0; i < inactive; i++){
        Markov_Transport(*currentSource, *nextSource, histories);

        //Update eigenvalue estimate
        _k = _k*(nextSource->size()/static_cast<double>(histories));    
        _InactiveEstimates.push_back(_k);

        // Update Sources
        tmpSource = currentSource; currentSource = nextSource;
        nextSource = tmpSource; nextSource->reset();

        // Store number of histories tracked
        if( i == 0 ) _CycleHistories.push_back(histories);
        else  _CycleHistories.push_back(_CycleHistories.back() + histories); 

        now = clock();
        cout.setf(std::ios_base::fixed, std::ios_base::floatfield);
        cout << "inactive:" << setw(5) << (i+1) 
            << ", k =" << setprecision(4) << setw(7) << _k 
            << ", time:" << setprecision(2) << setw(7) 
            << double(now-start)/CLOCKS_PER_SEC << " sec." << endl;
    }

    _active = true;
    _InactiveHistories = _CycleHistories.back();
    std::cout << "--------------ACTIVE ITERATIONS--------------------------\n";
    for( int i = 0; i < active; i++){
//      nextSource = Markov_Transport(currentSource, histories);
        Markov_Transport(*currentSource, *nextSource, histories);

        //Update eigenvalue estimate
        _k = _k*(nextSource->size()/static_cast<double>(histories));    
        _calcStatistics(*nextSource);

        // Update Sources
        tmpSource = currentSource; currentSource = nextSource;
        nextSource = tmpSource; nextSource->reset();

        _CycleHistories.push_back(_CycleHistories.back() + histories); 
        now = clock();
        cout << "active:" << setw(5) << (i+1) 
            << ", k =" << setprecision(4) << setw(7) << _k 
            << ", mean k =" << setprecision(4) << setw(7) 
            << _meanEigActive.back() 
            << ", std. dev. =" << setprecision(4) << setw(7) 
            << _stddevEig.back()
            << ", time:" << setprecision(2) << setw(7) 
            << double(now-start)/CLOCKS_PER_SEC << " sec." << endl;
    }
    delete nextSource;
}

/*Power<BankSource> score will add a random number of Particles to the source.*/
template<>
void Power<BankSource>::_score(BankSource& source, Particle& P) {
    double xi = _r.Fixed();
    const Material& mat = _field[*(P.zone())];
    int N = std::abs(P.weight())*(1.0/_k)*(mat.nu()*mat.xF()/mat.xT()) + xi;
    for( int i = 0; i< N; i++ ){ source.push_back(&P); }
}

/*Power<HistSource> will increase the size of a bin a random integer amount.*/
template<>
void Power<HistSource>::_score(HistSource& source, Particle& P) {
    double xi = _r.Fixed();
    const Material& mat = _field[*(P.zone())];
    int N = std::abs(P.weight())*(1.0/_k)*(mat.nu()*mat.xF()/mat.xT()) + xi;
    source[*(source.getZone(P.x()))] += N;
}


/* _calcStatistics will calulate the mean and variance of the eigenvalue
 * estimates so far and append their results to _mean_k_active and 
 * _var_mean_k_active respectively.*/
template<typename S>
void Power<S>::_calcStatistics(S& CurrentSource){
    _eigEstimates.push_back(_k);
    double mean; double stddev;
    StdDevMean(_eigEstimates, mean, stddev);
    _meanEigActive.push_back(mean);
    _stddevEig.push_back(stddev);

    _VectorStats(CurrentSource);
}

template<>
void Power<BankSource>::_VectorStats(BankSource& CurrentSource){
    vector<double> PDF = CurrentSource.discretized(_DiscMesh);
    Norm(PDF);
//  double sum = 0;
//  double value = 0;
//  vector<double>::iterator PDFIter = PDF.begin();
//  for( ; PDFIter != PDF.end(); ++PDFIter ){
//      value = std::abs(*PDFIter);
//      sum += value*value;
//  }
//  sum = std::sqrt(sum);

//  PDFIter = PDF.begin();
//  for( ; PDFIter != PDF.end(); ++PDFIter ) *PDFIter = *PDFIter/sum; 

    _CycleVectors.push_back(PDF);
    _CalcMeanVector();
}

template<>
void Power<HistSource>::_VectorStats(HistSource& CurrentSource){
    vector<double> PDF = CurrentSource.getPDF();
    double sum = 0;
    double value = 0;
    vector<double>::iterator PDFIter = PDF.begin();
    for( ; PDFIter != PDF.end(); ++PDFIter ){
        value = std::abs(*PDFIter);
        sum += value*value;
    }
    sum = std::sqrt(sum);

    PDFIter = PDF.begin();
    for( ; PDFIter != PDF.end(); ++PDFIter ) *PDFIter = *PDFIter/sum; 

    _CycleVectors.push_back(PDF);
    _CalcMeanVector();
}

template<typename S>
void Power<S>::_CalcMeanVector(){
    int N = _CycleVectors.size();
    int n = _CycleVectors[0].size();

    _MeanVector = vector<double>(n, 0.0);
    _StdDevVector = vector<double>(n, 0.0);

    vector<double> sum(n, 0.0);
    vector<double> sum2(n, 0.0);
    vector<double>::iterator sumIter;
    vector<double>::iterator sum2Iter;

    double value;
    // Iterator over _CycleVectors
    vector<vector<double> >::iterator CVIter = _CycleVectors.begin();
    vector<double>::iterator subIter;   // Iterator for one CycleVector

    // Calculate sums and sum of squares
    for( ; CVIter != _CycleVectors.end(); ++CVIter ){
        subIter = CVIter->begin();
        sumIter = sum.begin();
        sum2Iter = sum2.begin();
        for( ; sumIter != sum.end(); ++sumIter, ++sum2Iter, ++subIter ){
            value = *subIter;
            *sumIter += value;
            *sum2Iter += value*value;
        }
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

#endif
