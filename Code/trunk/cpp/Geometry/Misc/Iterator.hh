// Date Last Altered: $Date: 2008-08-07 11:13:18 -0600 (Thu, 07 Aug 2008) $
// Revision Number: $Revision: 348 $
//--------------------------------------------*-C++-*------------------------------------//
/*! \file Iterator.hh
 *  \author Greg Davidson
 *  \date November 6, 2006
 * 
 *  \brief Provides the interface and implementation of the \c Iterator class.
 * 
 *  This file provides the interface and implementation of the \c Iterator class.
 *  The \c Iterator is a wrapper for STL iterators with optional range checking provided.
 *  Additionally, this \c Iterator prevents comparisons between \c Iterator objects that 
 *  belong to unrelated classes but share implementation details (i.e., we
 *  don't want to be able to compare \c Iterator objects between <tt>Zone</tt>-centered 
 *  <tt>Field</tt>s and <tt>Node</tt>-centered <tt>Field</tt>s.) */

#ifndef ITERATOR_HH
#define ITERATOR_HH

#include <iterator>
#include <boost/concept_check.hpp>
#include <boost/utility.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/iterator/iterator_facade.hpp>
#include <boost/type_traits.hpp>
#include <boost/call_traits.hpp>
#include <boost/static_assert.hpp>

#include "IteratorExcept.hh"
#include "Distance.hh"
#include "MinimalContainerConcept.hh"

using std::advance;
using std::iterator_traits;
using boost::function_requires;
using boost::ContainerConcept;
using boost::InputIteratorConcept;
using boost::BidirectionalIteratorConcept;
using boost::RandomAccessIteratorConcept;
using boost::addressof;
using boost::iterator_facade;
using boost::enable_if;
using boost::disable_if;
using boost::is_convertible;
using boost::call_traits;
using boost::prior;
using boost::is_integral;

/*! \brief Provides a type-safe \c Iterator with optional range checking.
 * 
 *  The \c Iterator class is a wrapper for STL iterators with range checking provided.
 *  Additionally, this \c Iterator class prevent comparisons between \c Iterator objects 
 *  that belong to unrelated classes but share implementation details (i.e., we
 *  don't want to be able to compare \c Iterator objects between <tt>Zone</tt>-centered 
 *  <tt>Field</tt>s and <tt>Node</tt>-centered <tt>Field</tt>s).
 *  \par Template Parameters:
 *     <dl> <dt> \e container_type </dt>
 *          <dd> The container type that is being iterator over.  This parameter must satisfy
 *               the \c MinimalContainerConcept concept. </dd>
 *          <dt> \e base_type </dt>
 *          <dd> The base iterator type.  This is the basic iterator that points into the
 *               container.  Typically, this is an iterator without range checking that we
 *               wish to add range checking to.  This parameter must satisfy the
 *               \c boost::InputIteratorConcept. </dd>  </dl>
 *  \remarks  To turn off range checking, compile with \b NITERCHK defined.  With \b NITERCHK 
 *            defined and compilation optimizations turned on, \c Iterator should have no 
 *            greater overhead than the underlying \c Iter type. */
template<typename container_type, typename base_type = typename container_type::const_iterator>
class Iterator : public iterator_facade< Iterator<container_type, base_type>, 
                                         typename iterator_traits<base_type>::value_type, 
                                         typename iterator_traits<base_type>::iterator_category,
                                         typename iterator_traits<base_type>::reference >
{
private:
   /*! \brief Requires that the \c container_type template parameter satisfy the 
    *         \c MinimalContainerConcept concept. */
   BOOST_CLASS_REQUIRE(container_type, , MinimalContainerConcept);
   /*! \brief Requires that the \c base_type template parameter satisfy the
    *         \c boost::InputIteratorConcept concept. */
   BOOST_CLASS_REQUIRE(base_type, boost, InputIteratorConcept);
   
public:
   // ****** Type Definitions ******
   /// Defines the type of container that is iterated over.
   typedef container_type                 ContainerType;
   /// Defines the type of basic iterator that underlies Iterator.
   typedef base_type                      BaseType;       
   /// Defines the value type that the Iterator points to.
   typedef typename BaseType::value_type  value_type;     
   /// Defines a reference type to what the Iterator points to.
   typedef typename BaseType::reference   reference;      

   /*! \brief Imports the \c Iterator_AssignmentOutOfRange exception class into the
    *         \c Iterator scope.  This is thrown when an assignment outside the
    *         <tt>ContainerType</tt>'s range is attempted. */
   typedef Iterator_AssignmentOutOfRange           AssignmentOutOfRange;
   /*! \brief Imports the \c Iterator_CheckedIterLacksContainer exception class into 
    *         the \c Iterator scope.  This is thrown when a range-check is needed
    *         but the \c Iterator has no container assigned. */
   typedef Iterator_CheckedIteratorLacksContainer  CheckedIteratorLacksContainer;
   /*! \brief Imports the \c Iterator_DereferenceOutOfRange exception class into 
    *         the \c Iterator scope.  This is thrown when an out-of-range dereference 
    *         is attempted. */
   typedef Iterator_DereferenceOutOfRange          DereferenceOutOfRange;
   /*! \brief Imports the \c Iterator_IncrementBeyondRange exception class into 
    *         the \c Iterator scope.  This is thrown when an \c Iterator is
    *         incremented beyond the <tt>ContainerType<tt>'s range. */
   typedef Iterator_IncrementBeyondRange           IncrementBeyondRange;
   /*! \brief Imports the \c Iterator_DecrementBeyondRange exception class into 
    *         the \c Iterator scope.  This is thrown when an \c Iterator is
    *         decremented beyond the <tt>ContainerType<tt>'s range. */
   typedef Iterator_DecrementBeyondRange           DecrementBeyondRange;
   /*! \brief Imports the \c Iterator_AdvanceBeyondRange exception class into 
    *         the \c Iterator scope.  This is thrown when an \c Iterator is
    *         advanced beyond the <tt>ContainerType<tt>'s range. */   
   typedef Iterator_AdvanceBeyondRange             AdvanceBeyondRange;

   // ****** Constructors ******
   /// Default constructor.
   Iterator()
      :  mContainer(), mIterator()
   {  }
   
   /*! \brief Constructor.  Takes in a container to iterate over.
    *
    *  Constructor.  Takes in a container to iterate over.
    *  \param[in]  container_in  The container that we will iterate over. */
   Iterator(const ContainerType& container_in)
      : mContainer( addressof(container_in) ),
        mIterator()
   {  }
   
   /*! \brief Constructor.  Takes in a container and another iterator.
    *
    *  Constructor.  Takes in a container and another iterator.
    *  \par Template Parameters:
    *     <dl> <dt> \e OtherBase </dt>
    *          <dd> The type of the other iterator. </dd> </dl>
    *  \param[in] container_in The container that we will iterate over.
    *  \param[in] iter_in      The other iterator that we copy.
    *  \throw AssignmentOutOfRange  Throws the \c AssignmentOutOfRange exception
    *                               class if the \c Iterator is assigned a location
    *                               outside the container's range.
    *  \remarks  This constructor uses the \c boost::enable_if template
    *            metaprogramming facility so that this constructor is only
    *            enabled if \c BaseType and \c OtherBase are convertible. */
   template<typename OtherBase>
   Iterator(const ContainerType& container_in, const OtherBase& iter_in,
            typename enable_if< is_convertible< OtherBase, BaseType> >::type* = 0 )
                  throw(AssignmentOutOfRange)
      :  mContainer( addressof(container_in) ),
         mIterator(iter_in)
   { 
      function_requires< InputIteratorConcept<OtherBase> >(); 
#ifndef NITERCHK
      if( distance<ContainerType>(mContainer->begin(), iter_in) < 0 
             || distance<ContainerType>(mContainer->begin(), iter_in) 
                     > static_cast< Int4 >(mContainer->size()) )
      throw AssignmentOutOfRange(__LINE__, __FILE__);
#endif
   }

   /*! \brief Copy constructor.  Takes in a container and an \c Iterator.
    *
    *  Copy constructor.  Takes in another \c Iterator.  It
    *  uses a template to try to convert the base of the copied \c Iterator
    *  into this \c Iterator.
    *  \par Template Parameters:
    *     <dl> <dt> \e OtherBase </dt>
    *          <dd> The base type of the other \c Iterator. </dd> </dl>
    *  \param[in]  container_in The container that we will iterate over.
    *  \param[in]  iter_in      The other \c Iterator that we copy.
    *  \remarks  This constructor uses the \c boost::enable_if template
    *            metaprogramming facility so that this constructor is only
    *            enabled if \c BaseType and \c OtherBase are convertible. */   
   template<typename OtherBase>
   Iterator(const Iterator<ContainerType, OtherBase>& iter_in,
            typename enable_if< is_convertible<OtherBase, BaseType> >::type* = 0 )
      :  mContainer(iter_in.mContainer),
         mIterator(iter_in.mIterator)
   { 
      function_requires< InputIteratorConcept<OtherBase> >();
   }
   
   // ****** Copy operators ******
   /*! \brief Copy operator.  Takes in an \c Iterator with a different \c Base.
    *
    *  Copy operator.  Takes in an \c Iterator with a different \c Base.  It uses
    *  a template to try to convert the \c Base of the copied \c Iterator
    *  into this \c Iterator.
    *  \par Template Parameters:
    *     <dl> <dt> \e OtherBase </dt>
    *          <dd> The base of the copied \c Iterator. </dd> </dl>
    *  \param[in] iter_in             The \c Iterator to copy.
    *  \return                        Returns a reference to this \c Iterator.
    *  \throw AssignmentOutOfRange    Throws the \c AssignmentOutOfRange
    *                                 exception class if the assignment puts
    *                                 the iterator out of the container's range.
    *                                 This will not be checked if \b NITERCHK is defined.
    *  \remarks  This function has a \c BOOST_STATIC_ASSERT that checks at
    *            compile time that the base type of the copied \c Iterator and
    *            the base type of this \c Iterator are convertible. */
   template<typename OtherBase>
   Iterator<ContainerType, BaseType>&
   operator=(const Iterator<ContainerType, OtherBase>& iter_in) 
                     throw(AssignmentOutOfRange)
   {
      BOOST_STATIC_ASSERT(( is_convertible< OtherBase, 
                                            BaseType>::value ));
      mContainer = iter_in.mContainer;
#ifndef NITERCHK
      if( distance<ContainerType>(mContainer->begin(), 
                                  iter_in.mIterator) < 0 
              || distance<ContainerType>(mContainer->begin(), 
                                         iter_in.mIterator) 
                     > static_cast< Int4 >(mContainer->size()) )
      throw AssignmentOutOfRange(__LINE__, __FILE__);
#endif
      mIterator = iter_in.mIterator;
      return *this;
   }

   /*! \brief Copy operator.  Takes in any iterator type.
    *
    *  Copy operator.  Takes in any iterator type.  It uses
    *  a template to try to convert the \c OtherBase type to the base type
    *  of this \c Iterator.
    *  \par Template Parameters:
    *     <dl> <dt> \e OtherBase </dt>
    *          <dd> The type of the iterator to copy. </dd> </dl>
    *  \param[in] base_in             The iterator to copy.
    *  \return                        Returns a reference to this Iterator.
    *  \throw CheckedIteratorLacksContainer  Throws the \c CheckedIteratorLacksContainer
    *                                 exception class if we copy into an \c Iterator
    *                                 that does not have a container assigned to it.
    *                                 This will not be checked if \b NITERCHK is defined.
    *  \throw AssignmentOutOfRange    Throws the \c AssignmentOutOfRange
    *                                 exception class if the assignment puts
    *                                 the \c Iterator out of the container's range.
    *                                 This will not be checked if \b NITERCHK is defined.
    *  \remarks  This function has a \c BOOST_STATIC_ASSERT that checks at
    *            compile time that the base type of the copied iterator and
    *            the base type of this iterator are convertible. It also uses
    *            concept checking to ensure that the copied iterator satisfies the
    *            \c boost::InputIteratorConcept concept. */
   template<typename OtherBase>
   Iterator<ContainerType, BaseType>& operator=(const OtherBase& base_in)
                        throw(CheckedIteratorLacksContainer, AssignmentOutOfRange)
   {
      BOOST_STATIC_ASSERT(( is_convertible< OtherBase, BaseType>::value ));
      function_requires< InputIteratorConcept<OtherBase> >();
#ifndef NITERCHK
      if(mContainer == 0) throw 
         CheckedIteratorLacksContainer(__LINE__, __FILE__);
      if( distance<ContainerType>(mContainer->begin(), 
                                  base_in) < 0 
               || distance<ContainerType>(mContainer->begin(), 
                                          base_in) 
                     > static_cast< Int4 >( mContainer->size()) )
      throw AssignmentOutOfRange(__LINE__, __FILE__);
#endif
      mIterator = base_in;
      return *this;
   }

   // ****** Getters and Setters ******
   /*! \brief Returns a constant reference to the container.
    *
    *  Returns a constant reference to the container we are iterating over.
    *  \return A reference to the container. */   
   const ContainerType& container() const 
   { 
      return *mContainer; 
   }
   
   /*! \brief Sets the container we are iterating over.
    *
    *  Sets the container we are iterating over.
    *  \param[in] cont_in  A constant reference to the container. */
   void setContainer(const ContainerType& cont_in)
   {
      mContainer = addressof(cont_in);
   }

   /*! \brief Returns a constant reference to the underlying non-range-checked iterator.
    *
    *  Returns a constant reference to the underlying non-range-checked iterator.
    *  \return  A constant reference to the underlying non-range-checked iterator. */
   const BaseType& base() const
   {
      return mIterator;
   }


private:
   /// \brief This is required by \c boost::iterator_facade.
   friend class boost::iterator_core_access;
   /// \brief This is required to permit fast copying and comparisons.
   template<typename AnyContainer, typename AnyBase> friend class Iterator;

   // ****** Private functions required by boost::iterator_facade ******
   /*! \brief Returns a reference to the object pointed at by \c Iterator.
    *
    *  Returns a reference to the object contained within the \c Container and pointed
    *  at by \c Iterator.  This method is required by the \c boost::iterator_facade class
    *  and called by that class's methods.
    *  \return A reference to the object pointed at by \c Iterator.
    *  \throw CheckedIteratorLacksContainer  If the \c Iterator has not been assigned a
    *                                        container when this function is called, then
    *                                        \c CheckedIteratorLacksContainer is thrown.
    *                                        This will not be checked if \b NITERCHK is defined.
    *  \throw DereferenceOutOfRange If the \c Iterator currently points to a location out of
    *                               the container's range, then \c DereferenceOutOfRange is
    *                               thrown.
    *                               This will not be checked if \b NITERCHK is defined. */
   reference dereference() const throw(CheckedIteratorLacksContainer, DereferenceOutOfRange)
   {
#ifndef NITERCHK
      if(mContainer == 0) throw 
         CheckedIteratorLacksContainer(__LINE__, __FILE__);
      if( mIterator == prior( mContainer->begin() ) 
               || mIterator == mContainer->end() )
         throw DereferenceOutOfRange(__LINE__, __FILE__);
#endif
      return *mIterator;
   }
   
   /*! \brief Returns a boolean indicating if \a iter_in equals this \c Iterator.
    *
    *  Returns \a true or \a false depending on whether \a iter_in and this \c Iterator
    *  are equal.  Checks both the base iterator and the container for
    *  equality.  This method is required by the \c boost::iterator_facade class
    *  and called by that class's methods.
    *  \param[in] iter_in  The \c Iterator that we are testing equality with.
    *  \return A boolean indicating whether \a iter_in and this \c Iterator are
    *          equal. */
   bool equal(const Iterator<ContainerType, BaseType>& iter_in) const
   {      
      return (mContainer == iter_in.mContainer && mIterator == iter_in.mIterator);
   }
   
   /*! \brief Increments the \c Iterator.
    *
    *  This method increments the \c Iterator and performs optional range checking.
    *  This method is required by the \c boost::iterator_facade class and called 
    *  by that class's methods.
    *  \throw CheckedIteratorLacksContainer  If this \c Iterator does not currently
    *                                        have a container assigned, this
    *                                        exception class is thrown.
    *                                        This will not be checked if \b NITERCHK is defined.
    *  \throw IncrementBeyondRange If an increment beyond the valid range of the container
    *                              is attempted, this exception class is thrown.
    *                              This will not be checked if \b NITERCHK is defined. */
   void increment() throw(CheckedIteratorLacksContainer, IncrementBeyondRange)
   {
#ifndef NITERCHK
      if(mContainer == 0) throw 
         CheckedIteratorLacksContainer(__LINE__, __FILE__);
      if(mIterator == mContainer->end())
         throw IncrementBeyondRange(__LINE__, __FILE__);
#endif
      ++mIterator;
   }

   /*! \brief Decrements the \c Iterator.
    *
    *  This method decrements the \c Iterator and performs optional range checking.
    *  This method is required by the \c boost::iterator_facade class and called 
    *  by that class's methods.
    *  \throw CheckedIteratorLacksContainer  If this \c Iterator does not currently
    *                                        have a container assigned, this
    *                                        exception class is thrown.
    *                                        This will not be checked if \b NITERCHK is 
    *                                        defined.
    *  \throw DecrementBeyondRange If a decrement beyond the valid range of the container
    *                              is attempted, this exception class is thrown.
    *                              This will not be checked if \b NITERCHK is defined. */      
   void decrement() throw(CheckedIteratorLacksContainer, DecrementBeyondRange)
   {
      function_requires< BidirectionalIteratorConcept<BaseType> >();
#ifndef NITERCHK
      if(mContainer == 0) throw 
         CheckedIteratorLacksContainer(__LINE__, __FILE__);
      if(mIterator == mContainer->begin())
         throw DecrementBeyondRange(__LINE__, __FILE__);
#endif
      --mIterator;
   }

   /*! \brief Advances the \c Iterator by \a n.
    *
    *  This method advances the \c Iterator by a distance \a n.  Note that \a n may
    *  be positive or negative.
    *  This method is required by the \c boost::iterator_facade class and called 
    *  by that class's methods.
    *  \param[in] n   The distance to advance the \c Iterator.
    *  \throw CheckedIteratorLacksContainer  If this \c Iterator does not currently
    *                                        have a container assigned, this
    *                                        exception class is thrown.
    *                                        This will not be checked if \b NITERCHK is 
    *                                        defined.
    *  \throw AdvanceBeyondRange If the advance will move the \c Iterator beyond the valid
    *                            range of the container, this exception class is thrown.
    *                            This will not be checked if \b NITERCHK is defined. */  
   void advance(typename BaseType::difference_type n) throw(CheckedIteratorLacksContainer,
                                                            AdvanceBeyondRange)
   {
      function_requires< RandomAccessIteratorConcept<BaseType> >();
#ifndef NITERCHK
      if(mContainer == 0) throw 
         CheckedIteratorLacksContainer(__LINE__, __FILE__);
      if( mIterator+n > mContainer->end() 
            || mIterator+n < mContainer->begin())
         throw AdvanceBeyondRange(__LINE__, __FILE__, n);
#endif
      mIterator+=n;
   }
   
   /*! \brief Calculates the distance between \a iter_in and this \c Iterator.
    *
    *  This method calculates the distance between \a iter_in and this \c Iterator.  The
    *  resulting distance may be either positive or negative.
    *  This method is required by the \c boost::iterator_facade class and called 
    *  by that class's methods.
    *  \param[in]  iter_in  The \c Iterator to calculate the distance with.
    *  \return The signed distance between this \c Iterator and \a iter_in. */
   typename BaseType::difference_type distance_to(const Iterator<ContainerType, BaseType>& iter_in) const
   {
      function_requires< RandomAccessIteratorConcept<BaseType> >();
      
      return iter_in.mIterator - mIterator;
   }
   
   // ****** Private data ******
   /// Pointer to the container being iterated over.
   const ContainerType* mContainer;
   /// The base iterator that underlies this \c Iterator.
   BaseType mIterator;
};


// ****** Difference Operators ******
/*! \brief Difference operator for <tt>Iterator</tt>s with (possibly) different container and 
 *         base types.
 *
 *  Difference operator.  Calculates the signed distance between two <tt>Iterator</tt>s with
 *  (possibly) different container and base types.
 *  \param[in] iter1  The first \c Iterator to calculate the distance with.
 *  \param[in] iter2  The second \c Iterator to calculate the distance with.
 *  \returns   The signed difference between these two iterators.
 *  \remarks   This method that the base types for both iterators satisfy the
 *             \c RandomAccessIteratorConcept concept class. */
template<typename container_type1, typename container_type2, typename base_type1, typename base_type2>
inline typename base_type1::difference_type
operator-(const Iterator<container_type1, base_type1>& iter1,
          const Iterator<container_type2, base_type2>& iter2)
{
   function_requires< RandomAccessIteratorConcept<base_type1> >();
   function_requires< RandomAccessIteratorConcept<base_type2> >();
   
   return iter1.base() - iter2.base();
}

/*! \brief Difference operator for an arbitrary iterator and an \c Iterator.
 *
 *  Difference operator.  Calculates the signed distance between an arbitrary iterator
 *  and an \c Iterator.
 *  \param[in] iter1  An arbitrary iterator type.
 *  \param[in] iter2  An \c Iterator.
 *  \returns   The signed difference between these two iterators.
 *  \remarks   This method that \a iter2 and the base type for \a iter1 both satisfy the
 *             RandomAccessIteratorConcept concept class. */
template<typename container_type, typename base_type, typename other_iterator>
inline typename base_type::difference_type
operator-(const other_iterator& iter1,
          const Iterator<container_type, base_type>& iter2)
{
   function_requires< RandomAccessIteratorConcept<other_iterator> >();
   function_requires< RandomAccessIteratorConcept<base_type> >();

   return iter1 - iter2.base();
}

/*! \brief Difference operator for an \c Iterator and an arbitrary iterator.
 *
 *  Difference operator.  Calculates the signed distance between an \c Iterator and an
 *  arbitrary iterator.
 *  \param[in] iter1  An \c Iterator.
 *  \param[in] iter2  An arbitrary iterator.
 *  \returns   The signed difference between these two iterators.
 *  \remarks   This method that \a iter1 and the base type for \a iter2 both satisfy the
 *             RandomAccessIteratorConcept concept class. */
template<typename container_type, typename base_type, typename other_iterator>
inline typename disable_if< is_integral<other_iterator>, 
                            typename base_type::difference_type >::type 
operator-(const Iterator<container_type, base_type>& iter1,
          const other_iterator& iter2)
{
   function_requires< RandomAccessIteratorConcept<other_iterator> >();
   function_requires< RandomAccessIteratorConcept<base_type> >();
   
   return iter1.base() - iter2;
}



// ****** Equality Operators ******
/*! \brief  Tests whether two <tt>Iterator<tt>s with different container and base types
 *          are equal.
 *
 *  Returns whether two <tt>Iterator<tt>s with different container and/or base types
 *  are equal (point to the same location in memory).
 *  \param[in] iter1  The first \c Iterator to test for equality.
 *  \param[in] iter2  The second \c Iterator to test for equality.
 *  \return  Whether these two <tt>Iterator</tt>s are equal. */ 
template<typename container_type1, typename container_type2, typename base_type1,
         typename base_type2>
inline bool operator==(const Iterator<container_type1, base_type1>& iter1,
                       const Iterator<container_type2, base_type2>& iter2)
{
   return iter1.base() == iter2.base();
}

/*! \brief  Tests whether an \c Iterator and an arbitrary iterator are equal.
 *
 *  Returns whether an \c Iterator and an arbitrary iterator are equal (point to 
 *  the same location in memory).
 *  \param[in] iter1  The \c Iterator to test for equality.
 *  \param[in] iter2  The arbitrary iterator to test for equality.
 *  \return  Whether these two iterators are equal. */
template<typename container_type, typename base_type, typename other_iterator>
inline bool operator==(const Iterator<container_type, base_type>& iter1,
                       const other_iterator& iter2)
{
   function_requires< InputIteratorConcept<base_type> >();
   function_requires< InputIteratorConcept<other_iterator> >();

   return iter1.base() == iter2;
}

/*! \brief  Tests whether an arbitrary iterator and an \c Iterator are equal.
 *
 *  Returns whether an arbitrary iterator and an \c Iterator are equal (point to 
 *  the same location in memory).
 *  \param[in] iter1  The arbitrary iterator to test for equality.
 *  \param[in] iter2  The \c Iterator to test for equality. 
 *  \return  Whether these two iterators are equal. */
template<typename container_type, typename base_type, typename other_iterator>
inline bool operator==(const other_iterator& iter1,
                       const Iterator<container_type, base_type>& iter2)
{
   function_requires< InputIteratorConcept<base_type> >();
   function_requires< InputIteratorConcept<other_iterator> >();

   return iter1 == iter2.base();
}

/*! \brief  Tests whether two <tt>Iterator<tt>s with different container and base types
 *          are not equal.
 *
 *  Returns whether two <tt>Iterator<tt>s with different container and/or base types
 *  are not equal (do not point to the same location in memory).
 *  \param[in] iter1  The first \c Iterator to test for inequality.
 *  \param[in] iter2  The second \c Iterator to test for inequality.
 *  \return  Whether these two <tt>Iterator</tt>s are not equal. */ 
template<typename container_type1, typename container_type2, typename base_type1,
         typename base_type2>
inline bool operator!=(const Iterator<container_type1, base_type1>& iter1,
                       const Iterator<container_type2, base_type2>& iter2)
{
   return !( iter1.base() == iter2.base() );
}

/*! \brief  Tests whether an \c Iterator and an arbitrary iterator are not equal.
 *
 *  Returns whether an \c Iterator and an arbitrary iterator are not equal (do not 
 *  point to the same location in memory).
 *  \param[in] iter1  The \c Iterator to test for inequality.
 *  \param[in] iter2  The arbitrary iterator to test for inequality.
 *  \return  Whether these two iterators are not equal. */
template<typename container_type, typename base_type, typename other_iterator>
inline bool operator!=(const Iterator<container_type, base_type>& iter1,
                       const other_iterator& iter2)
{
   function_requires< InputIteratorConcept<base_type> >();
   function_requires< InputIteratorConcept<other_iterator> >();

   return !( iter1.base() == iter2 );
}

/*! \brief  Tests whether an arbitrary iterator and an \c Iterator are not equal.
 *
 *  Returns whether an arbitrary iterator and an \c Iterator are not equal (do not point to 
 *  the same location in memory).
 *  \param[in] iter1  The arbitrary iterator to test for inequality.
 *  \param[in] iter2  The \c Iterator to test for inequality. 
 *  \return  Whether these two iterators are not equal. */
template<typename container_type, typename base_type, typename other_iterator>
inline bool operator!=(const other_iterator& iter1,
                       const Iterator<container_type, base_type>& iter2)
{
   function_requires< InputIteratorConcept<base_type> >();
   function_requires< InputIteratorConcept<other_iterator> >();

   return !( iter1 == iter2.base() );
}


/// @}

#endif


