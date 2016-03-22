/*test_BoostVariant.cpp

 This file is used for testing/learning about Boost.org Variant library

*/

#include<iostream>
#include<string>
#include<vector>
#include "boost/variant.hpp"

using std::cout;
using std::endl;

class times_two_generic : public boost::static_visitor<>{
public:

    template <typename T>
    void operator()( T & operand ) const
    {
        cout << "before +=: " << operand << endl;
        operand += operand;
        cout << "after +=: " << operand << endl;
    }

};

int main(){
    std::vector< boost::variant<int, std::string> > vec;
    vec.push_back( 21 );
    vec.push_back( "hello " );
    vec.push_back( 2.0 );

    for( int i = 0; i < vec.size(); ++i ){
        cout << "i = " << i << ", " << vec[i] << endl;
    }

    times_two_generic visitor;
    std::for_each( vec.begin(), vec.end(), boost::apply_visitor(visitor) );
    
}
