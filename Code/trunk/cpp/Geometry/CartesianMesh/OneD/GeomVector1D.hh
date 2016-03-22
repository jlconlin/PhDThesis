// Date Last Altered: $Date: 2008-08-04 22:55:41 -0600 (Mon, 04 Aug 2008) $
// Revision Number: $Revision: 344 $
//--------------------------------------------*-C++-*------------------------------------//
/*! \file GeomVector1D.hh
 *  \author Greg Davidson
 *	 \date June 13, 2008
 *
 *  \brief Provides the GeomVector<OneD> geometric vector class.
 *
 *  This file provides the GeomVector<OneD> class.  The GeomVector class implements a 
 *  one-dimensional geometric vector. */

#ifndef GEOMVECTOR1D_HH
#define GEOMVECTOR1D_HH

#include "GeomVector_base.hh"

/*! \addtogroup MeshMod Mesh Module
 *  @{  */

/*! \brief Implements a one-dimensional geometric vector.
 *
 *  This class implements a one-dimensional geometric vector, inheriting some
 *  functionality from the \c GeomVector_base<OneD> class. */
template<>
class GeomVector<OneD> : public GeomVector_base<OneD>
{
private:
   /// Defines the \c GeomVector_base<OneD> as the \c Base type.
   typedef GeomVector_base<OneD>    Base;

public:   
   /// Imports the \c GeomVector_base<OneD>::OrdinateType into this scope.
   typedef Base::OrdinateType       OrdinateType;

   /*! \brief Copy constructor.
    *
    *  Copy constructor.
    *  \param[in] geom_vec_in  The \c GeomVector<OneD> object to copy. */
   GeomVector(const GeomVector<OneD>& geom_vec_in)
      : GeomVector_base<OneD>(geom_vec_in.mI)
   {  }
   
   /*! \brief Constructor.  Takes in the i-ordinate.
    *
    *  Constructor.  Takes in the i-ordinate to construct the 
    *  \c GeomVector<OneD>.
    *  \param[in] i_in  The i-ordinate for the \c GeomVector<OneD> object. */
   explicit GeomVector(OrdinateType i_in = 0.0)
      : GeomVector_base<OneD>(i_in)
   {  }

   /*! \brief Copy operator.
    *
    *  Copy operator.
    *  \param[in] geom_vec_in  The \c GeomVector<OneD> object to copy.
    *  \return  A reference to this \c GeomVector<OneD>. */
   GeomVector<OneD>& operator=(GeomVector<OneD>& geom_vec_in)
   {
      Base::mI = geom_vec_in.mI;
      return *this;
   }

   /*! \brief Returns the dot product.
    *
    *  Returns the dot product between \a geom_vec_in and this
    *  \c GeomVector<OneD>.
    *  \param[in] geom_vec_in  The \c GeomVector<OneD> that we
    *                          are calculating the dot product with.
    *  \return  The dot product between this \c GeomVector<OneD> and
    *           \a geom_vec_in. */
   OrdinateType dot(const GeomVector<OneD>& geom_vec_in)
   {
      return geom_vec_in.mI * Base::mI;
   }
   
};

/*! \brief Returns the dot product of \a geomvec_1 and \a geomvec_2.
 *
 *  Calculate the dot product of two \c GeomVector<OneD> objects.
 *  \param[in] geomvec_1  The first GeomVector to calculate the dot product with.
 *  \param[in] geomvec_2  The second GeomVector to calculate the dot product with.
 *  \return  The dot product between \a geomvec_1 and \a geomvec_2. */
inline GeomVector<OneD>::OrdinateType dot(const GeomVector<OneD>& geomvec_1,
				                       		   const GeomVector<OneD>& geomvec_2)
{
	return geomvec_1.i() * geomvec_2.i();
}

/// @}

#endif

