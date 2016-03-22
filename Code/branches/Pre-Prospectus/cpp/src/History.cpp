//History.cpp

#include<cmath>
#include "History.h"

//Constructors
History::History(Particle Neutron): mNeutron(Neutron) {
}

History::History(double x, double y, double z): mNeutron(x, y, z){
}


int History::Absorption(double SigmaF, double SigmaG, double SigmaS, double nu, double k){
  //This function returns the number of neutrons released after an absorption.  If > 0, we have fission.
  double interaction = mRandom.rang();
  if (interaction*(SigmaG + SigmaF) > SigmaG){ //We have fission
    return std::floor(nu/k + mRandom.rang());
  }
  else{ //We have absorption
    return 0;
  };

    
}
