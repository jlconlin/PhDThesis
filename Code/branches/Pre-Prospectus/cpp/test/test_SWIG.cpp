/*test_SWIG.cpp
 $Author: jlconlin $
 $Id: test_SWIG.cpp 230 2008-01-02 15:12:50Z jlconlin $
 $Revision: 230 $
 $Date: 2008-01-02 08:12:50 -0700 (Wed, 02 Jan 2008) $

 This is the cpp file for testing and learning SWIG features.

 */

#include "iostream"
#include "test_SWIG.h"

SWIGTester::SWIGTester(int length)
    : _length(length)    
{
    for(int i = 0; i < _length; i++ ){
        _myVector.push_back(i+5.0);
    }
    
}


void SWIGTester::testSWIG(){
    std::cout << "I am testing swig in void testSWIG().\n";
}

std::vector<double> SWIGTester::getVector(){
    return _myVector;
}

