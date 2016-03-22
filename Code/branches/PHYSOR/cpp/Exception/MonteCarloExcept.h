/*MonteCarloExcept.h
 $Author: jlconlin $
 $Id: MonteCarloExcept.h 224 2007-12-20 20:48:15Z jlconlin $
 $Revision: 224 $
 $Date: 2007-12-20 13:48:15 -0700 (Thu, 20 Dec 2007) $

 This file contains the exceptions for Monte Carlo problems.
 */

#ifndef MONTECARLOEXCEPT_H
#define MONTECARLOEXCEPT_H

#include<sstream>
#include "ExceptionBase.hh"
#include "Particle.h"

using std::ostringstream;

class LowWeightParticle : public ExceptionBase
{
public:
	/// The constructor takes the line number and filename where the exception 
    //was thrown.
	LowWeightParticle(const Particle& P, unsigned int line_number,
        const string& file)
		: ExceptionBase(line_number, file) { }

    string msg() const {
        ostringstream out;
        out << "Particle was killed due to low weight.\n";
        return out.str();
    }

};

class NotImplemented 
    : public ExceptionBase
{
public:
    NotImplemented(unsigned int line_number, const string& file)
        : ExceptionBase(line_number, file) {    }
    
    string msg() const {
        ostringstream out;
        out << "Function not implemented in file " << file()
            << " on line " << line();
        return out.str();
    }
};

#endif
