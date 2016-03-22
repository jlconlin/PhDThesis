/*HistSource.h
 $Author: jlconlin $
 $Id: HistSource.h 298 2008-05-12 17:44:56Z jlconlin $
 $Revision: 298 $
 $Date: 2008-05-12 11:44:56 -0600 (Mon, 12 May 2008) $

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
        HistSource( const Mesh& M , const vector<unsigned long>& seed)
            : Field_Zd(M, 0.0), FissionSource(seed),
              _CDF(mData.size(), 0.0) {  }

        HistSource( const HistSource&  source)
            : Field_Zd(source.mMesh, 0.0), FissionSource(source.seed()),
              _CDF(mData.size(), 0.0) {  }

        /*seed: Random number generator seed
        M: Mesh describing geometry of source
        Probs: Probability of picking from each zone in Mesh*/
        HistSource(const vector<unsigned long>& seed, const Mesh& M,
                    vector<double> Probs)
            : Field_Zd(M, Probs), FissionSource(seed), _CDF(mData.size(), 0.0)
        {  }

        /*seed: Random number generator seed
        F: Field_Zd describing sampling probabilities*/
        HistSource(const vector<unsigned long>& seed, Field_Zd F)
            : Field_Zd(F), FissionSource(seed), _CDF(mData.size(), 0.0)
        {  }

        ~HistSource() { }

        /*This function will return the PDF of the HistSource.  It should be 
         used carefully because the PDF my change after retrieving the data.
         */
        vector<double> getPDF() const { return mData; }
        vector<double> getCDF() const { return _CDF; }
    
        void sample( Particle& P );
        double size() const;
        void makeCDF(); // Make CDF

    private:
        vector<double> _CDF;


        HistSource& operator=( const HistSource& );
};

#endif

