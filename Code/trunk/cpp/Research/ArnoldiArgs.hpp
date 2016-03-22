/*ArnoldiArgs.hpp
 $Author: jlconlin $
 $Id: ArnoldiArgs.hpp 560 2009-05-27 15:21:00Z jlconlin $
 $Revision: 560 $
 $Date: 2009-05-27 09:21:00 -0600 (Wed, 27 May 2009) $

 This class inherits from Arguments and makes available commonly used Arnoldi
 arguments.

 */

#ifndef ARNOLDIARGS_HPP
#define ARNOLDIARGS_HPP

#include <iostream>
#include <string>
#include "Arguments.hpp"

using std::cout;
using std::endl;
using std::string;

class ArnoldiArgs : public Arguments {
public:
    ArnoldiArgs(string);

    TCLAP::ValueArg<unsigned int> nEigs;
    TCLAP::ValueArg<unsigned int> Iterations;
    TCLAP::ValueArg<double> relaxed;

private:
};

ArnoldiArgs::ArnoldiArgs(string cmdLine)
: Arguments(cmdLine),
relaxed("x", "relaxed", "Relaxation parameter", false, 0.0, "x"),
nEigs("n","eigenvalues", "# Desired eigenvalues", false, 2,"n"),
Iterations("I","Iterations", "# Iterations per Restart", false, 10,"I")
{

    cmd.add(relaxed);
    cmd.add(nEigs);
    cmd.add(Iterations);
}

//  TCLAP::ValueArg<string> method("m", "method", "Restart Method", 
//          false, "explicit", &MethodAllow);
//  Args.cmd.add(AMethod);
    
#endif

