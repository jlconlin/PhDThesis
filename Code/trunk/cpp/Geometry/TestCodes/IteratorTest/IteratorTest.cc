// Date Last Altered: $Date: 2008-12-01 13:34:05 -0700 (Mon, 01 Dec 2008) $
// Revision Number: $Revision: 423 $
//--------------------------------------------*-C++-*------------------------------------//
/*! \file IteratorTest.cc
 *  \author Greg Davidson
 *  \date May 29, 2008
 * 
 *  \brief Includes all of the unit tests for the Iterator and ReverseIterator
 *         functionality.
 * 
 *  This file runs all of the unit tests for the Iterator and ReverseIterator template
 *  classes. */


#define BOOST_TEST_MODULE IteratorTest
#include <boost/test/included/unit_test.hpp>

#include "Iterator.hh"
#include "ReverseIterator.hh"

// Begin including the test suites...
// *** Test the iterator on a vector ***
#include "IteratorVectorTest.hh"

// *** Test the reverse iterator on a vector ***
#include "ReverseIteratorTest.hh"


