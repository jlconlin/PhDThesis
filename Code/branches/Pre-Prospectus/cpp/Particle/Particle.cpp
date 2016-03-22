/*Particle.cpp
 $Author: jlconlin $
 $Id: Particle.cpp 236 2008-01-03 21:14:32Z jlconlin $
 $Revision: 236 $
 $Date: 2008-01-03 14:14:32 -0700 (Thu, 03 Jan 2008) $

This file provides the implementation for the Particle class.
The Particle represents (obviously) a Particle.

 */

#include<iostream>
#include<cassert>
#include<cmath>
#include<utility>
#include<vector>

#include "RandomLib/Random.hpp"
#include "OneDCartMesh.hh"
#include "OneDZone.hh"
#include "Field.hh"
#include "Particle.h"
#include "Material.h"

using std::vector;
using std::pair;

using boost::compressed_pair;

int Particle::_total = 0;  //Initialize variable

Particle::Particle(const vector<unsigned long>& seed, const Zone* zone)
    :  _ID(_total++), _geoZone(zone)
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
    const vector<unsigned long>& seed, const Zone* zone) 
    : _ID(_total++), _x(x), _y(y), _z(z), _weight(weight),
      _geoZone(zone)
{
    //Seed random number generator with unique seed
    vector<unsigned long> local_seed(seed);
    local_seed.push_back(_ID);
    _r = new RandomLib::Random(local_seed);
    setRandomDirection();
}

Particle::Particle(double x, double y, double z, double u, double v, double w,
    double weight, const vector<unsigned long>& seed, const Zone* zone) 
    : _ID(_total++), _x(x), _y(y), _z(z), _u(u), _v(v), _w(w),
      _weight(weight), _geoZone(zone)
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
      _weight(P.weight()), _geoZone(P.zone()) 
{
    //Seed random number generator with unique seed
    vector<unsigned long> local_seed(P.seed());
    local_seed.back() = _ID;
    _r = new RandomLib::Random(local_seed);    
}

/*//Copy constructor
Particle::Particle(const Particle& P){
    *this = P;
}*/

Particle& Particle::operator= (const Particle& P)
{
    _x = P.x();
    _y = P.y();
    _z = P.z();

    _u = P.u();
    _v = P.v();
    _w = P.w();

    _weight = P.weight();
    _geoZone = P.zone();

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
void Particle::collision(const Field<Zone, Material>& field){
    _move(field);
    setRandomDirection();
}

/*move will move the Particle to a new random position.  The distance the 
Particle will travel is sampled as a function of the mean free path of the 
in the material (1/xT).

Field&: Reference to geometry in which the particle moves.*/
void Particle::_move(const Field<Zone, Material>& F){
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
        const compressed_pair<const Node*, const Node*> zoneNodes = (*_geoZone).getNodes();
        if( tmpX < zoneNodes.first()->x() ){
            //Move to boundary
            _x = _findIntersection(zoneNodes.first());
            //Has it left the Field?
            _geoZone = &(zoneNodes.first()->getLeftZone());
        }
        else if( tmpX > zoneNodes.second()->x() ){
            //Move to boundary
            _x = _findIntersection(zoneNodes.second());
            //Has it left the Field?
            _geoZone = &(zoneNodes.second()->getRightZone());
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
void Particle::_move1D(const Field<Zone, Material>& F){
    _move(F);
}

void Particle::setRandomDirection(){
    double PI = 3.14159265358979323;
    double phi = 2*PI*_r->Fixed();
    _u = 2*_r->Fixed() - 1;
    _v = std::sqrt(1 - _u*_u)*std::cos(phi);
    _w = std::sqrt(1 - _u*_u)*std::sin(phi);
}


//Private methods---------------

double Particle::_distanceToCollision(double xT){
    //Sample distance to collision
    return (1.0/xT)*std::log(_r->Fixed());
}

/*findIntersection will determine the intersection of the particle with 
the Node N.*/
double Particle::_findIntersection(const Node* N){
    // In 1-D this is trivial.
    return N->x();
}
