/*gnuFile.h
 $Author: jlconlin $
 $Id: gnuFile.h 401 2008-10-17 21:11:31Z jlconlin $
 $Revision: 401 $
 $Date: 2008-10-17 15:11:31 -0600 (Fri, 17 Oct 2008) $

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
#include "Assert.hh"
#include "boost/tuple/tuple.hpp"
#include "boost/numeric/ublas/vector.hpp"
#include "boost/utility.hpp"
#include "Utilities.h"

using std::complex;
using std::cout;
using std::endl;
using boost::tuple;

namespace ublas = boost::numeric::ublas;

class gnuFile {
    public:
        typedef std::vector<double> svd;
        typedef std::vector<complex<double> > svc;
        typedef ublas::vector<double> uvd;
        typedef ublas::vector<complex<double> > uvc;

        typedef tuple<std::string, svd, svd, svd> dataTuple;

        gnuFile(std::string FileName)
            : _FileName(FileName), _HeaderExtras()
        {   }

        ~gnuFile();

        // Writing methods
        void appendH(std::string s){ _HeaderExtras += "# " + s + "\n"; }
        void append(std::string, svd&, svd&);
        void append(std::string, svd&, svd&, svd&);

// Three arguments
        template<typename X, typename Y, typename E>    // Neither X, Y, or E are complex
        void append(std::string, X&, Y&, E&,
                typename boost::enable_if<boost::is_convertible<
                    typename X::value_type, double> >::type* = 0,
                typename boost::enable_if<boost::is_convertible<
                    typename Y::value_type, double> >::type* = 0,
                typename boost::enable_if<boost::is_convertible<
                    typename E::value_type, double> >::type* = 0 );
        template<typename X, typename Y, typename E>    // Only X is complex
        void append(std::string, X&, Y&, E&,
                typename boost::enable_if<boost::is_complex<
                    typename X::value_type> >::type* = 0,
                typename boost::enable_if<boost::is_convertible<
                    typename Y::value_type, double> >::type* = 0,
                typename boost::enable_if<boost::is_convertible<
                    typename E::value_type, double> >::type* = 0 );
        template<typename X, typename Y, typename E>    // Only Y is complex
        void append(std::string, X&, Y&, E&,
                typename boost::enable_if<boost::is_convertible<
                    typename X::value_type, double> >::type* = 0,
                typename boost::enable_if<boost::is_complex<
                    typename Y::value_type> >::type* = 0,
                typename boost::enable_if<boost::is_convertible<
                    typename E::value_type, double> >::type* = 0 );
        template<typename X, typename Y, typename E>    // Only E is complex
        void append(std::string, X&, Y&, E&,
                typename boost::enable_if<boost::is_convertible<
                    typename X::value_type, double> >::type* = 0,
                typename boost::enable_if<boost::is_convertible<
                    typename Y::value_type, double> >::type* = 0,
                typename boost::enable_if<boost::is_complex<
                    typename E::value_type> >::type* = 0 );
        template<typename X, typename Y, typename E>    // Only X and Y are complex
        void append(std::string, X&, Y&, E&,
                typename boost::enable_if<boost::is_complex<
                    typename X::value_type> >::type* = 0,
                typename boost::enable_if<boost::is_complex<
                    typename Y::value_type> >::type* = 0,
                typename boost::enable_if<boost::is_convertible<
                    typename E::value_type, double> >::type* = 0 );
        template<typename X, typename Y, typename E>    // Only X and E are complex
        void append(std::string, X&, Y&, E&,
                typename boost::enable_if<boost::is_complex<
                    typename X::value_type> >::type* = 0,
                typename boost::enable_if<boost::is_convertible<
                    typename Y::value_type, double> >::type* = 0,
                typename boost::enable_if<boost::is_complex<
                    typename E::value_type> >::type* = 0 );
        template<typename X, typename Y, typename E>    // Only Y and E are complex
        void append(std::string, X&, Y&, E&,
                typename boost::enable_if<boost::is_convertible<
                    typename X::value_type, double> >::type* = 0,
                typename boost::enable_if<boost::is_complex<
                    typename Y::value_type> >::type* = 0,
                typename boost::enable_if<boost::is_complex<
                    typename E::value_type> >::type* = 0 );
        template<typename X, typename Y, typename E>    // X, Y, and E are all complex
        void append(std::string, X&, Y&, E&,
                typename boost::enable_if<boost::is_complex<
                    typename X::value_type> >::type* = 0,
                typename boost::enable_if<boost::is_complex<
                    typename Y::value_type> >::type* = 0,
                typename boost::enable_if<boost::is_complex<
                    typename E::value_type> >::type* = 0 );

// Two arguments
        template<typename X, typename Y>    // Neither X nor Y are complex
        void append(std::string, X&, Y&, 
                typename boost::enable_if<boost::is_convertible<
                    typename X::value_type, double> >::type* = 0,
                typename boost::enable_if<boost::is_convertible<
                    typename Y::value_type, double> >::type* = 0 );
        template<typename X, typename Y>    // Only Y is complex
        void append(std::string, X&, Y&, 
                typename boost::enable_if<boost::is_convertible<
                    typename X::value_type, double> >::type* = 0,
                typename boost::enable_if<boost::is_complex<
                    typename Y::value_type> >::type* = 0 );
        template<typename X, typename Y>    // Only X is  complex
        void append(std::string, X&, Y&, 
            typename boost::enable_if<boost::is_complex<
                typename X::value_type> >::type* = 0,
            typename boost::enable_if<boost::is_convertible<
                typename Y::value_type, double> >::type* = 0 );
        template<typename X, typename Y>    // Both X and Y are complex
        void append(std::string, X&, Y&, 
            typename boost::enable_if<boost::is_complex<
                typename X::value_type> >::type* = 0,
            typename boost::enable_if<boost::is_complex<
                typename Y::value_type> >::type* = 0 );

    private:
        std::string _FileName;
        std::string _HeaderExtras;
        static std::string _Header;
        std::vector<dataTuple> _Data;

        void _ColumnHeaders(std::ofstream& file);
        void _WriteData(std::ofstream& file);

};

inline void gnuFile::append(std::string name, svd& x, svd& y){

    std::vector<double> e;
    _Data.push_back(boost::tuples::make_tuple( name,x,y,e ) );
}
  
// Three arguments
// Neither X, Y, or E are complex
template<typename X, typename Y, typename E>    
void gnuFile::append(std::string name, X& x, Y& y, E& e,
        typename boost::enable_if<boost::is_convertible<
            typename X::value_type, double> >::type*,
        typename boost::enable_if<boost::is_convertible<
            typename Y::value_type, double> >::type*,
        typename boost::enable_if<boost::is_convertible<
            typename E::value_type, double> >::type* ){

    Assert(x.size() == y.size() && x.size() );
    Assert(x.size() == e.size() );

    std::vector<double> sx( x.begin(), x.end() );
    std::vector<double> sy( y.begin(), y.end() );
    std::vector<double> se( e.begin(), e.end() );
    append( name, sx, sy, se );
}

// Only X is complex
template<typename X, typename Y, typename E>    
void gnuFile::append(std::string name, X& x, Y& y, E& e,
        typename boost::enable_if<boost::is_complex<
            typename X::value_type> >::type*,
        typename boost::enable_if<boost::is_convertible<
            typename Y::value_type, double> >::type*,
        typename boost::enable_if<boost::is_convertible<
            typename E::value_type, double> >::type* ){

    Assert(x.size() == y.size() && x.size() );
    Assert(x.size() == e.size() );

    std::vector<double> xReal, xImag;
    SplitComplexVector(x, xReal, xImag);

    std::vector<double> sy( y.begin(), y.end() );
    std::vector<double> se( e.begin(), e.end() );
    append( name+"-Real", xReal, sy, se );
    append( name+"-Imag", xImag, sy, se );
}

// Only Y is complex
template<typename X, typename Y, typename E>    
void gnuFile::append(std::string name, X& x, Y& y, E& e,
        typename boost::enable_if<boost::is_convertible<
            typename X::value_type, double> >::type*,
        typename boost::enable_if<boost::is_complex<
            typename Y::value_type> >::type*,
        typename boost::enable_if<boost::is_convertible<
            typename E::value_type, double> >::type* ){

    Assert(x.size() == y.size() && x.size() );
    Assert(x.size() == e.size() );

    std::vector<double> yReal, yImag;
    SplitComplexVector(y, yReal, yImag);

    std::vector<double> sx( x.begin(), x.end() );
    std::vector<double> se( e.begin(), e.end() );
    append( name+"-Real", sx, yReal, se );
    append( name+"-Imag", sx, yImag, se );
}

// Only E is complex
template<typename X, typename Y, typename E>    
void gnuFile::append(std::string name, X& x, Y& y, E& e,
        typename boost::enable_if<boost::is_convertible<
            typename X::value_type, double> >::type*,
        typename boost::enable_if<boost::is_convertible<
            typename Y::value_type, double> >::type*,
        typename boost::enable_if<boost::is_complex<
            typename E::value_type> >::type* ){

    Assert(x.size() == y.size() && x.size() );
    Assert(x.size() == e.size() );

    std::vector<double> eReal, eImag;
    SplitComplexVector(e, eReal, eImag);

    std::vector<double> sx( x.begin(), x.end() );
    std::vector<double> sy( y.begin(), y.end() );
    append( name+"-Real", sx, sy, eReal );
    append( name+"-Imag", sx, sy, eImag );
}

// Only X and Y are complex
template<typename X, typename Y, typename E>    
void gnuFile::append(std::string name, X& x, Y& y, E& e,
        typename boost::enable_if<boost::is_complex<
            typename X::value_type> >::type*,
        typename boost::enable_if<boost::is_complex<
            typename Y::value_type> >::type*,
        typename boost::enable_if<boost::is_convertible<
            typename E::value_type, double> >::type* ){

    Assert(x.size() == y.size() && x.size() );
    Assert(x.size() == e.size() );

    std::vector<double> xReal, xImag;
    std::vector<double> yReal, yImag;
    SplitComplexVector(x, xReal, xImag);
    SplitComplexVector(y, yReal, yImag);

    std::vector<double> se( e.begin(), e.end() );
    append( name+"-Real", xReal, yReal, se );
    append( name+"-Imag", xImag, yImag, se );
}

// Only X and E are complex
template<typename X, typename Y, typename E>    
void gnuFile::append(std::string name, X& x, Y& y, E& e,
        typename boost::enable_if<boost::is_complex<
            typename X::value_type> >::type*,
        typename boost::enable_if<boost::is_convertible<
            typename Y::value_type, double> >::type*,
        typename boost::enable_if<boost::is_complex<
            typename E::value_type> >::type* ){

    Assert(x.size() == y.size() && x.size() );
    Assert(x.size() == e.size() );

    std::vector<double> xReal, xImag;
    std::vector<double> eReal, eImag;
    SplitComplexVector(x, xReal, xImag);
    SplitComplexVector(e, eReal, eImag);

    std::vector<double> sy( y.begin(), y.end() );
    append( name+"-Real", xReal, sy, eReal );
    append( name+"-Imag", xImag, sy, eImag );
}

// Only Y and E are complex
template<typename X, typename Y, typename E>    
void gnuFile::append(std::string name, X& x, Y& y, E& e,
        typename boost::enable_if<boost::is_convertible<
            typename X::value_type, double> >::type*,
        typename boost::enable_if<boost::is_complex<
            typename Y::value_type> >::type*,
        typename boost::enable_if<boost::is_complex<
            typename E::value_type> >::type* ){

    Assert(x.size() == y.size() && x.size() );
    Assert(x.size() == e.size() );

    std::vector<double> yReal, yImag;
    std::vector<double> eReal, eImag;
    SplitComplexVector(y, yReal, yImag);
    SplitComplexVector(e, eReal, eImag);

    std::vector<double> sx( x.begin(), x.end() );
    append( name+"-Real", sx, yReal, eReal );
    append( name+"-Imag", sx, yImag, eImag );
}

// X, y, and E are all complex
template<typename X, typename Y, typename E>    
void gnuFile::append(std::string name, X& x, Y& y, E& e,
        typename boost::enable_if<boost::is_complex<
            typename X::value_type> >::type*,
        typename boost::enable_if<boost::is_complex<
            typename Y::value_type> >::type*,
        typename boost::enable_if<boost::is_complex<
            typename E::value_type> >::type* ){

    Assert(x.size() == y.size() && x.size() );
    Assert(x.size() == e.size() );

    std::vector<double> xReal, xImag;
    std::vector<double> yReal, yImag;
    std::vector<double> eReal, eImag;
    SplitComplexVector(x, xReal, xImag);
    SplitComplexVector(y, yReal, yImag);
    SplitComplexVector(e, eReal, eImag);

    append( name+"-Real", xReal, yReal, eReal );
    append( name+"-Imag", xImag, yImag, eImag );
}


// Two arguments
template<typename X, typename Y>    // Neither X nor Y are complex
inline void gnuFile::append(std::string name, X& x, Y& y,
    typename boost::enable_if<boost::is_convertible<
        typename X::value_type, double> >::type*,
    typename boost::enable_if<boost::is_convertible<
        typename Y::value_type, double> >::type* ){

    Assert(x.size() == y.size() && x.size() );

    std::vector<double> sx( x.begin(), x.end() );
    std::vector<double> sy( y.begin(), y.end() );
    append( name, sx, sy );
}

template<typename X, typename Y>    // Only Y is  complex
inline void gnuFile::append(std::string name, X& x, Y& y, 
    typename boost::enable_if<boost::is_convertible<
        typename X::value_type, double> >::type*,
    typename boost::enable_if<boost::is_complex<
        typename Y::value_type> >::type* ){

    Assert(x.size() == y.size() && x.size() );

    std::vector<double> yReal, yImag;
    SplitComplexVector(y, yReal, yImag);

    append( name+"-Real", x, yReal );
    append( name+"-Imag", x, yImag );
}

template<typename X, typename Y>    // Only X is  complex
inline void gnuFile::append(std::string name, X& x, Y& y, 
    typename boost::enable_if<boost::is_complex<
        typename X::value_type> >::type*,
    typename boost::enable_if<boost::is_convertible<
        typename Y::value_type, double> >::type* ){

    Assert(x.size() == y.size() && x.size() );

    std::vector<double> xReal, xImag;
    SplitComplexVector(x, xReal, xImag);

    append( name+"-Real", xReal, y );
    append( name+"-Imag", xImag, y );
}

template<typename X, typename Y>    // Both X and Y are complex
inline void gnuFile::append(std::string name, X& x, Y& y, 
    typename boost::enable_if<boost::is_complex<
        typename X::value_type> >::type*,
    typename boost::enable_if<boost::is_complex<
        typename Y::value_type> >::type* ){

    Assert(x.size() == y.size() && x.size() );

    std::vector<double> xReal, xImag;
    std::vector<double> yReal, yImag;
    SplitComplexVector(x, xReal, xImag);
    SplitComplexVector(y, yReal, yImag);

    append( name+"-Real", xReal, yReal );
    append( name+"-Imag", xImag, yImag );
}

inline void gnuFile::append(std::string name, svd& x, svd& y, svd& e){

    Assert(x.size() == y.size() && x.size() == e.size() );
    _Data.push_back(boost::tuples::make_tuple( name,x,y,e ) );

}

#endif

