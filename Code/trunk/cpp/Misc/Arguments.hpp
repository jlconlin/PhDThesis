/*Arguments.hpp
 $Author: jlconlin $
 $Id: Arguments.hpp 683 2009-07-07 19:37:16Z jlconlin $
 $Revision: 683 $
 $Date: 2009-07-07 13:37:16 -0600 (Tue, 07 Jul 2009) $

 This header file contains common command line arguments used in several 
 different "codes".  It will facilitate using these options and creating 
 different codes faster.

 */

#ifndef ARGUMENTS_HPP
#define ARGUMENTS_HPP

#include <iostream>
#include <string>
#include "tclap/CmdLine.h"

using std::cout;
using std::endl;
using std::string;

class Arguments {
public:
    // Public members
    TCLAP::ValueArg<unsigned int> tracks;
    TCLAP::ValueArg<unsigned int> inactive;
    TCLAP::ValueArg<unsigned int> active;

    TCLAP::ValueArg<unsigned int> bins;
    TCLAP::ValueArg<double> width;
    TCLAP::ValueArg<unsigned long> seed;

    TCLAP::ValueArg<int> verbose;
    TCLAP::ValueArg<double> tolerance;
    TCLAP::ValueArg<string> terminal;   // Gnuplot terminal

    TCLAP::SwitchArg MultiMedia;    // Whether or not to plot multimedia
    TCLAP::ValueArg<string> filename;
    TCLAP::SwitchArg run;

    // Parse command line arguments
    void Parse(int argc, char* argv[] );

    // Return string representation of all arguments
    string Args();
    string cmdLine() const{ return _cmdLine; }

    // Comand Line
    TCLAP::CmdLine cmd;

    friend std::ostream& operator<<( std::ostream& os, Arguments& A){
        os << "Arguments:\n";

        std::list<TCLAP::Arg*> args = A.cmd.getArgList();
        TCLAP::ArgListIterator ArgIter = args.begin();
        for( ; ArgIter != args.end(); ++ArgIter ){
            os << "\t" << (*ArgIter)->getName() << " = " 
                << (*ArgIter)->stringRep() << "\n";
        }
        return os;
    }

protected:
    Arguments(string);

    string _cmdLine;
};

#endif

