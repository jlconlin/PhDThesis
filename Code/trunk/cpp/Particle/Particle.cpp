/*Particle.cpp
 $Author: jlconlin $
 $Id: Particle.cpp 480 2009-03-10 16:56:40Z jlconlin $
 $Revision: 480 $
 $Date: 2009-03-10 10:56:40 -0600 (Tue, 10 Mar 2009) $

This file provides the implementation for the Particle class.
The Particle represents (obviously) a Particle.

 */

#include<iostream>
#include<cassert>
#include<cmath>
#include<utility>
#include<vector>
#include "boost/tuple/tuple.hpp"
#include "RandomLib/Random.hpp"
#include "CartesianMesh1D.hh"
#include "Zone1D.hh"
#include "Field.hh"
#include "Particle.h"
#include "Material.h"

using std::vector;

int Particle::_total = 0;  //Initialize variable
double Particle::_PI = 3.14159265358979323;

Particle::Particle(const vector<unsigned long>& seed, const Zone<OneD>& zone)
    :  _ID(_total++), _geoZone(&zone), _leaked(false)
{
    _x = 0;
    _y = 0;
    _z = 0;
    _weight = 1.0;

    //Seed random number generator with unique seed
    vector<unsigned long> local_seed(seed);
    local_seed.push_back(_ID);
    _r = new RandomLib::Random(local_seed);
    setRandomDirection();
}

Particle::Particle(double x, double y, double z, double weight,
    const vector<unsigned long>& seed, const Zone<OneD>* zone) 
    : _ID(_total++), _x(x), _y(y), _z(z), _weight(weight),
      _geoZone(zone), _leaked(false)
{
    //Seed random number generator with unique seed
    vector<unsigned long> local_seed(seed);
    local_seed.push_back(_ID);
    _r = new RandomLib::Random(local_seed);
    setRandomDirection();
}

Particle::Particle(double x, double y, double z, double u, double v, double w,
    double weight, const vector<unsigned long>& seed, const Zone<OneD>* zone) 
    : _ID(_total++), _x(x), _y(y), _z(z), _u(u), _v(v), _w(w),
      _weight(weight), _geoZone(zone), _leaked(false)
{
    //Seed random number generator with unique seed
    vector<unsigned long> local_seed(seed);
    local_seed.push_back(_ID);
    _r = new RandomLib::Random(local_seed);
}


//Copy constructor
Particle::Particle(const Particle& P)
    : _ID(_total++),
      _x(P.x()), _y(P.y()), _z(P.z()), 
      _u(P.u()), _v(P.v()), _w(P.w()),
      _weight(P.weight()), _geoZone(P.zone()), _leaked(false) 
{
    //Seed random number generator with unique seed
    vector<unsigned long> local_seed(P.seed());
    local_seed.back() = _ID;
    _r = new RandomLib::Random(local_seed);    
}

Particle& Particle::operator= (const Particle& P)
{
    _x = P._x;
    _y = P._y;
    _z = P._z;

    _u = P._u;
    _v = P._v;
    _w = P._w;

    _weight = P._weight;
    _geoZone = P._geoZone;

    _total++;    //Increment particle count
    _ID = _total;    //Give unique ID

    //Seed random number generator with unique seed
    vector<unsigned long> local_seed(P.seed());
    local_seed.back() = _ID;

    _r = new RandomLib::Random(local_seed);

    return *this;
}

//Default destructor
Particle::~Particle(){
    delete _r;
}

/**/
void Particle::collision(const Field<Zone<OneD>, Material, ublas_vecM_t>& field){
    _move(field);
    if( not _leaked ) { setRandomDirection(); }
}

/*move will move the Particle to a new random position.  The distance the 
Particle will travel is sampled as a function of the mean free path of the 
in the material (1/xT).

Field&: Reference to geometry in which the particle moves.*/
void Particle::_move(const Field<Zone<OneD>, Material, ublas_vecM_t>& F){
    const Material& mat = F[*_geoZone];
    double tmpX, tmpY, tmpZ;
    double d;
    do {
        d = _distanceToCollision(mat.xT());
        //Temporarily move Particle
        tmpX = _x + d*_u;
        tmpY = _y + d*_v;
        tmpZ = _z + d*_w;

        //Detect if particle has left Zone
        typedef const std::pair<const Node1D*, const Node1D*> stdPair;
        stdPair zoneNodes = _geoZone->getNodes();
        if( tmpX < zoneNodes.first->x() ){
            //Move to boundary
            _x = _findIntersection(zoneNodes.first);
            //Has it left the Field?
            _leaked = zoneNodes.first->onLeftBoundary();
            if( _leaked ) return;
            else _geoZone = &(zoneNodes.first->getLeftZone());
            
        }
        else if( tmpX > zoneNodes.second->x() ){
            //Move to boundary
            _x = _findIntersection(zoneNodes.second);
            //Has it left the Field?
            _leaked = zoneNodes.second->onRightBoundary();
            if( _leaked ) return;
            else _geoZone = &(zoneNodes.second->getRightZone());
        }
        else {  //Particle remains in zone
            _x = tmpX;
            _y = tmpY;
            _z = tmpZ;

            return;
        }
    
    } while(true); //Only end when particle stays in zone or leaves mesh.
        //Resample distance to collision
}

/*move1D is the same as move, but it only moves the particle in 1 dimension.*/
void Particle::_move1D(const Field<Zone<OneD>, Material, ublas_vecM_t>& F){
    _move(F);
}

void Particle::setRandomDirection(){
    double phi = 2*_PI*_r->Fixed();
    _u = 2*_r->Fixed() - 1;
    double mu = std::sqrt(1 - _u*_u);
    _v = mu*std::cos(phi);
    _w = mu*std::sin(phi);
}

double Particle::_distanceToCollision(double xT){
    //Sample distance to collision
    return (1.0/xT)*std::log(_r->Fixed());
}

/*findIntersection will determine the intersection of the particle with 
the Node N.*/
double Particle::_findIntersection(const Node<OneD>* N){
    // In 1-D this is trivial.
    return N->x();
}
