/*test_BankSource.cpp
 $Author: jlconlin $
 $Id: test_BankSource.cpp 236 2008-01-03 21:14:32Z jlconlin $
 $Revision: 236 $
 $Date: 2008-01-03 14:14:32 -0700 (Thu, 03 Jan 2008) $

 This file is used for testing the BankSource class.

 */
#include<iostream>
#include<vector>

#include "RandomLib/Random.hpp"
#include "Field.hh"
#include "BankSource.h"
#include "Particle.h"
#include "Material.h"

using std::vector;
using std::cout;

int main(){
    vector<unsigned long> seed(2);
    seed[0] = RandomLib::Random::SeedWord();
    cout << "Master seed: " << seed[0] << "\n";
    
    unsigned int N  = 100000;
    unsigned int numZones = 10;
    
    cout << "Here I create a mesh of " << numZones 
              << " zones each of width 1.0.\n";
    Mesh meshU(1.0, 1.0, numZones);
    
    cout << "Here I am creating a Field of identical Materials.\n";
    vector<Material> myMaterials(numZones);
    for( vector<Material>::iterator matIter = myMaterials.begin();
        matIter != myMaterials.end(); ++matIter ){
        
        *matIter = Material(0.3, 0.3, 0.4, 1.0);
    }
    Field<Zone, Material> geometry(meshU, myMaterials);

    
    //Uniform source
    cout << "Here I define/create the source probabilities for "
              << "each Zone.\n";
    vector<double> sourceProbability(numZones, 1.0);
    Field<Zone, double> uSource(meshU, sourceProbability);
    
    cout << "BankSource will sample " << N << " particles.\n";
    BankSource* uBank = new BankSource(uSource, meshU, seed, N);
    cout << "Size of BankSource: " << uBank->size() << "\n";
    cout << "\nDiscretizing BankSource...\n";
    cout << "\tNumber of Particles in each Zone: \n";
    
    vector<double> disc = uBank->discretized(meshU);
    for( vector<double>::iterator dIter = disc.begin();
        dIter != disc.end(); ++dIter ){
        
        cout << *dIter << "\n";
    }
    delete uBank;
    return 0;
}

