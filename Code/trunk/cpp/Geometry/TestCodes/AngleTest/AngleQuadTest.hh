// Date Last Altered: $Date: 2008-12-01 13:34:05 -0700 (Mon, 01 Dec 2008) $
// Revision Number: $Revision: 423 $
//--------------------------------------------*-C++-*------------------------------------//
/*! \file AngleQuadTest.hh
 *  \author Greg Davidson
 *  \date July 26, 2008
 * 
 *  \brief Includes the unit tests for the \c AngleQuad class.
 * 
 *  This file contains the tests for the AngleQuad class. */


#ifndef ANGLEQUADTEST_HH
#define ANGLEQUADTEST_HH

#include <cmath>
#include <limits>

using std::sqrt;
using std::numeric_limits;


BOOST_AUTO_TEST_SUITE( anglequad_test )

// Test the S2 quadrature set.
BOOST_AUTO_TEST_CASE( s2_test )
{
   AngleQuad1D s2_quad(2);
   // Basic functionality
   BOOST_CHECK_EQUAL(s2_quad.order(), 2);
   BOOST_CHECK_EQUAL(s2_quad.numAngles(), 2);
   BOOST_CHECK( fabs(s2_quad.rho() - 1.0/sqrt(3.0) )
                           < 10.0*numeric_limits<AngleQuad1D::OrdinateType>::epsilon() );
   // Check the ordinates.
   BOOST_CHECK( fabs(s2_quad.mu(0) + 1.0/sqrt(3.0) )
                     < 10.0*numeric_limits<AngleQuad1D::OrdinateType>::epsilon() );
   BOOST_CHECK( fabs(s2_quad.mu(1) - 1.0/sqrt(3.0) )
                     < 10.0*numeric_limits<AngleQuad1D::OrdinateType>::epsilon() );
   BOOST_CHECK( fabs(s2_quad.weight(0) - 1.0 )
                     < 10.0*numeric_limits<AngleQuad1D::OrdinateType>::epsilon() );
   BOOST_CHECK( fabs(s2_quad.weight(1) - 1.0 )
                     < 10.0*numeric_limits<AngleQuad1D::OrdinateType>::epsilon() );
}

// Test the S4 quadrature set.
BOOST_AUTO_TEST_CASE( s4_test )
{
   AngleQuad1D s4_quad(4);
   // Basic functionality
   BOOST_CHECK_EQUAL(s4_quad.order(), 4);
   BOOST_CHECK_EQUAL(s4_quad.numAngles(), 4);

   // Check the ordinates
   BOOST_CHECK( fabs(s4_quad.mu(1) + 0.339981043584856)
                     < 10.0*numeric_limits<AngleQuad1D::OrdinateType>::epsilon() );
   BOOST_CHECK( fabs(s4_quad.mu(2) - 0.339981043584856)
                     < 10.0*numeric_limits<AngleQuad1D::OrdinateType>::epsilon() );                     
   BOOST_CHECK( fabs(s4_quad.mu(3) - 0.861136311594053)
                     < 10.0*numeric_limits<AngleQuad1D::OrdinateType>::epsilon() );
   BOOST_CHECK( fabs(s4_quad.weight(0) - 0.347854845137454)
                     < 10.0*numeric_limits<AngleQuad1D::OrdinateType>::epsilon() );
   BOOST_CHECK( fabs(s4_quad.weight(1) - 0.652145154862546)
                     < 10.0*numeric_limits<AngleQuad1D::OrdinateType>::epsilon() );
   BOOST_CHECK( fabs(s4_quad.weight(2) - 0.652145154862546)
                     < 10.0*numeric_limits<AngleQuad1D::OrdinateType>::epsilon() );
   BOOST_CHECK( fabs(s4_quad.weight(3) - 0.347854845137454)
                     < 10.0*numeric_limits<AngleQuad1D::OrdinateType>::epsilon() );
}


BOOST_AUTO_TEST_SUITE_END()

#endif


