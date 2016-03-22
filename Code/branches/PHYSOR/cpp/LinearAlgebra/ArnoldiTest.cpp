/*ArnoldiTest.cpp
 $Author: jlconlin $
 $Id: ArnoldiTest.cpp 281 2008-04-02 18:48:26Z jlconlin $
 $Revision: 281 $
 $Date: 2008-04-02 12:48:26 -0600 (Wed, 02 Apr 2008) $

 This is used to test the Arnoldi class
 */

#include<iostream>
#include "ArnoldiTest.h"

using std::cout;
using std::endl;

int main( int argc, char* argv[] ){
    cout << "I'm testing Arnoldi's method" << endl;

    ArnoldiTest ArnTest;

    ArnTest.EigenSorting();
    ArnTest.MakeRitzPairs();
    ArnTest.Normalize();
    ArnTest.Orthogonalize();
    return 0;
}

