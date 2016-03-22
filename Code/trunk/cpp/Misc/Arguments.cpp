/*Arguments.cpp
 $Author: jlconlin $
 $Id: Arguments.cpp 683 2009-07-07 19:37:16Z jlconlin $
 $Revision: 683 $
 $Date: 2009-07-07 13:37:16 -0600 (Tue, 07 Jul 2009) $

 */

#include <iostream>
#include <list>
#include <limits>
#include <sstream>
#include "tclap/CmdLine.h"
#include "Arguments.hpp"

using std::cout;
using std::endl;
using std::string;

Arguments::Arguments(string cmdLine)
:   cmd(cmdLine),
    tracks("t", "tracks", "# Histories to track", false, 50000, "N"),
    inactive("i", "inactive", "# Inactive Cycles", false, 0, "i"),
    active("a", "active", "# Active Cycles", false, 20, "a"),
    bins("b", "bins", "# spatial bins for discretization", false, 50, "b"),
    width("w", "width", "Width of slab", false, 20, "Width"),
    seed("s", "seed", "", false, 0, "Random seed"),
    verbose("v", "verbose", "The verbosity of the output", false, 2, "v"),
    tolerance( "", "tol", "Tolerance", false, 
            std::numeric_limits<double>::epsilon(), "tol"),
    terminal("", "terminal", "Which gnuplot terminal", false, "dumb", "term"),
    MultiMedia("m", "multimedia", "Multi Media problem", false),
    filename("f", "filename", "Filename for data", true, "data.dat", "f"),
    run("", "run", "Run simulation", false),
    _cmdLine()
{   
    // Add arguments to command line
    cmd.add(tracks);
    cmd.add(inactive);
    cmd.add(active);
    cmd.add(bins);
    cmd.add(width);
    cmd.add(seed);
    cmd.add(verbose);
    cmd.add(tolerance);
    cmd.add(terminal);
    cmd.add(MultiMedia);
    cmd.add(filename);
    cmd.add(run);
}

string Arguments::Args() {
    std::ostringstream arguments;
    arguments << "Arguments: ";

    std::list<TCLAP::Arg*> args = cmd.getArgList();
    TCLAP::ArgListIterator ArgIter = args.begin();
    for( ; ArgIter != args.end(); ++ArgIter ){
        arguments << (*ArgIter)->getName() << " = " 
            << (*ArgIter)->stringRep() << ", ";
    }

    return arguments.str();
}

// Parse command line arguments
void Arguments::Parse(int argc, char* argv[] ) { 
    std::ostringstream line;
    for( int i = 0; i < argc; ++i ) line << argv[i] << " ";
     _cmdLine = line.str();

    // Parse command line
    cmd.parse(argc, argv);
}

