/*Particle.h
 $Author: jlconlin $
 $Id: Particle.h 238 2008-01-05 22:05:09Z jlconlin $
 $Revision: 238 $
 $Date: 2008-01-05 15:05:09 -0700 (Sat, 05 Jan 2008) $

This file provides the C++ interface for the Particle class.
The Particle represents (obviously) a Particle.

 */
#ifndef PARTICLE_H
#define PARTICLE_H

#include<iostream>
#include<cassert>
#include<vector>

#include "RandomLib/Random.hpp"
#include "OneDCartMesh.hh"
#include "Field.hh"
#include "Material.h"


using std::vector;

//typedef Field<Zone, Material> matField;
class Particle {

    public:
        //Default constructor
        Particle(const vector<unsigned long>& seed, const Zone* Z);
        
        //Constructor with default positions specified
        Particle(double, double, double, double, 
                 const vector<unsigned long>&, const Zone*);

        //Constructor with default positions and direction specified
        Particle(double, double, double, double, double, double, double,
                const vector<unsigned long>&, const Zone*);

        ~Particle();    //Default destructor

        friend class BankSource;
        friend class HistSource;

        Particle(const Particle&);  //Copy constructor
        Particle& operator= (const Particle&);

        //Gettr functions
        double x() const { return _x; }
        double y() const { return _y; }
        double z() const { return _z; }

        double u() const { return _u; }
        double v() const { return _v; }
        double w() const { return _w; }

        double weight() const { return _weight;}
        void setWeight(double w) { _weight = w;}
        const Zone* zone() const { return _geoZone; }

        unsigned int total() const { return _total; }
        unsigned int ID() const { return _ID; }
        vector<unsigned long> seed() const { return _r->Seed(); }

        //Settr functions
        void Reseed(const vector<unsigned long>& seed) { _r->Reseed(seed); }

        //Methods
        void collision(const Field<Zone, Material>&);
        void setRandomDirection();    //Set random direction of Particle


    private:
        static int _total; //Count of Particles
        unsigned long _ID;    //Unique ID of Particle

        // Position of Particle
        double _x;
        double _y;
        double _z;
        
        //Direction of Particle
        double _u;
        double _v;
        double _w;

        double _weight; //Weight of Particle

//        vector<unsigned long> _seed;
        RandomLib::Random* _r;   //Random number generator
        const Zone* _geoZone;    //Geometric zone particle is in

        void _move(const Field<Zone, Material>&);
        void _move1D(const Field<Zone, Material>&);
        void _changeWeight();       //Change weight of Particle
        double _distanceToCollision(double);
        double _findIntersection(const Node*);

};

inline std::ostream& operator<<(std::ostream& os, const Particle& P){
    os << "{pos: (" << P.x() << ", " << P.y() << ", " << P.z() << ") "
        << "dir: (" << P.u() << ", " << P.v() << ", " << P.w() << ") "
        << "w = " << P.weight() << "; ID = " << P.ID() << "}";
    return os;
}
#endif
