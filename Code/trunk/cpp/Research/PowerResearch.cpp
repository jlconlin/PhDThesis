/*PowerResearch.cpp

  This code is used to run the power method

*/

#include <iostream>
#include <string>
#include "LinearSpaceSource.h"
#include "PowerArgs.hpp"
#include "Power.h"
#include "RandomLib/Random.hpp"
#include "gnuFile.h"
#include "gnuplot_i.hpp"
#include "CartesianMesh1D.hh"
#include "Field.hh"
#include "Material.h"
#include "HistSource.h"
#include "Utilities.h"
#include "Mesh_AddOns.hh"
#include "boost/numeric/ublas/vector.hpp"
#include "libs/numeric/bindings/lapack/test/utils.h"

using std::cout;
using std::endl;
using std::string;

template<class S>
void RunPower(Power<S>& PMC, S& InitialSource, CartesianMesh<OneD>& M, 
        std::vector<unsigned long>& seed, PowerArgs& Args){

    // Run power method
    PMC.power(&InitialSource, Args.active.getValue(), Args.inactive.getValue(),
            Args.tracks.getValue() );

    // Post Processing---------------------------------------------------------
    std::ostringstream ChartTitle;
    ChartTitle << Args.cmdLine() << ", seed = " << seed[0];
    std::string Title = ChartTitle.str();
    
    // Make sure Title isn't too long and flow off of chart
    if( Title.size() > 120 ){
        int j = Title.size()/120.0;
        for( int i = 0; i < j; ++i ){
            // Find space after 120-th character
            int pos = Title.find(" ", (i+1)*120);   
            if( pos > 0 ) Title.insert(pos, "\\n");
        }
    }
    // Store date in Data
    gnuFile Data( Args.filename.getValue() );
    Data.appendH( Title );
    Data.appendH( Args.Args() );

    // Gnuplot plotting
    Gnuplot GValues("errorlines");
    Gnuplot GVectors("histeps");
    Gnuplot GEntropy("lines");

    // Plot eigenvalue
    GValues.set_title("Eigenvalues\\n" + Title );
    std::vector<double> Tracks = PMC.Tracks();
    std::vector<double> MeanValues = PMC.MeanValues();
    std::vector<double> StdDevValues = PMC.StdDevValues();
    GValues.plot_xy_err( Tracks, MeanValues, StdDevValues, "Power" );
    Data.append("Power values", Tracks, MeanValues, StdDevValues);
    MeanValues = PMC.EigEstimates(true);    // Get Raw values
    Data.append("Power raw values", Tracks, MeanValues);

    // Plot eigenvector
    GVectors.set_title("Eigenvector\\n" + Title );
    ublas::vector<double> Centers = ZoneCenters( M );
    std::vector<double> MeanVector = PMC.MeanVector();
    GVectors.plot_xy( Centers, PMC.MeanVector(), "Power-0" );
    Data.append( "Power vector", Centers, MeanVector );

    // Get Entropy and store it in Data file
    GEntropy.set_title("Entropy\\n" + Title );
    std::vector<double> Entropy = PMC.Entropy();
    GEntropy.plot_xy( Tracks, Entropy, "Entropy");
    Data.append("Entropy", Tracks, Entropy);

    // Get time and store it in Data file
    std::vector<double> time = PMC.Time();
    Data.append("Power-Time", Tracks, time);

    // Get FOM and store it in Data file
    std::vector<double> FOM = PMC.FOM();
    std::vector<double> ActiveTracks = PMC.Tracks(false);
    Data.append("FOM-Power", ActiveTracks, FOM);

}

int main(int argc, char** argv){
    cout << "Running PowerResearch." << endl;

try{
    PowerArgs Args("PowerResearch will run the power method.");

    // Problem specific arguments
    // What Source types are allowed
    std::vector<string> AllowedSources;
    AllowedSources.push_back("Histogram"); AllowedSources.push_back("Linear");
    AllowedSources.push_back("Bank");
    TCLAP::ValuesConstraint<string> SourceAllow(AllowedSources);
    TCLAP::ValueArg<string> source("c", "source", "Fission Source Type", false, 
            "Bank", "c");
    source.Constrain(&SourceAllow);
    Args.cmd.add(source);

    Args.Parse(argc,argv);
    cout << "\n" << Args << endl;

    if( Args.run.getValue() ){
        cout << "Running the power method" << endl;

        // Set random seed
        std::vector<unsigned long> seed(2, 1);
        if( not Args.seed.getValue() ) seed[0] = RandomLib::Random::SeedWord();
        else seed[0] = Args.seed.getValue();

        cout << "Master seed: " << seed[0] << endl;

        // Material from Rathkopf and Martin (1986)
//      Material Absorber(0.5,0.5,0.0,1.0);  // Absorbing
        // Material from Modak et al. (1995)
        Material Absorber(0.8,0.2,0.0,5.0);  // Absorbing

        // Set up materials
        std::vector<double> ZoneWidths;
        ublas::vector<Material> Media;
        if( Args.MultiMedia.getValue() ){   // 5 slab problem from Ueki and Brown (2005)
            ZoneWidths.push_back(1.0);
            ZoneWidths.push_back(1.0);
            ZoneWidths.push_back(5.0);
            ZoneWidths.push_back(1.0);
            ZoneWidths.push_back(1.0);

            Material Scatter( 0.8, 0.0, 0.2, 0.0 );
            Material Fuel( 0.8, 0.1, 0.1, 3.0 );
            Material Absorber( 0.1, 0.0, 0.9, 0.0 );
            Media.resize(5);
            Media[0] = Fuel;
            Media[1] = Scatter;
            Media[2] = Absorber;
            Media[3] = Scatter;
            Media[4] = Fuel;
        }
        else{     // One zone
            ZoneWidths.push_back( Args.width.getValue() );

            Media.resize(1);
            Media[0] = Absorber;
        }

        // Geometry
        CartesianMesh<OneD> GeometryMesh( ZoneWidths );
        Field<Zone<OneD>, Material, ublas::vector<Material> > 
            GeometryField(GeometryMesh, Media);
        cout << "Materials:\n";
        for( int i = 0; i < Media.size(); ++i ){
            cout << Media[i] << endl;
        }

        // Initial Uniform Source
        double totalWidth;
        totalWidth = std::accumulate(ZoneWidths.begin(), ZoneWidths.end(), 0.0);
        CartesianMesh<OneD> SourceMesh(totalWidth, 1.0, Args.bins.getValue());
        if( source.getValue() == "Histogram" ){
            ublas::vector<double, std::vector<double> > 
                Probabilities(Args.bins.getValue(), 1.0);

            // Make initial source point source if two-media problem
            if( Args.MultiMedia.getValue() ){   // Two-media problem
                Probabilities *= 0.0;
                Probabilities[0] = 1.0;
            }
            HistSource InitialSource(seed, SourceMesh, Probabilities);

        }
        else if( source.getValue() == "Linear" ){
            ublas::vector<double> S(SourceMesh.numZones(), 0.0);
            ublas::vector<double> I(SourceMesh.numZones(), 1.0);

            // Make initial source point source if two-media problem
            if( Args.MultiMedia.getValue() ){   // Two-media problem
                I *= 0.0;
                I[0] = 1.0;
            }
            LinearSpaceSource InitialSource(S, I, SourceMesh, seed);

        }
        else if( source.getValue() == "Bank" ){
            ublas::vector<double, std::vector<double> > 
                P(SourceMesh.numZones(), 0.0);
            P[0] = 1.0;
            
            // Make initial source point source if two-media problem
            if( Args.MultiMedia.getValue() ){   // Two-media problem
                P *= 0.0;
                P[0] = 1.0;
            }
            Field<Zone<OneD>, double, ublas::vector<double, 
                std::vector<double> > > uSource(SourceMesh, P);
            BankSource InitialSource(uSource, SourceMesh, seed, 
                    Args.tracks.getValue() );

            Power<BankSource> PMC(seed, GeometryField, SourceMesh);

            RunPower(PMC, InitialSource, SourceMesh, seed, Args);
        }
    }
    }
catch (TCLAP::ArgException &e){ 
    std::cerr << "error: " << e.error() << " for arg " << e.argId() << endl; 
    return 1;
}
    return 0;
}
      
