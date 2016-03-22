/*PowerTest.cpp
 $Author: jlconlin $
 $Id: PowerTest.cpp 644 2009-06-25 15:29:34Z jlconlin $
 $Revision: 644 $
 $Date: 2009-06-25 09:29:34 -0600 (Thu, 25 Jun 2009) $

 This is used to test the Power method.
 */


#include<iostream>
#include "boost/numeric/ublas/vector.hpp"
#include "RandomLib/Random.hpp"
#include "Power.h"
#include "Material.h"
#include "HistSource.h"
#include "Field.hh"
#include "CartesianMesh1D.hh"
#include "gnuplot_i.hpp"
#include "Utilities.h"

using std::cout;
using std::endl;

namespace ublas = boost::numeric::ublas;

int main(int argc, char* argv[]){
    cout << "Testing the Power method." << endl;

    unsigned int histories = 50000;
    unsigned int active = 100;
    unsigned int inactive = 20;

    vector<unsigned long> seed(2);
    seed[0] = RandomLib::Random::SeedWord();
    seed[1] = 1;
    std::cout << "Master seed: " << seed[0] << "\n";

    // Geometry
    double width = 20;
    unsigned int zones = 1;
    CartesianMesh<OneD> meshU(width, 1.0, zones);     
    ublas::vector<Material> mat(zones, Material(0.5, 0.5, 0.0, 1.0));   
    Field<Zone<OneD>, Material, ublas::vector<Material> > geometry(meshU, mat);

    // Discretization mesh
    unsigned int DiscZones = 50;
    CartesianMesh<OneD> DiscMesh(width, 1.0, DiscZones);

    // BankSource
    // Initial uniform source
    ublas::vector<double, std::vector<double> > Probs(geometry.size(), 1.0);
    Field<Zone<OneD>, double, ublas::vector<double, std::vector<double> > > 
        uSource(meshU, Probs);
    BankSource* uBank = new BankSource(uSource, meshU, seed, histories);

    // HistSource
//  HistSource* uHist = new HistSource(seed, uSource);

    // bankPower method
    Power<BankSource> bankPower(seed, geometry, DiscMesh);
    bankPower.power(uBank, active, inactive, histories);

    // histPower method
//  Power<HistSource> histPower(seed, geometry, DiscMesh);
//  histPower.power(uHist, active, inactive, histories);

    // Plot Eigenvalue estimates
    Gnuplot GValues("linespoints");
    GValues.set_title("Power Method");
    GValues.set_xlabel("Active Histories");
    GValues.set_ylabel("Mean Eigenvector Estimate");

    GValues.plot_xy_err(bankPower.Tracks(), bankPower.MeanValues(), 
            bankPower.StdDevValues(), "BankPower" );

    // Plot Eigenvector
    Gnuplot GVectors("histeps");
    GVectors.set_title("Power Method");
    GVectors.set_xlabel("Slab width (mfp)");
    GVectors.set_ylabel("Mean Eigenvector Estimate");

    vector<double> Mean( bankPower.MeanVector() );
    vector<double> edges( Mean.size() );
//  ublas::vector<double> Mean( bankPower.MeanVector() );
//  ublas::vector<double> edges( Mean.size() );
    for( int i = 0; i < edges.size(); ++i ) edges[i] = i+1;
    GVectors.plot_xy(edges, Mean, "BankPower");

    cout << "Mean: " << Mean << endl;
    cout << "edges: " << edges << endl;
    sleep(1);
    return 0;
}
