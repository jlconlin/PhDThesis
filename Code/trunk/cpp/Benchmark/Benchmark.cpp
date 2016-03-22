/*Benchmark.cpp

  This code is used to benchmark my Arnoldi and Power method against published
  benchmarks.

*/

#include <iostream>
#include <list>
#include <string>
#include <complex>
#include "tclap/CmdLine.h"
#include "Material.h"
#include "ArnoldiMC.h"
#include "Power.h"
#include "RandomLib/Random.hpp"
#include "boost/numeric/ublas/vector.hpp"
#include "CartesianMesh1D.hh"
#include "Field.hh"
#include "Material.h"
#include "HistSource.h"
#include "gnuFile.h"
#include "gnuplot_i.hpp"


using std::cout;
using std::endl;
using std::complex;

int main(int argc, char* argv[]){
    cout << "Benchmarking" << endl;

try{
    TCLAP::CmdLine cmd("Benchmark will run my code against published \
benchmarks.");

    // Cross section
    TCLAP::ValueArg<double> nu("", "nu", "nu", false, 3.24, "nu", cmd);
    TCLAP::ValueArg<double> xF("", "xF", "fission XS", false, 0.081600, "XS", 
        cmd);
    TCLAP::ValueArg<double> xG("", "xG", "capture XS", false, 0.019584, "XS", 
        cmd);
    TCLAP::ValueArg<double> xS("", "xS", "scattering XS", false, 0.225216, "XS",
        cmd);
    TCLAP::ValueArg<double> xT("", "xT", "total XS", false, 0.32640, "XS", cmd);
    TCLAP::ValueArg<double> secondaries("", "cX", "mean secondaries", false,
        1.50, "XS", cmd);

    // Other benchmark inputs
    TCLAP::ValueArg<double> width("w", "width", "Critical half-width in (cm)", 
        false, 1.853722, "Width", cmd);

    // Monte Carlo necessities
    TCLAP::ValueArg<unsigned int> tracks("t", "tracks", 
        "Number of histories to track", false, 50000, "N", cmd);
    TCLAP::ValueArg<unsigned int> n("n", "numValues", 
        "Number of desired eigenvalues to keep", false, 2, "n", cmd);

    TCLAP::ValueArg<unsigned int> Iterations("I", "Iterations", "", false, 10, 
        "I", cmd);
    TCLAP::ValueArg<unsigned int> inactive("i", "inactive", 
        "Number of inactive Restarts", false, 2, "i", cmd);
    TCLAP::ValueArg<unsigned int> active("a", "active", 
        "Number of active Restarts", false, 20, "a", cmd);

    TCLAP::ValueArg<unsigned int> inCycles("c", "incycles",
        "Number of inactive power method cycles", false, 20, "c", cmd);
    TCLAP::ValueArg<unsigned int> aCycles("C", "acycles",
        "Number of active power method cycles", false, 200, "C", cmd);

    TCLAP::ValueArg<unsigned int> bins("b", "bins", 
        "Number of spatial bins for discretization", false, 50, "b", cmd);
    TCLAP::ValueArg<unsigned long> seedArg("s", "seed", "", false,
        0, "Random seed", cmd);
    TCLAP::ValueArg<int> verbose("v", "verbose", "The verbosity of the output", 
        false, 2, "v", cmd);
    TCLAP::ValueArg<double> tolerance( "", "tol", "Tolerance", false, 
            std::numeric_limits<double>::epsilon(), "tol", cmd);

    // Writing data to a file
    TCLAP::ValueArg<std::string> filename("f", "filename", "Filename for data", 
            false, "benchmark.dat", "f", cmd);
    TCLAP::SwitchArg run("r", "run", "Run simulation", cmd, false);

    // Parse the arguments
    cmd.parse(argc, argv);
    std::ostringstream arguments;

    {// Print the arguments to screen
    std::list<TCLAP::Arg*> Arguments = cmd.getArgList();
    TCLAP::ArgListIterator ArgIter = Arguments.begin();
    cout << "Arguments:" << endl;
    for( ; ArgIter != Arguments.end(); ++ArgIter ){
        cout << "\t" << (*ArgIter)->getName() << " = " 
            << (*ArgIter)->stringRep() << endl;

        arguments << (*ArgIter)->getName() << " = " 
            << (*ArgIter)->stringRep() << ", ";
    }
    }

    if( run.getValue() ){   // Run simulation
        cout << "\nPerforming PUa-1-0-SL Bare simulation." << endl;
        std::ostringstream ChartTitle;
        for( int i = 0; i < argc; ++i ) ChartTitle << argv[i] << " ";

        // Set random seed
        std::vector<unsigned long> seed(2, 1);
        if( not seedArg.getValue() ) seed[0] = RandomLib::Random::SeedWord();
        else seed[0] = seedArg.getValue();

        ChartTitle << " seed = " << seed[0];
        cout << "Master seed: " << seed[0] << endl;

        // Store date in Data
        gnuFile Data( filename.getValue() );

        // Material
        Material mat(xS.getValue(), xF.getValue(), xG.getValue(), 
            nu.getValue());

        // Geometry --- One zone
        CartesianMesh<OneD> GeometryMesh(2*width.getValue(), 1.0, 1); 
        ublas::vector<Material> Mats(1, mat );
        Field<Zone<OneD>, Material, ublas::vector<Material> > 
            GeometryField(GeometryMesh, Mats);

        // Initial Uniform Source
        CartesianMesh<OneD> SourceMesh(2*width.getValue(), 1.0, 
                bins.getValue());
        ublas::vector<double> Probabilities(bins.getValue(), 1.0);
        HistSource InitialSource(seed, SourceMesh, Probabilities);

        ArnoldiMC<HistSource> AMC(seed, GeometryField, SourceMesh, 0.0, verbose.getValue(), 
            tolerance.getValue() );
        RestartMethod RM = EXPLICIT;

        AMC.RAM( InitialSource, n.getValue(), Iterations.getValue(), 
            active.getValue(), inactive.getValue(), tracks.getValue(), RM, 0.0);

        // Initial UniformSource
        Field<Zone<OneD>, double, ublas::vector<double> > 
            uSource(SourceMesh, Probabilities);
        BankSource InitialBankSource(uSource, SourceMesh, seed, 
            tracks.getValue());

        cout << "\n==============================\n"
             << "-----Running Power method-----" 
             << "\n==============================\n" << endl;
        Power<BankSource> PMC(seed, GeometryField, SourceMesh);
        PMC.power(&InitialBankSource, aCycles.getValue(), inCycles.getValue(), 
            tracks.getValue() );

        std::vector<double> amcTracks(AMC.Tracks(true));

        // Plot Eigenvalue convergence
        Gnuplot GValues("linespoints");
        std::vector<complex<double> > mValue, sdValue;
        std::vector<double> mReal, mImag, sdReal, sdImag;
        for( int i = 0; i < 2; ++i ){
            std::ostringstream title; title << "RAM l-" << i;
            AMC.MeanValues(mValue, sdValue, i, true);
            SplitComplexVector(mValue, mReal, mImag);
            SplitComplexVector(sdValue, sdReal, sdImag);
            GValues.plot_xy_err( amcTracks, mReal, sdReal, title.str());

            title.str(""); title << "RAM eigenvalue-" << i;
            Data.append( title.str(), amcTracks, mValue, sdValue);
        }
        // Power method
        GValues.plot_xy_err( PMC.HperI(), PMC.MeanValues(), PMC.StdDevValues(), 
            "Power" );
        std::vector<double> HperI = PMC.HperI();
        std::vector<double> MeanValues = PMC.MeanValues();
        std::vector<double> StdDevValues = PMC.StdDevValues();
        Data.append(std::string("Power values"), HperI, MeanValues, 
                StdDevValues);
        sleep(1.0);

        // Plot mean eigenvectors
        Gnuplot GVectors("histeps");
        GVectors.set_xlabel("Slab width (mfp)");
        GVectors.set_ylabel("Fission Source Distribution");
        HistSource::ublas_vec_t Centers = InitialSource.Centers();
        ArnoldiMC<HistSource>::c_vec_t mVector; ArnoldiMC<HistSource>::c_vec_t sdVector;
        for( int i = 0; i < 2; ++i ){
            std::ostringstream title; title << "RAM v-" << i;
            AMC.MeanVector(i, mVector, sdVector);
            GVectors.plot_xy(Centers, real(mVector), title.str());
            
            title.str(""); title << "RAM eigenvector-" << i;
            Data.append( title.str(), Centers, mVector, sdVector );
        }
        // Power method
        GVectors.plot_xy( Centers, PMC.MeanVector(), "Power-0" );
        std::vector<double> MeanVector = PMC.MeanVector();
        Data.append( "Power vector", Centers, MeanVector );
        sleep(1);

    }

}

catch (TCLAP::ArgException &e){ 
    std::cerr << "error: " << e.error() << " for arg " << e.argId() << endl; 
    return 1;
}
    return 0;

}
