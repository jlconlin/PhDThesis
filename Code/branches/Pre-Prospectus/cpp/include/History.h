//History.h
#ifndef HISTORY_H
#define HISTORY_H

#include "Particle.h"
#include "Point.h"
#include "mcnp_random.h"

class History {
  private:
    //Required for constructor
    //double mSigmaT;
    Particle mNeutron;
    RNG mRandom;

  public:
    //Constructors
    History(Particle);
    History(double, double, double);

    //Get properties of the History
    Point Position() const { return mNeutron.Position(); }

    int Absorption(double, double, double, double, double);    //Collide with given cross sections

  
};

#endif
