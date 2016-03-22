/*Utilities.h
 $Author: jlconlin $
 $Id: Utilities.h 316 2008-06-24 14:41:01Z jlconlin $
 $Revision: 316 $
 $Date: 2008-06-24 08:41:01 -0600 (Tue, 24 Jun 2008) $

 This file contains various functions useful across various modules.
 */

#ifndef UTILITIES_H
#define UTILITIES_H

#include<iostream>
#include<iomanip>
#include<complex>
#include<valarray>
#include<vector>
#include<cmath>
#include<algorithm>
#include<cctype>
#include<string>
#include "boost/numeric/ublas/matrix.hpp"

using std::complex;
using std::vector;
using std::valarray;
using std::cout;
using std::endl;
using std::string;

namespace ublas =  boost::numeric::ublas;

/*Statistical Functions*/

/*StdDevMean will calculate the mean and standard deviation of the elements of 
 * the vector v.*/
template<typename T>
inline void StdDevMean(vector<T>& v, T& Mean, T& StdDev){
    int N = v.size();
    Mean = 0.0; StdDev = 0.0;
    T Sum; T Sum2;
    Sums(v, Sum, Sum2);
    // stddev = sqrt( (1/N)(<x^2> - <x>^2 )
    // Need (1/N) because this is a sample variance and not a regular variance?
    Mean = Sum/static_cast<double>(N);
    Sum2 = Sum2/static_cast<double>(N);
    StdDev = std::sqrt( (1.0/N)*( Sum2 - (Mean*Mean) ) );
}

/*Sums will calculate the sum and the sum of the squares of the elements of 
 * the vector v.
Sum: Sum of elements---Returned
Sum2: Sum of square of elements---Returned*/
template<typename T>
inline void Sums(vector<T>& v, T& Sum, T& Sum2){
    T Element;
    Sum = 0.0; Sum2 = 0.0;
    typename vector<T>::iterator vIter = v.begin();
    for( ; vIter != v.end(); ++vIter ){
        Element = *vIter;
        Sum += Element;
        Sum2 += Element*Element;
    }
}

/*End Statistical Functions*/

/*toupper will convert the string s to all uppercase*/
inline void toupper(string& s){

    std::transform(s.begin(), s.end(), s.begin(), (int(*)(int))std::toupper);
}


/*Vector Conversion utitilities*/
/*End Vector Conversion utitilities*/

template<typename T>
inline std::ostream& operator << (std::ostream& out, valarray<T> v){
    int length = v.size();
    out << "[";
    for( int j = 0; j < length; ++j ){
        out << std::setprecision(3) << std::setw(16) << v[j];
    }
    out << "]";

    return out;
}


template<typename T>
inline std::ostream& operator << (std::ostream& out, vector<T> v){
    out << "[";
    typename vector<T>::iterator Iter;
    for( Iter = v.begin(); Iter != v.end(); ++Iter ){
        out << std::setprecision(7) << std::fixed << *Iter << " ";
    }
    out << "]";
    
    return out;
}

/*array printing functions.*/
inline std::ostream& operator<<(std::ostream& out,
        vector<valarray<complex<double> > > v){

    out.setf(std::ios_base::fixed, std::ios_base::floatfield);

    int i = 0;
    vector<valarray<complex<double> > >::iterator vIter;
    for( vIter = v.begin(); vIter != v.end(); ++vIter, ++i ){
        out << *vIter << "\n";
    }
    return out;
}


/*PrintVector will print l elements of the vector v*/
inline void PrintVector(complex<double>* v, unsigned int l){
    cout << "[";
    for( int i = 0; i < l; ++i, ++v){
        cout << *v << ", ";
    }
    cout << "]" << endl;
}
/*array printing functions.*/

/*SplitComplexVector will split a vector<complex<double> > into the real and 
 * imaginary parts of the vector.
   v: Input vector
   real: Real part of complex vector --- returned
   imag: Imaginary part of complex vector --- returned*/
inline void SplitComplexVector(vector<complex<double> >&v, 
        vector<double>& real, vector<double>& imag){
    real = vector<double>(v.size());
    imag = vector<double>(v.size());
    vector<complex<double> >::iterator vIter = v.begin();
    vector<double>::iterator rIter = real.begin();
    vector<double>::iterator iIter = imag.begin();
    for( ; vIter != v.end(); ++vIter, ++rIter, ++iIter ){
        *rIter = vIter->real();
        *iIter = vIter->imag();
    }
}


/*SplitComplexValarray will split a vector<complex<double> > into the real and 
 * imaginary parts of the vector.
   v: Input vector
   real: Real part of complex vector --- returned
   imag: Imaginary part of complex vector --- returned*/
inline void SplitComplexValarray(valarray<complex<double> >&v,
        vector<double>& real,vector<double>& imag){
    int size = v.size();
    real = vector<double>(v.size());
    imag = vector<double>(v.size());
    for( int i = 0; i < size; ++i ){
        real[i] = v[i].real();
        imag[i] = v[i].imag();
    }
}


/*This function will normalize a vector so the sum of the absolute value of
 the parts is 1.0.*/
template<typename T>
inline void Norm(vector<T>& v){
    typename vector<T>::iterator vIter;
    T sum(0.0);
    for( vIter = v.begin(); vIter != v.end(); ++vIter) sum += std::abs(*vIter);
    for( vIter = v.begin(); vIter != v.end(); ++vIter) *vIter /= sum;
}

template<typename T>
inline void Norm(valarray<T>& v){
    T s = std::abs(v).sum();
    v /= s;
}

/*NormColumns will normalize the columns of the ublas matrix A.*/
template <typename T>
void NormColumns(ublas::matrix<T, ublas::column_major>& A){
    typedef ublas::matrix<T, ublas::column_major> Mat_t;
    typedef ublas::matrix_column<Mat_t> Column_t;
    typedef typename Mat_t::size_type size_t;

    for( size_t index = 0, end = A.size2(); index != end; ++index){
        Column_t column( A,index );
        column /= ublas::norm_2(column);
    }
}

#endif

