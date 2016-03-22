/*test_Eigenpair.cpp
 $Author: jlconlin $
 $Id: test_Eigenpair.cpp 275 2008-03-18 19:56:21Z jlconlin $
 $Revision: 275 $
 $Date: 2008-03-18 13:56:21 -0600 (Tue, 18 Mar 2008) $

 This file is used for testing the eigenpair class.

 */

#include<iostream>
#include<vector>

#include "Eigenpair.h"
using std::cout;

int main() {

    std::vector<Eigenpair<int> > vecPairs;
    
    for( int i = 0; i < 5; ++i ){
        int x = i;
        vecPairs.push_back(Eigenpair<int>(i, &i));
    }

    for(int i = 0; i < 5; ++i ){
        cout << "Getting eigenvalue " << i << ": " << vecPairs[i] << std::endl;
    }
}

