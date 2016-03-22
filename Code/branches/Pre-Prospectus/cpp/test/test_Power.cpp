/*test_Power.cpp
 $Author: jlconlin $
 $Id: test_Power.cpp 238 2008-01-05 22:05:09Z jlconlin $
 $Revision: 238 $
 $Date: 2008-01-05 15:05:09 -0700 (Sat, 05 Jan 2008) $

 This file is used for testing the Markov class.

 */

#include<iostream>
#include<string>
#include "RandomLib/Random.hpp"
#include "Particle.h"
#include "BankSource.h"
#include "Power.h"
#include "Material.h"

int main(){
    int histories = 5000;
    int active = 100;
    int inactive = 20;
    double width = 20.0;

    int zones = 2;
    Mesh meshU(width, 1.0, zones);     //Slab geometry, one zone, 20 units wide
    vector<Material> mat(zones, Material(0.5, 0.5, 0.0, 1.0));   //'Standard' material
    Field<Zone, Material> geometry(meshU, mat);
    vector<unsigned long> seed(2);
    seed[0] = RandomLib::Random::SeedWord();
    seed[1] = 1;
    std::cout << "Master seed: " << seed[0] << "\n";
    RandomLib::Random rng(seed);

    // BankSource power
    //Initial uniform source
    vector<double> sourceProbability(geometry.size(), 1.0);
    Field<Zone, double> uSource(meshU, sourceProbability);    
    BankSource* uBank = new BankSource(uSource, meshU, seed, histories);
    
    Power<BankSource> bankPower(seed, geometry);
    bankPower.power(uBank, active, inactive, histories);

    const Zone* tmpZone = new Zone(1.0, 1.0, 1);
    Particle p1(seed, tmpZone);
    std::cout << "Total particles: " << p1.ID() << std::endl;

    // HistSource power
    HistSource* uHist = new HistSource(seed, uSource); // Initial uniform Source
    Power<HistSource> histPower(seed, geometry);
    histPower.power(uHist, active, inactive, histories);

    Particle p2(seed, tmpZone);
    std::cout << "Total particles: " << p2.ID() << std::endl;
    
    return 0;
}


