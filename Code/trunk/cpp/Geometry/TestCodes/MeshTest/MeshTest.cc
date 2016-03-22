// Date Last Altered: $Date: 2008-12-01 13:34:05 -0700 (Mon, 01 Dec 2008) $
// Revision Number: $Revision: 423 $
//--------------------------------------------*-C++-*------------------------------------//
/*! \file MeshTest.cc
 *  \author Greg Davidson
 *  \date May 29, 2008
 * 
 *  \brief Includes all of the unit tests for the Mesh Module
 * 
 *  This file runs all of the unit tests for the Mesh Module by including all
 *  of the test suites defined in the include files. */


#define BOOST_TEST_MODULE MeshTest
#include <boost/test/included/unit_test.hpp>

#include "CartesianMesh1D.hh"

// Begin including the test suites...

// *** Test the simplest and most fundamental mesh components ***
#include "SimpleComponentTest.hh"
// *** Test the Zone class ***
#include "ZoneTest.hh"
// *** Test the Node class ***
#include "NodeTest.hh"
// *** Test the Corner class ***
#include "CornerTest.hh"
// *** Test the Mesh altogether ***
#include "MeshTest.hh"

