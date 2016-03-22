/*test_uBlas.cpp
 $Author: jlconlin $
 $Id: test_DateTime.cpp 342 2008-08-04 19:55:07Z jlconlin $
 $Revision: 342 $
 $Date: 2008-08-04 13:55:07 -0600 (Mon, 04 Aug 2008) $

 This file is used for testing/learning about the Boost.DateTime library.

 */

#include<iostream>
#include<ctime>
#include "boost/date_time/posix_time/posix_time.hpp"
#include "boost/date_time/microsec_time_clock.hpp"

using std::cout;
using std::endl;
using boost::posix_time::ptime;
namespace dt =  boost::date_time;

int main(){
    cout << "I'm testing/learning about Boost DateTime library." << endl;

    time_t start = time(0);
    ptime now = dt::microsec_clock<ptime>::local_time();

    cout << "microsec_clock::local_time = " 
        << dt::microsec_clock<ptime>::local_time() << endl;
    cout << "second_clock::local_time = " 
        << dt::second_clock<ptime>::local_time() << endl;
    cout << "microsec_clock::universal_time = " 
        << dt::microsec_clock<ptime>::universal_time() << endl;
    cout << "second_clock::universal_time = " 
        << dt::second_clock<ptime>::universal_time() << endl;

    while( (time(0) - start) < 2 );   // Wait for awhile

    ptime later(dt::microsec_clock<ptime>::local_time());
    cout << "now   = " << now 
         << "\nlater = " << later
         << "\n(later-now).total_milliseconds = " 
         << (later-now).total_milliseconds()
         << "\n(now-later).total_milliseconds = " 
         << (now-later).total_milliseconds() << endl;






    return 0;
}
