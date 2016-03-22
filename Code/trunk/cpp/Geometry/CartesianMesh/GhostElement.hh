// Date Last Altered: $Date: 2008-08-04 22:55:41 -0600 (Mon, 04 Aug 2008) $
// Revision Number: $Revision: 344 $
//--------------------------------------------*-C++-*------------------------------------//
/*! \file GhostElement.hh
 *  \author Greg Davidson
 *	 \date October 1, 2007
 *
 *  \brief Provides the \c GhostElement class template.
 *
 *  This file provides the \c GhostElement template.  This template inherits from a geometric
 *  element (such as \c Zone, \c Node, or \c Corner) and implements additional functionality to
 *  act as a ghost element. */

#ifndef GHOSTELEMENT_HH
#define GHOSTELEMENT_HH

#include <boost/utility.hpp>
#include <boost/concept_check.hpp>

#include "Dimension.hh"
#include "GeomId.hh"
#include "GeometricElementConcept.hh"

using boost::addressof;

/*! \addtogroup MeshMod Mesh Module
 *  @{  */

/// Forward declaration of \c Zone class.
template<typename dimension_type> class Zone;
/// Forward declaration of \c Node class.
template<typename dimension_type> class Node;
/// Forward declaration of \c Corner class.
template<typename dimension_type> class Corner;
/// Forward declaration of \c GhostElement class.
template<typename geom_element> class GhostElement;

// ****** Forward declarations of the output operators ******
/// Prints the \c GhostElement<Zone>::Id identification number.
template<typename dimension_type>
ostream& operator<<(ostream& out, 
                           const GeomId< GhostElement< Zone<dimension_type> > >& id);
/// Prints the \c GhostElement<Node>::Id identification number.
template<typename dimension_type>
ostream& operator<<(ostream& out, 
                           const GeomId< GhostElement< Node<dimension_type> > >& id);
/// Prints the \c GhostElement<Corner>::Id identification number.
template<typename dimension_type>
ostream& operator<<(ostream& out, 
                           const GeomId< GhostElement< Corner<dimension_type> > >& id);


/*! \brief Provides the \c GhostElement template class for creating ghost elements.
 *
 *  This class provides the \c GhostElement template class. This class creates
 *  ghost zones, ghost nodes, or ghost corners, depending on what is passed in as
 *  the template argument.  The \c GhostElement template also inherits from its
 *  template type, thereby giving it all of the functionality of its parent type.
 *  \par Template Parameters: 
 *     <dl> <dt> \e geom_type <dt>
 *          <dd> The geometric element we are "ghosting."  Must satisfy the
 *               \c GeometricElementConcept concept. </dd> </dl> */
template<typename geom_element>
class GhostElement : public geom_element
{
private:
   /*! \brief Requires that the \c geom_element type satisfy the 
    *         \c GeometricElementConcept concept. */
   BOOST_CLASS_REQUIRE(geom_element, , GeometricElementConcept);

public:
   /// Alias the \c geom_element type as \c ElementType.
   typedef geom_element                         ElementType;
   /// Define the id type for the ghost element.
	typedef GeomId< GhostElement<ElementType> >  Id;	
	
	/*! \brief Constructor.  Takes in the ghosted element and an id.
	 *
	 *  Constructor.  Takes in a constant reference to the element that we are ghosting,
	 *  plus an id number for this ghost element.
	 *  \param[in] element_in  The geometric element we are using as a "mirror" for the
	 *                         ghost element.
	 *  \param[in] id_in       The id number of the ghost element.*/
	GhostElement(const ElementType& element_in, const Id& id_in)
			: geom_element( typename ElementType::Id( id_in.idNumber() ) ),
			  mGhostedElement( addressof( element_in ) )
	{  }
   
   /*! \brief Returns a copy of the \c Id number for the ghost element.
    *
    *  Returns a copy of the \c Id number for the ghost element.
    *  \return A copy of the \c Id number for the ghost element. */
   Id id() const 
	{ 
	   return Id( id().idNumber() );
	}
	/*! \brief Prints the \c Id number to an output stream.
	 *
	 *  Prints the \c Id number for this ghost element to an output stream.
	 *  \param[in] out  The output stream we print the \c Id to.
	 *  \return         The output stream. */
   virtual ostream& printId(ostream& out) const
   {
      return out << Id( id().idNumber() );
   }

   /*! \brief Returns a constant reference to the ghosted element.
    *
    *  Returns a constant reference to the ghosted element that this
    *  ghost element uses as a "mirror."
    *  \return A constant reference to the ghosted element. */
	const ElementType& ghostedElement() const 
	{ 
	   return *mGhostedElement; 
	}
	
private:
   /// A pointer to the element we are "ghosting."
	const ElementType* mGhostedElement;
};


/*! \brief A \c Zone<OneD> template specialization of \c GhostElement.
 *
 *  This is a \c Zone<OneD> template specialization of the \c GhostElement
 *  template class for creating one-dimensional ghost zones. */
template<>
class GhostElement< Zone<OneD> > : public Zone<OneD>
{
public:
   /// The ElementType, which for this specialization is always a ghost zone.
   typedef Zone<OneD>                              ElementType;
   /// The Id type for one-dimensional ghost zones.
   typedef GeomId< GhostElement< Zone<OneD> > >    Id;
   
   /*! \brief Constructor.  Takes a \c Zone and an \c Id object.
    * 
    *  Constructor.  This constructor takes a \c Zone and an \c Id object.
    *  This constructor ensures that the ghost zone has the same length
    *  as the ghosted zone.
    *  \param[in] zone_in  The \c Zone we are ghosting.
    *  \param[in] id_in    The \c Id number for this ghost zone. */
   GhostElement(const Zone<OneD>& zone_in, const Id& id_in)
         : Zone<OneD>( Zone<OneD>::Id( id_in.idNumber() ), 
                       zone_in.length(), zone_in.area() ),
           mGhostedElement( addressof( zone_in ) )
   {  }

   /*! \brief Returns a copy of the \c Id number for the ghost zone.
    *
    *  Returns a copy of the \c Id number for the ghost zone.
    *  \return A copy of the \c Id number for the ghost zone. */   
   Id id() const
   {
      return Id( id().idNumber() );
   }
   /*! \brief Prints the \c Id number to an output stream.
	 *
	 *  Prints the \c Id number for this ghost zone to an output stream.
	 *  \param[in] out  The output stream we print the \c Id to.
	 *  \return         The output stream. */
   virtual ostream& printId(ostream& out) const
   {
      return out << Id( id().idNumber() );
   }
   /*! \brief Returns a constant reference to the ghosted \c Zone.
    *
    *  Returns a constant reference to the ghosted \c Zone that this
    *  ghost zone uses as a "mirror."
    *  \return A constant reference to the ghosted \c Zone. */   
   const Zone<OneD>& ghostedElement() const
   {
      return *mGhostedElement;
   }
   
private:
   /// A pointer to the \c Zone we are "ghosting."
   const Zone<OneD>* mGhostedElement;
};

/*! Prints the \c GhostElement<Zone>::Id identification number to the output 
 *  stream \a out.
 *  \par Template Parameters: 
 *     <dl> <dt> \e dimension_type </dt> 
 *          <dd> The dimension of the \c GhostElement<Zone>.  Must satisfy the
 *               \c DimensionConcept concept. </dd> </dl> 
 *  \param[in] out  The output stream to direct the output to.
 *  \param[in] id   The identification number to print.
 *  \return A reference to the output stream. */
template<typename dimension_type>
inline ostream& operator<<(ostream& out, 
                           const GeomId< GhostElement< Zone<dimension_type> > >& id)
{
   function_requires< DimensionConcept<dimension_type> >();
	return out << id.idNumber() << "(Ghost Zone)";
}

/*! Prints the \c GhostElement<Node>::Id identification number to the output 
 *  stream \a out.
 *  \par Template Parameters: 
 *     <dl> <dt> \e dimension_type </dt> 
 *          <dd> The dimension of the \c GhostElement<Node>.  Must satisfy the
 *               \c DimensionConcept concept. </dd> </dl> 
 *  \param[in] out  The output stream to direct the output to.
 *  \param[in] id   The identification number to print.
 *  \return A reference to the output stream. */
template<typename dimension_type>
inline ostream& operator<<(ostream& out, 
                           const GeomId< GhostElement< Node<dimension_type> > >& id)
{
   function_requires< DimensionConcept<dimension_type> >();
	return out << id.idNumber() << "(Ghost Node)";
}

/*! Prints the \c GhostElement<Corner>::Id identification number to the output 
 *  stream \a out.
 *  \par Template Parameters: 
 *     <dl> <dt> \e dimension_type </dt> 
 *          <dd> The dimension of the \c GhostElement<Corner>.  Must satisfy the
 *               \c DimensionConcept concept. </dd> </dl> 
 *  \param[in] out  The output stream to direct the output to.
 *  \param[in] id   The identification number to print.
 *  \return A reference to the output stream. */
template<typename dimension_type>
inline ostream& operator<<(ostream& out, 
                           const GeomId< GhostElement< Corner<dimension_type> > >& id)
{
   function_requires< DimensionConcept<dimension_type> >();
	return out << id.idNumber() << "(Ghost Corner)";
}
	
///  @}

#endif

