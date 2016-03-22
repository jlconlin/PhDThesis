// Date Last Altered: $Date: 2006-12-01 12:16:31 -0500 (Fri, 01 Dec 2006) $
// Revision Number: $Revision: 50 $
//--------------------------------------------*-C++-*------------------------------------//
/// \file: Field.hh
/// \author Greg Davidson
/// \date June 10, 2005
/// 
/// \brief This file provides the Field templated class.
///
/// This file provides the Field templated class. The purpose of the Field class is to store information
/// in a way that respects the size and connectivity of the Mesh and its associated geometric
/// sub-elements (Zone, Node, etc...).
#ifndef FIELD_HH
#define FIELD_HH

#include <vector>

#include <boost/call_traits.hpp>
#include <boost/concept_check.hpp>

#include "OneDCartMesh.hh"
#include "GeomElem_concept.hh"
#include "Assert.hh"
#include "Iterator.hh"
#include "ReverseIterator.hh"

using std::vector;

/*! \addtogroup FieldSys Field Subsystem
 *  The field subsystem consists of all of the classes and functionality necessary to
 *  hold geometry-dependent information on the spatial problem domain.
 *  @{
 */

/// \brief The Field class provides data-storage by Mesh sub-geometric elements.
///
/// The purpose of the Field class is to store information in a way that respects the size and 
/// connectivity of the Mesh and its associated geometric sub-elements (Zone, Node, etc...). The field
/// interface is similar to that of a vector, except geometric sub-elements are used in the bracket
/// operators instead of meaningless integers.  This prevents accidentally confusing Zone and Node data
/// and also helps prevent segmentation faults at very little extra cost.  The template parameters are
/// explained as follows:
/// \param
/// The \a GeomElem template parameter is the geometric sub-element upon which the data sits.  The
/// \a DataType template parameter specifies what kind of data the field stores.  Therefore, examples
/// of valid \a Fields would include \a Field<Zone, \a double> or 
/// \a Field<Node, \a vector<double> \a >.
template<typename GeomElem, typename DataType>
class Field
{
public:
	BOOST_CLASS_REQUIRE(GeomElem, , GeomElem_concept);
	
	// Call_Traits typedefs
	typedef typename boost::call_traits<DataType>::param_type                                      param_type;
	typedef typename boost::call_traits<DataType>::reference                                       reference;
	typedef typename boost::call_traits<DataType>::const_reference                                 const_reference;
	typedef DataType                                                                               value_type;
	typedef typename boost::call_traits<DataType>::value_type                                      result_type;
	
	// Iterator typedefs
    typedef Iterator< vector<value_type>, typename vector<value_type>::iterator,
					  Field<GeomElem, DataType> >                                                  iterator;
    typedef Iterator< vector<value_type>, typename vector<value_type>::const_iterator,
					  Field<GeomElem, DataType> >                                                  const_iterator;
    typedef ReverseIterator< Iterator< vector<value_type>, typename vector<value_type>::iterator,
									   Field<GeomElem, DataType> > >                               reverse_iterator;
    typedef ReverseIterator< Iterator< vector<value_type>,
									   typename vector<value_type>::const_iterator,
									   Field<GeomElem, DataType> > >                               const_reverse_iterator;
		
    /// Constructor using default constructor of \a value_type.
    explicit Field(const Mesh& mesh)
			: mMesh(mesh), mData(numCentering<GeomElem>(mesh), value_type() )
	{
		boost::function_requires< boost::DefaultConstructibleConcept<value_type> >();
	}

    /// Constructor using \a data as initializer for \a DataType.
    Field(const Mesh& mesh, param_type data)
			: mMesh(mesh), mData(numCentering<GeomElem>(mesh), data)
	{
		boost::function_requires< boost::CopyConstructibleConcept<value_type> >();
	}
	
    /// Constructor using \a vector<DataType> \a data to initialize the field.
    Field(const Mesh& mesh, const vector<value_type>& data)
			: mMesh(mesh), mData(data)
	{
		Assert(mData.size() == numCentering<GeomElem>(mesh));
	}

	/// Copy constructor
	Field(const Field<GeomElem, DataType>& field_in)
			: mMesh(field_in.mMesh), mData(field_in.mData)
	{ }

	// Copy operator
	Field<GeomElem, DataType>& operator=(const Field<GeomElem, DataType>& field_in)
	{
		mData = field_in.mData;
		return *this;
	}
	
    /// Returns a reference to the data corresponding to element \a elem.
    reference operator[](const GeomElem& elem)
	{
		unsigned int id = elem.id().id;
		Assert(id < mData.size());
		return mData[id];
	}

    /// Returns a constant reference to the data corresponding to element \a elem.
    const_reference operator[](const GeomElem& elem) const
	{
		unsigned int id = elem.id().id;
		Assert(id < mData.size());
		return mData[id];
	}

    /// Returns the size of the Field (number of zones or nodes)
    const unsigned int size() { return mData.size(); }
    
	/// Return a non-constant iterator to the beginning of the field.
    iterator begin() { return iterator(mData, mData.begin()); }
	/// Return a constant iterator to the beginning of the field.
    const_iterator begin() const { return const_iterator(mData, mData.begin()); }

	/// Return a non-constant iterator to the end of the field.
    iterator end() { return iterator(mData, mData.end()); }
	/// Return a constant iterator to the end of the field.
    const_iterator end() const { return const_iterator(mData, mData.end()); }

	/// Return a non-constant reverse iterator to the beginning of the field.
    reverse_iterator rbegin() { return reverse_iterator( iterator(mData, mData.end()) ); }
	/// Return a constant reverse iterator to the beginning of the field. 
    const_reverse_iterator rbegin() const { return const_reverse_iterator( const_iterator(mData, mData.end()) ); }

	/// Return a non-constant reverse iterator to the end of the field.
    reverse_iterator rend() { return reverse_iterator( iterator(mData, mData.begin()) ); }
	/// Return a constant reverse iterator to the end of the field.
    const_reverse_iterator rend() const { return const_reverse_iterator( const_iterator(mData, mData.end()) ); }

    //Get const reference to Mesh
    const Mesh& getMesh() const { return mMesh; }
    
    //Given a position, return the Zone in which that position is located
    const Zone* getZone(double x) const {return mMesh.getZone(x); }
protected:
    const Mesh& mMesh; ///< Stores a constant reference to the Mesh.
    vector<value_type> mData; ///< Stores the data for the Field.
};

/// @}

#endif
	
