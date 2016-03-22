// Date Last Altered: $Date: 2009-03-10 10:56:40 -0600 (Tue, 10 Mar 2009) $
// Revision Number: $Revision: 480 $
//--------------------------------------------*-C++-*------------------------------------//
/*! \file Zone1D.hh
 *  \author Greg Davidson
 *  \date June 13, 2008
 * 
 *  \brief Provides the interface for the one-dimensional \c Zone geometric element.
 *
 *  This file provides the interface for the one-dimensional \c Zone class.  The \c Zone class
 *  basically stores all of the information that you would expect a \c Zone to contain (basically
 *  just physical dimensions).  The Zone has pointers to its left and right \c Node objects and 
 *  the left and right \c Corner objects. */

#ifndef ZONE1D_HH
#define ZONE1D_HH

#include <cmath>
#include <limits>
#include <utility>

// #include <boost/tuple/tuple.hpp>

#include "Zone_base.hh"
#include "Angle1D.hh"
#include "Point1D.hh"
#include "MeshExcept.hh"

using std::fabs;
using std::numeric_limits;
// using boost::tuple;

/*! \addtogroup MeshMod Mesh Module
 *  @{ */


/*! \brief The \c Zone class stores a \c Zone object's physical dimensions and connectivity.
 *
 *  The \c Zone class contains all of the \c Zone specific information necessary to provide spatial
 *  information to the various physics packages.  Specifically, it knows its spatial dimensions
 *  and holds pointers to its neighboring \c Node objects and the \c Corner objects that it contains. 
 *  Some of the \c Zone class's functionality is inherited from the \c Zone_base<OneD> class. */
template<>
class Zone<OneD> : public Zone_base<OneD>
{
private:
   /// Define the \c Zone_base<OneD> class as \c Base.
   typedef Zone_base<OneD>   Base;
   
public:
   /// Import the \c Base::Id type into the \c Zone scope.
   typedef Base::Id          Id;
   /// Import the \c Base::LengthType type into the \c Zone scope.
   typedef Base::LengthType  LengthType;
   /// Import the \c Base::Node type into the \c Zone scope.
   typedef Base::Node        Node;
   /// Import the \c Base::Corner type into the \c Zone scope.
   typedef Base::Corner      Corner;

   /*! \brief Imports the \c NoElementConnected<Zone<OneD>, Node> exception class 
    *         into the \c Zone<OneD> scope. Thrown when an attempt is made to access a 
    *         non-existent connected \c Node. */
   typedef NoElementConnected< Zone<OneD>, Node >     NoNodeConnected;
   /*! \brief Imports the \c NoElementConnected<Zone<OneD>, Corner> exception class 
    *         into the \c Zone<OneD> scope. Thrown when an attempt is made to access a 
    *         non-existent connected \c Corner. */
   typedef NoElementConnected< Zone<OneD>, Corner >   NoCornerConnected;

   /*! \brief Constructor.  Takes in an \c Id type, a \c Zone length, and a \c Zone area.
    *
	 *  Constructor for the Zone class.  Takes in an \c Id type, a \c Zone length, and a
	 *  \c Zone area.
	 *  \param[in] id_in      The \c Zone object's \c Id.
	 *  \param[in] length_in  The length of the zone.
	 *  \param[in] area_in    The cross-sectional area of the zone.  */
	Zone(const Id& id_in, LengthType length_in, LengthType area_in)
	      : Zone_base<OneD>(id_in, length_in),
	        mArea(area_in), mNodes(0,0), mCorners(0,0)
	{  }
	      
   /// Destructor.
	~Zone() 
	{  }
	
	// ****** Data Accessors ******
	/*! \brief Returns the cross-sectional area of the \c Zone object.
	 *
	 *  Returns the cross-sectional area of the \c Zone object.
	 *  \return The cross-sectional area of the \c Zone object. */
	LengthType area() const 
	{ 
	   return mArea; 
	}
	/*! \brief Returns the volume of the \c Zone object.
	 *
	 *  Returns the volume of the \c Zone object.
	 *  \return The volume of the \c Zone object. */
	LengthType volume() const 
	{ 
	   return mLength*mArea; 
	}
	
	/// \brief  Returns the \c PointType located at the \c Zone object's center.
   PointType center() const;

	/*! \brief Returns a \c std::pair of pointers to the connected \c Node objects.
	 *
	 *  Returns a \c std::pair of the pointers to the connected \c Node objects.
	 *  \return A \c std::pair of the pointers to the connected \c Node objects. */
	const std::pair<const Node*, const Node*>& getNodes() const 
	{ 
	   return mNodes; 
	}
	/*! \brief Returns a \c std::pair of pointers to the connected \c Corner objects.
	 *
	 *  Returns a \c std::pair of the pointers to the connected \c Corner objects.
	 *  \return A \c std::pair of the pointers to the connected \c Corner objects. */
	const std::pair<const Corner*, const Corner*>& getCorners() const 
	{ 
	   return mCorners; 
	}
	
	/*! \brief Returns a reference to the \c Node corresponding to \a index.
	 *
    *  Returns a reference to the \c Node corresponding to \a index.
    *  \param[in]  index  If index equals 0 then the left \c Node is returned, and
    *                     if index equals 1 then the right \c Node is returned.
    *  \return  A reference to the \c Node corresponding to \a index. 
    *  \throws  NoNodeConnected  If index corresponds to a non-existent \c Node, then
    *                            this exception is thrown. */
	const Node& getNode(IndexType index) const throw(NoNodeConnected)
	{
      Assert(mNodes.first == 0);
      Assert(mNodes.second == 0);

      if(index == 0)  
         return *mNodes.first;
      else if(index == 1)
         return *mNodes.second;
      else
         throw NoNodeConnected(__LINE__, __FILE__, mId, index);
   }
	/*! \brief Returns a reference to the left \c Node connected to this 
	 *         \c Zone object.
	 *
	 *  Returns a reference to the left \c Node connected to this \c Zone object.
	 *  \return  The left \c Node connected to this \c Zone object. */
	const Node& getLeftNode() const 
	{ 
	   Assert(mNodes.first != 0);
	   return *mNodes.first; 
	}
	/*! \brief Returns a reference to the right \c Node connected to this 
	 *         \c Zone object.
	 *
	 *  Returns a reference to the right \c Node connected to this \c Zone object.
	 *  \return  The right \c Node connected to this \c Zone object. */
	const Node& getRightNode() const 
	{ 
	   Assert(mNodes.second != 0);
	   return *mNodes.second; 
	}
	
	/*! \brief Returns a reference to the \c Corner corresponding to \a index.
	 *
    *  Returns a reference to the \c Corner corresponding to \a index.
    *  \param[in] index  If index equals 0 then the left \c Corner is returned, and
    *                    if index equals 1 then the right \c Corner is returned.
    *  \return  A reference to the \c Corner corresponding to \a index. 
    *  \throws  NoCornerConnected  If index corresponds to a non-existent \c Corner<OneD>, 
    *                              then this exception is thrown. */
	const Corner& getCorner(IndexType index) const throw(NoCornerConnected)
   {
      Assert(mCorners.first == 0);
      Assert(mCorners.second == 0);

      if(index == 0)  
         return *mCorners.first;
      else if(index == 1)
         return *mCorners.second;
      else
         throw NoCornerConnected(__LINE__, __FILE__, mId, index);
   }
	/*! \brief Returns a reference to the left \c Corner connected to this 
	 *         \c Zone object.
	 *
	 *  Returns a reference to the left \c Corner connected to this \c Zone object.
	 *  \return  The left \c Corner connected to this \c Zone object. */
	const Corner& getLeftCorner() const 
	{ 
	   Assert(mCorners.first != 0);
	   return *mCorners.first; 
	}
	/*! \brief Returns a reference to the right \c Corner connected to this 
	 *         \c Zone object.
	 *
	 *  Returns a reference to the right \c Corner connected to this \c Zone object.
	 *  \return  The right \c Corner connected to this \c Zone object. */
	const Corner& getRightCorner() const 
	{ 
	   Assert(mCorners.second != 0);
	   return *mCorners.second; 
	}

   /*! \brief Calculates the distance to the \c Zone boundary from \a position along 
    *         direction \a angle. */
	LengthType distanceToBndry(const PointType& position, const Angle<OneD>& angle) const;
   
private:
   /// \c CartesianMesh<OneD> sets the connectivity of the \c Zone objects.
	friend class CartesianMesh<OneD>;
   /// Stores the cross-sectional area.
	LengthType mArea;
	/// Stores the pointers to the connected \c Node objects.
   std::pair<const Node*, const Node*>     mNodes;
 	/// Stores the pointers to the connected \c Corner objects.
   std::pair<const Corner*, const Corner*> mCorners;
};

// Include the implementation of certain methods
#include "Zone1D_Impl.hh"

/// @}

#endif


