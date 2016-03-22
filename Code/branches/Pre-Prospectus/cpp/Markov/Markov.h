/*Markov.h
 $Author: jlconlin $
 $Id: Markov.h 239 2008-01-07 17:47:47Z jlconlin $
 $Revision: 239 $
 $Date: 2008-01-07 10:47:47 -0700 (Mon, 07 Jan 2008) $

 This file contains header information for the Markov class.
 The Markov class handles all of the details for the actual Monte Carlo 
 particle tracking.

 */
#ifndef MARKOV_H
#define MARKOV_H
#include<vector>
#include<algorithm>
#include<cmath>
#include "Particle.h"
#include "Field.hh"
#include "MonteCarloExcept.h"
#include "OneDZone.hh"
#include "BankSource.h"

class HistSource;
template<typename S> class Markov;

#include "HistSource.h"


template<typename S>    //S is a FissionSource type
class Markov{
    public:
        Markov(const std::vector<unsigned long>& seed,
               const Field<Zone, Material>& field);

        //virtual ~Markov() { }
    
        /*score is where a Particle will score at each collision.  The function
        requires a FissionSource object to score into.*/
        virtual void score(S& source, Particle& P) = 0;

        S* Markov_Transport(S* const source, int N);

    protected:
        static unsigned long _total;
        unsigned long _ID;

        RandomLib::Random _r;   //Random number generator
        std::vector<unsigned long> _seed; //Seed for random number generator

        static const double _weightCutoff;
        static const double _pKill;    //Probability for killing if low weight

        const Field<Zone, Material> _field; //Where we are tracking particles

        bool _roulette(Particle&);
    private:
        //Prevent Markov copying
        Markov(const Markov&) { };
        Markov& operator=(const Markov&) {  };

        void _track(Particle&, S*);

};

template<typename S> unsigned long Markov<S>::_total = 0;   //Initialize Markov count
template<typename S> const double Markov<S>::_weightCutoff = 0.2;
template<typename S> const double Markov<S>::_pKill = 0.2;

template<typename S>
Markov<S>::Markov(const vector<unsigned long>& seed, 
    const Field<Zone, Material> & field)    : _field(field)
{
//    score = &in_score;
    _total++;
    _ID = _total;
      
    //Seed random number generator with unique seed
    _seed = seed;
    _seed.push_back(_ID);
    _r.Reseed(_seed);
}

/* transport takes a FissionSource object S, samples particles from it and 
 * transports those particles, creating a new FissionSource object which it 
 * returns.  This is the workhorse function for this class.
 * 
 * source: FissionSource object that is sampled to provide particles for tracking
 * N: the number of particles to sample and track.*/
template<>
BankSource* Markov<BankSource>::Markov_Transport(BankSource* const source, int N){

    const Zone* tmpZone = new Zone(1.0, 1.0, 1);
    Particle P(_seed, tmpZone);
    delete tmpZone;
    BankSource* newSource = new BankSource(*source);
    for( int i = 0; i < N; ++i ){
        source->sample(P);    //Sample new source
        _track(P, newSource);   //Track particle
    }    
    
    return newSource;
}

template<>
HistSource* Markov<HistSource>::Markov_Transport(HistSource* const source, int N){
    source->makeDF();
    const Zone* tmpZone = new Zone(1.0, 1.0, 1);
    Particle P(_seed, tmpZone);
    delete tmpZone;
    HistSource* newSource = new HistSource(*source);
    for( int i = 0; i < N; ++i ){
        source->sample(P);    //Sample new source
        _track(P, newSource);   //Track particle
    }    
    
    return newSource;
}


template<typename S>
void Markov<S>::_track(Particle& history, S* source){
    while( true ){
        try{ history.collision(_field); }
        catch(NoZoneConnected) { break; }

        this->score(*source, history);
        const Material& mat = _field[*(history.zone())];
        history.setWeight(history.weight()*(mat.xS()/mat.xT()));

        //Make sure we are not tracking low weight Particles
        if ( abs(history.weight()) < _weightCutoff ){
            if( _roulette(history) ) break;
//            try{ _roulette(history); }
//            catch(LowWeightParticle) { break; }
        }
    }
}

/* _roulette determines if a Particle is killed.  It returns true if the Particle
  should be killed, and false if the Particle survives.*/
template<typename S>
bool Markov<S>::_roulette(Particle& P){
    if( _r.Fixed() < _pKill ) { return true;
//        throw(LowWeightParticle(P, __LINE__, __FILE__)); 
    }
    else { P.setWeight(P.weight()/(1-_pKill)); return false;}

}


#endif
