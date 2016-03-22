/*HistSource.h

 This file contains header information for the HistSource class.
 A HistSource object will be the default FissionSource object for Arnoldi's
 method.

*/
#ifndef HISTSOURCE_H
#define HISTSOURCE_H

#include <iostream>
#include <vector>
#include <numeric>
#include <cmath>
#include "boost/numeric/ublas/vector.hpp"
#include "Field.hh"
#include "Field_AddOns.hh"
#include "CartesianMesh1D.hh"
#include "FissionSource.h"
#include "Particle.h"
#include "Utilities.h"

using std::vector;
using std::accumulate;

class HistSource : FissionSource {
    public:
        typedef ublas::vector<double, std::vector<double> > ublas_vec_t;
        typedef Field<Zone<OneD>, double, ublas_vec_t > field_t;
          
        // Constructor required for Markov class
        HistSource( const CartesianMesh<OneD>& M, 
            const vector<unsigned long>& seed)
            : _field(M, 0.0), FissionSource(seed),
              _CDF(M.numZones()) {  }

        HistSource( const HistSource&  source)
            : _field(source._field), FissionSource(source.seed()),
              _CDF(source._field.size()) {  }

        /*seed: Random number generator seed
        M: Mesh describing geometry of source
        Probs: Probability of picking from each zone in Mesh*/
        HistSource(const vector<unsigned long>& seed, 
            const CartesianMesh<OneD>& M, ublas_vec_t& Probs)
            : _field(M, Probs), 
            FissionSource(seed), 
            _CDF(M.numZones())
        {  }

        /*seed: Random number generator seed
        F: field_t describing sampling probabilities*/
        HistSource(const vector<unsigned long>& seed, field_t F)
            : _field(F), 
            FissionSource(seed),
            _CDF(F.size())
        {  }

        ~HistSource() { }

        /*This function will return the PDF of the HistSource.  It should be 
         used carefully because the PDF my change after retrieving the data.
         */
        ublas_vec_t PDF() const { return _field.mData; }
        ublas_vec_t CDF() const { return _CDF; }

        double Entropy() { return ShannonEntropy(_field.mData); }

        ublas_vec_t Edges() const;
        ublas_vec_t Centers() const;

        double& operator[](const Zone<OneD>& Z) { return _field[Z]; }
        const Zone<OneD>& getZone(double x) const{ return _field.getZone(x); }
    
        /*size will return the number of bins in the HistSource.*/
        unsigned int size() const { return _field.size(); }

        void sample( Particle& );
        double magnitude() const;
        void score( const Particle&, double );
        void PostScoring( int ) {    }

        void makeCDF(); // Make CDF

    private:
        ublas_vec_t _CDF;
        Field< Zone<OneD>, double, ublas_vec_t > _field;   


        HistSource& operator=( const HistSource& );
};

#endif

