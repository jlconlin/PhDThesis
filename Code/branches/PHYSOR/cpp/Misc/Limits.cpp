/*Limits.cpp
 $Author: jlconlin $
 $Id: Limits.cpp 297 2008-05-09 13:21:30Z jlconlin $
 $Revision: 297 $
 $Date: 2008-05-09 07:21:30 -0600 (Fri, 09 May 2008) $

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
         << endl;
    return 0;
}

