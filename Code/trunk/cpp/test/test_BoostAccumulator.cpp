/*test_BoostAccumulator.cpp

 This file is used for testing/learning about Boost.org Accumulator library

*/

#include<iostream>
#include "boost/accumulators/accumulators.hpp"
#include "boost/accumulators/statistics/stats.hpp"
#include "boost/accumulators/statistics/mean.hpp"
#include "boost/accumulators/statistics/variance.hpp"
#include "boost/numeric/ublas/vector.hpp"
#include "boost/numeric/ublas/io.hpp"
#include "libs/numeric/bindings/lapack/test/utils.h"

using std::cout;
using std::endl;

namespace acc = boost::accumulators;
namespace ublas = boost::numeric::ublas;

void WithUBlas();

int main(){

    cout << "Testing/learning the Boost Accumulator library." << endl;

    acc::accumulator_set<double, acc::stats<acc::tag::mean, 
            acc::tag::variance> > Acc;

    // Add some data
    for( int i = 0; i < 5; ++i ){
        cout << "Adding: " << (i+1) << endl;
        Acc(i+1);
    }

    // Display results
    cout << "\nMean = " << acc::mean(Acc) << endl;
    cout << "Variance = " << acc::variance(Acc) << endl;

//  WithUBlas();
    return 0;
}

void WithUBlas(){
    cout << "\nNow trying with uBlas vectors." << endl;

    ublas::vector<double> vec(5,1.0);
    cout << "vec: " << vec << endl;
    cout << "vec*2: " << vec*2 << endl;

    acc::accumulator_set<ublas::vector<double>, 
        acc::stats<acc::tag::mean> > Acc;

    // Add some data
    for( int i = 0; i < 5; ++i ) Acc( vec*(i+1) );

    // Display results
    cout << "Displaying results\nMean = " << acc::mean(Acc) << endl;



}

