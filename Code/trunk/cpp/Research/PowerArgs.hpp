/*PowerArgs.hpp
 $Author: jlconlin $
 $Id: PowerArgs.hpp 555 2009-05-21 02:01:12Z jlconlin $
 $Revision: 555 $
 $Date: 2009-05-20 20:01:12 -0600 (Wed, 20 May 2009) $

 This class inherits from Arguments and makes available commonly used Power
 arguments.

 */

#ifndef POWERARGS_HPP
#define POWERARGS_HPP

#include <iostream>
#include <string>
#include "Arguments.hpp"

using std::cout;
using std::endl;
using std::string;

class PowerArgs : public Arguments {
public:
    PowerArgs(string);


private:
};

PowerArgs::PowerArgs(string cmdLine)
: Arguments(cmdLine)
{
}

#endif

