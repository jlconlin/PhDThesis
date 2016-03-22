/*test_general.cpp

  This file is generally just a scratchpad so I can investigate random things.

 */

#include<iostream>
#include<string>
#include<vector>
#include "general.hpp"

#include "boost/numeric/ublas/vector.hpp"
#include "boost/numeric/ublas/vector_proxy.hpp"
#include "libs/numeric/bindings/lapack/test/utils.h"

using std::cout;
using std::endl;
namespace ublas = boost::numeric::ublas;

int main(int argc, char* argv[] ){
    cout << "test_general running." << endl;

    double x = 0.5;
    std::string str("garbage");

    DaughterTemplate<double> Ddouble(x);
    DaughterTemplate<std::string> Dtemplate(str);
    DaughterString Dstring(str);

    Ddouble.Express(42.0);
    Dtemplate.Express("Hello String");
    Dstring.Express("Hello World");

    std::vector<double> SVD(3,1.0);
    ublas::vector<double, std::vector<double> > UVD(3,2.0);

    print_v(SVD, "SVD");
    print_v(UVD, "UVD");

//  ublas::vector<double, std::vector<double> > UsVD(SVD);
//  print_v(UsVD, "UsVD");

    return 0;
}
