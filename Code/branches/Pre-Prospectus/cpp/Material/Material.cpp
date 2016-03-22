/*Material.cpp
 $Author: jlconlin $
 $Id: Material.cpp 187 2007-11-19 17:26:14Z jlconlin $
 $Revision: 187 $
 $Date: 2007-11-19 10:26:14 -0700 (Mon, 19 Nov 2007) $

    This file provides the implementation for the Material class.
 */

#include "Material.h"
//Constructors
Material::Material()
    : _xS(0.5), _xF(0.5), _xG(0.0), _nu(1.0),
      _xA(_xF + _xG), _xT(_xA + _xF) {

}

Material::Material(double xS, double xF, double xG, double nu)
    : _xS(xS), _xF(xF), _xG(xG), _nu(nu),
      _xA(_xF + _xG), _xT(_xA + _xF) {
}

Material::Material(const Material& M)
    : _xS(M.xS()), _xF(M.xF()), _xG(M.xG()), _nu(M.nu()),
      _xA(_xF + _xG), _xT(_xA + _xF) {
}
