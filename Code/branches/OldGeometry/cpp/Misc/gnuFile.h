/*gnuFile.h
 $Author: jlconlin $
 $Id: gnuFile.h 295 2008-05-05 17:23:46Z jlconlin $
 $Revision: 295 $
 $Date: 2008-05-05 11:23:46 -0600 (Mon, 05 May 2008) $

 The gnuFile class is used to write data to a text file that can be read by 
 gnuplot.

 */

#ifndef GNUFILE_H
#define GNUFILE_H

#include<iostream>
#include<fstream>
#include<ios>
#include<string>
#include<vector>
#include<complex>
#include "boost/tuple/tuple.hpp"

using std::vector;
using std::complex;
using std::string;
using std::cout;
using std::endl;
using boost::tuple;
typedef tuple<vector<double>, vector<double>, vector<double> > triVecTuple;
typedef tuple<string, triVecTuple> StrVecTuple;
typedef vector<double> vd;
typedef vector<complex<double> > vcd;

class gnuFile {
    public:
        gnuFile(string FileName)
            : _FileName(FileName), _HeaderExtras()
        {   }

        ~gnuFile();

        // Writing methods
        void appendH(string s){ _HeaderExtras += "# " + s + "\n"; }
        void append(string name, vd& x, vd& y, vd& e);
        void append(string name, vd& x, vd& y);
        void append(string name, vcd& x, vcd& y, vcd& e);
        void append(string name, vcd& x, vcd& y);

    private:
        string _FileName;
        string _HeaderExtras;
        static string _Header;
        vector<StrVecTuple> _Data;

        void _ColumnHeaders(std::ofstream& file);
        void _WriteData(std::ofstream& file);

};

#endif

