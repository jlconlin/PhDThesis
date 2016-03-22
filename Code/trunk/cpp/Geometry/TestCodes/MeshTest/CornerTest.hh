// Date Last Altered: $Date: 2008-12-01 13:34:05 -0700 (Mon, 01 Dec 2008) $
// Revision Number: $Revision: 423 $
//--------------------------------------------*-C++-*------------------------------------//
/*! \file CornerTest.hh
 *  \author Greg Davidson
 *  \date June 9, 2008
 * 
 *  \brief Provides tests of the Corner class
 * 
 *  This file provides a series of unit tests for the Corner class */
 
#ifndef CORNERTEST_HH
#define CORNERTEST_HH

#include <sstream>

using std::ostringstream;

BOOST_AUTO_TEST_SUITE( CornerTest )

// Run a series of tests on the Corner class.
BOOST_AUTO_TEST_CASE( CornerProperties )
{
   Corner<OneD> corner( GeomId< Corner<OneD> >(6) );
   BOOST_CHECK_EQUAL(corner.id().idNumber(), 6);
   ostringstream stream;
   stream << corner.id();
   BOOST_CHECK_EQUAL(stream.str(), "6");
}

BOOST_AUTO_TEST_SUITE_END()

#endif

