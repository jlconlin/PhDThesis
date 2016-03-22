/*test_CPP.cpp
 $Author: jlconlin $
 $Id: test_CPP.cpp 443 2009-01-17 22:30:03Z jlconlin $
 $Revision: 443 $
 $Date: 2009-01-17 15:30:03 -0700 (Sat, 17 Jan 2009) $

 This file is used for testing/learning standard C++.

 */

#include<iostream>
#include<vector>
#include<algorithm>
#include "Utilities.h"

using std::cout;
using std::endl;

void VectorInsert();
void AlgorithmSearching();

int main(int argc, char* argv[]){

//  VectorInsert();
    AlgorithmSearching();

    return 0;
}

void AlgorithmSearching(){

    std::vector<double> v;
    std::vector<double>::iterator lower;
    std::vector<double>::iterator upper;
    for(int i = 0; i < 10; ++i ){
        v.push_back(i);
    }

    double x = 3.5;

}

void VectorInsert(){
    unsigned int n = 5;
    std::vector<double> a(n);
    std::vector<double> b(n);

    unsigned int k = 0;
    for(int i = 0; i < n; ++i) a[i] = ++k;
    for(int i = 0; i < n; ++i) b[i] = ++k;
    
    cout << "a: " << a
         << "\nb: " << b << endl;

    cout << "\nInserting b into a at the beginning." << endl;
    a.insert( a.begin(), b.begin(), b.end() );
    cout << "a: " << a << endl;
}

