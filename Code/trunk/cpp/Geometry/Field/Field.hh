// Date Last Altered: $Date: 2009-05-14 15:08:28 -0600 (Thu, 14 May 2009) $
// Revision Number: $Revision: 545 $
//--------------------------------------------*-C++-*------------------------------------//
/*! \file: Field.hh
 *  \author Greg Davidson
 *  \date June 10, 2005
 *  
 *  \brief Provides the \c Field templated class for storing spatial information.
 *
 *  This file provides the \c Field templated class. The purpose of the \c Field class is to 
 *  store spatially-dependent information in a way that respects the size and connectivity of 
 *  the \c CartesianMesh and its associated geometric elements (\c Zone, \c Node, etc...). */
 
#ifndef FIELD_HH
#define FIELD_HH

#include <vector>

#include <boost/call_traits.hpp>
#include <boost/concept_check.hpp>

#include "Assert.hh"
#include "FieldExcept.hh"
#include "Iterator.hh"
#include "ReverseIterator.hh"
#include "CartesianMesh1D.hh"
#include "GeometricElementConcept.hh"
#include "MinimalContainerConcept.hh"

class HistSource;

using std::vector;
using boost::is_convertible;
using boost::DefaultConstructibleConcept;
using boost::CopyConstructibleConcept;
using boost::ConvertibleConcept;
using boost::Mutable_RandomAccessContainerConcept;
using boost::RandomAccessContainerConcept;

/// \defgroup FieldMod Field Module
/*! \addtogroup FieldMod Field Module
 *  \brief The \c Field Module contains all the functionality for storing spatially-dependent
 *         data.
 *
 *  The \c Field Module consists of all of the classes and functionality necessary to
 *  hold spatially-dependent data.  The \c Field class is designed such that data can be
 *  "centered" on any geometric element within the \c CartesianMesh.
 *  @{  */

/*! \brief The \c Field class provides storage of spatially-dependent data.
 *
 *  The purpose of the \c Field class is to store information in a way that respects the size and 
 *  connectivity of the \c CartesianMesh and its associated geometric elements (\c Zone, \c Node, 
 *  etc...). The \c Field interface is similar to that of a \c vector, except geometric elements 
 *  are used in the bracket operators instead of meaningless integers.  This prevents accidentally 
 *  confusing \c Zone and \c Node data and also helps prevent segmentation faults at very little 
 *  extra cost.  We note that the \c Field class is guaranteed to order its data in the same order
 *  that the \c CartesianMesh orders its geometric elements.  Therefore, this is guaranteed to work:
 *  \code
 *     CartMesh1D::zoneIterator zone_iter           = mesh.zoneBegin();
 *     Field< Zone1D, double >::iterator field_iter = field.begin();
 *     for(; zone_iter != mesh.zoneEnd(); ++field_iter, ++zone_iter)
 *     {
 *        // The field_iter will always correspond to the data that belongs to the Zone
 *        // pointed at by zone_iter, and is guaranteed not to go out-of-range!
 *     }
 *  \endcode
 *  The \c Field class also has optional range and size checking.  This can be turned
 *  off by compiling with \b NFIELDCHK defined.
 *  \par Template Parameters:
 *     <dl> <dt> \e geom_elem </dt>
 *          <dd> The geometric element that the spatially-dependent data is centered on. 
 *               This type must satisfy the \c GeometricElementConcept concept. </dd>
 *          <dt> \e data_type </dt>
 *          <dd> The type of data being stored in the \c Field. </dd>
 *          <dt> \e container_type </dt>
 *          <dd> The container used to hold the data.  This is a \c std::vector by default. 
 *               This type must satisfy the \c MinimalContainerConcept. </dd> </dl> */
template< typename geom_elem, typename data_type, 
          typename container_type = vector<data_type> >
class Field
{
private:
   /// Requires that the \c geom_elem type satisfy the \c GeometricElementConcept.
  	BOOST_CLASS_REQUIRE(geom_elem, , GeometricElementConcept);
    /// Requires that the \c container_type satisfy the \c MinimalContainerConcept.
  	BOOST_CLASS_REQUIRE(container_type, , MinimalContainerConcept);
    /// Requires that \c data_type and \c container_type::value_type be convertible.
    BOOST_STATIC_ASSERT(( is_convertible<data_type, typename container_type::value_type>::value ));
   
   /// The type of this \c Field.
   typedef Field<geom_elem, data_type, container_type>  thisType;

   friend class HistSource;
   friend class LinearSpaceSource;

public:
   // ****** Define the public types ******
   /// Defines the \c geom_elem template parameter as the data \c CenteringType.	
	typedef geom_elem                                         CenteringType;
	/// Defines the \c MeshType.
	typedef CartesianMesh<typename geom_elem::DimensionType>  MeshType;
	/// Aliases the \c container_type template paramater as \c ContainerType.
	typedef container_type                                    ContainerType;
	
	/// Aliases the \c data_type template parameter as \c DataType.
	typedef data_type                                         DataType;
	/// For container concept compliance, defines the \c value_type.
   typedef DataType                                          value_type;
   /// For container concept compliance, defines the \c reference type.
   typedef typename call_traits<DataType>::reference         reference;
   /// For container concept compliance, defines the \c const_reference type.
   typedef typename call_traits<DataType>::const_reference   const_reference;
   /// For container concept compliance, defines the \c pointer type.
   typedef DataType*                                         pointer;
   /// For container concept compliance, defines a \c size_type.
   typedef UnsignedInt4                                      size_type;
   
   /// Defines an \c iterator type.
   typedef Iterator< ContainerType, typename ContainerType::iterator >        iterator;
   /// Defines a \c const_iterator type.
   typedef Iterator< ContainerType, typename ContainerType::const_iterator >  const_iterator;
   /// Defines a \c reverse_iterator type.
   typedef ReverseIterator< iterator >                                        reverse_iterator;
   /// Defines a \c const_reverse_iterator type.
   typedef ReverseIterator< const_iterator >                                  const_reverse_iterator;   
   
   /*! \brief Imports the \c WrongSizedField<FieldType> into the \c Field scope.  
    *         Thrown when the \c Field is constructed with the wrong size. */
   typedef WrongSizedField<thisType>      WrongSizedField;
   /*! \brief Imports the \c FieldRangeError<thisType> into the \c Field scope.  
    *         Thrown when an out-of-range access is attempted. */
   typedef FieldRangeError<thisType>      FieldRangeError;
   
   // ****** Constructors ******
   /*! \brief Constructor.  Takes in a \c CartesianMesh and default constructs the data.
    *
    *  Constructor.  Takes in a \c CartesianMesh to size the \c Field.  Uses the default
    *  constructor to construct the data, so it is required that the \c DataType be
    *  default constructible.
    *  \param[in] mesh_in  The \c CartesianMesh that dictates the size of this \c Field. */
   explicit Field(const MeshType& mesh_in)
			: mData( numCentering<typename CenteringType::DimensionType, CenteringType>(mesh_in) ),
			  mMesh(mesh_in) 
	{  
	   function_requires< DefaultConstructibleConcept<DataType> >();
	}

   /*! \brief Constructor.  Takes in a \c CartesianMesh and makes copies of \a data_in.
    *
    *  Constructor.  Takes in a \c CartesianMesh to size the \c Field.  Fills the \c Field
    *  with copies of \a data_in, so \c DataType must be copy constructible.
    *  \param[in] mesh_in  The \c CartesianMesh that dictates the size of this \c Field.
    *  \param[in] data_in  The value to fill the \c Field with copies of. */
   Field(const MeshType& mesh_in, typename call_traits<DataType>::param_type data_in)
			: mData( numCentering<typename CenteringType::DimensionType, CenteringType>(mesh_in),
			         data_in ),
   		  mMesh(mesh_in)
	{  
	   function_requires< CopyConstructibleConcept<value_type> >();
	}
	
   /*! \brief Constructor.  Takes in a \c CartesianMesh and a \c ContainerType full of data.
    *
    *  Constructor.  Takes in a \c CartesianMesh and a container full of data.  The container
    *  must be the same size as the \c Field. 
    *  \param[in] mesh_in  The \c CartesianMesh that dictates the size of this \c Field.
    *  \param[in] data_in  A \c ContainerType full of data to fill the \c Field with. 
    *  \throw WrongSizedField  If the size of \a data_in is different than the number of
    *                          \c CenteringType objects in the \c CartesianMesh, this
    *                          exception will be thrown.  This checking can be disabled
    *                          by compiling with \b NFIELDCHK defined.  */   
   Field(const MeshType& mesh_in, const ContainerType& data_in)
//                   throw(WrongSizedField)
			: mData( data_in ),
			  mMesh(mesh_in)
   {
#ifndef NFIELDCHK
      if( mData.size() != numCentering< typename CenteringType::DimensionType, 
                                        CenteringType >(mMesh) )
      {
         throw WrongSizedField(mMesh, *this, __LINE__, __FILE__);
      }
#endif                                  
		Assert(( mData.size() == 
		            numCentering<typename CenteringType::DimensionType, CenteringType>(mMesh) ));
	}

   /*! \brief Constructor.  Takes in a \c CartesianMesh and a \c ContainerType2 full of data.
    *
    *  Constructor.  Takes in a \c CartesianMesh and a container full of data.  The container
    *  must be the same size as the \c Field. 
    *  \par Template Parameters:
    *     <dl> <dt> \e ContainerType2 </dt>
    *          <dd> The container type that holds data to fill the \c Field with. </dd> </dl>
    *  \param[in] mesh_in  The \c CartesianMesh that dictates the size of this \c Field.
    *  \param[in] data_in  A \c ContainerType2 full of data to fill the \c Field with. 
    *  \throw WrongSizedField  If the size of \a data_in is different than the number of
    *                          \c CenteringType objects in the \c CartesianMesh, this
    *                          exception will be thrown.  This checking can be disabled
    *                          by compiling with \b NFIELDCHK defined.  */   
   template<typename ContainerType2>
   Field(const MeshType& mesh_in, const ContainerType2& data_in)
//                   throw(WrongSizedField)
      : mData( data_in.begin(), data_in.end() ),
        mMesh( mesh_in )
   {
      function_requires< ConvertibleConcept<DataType, typename ContainerType2::value_type> >();
#ifndef NFIELDCHK
      if( mData.size() != numCentering< typename CenteringType::DimensionType, 
                                        CenteringType >(mMesh) )
      {
         throw WrongSizedField(mMesh, *this, __LINE__, __FILE__);
      }
#endif
      Assert(( mData.size() ==
  		            numCentering<typename CenteringType::DimensionType, CenteringType>(mMesh) ));
   }
   
	/*! \brief Copy constructor.
	 *
	 *  Copy constructor.  Takes in another \c Field of the same type to copy.
	 *  \param[in] field_in  The \c Field to copy. */
	Field(const Field<CenteringType, DataType, ContainerType>& field_in)
			: mData(field_in.mData),
    		  mMesh(field_in.mMesh)
	{  }

   // ****** Copy Operators ******
	/*! \brief Copy operator.
	 *
	 *  Copy operator.  Takes in another \c Field to copy.
	 *  \param[in] field_in  The \c Field to copy. */
	Field<CenteringType, DataType, ContainerType>&
	operator=(const Field<CenteringType, DataType, ContainerType>& field_in)
	{
		mData = field_in.mData;
		return *this;
	}
	
	/*! \brief Copy operator, with any \c Field with the same centering.
	 *
	 *  Copy operator.  Takes in another \c Field to copy.  The \c Field to be copied
	 *  may have any convertible data type and any container.
	 *  \param[in] field_in  The \c Field to copy.  May have any convertible data type and
	 *                       any container. */
   template<typename ContainerType2, typename DataType2>
   Field<CenteringType, DataType, ContainerType>&
   operator=(const Field<CenteringType, DataType2, ContainerType2>& field_in)
   {
      function_requires< ConvertibleConcept<DataType, DataType2> >();

      typename ContainerType::iterator data_iter = mData.begin();
      for(typename Field<CenteringType, DataType2, ContainerType2>::const_iterator iter = field_in.begin();
          iter != field_in.end(); ++data_iter, ++iter)
      {
         *data_iter = *iter;
      }
      return *this;
   }
   
   // ****** Random access data access operators ******
   /*! \brief Bracket operator.  Returns the data corresponding to \a elem.
    *
    *  Bracket operator.  Receives a \c CenteringType \a elem and returns a \c reference to the
    *  data corresponding to \elem.  The underlying \c ContainerType for this \c Field must
    *  satisfy the \c Mutable_RandomAccessContainerConcept for this method to be used.
    *  \param[in] elem  The geometric element that we wish to receive data for. 
    *  \return  A reference to the data
    *  \throw FieldRangeError  If the geometric element \a elem exists outside the \c CartesianMesh,
    *                          this exception will get thrown.  Range checking may be disabled by 
    *                          compiling with \b NFIELDCHK defined. */
   reference operator[](const CenteringType& elem) // throw(FieldRangeError)
   {
      function_requires< Mutable_RandomAccessContainerConcept< ContainerType > >();
#ifndef NFIELDCHK
      if( elem.id().idNumber() >=
                  numCentering<typename CenteringType::DimensionType, CenteringType>(mMesh) )
      {
         throw FieldRangeError(elem, *this, __LINE__, __FILE__);
      }
#endif
      Assert(( elem.id().idNumber() < 
                  numCentering<typename CenteringType::DimensionType, CenteringType>(mMesh) ));
      
      return mData[ elem.id().idNumber() ];
   }
   
   /*! \brief Bracket operator.  Returns the data corresponding to \a elem.
    *
    *  Bracket operator.  Receives a \c CenteringType \a elem and returns a \c const_reference 
    *  to the data corresponding to \elem.  The underlying \c ContainerType for this \c Field 
    *  must satisfy the \c Mutable_RandomAccessContainerConcept for this method to be used.
    *  \param[in] elem  The geometric element that we wish to receive data for. 
    *  \throw FieldRangeError  If the geometric element \a elem exists outside the \c CartesianMesh,
    *                          this exception will get thrown.  Range checking may be disabled by 
    *                          compiling with \b NFIELDCHK defined. */
   const_reference operator[](const CenteringType& elem) const // throw(FieldRangeError)
   {
      function_requires< RandomAccessContainerConcept< ContainerType > >();
#ifndef NFIELDCHK
      if( elem.id().idNumber() >=
                  numCentering<typename CenteringType::DimensionType, CenteringType>(mMesh) )
      {
         throw FieldRangeError(elem, *this, __LINE__, __FILE__);
      }
#endif
      Assert(( elem.id().idNumber() < 
                  numCentering<typename CenteringType::DimensionType, CenteringType>(mMesh) ));
      
      return mData[ elem.id().idNumber() ];
   }

   // ****** Container property methods ******
   /*! \brief  Returns the number of elements in the \c Field.
    *
    *  Returns the size (number of data elements) in the \c Field.
    *  \return The size of the \c Field. */
   size_type size() const 
   { 
      return mData.size(); 
   }
   /*! \brief  Returns the maximum size of the \c Field.
    *
    *  Returns the maximum size of the \c Field.  Since <tt>Field</tt>s are fixed-size containers,
    *  this method is equivalent to the \c size() method.  This method is provided for container
    *  concept compatibility.
    *  \return The maximum size of the \c Field. */
   size_type max_size() const 
   {
      return mData.size(); 
   }
   /*! \brief  Returns whether this \c Field is empty.
    *
    *  Returns whether this \c Field is empty.  Because <tt>Field</tt>s are fixed-size containers,
    *  a \c Field may never be empty, so this method always returns \a false.  This method is
    *  provided for container concept compatibility.
    *  \return Whether the \c Field is empty. */
   bool empty() const 
   { 
      return false; 
   }
   /*! \brief  Swaps the data of this \c Field and \a field_in.
    *
    *  Swaps the data of this \c Field and \a field_in.
    *  \param[in] field_in  The \c Field we wish to swap data with. */
   void swap(Field<CenteringType, DataType, ContainerType>& field_in)
   {
      mData.swap(field_in.mData);
   }   

   // ****** Iterator methods ******
   /*! \brief Returns a non-const iterator pointing to the beginning of the \c Field.
    *
    *  Returns a non-const iterator pointing to the beginning of the \c Field.
    *  <tt>Field</tt>s are guaranteed to be ordered in the same order as the \c CartesianMesh.
    *  \return A non-const iterator pointing to the beginning of the \c Field. */
   iterator begin() 
   { 
      return iterator( mData, mData.begin() ); 
   }
   /*! \brief Returns a const iterator pointing to the beginning of the \c Field.
    *
    *  Returns a const iterator pointing to the beginning of the \c Field.
    *  <tt>Field</tt>s are guaranteed to be ordered in the same order as the \c CartesianMesh.
    *  \return A const iterator pointing to the beginning of the \c Field. */
   const_iterator begin() const 
   { 
      return const_iterator( mData, mData.begin() ); 
   }

   /*! \brief Returns a non-const iterator pointing to the end of the \c Field.
    *
    *  Returns a non-const iterator pointing to the end (one past the last element) of the 
    *  \c Field. <tt>Field</tt>s are guaranteed to be ordered in the same order as the 
    *  \c CartesianMesh.
    *  \return A non-const iterator pointing to the end of the \c Field. */
   iterator end() 
   { 
      return iterator( mData, mData.end() ); 
   }
   /*! \brief Returns a const iterator pointing to the end of the \c Field.
    *
    *  Returns a const iterator pointing to the end (one past the last element) of the 
    *  \c Field. <tt>Field</tt>s are guaranteed to be ordered in the same order as the 
    *  \c CartesianMesh.
    *  \return A const iterator pointing to the end of the \c Field. */
   const_iterator end() const 
   { 
      return const_iterator( mData, mData.end() ); 
   }

   /*! \brief Returns a non-const reverse iterator pointing to the reverse beginning of 
    *         the \c Field.
    *
    *  Returns a non-const reverse iterator pointing to the reverse beginning of the 
    *  \c Field. <tt>Field</tt>s are guaranteed to be ordered in the same order as the 
    *  \c CartesianMesh.
    *  \return A non-const reverse iterator pointing to the beginning of the \c Field. */
   reverse_iterator rbegin() 
   { 
      return reverse_iterator( mData, mData.rbegin() ); 
   }
   /*! \brief Returns a const reverse iterator pointing to the reverse beginning of 
    *         the \c Field.
    *
    *  Returns a const reverse iterator pointing to the reverse beginning of the 
    *  \c Field. <tt>Field</tt>s are guaranteed to be ordered in the same order as the 
    *  \c CartesianMesh.
    *  \return A const reverse iterator pointing to the beginning of the \c Field. */
   const_reverse_iterator rbegin() const 
   { 
      return const_reverse_iterator( mData, mData.rbegin() ); 
   }

   /*! \brief Returns a non-const reverse iterator pointing to the reverse end of 
    *         the \c Field.
    *
    *  Returns a non-const reverse iterator pointing to the reverse end of the 
    *  \c Field. <tt>Field</tt>s are guaranteed to be ordered in the same order as the 
    *  \c CartesianMesh.
    *  \return A non-const reverse iterator pointing to the end of the \c Field. */
   reverse_iterator rend() 
   { 
      return reverse_iterator( mData, mData.rend() ); 
   }
   /*! \brief Returns a const reverse iterator pointing to the reverse end of 
    *         the \c Field.
    *
    *  Returns a const reverse iterator pointing to the reverse end of the 
    *  \c Field. <tt>Field</tt>s are guaranteed to be ordered in the same order as the 
    *  \c CartesianMesh.
    *  \return A const reverse iterator pointing to the end of the \c Field. */
   const_reverse_iterator rend() const 
   { 
      return const_reverse_iterator( mData, mData.rend() ); 
   }

    //Given a position, return the Zone in which that position is located
    const Zone<OneD>& getZone(double x) const {return mMesh.getZone(x); }

    const MeshType& getMesh() const { return mMesh; }

protected:
private:
   /// Contains the data of the \c Field.
   ContainerType    mData;
   /// Refers to the \c CartesianMesh that this \c Field corresponds to.
   const MeshType&  mMesh;
};


/// @}

#endif
	
