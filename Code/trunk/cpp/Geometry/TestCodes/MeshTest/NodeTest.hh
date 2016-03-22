// Date Last Altered: $Date: 2008-12-01 13:34:05 -0700 (Mon, 01 Dec 2008) $
// Revision Number: $Revision: 423 $
//--------------------------------------------*-C++-*------------------------------------//
/*! \file NodeTest.hh
 *  \author Greg Davidson
 *  \date June 9, 2008
 * 
 *  \brief Provides tests of the Node class
 * 
 *  This file provides a series of unit tests for the Node class */
 
#ifndef NODETEST_HH
#define NODETEST_HH

#include <sstream>

using std::ostringstream;


BOOST_AUTO_TEST_SUITE( NodeTest )

// Run some tests on the compile-time checks.
BOOST_AUTO_TEST_CASE( CompilerTest )
{
   // If any of these statements are uncommented, the code should fail to compile...
   //Node<TwoD> node;
   
   //Node_base<OneD> node_base;
}

// Run tests on the basic node properties
BOOST_AUTO_TEST_CASE( NodeProperties )
{
   // Check default constructor
   Node<OneD> node1( GeomId< Node<OneD> >(5) );
   BOOST_CHECK_EQUAL(node1.id().idNumber(), 5);
   BOOST_CHECK( fabs(node1.x()) 
                        < 10.0*numeric_limits< Node<OneD>::PointType::PointType >::epsilon() );

   Node<OneD> node2( GeomId< Node<OneD> >(3), Point<OneD>(4.7) );
   
   BOOST_CHECK_EQUAL(node2.id().idNumber(), 3);
   BOOST_CHECK( fabs(node2.x()-4.7) < 10.0*numeric_limits< Node<OneD>::PointType::PointType >::epsilon() );
   const Point<OneD>& point = node2.position();
   BOOST_CHECK( fabs(point.x()-4.7) < 10.0*numeric_limits< Point<OneD>::PointType >::epsilon() );
   
   BOOST_CHECK_EQUAL(node1.onLeftBoundary(), true);
   BOOST_CHECK_EQUAL(node1.onRightBoundary(), true);
   BOOST_CHECK_EQUAL(node1.onBoundary(), true);
   
   ostringstream stream;
   stream << node2.id();
   BOOST_CHECK_EQUAL(stream.str(), "3");
}

// Check the node connectivity
BOOST_AUTO_TEST_CASE( NodeDefaultConnectivity )
{
   Node<OneD> node1(GeomId< Node<OneD> >(4), Point<OneD>(3.2) );
   
   const tuple< const Zone<OneD>*, const Zone<OneD>* >& zones = node1.getZones();
   BOOST_CHECK_EQUAL( zones.get<0>(), static_cast< Zone<OneD>* >(0) );
   BOOST_CHECK_EQUAL( zones.get<1>(), static_cast< Zone<OneD>* >(0) );
   
   const tuple< const Corner<OneD>*, const Corner<OneD>* >& corners = node1.getCorners();
   BOOST_CHECK_EQUAL( corners.get<0>(), static_cast< Corner<OneD>* >(0) );
   BOOST_CHECK_EQUAL( corners.get<1>(), static_cast< Corner<OneD>* >(0) );
}

// Check the NoZoneConnected Exception
BOOST_AUTO_TEST_CASE( NoZoneConnectedTest )
{
   Node<OneD> node1(GeomId< Node<OneD> >(5), Point<OneD>(6.7) );
   
   try
   {
      node1.getZone(3);
   }
   catch(NoElementConnected< Node<OneD>, Zone<OneD> >& e)
   {
      BOOST_CHECK_EQUAL(e.getSourceId().idNumber(), 5);
      BOOST_CHECK_EQUAL(e.getLocalTargetId(), 3);
    
      string msg = string("Node< 1D > number 5 attempted to reference a non-existent ")
                      + string("Zone< 1D > with local id number 3");
      BOOST_CHECK_EQUAL(e.msg(), msg);
    
      string what = "NoElementConnected< Node< 1D >, Zone< 1D > >";
      BOOST_CHECK_EQUAL(e.what(), what);
   }
}

// Check the NoCornerConnected Exception
BOOST_AUTO_TEST_CASE( NoCornerConnectedTest )
{
   Node<OneD> node1(GeomId< Node<OneD> >(7), Point<OneD>(8.2) );
   
   try
   {
      node1.getCorner(3);
   }
   catch(NoElementConnected< Node<OneD>, Corner<OneD> >& e)
   {
      BOOST_CHECK_EQUAL(e.getSourceId().idNumber(), 7);
      BOOST_CHECK_EQUAL(e.getLocalTargetId(), 3);
      
      string msg = string("Node< 1D > number 7 attempted to reference a non-existent ")
                      + string("Corner< 1D > with local id number 3");
      BOOST_CHECK_EQUAL(e.msg(), msg);
    
      string what = "NoElementConnected< Node< 1D >, Corner< 1D > >";
      BOOST_CHECK_EQUAL(e.what(), what);
   }
}


BOOST_AUTO_TEST_SUITE_END()

#endif

