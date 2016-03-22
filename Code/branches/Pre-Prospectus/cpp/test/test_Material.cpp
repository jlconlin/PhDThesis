/*test_Particle.cpp
 $Author: jlconlin $
 $Id: test_Material.cpp 184 2007-11-16 19:00:56Z jlconlin $
 $Revision: 184 $
 $Date: 2007-11-16 12:00:56 -0700 (Fri, 16 Nov 2007) $

 This file is used for testing the Material class.

 */

#include<iostream>
#include "Material.h"

using std::cout;


int main(){
    double xS = 0.6;
    double xF = 0.5;
    double xG = 0.0;
    double nu = 2.24;

    Material mat(xS, xF, xG, nu);
    cout << "Here is the material.\n" << mat << std::endl;
    cout << "Here is the copy constructed of the same \n"
        << Material(mat) << std::endl;
    return 0;
}
