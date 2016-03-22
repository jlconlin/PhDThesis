/*Utilities.h
 $Author: jlconlin $
 $Id: Utilities.h 645 2009-06-25 18:36:04Z jlconlin $
 $Revision: 645 $
 $Date: 2009-06-25 12:36:04 -0600 (Thu, 25 Jun 2009) $

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
#include "boost/format.hpp"
#include "boost/numeric/ublas/matrix.hpp"
#include "libs/numeric/bindings/lapack/test/utils.h"

using std::complex;
using std::vector;
using std::valarray;
using std::cout;
using std::endl;
using std::string;
using boost::format;

namespace ublas =  boost::numeric::ublas;

/*Statistical Functions*/

/*StdDevMean will calculate the mean and standard deviation of the elements of 
 * the vector v.*/
template<typename T>
inline void StdDevMean(T& v, typename T::value_type& Mean, 
        typename T::value_type& StdDev){
    int N = v.size();
    Mean = 0.0; StdDev = 0.0;
    typename T::value_type Sum, Sum2;
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
inline void Sums(T& v, typename T::value_type& Sum,
        typename T::value_type& Sum2){
    typename T::value_type Element;
    Sum = 0.0; Sum2 = 0.0;
    typename T::iterator vIter = v.begin();
    for( ; vIter != v.end(); ++vIter ){
        Element = *vIter;
        Sum += Element;
        Sum2 += Element*Element;
    }
}

/*End Statistical Functions*/

/* This function will return the Shannon entropy.
 * Based on the paper by Taro Ueki and Forrest B. Brown, LA-UR-03-3949.
*/
template<typename T>
double ShannonEntropy(const T& vec) {
    double H(0.0);

    // Normalize
    double sum(0.0);
    typename T::const_iterator vecIter = vec.begin();
    for( ; vecIter != vec.end(); ++vecIter ) sum += std::fabs(*vecIter);

    vecIter = vec.begin();
    double value;
    for( ; vecIter != vec.end(); ++vecIter ){
        value = std::fabs(*vecIter)/sum;
        if( value > 1E-14 ) H += value*std::log(value);
        // Add zero if value is zero.
    }

    // Can speed up calculation by converting bases at the end
//  H /= std::log(2);   // Only needed to convert scales

    return -H;
}

/*toupper will convert the string s to all uppercase*/
inline void toupper(string& s){

    std::transform(s.begin(), s.end(), s.begin(), (int(*)(int))std::toupper);
}

template<class T>
void CheckOrthogonality(T& V){
    cout << "Checking Orthogonality." << endl;
    cout << format("V.size1 = %d, V.size2 = %d") %V.size1() %V.size2() << endl;
    T O(V);
    cout << format("O.size1 = %d, O.size2 = %d") %O.size1() %O.size2() << endl;
    O = prod(conj(trans(V)), V );
    print_m(O, "O");

    cout << "Done with Orthgonality." 
         << "=========================================\n\n" << endl;
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
template<typename V, typename R>
inline void SplitComplexVector(const V& v, R& real, R& imag){
    typedef typename V::value_type c_type;
    typedef typename c_type::value_type value_type;

    int size = v.size();
    real.resize(size);
    imag.resize(size);
    for( int i = 0; i < size; ++i ){
        real[i] = v[i].real();
        imag[i] = v[i].imag();
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

