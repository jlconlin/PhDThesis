// Date Last Altered: $Date: 2008-12-01 13:34:05 -0700 (Mon, 01 Dec 2008) $
// Revision Number: $Revision: 423 $
//--------------------------------------------*-C++-*------------------------------------//
/*! \file AngleTest.hh
 *  \author Greg Davidson
 *  \date May 29, 2008
 * 
 *  \brief Includes the unit tests for the \c Angle class.
 * 
 *  This file contains the tests for the Angle class. */
 
#ifndef ANGLETEST_HH
#define ANGLETEST_HH

#include <cmath>
#include <limits>

using std::fabs;
using std::numeric_limits;


BOOST_AUTO_TEST_SUITE( angle_test )

BOOST_AUTO_TEST_CASE( simple_tests )
{
   // Try the default constructor
   Angle1D a1;
   
   BOOST_CHECK( fabs( a1.angle().i() ) 
                  < numeric_limits<Angle1D::AngleType::OrdinateType>::epsilon() );
   BOOST_CHECK( fabs( a1.mu() )
                  < numeric_limits<Angle1D::OrdinateType>::epsilon() );
   BOOST_CHECK( fabs( a1.weight() )
                  < numeric_limits<Angle1D::WeightType>::epsilon() );
   
   // Try setting the mu() and weight() 
   a1.mu() = 1.0;
   a1.weight() = 2.0;

   BOOST_CHECK( fabs( a1.mu() - 1.0)
                  < numeric_limits<Angle1D::OrdinateType>::epsilon() );
   BOOST_CHECK( fabs( a1.weight() - 2.0)
                  < numeric_limits<Angle1D::WeightType>::epsilon() );
}

BOOST_AUTO_TEST_CASE( constructor_test )
{
   // Try the default constructor
   Angle1D a1( GeomVector<OneD>(3.2), 1.7);
   
   BOOST_CHECK( fabs( a1.angle().i() - 3.2) 
                  < numeric_limits<Angle1D::AngleType::OrdinateType>::epsilon() );
   BOOST_CHECK( fabs( a1.mu() - 3.2)
                  < numeric_limits<Angle1D::OrdinateType>::epsilon() );
   BOOST_CHECK( fabs( a1.weight() - 1.7)
                  < numeric_limits<Angle1D::WeightType>::epsilon() );
}

BOOST_AUTO_TEST_CASE( copy_constructor )
{
   Angle1D a1( GeomVector<OneD>(5.2), 1.8);
   Angle1D a2( a1 );
   
   BOOST_CHECK( fabs( a2.angle().i() - 5.2) 
                  < numeric_limits<Angle1D::AngleType::OrdinateType>::epsilon() );
   BOOST_CHECK( fabs( a2.mu() - 5.2)
                  < numeric_limits<Angle1D::OrdinateType>::epsilon() );
   BOOST_CHECK( fabs( a2.weight() - 1.8)
                  < numeric_limits<Angle1D::WeightType>::epsilon() );
}


BOOST_AUTO_TEST_SUITE_END()

#endif

