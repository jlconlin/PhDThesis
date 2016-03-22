/*ArnoldiMatrix.cpp
 $Author: jlconlin $
 $Id: ArnoldiMatrix.cpp 342 2008-08-04 19:55:07Z jlconlin $
 $Revision: 342 $
 $Date: 2008-08-04 13:55:07 -0600 (Mon, 04 Aug 2008) $

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

#include "libs/numeric/bindings/lapack/test/utils.h"

using std::cout;
using std::endl;
using std::abs;
using boost::format;

Arnoldi::cVec_t ArnoldiMatrix::_Av(){
    return prod( _A, cColumn_t(_V, _k) );
}

/*_IRestart is a restart for IRAM.*/
void ArnoldiMatrix::_IRestart(cVec_t& v){
    ptime now( boost::date_time::microsec_clock<ptime>::local_time() );

    Arnoldi::_IRestart(v);
    if( _Verbose ){
        double duration = (now-_start).total_microseconds()/1000000.0;
        cout << format( "Restart #: %i, Eigvalue = %10.8f,") 
            %_R %_Eigenvalues( _Eigenvalues.size()-1 )
             << format( " res = %9.3G, time: %6.2G sec." ) %_residual %duration
              << endl;
    }
}

/*_ERestart is a restart for ERAM.*/
void ArnoldiMatrix::_ERestart(cVec_t& v){
    ptime now( boost::date_time::microsec_clock<ptime>::local_time() );

    Arnoldi::_ERestart(v);
    if( _Verbose ){
        double duration = (now-_start).total_microseconds()/1000000.0;
        cout << format( "Restart #: %i, Eigvalue = %10.8f,") 
            %_R %_Eigenvalues( _Eigenvalues.size()-1 )
             << format( " res = %9.3G, time: %6.2G sec." ) %_residual %duration
              << endl;
    }
}

void ArnoldiMatrix::_Iteration(cVec_t& v){
    ptime now( boost::date_time::microsec_clock<ptime>::local_time() );

    Arnoldi::_Iteration(v);
    if( _Verbose ){
        double duration = (now-_startR).total_microseconds()/1000000.0;
        cout << format( "\tIteration #: %i, time: %6.2G sec." )
                %_k %duration << endl;
    }
    if( abs( _H(_k,_k-1) ) < _Tolerance ){
        _stoppedI = true;
        if( _Verbose ){
            cout << format("Stopping because H[%i,%i] = %8.2E < %8.2E.") 
                %_k %(_k-1) %_H(_k,_k-1) %_Tolerance << endl;
        }
    }
}


