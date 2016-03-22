// Date Last Altered: $Date: 2008-08-04 22:55:41 -0600 (Mon, 04 Aug 2008) $
// Revision Number: $Revision: 344 $
//--------------------------------------------*-C++-*------------------------------------//
/*! \file Distance.hh
 *  \author Greg Davidson
 *  \date June 4, 2008
 *  
 *  \brief Provides a bunch of overloaded distance function templates.
 * 
 *  This file provides a bunch of overloaded distance function templates for help with
 *  \c Iterator range-checking calculations. */

#ifndef DISTANCE_HH
#define DISTANCE_HH

#include <iterator>
#include <boost/utility/enable_if.hpp>
#include <boost/type_traits.hpp>
#include <boost/concept_check.hpp>
#include <boost/call_traits.hpp>

#include "MinimalContainerConcept.hh"

using std::iterator;
using boost::enable_if;
using boost::is_convertible;
using boost::function_requires;
using boost::ContainerConcept;
using boost::InputIteratorConcept;
using boost::call_traits;

/*! \brief Calculates the distance between an arbitrary iterator and a \c Cont::const_iterator.
 *
 *  This function calculates the distance between \a iter1, an arbitrary forward iterator, 
 *  and \a iter2, a \c Cont::const_iterator.
 *  \par Template Parameters: 
 *     <dl> <dt> \e Cont </dt>
 *          <dd> The container type \a iter2 belongs to. </dd>
 *          <dt> \e OtherIterator </dt>
 *          <dd> The type of \a iter1. </dd> </dl>
 *  \param[in] iter1 An arbitrary forward iterator.
 *  \param[in] iter2 A \c Cont::const_iterator iterator.
 *  \returns The signed distance between \a iter1 and \a iter2.  Note that this may be
 *           positive or negative.
 *  \remarks We use boost template metaprogramming to disable this function when
 *           the \c OtherIterator type and the \c Cont::const_iterator type are not
 *           convertible. */
template<typename Cont, typename OtherIterator>
inline typename Cont::const_iterator::difference_type 
distance(typename call_traits<OtherIterator>::param_type iter1, 
         const typename Cont::const_iterator& iter2,
         typename enable_if< is_convertible<OtherIterator, typename Cont::const_iterator> >::type* = 0)
{
  function_requires< MinimalContainerConcept<Cont> >();
  function_requires< InputIteratorConcept<typename Cont::const_iterator> >();
  function_requires< InputIteratorConcept<OtherIterator> >();  

  return distance(static_cast<typename Cont::const_iterator>(iter1), iter2);
}

/*! \brief Calculates the distance between a \c Cont::const_iterator and an arbitrary iterator.
 *
 *  This function calculates the distance between \a iter1, a \c Cont::const_iterator iterator, 
 *  and \a iter2, an arbitrary iterator.
 *  \par Template Parameters: 
 *     <dl> <dt> \e Cont </dt>
 *          <dd> The container type \a iter1 belongs to. </dd>
 *          <dt> \e OtherIterator </dt>
 *          <dd> The type of \a iter2. </dd> </dl>
 *  \param[in] iter1 A \c Cont::const_iterator iterator.
 *  \param[in] iter2 An arbitrary forward iterator.
 *  \returns  The signed distance between \a iter1 and \a iter2.  Note that this may be
 *            positive or negative.
 *  \remarks  We use boost template metaprogramming to disable this function when
 *            the \c OtherIterator type and the \c Cont::const_iterator type are not
 *            convertible. */
template<typename Cont, typename OtherIterator>
inline typename Cont::const_iterator::difference_type 
distance(const typename Cont::const_iterator& iter1, 
         typename call_traits<OtherIterator>::param_type iter2,
         typename enable_if< is_convertible<typename Cont::const_iterator, OtherIterator> >::type* = 0)
{
  function_requires< MinimalContainerConcept<Cont> >();
  function_requires< InputIteratorConcept<OtherIterator> >();
  function_requires< InputIteratorConcept<typename Cont::const_iterator> >(); 
  
  return distance(iter1, static_cast<typename Cont::const_iterator>(iter2));
}

/*! \brief Calculates the distance between two \c Cont::const_iterator iterators.
 *
 *  This function calculates the distance between two \c Cont::const_iterator 
 *  iterators \a iter1 and \a iter2.
 *  \par Template Parameters: 
 *     <dl> <dt> \e Cont </dt>
 *          <dd> The container type \a iter1 and \a iter2 belongs to. </dd> </dl>
 *  \param[in] iter1 A \c Cont::const_iterator iterator.
 *  \param[in] iter2 A \c Cont::const_iterator iterator.
 *  \returns The signed distance between \a iter1 and \a iter2.  Note that this may be
 *           positive or negative. */
template<typename Cont>
inline typename Cont::const_iterator::difference_type
distance(const typename Cont::const_iterator& iter1, const typename Cont::const_iterator& iter2)
{
  function_requires< MinimalContainerConcept<Cont> >();
  function_requires< InputIteratorConcept<typename Cont::const_iterator> >(); 
  
  return distance(iter1, iter2);
}

#endif


