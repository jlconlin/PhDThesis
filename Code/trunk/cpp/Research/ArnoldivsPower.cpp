/*ArnoldivsPower.cpp

 This code is used to compare Arnoldi's method (both EXPLICIT and IMPLICIT)
 to the traditional Power method.

*/

#include <iostream>
#include <list>
#include "boost/format.hpp"
#include "boost/numeric/ublas/io.hpp"
#include "tclap/CmdLine.h"
#include "ArnoldiMC.h"
#include "Power.h"
#include "RandomLib/Random.hpp"
#include "CartesianMesh1D.hh"
#include "Field.hh"
#include "Material.h"
#include "HistSource.h"
#include "gnuplot_i.hpp"
#include "Utilities.h"
#include "gnuFile.h"

using std::cout;
using std::endl;
using std::list;
using std::string;

template<typename V, typename X>
void AddToGnufile(X&, V&, gnuFile&, std::string);

int main(int argc, char** argv){
    cout << "Running ArnoldivsPower." << endl;
      
// Get command line stuff
try {
    TCLAP::CmdLine cmd("ArnoldivsPower will compare Monte Carlo Arnoldi's \
method with the traditional power method.");

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
        "Number of spatial bins for discretization", false, 50, "b", cmd);
    TCLAP::ValueArg<double> width("w", "width", "Width of slab", false, 20, 
        "Width", cmd);
    TCLAP::ValueArg<unsigned long> seedArg("s", "seed", "", false,
        0, "Random seed", cmd);
    TCLAP::ValueArg<int> verbose("v", "verbose", "The verbosity of the output", 
        false, 2, "v", cmd);
    TCLAP::ValueArg<double> tolerance( "", "tol", "Tolerance", false, 
            std::numeric_limits<double>::epsilon(), "tol", cmd);

    // What Arnoldi methods are allowed
    std::vector<string> AllowedMethods; AllowedMethods.push_back("implicit");
    AllowedMethods.push_back("explicit");
    TCLAP::ValuesConstraint<string> MethodAllow(AllowedMethods);
//  TCLAP::ValueArg<string> AMethod("m", "arnoldimethod", "Restart Method", 
//          false, "implicit", &MethodAllow, cmd);
    TCLAP::ValueArg<string> AMethod("m", "arnoldimethod", "Restart Method", 
            false, "implicit","m");
    AMethod.Constrain(&MethodAllow);
    cmd.add(AMethod);

    // Plotting/storing arguments
    TCLAP::SwitchArg pResidual("d", "residual", "Plot Residual", cmd, false);
    TCLAP::ValueArg<unsigned int> pEigenvalues("", "evalues", 
            "Number of eigenvalues to plot", false, 2, "e", cmd);
    TCLAP::ValueArg<unsigned int> pEigenvectors("", "evectors", 
            "Number of eigenvectors to plot", false, 2, "e", cmd);
    TCLAP::SwitchArg raw("", "raw", " Plot Raw Eigenvalues", cmd, false);
    TCLAP::ValueArg<unsigned int> sVectors("", "storevectors", 
            "Store Dominant eigenvectors", false, 0, "v", cmd);

    TCLAP::SwitchArg arnoldi("A", "Arnoldi", "Run Arnoldi's method", cmd, true);
    TCLAP::SwitchArg power("p", "power", "Run Power method", cmd, true);
    TCLAP::SwitchArg run("r", "run", "Run simulation", cmd, false);

    // Writing data to a file
    TCLAP::ValueArg<string> filename("f", "filename", "Filename for data", 
            true, "data.dat", "f", cmd);

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


    if( run.getValue() ){   // Run simulation
        std::ostringstream ChartTitle;
        for( int i = 0; i < argc; ++i ) ChartTitle << argv[i] << " ";

        // Set random seed
        std::vector<unsigned long> seed(2, 1);
        if( not seedArg.getValue() ){
            seed[0] = RandomLib::Random::SeedWord();
            ChartTitle << " seed = " << seed[0];
        }
        else{
            seed[0] = seedArg.getValue();
            ChartTitle << " seed = " << seed[0];
        }

        cout << "Master seed: " << seed[0] << endl;

        // Store date in Data
        gnuFile Data( filename.getValue() );

        // Make sure Title isn't too long and flow off of chart
        std::string Title = ChartTitle.str();
        if( Title.size() > 120 ){
            int j = Title.size()/120.0;
            for( int i = 0; i < j; ++i ){
                // Find space after 120-th character
                int pos = Title.find(" ", (i+1)*120);   
                if( pos > 0 ) Title.insert(pos, "\\n");
            }
        }
        Data.appendH( Title );
        Data.appendH( arguments.str() );

        // Plotting objects
        Gnuplot GValues("linespoints"); 
        Gnuplot GVectors("histeps");
        Gnuplot GDomVectors("histeps");
        Gnuplot GResidual("linespoints");
        GValues.set_title("Eigenvalues\\n" + Title );
        GVectors.set_title("Eigenvectors\\n" + Title );
        GDomVectors.set_title("Dominant Eigenvectors\\n" + Title );
        GResidual.set_title("Residual\\n" + Title );

        // Geometry --- One zone
        CartesianMesh<OneD> GeometryMesh(width.getValue(), 1.0, 1); 
//      ublas::vector<Material> Mats(1, Material(0.9,0.1,0.0,1.0) ); // Scatter
        ublas::vector<Material> Mats(1, Material(0.5,0.5,0.0,1.0) ); // Absorb
        Field<Zone<OneD>, Material, ublas::vector<Material> > 
            GeometryField(GeometryMesh, Mats);

        cout << Mats[0] << endl;
        cout << Mats[0] << endl;
        cout << Mats[0] << endl;
        cout << Mats[0] << endl;
        cout << Mats[0] << endl;

        // Initial Uniform Source
        CartesianMesh<OneD> SourceMesh(width.getValue(), 1.0, bins.getValue());
        ublas::vector<double, std::vector<double> > 
            Probabilities(bins.getValue(), 1.0);
        HistSource InitialSource(seed, SourceMesh, Probabilities);

        HistSource::ublas_vec_t Centers = InitialSource.Centers();

        // Arnoldi
        if( arnoldi.getValue() ){
            ArnoldiMC<HistSource> AMC(seed, GeometryField, SourceMesh, 
                relaxed.getValue(), verbose.getValue(), tolerance.getValue() );
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
                active.getValue(), inactive.getValue(), tracks.getValue(), RM );

            std::vector<double> Tracks(AMC.Tracks(true));
            std::vector<double> TracksActive(AMC.Tracks(false));

            if( pResidual.getValue() ){// Plot Residual
                GResidual.set_xlabel("Restarts");
                GResidual.set_ylabel("Residual");
                GResidual.set_ylogscale();
                std::vector<double> Real, Imag;
                SplitComplexVector(AMC.Residual(true), Real, Imag);
                GResidual.plot_xy( Tracks, Real, "Residual");
                
                std::vector<complex<double> > Residual = AMC.Residual(true);
                Data.append( "Residual", Tracks, Residual );
            }

            if( pEigenvalues.getValue() ){// Plot Eigenvalues
                GValues.set_xlabel("Histories Tracked");
                GValues.set_ylabel("Mean Eigenvalues");
                std::vector<complex<double> > Mean, SD;
                std::vector<double> mReal, mImag, sReal, sImag;

                for( int i = 0; i < pEigenvalues.getValue(); ++i ){
                    std::ostringstream title; title << "RAM l-" << i;
                    AMC.MeanValues(Mean, SD, i, true);
                    SplitComplexVector(Mean, mReal, mImag);
                    SplitComplexVector(SD, sReal, sImag);
                    GValues.plot_xy_err( Tracks, mReal, sReal, 
                        title.str());

                    title.str(""); title << "RAM eValue SD-" << i;
                    Data.append( title.str(), Tracks, SD );

                    title.str(""); title << "RAM eigenvalue-" << i;
                    Data.append( title.str(), Tracks, Mean, SD);
                }
                sleep(1);
            }

            if( pEigenvectors.getValue() ){ // Plot Eigenvectors
                GVectors.set_xlabel("Slab width (mfp)");
                GVectors.set_ylabel("Fission Source Distribution");

                ArnoldiMC<HistSource>::c_vec_t Mean; ArnoldiMC<HistSource>::c_vec_t SD;
                for( int i = 0; i < pEigenvectors.getValue(); ++i ){
                    std::ostringstream title; title << "RAM v-" << i;
                    AMC.MeanVector(i, Mean, SD);
                    GVectors.plot_xy(Centers, real(Mean), title.str());
                    
                    title.str(""); title << "RAM eigenvector-" << i;
                    Data.append( title.str(), Centers, Mean, SD );
                }
                sleep(1);
            }

            if( raw.getValue() ){
                Gnuplot rawValues("linespoints");
                rawValues.set_title("Raw Eigenvalues\\n" + Title );
                rawValues.set_ylabel("Eigenvalue Estimates");
                std::vector<complex<double> > rawE = AMC.Values(0, true);
                std::vector<double> real, imag;
                SplitComplexVector(rawE, real, imag);
                rawValues.plot_xy(Tracks, real);

                Data.append( "Raw Eigenvalues", Tracks, rawE );
                sleep(1);
            }

            // Add Figure of Merit information
            std::vector<double> FOM = AMC.FOM();
            std::vector<double> Time = AMC.Time(false);
            Data.append("FOM-Arnoldi", TracksActive, FOM );

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
            Field<Zone<OneD>, double, ublas::vector<double, 
                std::vector<double> > > uSource(SourceMesh, Probabilities);
            BankSource InitialBankSource(uSource, SourceMesh, seed, 
                tracks.getValue());

            cout << "\n==============================\n"
                 << "-----Running Power method-----" 
                 << "\n==============================\n" << endl;
            Power<BankSource> PMC(seed, GeometryField, SourceMesh);
            PMC.power(&InitialBankSource, aCycles.getValue(), 
                    inCycles.getValue(), tracks.getValue() );

            std::vector<double> Tracks = PMC.Tracks();
            if( pEigenvalues.getValue() ){
                GValues.plot_xy_err( PMC.Tracks(), PMC.MeanValues(), 
                        PMC.StdDevValues(), "Power" );

                std::vector<double> MeanValues = PMC.MeanValues();
                std::vector<double> StdDevValues = PMC.StdDevValues();
                Data.append("Power values", Tracks, MeanValues, StdDevValues);

                Data.append("Power eValues SD", Tracks, StdDevValues);
            }

            if( pEigenvectors.getValue() ){ // Plot Eigenvector
                ublas::vector<double> Centers = InitialSource.Centers();
                GVectors.plot_xy( Centers, PMC.MeanVector(), "Power-0" );

                std::vector<double> MeanVector = PMC.MeanVector();
                Data.append( "Power vector", Centers, MeanVector );
            }

            // Add Figure of Merit information
            std::vector<double> FOM = PMC.FOM();
            std::vector<double> hist;
            // Add histories
            hist.insert(hist.begin(), Tracks.begin() + inCycles.getValue(), 
                    Tracks.end());
//          std::copy(Tracks.begin() + inCycles.getValue(), Tracks.end(), 
//                  hist.begin() );
            print_v(Tracks, "Tracks");
            print_v(hist, "hist");
            Data.append("FOM-Power", hist, FOM );


        }
        sleep(1);
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

