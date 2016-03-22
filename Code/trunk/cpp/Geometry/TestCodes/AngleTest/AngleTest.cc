// Date Last Altered: $Date: 2008-12-01 13:34:05 -0700 (Mon, 01 Dec 2008) $
// Revision Number: $Revision: 423 $
//--------------------------------------------*-C++-*------------------------------------//
/*! \file AngleTest.cc
 *  \author Greg Davidson
 *  \date May 29, 2008
 * 
 *  \brief Includes all of the unit tests for the Angular Quadrature Module
 * 
 *  This file runs all of the unit tests for the Field Module by including all
 *  of the test suites defined in the include files. */

#define BOOST_TEST_MODULE AngleTest
#include <boost/test/included/unit_test.hpp>

#include "Angle1D.hh"
#include "AngleQuad1D.hh"
#include "AngleVector1D.hh"



// Test the Angle1D class
#include "AngleTest.hh"

// Test the Angular Quadrature
#include "AngleQuadTest.hh"

// Test the AngleVector class
#include "AngleVectorTest.hh"


