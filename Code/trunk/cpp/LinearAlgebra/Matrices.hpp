/*Matrices.hpp
 $Author: jlconlin $
 $Id: Matrices.hpp 522 2009-04-24 13:35:29Z jlconlin $
 $Revision: 522 $
 $Date: 2009-04-24 07:35:29 -0600 (Fri, 24 Apr 2009) $

 This file creates several types of matrices for use in other files.
 */

#ifndef MATRICES_HPP
#define MATRICES_HPP

#include<iostream>
#include "boost/numeric/ublas/matrix.hpp"
#include "libs/numeric/bindings/lapack/test/utils.h"

using std::cout;
using std::endl;

namespace ublas =  boost::numeric::ublas;

namespace Matrices{
    /* Diagonal creates a diagonal matrix of increasing values along the
     * diagonal of the matrix
     */
    template<typename T, typename M>
    inline void Diagonal(ublas::matrix<T, M>& A){
        typedef typename ublas::matrix<T, M>::value_type value_type;  
        typedef typename ublas::matrix<T, M>::size_type size_t;        

        A *= 0;
        value_type v = 0.0;
        for( size_t i = 0; (i < A.size1() and i < A.size2() ); ++i ){
            v += 1;
            A(i,i) = v;
        }
    }

    /*Full creates a matrix with all non-zero elements.  The elements are 
     * increase along the columns and then rows.*/
    template<typename T, typename M>
    inline void Full(ublas::matrix<T, M>& A){
        typedef typename ublas::matrix<T, M>::value_type value_type;  
        typedef typename ublas::matrix<T, M>::size_type size_t;        

        value_type v = 0.0;
        for( size_t i = 0; i < A.size1(); ++i ){
            for( size_t j = 0; j < A.size2(); ++j ){
                v += 1.0;

                A(i,j) = v;
            }
        }
    }
    
    /*Standard creates the 'standard' matrix I use for testing.  It is like the
     * Diagonal matrix but also sets the (2,1) element to 1 so the eigenvectors 
     * are more interesting.*/
    template<typename T, typename M>
    inline void Standard(ublas::matrix<T, M>& A){
        Diagonal(A);
        A(2,1) = 1.0;
    }

    /*UHessenberg creates an upper Hessenberg matrix with increasing elements.*/
    template<typename T, typename M>
    inline void UHessenberg(ublas::matrix<T, M>& A){
        typedef typename ublas::matrix<T, M>::value_type value_type;  
        typedef typename ublas::matrix<T, M>::size_type size_t;        

        A *= 0.0;
        value_type k = 0.0;
        for( size_t i = 0; i < A.size1(); ++i ){
            for( size_t j = 0; j < A.size2(); ++j ){
                if( j >= i-1 or i == 0 ){
                    k += 1;
                    A(i, j) = k;
                }
                else A(i,j) = 0.0;
            }
        }
    }

    /*Watkins creates the 2x2 matrix used in 
     *      Fundamentals of Matrix Computations pg. 357*/
    template<typename T, typename M>
    inline void Watkins(ublas::matrix<T,M>& A){
        A.resize(2,2);
        A(0,0) = 8.0;
        A(1,0) = 2.0;
        A(0,1) = 2.0;
        A(1,1) = 5.0;
    }

    /*Moler creates the matrix used in 
     * http://www.mathworks.com/company/newsletters/news_notes/pdf/sum95cleve.pdf*/
    template<typename T, typename M>
    inline void Moler(ublas::matrix<T,M>& A){
        A.resize(3,3);
        A(0,0) = -149; A(0,1) = -50; A(0,2) = -154;
        A(1,0) = 537; A(1,1) = 180; A(1,2) = 546;
        A(2,0) = -27; A(2,1) = -9; A(2,2) = -25;
    }



}

#endif

