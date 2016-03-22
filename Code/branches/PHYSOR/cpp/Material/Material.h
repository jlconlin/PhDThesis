/*Material.h
 $Author: jlconlin $
 $Id: Material.h 230 2008-01-02 15:12:50Z jlconlin $
 $Revision: 230 $
 $Date: 2008-01-02 08:12:50 -0700 (Wed, 02 Jan 2008) $

    This file provides the C++ interface for the Material class.  

    The Material class holds all the cross section information for a material.
 */
#ifndef MATERIAL_H
#define MATERIAL_H

#include<iostream>
#include<sstream>

class Material{
    public:
        Material();
        Material(double xS, double xF, double xG, double nu);

        //Copy constructor
        Material(const Material& M);

        ~Material() {   };

        double xS() const { return _xS; }
        double xF() const { return _xF; }
        double xG() const { return _xG; }
        double xA() const { return _xA; }
        double xT() const { return _xT; }
        double nu() const { return _nu; }

        //String representation of Material
        std::string stringRep() const;

    private:
        double _xS; //Scattering cross section
        double _xF; //Fission cross section
        double _xG; //Capture cross section
        double _nu; //Average number of fission neutrons per fission
        double _xA; //Absorption cross section
        double _xT; //Total cross section
};



inline std::string Material::stringRep() const{
    std::ostringstream S;
    S << "[(nu: " << _nu << ", xF: "  << _xF << ", xG: " << _xG
      <<", xS: " << _xS << ") , xA: " << _xA << ", xT: " << _xT << "]";

    return S.str();
}

inline std::ostream& operator<<(std::ostream& os, const Material& M){
    os << M.stringRep();
    return os;
}
#endif

