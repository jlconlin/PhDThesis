/*trivialEigenvalue.cpp
 * This file/code is my beginnings of writing a Monte Carlo Eigenvalue code.  It begins with an infinite
 * homgeneous problem for simplicity
 *
 * created by: Jeremy Conlin
 * Last Modified: September 18, 2006
 * -----------------------------------------------------------------------------------------------------*/
#include<iostream>

#include "History.h"

int main(){
  Particle Neutron(0.0, 0.0, 0.0);
  History eigen(Neutron);
  History value(2.0, 3.0, 4.0);
  std::cout << eigen.Position() << std::endl;
  std::cout << value.Position() << std::endl;

  RNG randomizer;
  std::cout << randomizer.rang() << std::endl;
  std::cout << randomizer.rang() << std::endl;
  std::cout << randomizer.rang() << std::endl;
  std::cout << randomizer.rang() << std::endl;
  std::cout << randomizer.rang() << std::endl;
  std::cout << randomizer.rang() << std::endl;

  return 0;
}
