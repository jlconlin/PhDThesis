// Date Last Altered: $Date: 2008-08-04 22:55:41 -0600 (Mon, 04 Aug 2008) $
// Revision Number: $Revision: 344 $
//----------------------------------*-C++-*------------------------------------//
/*! \file Dimension.hh
 *  \author Greg Davidson
 *  \date August 4, 2007
 *
 *  \brief Provides the \c Dimension class template and \c DimensionConcept 
 *         concept.
 *
 *  This file provides the \c Dimension class template and the 
 *  \c DimensionConcept concept.  The \c Dimension class stores what spatial 
 *  dimension we are working in.  The \c DimensionConcept sets the functionality 
 *  that a \c Dimension type must satisfy. */

#ifndef DIMENSION_HH
#define DIMENSION_HH

#include <boost/static_assert.hpp>
#include <boost/concept_check.hpp>

#include "Types.hh"

using boost::function_requires;
using boost::UnsignedIntegerConcept;

/*! \addtogroup MeshMod Mesh Module
 *  @{  */
 
/// Defines the type that stores the dimension number.
typedef UnsignedInt2  DimensionValue;

/*! \brief Stores what spatial dimension we are working in.
 *
 *  This class depicts what spatial dimension we are working
 *  in.
 *  \par Template Parameters: 
 *     <dl> <dt> \e val </dt>
 *          <dd> This is the numerical value of the spatial dimension.  Valid
 *               values are 1, 2, and 3. </dd> </dl> */
template<DimensionValue val>
class Dimension
{
public:
   /*! \brief This static assert is used to ensure that only spatial
    *         values of 1, 2, or 3 are used. */
   BOOST_STATIC_ASSERT(val <= 3 && val != 0);
   
   /// Define \c type as the type of this class.
   typedef Dimension<val>     type;
   /// Define the type of the spatial dimension value.
   typedef DimensionValue     value_type;
   /// Stores the numerical value of the spatial dimension.
   static const value_type    value = val;
};

/// Defines the \c OneD type as a \c Dimension<1>.
typedef Dimension<1>   OneD;
/// Defines the \c TwoD type as a \c Dimension<2>.
typedef Dimension<2>   TwoD;
/// Defines the \c ThreeD type as a \c Dimension<3>.
typedef Dimension<3>   ThreeD;


/*! \brief The \c DimensionConcept concept class ensures that a type
 *         provides the \c Dimension functionality.
 *
 *  The \c DimensionConcept concept class ensures that a type
 *  provides the functionality necessary to function as a \c Dimension
 *  instantiation.
 *  \par Template Parameters:
 *     <dl> <dt> \e dimension_type </dt>
 *          <dd> This is the type we wish to check for concept 
 *               compatibility. </dd> </dl>
 *  \par Concept Requirements:
 *       The following types must be provided by the \c dimension_type type:
 *       <TABLE BORDER="0" CELLPADDING="0" CELLSPACING="0" WIDTH="100%">
 *          <TR>  <TD> \arg \c type </TD> 
 *                <TD> The type of the \c Dimension template. </TD> </TR>
 *          <TR>  <TD> \arg \c value_type </TD> 
 *                <TD> The value of the spatial dimension. </TD> </TR>
 *       </TABLE>
 *       The following functionality must be provided by the \c dimension_type type:
 *       \arg The \c value_type type must satisfy the \c boost::UnsignedIntegerConcept concept.
 *       \arg The \a value must be between 1 and 3 inclusively.
 *  \remarks  It should be noted that this class is compiled but never executed, so concept
 *            checking does not imply any runtime overhead. */
template<typename dimension_type>
class DimensionConcept
{
public:
   /// Alias the \c dimension_type as a \c DimensionType.
   typedef dimension_type                       DimensionType;
   /// Require the \c DimensionType to provide a \c type.
   typedef typename DimensionType::type         type;
   /// Require the \c DimensionType to provide a \c value_type.
   typedef typename DimensionType::value_type   value_type;

   /*! \brief The constraints method tests that the \c dimension_type 
    *         provides certain functionality. */
   void constraints()
   {
      function_requires< UnsignedIntegerConcept<value_type> >();
      
      BOOST_STATIC_ASSERT(DimensionType::value <= 3 
                              && DimensionType::value != 0);
   }
};

///  @}

#endif

