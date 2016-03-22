// Date Last Altered: $Date: 2008-08-05 12:40:59 -0600 (Tue, 05 Aug 2008) $
// Revision Number: $Revision: 346 $
//--------------------------------------------*-C++-*------------------------------------//
/*! \file MinimalContainerConcept.hh
 *  \author Greg Davidson
 *  \date June 24, 2008
 *  
 *  \brief Provides the \c MinimalContainerConcept concept class.
 * 
 *  This file provides the \c MinimalContainerConcept concept class.  This class dictates 
 *  what the minimum funtionality a container must implement to be used for range-checked
 *  iterators and for the underlying storage of the \c Field class. */

#ifndef MINIMALCONTAINERCONCEPT_HH
#define MINIMALCONTAINERCONCEPT_HH

/*! \brief Dictates the minimal functionality a container must offer for some functions.
 *
 *  The purpose of the \c MinimalContainerConcept concept class is to dictate what functionality
 *  a container must offer to be used with a range-checked iterator and with the \c Field classes.
 *  \par Template Parameters:
 *     <dl> <dt> \e container_type </dt>
 *          <dd> The type that must implement the requirements of the \c MinimalContainerConcept
 *               concept class. </dd> </dl>
 *  \par Concept Requirements
 *       The following types must be provided by the \a container_type type:
 *       <TABLE BORDER="0" CELLPADDING="0" CELLSPACING="0" WIDTH="100%">
 *          <TR> <TD> \arg \c iterator </TD>
 *               <TD> The container must have an \c iterator type. </TD> </TR>
 *          <TR> <TD> \arg \c const_iterator </TD>
 *               <TD> The container must have a \c const_iterator type. </TD> </TR>
 *          <TR> <TD> \arg \c reverse_iterator </TD>
 *               <TD> The container must have a \c reverse_iterator type. </TD> </TR>
 *          <TR> <TD> \arg \c const_reverse_iterator </TD>
 *               <TD> The container must have a \c const_reverse_iterator type. </TD> </TR>
 *          <TR> <TD> \arg \c size_type </TD>
 *               <TD> The container must have a \c size_type. </TD> </TR>
 *          <TR> <TD> \arg \c value_type </TD>
 *               <TD> The container must have a \c value_type. </TD> </TR>
 *          <TR> <TD> \arg \c reference </TD>
 *               <TD> The container must have a \c reference type. </TD> </TR>
 *          <TR> <TD> \arg \c const_reference </TD>
 *               <TD> The container must have a \c const_reference type. </TD> </TR>
 *       </TABLE>
 *       The following functionality must be provided by the \c container_type type:
 *       \arg The \c container_type must have a \c begin() method.
 *       \arg The \c container_type must have an \c end() method.
 *       \arg The \c container_type must have a \c rbegin() method.
 *       \arg The \c container_type must have a \c rend() method. 
 *       \arg The \c container_type must have a \c size() method.
 *       \arg The \c container_type must have an \c empty() method.
 *       \arg The \c container_type must have a \c swap() method. 
 *  \remarks  It should be noted that this class is compiled but never executed, so concept
 *            checking does not imply any runtime overhead. */
template<typename container_type>
class MinimalContainerConcept
{
public:
   /// Aliases the \c container_type.
   typedef container_type                                   ContainerType;
   /// The \c container_type must provide an \c iterator type.
   typedef typename ContainerType::iterator                 iterator;
   /// The \c container_type must provide a \c const_iterator type.
   typedef typename ContainerType::const_iterator           const_iterator;
   /// The \c container_type must provide a \c reverse_iterator type.
   typedef typename ContainerType::reverse_iterator         reverse_iterator;
   /// The \c container_type must provide a \c const_reverse_iterator type.
   typedef typename ContainerType::const_reverse_iterator   const_reverse_iterator;
   /// The \c container_type must provide a \c size_type type.
   typedef typename ContainerType::size_type                size_type;
   /// The \c container_type must provide a \c value_type type.
   typedef typename ContainerType::value_type               value_type;
   /// The \c container_type must provide a \c reference type.
   typedef typename ContainerType::reference                reference;
   /// The \c container_type must provide a \c const_reference type.
   typedef typename ContainerType::const_reference          const_reference;
   
   /*! \brief This method tests that the \c container_type fulfills certain 
    *         functional requirements */
   void constraints()
   {
      mConstIterator          = mContainer->begin();
      mConstIterator          = mContainer->end();
//    mConstReverseIterator   = mContainer->rbegin();
//    mConstReverseIterator   = mContainer->rend();
      mSize                   = mContainer->size();
      mEmpty                  = mContainer->empty();
      mContainer->swap( *mContainer );
   }    

private:
   /// Stores a pointer to the \c ContainerType for testing.
   ContainerType*          mContainer;
   /*! \brief Used to test that the \c begin() and \c end() methods return a
    *         \c const_iterator. */
   const_iterator          mConstIterator;
   /// Used to test that the \c size() method returns a \c size_type.
   size_type               mSize;
   /*! \brief Used to test that the \c rbegin() and \c rend() methods return
    *         a \c const_reverse_iterator. */
   const_reverse_iterator  mConstReverseIterator;
   /// \brief Used to test that the \c empty() method returns a boolean.
   bool                    mEmpty;
};

#endif


