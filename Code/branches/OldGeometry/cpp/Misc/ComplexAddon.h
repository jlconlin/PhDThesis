/*ComplexAddon.h
 $Author: jlconlin $
 $Id: ComplexAddon.h 317 2008-06-25 18:20:23Z jlconlin $
 $Revision: 317 $
 $Date: 2008-06-25 12:20:23 -0600 (Wed, 25 Jun 2008) $

 This header file provides some added functionality for Complex types not 
 included in the standard library.
 */

#ifndef COMPLEXADDON_H
#define COMPLEXADDON_H

#include<complex>
#include<iostream>

using std::complex;

inline bool operator<(complex<double> const a, complex<double> const b){
    std::cout << "compareing complex<double>s" << std::endl;
    if( a.real() ==  b.real() ){
        return a.imag() < b.imag();
    }
    else return a.real() < b.real();
}

#endif

