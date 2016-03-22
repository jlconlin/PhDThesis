/*gnuFile.cpp
 $Author: jlconlin $
 $Id: gnuFile.cpp 295 2008-05-05 17:23:46Z jlconlin $
 $Revision: 295 $
 $Date: 2008-05-05 11:23:46 -0600 (Mon, 05 May 2008) $

 */
#include<iostream>
#include<sstream>
#include<fstream>
#include<algorithm>
#include "boost/format.hpp"
#include "Assert.hh"
#include "gnuFile.h"
#include "Utilities.h"

using std::vector;
using std::complex;
using std::string;
using std::cout;
using std::endl;

using boost::tie;
bool Compare(const StrVecTuple&, const StrVecTuple&);
using boost::format;

string gnuFile::_Header = 
"# This file is written with the intent of being used to plot the data with \n\
# Gnuplot.  It contains no information regarding how the plot should be \n\
# formatted.\n\
# =========================================================================\n";

gnuFile::~gnuFile(){
    vector<StrVecTuple>::iterator DatIter;
    for( DatIter = _Data.begin(); DatIter != _Data.end(); ++DatIter ){
    }
    std::stable_sort( _Data.begin(), _Data.end(), Compare );
    for( DatIter = _Data.begin(); DatIter != _Data.end(); ++DatIter ){
    }

    std::ofstream out(_FileName.c_str());
    out << _Header << _HeaderExtras << endl;
    _ColumnHeaders(out);
    _WriteData(out);
}


void gnuFile::append(string name, vd& x, vd& y, vd& e){

    Assert(x.size() == y.size() && x.size() == e.size() );
    _Data.push_back(boost::tuples::make_tuple( name, tie(x,y,e) ) );

}


void gnuFile::append(string name, vd& x, vd& y){

    Assert(x.size() == y.size() && x.size() );

    vector<double> e;
    _Data.push_back(boost::tuples::make_tuple(name, tie(x, y, e) ) );


}


void gnuFile::append(string name, vcd& x, vcd& y, vcd& e){

    Assert(x.size() == y.size() && x.size() == e.size() );

    vector<double> xReal; vector<double> xImag;
    SplitComplexVector(x, xReal, xImag);
    vector<double> yReal; vector<double> yImag;
    SplitComplexVector(y, yReal, yImag);
    vector<double> eReal; vector<double> eImag;
    SplitComplexVector(e, eReal, eImag);

    append(name+"-Real", xReal, yReal, eReal);
    append(name+"-Imag", xImag, yImag, eImag);
}


void gnuFile::append(string name, vcd& x, vcd& y){

    Assert(x.size() == y.size() );

    vector<double> xReal; vector<double> xImag;
    SplitComplexVector(x, xReal, xImag);
    vector<double> yReal; vector<double> yImag;
    SplitComplexVector(y, yReal, yImag);

    append(name+"-Real", xReal, yReal);
    append(name+"-Imag", xImag, yImag);
}


/*_ColumnHeaders will write the column headers and column line numbers to the 
 output file.*/
void gnuFile::_ColumnHeaders(std::ofstream& file){
    std::ostringstream colNumbers; colNumbers << "# ";
    std::ostringstream colLabels; colLabels << "# ";
    std::ostringstream dataLabels; dataLabels << "#  ";

    int i = 0;
    vector<StrVecTuple>::iterator DatIter;
    for( DatIter = _Data.begin(); DatIter != _Data.end(); ++DatIter ){
        if( DatIter->get<1>().get<2>().size() == 0){
            colNumbers << format(" %-13i%-13i") % (i+1) % (i+2) ;
            i += 2;
            colLabels << format("|-%=22s-| ") % DatIter->get<0>();
            dataLabels << format("%-13s%-13s ") % "x-coord" % "y-coord";

        }
        else {  // Data has errorbar information
            colNumbers << format(" %-13i%-13i%-13i") % (i+1) % (i+2) % (i+3);
            i +=3;
            colLabels << format("|-%=35s-| ") % DatIter->get<0>();
            dataLabels << format("%-13s%-13s%-13s ") 
                % "x-coord" % "y-coord" % "error";

        }
    }
    
    file <<  colNumbers.str() << "\n" 
         << colLabels.str() << "\n" 
         << dataLabels.str() << endl;

}


void gnuFile::_WriteData(std::ofstream& file){
    
    // Length of largest data vector
    unsigned int length = _Data[0].get<1>().get<0>().size();
    for( unsigned int i = 0; i < length; ++i ){
        file << "   ";
        vector<StrVecTuple>::iterator DatIter = _Data.begin();
        for( ; DatIter != _Data.end(); ++DatIter ){
            triVecTuple segment = DatIter->get<1>();
            if( segment.get<0>().size() > i ){
                if( segment.get<2>().size() > 0 ){
                    file << format("%-13.5G%-13.5G%-13.5G ")
                        % DatIter->get<1>().get<0>()[i]
                        % DatIter->get<1>().get<1>()[i]
                        % DatIter->get<1>().get<2>()[i];
                }
                else{
                    file << format("%-13.5G%-13.5G ")
                        % DatIter->get<1>().get<0>()[i]
                        % DatIter->get<1>().get<1>()[i];
                }
            }
        }
        file << endl;
    }

}


/*Compare compares the length of the vectors. Returns true if a > b.*/
bool Compare(const StrVecTuple& a, const StrVecTuple& b){
    return ( a.get<1>().get<0>().size() > b.get<1>().get<0>().size() );
}
#if 0
        vector<StrVecTuple>::iterator DatIter;
        for( DatIter = _Data.begin(); DatIter != _Data.end(); ++DatIter ){
            triVecTuple DataTuple = DatIter->get<1>();
            // Only print if there is enough data in particular data
            if( DataTuple.get<1>().size() >= length){   
                if( DataTuple.get<2>().size() == 0){
                    file << format("%-12.5G%-12.5G%-12.5G")
                        % DataTuple.get<0>()[i]
                        % DataTuple.get<1>()[i]
                        % DataTuple.get<2>()[i];
                }
                else {  // Data has errorbar information
                    file << format("%-12.5G%-12.5G")
                        % DataTuple.get<0>()[i]
                        % DataTuple.get<1>()[i];
                }
            }
#endif
