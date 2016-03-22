// Date Last Altered: $Date: 2008-08-04 22:55:41 -0600 (Mon, 04 Aug 2008) $
// Revision Number: $Revision: 344 $
//--------------------------------------------*-C++-*------------------------------------//
/*! \file SweepIterator1D.hh
 *  \author Greg Davidson
 *  \date February 6, 2006
 * 
 *  \brief Provides a one-dimensional sweep iterator.
 *
 *  This file provides the \c SweepIterator<OneD> class.  This class provides the 
 *  ability to iterate over the \c Zone<OneD> objects in the \c CartesianMesh<OneD>
 *  in a sweeping pattern, dependent upon the streaming angle.  This class receives
 *  some functionality from \c SweepIterator_base<OneD> through inheritance. */

#ifndef SWEEPITERATOR1D_HH
#define SWEEPITERATOR1D_HH

#include <boost/iterator/iterator_facade.hpp>

#include "SweepIterator_base.hh"
#include "Angle1D.hh"

using boost::iterator_facade;

/*! \addtogroup MeshMod Mesh Module
 *  @{ */
 
/*! \brief Iterates over the <tt>Zone</tt>s in a one-dimensional \c CartesianMesh in a sweep 
 *         pattern.
 *
 *  The \c SweepIterator<OneD> class iterates over the \c Zone<OneD> objects stored in
 *  a \c CartesianMesh<OneD> in the correct order for a sweeping pattern given the streaming 
 *  angle \f$ \mu \f$.  If \b NITERCHK is defined, range checking for this iterator will be 
 *  disabled. */
template<>
class SweepIterator<OneD, ptr_vector< Zone<OneD> > > 
                                 : public SweepIterator_base<OneD, ptr_vector< Zone<OneD> > >,
                                   public iterator_facade< SweepIterator<OneD, ptr_vector< Zone<OneD> > >,
                                                           Zone<OneD>,
                                                           boost::forward_traversal_tag,
                                                           const Zone<OneD>& >
{
private:
   /// Defines the \c Base type.
   typedef SweepIterator_base<OneD, ptr_vector< Zone<OneD> > >    Base;
   /// Defines the \c Zone type.
   typedef Zone<OneD>                  Zone;
   
public:
   /// Imports the \c AngleType from the \c Base type.
   typedef Base::AngleType       AngleType;
   /// Defines the \c const_reference type for this iterator.
   typedef const Zone&           const_reference;

   /*! \brief Bring the \c IncrementBeyondRange exception class into the \c SweepIterator scope.
    *         Thrown when the iterator is incremented beyond the range of the \c CartesianMesh. */
   typedef Iterator_IncrementBeyondRange     IncrementBeyondSweepRange;
   /*! \brief Bring the \c DereferenceOutOfRange exception class into the \c SweepIterator scope.
    *         Thrown when an out-of-range \c SweepIterator is dereferenced. */
   typedef Iterator_DereferenceOutOfRange    DereferenceOutOfSweepRange;  

	/// Default constructor.
	SweepIterator()
			: SweepIterator_base<OneD, ptr_vector< Zone > >()
	{  }

	/*! \brief Constructor.  Takes an angle, the container reference, and a \c const_iterator.
	 *
	 *  Constructor. Takes an angle, a reference to the container holding the \c Zone
	 *  objects, and a \c const_iterator for the container.
	 *  \param[in] angle_in  The \f$ S_N \f$ quadrature angle of the sweep.
	 *  \param[in] cont_in   The container holding the \c Zone objects.
	 *  \param[in] iter_in   The \c ContainerType::const_iterator over the container of 
	                     \c Zone objects. */
   SweepIterator(const Angle<OneD>& angle_in, const ContainerType& cont_in,
	              const ContainerType::const_iterator& iter_in)
	   : SweepIterator_base<OneD, ptr_vector< Zone > >(angle_in, cont_in, iter_in)
	{  }
        
   /*! \brief Copy constructor.
    *
    *  Copy constructor.
	 *  \param[in] iter_in  The \c SweepIterator to be copied. */
    SweepIterator(const SweepIterator<OneD, ptr_vector< Zone > >& iter_in)
			: SweepIterator_base<OneD, ptr_vector< Zone > >( iter_in.mAngle, iter_in.mContainer, 
			                            iter_in.mIterator )
	{  }

private:
	/// Necessary to use \c boost::iterator_facade.
	friend class boost::iterator_core_access;

	/*! \brief Increments the iterator.
	 *
	 *  Increments the iterator.  Used by \c boost::iterator_facade.
	 *  \throw IncrementBeyondSweepRange  Throws this exception class if the iterator is incremented 
	 *                                    out of the container's range. */
	void increment() throw(IncrementBeyondSweepRange)
	{
		if(mAngle.mu() > 0)  // Positive angle.
		{
#ifndef NITERCHK
			if(mIterator == mContainer->end())
				throw IncrementBeyondSweepRange(__LINE__, __FILE__);
#endif
			++mIterator;
		}					  
		else                 // Negative angle.
		{
#ifndef NITERCHK
			if(mIterator == mContainer->begin())
				throw IncrementBeyondSweepRange(__LINE__, __FILE__);
#endif
			--mIterator;
		}
	}
	
	
   /*! \brief Test for equality.
    *
    *  Tests whether two \c SweepIterator objects are equal
	 *  \param[in] iter_in  The iterator to test for equality. 
	 *  \return    Whether \a iter_in is equal to this \c SweepIterator. */
	bool equal(const SweepIterator<DimensionType, ContainerType>& iter_in) const
	{
	   typedef AngleType::AngleType::OrdinateType  AngleOrdType;
	   
	   if( fabs(iter_in.mAngle.mu() - Base::mAngle.mu()) < 1.0e-6 )
	      return (iter_in.mContainer == Base::mContainer && iter_in.mIterator == Base::mIterator);
	   else
	      return false;
	}

   /*! \brief Return a \c const_reference to the object pointed at by this \c SweepIterator.
    *
    *  Returns a \c const_reference to the object pointed at by this \c SweepIterator.
    *  \return A \c const_reference to the object pointed at by this \c SweepIterator. 
    *  \throw  DereferenceOutOfSweepRange  Throws this exception is a dereference of an
    *                                      out-of-range \c SweepIterator is attempted. */
	const_reference dereference() const throw(DereferenceOutOfSweepRange)
   {
#ifndef NITERCHK
      if(mAngle.mu() > 0)
      {
         if( mIterator == mContainer->begin()-1 || mIterator == mContainer->end() )
            throw DereferenceOutOfSweepRange(__LINE__, __FILE__);
      }
      else
      {
         if( mIterator == mContainer->begin() || mIterator == mContainer->end()+1 )
            throw DereferenceOutOfSweepRange(__LINE__, __FILE__);
      }
#endif
      if(mAngle.mu() > 0)
         return *mIterator;
      else
         return *(mIterator-1);
   }
};

/// @}

#endif


