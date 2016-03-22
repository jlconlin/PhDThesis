// Date Last Altered: $Date: 2008-12-01 13:34:05 -0700 (Mon, 01 Dec 2008) $
// Revision Number: $Revision: 423 $
//--------------------------------------------*-C++-*------------------------------------//
/*! \file ZoneTest.hh
 *  \author Greg Davidson
 *  \date May 29, 2008
 * 
 *  \brief Provides tests of the Zone class
 * 
 *  This file provides a series of unit tests for the Zone class */
 
#ifndef ZONETEST_HH
#define ZONETEST_HH

#include <sstream>
#include <ostream>
#include <boost/tuple/tuple.hpp>

using boost::tuple;
using std::ostringstream;
using std::cout;

BOOST_AUTO_TEST_SUITE( ZoneTest )

// Test the compile-time checks
BOOST_AUTO_TEST_CASE( CompilerTest )
{
   // Compilation should fail if any of these statements are uncommented...
   //Zone<TwoD> zone;
   
   //Zone_base<OneD> zone_base;
}

// Test the simplest properties of the Zone
BOOST_AUTO_TEST_CASE( ZoneProperties )
{
  typedef Zone<OneD>::LengthType length_type;

  Zone<OneD> zone_a(GeomId< Zone<OneD> >(3), 2.3, 3.5);
  
  BOOST_CHECK_EQUAL(zone_a.id().idNumber(), 3);
  BOOST_CHECK( fabs(zone_a.length() - 2.3) < 10.0*numeric_limits<length_type>::epsilon() );
  BOOST_CHECK( fabs(zone_a.area() - 3.5) < 10.0*numeric_limits<length_type>::epsilon() );
  BOOST_CHECK( fabs(zone_a.volume() - 2.3*3.5) < 10.0*numeric_limits<length_type>::epsilon() );
  
  string id_string = "3";
  ostringstream id_stream_string;
  id_stream_string << zone_a.id();
  BOOST_CHECK_EQUAL( id_string, id_stream_string.str() );
}

// Test the default connectivity and the exception handling...
BOOST_AUTO_TEST_CASE( ZoneDefaultConnectivity )
{
  Zone<OneD> zone_a(GeomId< Zone<OneD> >(5), 1.0, 1.0);

  const tuple< const Node<OneD>*, const Node<OneD>* >& nodes = zone_a.getNodes();
  BOOST_CHECK_EQUAL(nodes.get<0>(), (Node<OneD>*)(0) );
  BOOST_CHECK_EQUAL(nodes.get<1>(), (Node<OneD>*)(0) );
  
  const tuple< const Corner<OneD>*, const Corner<OneD>* >& corners = zone_a.getCorners();
  BOOST_CHECK_EQUAL(corners.get<0>(), (Corner<OneD>*)(0) );
  BOOST_CHECK_EQUAL(corners.get<1>(), (Corner<OneD>*)(0) );
}

// Test the NoNodeConnected Exception
BOOST_AUTO_TEST_CASE( NoNodeConnectedTest )
{
  Zone<OneD> zone_a(GeomId< Zone<OneD> >(7), 1.0, 1.0);
  
  // Can't get real node, because there is none connected, but we can try
  // getting "Node 3"
  try
  {
    zone_a.getNode(3);
  }
  catch(const NoElementConnected< Zone<OneD>, Node<OneD> >& e)
  {
    BOOST_CHECK_EQUAL(e.getSourceId().idNumber(), 7);
    BOOST_CHECK_EQUAL(e.getLocalTargetId(), 3);
    
    string msg = string("Zone< 1D > number 7 attempted to reference a non-existent ")
                 + string("Node< 1D > with local id number 3");
    BOOST_CHECK_EQUAL(e.msg(), msg);
    
    string what = "NoElementConnected< Zone< 1D >, Node< 1D > >";
    BOOST_CHECK_EQUAL(e.what(), what);
  }
}

// Test the NoCornerConnected Exception
BOOST_AUTO_TEST_CASE( NoCornerConnectedTest )
{
  Zone<OneD> zone_a(GeomId< Zone<OneD> >(4), 1.0, 1.0);
  
  // Can't get a real corner because there are none connected, but we can try
  // getting "Corner 3"
  try
  {
    zone_a.getCorner(3);
  }
  catch(const NoElementConnected< Zone<OneD>, Corner<OneD> >& e)
  {
    BOOST_CHECK_EQUAL(e.getSourceId().idNumber(), 4);
    BOOST_CHECK_EQUAL(e.getLocalTargetId(), 3);
    
    string msg = string("Zone< 1D > number 4 attempted to reference a non-existent ")
                 + string("Corner< 1D > with local id number 3");
    BOOST_CHECK_EQUAL(e.msg(), msg);
    
    string what = "NoElementConnected< Zone< 1D >, Corner< 1D > >";
    BOOST_CHECK_EQUAL(e.what(), what);
  }
}

BOOST_AUTO_TEST_SUITE_END()

#endif
  
  

