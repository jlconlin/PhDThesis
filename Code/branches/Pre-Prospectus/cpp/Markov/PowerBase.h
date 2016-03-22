/*PowerBase.h
 $Author: jlconlin $
 $Id: PowerBase.h 239 2008-01-07 17:47:47Z jlconlin $
 $Revision: 239 $
 $Date: 2008-01-07 10:47:47 -0700 (Mon, 07 Jan 2008) $

 The PowerBase class is intended to be used as a Python base object.  It is
 written in C++ to facilitate using the score function.
 */

#ifndef POWERBASE_H
#define POWERBASE_H

#include<vector>
#include "Markov.h"
#include "BankSource.h"

class PowerBase : public Markov<BankSource> {
    public:
        PowerBase(const std::vector<unsigned long>& seed,
               const Field<Zone, Material>& field)
            : Markov<BankSource>(seed, field) { k = 1; }

        ~PowerBase() {  }
        // Scoring function
        void score(BankSource& source, Particle& P);

        // Members used in Python
        double k;   //Eigenvalue estimate
};

/*score will score in the source from P.*/
void PowerBase::score(BankSource& source, Particle& P){
    double xi = _r.Fixed();
    const Material& mat = _field[*(P.zone())];
    int N = P.weight()*(1.0/k)*(mat.nu()*mat.xF()/mat.xT()) + xi;
    for( int i = 0; i< N; i++ ){ source.push_back(new Particle(P)); }
}
#endif
