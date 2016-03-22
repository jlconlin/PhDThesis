// Date Last Altered: $Date: 2008-08-04 22:55:41 -0600 (Mon, 04 Aug 2008) $
// Revision Number: $Revision: 344 $
//--------------------------------------------*-C++-*------------------------------------//
/*! \file SweepIterator_base.hh
 *  \author Greg Davidson
 *  \date February 6, 2006
 * 
 *  \brief Provides SweepIterator_base, which implements functionality for \c SweepIterators
 *         of all spatial dimensions.
 *
 *  This file provides the SweepIterator_base class, which implements functionality that is
 *  common to \c SweepIterator classes of all spatial dimensions.  The \c SweepIterator type
 *  is used to iterate over all of the \c Zone objects stored in a \c CartesianMesh in a
 *  proper sweep pattern. */

#ifndef SWEEPITERATOR_BASE_HH
#define SWEEPITERATOR_BASE_HH

#include <vector>

#include <boost/utility.hpp>
#include <boost/concept_check.hpp>

#include "Dimension.hh"
#include "IteratorExcept.hh"
#include "Angle_base.hh"
#include "MinimalContainerConcept.hh"

using std::vector;
using boost::ContainerConcept;

/*! \addtogroup MeshMod Mesh Subsystem
 *  @{  */

/// Forward declaration of the \c Zone class template.
template<typename dimension_type> class Zone;

/*! \brief Implements functionality common to \c SweepIterators of all spatial dimensions.
 *
 *  The \c SweepIterator_base class implements functionality that is common to 
 *  \c SweepIterator classes of all spatial dimensions.  The \c SweepIterator type is 
 *  used to iterate over all of the \c Zone objects stored in a \c CartesianMesh in a 
 *  proper sweep pattern.
 *  \par Template Parameters:
 *     <dl> <dt> \e dimension_type </dt>
 *          <dd> The spatial dimension of the \c Zone objects we are sweeping over.
 *               Must satisfy the \c DimensionConcept concept. </dd>
 *          <dt> \e container_type </dt>
 *          <dd> This is the container that the \c CartesianMesh uses to store the
 *               <tt>Zone</tt>s. </dd> </dl> */
template< typename dimension_type, 
          typename container_type = vector< Zone<dimension_type>* > >
class SweepIterator_base : public boost::noncopyable
{
private:
   /// Requires the \c dimension_type to satisfy the \c DimensionConcept concept.
   BOOST_CLASS_REQUIRE(dimension_type, , DimensionConcept);
   /// Requires the \c container_type to satisfy the \c MinimalContainerConcept concept.
   BOOST_CLASS_REQUIRE(container_type, , MinimalContainerConcept);
   /// Define the \c Zone<dimension_type> as a \c ZoneType.
   typedef Zone<dimension_type>          ZoneType;
   
public:
   /// Aliases the \c dimension_type as a \c DimensionType type.
   typedef dimension_type                           DimensionType;
   /// Defines the \c Angle<DimensionType> as an \c AngleType type.
   typedef Angle<DimensionType>                     AngleType;
   /// Defines the \c container_type as a \c ContainerType.
   typedef container_type                           ContainerType;
   /// Defines the \c const_reference type.
   typedef typename ContainerType::const_reference  const_reference;
   
protected:
   /// Default constructor.
   SweepIterator_base()
      : mAngle(), mContainer(), mIterator()
   {  }
   
   /*! \brief Constructor.  Takes in an angle, a container, and a const_iterator.
    *
    *  Constructor.  Takes in an angle, a container, and a const_iterator.
    *  \param[in] angle_in      The angular direction we are sweeping in.
    *  \param[in] container_in  The container we are iterating over.
    *  \param[in] iterator_in   The underlying iterator we are using to iterate with. */
   SweepIterator_base(const AngleType& angle_in, 
                      const ContainerType& container_in,
                      const typename ContainerType::const_iterator& iterator_in)
      : mAngle(angle_in), mContainer( addressof(container_in) ),
        mIterator(iterator_in)
   {  }
   
   /*! \brief Constructor.  Takes in an angle, a pointer to a container, and a 
    *         const_iterator.
    *
    *  Constructor.  Takes in an angle, a pointer to a container, and a const_iterator.
    *  \param[in] angle_in      The angular direction we are sweeping in.
    *  \param[in] container_in  A pointer to the container we are iterating over.
    *  \param[in] iterator_in   The underlying iterator we are using to iterate with. */
   SweepIterator_base(const Angle<DimensionType>& angle_in,
                      const ContainerType* container_ptr_in,
                      const typename ContainerType::const_iterator& iterator_in)
      : mAngle(angle_in), mContainer(container_ptr_in),
        mIterator(iterator_in)
   {  }   

   /// Stores the direction of our sweep.
	AngleType mAngle;
	/// A pointer to the container we are iterating over.
   const ContainerType* mContainer;
   /// The iterator we are using as our underlying iterator.
   typename ContainerType::const_iterator mIterator;
};

/*! \brief General dimensional \c SweepIterator type.  No public constructors.
 *
 *  General dimensional \c SweepIterator type.  This class has no public
 *  constructors, so that SweepIterators of any dimension cannot be
 *  instantiated.  SweepIterators must be instantiated through
 *  the appropriate template specialization.
 *  \par Template Parameters:
 *     <dl> <dt> \e dimension_type </dt>
 *          <dd> The spatial dimension of the \c Zone objects we are sweeping over.
 *               Must satisfy the \c DimensionConcept concept. </dd>
 *          <dt> \e container_type </dt>
 *          <dd> This is the container that the \c CartesianMesh uses to store the
 *               <tt>Zone</tt>s. </dd> </dl> */
template< typename dimension_type, 
          typename container_type = vector< Zone<dimension_type>* > >
class SweepIterator : public boost::noncopyable
{
private:
   /// This default constructor is private to prevent instantiation.
   SweepIterator();
};

/// @}

#endif

