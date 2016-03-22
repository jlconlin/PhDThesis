/*ArnoldiTest.cpp
 $Author: jlconlin $
 $Id: ArnoldiTest.cpp 401 2008-10-17 21:11:31Z jlconlin $
 $Revision: 401 $
 $Date: 2008-10-17 15:11:31 -0600 (Fri, 17 Oct 2008) $

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
//  arnTest.Orthogonalize();
//  arnTest.CalcEigenPairs();
//  arnTest.SortEigenPairs();

    return 0;
}

