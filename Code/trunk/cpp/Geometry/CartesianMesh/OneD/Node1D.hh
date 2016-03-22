// Date Last Altered: $Date: 2009-03-10 10:56:40 -0600 (Tue, 10 Mar 2009) $
// Revision Number: $Revision: 480 $
//--------------------------------------------*-C++-*------------------------------------//
/*! \file: Node1D.hh
 *  \author Greg Davidson
 *  \date June 3, 2005
 * 
 *  \brief Provides one-dimensional Node geometric element.
 *
 *  This file provides one-dimensional Node class.  The Node class
 *  stores positional information and references to its connected \c Zone and \c Corner
 *  objects. */

#ifndef NODE1D_HH
#define NODE1D_HH

#include <utility>
// #include <boost/tuple/tuple.hpp>

#include "Point1D.hh"
#include "Node_base.hh"
#include "Zone_base.hh"

// using std::pair;

/*! \addtogroup MeshMod Mesh Module
 *  @{  */

/*! \brief The \c Node<OneD> class stores positional information and \c Zone and \c Corner 
 *         connectivities.
 *
 *  The \c Node<OneD> class stores the position of the \c Node<OneD>, and also stores pointers 
 *  to the \c Zone<OneD> and \c Corner<OneD> objects that the \c Node<OneD> is connected to 
 *  (on its left and right).  A \c Node<OneD> is on the domain boundary if it does not contain 
 *  both a left and a right \c Zone<OneD> reference.  This class inherits functionality from
 *  the \c Node_base<OneD> class. */
template<>
class Node<OneD> : public Node_base<OneD>
{
public:
   /// Imports the \c Id class from \c Node_base<OneD> into the \c Node<OneD> scope.
   typedef Node_base<OneD>::Id         Id;
   /// Imports the \c PointType class from \c Node_base<OneD> into the \c Node<OneD> scope.
   typedef Node_base<OneD>::PointType  PointType;
   /// Imports the \c Zone class from \c Node_base<OneD> into the \c Node<OneD> scope.
   typedef Node_base<OneD>::Zone       Zone;
   /// Imports the \c IndexType class from \c Node_base<OneD> into the \c Node<OneD> scope.
   typedef Node_base<OneD>::IndexType  IndexType;

   typedef Node_base<OneD>::Corner     Corner;

   /*! \brief Imports the \c NoElementConnected<Node<OneD>, Zone> exception class 
    *         into the \c Node<OneD> scope. Thrown when an attempt is made to access a 
    *         non-existent connected \c Zone. */
   typedef NoElementConnected< Node<OneD>, Zone >     NoZoneConnected;
   /*! \brief Imports the \c NoElementConnected<Node<OneD>, Corner> exception class 
    *         into the \c Node<OneD> scope. Thrown when an attempt is made to access a 
    *         non-existent connected \c Corner. */   
   typedef NoElementConnected< Node<OneD>, Corner >   NoCornerConnected;

	// ****** Constructor ******
	/*! \brief Constructor for the \c Node<OneD> class.  Takes in an \c Id and a 
	 *         \c PointType.
	 *
	 *  Constructor for the Node<OneD> class.  Takes in an \c Id for identification
	 *  and a \c PointType for the Node<OneD> location. 
	 *  \param id_in   The \c Node<OneD> object's id number.
	 *  \param pos     The \c Node<OneD> object's position. */
	explicit Node(const Id& id_in, const PointType& pos = PointType())
			: Node_base<OneD>(id_in, pos),
			  mZones(0,0), mCorners(0,0)
	{ }
	
	// ****** Connectivity Accessors ******
	/*! \brief  Returns a \c std::pair storing pointers to the connected
	 *          \c Zone<OneD> objects.
	 *
	 *  Returns a \c boost::std::pair storing pointers to the \c Zone<OneD> 
	 *  objects connected to this \c Node<OneD> object.
	 *  \return  A \c boost::std::pair storing pointers to the connected
	 *           \c Zone<OneD> objects. */
	const std::pair<const Zone*, const Zone*>& getZones() const 
	{ 
	   return mZones; 
	}
	/*! \brief  Returns a \c std::pair storing pointers to the connected
	 *          \c Corner<OneD> objects.
	 *
	 *  Returns a \c std::pair storing pointers to the \c Corner<OneD> 
	 *  objects connected to this \c Node<OneD> object.
	 *  \return  A \c std::pair storing pointers to the connected
	 *           \c Corner<OneD> objects. */
	const std::pair<const Corner*, const Corner*>& getCorners() const 
	{ 
	   return mCorners; 
	}
	/*! \brief Returns a constant reference to the \c Zone<OneD> corresponding to
	 *         \a index.
	 *
    *  Returns a constant reference to the \c Zone<OneD> corresponding to \a index.
    *  \param  index  If \a index is 0 then this method returns the left \c Zone<OneD>
    *                 and if \a index is 1 then this method returns the right \c Zone<OneD>.
    *  \return  A constant reference to the \c Zone1D corresponding to \a index.
    *  \throws  NoZoneConnected  Throws \c NoZoneConnected if an attempt is made to access a 
    *                            non-existent \c Zone<OneD>. */
   const Zone& getZone(Node<OneD>::IndexType index) const throw(NoZoneConnected)
   {
	   if(index == 0)
	   {
	      if(mZones.first == 0)
	         throw NoZoneConnected(__LINE__, __FILE__, mId, 0);
	      else return *mZones.first;
	   }
	   else if(index == 1)
	   {
	      if(mZones.second == 0)
		      throw NoZoneConnected(__LINE__, __FILE__, mId, 1);
		   else return *mZones.second;
	   }
	   else 
         throw NoZoneConnected(__LINE__, __FILE__, mId, index);
   }
   /*! \brief Returns the left \c Zone<OneD> connected to this \c Node<OneD> object.
    *
	 *  Returns the left \c Zone<OneD> connected to this \c Node<OneD> object.
    *  \return  A constant reference to the \c Zone<OneD> connected to the left-side
    *           of this \c Node<OneD> object. 
    *  \throws  NoZoneConnected  Throws this exception if there is no \c Zone<OneD>
    *                            connected to the left side of this \c Node<OneD> object. */
	const Zone& getLeftZone() const throw(NoZoneConnected)
   { 
	   if(mZones.first == 0) 
	      throw NoZoneConnected(__LINE__, __FILE__, mId, 0);
  	   return *mZones.first;  
   }
   /*! \brief Returns the right \c Zone<OneD> connected to this \c Node<OneD> object.
	 *	
	 *  Returns the right \c Zone<OneD> connected to this \c Node<OneD> object.
    *  \return  A constant reference to the \c Zone<OneD> connected to the right-side
    *           of this \c Node<OneD> object. 
    *  \throws  NoZoneConnected  Throws this exception if there is no \c Zone<OneD> connected 
    *                            to the right side of this \c Node<OneD> object. */
	const Zone& getRightZone() const throw(NoZoneConnected)
   { 
	   if(mZones.second == 0) 
	      throw NoZoneConnected(__LINE__, __FILE__, mId, 1);
	   return *mZones.second; 
   }
	
   /*! \brief Returns a constant reference to the \c Corner<OneD> corresponding to
	 *         \a index.
	 *
    *  Returns a constant reference to the \c Corner<OneD> corresponding to \a index.
    *  \param  index  If \a index is 0 then this method returns the left \c Corner<OneD> 
    *                 and if \a index is 1 then this method returns the right \c Corner<OneD>.
    *  \return  A constant reference to the \c Corner<OneD> corresponding to \a index.
    *  \throws  NoCornerConnected  Throws \c NoCornerConnected if an attempt is made to access 
    *                              a non-existent \c Corner<OneD>. */
	const Corner& getCorner(IndexType index) const throw(NoCornerConnected)
   {
      if(index == 0)
      {
   	   if(mCorners.first == 0) 
   	      throw NoCornerConnected(__LINE__, __FILE__, mId, 0);
   	   else return *mCorners.first;
   	}
   	else if(index == 1)
   	{
   	   if(mCorners.second == 0)
   	      throw NoCornerConnected(__LINE__, __FILE__, mId, 1);
   	   else return *mCorners.second;
   	}
   	else
   	   throw NoCornerConnected(__LINE__, __FILE__, mId, index);
   }
   /*! \brief Returns the left \c Corner<OneD> connected to this \c Node<OneD> object.
	 *
	 *  Returns the left \c Corner<OneD> connected to this \c Node<OneD> object.
    *  \return  A constant reference to the \c Corner<OneD> connected to the left-side
    *           of this \c Node<OneD> object. 
    *  \throws  NoCornerConnected  Throws this exception if there is no \c Corner<OneD>
    *                              connected to the left side of this \c Node<OneD> object. */
   const Corner& getLeftCorner() const throw(NoCornerConnected)
   {
   	if(mCorners.first == 0) 
   	   throw NoCornerConnected(__LINE__, __FILE__, mId, 0);
   	return *mCorners.first;
   }
   /*! \brief Returns the right \c Corner<OneD> connected to this \c Node<OneD> object.
    *
    *  Returns the right \c Corner<OneD> connected to this \c Node<OneD> object.
    *  \return  A constant reference to the \c Corner<OneD> connected to the right-side
    *           of this \c Node<OneD> object. 
    *  \throw  NoCornerConnected  Throws this exception if there is no \c Corner<OneD>
    *                             connected to the right side of this \c Node<OneD> object. */
   const Corner& getRightCorner() const throw(NoCornerConnected)
   {
   	if(mCorners.second == 0) 
   	   throw NoCornerConnected(__LINE__, __FILE__, mId, 1);
   	return *mCorners.second;
   }
	
	/*! \brief Returns true if this \c Node<OneD> sits on a domain boundary.
	 *
	 *  Returns true if this \c Node<OneD> sits on a domain boundary.
	 *  \return Returns true if this \c Node<OneD> sits on a domain boundary. */
	bool onBoundary() const 
	{ 
		return ( onLeftBoundary() || onRightBoundary() );
	}
	/*! \brief Returns true if this \c Node<OneD> sits on the left domain boundary.
	 *
	 *  Returns true if this \c Node<OneD> sits on the left domain boundary.
	 *  \return Returns true if this \c Node<OneD> sits on the left domain boundary. */
	bool onLeftBoundary() const
	{
		return (mZones.first == 0);
	}
	/*! \brief Returns true if this \c Node<OneD> sits on the right domain boundary.
	 *
	 *  Returns true if this \c Node<OneD> sits on the right domain boundary.
	 *  \return Returns true if this \c Node<OneD> sits on the right domain boundary. */
	bool onRightBoundary() const
	{
		return (mZones.second == 0);
	}
	
private:
   /// \c CartesianMesh<OneD> is a friend of \c Node<OneD> so it can set the connectivity.
	friend class CartesianMesh<OneD>;

   /// Stores pointers to the \c Zone<OneD> objects connected to this \c Node<OneD> object.
	std::pair<const Zone*, const Zone*> mZones;
   /// Stores pointers to the \c Corner<OneD> objects connected to this \c Node<OneD> object.
	std::pair<const Corner*, const Corner*> mCorners;
};


/// @}

#endif

