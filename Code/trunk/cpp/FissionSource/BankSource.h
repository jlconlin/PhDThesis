/*BankSource.h

 This file contains header information for the BankSource class.
 The BankSource class is simply a vector of Particles with some extra 
 functionality.

*/
#ifndef BANKSOURCE_H
#define BANKSOURCE_H

#include<vector>
#include "boost/numeric/ublas/vector.hpp"
#include "Field.hh"
#include "FissionSource.h"
#include "Particle.h"
#include "Utilities.h"

class BankSource : public FissionSource {
    
    public:
        typedef ublas::vector<double, std::vector<double> > ublas_vec_t;
        
        BankSource(const std::vector<unsigned long>& seed)
            : FissionSource(seed), _bank(), _size(0) { } 

        BankSource(const std::vector<unsigned long>& seed, int N)
            : FissionSource(seed), _bank(), _size(0)
        { _bank.reserve(N); }

        BankSource(Field<Zone<OneD>, double, ublas_vec_t>& F, 
            const CartesianMesh<OneD>& M, 
            const std::vector<unsigned long>& seed, int N);

        //Copy Constructor
        // Not really a copy constructor, but need this for use with Markov
        BankSource(const BankSource& source)
            : FissionSource(source.seed())  {   }

        ~BankSource();  //Destructor

        //Gettr functions
        std::vector<vector<double> > bank() const { return _bank;}
        double magnitude() const { return _size; }

        //Given a Mesh, return a Field representing the discretized source
        ublas_vec_t discretized(CartesianMesh<OneD>& M) const;
        // Given a Mesh, return the Shannon entropy of the source
        double Entropy(CartesianMesh<OneD>& M) const{
            ublas_vec_t vec = discretized(M);
            return ShannonEntropy( vec );
        }
    
        //Member functions
        void sample( Particle&);
        void score ( const Particle&, double);
        void PostScoring( int ) {    }
        void push_back(const Particle&);

        /*reset will prepare the source to know that it has no "important"
         * particles.  This is important to avoid reallocating data all the
         * time.*/
        void reset(){ _size = 0; }

        /*reserve enough space in memory*/
        void reserve(unsigned int n) { _bank.reserve(n); }

    private:
        /*vector contains [x,y,z weight] data in that order*/
        std::vector<vector<double> > _bank;
        unsigned int _size;     // Number of "important" particles
    
        BankSource& operator=(const BankSource&);
};

#endif
