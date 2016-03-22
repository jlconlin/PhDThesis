/*test_Particle.cpp
 $Author: jlconlin $
 $Id: test_forLeaks.cpp 236 2008-01-03 21:14:32Z jlconlin $
 $Revision: 236 $
 $Date: 2008-01-03 14:14:32 -0700 (Thu, 03 Jan 2008) $

 This file is used for testing the Particle class.
 It will test the class by timing different ways to make a vector<Particle>.

 */

#include<iostream>
#include<vector>
#include<deque>
#include<algorithm>
#include<cstdlib>
#include<ctime>

#include "RandomLib/Random.hpp"
#include "OneDCartMesh.hh"
#include "Field.hh"
#include "Assert.hh"

#include "Particle.h"
#include "Material.h"

using std::cout;
using std::vector;
using std::deque;
using std::for_each;
using std::rand;

void TimingInfo(int, clock_t, clock_t);
void vecInstance(int, Particle);
void vecPoint(int, Particle);

int main(){
    int N = 1E5;    //Number of Particles
    //Preliminaries
    Mesh meshU(20, 1.0, 1);     //Slab geometry, one zone, 20 units wide
    Material mat(0.5, 0.5, 0.0, 1.0);   //'Standard' material
    Field<Zone, Material> geometry(meshU, mat);
    vector<unsigned long> seed(2);
    seed[0] = RandomLib::Random::SeedWord();
    std::cout << "Master seed: " << seed[0] << "\n";
    const Zone::Id id(0);
    const Zone& z = meshU.getZone(id);
    Particle stdParticle(0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, seed, &z);
    Particle* pParticle = new Particle(0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, seed, &z);
    delete pParticle;

    clock_t start;
    clock_t end;

    //vector<Particle>
    start = clock(); vecInstance(N, stdParticle);
    end = clock(); TimingInfo(N, start, end);
    
    //vector<Particle*>
    start = clock(); vecPoint(N, stdParticle);
    end = clock(); TimingInfo(N, start, end);
    
    //Work for awhile so I can check for memory leaks.
    start = clock(); for( int i = 0; i < 1E9; i++ ); 
    end = clock(); TimingInfo(N, start, end);

    return 0;
}



void TimingInfo(int N, clock_t start, clock_t end){
    double diff = double(end - start)/CLOCKS_PER_SEC;
    cout << "Elapsed time: " << diff << " sec.\n"
        << "Time per particle: " << diff/N << " sec/particle\n";
}

void vecInstance(int N, Particle stdParticle){
    cout << "\nI am creating a vector of particles.\n";
    vector<Particle> vecParticle;
    for(int i = 0; i < N; i++){
        vecParticle.push_back(Particle(stdParticle));
    }
}

void vecPoint(int N, Particle stdParticle){
    cout << "\nI am creating a vector of pointers to particles.\n";
    vector<Particle*> vecParticle;
    for(int i = 0; i < N; i++){
        vecParticle.push_back(new Particle(stdParticle));
    }
    for( int i = 0; i < N; i++){
        delete vecParticle[i];
    }
}