// Date Last Altered: $Date: 2008-08-04 22:55:41 -0600 (Mon, 04 Aug 2008) $
// Revision Number: $Revision: 344 $
//--------------------------------------------*-C++-*------------------------------------//
/*! \file GeomId.hh
 *  \author Greg Davidson
 *  \date November 5, 2006
 *
 *  \brief This file provides the \c GeomId classes.
 *
 *  This file provides the \c GeomId classes.  Every geometric element has a unique id number
 *  which is stored in the appropriate id class. */

#ifndef GEOMID_HH
#define GEOMID_HH

#include <ostream>

#include "Types.hh"

using std::ostream;

/// Forward declaration of the \c Zone type.
template<typename dimension_type> class Zone;
/// Forward declaration of the \c Node type.
template<typename dimension_type> class Node;
/// Forward declaration of the \c Corner type.
template<typename dimension_type> class Corner;


/*! \addtogroup MeshMod Mesh Subsystem
 *  @{  */

/*! \brief Defines an id type for geometric elements.
 *
 *  This class defines an id type for geometric elements such as \c Zone
 *  and \c Node.  It stores a unique identification number for these
 *  elements to ease member access and debugging. 
 *  \par Template Parameters:
 *     <dl> <dt> \e elem_type </dt>
 *          <dd> The \c GeomId class is templated on \c elem_type to make
 *               every id type for every geometric element unique. </dd> </dl> */
template<typename elem_type>
class GeomId
{
public:
   /// Defines the id number type.
	typedef UnsignedInt4    IdType;
	/// Defines the geometric element type this \c GeomId is for.
	typedef elem_type       GeomType;   
	
	/*! \brief Constructor.  Takes in an id number.
	 *  
	 *  Constructor.  Takes in an id number.
	 *  \param id_in   The number for this id. */
	explicit GeomId(IdType id_in)
			: mId(id_in)
	{  }
	
	/*! \brief Returns the identification number.
	 *
	 *  Returns the identification number.
	 *  \return  The identification number. */
	IdType idNumber() const 
	{ 
	   return mId; 
	}
private:
   /// Stores the identification number.
	IdType mId;                          
};

/*! \brief Prints the \c Zone::Id identification number.
 *
 *  Prints the \c Zone::Id identification number to the output stream \a out.
 *  \par Template Parameters:
 *     <dl> <dt> \e dimension_type </dt>
 *          <dd> The dimension of the \c Zone. </dd> </dl>
 *  \param[in] out  The output stream to direct the output to.
 *  \param[in] id   The identification number to print.
 *  \return A reference to the output stream. */
template<typename dimension_type>
inline ostream& operator<<(ostream& out, const GeomId< Zone<dimension_type> >& id)
{
   return out << id.idNumber();
}
/*! \brief Prints the \c Node::Id identification number.
 *
 *  Prints the \c Node::Id identification number to the output stream \a out.
 *  \par Template Parameters:
 *     <dl> <dt> \e dimension_type </dt>
 *          <dd> The dimension of the \c Node. </dd> </dl>
 *  \param[in] out  The output stream to direct the output to.
 *  \param[in] id   The identification number to print.
 *  \return A reference to the output stream. */
template<typename dimension_type>
inline ostream& operator<<(ostream& out, const GeomId< Node<dimension_type> >& id)
{
   return out << id.idNumber();
}
/*! \brief Prints the \c Corner::Id identification number.
 *
 *  Prints the \c Corner::Id identification number to the output stream \a out.
 *  \par Template Parameters:
 *     <dl> <dt> \e dimension_type </dt>
 *          <dd> The dimension of the \c Corner. </dd> </dl>
 *  \param[in] out  The output stream to direct the output to.
 *  \param[in] id   The identification number to print.
 *  \return A reference to the output stream. */
template<typename dimension_type>
inline ostream& operator<<(ostream& out, const GeomId< Corner<dimension_type> >& id)
{
   return out << id.idNumber();
}

/// @}

#endif

