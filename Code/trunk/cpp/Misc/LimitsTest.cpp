/*Limits.cpp
 $Author: jlconlin $
 $Id: LimitsTest.cpp 374 2008-09-09 20:44:16Z jlconlin $
 $Revision: 374 $
 $Date: 2008-09-09 14:44:16 -0600 (Tue, 09 Sep 2008) $

 This file will help me understand what the numerical limts are for a 
 particular machine.
 */

#include<limits>
#include<iostream>
#include<ctime>

using std::cout;
using std::endl;

int main(){
    cout << "Largest int == " << std::numeric_limits<int>::max()
         << "\nLargest unsigned int == " 
         << std::numeric_limits<unsigned int>::max()
         << "\nLargest unsigned long int == " 
         << std::numeric_limits<unsigned long int>::max()
         << "\nLargest unsigned long long int == " 
         << std::numeric_limits<unsigned long long int>::max() << endl;
    cout << "Largest double == " << std::numeric_limits<double>::max() << endl;
    cout << "Largest clock_t == " << std::numeric_limits<clock_t>::max() 
         << ", Largest time == " 
         << std::numeric_limits<clock_t>::max()/CLOCKS_PER_SEC << endl;
    cout << "Largest time_t == " << std::numeric_limits<time_t>::max() 
         << ", Largest time (in hours) == " 
         << std::numeric_limits<time_t>::max()/3600.0 << endl;

    cout << "\nint epsilon == " << std::numeric_limits<int>::epsilon()
         << "\nlong int epsilon == " << std::numeric_limits<long int>::epsilon()
         << "\nunsigned int epsilon == " << std::numeric_limits<unsigned int>::epsilon()
         << "\ndouble epsilon == " << std::numeric_limits<double>::epsilon()
         << endl;
#if 0
    time_t t1, t2, t3, t4, time_max;
    tm tm1, tm2;
    double x = 0.0;

    time_max = std::numeric_limits<time_t>::max();
    t1 = time(0);
    t3 = std::difftime(time_max, time(0));  // Zero out time
    tm1 = *std::localtime(&t1);
    cout << "t1 = " << t1 << " = " << std::asctime(&tm1) << endl;
    cout << "t3 = " << t3 << endl;
    std::cin >> x;
    t2 = time(0);
    t4 = difftime(time_max, time(0));
    tm2 = *std::localtime(&t2);
    cout << "t2 = " << t2 << " = " << std::asctime(&tm2)
         << "\n(t2-t1) = " << std::difftime(t2,t1) 
         << "\ntime_max - (t4-t3) = " << std::difftime(time_max, std::difftime(t4,t3))
         << "\n(t4-t3) - time_max = " << std::difftime(std::difftime(t4,t3), time_max)
         << "\n(t4-t3) = " << std::difftime(t4,t3) << endl;
#endif

    return 0;
}

