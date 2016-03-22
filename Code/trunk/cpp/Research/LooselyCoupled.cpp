/*LooselyCoupled.cpp

 This code is used to investigate using Arnoldi's method for a loosely coupled
 system.  

*/

#include <iostream>
#include <list>
#include "boost/format.hpp"
#include "boost/numeric/ublas/io.hpp"
#include "tclap/CmdLine.h"
#include "gnuFile.h"
#include "ArnoldiMC.h"
#include "Power.h"
#include "RandomLib/Random.hpp"
#include "CartesianMesh1D.hh"
#include "Field.hh"
#include "Material.h"
#include "HistSource.h"
#include "gnuplot_i.hpp"
#include "Utilities.h"

using std::cout;
using std::endl;
using std::list;
using std::string;

template<typename V, typename X>
void AddToGnufile(X&, V&, gnuFile&, std::string);

int main(int argc, char** argv){
    cout << "Running LooselyCouple." << endl;
      
// Get command line stuff
try {
    TCLAP::CmdLine cmd("LooselyCoupled will investigate using Arnoldi's method\
on a loosely coupled system.");

    // Add command line arguments
    TCLAP::ValueArg<unsigned int> tracks("t", "tracks", 
        "Number of histories to track", false, 50000, "N", cmd);
    TCLAP::ValueArg<double> relaxed("x", "relaxed",
        "Matrix-vector product relaxation parameter", false, 0.0, "x", cmd);
    TCLAP::ValueArg<unsigned int> n("n", "numValues", 
        "Number of desired eigenvalues to keep", false, 2, "n", cmd);

    TCLAP::ValueArg<unsigned int> Iterations("I", "Iterations", "", false, 10, 
        "I", cmd);
    TCLAP::ValueArg<unsigned int> inactive("i", "inactive", 
        "Number of inactive Restarts", false, 0, "i", cmd);
    TCLAP::ValueArg<unsigned int> active("a", "active", 
        "Number of active Restarts", false, 20, "a", cmd);

    TCLAP::ValueArg<unsigned int> inCycles("c", "incycles",
        "Number of inactive power method cycles", false, 20, "c", cmd);
    TCLAP::ValueArg<unsigned int> aCycles("C", "acycles",
        "Number of active power method cycles", false, 200, "C", cmd);

    TCLAP::ValueArg<unsigned int> bins("b", "bins", 
        "Number of spatial bins (in each region) for discretization", false, 50,
        "b", cmd);
    TCLAP::ValueArg<double> cWidth("", "cw", "Width of center slab", false, 50, 
        "Width", cmd);
    TCLAP::ValueArg<double> rWidth("", "rw", "Width of 'reactor' slab", false, 
        20, "Width", cmd);
    TCLAP::ValueArg<unsigned long> seedArg("s", "seed", "", false,
        0, "Random seed", cmd);
    TCLAP::ValueArg<int> verbose("v", "verbose", "The verbosity of the output", 
        false, 2, "v", cmd);
    TCLAP::ValueArg<double> tolerance( "", "tol", "Tolerance", false, 
            std::numeric_limits<double>::epsilon(), "tol", cmd);

    // What types of initial source are allowed
    std::vector<string> SourceTypes; 
    SourceTypes.push_back("left");  // Left reactor only
    SourceTypes.push_back("right"); // Right reactor only
    SourceTypes.push_back("both");  // Both reactors
    TCLAP::ValuesConstraint<string> SourceAllow(SourceTypes);
    TCLAP::ValueArg<string> Source("", "source", "Initial Source points",
            false, "left", &SourceAllow, cmd);

    // What Arnoldi methods are allowed
    std::vector<string> AllowedMethods; 
    AllowedMethods.push_back("implicit");
    AllowedMethods.push_back("explicit");
    TCLAP::ValuesConstraint<string> MethodAllow(AllowedMethods);
    TCLAP::ValueArg<string> AMethod("m", "arnoldimethod", "Restart Method", 
            false, "explicit", &MethodAllow, cmd);

    TCLAP::SwitchArg arnoldi("A", "Arnoldi", "Run Arnoldi's method", cmd, true);
    TCLAP::SwitchArg power("p", "power", "Run Power method", cmd, true);
    TCLAP::SwitchArg run("r", "run", "Run simulation", cmd, false);

    // Writing data to a file
    TCLAP::ValueArg<string> filename("f", "filename", "Filename for data", 
            false, "data.dat", "f", cmd);

    // Parse the arguments
    cmd.parse(argc, argv);
    std::ostringstream arguments;

    {// Print the arguments to screen
    list<TCLAP::Arg*> Arguments = cmd.getArgList();
    TCLAP::ArgListIterator ArgIter = Arguments.begin();
    cout << "Arguments:" << endl;
    for( ; ArgIter != Arguments.end(); ++ArgIter ){
        cout << "\t" << (*ArgIter)->getName() << " = " 
            << (*ArgIter)->stringRep() << endl;

        arguments << (*ArgIter)->getName() << " = " 
            << (*ArgIter)->stringRep() << ", ";
    }
    }

    // Set random seed
    std::vector<unsigned long> seed(2, 1);
    if( not seedArg.getValue() ){
        seed[0] = RandomLib::Random::SeedWord();
    }
    else{
        seed[0] = seedArg.getValue();
    }

    // Source Mesh
    double totalWidth = 2*rWidth.getValue() + cWidth.getValue();
    CartesianMesh<OneD> SourceMesh( totalWidth, 1.0, bins.getValue()*3 );
    ublas::vector<double> Probabilities( bins.getValue()*3, 0.0 );
    unsigned int start, end;
    if( Source.getValue() == "left" ){
        start = 0;
        end = bins.getValue();
    }
    else if( Source.getValue() == "right" ){
        start = bins.getValue()*2;
        end = bins.getValue()*3;
    }
    else{   // Source.getValue() == "both"
        start = bins.getValue()*2;
        end = bins.getValue()*3;
        for( int j = 0; j < bins.getValue(); ++j ) Probabilities[j] = 1.0;
    }
    for( int j = start; j < end; ++j ) Probabilities[j] = 1.0;
    print_v(Probabilities, "\nInitial Source probabilities");
    HistSource InitialSource(seed, SourceMesh, Probabilities);

    if( run.getValue() ){   // Run simulation
        cout << "Master seed: " << seed[0] << endl;

        // Store date in Data
        gnuFile Data( filename.getValue() );
        Data.appendH( arguments.str() );

        // Plotting objects
        Gnuplot GValues("linespoints"); 
        Gnuplot GVectors("histeps");

        // Materials
        Material Reactor(0.5,0.5,0.0,1.0);
        Material Center(0.4,0.0,0.6,1.0);
        ublas::vector<Material> Mats(3, Reactor);
        Mats[1] = Center;

        // Geometry
        std::vector<double> widths(3, rWidth.getValue() );
        widths[1] = cWidth.getValue();
        CartesianMesh<OneD> GeometryMesh(widths);
        Field<Zone<OneD>, Material, ublas::vector<Material> >
            GeometryField(GeometryMesh, Mats);

        // Arnoldi
        if( arnoldi.getValue() ){
            ArnoldiMC<HistSource> AMC(seed, GeometryField, SourceMesh, 0.0, 
                    verbose.getValue(), tolerance.getValue() );
            RestartMethod RM;
            if( AMethod.getValue() == "implicit" ){
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
            AMC.RAM( InitialSource, n.getValue(), Iterations.getValue(), 
                active.getValue(), inactive.getValue(), tracks.getValue(), RM,
                relaxed.getValue() );

            GValues.set_title("Eigenvalues");
            GValues.set_xlabel("Histories Tracked");
            GValues.set_ylabel("Mean Eigenvalues");

            std::vector<double> Tracks(AMC.Tracks(true));
            std::vector<complex<double> > Mean, SD;
            std::vector<double> mReal, mImag, sReal, sImag;

            for( int i = 0; i < n.getValue(); ++i ){
                std::ostringstream title; title << "RAM l-" << i;
                AMC.MeanValues(Mean, SD, i, true);
                SplitComplexVector(Mean, mReal, mImag);
                SplitComplexVector(SD, sReal, sImag);
                GValues.plot_xy_err( Tracks, mReal, sReal, 
                    title.str());

                title.str(""); title << "RAM eigenvalue-" << i;
                Data.append( title.str(), Tracks, Mean, SD);
            }
            sleep(1);

            GVectors.set_title("Eigenvectors");
            GVectors.set_xlabel("Slab width (mfp)");
            GVectors.set_ylabel("Fission Source Distribution");

            HistSource::ublas_vec_t Centers = InitialSource.Centers();
            ArnoldiMC<HistSource>::c_vec_t MeanVector; ArnoldiMC<HistSource>::c_vec_t SDVector;
            for( int i = 0; i < n.getValue(); ++i ){
                std::ostringstream title; title << "RAM v-" << i;
                AMC.MeanVector(i, MeanVector, SDVector);
                GVectors.plot_xy(Centers, real(MeanVector), title.str());
                
                title.str(""); title << "RAM eigenvector-" << i;
                Data.append( title.str(), Centers, MeanVector, SDVector );
            }
            sleep(1);

            // Add sequence information
            std::vector<ArnoldiMC<HistSource>::c_vec_t> vectors;
            vectors = AMC.VectorEst(0, true);
            // Fundamental estimates
            AddToGnufile(Centers, vectors, Data, "RAM 0 vEst" );

            // First harmonic estimates
            vectors = AMC.VectorEst(1, true);
            AddToGnufile(Centers, vectors, Data, "RAM 1 vEst" );

            // Fundamental mean
            ArnoldiMC<HistSource>::c_vec_t mean;
            ArnoldiMC<HistSource>::c_vec_t sd;
            double R = AMC.Restarts().back();
            vectors.clear();
            for( unsigned int r = 0; r <= R; ++r ){
                AMC.MeanRestartVector(0, r, mean, sd);
                vectors.push_back(mean);
            }
            AddToGnufile(Centers, vectors, Data, "RAM 0 mean" );
            // First harmonic mean
            vectors.clear();
            for( unsigned int r = 0; r <= R; ++r ){
                AMC.MeanRestartVector(1, r, mean, sd);
                vectors.push_back(mean);
            }
            AddToGnufile(Centers, vectors, Data, "RAM 1 mean" );
        }

        // Power method
        if( power.getValue() ){

            // Initial UniformSource
            Field<Zone<OneD>, double, ublas::vector<double> > 
                uSource(SourceMesh, Probabilities);
            BankSource InitialBankSource(uSource, SourceMesh, seed, 
                tracks.getValue());

            cout << "\n==============================\n"
                 << "-----Running Power method-----" 
                 << "\n==============================\n" << endl;
            Power<BankSource> PMC(seed, GeometryField, SourceMesh);
            PMC.power(&InitialBankSource, aCycles.getValue(), 
                inCycles.getValue(), tracks.getValue() );

            GValues.plot_xy_err( PMC.HperI(), PMC.MeanValues(), 
                    PMC.StdDevValues(), "Power" );

            std::vector<double> HperI = PMC.HperI();
            std::vector<double> MeanValues = PMC.MeanValues();
            std::vector<double> StdDevValues = PMC.StdDevValues();
            Data.append(std::string("Power values"), HperI, MeanValues, 
                    StdDevValues);

            ublas::vector<double> Centers = InitialSource.Centers();
            GVectors.plot_xy( Centers, PMC.MeanVector(), "Power-0" );

            std::vector<double> MeanVector = PMC.MeanVector();
            Data.append( "Power vector", Centers, MeanVector );
        }

    }
} 

catch (TCLAP::ArgException &e){ 
    std::cerr << "error: " << e.error() << " for arg " << e.argId() << endl; 
    return 1;
}

    return 0;
}


template<typename V, typename X>
void AddToGnufile(X& x, V& v, gnuFile& gnu, std::string name){
    int index = 0;
    typename V::iterator vIter = v.begin();
    for( ; vIter != v.end(); ++vIter, ++index ){
        std::ostringstream title; title << name+"-" << index;
        gnu.append( title.str(), x, *vIter );
    }
}
