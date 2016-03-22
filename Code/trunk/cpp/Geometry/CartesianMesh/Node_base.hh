// Date Last Altered: $Date: 2008-08-04 22:55:41 -0600 (Mon, 04 Aug 2008) $
// Revision Number: $Revision: 344 $
//--------------------------------------------*-C++-*------------------------------------//
/*! \file: Node_base.hh
 *  \author Greg Davidson
 *  \date June 3, 2005
 * 
 *  \brief Provides the \c Node_base class which implements functionality for \c Node types
 *         of all spatial dimensions.
 *
 *  This file provides the implementation of the \c Node_base class.  This class implements
 *  certain functionality for \c Node types of all spatial dimensions. */

#ifndef NODE_BASE_HH
#define NODE_BASE_HH 

#include <ostream>
#include <boost/utility.hpp>

#include "Dimension.hh"
#include "Point_base.hh"
#include "MeshExcept.hh"
#include "GeomId.hh"
#include "Corner.hh"

using std::ostream;

/*! \addtogroup MeshMod Mesh Subsystem
 *  @{  */

/*! \brief Provides functionality common to \c Node types of all spatial dimensions.
 *
 *  The \c Node_base class provides the functionality common to \c Node types of all
 *  spatial dimensions, which the receive by inheritance.
 *  \par Template Parameters:
 *     <dl> <dt> \e dimension_type </dt>
 *          <dd> Gives the dimension of the spatial space that the \c Node_base
 *               spans.  This type must fulfill the \c DimensionConcept. </dd> </dl> */
template<typename dimension_type>
class Node_base : public boost::noncopyable
{
private:
   /// Requires \c dimension_type to satisfy the \c DimensionConcept concept.
   BOOST_CLASS_REQUIRE(dimension_type, , DimensionConcept);

public:
   /// Aliases the \c dimension_type as \c DimensionType type.
   typedef dimension_type                 DimensionType;
   /// Defines an \c IndexType for indexing connected elements.
	typedef UnsignedInt4                   IndexType;
	/// Defines a \c PointType for storing spatial points.
   typedef Point<DimensionType>           PointType;
   /// Defines an \c Id type for storing the id number for a \c Node object.
	typedef GeomId< Node<DimensionType> >  Id;
	/// Defines a \c Zone type for storing connected \c Zone objects.
	typedef Zone<DimensionType>            Zone;
	/// Defines a \c Corner type for storing connected \c Corner objects.
	typedef Corner<DimensionType>          Corner;

	// ****** Data Accessors ******
	/*! \brief Returns the position of the \c Node object.
	 *
	 *  Returns the position of the \c Node object.
	 *  \return The position of the \c Node. */
	const PointType& position() const 
	{ 
	   return mPosition; 
	}
	/*! \brief Returns the x-position of the \c Node object.
    *
    *  Returns the x-position of the \c Node object.
    *  \return The x-position of the \c Node. */
	typename PointType::PointType x() const 
	{ 
	   return mPosition.x(); 
	}
	/*! \brief Returns the identification number of the \c Node object.
	 *
	 *  Returns a constant reference to the identification number of
	 *  the \c Node object.
	 *  \return The id of the \c Node. */
	const Id& id() const 
	{ 
	   return mId; 
	}
	/*! \brief Prints the identification number to an output stream.
	 *
	 *  Prints the identification number for the \c Node object to
	 *  an output stream.
	 *  \param[in] out  The \c ostream to print to.
	 *  \return A reference to the output stream. */
	virtual ostream& printId(ostream& out) const
	{
	   return out << mId;
	}

protected:
   /*! \brief Constructor.  Takes an \c Id and a \c PointType.
    *
    *  Constructor.  Takes in the \c Id identification number of
    *  the \c Node and the \c PointType position of the \c Node.  This
    *  constructor is protected so that it may only be used by the
    *  \c Node classes that inherit this base class.
    *  \param[in] id_in       The identification number of the \c Node object.
    *  \param[in] position_in The position of the \c Node. */
   Node_base(const Id& id_in, 
             const PointType& position_in)
      : mId(id_in), mPosition(position_in)
   {  }

   /// The \c Node identification number.
   Id mId;
   /// The position of the \c Node.
	PointType mPosition;

private:
   /// The default constructor is private to prevent instantiation.
   Node_base();
};



/*! \brief This \c Node class prevents instantiation of general-dimensional Nodes.
 *
 *  This Node class prevents instantiation of general-dimensional Nodes by
 *  only defining a private constructor. A \c Node must be implemented through
 *  template specializations of this class.
 *  \par Template Parameters:
 *     <dl> <dt> \e dimension_type </dt>
 *          <dd> The spatial dimension that the \c Node spans. </dd> </dl> */
template<typename dimension_type>
class Node : public Node_base<dimension_type>, boost::noncopyable
{  
private:
   /// The default constructor is private to prevent instantiation.   
   Node();
};

///  @}

#endif

