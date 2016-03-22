/*HistSource.h
 $Author: jlconlin $
 $Id: Field_AddOns.hh 448 2009-01-22 22:08:28Z jlconlin $
 $Revision: 448 $
 $Date: 2009-01-22 15:08:28 -0700 (Thu, 22 Jan 2009) $

 This file defines some functions to simplify some access to the Field class.

 */

#ifndef FIELD_ADDONS_HH
#define FIELD_ADDONS_HH

/*GetFieldContainer will return a copy of the data contained by the field.*/
template<typename G, typename T, typename C>
C GetFieldContainer( const Field<G,T,C>& F ){
    C ReturnMe( F.size() );     // Return container with the same size as F

    // Copy data
    typename C::iterator cIter = ReturnMe.begin();
    typename Field<G,T,C>::const_iterator fIter = F.begin();
    for( ; cIter != ReturnMe.end(); ++cIter, ++fIter ){
        *cIter = *fIter;
    }

    return ReturnMe;
}

#endif

