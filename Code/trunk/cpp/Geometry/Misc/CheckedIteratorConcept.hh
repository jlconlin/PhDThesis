// Date Last Altered: $Date: 2008-08-04 22:55:41 -0600 (Mon, 04 Aug 2008) $
// Revision Number: $Revision: 344 $
//--------------------------------------------*-C++-*------------------------------------//
/*! \file CheckedIteratorConcept.hh
 *  \author Greg Davidson
 *  \date May 17, 2008
 *  
 *  \brief Provides the \c CheckedIteratorConcept concept class.
 * 
 *  This file provides the \c CheckedIteratorConcept concept class.  This class dictates 
 *  what funtionality a checked iterator must implement. */

#ifndef CHECKEDITERATORCONCEPT_HH
#define CHECKEDITERATORCONCEPT_HH

/*! \brief The \c CheckedIteratorConcept class dictates what functionality a checked iterator
 *         must provide.
 *
 *  The purpose of the \c CheckedIteratorConcept concept class is to dictate what functionality
 *  any type used as a checked iterator must provide.  Concept classes are used with the
 *  boost concept checking utilities.  If any type is used as a checked iterator that does
 *  not fulfill the \c CheckedIteratorConcept requirements, the code will not compile
 *  and a comprehensible compiler error will result.
 *  \par Template Parameters: 
 *     <dl> <dt> \e CheckedIter </dt>
 *          <dd> The type that must implement the requirements of the 
 *               \c CheckedIteratorConcept class. </dd> </dl>
 *  \par Concept Requirements
 *       The following types must be provided by the \c CheckedIter type:
 *       <TABLE BORDER="0" CELLPADDING="0" CELLSPACING="0" WIDTH="100%">
 *          <TR>  <TD> \arg \c ContainerType </TD> 
 *                <TD> The container that the iterator iterates over. </TD> </TR>
 *          <TR>  <TD> \arg \c BaseType </TD> 
 *                 <TD> The underlying iterator type. </TD> </TR>
 *       </TABLE>
 *       The following functionality must be provided by the \c CheckIter type:
 *       \arg The \c CheckedIter type must satisfy the \c boost::InputIteratorConcept concept.
 *       \arg The \c ContainerType type must satisfy the \c boost::ContainerConcept concept.
 *  \remarks  It should be noted that this class is compiled but never executed, so concept
 *            checking does not imply any runtime overhead. */
template<typename CheckedIter>
class CheckedIteratorConcept
{
public:
   /// The \c CheckedIter must provide the \c ContainerType type.
   typedef typename CheckedIter::ContainerType  ContainerType;
   /// The \c CheckedIter must provide the \c BaseType type.
   typedef typename CheckedIter::BaseType       BaseType;
   
   /// The constraints method tests that the \c CheckedIter provides certain functionality.
   void constraints()
   {
      function_requires< InputIteratorConcept<CheckedIter> >();
      function_requires< ContainerConcept<ContainerType> >();
   }
};


#endif

