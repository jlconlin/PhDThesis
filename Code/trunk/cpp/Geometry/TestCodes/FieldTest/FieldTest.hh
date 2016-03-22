// Date Last Altered: $Date: 2008-12-01 13:34:05 -0700 (Mon, 01 Dec 2008) $
// Revision Number: $Revision: 423 $
//----------------------------------*-C++-*------------------------------------//
/*! \file FieldTest.hh
 *  \author Greg Davidson
 *  \date July 1, 2008 */
 
#ifndef FIELDTEST_HH
#define FIELDTEST_HH

#include <algorithm>
#include <list>

using std::list;
using std::copy;

BOOST_AUTO_TEST_SUITE( field_test )

// Test a simple zone field with the default data constructor.
BOOST_AUTO_TEST_CASE( default_simple_zone_field )
{
   CartMesh1D mesh1(1.0, 1.0, 10);
   
   Field< Zone1D, double > field1(mesh1);
   BOOST_CHECK_EQUAL( field1.size(), mesh1.numZones() );
   BOOST_CHECK_EQUAL( field1.max_size(), mesh1.numZones() );
   
   BOOST_CHECK_EQUAL( field1.empty(), false );
}

// Test a simple node field with the default data constructor.
BOOST_AUTO_TEST_CASE( default_simple_node_field )
{
   CartMesh1D mesh1(1.0, 1.0, 10);
   Field< Node1D, int > field1(mesh1);
   BOOST_CHECK_EQUAL( field1.size(), mesh1.numNodes() );
   BOOST_CHECK_EQUAL( field1.max_size(), mesh1.numNodes() );
   
   BOOST_CHECK_EQUAL( field1.empty(), false );
}

// Test a simple corner field with the default data constructor
BOOST_AUTO_TEST_CASE( default_simple_corner_field )
{
   CartMesh1D mesh1(1.0, 1.0, 10);
   Field< Corner1D, bool > field1(mesh1);
   BOOST_CHECK_EQUAL( field1.size(), mesh1.numCorners() );
   BOOST_CHECK_EQUAL( field1.max_size(), mesh1.numCorners() );
   
   BOOST_CHECK_EQUAL( field1.empty(), false );
}

// Test the copy constructor
BOOST_AUTO_TEST_CASE( copy_constructor )
{
   CartMesh1D mesh1(1.0, 1.0, 20);
   Field< Zone1D, int > field1(mesh1, 2);
   
   for(Field<Zone1D, int>::const_iterator iter = field1.begin();
       iter != field1.end(); ++iter)
   {
      BOOST_CHECK_EQUAL( *iter, 2 );
   }
}

// Initialize the field with a vector
BOOST_AUTO_TEST_CASE( vector_constructor )
{
   CartMesh1D mesh1(1.0, 1.0, 20);
   vector<int> a(20, 3);
   Field< Zone1D, int> field1(mesh1, a);

   for(Field<Zone1D, int>::const_iterator iter = field1.begin();
       iter != field1.end(); ++iter)
   {
      BOOST_CHECK_EQUAL( *iter, 3 );
   }
}

// Test the copy operator
BOOST_AUTO_TEST_CASE( copy_operator )
{
   CartMesh1D mesh1(1.0, 1.0, 50);
   Field<Zone1D, int> field1(mesh1, 3);
   Field<Zone1D, int> field2(mesh1, 4);
   
   field1 = field2;
   for(Field<Zone1D, int>::const_iterator iter = field1.begin();
       iter != field1.end(); ++iter)
   {
      BOOST_CHECK_EQUAL( *iter, 4 );
   }
}

// Test the copy operator with different underlying containers
BOOST_AUTO_TEST_CASE( copy_operator2 )
{
   CartMesh1D mesh1(1.0, 1.0, 50);
   Field< Zone1D, int, list<int> > field1(mesh1, 3);
   Field< Zone1D, int> field2(mesh1, 4);
   
   field1 = field2;
   for(Field< Zone1D, int, list<int> >::const_iterator iter = field1.begin();
       iter != field1.end(); ++iter)
   {
      BOOST_CHECK_EQUAL( *iter, 4 );
   }
}

// Test the random access accessor methods.
BOOST_AUTO_TEST_CASE( random_access_test )
{
   CartMesh1D mesh1(1.0, 1.0, 50);
   Field<Zone1D, int> field1(mesh1, 3);

   BOOST_CHECK_EQUAL( field1[ mesh1.getZone( Zone1D::Id(3) ) ], 3 );
   field1[ mesh1.getZone(Zone1D::Id(4)) ] = 4;
   BOOST_CHECK_EQUAL( field1[ mesh1.getZone(Zone1D::Id(4)) ], 4 );
}

// Test the iterators
BOOST_AUTO_TEST_CASE( iterator_test )
{
   vector<int> vec1(30);
   int counter = 1;
   for(vector<int>::iterator iter = vec1.begin(); iter != vec1.end();
       ++counter, ++iter)
   {
      *iter = counter;
   }

   CartMesh1D mesh1(1.0, 1.0, 30);
   Field<Zone1D, int> field1(mesh1, vec1);
   
   counter = 1;
   for(Field<Zone1D, int>::const_iterator field_iter = field1.begin();
       field_iter != field1.end(); ++counter, ++field_iter)
   {
      BOOST_CHECK_EQUAL(*field_iter, counter);
   }
   
   for(Field<Zone1D, int>::iterator field_iter = field1.begin();
       field_iter != field1.end(); ++field_iter)
   {
      *field_iter = 4;
   }
   for(Field<Zone1D, int>::const_iterator field_iter = field1.begin();
       field_iter != field1.end(); ++field_iter)
   {
      BOOST_CHECK_EQUAL(*field_iter, 4);
   }
}
   

BOOST_AUTO_TEST_SUITE_END()

#endif

