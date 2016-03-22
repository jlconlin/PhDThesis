// Date Last Altered: $Date: 2008-12-01 13:34:05 -0700 (Mon, 01 Dec 2008) $
// Revision Number: $Revision: 423 $
//--------------------------------------------*-C++-*------------------------------------//
/*! \file ReverseIteratorTest.cc
 *  \author Greg Davidson
 *  \date May 29, 2008
 * 
 *  \brief Runs a series of tests on a ReverseIterator.
 * 
 *  Runs a series of tests on a ReverseIterator, where an Iterator over a std::vector
 *  is used as the forward iterator. */


#ifndef REVERSEITERATORTEST_HH
#define REVERSEITERATORTEST_HH

#include <vector>
#include "Iterator.hh"

using std::cout;
using std::endl;

using std::vector;

BOOST_AUTO_TEST_SUITE( ReverseIteratorTest )

/// Test the ReverseIterator constructors.
BOOST_AUTO_TEST_CASE( ConstructReverseIterators )
{
   // Define a forward iterator
   vector<double> a(10, 1.0);
   Iterator< vector<double> > forward_iter(a, a.begin());
   
   // Define a boost::reverse_iterator type
   typedef boost::reverse_iterator< Iterator< vector<double> > > rev_iter;
   
   // Create several boost::reverse_iterators
   rev_iter reverse_iter1;
   rev_iter reverse_iter2(forward_iter);
   rev_iter reverse_iter3(reverse_iter2);
   BOOST_CHECK(reverse_iter2 == reverse_iter3);
   
   rev_iter reverse_iter4( Iterator< vector<double> >(a, a.end() ) );
   
   // Create a ReverseIterator, and use a boost::reverse_iterator to construct it.
   ReverseIterator< Iterator< vector<double> > > reverse_iter5(a, a.rbegin());   
   
   BOOST_CHECK( reverse_iter4 == reverse_iter5 );      

   // Execute the increment and decrement methods on the forward iterator.
   ++forward_iter;
   forward_iter++;
   --forward_iter;
   forward_iter--;
   
   // Execute the increment and decrement methods on the ReverseIterator.
   ++reverse_iter5;
   reverse_iter5++;
   --reverse_iter5;
   reverse_iter5--;
}

// Test a ReverseIterator inside a loop.
BOOST_AUTO_TEST_CASE( TestReverseIteratorLoops )
{
   // Fill a test vector.
   vector<double> a(10);
   for(unsigned int i = 0; i < 10; ++i) a[i] = i;
   
   // Create a ReverseIterator type.
   typedef ReverseIterator<Iterator<vector<double> > > RevIter;
   
   // Check the ReverseIterator in a loop.
   unsigned int counter = 9;
   RevIter iter(a, a.rbegin());
   for(; iter != a.rend(); ++iter)
   {
      BOOST_CHECK_EQUAL(*iter, counter);
      --counter;
   }
}


BOOST_AUTO_TEST_SUITE_END()

#endif
   
