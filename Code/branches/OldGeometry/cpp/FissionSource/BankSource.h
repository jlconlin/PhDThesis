/*BankSource.h
 $Author: jlconlin $
 $Id: BankSource.h 298 2008-05-12 17:44:56Z jlconlin $
 $Revision: 298 $
 $Date: 2008-05-12 11:44:56 -0600 (Mon, 12 May 2008) $

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
            : FissionSource(seed), _bank(), _size(0) { } 

        BankSource(const std::vector<unsigned long>& seed, int N)
            : FissionSource(seed), _bank(), _size(0)
        { _bank.reserve(N); }

        BankSource(Field<Zone, double>& F, const Mesh& M, 
                   const std::vector<unsigned long>& seed, int N);

        //Copy Constructor
        // Not really a copy constructor, but need this for use with Markov
        BankSource(const BankSource& source)
            : FissionSource(source.seed())  {   }

        ~BankSource();  //Destructor

        //Gettr functions
        std::vector<vector<double> > bank() const { return _bank;}
        double size() const { return _size; }

        //Given a Mesh, return a Field representing the discretized source
        std::vector<double> discretized(Mesh& M) const;
    
        //Member functions
        void sample( Particle& P);
        void push_back(Particle* P);

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
