/*Markov.h

 This file contains header information for the Markov class.
 The Markov class handles all of the details for the actual Monte Carlo 
 particle tracking.

*/

#ifndef MARKOV_H
#define MARKOV_H
#include<iostream>
#include<vector>
#include<algorithm>
#include<cmath>
#include "Particle.h"
#include "Field.hh"
#include "CartesianMesh1D.hh"
#include "BankSource.h"
#include "Material.h"

class HistSource;
#include "HistSource.h"

class LinearSpaceSource;
#include "LinearSpaceSource.h"

using std::cout;
using std::endl;
using std::vector;


template<typename S>    //S is a FissionSource type
class Markov{
    public:
        typedef ublas::vector<double> ublas_vec_t;
        typedef Field<Zone<OneD>, Material, ublas::vector<Material> > field_t;

        Markov(const vector<unsigned long>& seed, const field_t& field);

        virtual ~Markov() { }
    
        void Markov_Transport(S& CurrentSource, S& NewSource, int N);

        const vector<unsigned long> Seed() const { return _r.Seed(); }

    protected:
        /*_score is where a Particle will score at each collision.  The function
        requires a FissionSource object to score into.*/
        virtual void _score(S& source, Particle& P) = 0;

        static unsigned long _total;
        unsigned long _ID;

        RandomLib::Random _r;   //Random number generator
        vector<unsigned long> _seed; //Seed for random number generator

        static const double _weightCutoff;
        static const double _pKill;    //Probability for killing if low weight

        const field_t _field; //Where we are tracking particles

        bool _roulette(Particle&);
        void _preSample(S&);    // Perform some function before sampling
        void _postSample(S&, int);    // Perform some function before sampling
    private:
        //Prevent Markov copying
        Markov(const Markov&);
        Markov& operator=(const Markov&);

        void _track(Particle&, S&);

};

template<typename S> unsigned long Markov<S>::_total = 0;   
template<typename S> const double Markov<S>::_weightCutoff = 0.2;
template<typename S> const double Markov<S>::_pKill = 0.2;

template<typename S>
Markov<S>::Markov(const vector<unsigned long>& seed, const field_t& field)
    : _field(field)
{
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
template<typename S>
inline void Markov<S>::Markov_Transport(S& CurrentSource, S& NewSource, int N){
    
    Particle P( _seed, _field.getZone(0.0) );

    _preSample(CurrentSource);
    for( int i = 0; i < N; ++i ){
        CurrentSource.sample(P);    //Sample new source
        P.SetZone(_field.getZone(P.x()));
        P.Leaked(false);
        _track(P, NewSource);   //Track particle
    }        
    _postSample(NewSource, N);
}

template<typename S>
inline void Markov<S>::_preSample(S& s){    /*Do nothing (as of yet).*/ }

template<>
inline void Markov<HistSource>::_preSample(HistSource& S){
    S.makeCDF();
}

template<>
inline void Markov<LinearSpaceSource>::_preSample(LinearSpaceSource& LSS){
    LSS.Normalize();
}

template<typename S>
inline void Markov<S>::_postSample(S& s, int N){   /*Do nothing (as of yet).*/ }

template<>
inline void Markov<LinearSpaceSource>::_postSample(LinearSpaceSource& LSS,
        int N){
    LSS.PostScoring( N );
}

template<typename S>
void Markov<S>::_track(Particle& history, S& source){
    while( true ){
        history.collision(_field);
        if( history.Leaked() ) break;

        _score(source, history);
        const Material& mat = _field[*(history.zone())];
        history.setWeight(history.weight()*(mat.xS()/mat.xT()));

        //Make sure we are not tracking low weight Particles
        if ( abs(history.weight()) < _weightCutoff ){
            if( _roulette(history) ) break; }
    }
}

/* _roulette determines if a Particle is killed.  It returns true if the Particle
  should be killed, and false if the Particle survives.*/
template<typename S>
bool Markov<S>::_roulette(Particle& P){
    if( _r.Fixed() < _pKill ) { return true; }
    else { P.setWeight(P.weight()/(1-_pKill)); return false; }

}



#endif
