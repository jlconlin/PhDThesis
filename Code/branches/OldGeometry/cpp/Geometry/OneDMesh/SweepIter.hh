// Date Last Altered: $Date: 2006-11-15 20:51:22 -0500 (Wed, 15 Nov 2006) $
// Revision Number: $Revision: 39 $
//--------------------------------------------*-C++-*------------------------------------//
/// \file: SweepIter.hh
/// \author Greg Davidson
/// \date February 6, 2006
/// 
/// \brief This file provides the sweep iterator class.
///
/// This file provides the SweepIter class.  This class provides the ability to
/// iterate over the zones in a sweeping pattern, dependent upon the streaming
/// angle.

#ifndef SWEEPITER_HH
#define SWEEPITER_HH

#include <iterator>
#include <cmath>
#include <vector>
#include <limits>

#include <boost/utility.hpp>

#include "OneDCartMesh.hh"
#include "IteratorExcept.hh"

using std::iterator;
using std::vector;
using std::fabs;
using std::numeric_limits;
using boost::addressof;

/**  \addtogroup MeshSys Mesh Subsystem
 *   @{
 */
/// \brief  The SweepIter class iterates over the zones in a sweep pattern.
///
/// The SweepIter class iterates over the zones in the correct order for a sweeping pattern
/// given the streaming angle \f$ \mu \f$.
class SweepIter : public iterator< std::bidirectional_iterator_tag, Zone >
{
public:
	/// Default constructor.
	SweepIter()
	: mMu(0), mCont(0), mIter() {  }

	/// Constructor taking an angle, a reference to the zone container, and a const_iterator.
    SweepIter(double mu, const vector<Zone>& cont_in, const vector<Zone>::const_iterator iter_in)
        : mMu(mu), mCont( addressof(cont_in) ), mIter(iter_in) {  }
        
    /// Copy constructor
    SweepIter(const SweepIter& iter_in)
        : mMu(iter_in.mMu), mCont( iter_in.mCont ), mIter(iter_in.mIter) {  }
    
    /// Retrieve the iterator into the zone vector
    const vector<Zone>::const_iterator& iterator() const { return mIter; }
    /// Retrieve the container
    const vector<Zone>& container() const { return *mCont; }
    /// Retrieve the angle
    double angle() const { return mMu; }

    /// Copy operator
    SweepIter& operator=(const SweepIter& iter_in)
    {
        mCont = addressof(iter_in.container());
        mIter = iter_in.iterator();
	mMu = iter_in.angle();
	return *this;
    }

    /// Dereference Operator
    const Zone& operator*() const throw(IteratorOutOfRange)
    {
#ifndef NITERCHK
        if( mIter == mCont->end() || mIter == mCont->begin()-1) throw(IteratorOutOfRange(__LINE__, __FILE__) );
#endif
        return *mIter;
    }
    /// Indirection operator
    const Zone* operator->() const throw(IteratorOutOfRange)
    {
#ifndef NITERCHK
        if( mIter == mCont->end() || mIter == mCont->begin()-1) throw(IteratorOutOfRange(__LINE__, __FILE__) );
#endif
        return addressof(*mIter);
    }
    /// Prefix increment operator
    SweepIter& operator++() throw(IteratorOutOfRange)
    {
#ifndef NITERCHK
        if(mMu > 0 && mIter == mCont->end() ) throw(IteratorOutOfRange(__LINE__, __FILE__) );
        else if(mMu < 0 && mIter == mCont->begin()-1) throw(IteratorOutOfRange(__LINE__, __FILE__) );
#endif
        if(mMu > 0) ++mIter;
        else --mIter;
        
        return *this;
    }
    /// Postfix increment operator
    SweepIter operator++(int) throw(IteratorOutOfRange)
    {
        SweepIter tmp = *this;
        ++*this;
        return tmp;
    }

    /// Prefix increment operator
    SweepIter& operator--() throw(IteratorOutOfRange)
    {
#ifndef NITERCHK
        if(mMu > 0 && mIter == mCont->begin() ) throw(IteratorOutOfRange(__LINE__, __FILE__) );
        else if(mMu < 0 && mIter == mCont->end()-1 ) throw(IteratorOutOfRange(__LINE__, __FILE__) );
#endif
        if(mMu > 0) ++mIter;
        else --mIter;

        return *this;
    }
    /// Postfix increment operator
    SweepIter operator--(int) throw(IteratorOutOfRange)
    {
        SweepIter tmp = *this;
        --*this;
        return tmp;
    }

    /// Equality operator
    bool operator==(const SweepIter& iter_in) const
    {
        if(fabs(mMu-iter_in.angle()) < numeric_limits<double>::epsilon()
		   && mCont == iter_in.mCont && mIter == iter_in.mIter) return true;
        else return false;
    }
    /// Inequality operator
    bool operator!=(const SweepIter& iter_in) const
    {
        return !operator==(iter_in);
    }
           
protected:
    double mMu;   ///< Angle of sweep
    const vector<Zone>* mCont;   ///< Pointer to container
    vector<Zone>::const_iterator mIter;   ///< Iterator into container
};

/// @}

#endif

