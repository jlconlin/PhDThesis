/*BankSource.h
 $Author: jlconlin $
 $Id: BankSource.h 239 2008-01-07 17:47:47Z jlconlin $
 $Revision: 239 $
 $Date: 2008-01-07 10:47:47 -0700 (Mon, 07 Jan 2008) $

 This file contains header information for the BankSource class.
 The BankSource class is simply a vector of Particles with some extra 
 functionality.

 */
#ifndef BANKSOURCE_H
#define BANKSOURCE_H

#include<vector>
#include "Field.hh"
#include "FissionSource.h"
#include "Particle.h"

//using std::vector;

class BankSource : public FissionSource {
    
    public:
        
        BankSource(const std::vector<unsigned long>& seed)
            : FissionSource(seed) { } 

        BankSource(const std::vector<unsigned long>& seed, int N)
            : FissionSource(seed)
        { _bank.reserve(N); }

        BankSource(Field<Zone, double>& F, const Mesh& M, 
                   const std::vector<unsigned long>& seed, int N);

        //Copy Constructor
        // Not really a copy constructor, but need this for use with Markov
        BankSource(const BankSource& source)
            : FissionSource(source.seed())  {   }

        ~BankSource();  //Destructor

        //Gettr functions
        std::vector<Particle*> bank() const { return _bank;}
        int size() const { return _bank.size(); }

        //Given a Mesh, return a Field representing the discretized source
        std::vector<double> discretized(Mesh& M) const;
    
        //Member functions
        void sample( Particle& P);
        void push_back(Particle* P);

        void PrintBank( int N);
    private:
        std::vector<Particle*> _bank;
    
        BankSource& operator=(const BankSource&);
};

#endif
