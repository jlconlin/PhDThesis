/*Eigenpair.h
 $Author: jlconlin $
 $Id: Eigenpair.h 278 2008-03-28 14:56:02Z jlconlin $
 $Revision: 278 $
 $Date: 2008-03-28 08:56:02 -0600 (Fri, 28 Mar 2008) $

 This is the interface of the Eigenpair class.  The Eigenpair class provides 
 a mechanism for sorting a std::vector of Eigenpairs by the Eigenvalue and 
 keeping the Eigenvector with the Eigenvalue.  It inherits from the std::pair
 class.
 */

#ifndef EIGENPAIR_H
#define EIGENPAIR_H

#include<iostream>
#include<complex>
#include "ComplexAddon.h"

template<typename T>    // T is type stored in Eigenpair
class Eigenpair {
    public:
        Eigenpair()
            : _Value(0) { _Vector = 0; }

        Eigenpair(const T value, T* vector) 
            : _Value(value) { _Vector = vector;  }

        /*Should not delete pointers because they point to objects that are 
        "owned" by something else.*/
        ~Eigenpair() {  }

//      template<T>
        Eigenpair<T>& operator= (const Eigenpair<T>& RHS){
            _Value = RHS._Value;
            _Vector = RHS._Vector;
        }

        bool operator< (const Eigenpair<double>& RHS) const{
            return _Value < RHS._Value; 
        }
        bool operator< (const Eigenpair<std::complex<double> >& RHS) const{
            if( _Value.real() == RHS._Value.real() ) {
                return _Value.imag() < RHS._Value.imag();
            }
            else return _Value.real() < RHS._Value.real(); 
        }
        
        T Value() const { return _Value; }
        T* Vector() const { return _Vector; }

    private:
        T _Value;
        T* _Vector;

};

template<typename T>
std::ostream& operator<< (std::ostream& os, Eigenpair<T> Pair){
    os << "<" << Pair.Value() << ", " << Pair.Vector() << ">";
    return os;
}

#endif

