/*gnuFileTest.cpp
 $Author: jlconlin $
 $Id: gnuFileTest.cpp 387 2008-10-03 16:47:13Z jlconlin $
 $Revision: 387 $
 $Date: 2008-10-03 10:47:13 -0600 (Fri, 03 Oct 2008) $

 */

#include<iostream>
#include "Utilities.h"
#include "gnuFile.h"
#include "boost/numeric/ublas/vector.hpp"
#include "boost/numeric/ublas/io.hpp"

using std::cout;
using std::endl;
using std::complex;

int main(int argc, char* argv[]){

    gnuFile gFile( "xyz" );
    
    int length = 10;
    
    // std vectors
    std::vector<double> d_ones( length, 1.1);
    std::vector<double> d_zeros( length, 0.1);
    std::vector<double> d_err( length, 0.0003);

    std::vector<int> i_ones( length, 1.1);
    std::vector<int> i_zeros( length, 0.1);
    std::vector<int> i_err( length, 0.0003);

    std::vector<unsigned int> u_ones( length, 1.1);
    std::vector<unsigned int> u_zeros( length, 0.1);
    std::vector<unsigned int> u_err( length, 0.0003);

    std::vector<complex<double> > c_ones( length, complex<double>(1.1,1.2) );
    std::vector<complex<double> > c_zeros( length, complex<double>(0.1,0.2) );
    std::vector<complex<double> > c_err( length, 
            complex<double>(0.0003,0.0004) );

    ublas::vector<double> ud_threes( length, 3.0 );
    ublas::vector<complex<double> > uc_threes( 
            length, complex<double>(3.1,3.2) );

    // Two containers
    gFile.append("svd*2", d_ones, d_zeros);
    gFile.append("svi*2", i_ones, i_zeros);
    gFile.append("svi, svd", i_ones, d_zeros);
    gFile.append("svu*2", u_ones, u_zeros);
    gFile.append("svd, svc", d_ones, c_zeros);
    gFile.append("svc, svd", c_ones, d_zeros);
    gFile.append("svc*2", c_ones, c_zeros);
    gFile.append("uvc, uvd", uc_threes, ud_threes );
    gFile.append("uvc, svd", uc_threes, d_ones);

    // Three standard vectors
    gFile.append("svd*3", d_ones, d_zeros, d_err);

    gFile.append("svc, svd, svd", c_ones, d_zeros, d_err);
    gFile.append("svd, svc, svd", d_ones, c_zeros, d_err);
    gFile.append("svd, svd, svc", d_ones, d_zeros, c_err);

    gFile.append("svc, svc, svd", c_ones, c_zeros, d_err);
    gFile.append("svc, svd, svc", c_ones, d_zeros, c_err);
    gFile.append("svd, svc, svc", d_ones, c_zeros, c_err);

    gFile.append("svc, svc, svc", c_ones, c_zeros, c_err);

    // Three containers
    gFile.append("uvc, svd, uvd", uc_threes, d_zeros, d_err);
    gFile.append("uvd, svd, uvd", ud_threes, d_zeros, d_err);

    cout << "d_ones: " << d_ones << endl;
    cout << "d_zeros: " << d_zeros << endl;
    cout << "d_err: " << d_err << endl;

    return 0;
}

