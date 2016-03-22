/*test_SWIG.h
 $Author: jlconlin $
 $Id: test_SWIG.h 230 2008-01-02 15:12:50Z jlconlin $
 $Revision: 230 $
 $Date: 2008-01-02 08:12:50 -0700 (Wed, 02 Jan 2008) $

 This is the header file for testing and learning SWIG features.

 */
#ifndef TEST_SWIG_H
#define TEST_SWIG_H

#include "Python.h"
#include<vector>

class SWIGTester {
    public:
        SWIGTester(int length);

        void testSWIG();
        std::vector<double> getVector();

    private:
        std::vector<double> _myVector;
        int _length;
};

#endif
