/*Particle.h
 $Author: jlconlin $
 $Id: Particle.h 545 2009-05-14 21:08:28Z jlconlin $
 $Revision: 545 $
 $Date: 2009-05-14 15:08:28 -0600 (Thu, 14 May 2009) $

This file provides the C++ interface for the Particle class.
The Particle represents (obviously) a Particle.

 */
#ifndef PARTICLE_H
#define PARTICLE_H

#include<iostream>
#include<cassert>
#include<vector>
#include "boost/numeric/ublas/vector.hpp"
#include "RandomLib/Random.hpp"
#include "CartesianMesh1D.hh"
#include "Field.hh"
#include "Material.h"

using std::vector;

namespace ublas = boost::numeric::ublas;

class Particle {

    public:
        typedef ublas::vector<Material> ublas_vecM_t;

        //Default constructor
        Particle(const vector<unsigned long>& seed, const Zone<OneD>& Z);
        
        //Constructor with default positions specified
        Particle(double, double, double, double, 
                 const vector<unsigned long>&, const Zone<OneD>*);

        //Constructor with default positions and direction specified
        Particle(double, double, double, double, double, double, double,
                const vector<unsigned long>&, const Zone<OneD>*);

        ~Particle();    //Default destructor

        friend class BankSource;
        friend class HistSource;
        friend class ParticleTest;
        friend class LinearSpaceSource;

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
        bool Leaked() const { return _leaked; }
        void Leaked(bool L) { _leaked = L; }
        const Zone<OneD>* zone() const { return _geoZone; }

        unsigned int total() const { return _total; }
        unsigned int ID() const { return _ID; }
        vector<unsigned long> seed() const { return _r->Seed(); }

        //Settr functions
        void Reseed(const vector<unsigned long>& seed) { _r->Reseed(seed); }
        void SetZone(const Zone<OneD>& Z) { _geoZone = &Z; }

        //Methods
        void collision(const Field<Zone<OneD>, Material, ublas_vecM_t>&);
        void setRandomDirection();    //Set random direction of Particle


    private:
        static double _PI;
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
        bool _leaked;   // Whether particle has left geometry

//        vector<unsigned long> _seed;
        RandomLib::Random* _r;   //Random number generator
        const Zone<OneD>* _geoZone;    //Geometric zone particle is in

        void _move(const Field<Zone<OneD>, Material, ublas_vecM_t>&);
        void _move1D(const Field<Zone<OneD>, Material, ublas_vecM_t>&);
        void _changeWeight();       //Change weight of Particle
        double _distanceToCollision(double);
        double _findIntersection(const Node<OneD>*);

};

inline std::ostream& operator<<(std::ostream& os, const Particle& P){
    os << "{pos: (" << P.x() << ", " << P.y() << ", " << P.z() << ") "
        << "dir: (" << P.u() << ", " << P.v() << ", " << P.w() << ") "
        << "w = " << P.weight() << "; ID = " << P.ID() << "}";
    return os;
}
#endif
