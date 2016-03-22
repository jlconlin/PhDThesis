/*PowerMCBase.h
 $Author: jlconlin $
 $Id: PowerMCBase.h 243 2008-01-08 20:25:20Z jlconlin $
 $Revision: 243 $
 $Date: 2008-01-08 13:25:20 -0700 (Tue, 08 Jan 2008) $

 The PowerMCBase class is intended to be used as a Python base object.  It is
 written in C++ to facilitate using the score function.
 */

#ifndef POWERMCBASE_H
#define POWERMCBASE_H

#include<vector>
#include "Markov.h"
#include "BankSource.h"

class PowerMCBase : public Markov<BankSource> {
    public:
        PowerMCBase(const std::vector<unsigned long>& seed,
               const Field<Zone, Material>& field)
            : Markov<BankSource>(seed, field) { k = 1; }

        ~PowerMCBase() {  }
        // Scoring function
        void score(BankSource& source, Particle& P);

        // Members used in Python
        double k;   //Eigenvalue estimate
};

/*score will score in the source from P.*/
void PowerMCBase::score(BankSource& source, Particle& P){
    double xi = _r.Fixed();
    const Material& mat = _field[*(P.zone())];
    int N = P.weight()*(1.0/k)*(mat.nu()*mat.xF()/mat.xT()) + xi;
    for( int i = 0; i< N; i++ ){ source.push_back(new Particle(P)); }
}
#endif
