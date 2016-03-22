/*ArnoldiMatrixTest.cpp
 $Author: jlconlin $
 $Id: ArnoldiMatrixTest.cpp 528 2009-04-29 20:14:39Z jlconlin $
 $Revision: 528 $
 $Date: 2009-04-29 14:14:39 -0600 (Wed, 29 Apr 2009) $

 This is used to test the ArnoldiMatrix class
 */

#include<iostream>
#include<string>
#include<list>
#include "tclap/CmdLine.h"
#include "ArnoldiMatrixTest.h"
#include "ArnoldiMatrix.h"
#include "Matrices.hpp"
#include "boost/numeric/ublas/matrix.hpp"
#include "boost/numeric/bindings/traits/ublas_matrix.hpp"
#include "libs/numeric/bindings/lapack/test/utils.h"

using std::cout;
using std::endl;
using std::complex;
using std::string;
using std::list;

namespace ublas =  boost::numeric::ublas;

int main( int argc, char* argv[] ){

    cout << "I'm testing ArnoldiMatrix class." << endl;
    
try {
    TCLAP::CmdLine cmd("ArnoldiMatrix testing.");

    // Add command line arguments
    TCLAP::ValueArg<unsigned int> dimension("d", "dimension",
            "Dimension of matrix", false, 10, "d", cmd);
    TCLAP::ValueArg<unsigned int> restarts("R", "restarts",
            "Number of Arnoldi Restarts to perform", false, 10, "r", cmd);
    TCLAP::ValueArg<unsigned int> iterations("i", "iterations",
            "Number of Arnoldi Iterations per restart", false, 10, "i", cmd);
    TCLAP::ValueArg<unsigned int> n("n", "numvalues", 
            "Number of desired eigenvalues", false, 1, "n", cmd);
    TCLAP::ValueArg<double> noise("", "noise", "Standard deviation of Noise",
            false, 0.0, "noise", cmd);
    TCLAP::ValueArg<unsigned long> seedArg("s", "seed", "", false,
        0, "Random seed", cmd);
    TCLAP::SwitchArg run("r", "run", "Run simulation", cmd, false);
    
    // What Arnoldi methods are allowed
    std::vector<string> AllowedMethods; 
    AllowedMethods.push_back("implicit"); AllowedMethods.push_back("explicit");
    TCLAP::ValuesConstraint<string> MethodAllow(AllowedMethods);
    TCLAP::ValueArg<string> method("m", "arnoldimethod", "Restart Method", 
            false, "implicit", &MethodAllow, cmd);

    // What Matrices are available
    std::vector<string> Matrices;
    Matrices.push_back("standard"); Matrices.push_back("full"); 
    Matrices.push_back("diagonal"); Matrices.push_back("uhessenberg");
    TCLAP::ValuesConstraint<string> AvailableMatrices(Matrices);
    TCLAP::ValueArg<string> Matrix("M", "matrix", "Matrix Type",
            false, "standard", &AvailableMatrices, cmd);

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

    unsigned int d = dimension.getValue();
    ublas::matrix<double, ublas::column_major> A(d,d);

    // Set random seed
    std::vector<unsigned long> seed(2, 1);
    if( not seedArg.getValue() ){ seed[0] = RandomLib::Random::SeedWord(); }
    else{ seed[0] = seedArg.getValue(); }
    cout << "Master seed: " << seed[0] << endl;

    if( Matrix.getValue() == "standard" ) Matrices::Standard(A);
    else if( Matrix.getValue() == "full" ) Matrices::Full(A);
    else if( Matrix.getValue() == "diagonal" ) Matrices::Diagonal(A);
    else if( Matrix.getValue() == "uhessenberg" ) Matrices::UHessenberg(A);

    ArnoldiMatrixTest arnMatTest(A, seed);
    RestartMethod RM;

    print_m(A, "Input Matrix");
    if( run.getValue() ){
        cout << endl;
        if( method.getValue() == "implicit" ){
            cout << "====================================\n"
                 << "Running IMPLICIT Arnoldi." << endl;
            RM = IMPLICIT;
        }
        else{
            cout << "====================================\n"
                 << "Running EXPLICIT Arnoldi." << endl;
            RM = EXPLICIT;
        }
        arnMatTest.RAM( n.getValue(), iterations.getValue(), 
                restarts.getValue(), RM, noise.getValue());
    }
}

catch (TCLAP::ArgException &e){ 
    std::cerr << "error: " << e.error() << " for arg " << e.argId() << endl; 
    return 1;
}

    return 0;
}

