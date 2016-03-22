// Date Last Altered: $Date: 2008-12-01 13:34:05 -0700 (Mon, 01 Dec 2008) $
// Revision Number: $Revision: 423 $
//--------------------------------------------*-C++-*------------------------------------//
/*! \file SimpleComponentTest.hh
 *  \author Greg Davidson
 *  \date May 29, 2008
 * 
 *  \brief Provides tests of simple individual Mesh Module componenets
 * 
 *  This file provides a series of unit tests for the simplest components
 *  in the Mesh Module: the GeomId class, the GeomVector class, and the
 *  Point class. */

#ifndef SIMPLECOMPONENTTEST_HH
#define SIMPLECOMPONENTTEST_HH

#include <sstream>

using std::stringstream;

// ************************************************************
// Test the simplest components of the mesh system
BOOST_AUTO_TEST_SUITE( mesh_component_suite )

// Test the compiler checking
BOOST_AUTO_TEST_CASE( CompilerTest )
{
   // Attempt to compile an Angle<TwoD>.  This should fail to compile
   //Angle<TwoD> angle2d;
   
   // Attempt to compile a GeomVector<TwoD>.  This should fail to compile.
   //GeomVector<TwoD> geomvec2d;
   
   // Attempt to compile a Point<TwoD>.  This should fail to compile.
   //Point<TwoD> point2d;
}


// Test the geometric ids
BOOST_AUTO_TEST_CASE( GeomId_Test )
{
  GeomId< Zone<OneD> > zone_id(5);
  GeomId< Node<OneD> > node_id(3);
  GeomId< Corner<OneD> > corner_id(1);
  
  BOOST_CHECK_EQUAL(zone_id.idNumber(), 5);
  BOOST_CHECK_EQUAL(node_id.idNumber(), 3);
  BOOST_CHECK_EQUAL(corner_id.idNumber(), 1);
  
  stringstream stream_a;
  stream_a << zone_id;
  BOOST_CHECK_EQUAL(stream_a.str(), "5");
  stringstream stream_b;
  stream_b << node_id;
  BOOST_CHECK_EQUAL(stream_b.str(), "3");
  stringstream stream_c;
  stream_c << corner_id;
  BOOST_CHECK_EQUAL(stream_c.str(), "1");
  
  GeomId< GhostElement< Zone<OneD> > >   ghost_zone_id(8);
  GeomId< GhostElement< Node<OneD> > >   ghost_node_id(6);
  GeomId< GhostElement< Corner<OneD> > > ghost_corner_id(3);
  
  BOOST_CHECK_EQUAL(ghost_zone_id.idNumber(), 8);
  BOOST_CHECK_EQUAL(ghost_node_id.idNumber(), 6);
  BOOST_CHECK_EQUAL(ghost_corner_id.idNumber(), 3);
  
  stringstream stream_d;
  stream_d << ghost_zone_id;
  BOOST_CHECK_EQUAL(stream_d.str(), "8(Ghost Zone)");
  stringstream stream_e;
  stream_e << ghost_node_id;
  BOOST_CHECK_EQUAL(stream_e.str(), "6(Ghost Node)");
  stringstream stream_f;
  stream_f << ghost_corner_id;
  BOOST_CHECK_EQUAL(stream_f.str(), "3(Ghost Corner)");
}

// Test the Angle
BOOST_AUTO_TEST_CASE( Angle_Test )
{

   typedef Angle<OneD>::AngleType::OrdinateType    ord_type;
   typedef Angle<OneD>::AngleType                  angle_type;
   typedef Angle<OneD>::WeightType                 weight_type;
   
   // Test the default constructor   
   Angle<OneD> angle_a;
   BOOST_CHECK( fabs(angle_a.mu()) < numeric_limits<ord_type>::epsilon() );
   BOOST_CHECK( fabs(angle_a.weight()) < numeric_limits<weight_type>::epsilon() );

   // Test the constructor with ordinate and weights
   Angle<OneD> angle_b( angle_type(1.0), 1.0);
   BOOST_CHECK( fabs(angle_b.mu() - 1.0 ) < numeric_limits<ord_type>::epsilon() );
   BOOST_CHECK( fabs(angle_b.weight() - 1.0) < numeric_limits<weight_type>::epsilon() );
   BOOST_CHECK( fabs(angle_b.angle().i() - 1.0) < numeric_limits<ord_type>::epsilon() );
   
   // Test the copy constructor
   Angle<OneD> angle_c( angle_b );
   BOOST_CHECK( fabs( angle_c.mu() - angle_b.mu() ) < numeric_limits<ord_type>::epsilon() );
   BOOST_CHECK( fabs( angle_c.weight() - angle_b.weight() ) < numeric_limits<ord_type>::epsilon() );
   BOOST_CHECK( fabs( angle_c.angle().i() - angle_b.angle().i() )
                                    < numeric_limits<ord_type>::epsilon() );
   
   // Test the non-const functionality
   Angle<OneD> angle_d;
   angle_d.mu() = 2.0;
   BOOST_CHECK( fabs(angle_d.mu() - 2.0) < numeric_limits<ord_type>::epsilon() );
   angle_d.weight() = 3.0;
   BOOST_CHECK( fabs(angle_d.weight() - 3.0) < numeric_limits<weight_type>::epsilon() );
   GeomVector<OneD>& angle = angle_d.angle();
   angle.i() = 4.0;
   BOOST_CHECK( fabs(angle_d.mu() - 4.0) < numeric_limits<ord_type>::epsilon() );
}

// Test the GeomVector
BOOST_AUTO_TEST_CASE( GeomVector_Test )
{
  typedef GeomVector<OneD>::OrdinateType ord_type;
  
  // Test default operator
  GeomVector<OneD> geom_vec_a;
  BOOST_CHECK( fabs(geom_vec_a.i()) < numeric_limits<ord_type>::epsilon() );
  // Test non-const i() operator
  geom_vec_a.i() = 5.0;
  BOOST_CHECK( fabs(geom_vec_a.i() - 5.0) < numeric_limits<ord_type>::epsilon() );
  // Test constructor with ordinate
  GeomVector<OneD> geom_vec_b(2.0);
  BOOST_CHECK( fabs(geom_vec_b.i() - 2.0) < numeric_limits<ord_type>::epsilon() );
  // Test copy constructor
  GeomVector<OneD> geom_vec_c( geom_vec_b );
  BOOST_CHECK( fabs(geom_vec_c.i() - 2.0) < numeric_limits<ord_type>::epsilon() );
  // Test copy operator
  GeomVector<OneD> geom_vec_d;
  geom_vec_d = geom_vec_c;
  BOOST_CHECK( fabs(geom_vec_c.i() - geom_vec_d.i()) < numeric_limits<ord_type>::epsilon() );
  // Test in-class dot operator
  ord_type dot1 = geom_vec_b.dot(geom_vec_a);
  BOOST_CHECK( fabs(dot1 - 10.0) < numeric_limits<ord_type>::epsilon() );
  // Test dot function
  ord_type dot2 = dot(geom_vec_a, geom_vec_b);
  BOOST_CHECK( fabs(dot2 - 10.0) < numeric_limits<ord_type>::epsilon() );
}

// Test the point class
BOOST_AUTO_TEST_CASE( Point_Test )
{
  typedef Point<OneD>::PointType pos_type;
  
  // Test default constructor
  Point<OneD> point_a;
  BOOST_CHECK( fabs( point_a.x() ) < numeric_limits<pos_type>::epsilon() );
  // Test non-const x() method
  point_a.x() = 100.1;
  BOOST_CHECK( fabs(point_a.x() - 100.1) < numeric_limits<pos_type>::epsilon() );
  // Test pos_type constructor
  Point<OneD> point_b(5.2);
  BOOST_CHECK( fabs(point_b.x() - 5.2) < numeric_limits<pos_type>::epsilon() );
  // Test copy operator
  Point<OneD> point_c;
  point_c = point_b;
  BOOST_CHECK( fabs(point_c.x() - point_b.x()) < numeric_limits<pos_type>::epsilon() );
  // Test copy constructor
  Point<OneD> point_d( point_c );
  BOOST_CHECK( fabs(point_c.x() - 5.2) < numeric_limits<pos_type>::epsilon() );
}

BOOST_AUTO_TEST_SUITE_END()

#endif

