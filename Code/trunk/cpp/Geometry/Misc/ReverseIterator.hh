// Date Last Altered: $Date: 2008-08-04 22:55:41 -0600 (Mon, 04 Aug 2008) $
// Revision Number: $Revision: 344 $
//--------------------------------------------*-C++-*------------------------------------//
/*! \file ReverseIterator.hh
 *  \author Greg Davidson
 *  \date February 6, 2006
 *  
 *  \brief Provides a general \c ReverseIterator reverse iterator adaptor.
 * 
 *  This file provides the \c ReverseIterator template class.  The \c ReverseIterator 
 *  class provides a reverse iterator over a general container.  This class is an iterator 
 *  adaptor, so if it is given a checked iterator, it too will be checked.  Note the the 
 *  STL also offers a \c std::reverse_iterator iterator adaptor.  However, I believe it 
 *  is flawed, in that its comparison operators are not templated such that 
 *  \c std::reverse_iterators and \c std::const_reverse_iterators can not be compared. */

#ifndef REVERSEITERATOR_HH
#define REVERSEITERATOR_HH

#include <boost/concept_check.hpp>
#include <boost/iterator/reverse_iterator.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/call_traits.hpp>
#include <boost/utility.hpp>

#include "IteratorExcept.hh"
#include "CheckedIteratorConcept.hh"

using boost::BidirectionalIteratorConcept;
using boost::enable_if;
using boost::call_traits;
using boost::addressof;

/*! \brief This class provides a general \c ReverseIterator iterator adaptor.
 * 
 *  The \c ReverseIterator template class provides a reverse iterator over a general 
 *  container.  This class is an iterator adaptor, so if it is given a checked iterator, 
 *  it too will be checked.  Note that the STL also offers a \c std::reverse_iterator
 *  iterator adaptor.  However, it is flawed, in that its comparison operators
 *  are not templated such that \c std::reverse_iterators and 
 *  \c std::const_reverse_iterators can be compared.
 *  \par Template Parameters:
 *     <dl> <dt> \e ForwardIterator </dt>
 *          <dd> The forward iterator that we are applying the \c ReverseIterator
 *               iterator adaptor to. </dd> </dl> */
template<typename ForwardIterator>
class ReverseIterator : public boost::reverse_iterator<ForwardIterator>
{
private:
   /*! \brief The \c ReverseIterator iterator adaptor requires that the 
    *         \c ForwardIterator satisfy the \c BidirectionalIteratorConcept concept. */
   BOOST_CLASS_REQUIRE(ForwardIterator, boost, BidirectionalIteratorConcept);
   
public:
   // ****** Type Definitions ******
   /// Define the \c iterator_type as the forward iterator.
   typedef ForwardIterator                              iterator_type;
   /// To emulate the boost library, we define the base type.
   typedef ForwardIterator                              base_type;
   /// Defines the reference to what \c ReverseIterator points to.
   typedef typename boost::reverse_iterator<base_type>
                                          ::reference   reference;
   /// Defines the value type to what \c ReverseIterator point to.
   typedef typename boost::reverse_iterator<base_type>
                                          ::value_type  value_type;


   /*! \brief Imports the \c Iterator_DereferenceOutOfRange exception class into 
    *         the \c ReverseIterator scope.  This is thrown when an out-of-range 
    *         dereference is attempted. */                                          
   typedef Iterator_DereferenceOutOfRange       DereferenceOutOfRange;
   /*! \brief Imports the \c RevIter_ReverseIncrementBeyondRange exception class into 
    *         the \c ReverseIterator scope.  This is thrown when a the \c ReverseIterator
    *         is incremented beyond the range of the underlying container. */                     
   typedef RevIter_ReverseIncrementBeyondRange  ReverseIncrementBeyondRange;
   /*! \brief Imports the \c RevIter_ReverseIncrementBeyondRange exception class into 
    *         the \c ReverseIterator scope.  This is thrown when a the \c ReverseIterator
    *         is decremented beyond the range of the underlying container. */                        
   typedef RevIter_ReverseDecrementBeyondRange  ReverseDecrementBeyondRange;
   /*! \brief Imports the \c RevIter_AdvanceBeyondRange exception class into 
    *         the \c ReverseIterator scope.  This is thrown when a the \c ReverseIterator
    *         is advanced beyond the range of the underlying container. */                        
   typedef Iterator_AdvanceBeyondRange          AdvanceBeyondRange;

   // ****** Constructors ******
   /*! \brief Constructor.  Takes in a container and a general reverse iterator.
    *
    *  Constructor.  Takes in a container (for range checking) and any
    *  reverse iterator.  This constructor is primarily for building
    *  range-checked <tt>ReverseIterator</tt>s using STL containers and STL
    *  reverse iterators.
    *  \param[in] cont_in  The container that we wish to iterate over.
    *  \param[in] iter_in  The non-range-checked reverse iterator that
                           we want to apply range-checking to. */
   template<typename Cont, typename ReverseIter>
   ReverseIterator(const Cont& cont_in, const ReverseIter& iter_in)
      : boost::reverse_iterator<base_type>( base_type(cont_in, iter_in.base() ) )
   {  }
   
   /*! \brief Constructor.  Takes in any forward iterator.
    *
    *  Constructor.  Takes in any forward iterator.
    *  \param[in] iter_in The forward iterator to use as a base. */
   template<typename OtherForwardIterator>
   ReverseIterator(const OtherForwardIterator& iter_in)
      : boost::reverse_iterator<base_type>( iter_in )
   {  }
   
   // ****** Equality Operator ******
   /*! \brief Equality operator.  Takes in any reverse iterator.
    *
    *  Equality operator.  Takes in any reverse iterator.  Operates
    *  by using the equality operator of the underlying forward
    *  iterators.
    *  \param[in] iter_in  The reverse iterator to test equality with. */
   template<typename ReverseIter>
   bool operator==(const ReverseIter& iter_in) const
   {
      return iter_in.base() == boost::reverse_iterator<base_type>::base();
   }

private:
   /*! \brief The \c boost:reverse_iterator iterator adaptor requires that
    *         \c iterator_core_access be a friend of this class. */
   friend class iterator_core_access;

   // ****** Private functions required by boost::iterator_adaptor ******   
   /*! \brief Returns a reference to what the \c ReverseIterator points to.
    *
    *  This method returns a reference to what the \c ReverseIterator point
    *  to.  This method is required by the \c boost::reverse_iterator
    *  iterator adaptor.
    *  \returns  A reference to whatever the \c ReverseIterator points to.
    *  \throw DereferenceOutOfRange If the \c ReverseIterator points to
    *                               a location outside of the range of
    *                               the container, and if the underlying
    *                               forward iterator has range-checking, 
    *                               this exception class gets thrown. */
   reference dereference() const throw(DereferenceOutOfRange)
   {
      try 
      {
         return boost::reverse_iterator<base_type>::dereference();
      }
      catch(DereferenceOutOfRange)
      {
         throw DereferenceOutOfRange(__LINE__, __FILE__);
      }
   }      
   
   /*! \brief Tests whether this \c ReverseIterator and \a iter_in are equal.
    *
    *  Uses the equality operator of the underlying forward iterators to
    *  check whether \a iter_in and this \c ReverseIterator are equal. This 
    *  method is required by the \c boost::reverse_iterator iterator adaptor.
    *  \return Whether \a iter_in and this \c ReverseIterator are equal. */
   template<typename OtherBase>
   bool equal(const ReverseIterator<OtherBase>& iter_in) const
   {
      return boost::reverse_iterator<base_type>::operator==(iter_in);
   }
   
   /*! \brief Increments the \c ReverseIterator.
    *
    *  Increments (one location backward) the \c ReverseIterator.  This method 
    *  is required by the \c boost::reverse_iterator iterator adaptor.
    *  \throw ReverseIncrementBeyondRange  If an increment beyond the range
    *                                      of the iterator is attempted, and if
    *                                      the underlying forward iterator has
    *                                      range checking this exception class gets 
    *                                      thrown. */
   void increment() throw(ReverseIncrementBeyondRange)
   {
      try
      {
         boost::reverse_iterator<base_type>::increment();
      }
      catch(Iterator_DecrementBeyondRange)
      {
         throw ReverseIncrementBeyondRange(__LINE__, __FILE__);
      }
   }
   
   /*! \brief Decrements the \c ReverseIterator.
    *
    *  Decrements (one location forward) the \c ReverseIterator.  This method
    *  is required by the \c boost::reverse_iterator iterator adaptor.
    *  \throw ReverseDecrementBeyondRange  If a decrement beyond the range
    *                                      of the iterator is attempted, and if
    *                                      the underlying forward iterator has
    *                                      range checking this exception class gets 
    *                                      thrown. */
   void decrement() throw(ReverseDecrementBeyondRange)
   {
      try
      {
         boost::reverse_iterator<base_type>::decrement();
      }
      catch(Iterator_IncrementBeyondRange)
      {
         throw ReverseDecrementBeyondRange(__LINE__, __FILE__, *this);
      }
   }
   
   /*! \brief Calculates the signed distance between this \c ReverseIterator and
    *  \a iter_in.
    *
    *  Calculates the signed distance between this \c ReverseIterator and
    *  another \c ReverseIterator (with a possibly different underlying forward
    *  iterator).  Note that this distance may be positive or negative.
    *  \param[in] reverse_iter_in The iterator to calculate the distance with.
    *  \return The signed distance between \a reverse_iter_in and this iterator.
               Note that this distance may be positive or negative. */
   template<typename OtherBase>
   typename boost::reverse_iterator<base_type>::difference_type
   distance_to(const ReverseIterator<OtherBase>& reverse_iter_in) const
   {
      function_requires< RandomAccessIteratorConcept<OtherBase> >();
      function_requires< RandomAccessIteratorConcept<base_type> >();
      
      return boost::reverse_iterator<base_type>::distance_to(reverse_iter_in);
   }

   /*! \brief Advances the \c ReverseIterator by a distance \a n.
    *
    *  Advances the \c ReverseIterator by a distance \a n.  Note that \a n may be
    *  either positive or negative.
    *  \param[in] n The distance to advance this \c ReverseIterator.
    *  \throw AdvanceBeyondRange If the advance would move the \c ReverseIterator beyond
    *                            the range of the container being iterator upon, and
    *                            if the underlying forward iterator has range-checking,
    *                            then this exception class will be thrown. */
   void advance(typename boost::reverse_iterator<base_type>::difference_type n)
                  throw(AdvanceBeyondRange)
   {
      function_requires< RandomAccessIteratorConcept<base_type> >();
      try
      {
         boost::reverse_iterator<base_type>::advance(n);
      }
      catch(AdvanceBeyondRange)
      {
         throw AdvanceBeyondRange(__LINE__, __FILE__, *this);
      }
   }
   
};   

// ****** Additional Operators ******
/*! \brief Equality operator between a \c ReverseIterator and another reverse iterator.
 *
 *  Tests for equality between \a iter1, a \c ReverseIterator, and \a iter2, a generic
 *  reverse iterator.
 *  \par Template Parameters:
 *     <dl> <dt> \e forward_iterator </dt>
 *          <dd> The base forward iterator for \a iter1. </dd>
 *          <dt> \e ReverseIter </dt>
 *          <dd> The reverse iterator type of \a iter2. </dd> </dl>
 *  \param[in] iter1 A generic reverse iterator to test equality with.
 *  \param[in] iter2 A \c ReverseIterator to test equality with.
 *  \return Whether these two reverse iterators are equal. */
template<typename forward_iterator, typename ReverseIter>
bool operator==(const ReverseIter& iter1, const ReverseIterator<forward_iterator>& iter2)
{
   return iter2.operator==(iter1);
}

/*! \brief Inequality operator between two \c ReverseIterator types.
 *
 *  Tests whether \a iter1 and \a iter2, both <tt>ReverseIterator</tt>s, are not equal.
 *  \par Template Parameters:
 *     <dl> <dt> \e forward_iterator1 </dt>
 *          <dd> The base forward iterator for \a iter1. </dd>
 *          <dt> \e forward_iterator2 </dt>
 *          <dd> The base forward iterator for \a iter2. </dd>
 *  \param[in] iter1 The first \c ReverseIterator to test for inequality.
 *  \param[in] iter2 The second \c ReverseIterator to test for inequality.
 *  \return Whether \a iter1 and \a iter2 are not equal. */
template<typename forward_iterator1, typename forward_iterator2>
bool operator!=(const ReverseIterator<forward_iterator1>& iter1,
                const ReverseIterator<forward_iterator2>& iter2)
{
   return !( iter1.operator==(iter2) );
}

/*! \brief Inequality operator between a \c ReverseIterator and a general reverse
 *  iterator.
 *
 *  Tests whether \a iter1, a \c ReverseIterator, and \a iter2, a generic reverse
 *  iterator are not equal.
 *  \par Template Parameters:
 *     <dl> <dt> \e forward_iterator </dt>
 *          <dd> The base forward iterator for \a iter1. </dd>
 *          <dt> \e ReverseIter </dt>
 *          <dd> The reverse iterator type of \a iter2. </dd> </dl>
 *  \param[in] iter1 The \c ReverseIterator to test for inequality.
 *  \param[in] iter2 The generic reverse iterator to test for inequality.
 *  \return Whether \a iter1 and \a iter2 are not equal. */
template<typename forward_iterator, typename ReverseIter>
bool operator!=(const ReverseIterator<forward_iterator>& iter1, const ReverseIter& iter2)
{
   return !( iter1.operator==(iter2) );
}

/*! \brief Inequality operator between a \c ReverseIterator and a general reverse
 *  iterator.
 *
 *  Tests whether \a iter1, a generic reverse iterator, and \a iter2, a 
 *  \c ReverseIterator are not equal.
 *  \par Template Parameters:
 *     <dl> <dt> \e forward_iterator </dt>
 *          <dd> The base forward iterator for \a iter2. </dd>
 *          <dt> \e ReverseIter </dt>
 *          <dd> The reverse iterator type of \a iter1. </dd> </dl>
 *  \param[in] iter1 The generic reverse iterator to test for inequality.
 *  \param[in] iter2 The \c ReverseIterator to test for inequality.
 *  \return Whether \a iter1 and \a iter2 are not equal. */
template<typename forward_iterator, typename ReverseIter>
bool operator!=(const ReverseIter& iter1, const ReverseIterator<forward_iterator>& iter2)
{
   return !( iter2.operator==(iter1) );
}
   

/// @}

#endif

