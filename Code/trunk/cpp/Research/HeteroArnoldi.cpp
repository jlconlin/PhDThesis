/*Hetero.cpp

  This code is used to run Arnoldi's method.

*/

#include <iostream>
#include <string>
#include <complex>
#include "LinearSpaceSource.h"
#include "ArnoldiMC.h"
#include "ArnoldiArgs.hpp"
#include "RandomLib/Random.hpp"
#include "gnuFile.h"
#include "gnuplot_i.hpp"
#include "CartesianMesh1D.hh"
#include "Field.hh"
#include "Material.h"
#include "HistSource.h"
#include "Utilities.h"
#include "boost/numeric/ublas/vector.hpp"
#include "boost/numeric/ublas/vector_expression.hpp"
#include "boost/numeric/bindings/traits/ublas_vector.hpp"
#include "boost/numeric/ublas/io.hpp"

using std::cout;
using std::endl;
using std::string;
using std::complex;

namespace ublas =  boost::numeric::ublas;

template<class S>
Gnuplot& PlotVectors(ArnoldiArgs&, ArnoldiMC<S>&, S&, Gnuplot&, 
        CartesianMesh<OneD>&, gnuFile&);

template<>
Gnuplot& PlotVectors(ArnoldiArgs& Args, ArnoldiMC<HistSource>& AMC, 
        HistSource& InitialSource, Gnuplot& G, CartesianMesh<OneD>& M, 
        gnuFile& Data){
    G.set_style("histeps");

    HistSource::ublas_vec_t Centers = InitialSource.Centers();
    ArnoldiMC<HistSource>::c_vec_t Mean, SD;
    ublas::vector<double> mReal, mImag, sReal, sImag;
    for( int i = 0; i < Args.nEigs.getValue(); ++i ){
        std::ostringstream title; title << "RAM l-" << i;

        AMC.MeanVector(i, Mean, SD);

#ifndef NOPLOT
        G.plot_xy(Centers, real(Mean), title.str());
#endif

        // Append data to file
        title.str(""); title << "RAM eigenvector-" << i;
        Data.append(title.str(), Centers, Mean );

    }
}

Gnuplot& PlotVectors(ArnoldiArgs& Args, ArnoldiMC<LinearSpaceSource>& AMC, 
        LinearSpaceSource& InitialSource, Gnuplot& G, CartesianMesh<OneD>& M, 
        gnuFile& Data){
    G.set_style("lines");

    ArnoldiMC<HistSource>::c_vec_t Mean, SD;
    std::vector<double> Slopes, Intercepts;
    std::vector<double>::iterator sIter;
    std::vector<double>::iterator iIter;
    std::vector<double>::iterator mIter;

    std::vector<double> x,y;
    for( int i = 0; i < Args.nEigs.getValue(); ++i ){
        std::ostringstream title; title << "RAM l-" << i;

        AMC.MeanVector(i, Mean, SD);

        // Make LinearSpaceSource from Arnoldi vector
        LinearSpaceSource LSS(AMC.Seed(), M, real(Mean));
        LSS.PlotPoints(x,y);
       
#ifndef NOPLOT
        G.plot_xy(x,y, title.str() );
#endif

        // Append data to file
        title.str(""); title << "RAM eigenvector-" << i;
        Data.append(title.str(), x,y);

    }
}

template<class S>
void RunArnoldi(ArnoldiMC<S>& AMC, S& InitialSource, RestartMethod& RM, 
        std::vector<unsigned long>& seed, CartesianMesh<OneD>& SourceMesh,
        ArnoldiArgs& Args){

    AMC.RAM( InitialSource, Args.nEigs.getValue(), Args.Iterations.getValue(),
            Args.active.getValue(), Args.inactive.getValue(), 
            Args.tracks.getValue(), RM);

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

#ifndef NOPLOT
    // Gnuplot plotting
    Gnuplot GValues("linespoints");
    Gnuplot GVectors("histeps");
    Gnuplot GEntropy("lines");
    cout << "set terminal " + Args.terminal.getValue() << endl;
    GValues.cmd("set terminal " + Args.terminal.getValue() );
    GVectors.cmd("set terminal " + Args.terminal.getValue() );
    GVectors.cmd("set xzeroaxis");
    GEntropy.cmd("set terminal " + Args.terminal.getValue() );


    // Plot Eigenvalues
    GValues.set_title("Eigenvalues\\n" + Title );
    GValues.set_xlabel("Histories Tracked");
    GValues.set_ylabel("Mean Eigenvalues");
#endif

    std::vector<double> Tracks(AMC.Tracks(true));
    std::vector<complex<double> > Mean, SD, Values;
    std::vector<double> mReal, mImag, sReal, sImag;
    for( int n = 0; n < Args.nEigs.getValue(); ++n ){
        std::ostringstream title; title << "RAM l-" << n;
        AMC.MeanValues(Mean, SD, n, true);
        SplitComplexVector(Mean, mReal, mImag);
        SplitComplexVector(SD, sReal, sImag);
#ifndef NOPLOT
        GValues.plot_xy_err( Tracks, mReal, sReal, title.str());
#endif

        title.str(""); title << "RAM eigenvalue-" << n;
        Data.append( title.str(), Tracks, Mean, SD);
        title.str(""); title << "raw value-0" << n;
        Values = AMC.Values(n, true);
        Data.append( title.str(), Tracks, Values );

    }

#ifndef NOPLOT
    // Plot Eigenvectors
    GVectors.set_title("Eigenvectors\\n" + Title );
    PlotVectors(Args, AMC, InitialSource, GVectors, SourceMesh, Data);
#endif

    // Get time and store it in Data file
    std::vector<double> time = AMC.Time();
    Data.append("Arnoldi-Time", Tracks, time);

    // Get FOM and store it in Data file
    std::vector<double> FOM = AMC.FOM();
    std::vector<double> ActiveTracks = AMC.Tracks(false);
    Data.append("FOM-Arnoldi", ActiveTracks, FOM);

    // Plot and store Entropy
    GEntropy.set_title("Entropy\\n" + Title );
    std::vector<double> Entropy = AMC.Entropy(true);
    std::vector<double> IterationsTracks = AMC.TracksbyIteration(true);
    GEntropy.plot_xy( IterationsTracks, Entropy, "Entropy");
    Data.append("Entropy", IterationsTracks, Entropy );
}

int main(int argc, char** argv){
    cout << "Running Hetero." << endl;

try{
    ArnoldiArgs Args("Arnoldi will run Arnoldi's method.");

    // Problem specific arguments
    // What Arnoldi methods are allowed
    std::vector<string> AllowedMethods; 
    AllowedMethods.push_back("implicit"); AllowedMethods.push_back("explicit");
    TCLAP::ValuesConstraint<string> MethodAllow(AllowedMethods);
    TCLAP::ValueArg<string> method("m", "method", "Restart Method", false, 
            "explicit","m");
    method.Constrain(&MethodAllow);
    Args.cmd.add(method);

    // What Source types are allowed
    std::vector<string> AllowedSources;
    AllowedSources.push_back("Histogram"); AllowedSources.push_back("Linear");
    TCLAP::ValuesConstraint<string> SourceAllow(AllowedSources);
    TCLAP::ValueArg<string> source("c", "source", "Fission Source Type", false, 
            "Histogram", "c");
    source.Constrain(&SourceAllow);
    Args.cmd.add(source);

    // Width of reflector
    TCLAP::ValueArg<double> width2("", "width2", "Second Media width", false, 
            0.0, "w");
    Args.cmd.add(width2);

    // Number of regions
    TCLAP::ValueArg<unsigned int> Regions("g", "regions", "Number of Regions", 
            false, 1, "reg");
    Args.cmd.add(Regions);

    // Parse command line arguments
    Args.Parse(argc,argv);
    cout << "\n" << Args << endl;

    if( Args.run.getValue() ){
        cout << "Running Arnoldi's Method" << endl;
        //
        // Set random seed
        std::vector<unsigned long> seed(2, 1);
        if( not Args.seed.getValue() ) seed[0] = RandomLib::Random::SeedWord();
        else seed[0] = Args.seed.getValue();

        cout << "Master seed: " << seed[0] << endl;
        
        Material Mat1(0.2,0.8,0.0,1.0); // Absorb
        Material Mat2(0.9,0.1,0.0,1.0); // Scatter

        // Geometry
        ublas::vector<Material> Media(2*Regions.getValue()+1, Mat1);
        std::vector<double> ZoneWidths;
        for( unsigned int i = 0; i < Regions.getValue(); ++i ){
            ZoneWidths.push_back(Args.width.getValue());
            ZoneWidths.push_back(width2.getValue());
            Media[2*i+1] = Mat2;
        }
        CartesianMesh<OneD> GeometryMesh( ZoneWidths );

        Field<Zone<OneD>, Material, ublas::vector<Material> > 
            GeometryField(GeometryMesh, Media);
        cout << "Materials:\n";
        for( int i = 0; i < Media.size(); ++i ){
            cout << Media[i] << endl;
        }
        cout << endl;

        // Restart Method
        RestartMethod RM;
        if( method.getValue() == "implicit" ){
            cout << "\n==============================\n"
                 << "---Running Implicit Arnoldi---" 
                 << "\n==============================\n" << endl;
            RM = IMPLICIT;
        }
        else{ // explicit Arnoldi
            cout << "\n==============================\n"
                 << "---Running Explicit Arnoldi---" 
                 << "\n==============================\n" << endl;
            RM = EXPLICIT;
        }

        // Initial Source
        double totalWidth;
        totalWidth = std::accumulate(ZoneWidths.begin(), ZoneWidths.end(), 0.0);
        CartesianMesh<OneD> SourceMesh(totalWidth, 1.0, Args.bins.getValue());
        if( source.getValue() == "Histogram" ){
            ublas::vector<double, std::vector<double> > 
                Probabilities(Args.bins.getValue(), 1.0);
            HistSource InitialSource(seed, SourceMesh, Probabilities);

            ArnoldiMC<HistSource> AMC(seed, GeometryField, SourceMesh, 
                    Args.relaxed.getValue(), Args.verbose.getValue(), 
                    Args.tolerance.getValue() );

            RunArnoldi(AMC, InitialSource, RM, seed, SourceMesh, Args);
        }
        else if( source.getValue() == "Linear" ){
            ublas::vector<double> S(SourceMesh.numZones(), 0.0);
            ublas::vector<double> I(SourceMesh.numZones(), 1.0);
            LinearSpaceSource InitialSource(S, I, SourceMesh, seed);

            ArnoldiMC<LinearSpaceSource> AMC(seed, GeometryField, SourceMesh, 
                    Args.relaxed.getValue(), Args.verbose.getValue(), 
                    Args.tolerance.getValue() );

            RunArnoldi(AMC, InitialSource, RM, seed, SourceMesh, Args);
        }
    }
}

catch (TCLAP::ArgException &e){ 
    std::cerr << "error: " << e.error() << " for arg " << e.argId() << endl; 
    return 1;
}
    return 0;
}

