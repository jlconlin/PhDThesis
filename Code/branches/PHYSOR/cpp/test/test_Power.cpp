/*test_Power.cpp
 $Author: jlconlin $
 $Id: test_Power.cpp 275 2008-03-18 19:56:21Z jlconlin $
 $Revision: 275 $
 $Date: 2008-03-18 13:56:21 -0600 (Tue, 18 Mar 2008) $

 This file is used for testing the Power class.

 */

#include<cstdlib>
#include<iostream>
#include<iomanip>
#include<string>
#include "RandomLib/Random.hpp"
#include "Particle.h"
#include "BankSource.h"
#include "Power.h"
#include "Material.h"

int main( int argc, char* argv[]){
    double width = std::atof(argv[1]);
    int histories = std::atoi(argv[2]);
    int active = 100;
    int inactive = 20;

    int zones = 10;
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
    
    Power<BankSource> bankPower(seed, geometry, meshU);
    bankPower.power(uBank, active, inactive, histories);
    std::cout << "\nMean Vector:\n" << bankPower.MeanVector() << std::endl;
    std::cout << "\nStdDec Vector:\n" << std::setprecision(7) << bankPower.StdDevVector() << std::endl;

    const Zone* tmpZone = new Zone(1.0, 1.0, 1);
    Particle p1(seed, tmpZone);
    std::cout << "Total particles: " << p1.ID() << std::endl;

    // HistSource power
    HistSource* uHist = new HistSource(seed, uSource); // Initial uniform Source
    Power<HistSource> histPower(seed, geometry, meshU);
//  histPower.power(uHist, active, inactive, histories);

    Particle p2(seed, tmpZone);
    std::cout << "Total particles: " << p2.ID() << std::endl;
    
    return 0;
}


