/*Eigenpair.h
 $Author: jlconlin $
 $Id: Eigenpair.h 317 2008-06-25 18:20:23Z jlconlin $
 $Revision: 317 $
 $Date: 2008-06-25 12:20:23 -0600 (Wed, 25 Jun 2008) $

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
            : _Value(0) { _Index = 0; }

        Eigenpair(const T value, unsigned int index )
            : _Value(value) { _Index = index;  }

        Eigenpair<T>& operator= (const Eigenpair<T>& RHS){
            _Value = RHS._Value;
            _Index = RHS._Index;
        }

        bool operator< (const Eigenpair<double>& RHS) const{
            return _Value < RHS._Value; 
        }
        
        T Value() const { return _Value; }
        unsigned int Index() const { return _Index; }

    private:
        T _Value;
        unsigned int _Index;

};

template<typename T>
std::ostream& operator<< (std::ostream& os, Eigenpair<T> Pair){
    os << "<" << Pair.Value() << ", " << Pair.Index() << ">";
    return os;
}

#endif

