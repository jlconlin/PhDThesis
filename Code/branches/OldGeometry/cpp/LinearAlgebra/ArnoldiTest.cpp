/*ArnoldiTest.cpp
 $Author: jlconlin $
 $Id: ArnoldiTest.cpp 314 2008-06-20 14:29:41Z jlconlin $
 $Revision: 314 $
 $Date: 2008-06-20 08:29:41 -0600 (Fri, 20 Jun 2008) $

 This is used to test the Arnoldi class
 */

#include<iostream>
#include "ArnoldiTest.h"

using std::cout;
using std::endl;

int main( int argc, char* argv[] ){
    cout << "I'm testing Arnoldi's method" << endl;

    ArnoldiTest arnTest;

//  arnTest.Normalize();
    arnTest.Orthogonalize();
    arnTest.CalcEigenPairs();

    return 0;
}

