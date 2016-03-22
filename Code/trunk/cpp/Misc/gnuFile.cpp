/*gnuFile.cpp
 $Author: jlconlin $
 $Id: gnuFile.cpp 575 2009-06-02 02:19:48Z jlconlin $
 $Revision: 575 $
 $Date: 2009-06-01 20:19:48 -0600 (Mon, 01 Jun 2009) $

 */
#include<iostream>
#include<sstream>
#include<fstream>
#include<algorithm>
#include "boost/format.hpp"
#include "Assert.hh"
#include "gnuFile.h"
#include "Utilities.h"

using std::complex;
using std::cout;
using std::endl;

using boost::format;
bool Compare(const gnuFile::dataTuple&, const gnuFile::dataTuple&);

std::string gnuFile::_Header = 
"# This file is written with the intent of being used to plot the data with \n\
# Gnuplot.  It contains no information regarding how the plot should be \n\
# formatted.\n\
# =========================================================================\n";

gnuFile::~gnuFile(){

    std::stable_sort( _Data.begin(), _Data.end(), Compare );

    std::ofstream out(_FileName.c_str());
    out << _Header << _HeaderExtras << endl;
    _ColumnHeaders(out);
    _WriteData(out);
}

/*_ColumnHeaders will write the column headers and column line numbers to the 
 output file.*/
void gnuFile::_ColumnHeaders(std::ofstream& file){
    std::ostringstream colNumbers; colNumbers << "# ";
    std::ostringstream colLabels; colLabels << "# ";
    std::ostringstream dataLabels; dataLabels << "#  ";

    int i = 0;
    std::vector<dataTuple>::iterator DatIter;
    for( DatIter = _Data.begin(); DatIter != _Data.end(); ++DatIter ){
        if( DatIter->get<3>().size() == 0){
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
    unsigned int length = _Data[0].get<1>().size();
    for( unsigned int i = 0; i < length; ++i ){
        file << "   ";
        std::vector<dataTuple>::iterator DatIter = _Data.begin();
        for( ; DatIter != _Data.end(); ++DatIter ){
            dataTuple data = *DatIter;
            if( data.get<1>().size() > i ){
                if( data.get<3>().size() > 0 ){
                    file << format("%-13.6G%-13.6G%-13.6G ")
                        % data.get<1>()[i]
                        % data.get<2>()[i]
                        % data.get<3>()[i];
                }
                else{   // Data has errorbar information
                    file << format("%-13.6G%-13.6G ")
                        % data.get<1>()[i]
                        % data.get<2>()[i];
                }
            }
        }
        file << endl;
    }

}

/*Compare compares the length of the vectors. Returns true if a > b.*/
bool Compare(const gnuFile::dataTuple& a, const gnuFile::dataTuple& b){
    return ( a.get<1>().size() > b.get<1>().size() );
}
