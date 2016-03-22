// Date Last Altered: $Date: 2008-08-04 22:55:41 -0600 (Mon, 04 Aug 2008) $
// Revision Number: $Revision: 344 $
//----------------------------------*-C++-*------------------------------------//
/*! \file Corner.hh
 *  \author Greg Davidson
 *  \date August 22, 2006
 *
 *  \brief Provides the \c Corner geometric element.
 *
 *  This file provides the \c Corner template class.  The \c Corner class does 
 *  not provide any geometric information, but does provide a centering-type 
 *  for the \c Field class.  A \c Corner possesses connectivity to a \c Zone 
 *  and a \c Node. */

#ifndef CORNER_HH
#define CORNER_HH

#include <ostream>

#include "Dimension.hh"
#include "GeomId.hh"

using std::ostream;

/// Forward declaration of the \c Zone template class.
template<typename dimension_type> class Zone;
/// Forward declaration of the \c Node template class.
template<typename dimension_type> class Node;

/*!  \addtogroup MeshMod Mesh Subsystem
 *   @{  */

/*! \brief The \c Corner class provides a <tt>Field</tt>-centering geometric element.
 *
 *  The \c Corner class does not store any information about the problem domain, but provides 
 *  a new centering for use with the \c Field class.  Typically, <tt>Corner</tt>s are imagined 
 *  to sit at the location of a \c Node, but just inside the \c Zone.  Therefore, a 
 *  one-dimensional \c Zone will have two <tt>Corner</tt>s.
 *  \par Template Parameters:
 *     <dl> <dt> \e dimension_type </dt>
 *          <dd> The spatial dimension that the \c Corner spans.  This type must satisfy the
 *               \c DimensionConcept concept. </dd> </dl> */
template<typename dimension_type>
class Corner
{
private:
   /// Requires the \c dimension_type satisfies the \c DimensionConcept concept.
   BOOST_CLASS_REQUIRE(dimension_type, , DimensionConcept);

public:
   // ****** Define the public types for the Corner. ******
   /// Alias the \c dimension_type as a \c DimensionType.
   typedef dimension_type                   DimensionType;
   /// Defines an \c Id type.
	typedef GeomId< Corner<DimensionType> >  Id;
   /// Defines a \c Zone type.
	typedef Zone<DimensionType>              Zone;
	/// Defines the \c Node type.
	typedef Node<DimensionType>              Node;

	// ****** Constructor *******
	/*! \brief Constructor.  Takes in the \c Corner id number.
	 *
	 *  Constructor.  Takes in the \c Corner id number.
	 *  \param[in] id_in  The id number for the \c Corner. */
	explicit Corner(const Id& id_in)
			: mId(id_in), mZone(0), mNode(0)
	{  }
	
	// ****** Data Accessors ******
	/*! \brief Returns the \c Id identification class.
	 *
	 *  Returns the \c Id for this \c Corner.
	 *  \return The \c Id for this \c Corner. */
	const Id& id() const 
	{ 
	   return mId;
	}
	/*! \brief Prints the \c Id for this \c Corner to the \c std::ostream.
	 *
	 *  Prints the \c Id for this \c Corner to the \c std::ostream.
	 *  \param[in] out The output stream to receive the output. */
	virtual ostream& printId(ostream& out) const
	{
	   return out << mId;
	}

	// ****** Connectivity Accessors ******
	/*! \brief Returns a reference to the connected \c Zone.
	 *
	 *  Returns a reference to the connected \c Zone.
	 *  \return A reference to the connected \c Zone. */
	const Zone& getZone() const
	{
	   Assert(mZone != 0);
		return *mZone;
	}
	/*! \brief Returns a reference to the connected \c Node.
	 *
	 *  Returns a reference to the connected \c Node.
	 *  \return A reference to the connected \c Node. */
	const Node& getNode() const
	{
	   Assert(mNode != 0);
		return *mNode;
	}
	
private:
   /// \c CartesianMesh needs to be a friend to set the \c Corner connectivity.
	friend class CartesianMesh<DimensionType>;
   /// The \c Id for this \c Corner.
   Id mId;
   /// A pointer to the \c Zone that this \c Corner is connected to.
	const Zone* mZone;
	/// A pointer to the \c Node that this \c Corner is connected to.
	const Node* mNode;
};

///  @}

#endif

