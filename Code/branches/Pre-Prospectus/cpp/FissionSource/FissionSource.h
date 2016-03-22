/*FissionSource.h
 $Author: jlconlin $
 $Id: FissionSource.h 238 2008-01-05 22:05:09Z jlconlin $
 $Revision: 238 $
 $Date: 2008-01-05 15:05:09 -0700 (Sat, 05 Jan 2008) $

 This file contains header information for the FissionSource class.
 The FissionSource class is a base class defining methods for all fission 
 sources.

 */
#ifndef FISSIONSOURCE_H
#define FISSIONSOURCE_H

#include<vector>

#include "Particle.h"

using std::vector;
class FissionSource {

    public:
        const vector<unsigned long> seed() const { return _r->Seed(); }
    
        virtual ~FissionSource() { delete _r; }
    
        /*sample is a required method and it returns a vector of Particles of
         * length N.*/
        virtual void sample( Particle& P ) = 0;

        virtual int size() const = 0;
    protected:
        static unsigned long _total; //Count of FissionSources
        unsigned long _ID;    //Unique ID of FissionSource

        RandomLib::Random* _r;   //Random number generator

        FissionSource(const vector<unsigned long> seed)
            : _ID(_total++)
        {
            //Seed random number generator with unique seed
            vector<unsigned long> local_seed(seed);
            local_seed.push_back(_ID);
            _r = new RandomLib::Random(local_seed);
        }
        

    private:
        //Prevent FissionSource copying
        FissionSource(const FissionSource&) { };  
        FissionSource& operator=(const FissionSource&);
};

#endif 

