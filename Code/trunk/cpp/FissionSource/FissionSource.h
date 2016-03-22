/*FissionSource.h

 This file contains header information for the FissionSource class.
 The FissionSource class is a base class defining methods for all fission 
 sources.

*/
#ifndef FISSIONSOURCE_H
#define FISSIONSOURCE_H

#include<vector>
#include "RandomLib/Random.hpp"
#include "Particle.h"

class FissionSource {

    public:
        const std::vector<unsigned long> seed() const { return _r->Seed(); }
    
        virtual ~FissionSource() { delete _r; }
    
        virtual void sample( Particle& P ) = 0;

        /*magnitude gives some type of information regarding how big the 
         * FissionSource object is.*/
        virtual double magnitude() const = 0;

        /*The score function will handle everything when scoring in a 
         * FissionSource.  
         * P: Particle doing the scoring
         * s: What value to score.  How this is used will vary betwen types of
         *      FissionSources.*/
        virtual void score( const Particle& P, double s) = 0;

        virtual void PostScoring( int ) = 0;

    protected:
        static unsigned long _total; //Count of FissionSources
        unsigned long _ID;    //Unique ID of FissionSource

        RandomLib::Random* _r;   //Random number generator

        FissionSource(const std::vector<unsigned long> seed)
            : _ID(_total++)
        {
            //Seed random number generator with unique seed
            std::vector<unsigned long> local_seed(seed);
            local_seed.push_back(_ID);
            _r = new RandomLib::Random(local_seed);
        }
        

    private:
        //Prevent FissionSource copying
        FissionSource(const FissionSource&) { };  
        FissionSource& operator=(const FissionSource&);
};

#endif 

