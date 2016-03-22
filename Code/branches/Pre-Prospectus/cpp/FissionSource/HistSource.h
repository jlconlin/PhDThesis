/*HistSource.h
 $Author: jlconlin $
 $Id: HistSource.h 239 2008-01-07 17:47:47Z jlconlin $
 $Revision: 239 $
 $Date: 2008-01-07 10:47:47 -0700 (Mon, 07 Jan 2008) $

 This file contains header information for the HistSource class.
 A HistSource object will be the default FissionSource object for Arnoldi's
 method.

 */
#ifndef HISTSOURCE_H
#define HISTSOURCE_H

#include <vector>
#include <numeric>
#include <cmath>
#include "Field.hh"
#include "OneDCartMesh.hh"
#include "FissionSource.h"
#include "Particle.h"

using std::vector;
using std::accumulate;

typedef Field<Zone, double> Field_Zd;   // For simplicity

class HistSource : public Field_Zd, FissionSource {

    public:
        // Constructor required for Markov class
        HistSource( const HistSource&  source)
            : Field_Zd(source.mMesh, 0.0), FissionSource(source.seed()),
              _CDF(mData.size(), 0.0) { makeDF(); }
        // No need to create DFs because they will be zero 

        /*seed: Random number generator seed
        M: Mesh describing geometry of source
        Probs: Probability of picking from each zone in Mesh*/
        HistSource(const std::vector<unsigned long>& seed, Mesh& M,
                    std::vector<double> Probs)
            : Field_Zd(M, Probs), FissionSource(seed), _CDF(mData.size(), 0.0)
        { makeDF(); }

        /*seed: Random number generator seed
        F: Field_Zd describing sampling probabilities*/
        HistSource(const std::vector<unsigned long>& seed, Field_Zd F)
            : Field_Zd(F), FissionSource(seed), _CDF(mData.size(), 0.0)
        { makeDF(); }

        ~HistSource() { }

        void sample( Particle& P );
        int size() const;
        void makeDF(); // Make PDF and CDF

    private:
        vector<double> _CDF;


        HistSource& operator=( const HistSource& );
};

#endif

