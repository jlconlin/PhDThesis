// Date Last Altered: $Date: 2008-12-01 13:34:05 -0700 (Mon, 01 Dec 2008) $
// Revision Number: $Revision: 423 $
//--------------------------------------------*-C++-*------------------------------------//
/*! \file IteratorVectorTest.cc
 *  \author Greg Davidson
 *  \date May 29, 2008
 * 
 *  \brief Runs a series of tests on an Iterator over a std::vector.
 * 
 *  Runs a series of tests on an Iterator over a std::vector. */

#ifndef ITERATORVECTORTEST_HH
#define ITERATORVECTORTEST_HH

#include <vector>
#include <string>

using std::vector;
using std::string;


BOOST_AUTO_TEST_SUITE( IteratorVectorTest )

/// Tests the compile-time checks on the Iterator.
BOOST_AUTO_TEST_CASE( IteratorConceptChecking )
{
   // This function will attempt to invoke the concept checking.
   // If any of the following lines are uncommented, it should
   // not compile.

   // Create an iterator using an int for the container
   // Iterator< int > iter1;
   // Create a vector using an int for an iterator
   // Iterator< vector<double>, int > iter2;
  
   // Test the concept checking for the equal method
   // Iterator< vector<double> > iter1;
   // int a;
   // iter1 == a;
}

// Check default and Container constructor only.  Check basic operations
BOOST_AUTO_TEST_CASE( IteratorVectorTest1 )
{
   // Create and fill a simple vector
   vector<unsigned int> a(10);
   for(unsigned int i=0; i < a.size(); ++i)  a[i] = i;
  
   // Create an iterator over this vector (default constructor)
   Iterator< vector<unsigned int> > iter1;
  
   // Create an iterator set to vector a.  Check operator*, =, and ++
   Iterator< vector<unsigned int> > iter2(a);
   iter2 = a.begin();
   for(vector<unsigned int>::const_iterator vec_iter = a.begin(); vec_iter != a.end(); 
       ++vec_iter)
   {
      BOOST_CHECK_EQUAL(*vec_iter, *iter2);
      ++iter2;
   }
   // Check operator-- and operator==
   iter2 = a.end()-1;
   for(vector<unsigned int>::const_iterator vec_iter = a.end()-1; vec_iter != a.begin(); 
       --vec_iter)
   {
      // Compare different iterators
      BOOST_CHECK(iter2 == vec_iter);
      BOOST_CHECK_EQUAL(*vec_iter, *iter2);
      --iter2;
   }
  
   // Check advance
   iter2 = a.begin();
   iter2+= 5;
   BOOST_CHECK(iter2 == a.begin()+5);
  
   // Check subtraction
   Iterator< vector<unsigned int> > iter3(a, a.end());
   Iterator< vector<unsigned int> >::difference_type d1 = iter2-iter3;
   BOOST_CHECK_EQUAL(d1, a.size()-5);
   Iterator< vector<unsigned int> >::difference_type d2 = iter3-a.begin();
   BOOST_CHECK_EQUAL(d2, a.size());
} 

// Check reassigning containers
BOOST_AUTO_TEST_CASE( IteratorVectorTest2 )
{
   // Create and fill a simple vector
   vector<unsigned int> a(10);
   for(unsigned int i=0; i < a.size(); ++i)  a[i] = i;

   // Create and fill another simple vector
   vector<unsigned int> b(20);
   for(unsigned int i=0; i < b.size(); ++i)  b[i] = i+100;

   // Assign an iterator to a
   Iterator< vector<unsigned int> > iter1(a);
   // Get the container
   const vector<unsigned int>& a_ref = iter1.container();
   BOOST_CHECK( addressof(a) == addressof(a_ref) );
  
   // Reassign the iterator to b
   iter1.setContainer(b);
   const vector<unsigned int>& b_ref = iter1.container();
   BOOST_CHECK( addressof(b) == addressof(b_ref) );
}

// Test const_Iterators.
BOOST_AUTO_TEST_CASE( IteratorConstTesting )
{
   // Create and fill a simple vector
   vector<unsigned int> a(10);
   for(unsigned int i=0; i < a.size(); ++i)  a[i] = i;

   // Create a const iterator
   Iterator< vector<unsigned int> > const_iter(a, a.begin());
   // Create a non-const iterator
   Iterator< vector<unsigned int>, vector<unsigned int>::iterator > iter(a, a.begin());
  
   // Refill a with the non-const iterator
   for(; iter != a.end(); ++iter)
   {
      *iter = 5;
   }
   // Check that the values were really changed
   for(vector<unsigned int>::const_iterator vec_iter = a.begin(); vec_iter != a.end();
       ++vec_iter)
   {
      BOOST_CHECK_EQUAL( *vec_iter, 5 );
   }
   //  Attempt to change a with the const vector.  This should not compile!!
   //for(; const_iter != a.end(); ++const_iter)   *const_iter = 5;
  
   // Ensure that comparisons between const and non-const are possible
   iter = a.begin();
   const_iter = a.begin();
   BOOST_CHECK(iter == const_iter);
   ++const_iter;
   BOOST_CHECK(iter != const_iter);
   BOOST_CHECK(iter > const_iter);
   // Ensure that assignment to iterator is possible
   const_iter = iter;
   BOOST_CHECK(iter == const_iter);  
}

// Test the iterator exceptions.
BOOST_AUTO_TEST_CASE( IteratorExceptionCase )
{
   const vector<double> a(10, 1.0);
   Iterator< vector<double> > iter;
   // Test the CheckedIteratorLacksContainer exception.
   try
   {
      iter = a.begin();
   }
   catch(const Iterator< vector<double> >::CheckedIteratorLacksContainer& e)
   {
      BOOST_CHECK(string(e.msg()) == 
                  "Attempted to manipulate a checked iterator that does not point to a container");
      BOOST_CHECK(string(e.what()) == "CheckedIteratorLacksContainer");
   }
   // Test the AssignmentOutOfRange exception.
   iter.setContainer(a);
   try
   {
      iter = a.begin()-1;
   }
   catch(const Iterator< vector<double> >::AssignmentOutOfRange& e)
   {
      BOOST_CHECK(string(e.msg()) == "Attempted to assign an out-of-range iterator");
      BOOST_CHECK(string(e.what()) == "AssignmentOutOfRange");
   }
   // Test the IncrementBeyondRange exception.
   iter = a.end();
   try
   {
      ++iter;
   }
   catch(const Iterator< vector<double> >::IncrementBeyondRange& e)
   {
      BOOST_CHECK(string(e.msg()) 
                     == "Attempted to increment an iterator beyond Container::end()");
      BOOST_CHECK(string(e.what()) == "IncrementBeyondRange");
   }
   // Test the DecrementBeyondRange exception.
   iter = a.begin();
   try
   {
      --iter;
   }
   catch(const Iterator< vector<double> >::DecrementBeyondRange& e)
   {
      BOOST_CHECK(string(e.msg()) 
                     == "Attempted to decrement an iterator beyond Container::begin()");
      BOOST_CHECK(string(e.what()) == "DecrementBeyondRange");
   }
   // Test the AdvanceBeyondRange exception.
   iter = a.begin();
   try
   {
      iter+=11;
   }
   catch(const Iterator< vector<double> >::AdvanceBeyondRange& e)
   {
      BOOST_CHECK(string(e.msg()) == string("Attempted to advance a distance 11 which moved the iterator")
                                                   + " out of the Container's range");
      BOOST_CHECK(string(e.what()) == "AdvanceBeyondRange");
   }
}

BOOST_AUTO_TEST_SUITE_END()


#endif
  
