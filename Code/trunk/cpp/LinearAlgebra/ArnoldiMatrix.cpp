/*ArnoldiMatrix.cpp
 $Author: jlconlin $
 $Id: ArnoldiMatrix.cpp 530 2009-05-08 17:29:28Z jlconlin $
 $Revision: 530 $
 $Date: 2009-05-08 11:29:28 -0600 (Fri, 08 May 2009) $

 This is the implementation of Arnoldi's method for a smiple Matrix.
 */

#include <iostream>
#include <iomanip>
#include <ctime>
#include <cmath>
#include "Arnoldi.h"
#include "ArnoldiMatrix.h"
#include "boost/format.hpp"
#include "boost/numeric/ublas/io.hpp"
#include "RandomLib/NormalDistribution.hpp"

#include "libs/numeric/bindings/lapack/test/utils.h"

using std::cout;
using std::endl;
using std::abs;
using boost::format;

ArnoldiMatrix::T_vec_t ArnoldiMatrix::_Av(){
    if( not _Noise ){   // Add noise to vector
        T_vec_t noiseVec(_length);
        T_vec_t::iterator noiseIter = noiseVec.begin();
        for( ; noiseIter != noiseVec.end(); ++noiseIter ){
            // Add noise, normally distributed with _Noise as standard deviation
            *noiseIter = _NormalD(*_r, 0, _Noise);
        }

        return prod( _A, T_column_t(_V, _k) ) + noiseVec;
    }
    else return prod( _A, T_column_t(_V, _k) );
}

/*_Restart is a restart for Restarted Arnoldi's Method.*/
void ArnoldiMatrix::_Restart(T_vec_t& v){
    ptime now( boost::date_time::microsec_clock<ptime>::local_time() );

    Arnoldi<double>::_Restart(v);
    print_m(_H, "_H");
    print_m(_V, "_V");
    if( _Verbose ){
        double duration = (now-_start).total_microseconds()/1000000.0;
        cout << format( "Restart #: %i, Eigvalue = %10.8f,") 
            %_R %_Eigenvalues( _Eigenvalues.size()-1 )
             << format( " res = %9.3G, time: %6.2G sec." ) %_residual %duration
              << endl;
    }
}

void ArnoldiMatrix::_Iteration(T_vec_t& v){
    ptime now( boost::date_time::microsec_clock<ptime>::local_time() );

    Arnoldi<double>::_Iteration(v);
    if( _Verbose ){
        double duration = (now-_startR).total_microseconds()/1000000.0;
        cout << format( "\tIteration #: %i, time: %6.2G sec." )
                %_k %duration << endl;
    }
    if( abs( _H(_k,_k-1) ) < _Tolerance ){
        _stoppedI = true;
        if( _Verbose ){
            cout << format("Stopping because H[%i,%i] = %8.2E < %8.2E.") 
                %(_k) %(_k-1) %_H(_k,_k-1) %_Tolerance << endl;
        }
    }
}


