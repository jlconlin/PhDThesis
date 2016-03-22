/*ArnoldiMC.h
 $Author: jlconlin $
 $Id: ArnoldiMC.h 300 2008-05-15 17:26:48Z jlconlin $
 $Revision: 300 $
 $Date: 2008-05-15 11:26:48 -0600 (Thu, 15 May 2008) $

 The ArnoldiMC class is the C++ implementation of Monte Carlo Arnoldi's method.
 */

#ifndef ARNOLDIMC_H
#define ARNOLDIMC_H

#include<vector>
#include<complex>
#include<valarray>
#include "Field.hh"
#include "OneDCartMesh.hh"
#include "Markov.h"
#include "HistSource.h"
#include "Particle.h"
#include "Teuchos_SerialDenseMatrix.hpp"
#include "Teuchos_SerialDenseVector.hpp"

class Arnoldi;
#include "Arnoldi.h"

using std::complex;
using std::vector;
using std::valarray;

typedef Teuchos::SerialDenseVector<int, double> dSDV;
typedef Teuchos::SerialDenseMatrix<int, double> dSDM;
typedef Teuchos::SerialDenseMatrix<int, complex<double> > cSDM;
typedef Teuchos::SerialDenseVector<int, complex<double> > cSDV;

enum Method { ARNOLDI, POWER };

class ArnoldiMC: public Markov<HistSource>, public Arnoldi {
    public:
        /*seed: Seed for random number generator
          geometry: Geometry of problem for scoring
          ScoringMesh: Spatial discretization for scoring
          v: Verbose true/false
          rT: Tolerance for Residual
          iT: Tolerance for Invariance*/
        ArnoldiMC(const vector<unsigned long>& seed,
                const Field<Zone, Material>& geometry,
                Mesh& ScoringMesh, double relaxed = 0.0,
                bool v = true, double rT = 1E-10, double iT = 1E-12)
            : Arnoldi(v, rT, iT), Markov<HistSource>(seed, geometry),
              _CycleValues(), _CycleVectors(), _relaxed(relaxed),
              _StdDevValues(), _StdDevVectors(), _IterationHistories(),
              _histories(), _RelaxHistories(), _ScoringMesh(ScoringMesh) { }

        void ERAM( HistSource& source, int iR, int aR, int I,
                 unsigned long int histories, Method method = ARNOLDI);

        void arnoldi(HistSource& source, int I, int histories){
            _histories = histories;
            _RelaxHistories = histories;

            vector<double> PDF = source.getPDF();
            dSDV q(PDF.size());
            vector<double>::iterator SIter = PDF.begin();
            double* qIter = q.values();
            for( ; SIter != PDF.end(); ++ SIter, ++qIter ){
                *qIter = *SIter;
            }

            Arnoldi::arnoldi(q, I);
        }

        ~ArnoldiMC() {  }

        void RealMeanValues(int index, vector<double>& Mean,
                vector<double>& StdDev);
        void ImagMeanValues(int index, vector<double>& Mean,
                vector<double>& StdDev);
        void RealMeanVectors(int index, vector<double>& Mean,
                vector<double>& StdDev);
        void ImagMeanVectors(int index, vector<double>& Mean, 
                vector<double>& StdDev);

        // Get attribute methods
        vector<vector<complex<double> > > CycleValues() const { 
            return _CycleValues; }
        vector<vector<valarray<complex<double> > > > CycleVectors() const {
            return _CycleVectors; }
        vector<valarray<complex<double> > > MeanVectors() const {
            return _MeanVectors; }
        vector<valarray<complex<double> > > StdDevVectors() const{
            return _StdDevVectors; }
        /*Sum of histories tracked at each Restart*/
        vector<double> HperR() const { return _HperR; }

//      vector<complex<double> > MeanValues(int index){
//          return _MeanValues[index];
//      }
//      vector<complex<double> > StdDevValues(int index){
//          return _StdDevValues[index];
//      }


        /*MeanValues returns the mean value of the eigenvalue estimate. 
         * Includes the estimates from the inactive restarts as well as the 
         * mean of the active restarts.
        index: Which eigenvalue desired.*/
        vector<double> MeanValues(unsigned int index){
            vector<double> tmp(_InactiveValues);
            tmp.reserve( _HperR.size() );

            vector<double> tmpMean; vector<double> tmpSD;
            RealMeanValues(index, tmpMean, tmpSD);
            tmp.insert( tmp.end(), tmpMean.begin(), tmpMean.end() );
            return tmp;
        }
        vector<double> StdDevValues(unsigned int index){
            vector<double> tmp(_InactiveValues.size(), 0.0);
            tmp.reserve( _HperR.size() );

            vector<double> tmpMean; vector<double> tmpSD;
            RealMeanValues(index, tmpMean, tmpSD);
            tmp.insert( tmp.end(), tmpSD.begin(), tmpSD.end() );

            return tmp;
         }

        friend class ArnoldiMCTest;

    private:
        /*score will add nuSigmaF/SigmaT to the zone where the Particle lives.
         *
         * source: HistSource where Particle scores
         * P: Particle being tracked*/
        void ArnoldiMC::_score(HistSource& source, Particle& P){
            const Material& mat  = _field[*(P.zone())];
            const Zone* Z = source.getZone(P.x());
            source[*Z] += P.weight()*(mat.nu()*mat.xF()/mat.xT());
        }

        /* Storage for statistical computations
         * The ith element of _CycleValues contains the restart estimates of
         * Ritzvalue 'i'.  Similarly for _MeanValues and _StdDevValues, except
         * it contains the mean and std. dev. of the estimates.
         * The ith element of _CycleVectors  contains the Ritzvector estimates
         * for restart 'i'.  Similarly for _MeanVectors and _StdDevVectors.*/
        vector<vector<complex<double> > > _CycleValues;
        vector<double> _InactiveValues;
        vector<vector<valarray<complex<double> > > > _CycleVectors;
        vector<vector<complex<double> > > _MeanValues;
        vector<vector<complex<double> > > _StdDevValues;
        vector<valarray<complex<double> > > _MeanVectors;
        vector<valarray<complex<double> > > _StdDevVectors;

        vector<double> _HperR;    // Sum of histories tracked at each Restart

        // Methods for statistical computations
        void _ValueStats();
        void _VectorStats();
        
        unsigned long int _histories;             // Number of initial histories
        unsigned long int _IterationHistories;    // Number of histories tracked 
        unsigned long int _RelaxHistories;    // Number of histories to run for relaxation
        Mesh& _ScoringMesh;
        double _relaxed;              // Running relaxed Arnoldi?

        bool _active;
        unsigned int _InactiveHistories;

        // Methods for inactive iterations
        void InactiveARNOLDI(HistSource&, int, int, int);
        void InactivePOWER(HistSource&, int, int);

        // Methods for Arnoldi's method
        void _Iteration();
        void _Restart(int);
        cSDV _RestartVector();
        cSDV _Aq();
        cSDV _Scale(HistSource&, HistSource&);
        void _FixFirstHarmonic();
};

#endif

