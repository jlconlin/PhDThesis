
#ifndef GENERAL_HPP
#define GENERAL_HPP

#include<iostream>

using std::cout;
using std::endl;

template<class S>
class Base {
public:

    virtual void Express(S x) = 0;

protected:
    Base(S& init) : _bVar(3.7), _SVar(init) {   }
    S _SVar;

private:
    double _bVar;
};

template<class S>
class DaughterTemplate : public Base<S>{
    using Base<S>::_SVar;
public:
    DaughterTemplate(S& init) : Base<S>(init) {   }

    void Express(S x) { 
        cout << "Expressing Daughter with x = " << x 
             << "\n\tBase variables: _SVar = " << _SVar
             << "\n\tDaughter variables: _dVar = " << _dVar << endl;
    }

private:
     S _dVar;

};

class DaughterString : public Base<std::string>{
public:
    DaughterString(std::string& init) : Base<std::string>(init) {   }

    void Express(std::string x) { 
        cout << "Expressing Daughter with x = " << x 
             << "\n\tBase variables: _SVar = " << _SVar
             << "\n\tDaughter variables: _dVar = " << _dVar << endl;
    }

private:
    std::string _dVar;

};

#endif
