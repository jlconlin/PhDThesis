/*gnuFileTest.cpp
 $Author: jlconlin $
 $Id: gnuFileTest.cpp 289 2008-04-28 21:57:25Z jlconlin $
 $Revision: 289 $
 $Date: 2008-04-28 15:57:25 -0600 (Mon, 28 Apr 2008) $

 */

#include<iostream>
#include "Utilities.h"
#include "gnuFile.h"

using std::cout;
using std::endl;
using std::vector;
using std::complex;

int main(int argc, char* argv[]){

    gnuFile gFile( string("xyz") );
    
    int length = 10;
    vector<double> ones( length, 1.0);
    vector<double> zeros( length, 0.0);
    vector<double> err( length, 0.0003);

    vector<complex<double> > cOnes( length*2, 1.0);
    vector<complex<double> > cZeros( length*2, 0.0);

    vector<complex<double> > cIncrease( length+5 );
    vector<complex<double> > cDecrease( length+5 );
    vector<complex<double> > cConstant( length+5 );

    int i = 1;
    vector<complex<double> >::iterator IncIter = cIncrease.begin();
    vector<complex<double> >::iterator DecIter = cDecrease.begin();
    vector<complex<double> >::iterator ConIter = cConstant.begin();
    for( ; IncIter != cIncrease.end(); ++IncIter, ++DecIter, ++i ){
        *IncIter = complex<double>(i, length - i);
        *DecIter = complex<double>(length - i, i);
    }

    gFile.append(string("complex"), cOnes, cZeros);
    gFile.append(string("double-err"), ones, zeros, err);
    gFile.append(string("increment"), cIncrease, cDecrease, cConstant);

    cout << "ones: " << ones << endl;
    cout << "zeros: " << zeros << endl;
    cout << "err: " << err << endl;
    cout << "cOnes: " << cOnes << endl;
    cout << "cZeros: " << cZeros << endl;
    cout << "Increase: " << cIncrease << endl;
    cout << "Decrease: " << cDecrease << endl;
    cout << "Constant: " << cConstant << endl;
    return 0;
}

