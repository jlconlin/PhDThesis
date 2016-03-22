/*BankSource.cpp
 $Author: jlconlin $
 $Id: BankSource.cpp 238 2008-01-05 22:05:09Z jlconlin $
 $Revision: 238 $
 $Date: 2008-01-05 15:05:09 -0700 (Sat, 05 Jan 2008) $

This file provides the implementation for the BankSource class.

 */

#include<iostream>
#include<vector>
#include<cassert>
#include "boost/utility.hpp"
#include "BankSource.h"
#include "OneDCartMesh.hh"
#include "Particle.h"
#include "OneDZone.hh"


using std::vector;
using std::cout;
using boost::compressed_pair;


/*This constructor will probably be the one most used.  The Field object input
 * is the relative probability of picking a particle from that Zone.
 * WARNING: Assumption is made that the Field passed in, is the same Field
 * that is used for the geometry of the problem.
 * 
 * N: The number of particles to sample to make initial source.*/
BankSource::BankSource(Field<Zone, double>& F, const Mesh& M,
        const vector<unsigned long>& seed, int N)
    : FissionSource(seed)
{
    //Make pdf
    int size = F.size();
    vector<double> PDF(size, 0.0);
    vector<double> CDF(size, 0.0);
    double totalProbability(0);
    totalProbability = std::accumulate(F.begin(), F.end(), totalProbability);
    Field<Zone, double>::iterator fIter = F.begin();
    vector<double>::iterator vecIter = PDF.begin();
    for( vecIter = PDF.begin(); vecIter != PDF.end(); ++vecIter, ++fIter ){
        *vecIter = *fIter/totalProbability;
    }
    CDF[0] = PDF[0];
    for ( int i = 1; i < size; i++ ){
        CDF[i] = CDF[i-1] + PDF[i];
    }

    //Sample
    _bank.reserve(N);  
    double probPDF;     //Probability from PDF
    compressed_pair<const Node*, const Node*> zN;      //Nodes around a Zone
    Mesh::const_ZoneIterator zMIter;
    double x, max, min;
    Particle* P;
    const Zone* Z = 0;
    for( int i = 0; i < N; i++ ){   // Sample N particles
        probPDF = _r->Fixed();
        vecIter = CDF.begin(); zMIter = M.zoneBegin();  //Get iterators ready
        //Find the zone where particle should be sampled from
        for( ; vecIter != CDF.end(); ++vecIter, ++zMIter ){
            if( probPDF <= *vecIter ) { Z = &(*zMIter); break; }    //Ugly!
        }
        zN = Z->getNodes();
        max = zN.second()->x(); min = zN.first()->x();
        x = (max - min)*_r->Fixed() + min;      //Pick a position
        Z = M.getZone(x);
        P = new Particle(x, 0, 0, 1.0, seed, Z);
        _bank.push_back(P);
    }
}

BankSource::~BankSource(){
    for( vector<Particle*>::iterator iter=_bank.begin(); iter!=_bank.end(); 
            ++iter )
    { delete *iter; }
}

/* sample will change the properties of Particle P.  It samples a Particle
   from _bank and makes P equal to it.  It will randomize the direction of
   the particle.There is no guarantee of sampling a particle multiple times.*/
void BankSource::sample( Particle& P ){
    assert(_bank.size() > 0);
    
    int size = _bank.size();
    
    int index = _r->IntegerC(0,size-1);  //Pick a particle
    Particle* oldParticle;
    oldParticle = _bank[index];
    P._x = oldParticle->_x;
    P._y = oldParticle->_y;
    P._z = oldParticle->_z;
    P.setRandomDirection();
    P._geoZone = oldParticle->_geoZone;
    if( oldParticle->_weight < 0 ) { P._weight = -1.0; }
    else { P._weight = 1.0; }
}

void BankSource::push_back(Particle* P){
    P->Reseed(_r->Seed());
    _bank.push_back(P);
}

//Given a Mesh, return a Field representing the discretized source
std::vector<double> BankSource::discretized(Mesh& M) const{
    std::vector<double> histogram(M.numZones(), 0.0);
    Field<Zone, double> F(M, 0.0);
    
    const Zone* Z;
    for( vector<Particle*>::const_iterator iter = _bank.begin(); iter != _bank.end();
        ++iter ){
        Z = M.getZone((*iter)->x());
        F[*Z] += (*iter)->weight();
    }
    
    Field<Zone, double>::iterator fieldIter = F.begin();
    vector<double>::iterator histIter = histogram.begin();
    for( ; histIter != histogram.end(); ++histIter, ++fieldIter ){
        *histIter = *fieldIter;
    }
    return histogram;
}

/*PrintBank will write to the screen the first N Particles.*/
void BankSource::PrintBank(int N){
    int n = 0;
	for ( unsigned int i = 0; i < _bank.size(); i++ ){
		cout << "(" << i << "): " << _bank[i] << "\n";
        if ( n++ > N) break;
	}
}
