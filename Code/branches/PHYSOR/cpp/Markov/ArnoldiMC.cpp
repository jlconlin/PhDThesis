/*ArnoldiMC.cpp
 $Author: jlconlin $
 $Id: ArnoldiMC.cpp 397 2008-10-13 21:06:07Z jlconlin $
 $Revision: 397 $
 $Date: 2008-10-13 15:06:07 -0600 (Mon, 13 Oct 2008) $

 The ArnoldiMC class is the C++ implementation of Monte Carlo Arnoldi's method.
 */


#include <iostream>
#include <vector>
#include <complex>
#include <cmath>
#include <algorithm>
#include "Utilities.h"
#include "Assert.hh"
#include "ArnoldiMC.h"
#include "Arnoldi.h"
#include "Teuchos_SerialDenseMatrix.hpp"
#include "Teuchos_SerialDenseVector.hpp"

using std::setw;
using std::setprecision;
using std::fixed;
using std::scientific;
using std::cout;
using std::complex;
using std::vector;
using std::endl;

typedef Teuchos::SerialDenseMatrix<int, double> dSDM;
typedef Teuchos::SerialDenseMatrix<int, complex<double> > cSDM;
typedef Teuchos::SerialDenseVector<int, complex<double> > cSDV;	
typedef vector<valarray<complex<double> > >::iterator cVecValIter;


/*ERAM is Explicitly Restart Arnoldi's Method.  This one takes care of 
 * performing some 'inactive' restarts before tallying.
    source: Initial fission source
    iR: Number of inactive restarts
    aR: Number of active restarts
    I: Number of Iterations per Restart
    H: Number of histories to track per Iteration
    method: Which algorithm for inactive cycles*/
void ArnoldiMC::ERAM( HistSource& source, int iR, int aR, int I, 
        unsigned long int H, Method method ){
    _histories = H;
    _RelaxHistories = H;

    _CycleValues = vector<vector<complex<double> > >();
    _CycleVectors = vector<vector<valarray<complex<double> > > >();
    _MeanValues = vector<vector<complex<double> > >();
    _StdDevValues = vector<vector<complex<double> > >();

    clock_t timer = clock();
    if( _Verbose ){ 
        cout << "\n\n----------------Inactive Restarts----------------\n"; 
    }
    _active = false;
    
    switch(method){
        default:
        case ARNOLDI:   // Perform Arnoldi's method Restarts to converge source
            InactiveARNOLDI(source, iR, I, H);
            break;
        case POWER:     // Perform power method Iterations to converge source
            InactivePOWER(source, I*iR, H);
            break;
    }

    _active = true;
    if( _Verbose ){ 
        cout << "\n\n----------------Active Restarts----------------\n"; 
    }

    // Perform active restarts
    cSDV q = _RestartVector();
    Arnoldi::ERAM(q, aR, I);

    cout << "\nTOTAL TIME:" << setprecision(3) << setw(8) 
         << double(_nowR-timer)/CLOCKS_PER_SEC << " sec." << endl;
}

/*InactiveARNOLDI will perform inactive Restarts
 *
 R: Number of inactive Restarts
 I: Number if Iterations per Restart
 H: Number of Histories tracked per Iteration*/
void ArnoldiMC::InactiveARNOLDI( HistSource& source, int R, int I, int H){
      
    // Make complex array from real source
    vector<double> PDF = source.getPDF();
    _length = PDF.size();
    valarray<complex<double> > s(_length);
    complex<double>* valIter = &(s[0]);
    vector<double>::iterator PDFIter = PDF.begin();
    for( ; PDFIter != PDF.end(); ++PDFIter, ++valIter ) *valIter = *PDFIter;
    _Ritzvectors = vector<valarray<complex<double> > >(2, s);

    // Perform Iterations
    for( _restart = -R; _restart < 0; ++_restart ){
        _Restart(I);
        _InactiveValues.push_back(_Ritzvalues[0].real());
    }
}


/*InactivePOWER will perform inactive Power iterations
 *
 I: Number of inactive Iterations
 H: Number of Histories tracked per Iteration*/
void ArnoldiMC::InactivePOWER(HistSource& source, int I, int H){
}


cSDV ArnoldiMC::_Aq(){
    // Make vector of probabilities
    complex<double>* QIter = _Q[_k];
    vector<double> Probabilities(_length);
    vector<double>::iterator ProbIter = Probabilities.begin();
    for( ; ProbIter != Probabilities.end(); ++ProbIter, ++QIter ){
        *ProbIter = (*QIter).real();
    }

    // Convert to fission source
    HistSource CurrentSource(_r.Seed(), _ScoringMesh, Probabilities);
    HistSource NextSource( _ScoringMesh, _r.Seed() );
    Markov_Transport(CurrentSource, NextSource, _RelaxHistories);

    cSDV q = _Scale(NextSource, CurrentSource); // Make return vector
    return q;
}


/*_Scale will process the results from a Monte Carlo run and scale it so the
 area under the curve is 1.
 
  N: New fission source
  C: Current or old fission Source*/
cSDV ArnoldiMC::_Scale(HistSource& N, HistSource& C){
    double size = C.size();
    double Scaler = size/_RelaxHistories;

    vector<double> PDF = N.getPDF();
    cSDV q(PDF.size());
    vector<double>::iterator SIter = PDF.begin();
    complex<double>* qIter = q.values();
    for( ; SIter != PDF.end(); ++SIter, ++qIter ){
        *qIter = (*SIter)*(Scaler);
    }

    return q;
}

void ArnoldiMC::_Iteration(){
    cout.setf(std::ios_base::fixed, std::ios_base::floatfield);
    Arnoldi::_Iteration();
    _IterationHistories += _RelaxHistories;

    // Relax number of histories
    if( (_relaxed > 0.0) and _active ){
        double res = _Residual.back();
        double eta2 = _relaxed*_relaxed;
        _RelaxHistories = _histories*( std::min(res*res, eta2)/(eta2) );
    }

    _nowI = clock();
    if( _Verbose ){
        cout << "\tIteration #: " << setw(3) << _k+1
             << ", Residual = " << setprecision(5) << setw(7) << fixed
             << _Residual.back() << ", N = " << setw(5) << _RelaxHistories 
             << ", time: " << double(_nowI-_startI)/CLOCKS_PER_SEC << " sec." 
             << endl;
    }
}


void ArnoldiMC::_Restart(int I){
    cout.setf(std::ios_base::fixed, std::ios_base::floatfield);
      
    // Restart vector is real part largest Ritzvector from previous iteration
    cSDV q = _RestartVector();
    Arnoldi::arnoldi(q, I);

    // Normalize vectors to power of 1
    cVecValIter vecIter;
    for( vecIter = _Ritzvectors.begin(); vecIter != _Ritzvectors.end();
            ++vecIter ){
        Norm(*vecIter);
    }

    // Fix first harmonic
    _FixFirstHarmonic();
    // Calculate statistics
    _CycleVectors.push_back(_Ritzvectors);
    _ValueStats();
    _VectorStats();

    // Store number of histories tracked
    if( _HperR.size() <= 0 ) _HperR.push_back(_IterationHistories);
    else _HperR.push_back(_IterationHistories + _HperR.back());
    _IterationHistories= 0;
    _RelaxHistories = _histories;
    
//  cout << "H:\n" << _H << endl;
//  cout <<"restart vector:\n" << q << endl;
//  cout << "_Eigenvalues:\n" << _Ritzvalues  << endl;
//  cout << "_Ritzvectors:\n" << _Ritzvectors << endl;

    _nowR = clock();
    if( _Verbose ){
        if( _active ){
            cout << "Restart #:" << setw(4) << _restart
                 << ", Eigvalue =" << setprecision(5) << setw(19) 
                 << _Ritzvalues[0] << ", Mean =" << _MeanValues[0].back()
                 << ", std. dev. =" << _StdDevValues[0].back().real()
                 << ", residual =" << scientific << setprecision(2) << setw(9)
                 << _Residual.back() << ", time:" << fixed << setprecision(3)
                 << setw(8) << double(_nowR-_startR)/CLOCKS_PER_SEC << " sec." 
                 << endl;
        }
        // Inactive cycles
        else cout << "Restart #" << setw(4) << _restart << endl;
    }

}


// Restart vector is real part largest Ritzvector from previous iteration
cSDV ArnoldiMC::_RestartVector(){
    cSDV q; q.size(_length);    // Define and initialize

    for( int i = 0; i < 1; ++i ){
        complex<double>* RVIter = &((_Ritzvectors[i])[0]);  
        complex<double>* qIter = q.values();
        for( int i = 0; i < _length; ++i, ++qIter, ++RVIter ){
    //      *qIter = RVIter->real();
            *qIter += std::abs(*RVIter);
        }
    }
    return q;
}


/*_ValueStats will calculate the mean and standard deviation of the Ritzvalues
 * calculated at each Restart.*/
void ArnoldiMC::_ValueStats(){
    if( _restart == 0 ){    // Set standard deviations zero
        _CycleValues = vector<vector<complex<double> > >( _Ritzvalues.size() );
        _MeanValues = vector<vector<complex<double> > >( _Ritzvalues.size() );
        _StdDevValues = vector<vector<complex<double> > >( _Ritzvalues.size() );
    }
    complex<double> Mean;
    complex<double> StdDev;

    // Iterators
    typedef vector<vector<complex<double> > >::iterator Iter;
    Iter CVIter = _CycleValues.begin();
    Iter MVIter = _MeanValues.begin();
    Iter SDIter = _StdDevValues.begin();
    vector<complex<double> >::iterator RVIter = _Ritzvalues.begin();  

    // Add current estimates, calculate and store means
    for( ; CVIter != _CycleValues.end(); 
            ++CVIter, ++MVIter, ++SDIter, ++RVIter ){
        CVIter->push_back(*RVIter);
        StdDevMean(*CVIter, Mean, StdDev);
        MVIter->push_back(Mean);
        SDIter->push_back(StdDev);
    }
}


/*_VectorStats will calculate the mean and standard deviation of the Ritzvectors
 * calculated at each Restart.*/
void ArnoldiMC::_VectorStats(){

    // Initialize
    int N = _CycleVectors.size();       // Number of Restarts
    int n = _CycleVectors[0].size();    // Number of vectors in each Restart
    int l = _CycleVectors[0][0].size(); // Length of each vector


    if( _restart > 0 ){
        // Add vectors to storage
        _MeanVectors = vector<valarray<complex<double> > >();
        _StdDevVectors = vector<valarray<complex<double> > >();
        valarray<complex<double> > v(l);
        for( int i = 0; i < n; ++i ) {
            _MeanVectors.push_back(v);
            _StdDevVectors.push_back(v);
        }

        vector<valarray<complex<double> >  > sum2(n, v);  // Sum of square
        vector<vector<valarray<complex<double> > > >::iterator XIter;
        cVecValIter ElemIter; cVecValIter sumIter; cVecValIter sum2Iter;
        valarray<complex<double> >   ElementValue(_CycleVectors[0][0].size());

        // Calculate sum(x) and sum(x^2)
        for( XIter = _CycleVectors.begin() ; XIter != _CycleVectors.end(); 
                ++XIter ){
            ElemIter = XIter->begin();
            sumIter = _MeanVectors.begin();
            sum2Iter = sum2.begin();
            for( ; ElemIter != XIter->end(); 
                    ++ElemIter, ++sumIter, ++sum2Iter ){
                ElementValue = *ElemIter;
                *sumIter += ElementValue;
                *sum2Iter += ElementValue*ElementValue;
            }
        }

        cVecValIter MIter = _MeanVectors.begin();
        cVecValIter SDIter = _StdDevVectors.begin();
        sum2Iter = sum2.begin();
        for( ; MIter != _MeanVectors.end(); ++MIter, ++SDIter, ++sum2Iter ){
            *MIter *= (1.0/static_cast<double>(N));
            *sum2Iter *= (1.0/static_cast<double>(N));
            // stddev = sqrt( (1/N)(<x^2> - <x>^2 )
            *SDIter = std::sqrt( (1.0/N)*( (*sum2Iter) - (*MIter)*(*MIter) ) );
        }
    }
    else if( _restart == 0 ){
        _MeanVectors = _Ritzvectors;
        valarray<complex<double> > v(l);
        _StdDevVectors = vector<valarray<complex<double> > >(n,v);
    }

}


/*RealMeanValues will split the complex Ritzvalue estimates into real and 
 * imaginary parts and return the real part of the Mean and Standard Deviation.
   index: Which Ritzvalue
   Mean: Real part of mean of Ritzvalue estimates---RETURNED
   StdDev: Real part of standard deviation of Ritzvalue estimates---RETURNED*/
void ArnoldiMC::RealMeanValues(int index, vector<double>& Mean, 
        vector<double>& StdDev){
    vector<double> ImagValues; vector<double> ImagStdDev;
    SplitComplexVector(_MeanValues[index], Mean, ImagValues);
    SplitComplexVector(_StdDevValues[index], StdDev, ImagStdDev);
}


/*ImagMeanValues will split the complex Ritzvalue estimates into real and 
 * imaginary parts and return the real part of the Mean and Standard Deviation.
   index: Which Ritzvalue
   Mean: Imag part of mean of Ritzvalue estimates---RETURNED
   StdDev: Imag part of standard deviation of Ritzvalue estimates---RETURNED*/
void ArnoldiMC::ImagMeanValues(int index, vector<double>& Mean, 
        vector<double>& StdDev){
    vector<double> RealValues; vector<double> RealStdDev;
    SplitComplexVector(_MeanValues[index], RealValues, Mean);
    SplitComplexVector(_StdDevValues[index], RealStdDev, StdDev);
}


/*RealMeanVectors returns the real part of the mean of the Ritzvector estimates
 * */
void ArnoldiMC::RealMeanVectors(int index, vector<double>& Mean,
        vector<double>& StdDev){
    Mean = vector<double>();
    StdDev = vector<double>();
    valarray<complex<double> > v = _MeanVectors[index];
    valarray<complex<double> > stddev = _StdDevVectors[index];
    for( int i = 0; i < v.size(); ++i ){
        Mean.push_back(v[i].real());
        StdDev.push_back(stddev[i].real());
    }
}


/*ImagMeanVectors returns the imaginary part of the mean of the Ritzvector 
 * estimates. */
void ArnoldiMC::ImagMeanVectors(int index, vector<double>& Mean,
        vector<double>& StdDev){
    Mean = vector<double>();
    StdDev = vector<double>();
    valarray<complex<double> > v = _MeanVectors[index];
    valarray<complex<double> > stddev = _StdDevVectors[index];
    for( int i = 0; i < v.size(); ++i ){
        Mean.push_back(v[i].imag());
        StdDev.push_back(stddev[i].imag());
    }
}

/*_FixFirstHarmonic ensures the first harmonic doesn't switch signs between
 * iterations.  It's a hack.*/
void ArnoldiMC::_FixFirstHarmonic(){
    if( _restart > 0 ){
        vector<double> vbarReal; vector<double> Imag;
        SplitComplexValarray(_MeanVectors[1], vbarReal, Imag);
        vector<double> vReal;
        SplitComplexValarray(_Ritzvectors[1], vReal, Imag);

        // Dot product of real parts
        double dot = 0.0;
        vector<double>::iterator vIter = vReal.begin();
        vector<double>::iterator barIter = vbarReal.begin();
        for( ; vIter != vReal.end(); ++vIter, ++barIter ){
            dot += (*vIter)*(*barIter);
        }
        if( dot < 0 ) _Ritzvectors[1] *= -1;
    }
}
