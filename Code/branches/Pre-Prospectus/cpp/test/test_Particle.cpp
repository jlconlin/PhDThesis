/*test_Particle.cpp
 $Author: jlconlin $
 $Id: test_Particle.cpp 236 2008-01-03 21:14:32Z jlconlin $
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
void ConstructAndCopy(int, Particle);
void PushBack(int, Particle);
void LengthTest(int, Particle);
void SizeTest(int, Particle);
void Reserve(int, Particle);
void BasicDeque(int, Particle);
void vecPoint(int, Particle);

int main(){
    int N = 1E4;    //Number of Particles
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

    clock_t start;
    clock_t end;

    //Default constructor and copy
    start = clock(); ConstructAndCopy(N, stdParticle);
    end = clock(); TimingInfo(N, start, end);

    //push_back
    start = clock(); PushBack(N, stdParticle);
    end = clock(); TimingInfo(N, start, end);

    //Copy with test for length
    start = clock(); LengthTest(N, stdParticle);
    end = clock(); TimingInfo(N, start, end);
    
//    //Copy with test for size
//    start = clock(); SizeTest(N, stdParticle);
//    end = clock(); TimingInfo(N, start, end);
//    
//    //Reserve
//    start = clock(); Reserve(N, stdParticle);
//    end = clock(); TimingInfo(N, start, end);
//
//    //Basic Deque
//    start = clock(); BasicDeque(N, stdParticle);
//    end = clock(); TimingInfo(N, start, end);
    
    //vector<Particle*>
    start = clock(); vecPoint(N, stdParticle);
    end = clock(); TimingInfo(N, start, end);

    return 0;
}



void TimingInfo(int N, clock_t start, clock_t end){
    double diff = double(end - start)/CLOCKS_PER_SEC;
    cout << "Elapsed time: " << diff << " sec.\n"
        << "Time per particle: " << diff/N << " sec/particle\n";
}

void ConstructAndCopy(int N, Particle stdParticle){
    cout << "In this loop, I will create " << N << " Particle through the\n"
        << "default constructor and then use the copy operator to put the \n"
        << "correct data in the Particle.\n";
    vector<Particle> vecAssignment; 
    vecAssignment.reserve(N);
    for (int i = 0; i < N-5; i++){
        if( i < vecAssignment.size() ){
        vecAssignment[i] = stdParticle;
        }
        else{ vecAssignment.push_back(stdParticle); }
    }
}

void PushBack(int N, Particle stdParticle){
    cout << "\nIn this loop, I will create " << N << " Particles and add them\n"
        << "to the vector with push_back\n";
    vector<Particle> vecPushBack; 
    for (int i = 0; i < N; i++){
        vecPushBack.push_back(stdParticle);
    }
}
 
void LengthTest(int N, Particle stdParticle){
    cout << "\nI will create a vector and fill it with 'default'\n"
    << " Particles.  I will test for the number of Particles \n"
    << "decide if I should push_back or copy.\n";
    vector<Particle> vecSelectionInt;
    vecSelectionInt.reserve(N-1);
    for (int i = 0; i < N; i++){
        if( i < N-1 ){
            vecSelectionInt[i] = stdParticle;
        }
        else{
            vecSelectionInt.push_back(stdParticle);
        }
    }
}

void SizeTest(int N, Particle stdParticle){
    cout << "\nI will create a vector and fill it with 'default'\n"
        << " Particles.  I will test for the length of the vector to \n"
        << "decide if I should push_back or copy.\n";
    vector<Particle> vecSelectionSize;
    vecSelectionSize.reserve(N);
    for (int i = 0; i < N; i++){
        if( i < vecSelectionSize.size() ){
            vecSelectionSize[i] = stdParticle;
        }
        else{
            vecSelectionSize.push_back(stdParticle);
        }
    }
}

void Reserve(int N, Particle stdParticle){
    cout << "\nI'm going to reserve 'almost' enough space in my vector, but \n"
        << "I won't fill it initially.\n";
    vector<Particle> vecParticle;
    vecParticle.reserve(N-1);
    for( int i = 0; i < N; i++ ){
        vecParticle.push_back(stdParticle);
    }

}

void BasicDeque(int N, Particle stdParticle){
    cout << "\nHere I test using a deque instead of a vector.  I will merely \n"
        << "push_back every particle.\n";
    deque<Particle> deqParticle;
    for( int i = 0; i < N; i++ ){
        deqParticle.push_back(stdParticle);
    }

}

void vecPoint(int N, Particle stdParticle){
    cout << "\nNow I will use a vector of pointers to particles.\n";
    vector<Particle*> vecParticle;
    for(int i = 0; i < N; i++){
        vecParticle.push_back(new Particle(stdParticle));
    }
}
