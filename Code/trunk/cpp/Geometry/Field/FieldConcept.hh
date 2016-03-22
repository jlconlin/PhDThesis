// Date Last Altered: $Date: 2008-08-04 22:55:41 -0600 (Mon, 04 Aug 2008) $
// Revision Number: $Revision: 344 $
//--------------------------------------------*-C++-*------------------------------------//
/*! \file FieldConcept.hh
 *  \author Greg Davidson
 *  \date November 6, 2006
 *  
 *  \brief Provides the \c FieldConcept concept class.
 * 
 *  This file provides the \c FieldConcept concept class.  This class is used with the
 *  Boost library's concept checking functionality to check whether a given type provides
 *  the minimal functionality of a \c Field. */

#ifndef FIELDCONCEPT_HH
#define FIELDCONCEPT_HH

#include <boost/concept_check.hpp>

/*! \addtogroup FieldMod Field Module
 *  @{  */
 
/*! \brief Ensures that the \c field_type provides the functionality of a \c Field.
 *
 *  The \c FieldConcept class template dictates what functionality a type must implement
 *  to satisfy a \c Field concept.
 *  \par Template Parameters: 
 *     <dl> <dt> \e field_type </dt> 
 *          <dd> The type to check for \c Field concept conformity. </dd> </dl> 
 *  \par Concept Requirements:
 *       The following types must be provided by the \c field_type type:
 *       <TABLE BORDER="0" CELLPADDING="0" CELLSPACING="0" WIDTH="100%">
 *          <TR>  <TD> \arg \c CenteringType </TD> 
 *                <TD> The \c field_type type must provide a \c CenteringType. </TD> </TR>
 *          <TR>  <TD> \arg \c MeshType </TD>
 *                <TD> The \c field_type must provide a \c MeshType. </TD> </TR>
 *          <TR>  <TD> \arg \c ContainerType </TD>
 *                <TD> The \c field_type must provide a \c ContainerType. </TD> </TR>
 *          <TR>  <TD> \arg \c DataType </TD>
 *                <TD> The \c field_type must provide a \c DataType. </TD> </TR>
 *          <TR>  <TD> \arg \c value_type </TD>
 *                <TD> The \c field_type must provide a \c value_type. </TD> </TR>
 *          <TR>  <TD> \arg \c reference </TD>
 *                <TD> The \c field_type must provide a \c reference type. </TD> </TR>
 *          <TR>  <TD> \arg \c const_reference </TD>
 *                <TD> The \c field_type must provide a \c const_reference type. </TD> </TR>
 *          <TR>  <TD> \arg \c pointer </TD>
 *                <TD> The \c field_type must provide a \c pointer type. </TD> </TR>
 *          <TR>  <TD> \arg \c size_type </TD>
 *                <TD> The \c field_type must provide a \c size_type. </TD> </TR>
 *          <TR>  <TD> \arg \c iterator </TD>
 *                <TD> The \c field_type must provide an \c iterator type. </TD> </TR>
 *          <TR>  <TD> \arg \c const_iterator </TD>
 *                <TD> The \c field_type must provide a \c const_iterator type. </TD> </TR>
 *          <TR>  <TD> \arg \c reverse_iterator </TD>
 *                <TD> The \c field_type must provide a \c reverse_iterator type. </TD> </TR>
 *          <TR>  <TD> \arg \c const_reverse_iterator </TD>
 *                <TD> The \c field_type must provide a \c const_reverse_iterator type. </TD> </TR>
 *       </TABLE>
 *       The following functionality must be provided by the \c mesh_type type:
 *       \arg The \c field_type must provide a constructor taking a \c CartesianMesh.
 *       \arg The \c field_type must provide a constructor taking a \c CartesianMesh and data.
 *       \arg The \c field_type must provide a constructor taking a \c CartesianMesh and a container of data.
 *       \arg The \c field_type must provide a copy constructor.
 *       \arg The \c field_type must provide a copy operator.
 *       \arg The \c field_type must provide a \c size() method.
 *       \arg The \c field_type must provide a \c maxsize() method.
 *       \arg The \c field_type must provide an \c empty() method.
 *       \arg The \c field_type must provide a \c swap() method.
 *       \arg The \c field_type must provide a \c begin() method.
 *       \arg The \c field_type must provide an \c end() method.
 *       \arg The \c field_type must provide a \c rbegin() method.
 *       \arg The \c field_type must provide a \c rend() method.
 *  \remarks  It should be noted that this class is compiled but never executed, so concept
 *            checking does not imply any runtime overhead. */
template<typename field_type>
class FieldConcept
{
public:
   /// Define the \c field_type template parameter as \c FieldType.
   typedef field_type                                  FieldType;
   /// Requires the \c field_type to provide a \c CenteringType.
   typedef typename FieldType::CenteringType           CenteringType;
   /// Requires the \c field_type to provide a \c MeshType for the corresponding \c CartesianMesh.
   typedef typename FieldType::MeshType                MeshType;
   /// Requires the \c field_type to provide a \c ContainerType for storing the data.
   typedef typename FieldType::ContainerType           ContainerType;
   /// Requires the \c field_type to provide a \c DataType.
   typedef typename FieldType::DataType                DataType;
   /// Requires the \c field_type to provide a \c value_type (same as \c DataType).
   typedef typename FieldType::value_type              value_type;
   /// Requires the \c field_type to provide a \c reference type.
   typedef typename FieldType::reference               reference;
   /// Requires the \c field_type to provide a \c const_reference type.
   typedef typename FieldType::const_reference         const_reference;
   /// Requires the \c field_type to provide a \c pointer type.
   typedef typename FieldType::pointer                 pointer;
   /// Requires the \c field_type to provide a \c size_type.
   typedef typename FieldType::size_type               size_type;
   /// Requires the \c field_type to provide an \c iterator type.
   typedef typename FieldType::iterator                iterator;
   /// Requires the \c field_type to provide an \c const_iterator type.
   typedef typename FieldType::const_iterator          const_iterator;
   /// Requires the \c field_type to provide an \c reverse_iterator type.
   typedef typename FieldType::reverse_iterator        reverse_iterator;
   /// Requires the \c field_type to provide an \c const_reverse_iterator type.
   typedef typename FieldType::const_reverse_iterator  const_reverse_iterator;   

   // ****** Tests the required mesh functionality ******
   /*! \brief The constraints method tests that the \c field_type 
    *         provides certain functionality. */
   void constraints()
   {
      // Test constructors...
      field_type field1(*mMesh);
      field_type field2(*mMesh, *mData);
      field_type field3(*mMesh, *mContainer);
      field_type field4(field1);
         
      // Test equality operator;
      field1 = field2;
      
      mSize = field1.size();
      mSize = field1.max_size();
      mBool = field1.empty();
      field1.swap(field2);
      
      *mIterator        = field1.begin();
      *mIterator        = field1.end();
      *mReverseIterator = field1.rbegin();
      *mReverseIterator = field1.rend();
   }
   
private:
   /// Stores a \c CartesianMesh pointer for testing the constructors
   MeshType*         mMesh;
   /// Stores a \c DataType pointer for testing the constructors.
   DataType*         mData;
   /// Stores a \c ContainerType pointer for testing the constructors.
   ContainerType*    mContainer;
   /// Stores a \c size_type for testing the \c size() and \c maxsize() methods.
   size_type         mSize;
   /// Stores a \c bool for testing the \c empty() method.
   bool              mBool;
   /// Stores an \c iterator pointer for testing the \c begin() and \c end() methods.
   iterator*         mIterator;
   /// Stores a \c reverse_iterator pointer for testing the \c rbegin() and \c rend() methods.
   reverse_iterator* mReverseIterator;
};      


/// @}

#endif



