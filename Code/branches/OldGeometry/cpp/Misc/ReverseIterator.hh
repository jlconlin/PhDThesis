// Date Last Altered: $Date: 2006-11-15 20:51:22 -0500 (Wed, 15 Nov 2006) $
// Revision Number: $Revision: 39 $
//--------------------------------------------*-C++-*------------------------------------//
/// \file: ReverseIterator.hh
/// \author Greg Davidson
/// \date February 6, 2006
/// 
/// \brief This file provides a general ReverseIterator.
///
/// This file provides the ReverseIterator class.  The ReverseIterator class provides a reverse
/// iterator over a general container.  This class is an iterator adapter, so if it is given
/// a checked iterator, it too will be checked.  Note the the STL also offers a reverse_iterator
/// iterator adapter.  However, I believe it is flawed, in that its comparison operators
/// are not templated such that reverse_iterators and const_reverse_iterators can not be
/// compared.
#ifndef REVERSEITERATOR_HH
#define REVERSEITERATOR_HH

#include <iterator>
#include "IteratorExcept.hh"

using std::iterator_traits;
using std::iterator;

/// \brief This class provides a general ReverseIterator.iterator adapter.
///
/// The ReverseIterator class provides a reverse
/// iterator over a general container.  This class is an iterator adapter, so if it is given
/// a checked iterator, it too will be checked.  Note the the STL also offers a reverse_iterator
/// iterator adapter.  However, I believe it is flawed, in that its comparison operators
/// are not templated such that reverse_iterators and const_reverse_iterators can not be
/// compared.
template<typename Iter>
class ReverseIterator : public iterator< typename iterator_traits<Iter>::iterator_category,
										 typename iterator_traits<Iter>::value_type,
										 typename iterator_traits<Iter>::difference_type,
										 typename iterator_traits<Iter>::pointer,
										 typename iterator_traits<Iter>::reference >
{
public:
	/// Type of iterator we are reversing over
    typedef Iter iterator_type;

	/// Default constructor
    ReverseIterator() : mIter() {  }
    /// Constructor taking a forward iterator
    explicit ReverseIterator(const Iter& x) throw(IteratorOutOfRange)
			: mIter(x)
	{ }
    /// Constructor taking a reverse iterator
    template<typename Iter_2>
    ReverseIterator(const ReverseIterator<Iter_2>& iter_in)
			: mIter(iter_in.base())
	{ }
	/// Return the forward iterator
    Iter base() const { return mIter; }
    /// Dereference operator
    typename Iter::reference operator*() const throw(IteratorOutOfRange)
	{
		Iter tmp = mIter;
		return *(--tmp);
	}
	/// Pointer operator
    typename Iter::pointer operator->() const throw(IteratorOutOfRange)
	{
		Iter tmp = mIter;
		return &(*--tmp);
	}
	/// Bracket operator
    typename Iter::reference operator[](typename Iter::difference_type n) const throw(IteratorOutOfRange)
	{
		return mIter[-n-1];
	}
	/// Increment prefix operator
    ReverseIterator<Iter>& operator++() throw(IteratorOutOfRange)
	{
		--mIter;
		return *this;
	}
	/// Increment postfix operator
    ReverseIterator<Iter> operator++(int) throw(IteratorOutOfRange)
	{
		ReverseIterator<Iter> tmp = *this;
		--mIter;
		return tmp;
	}
    /// Decrement prefix operator
    ReverseIterator<Iter>& operator--() throw(IteratorOutOfRange)
	{
		++mIter;
		return *this;
	}
	/// Increment postfix
    ReverseIterator<Iter> operator--(int) throw(IteratorOutOfRange)
	{
		ReverseIterator<Iter> tmp = *this;
		++mIter;
		return tmp;
	}
	/// Addition operator
    ReverseIterator<Iter> operator+(typename Iter::difference_type n) const
		throw(IteratorOutOfRange)
	{
		return ReverseIterator<Iter>(mIter-n);
	}
    /// Plus-equal operator
    ReverseIterator<Iter>& operator+=(typename Iter::difference_type n) const
		throw(IteratorOutOfRange)
	{
		mIter -= n;
		return *this;
	}
	/// Minus operator
    ReverseIterator<Iter> operator-(typename Iter::difference_type n) const
		throw(IteratorOutOfRange)
	{
		return ReverseIterator<Iter>(mIter+n);
	}
	/// Minus-equal operator
    ReverseIterator<Iter>& operator-=(typename Iter::difference_type n) const
		throw(IteratorOutOfRange)
	{
		mIter += n;
		return *this;
	}
	/// Difference operator
	template< typename Iter_2 >
    typename Iter::difference_type operator-(const ReverseIterator<Iter_2>& iter_in) const
	{
		return iter_in.base() - mIter;
	}
	
private:
    Iter mIter;  ///< Stores the forward iterator
};


/// Addition operator
template<typename Iter>
ReverseIterator<Iter> operator+(typename Iter::difference_type n, const ReverseIterator<Iter>& iter)
{
    return iter+n;
}
/// Subtraction operator
template<typename Iter>
ReverseIterator<Iter> operator-(typename Iter::difference_type n, const ReverseIterator<Iter>& iter)
{
    return iter-n;
}

// ****** Reverse Iterator Comparisons ****** //
/// Equal comparison between two different reverse iterators over the same container.
template<typename Iter1, typename Iter2>
bool operator==( const ReverseIterator<Iter1>& iter1,
				 const ReverseIterator<Iter2>& iter2 )
{
	if(iter1.base() == iter2.base()) return true;
	else return false;
}
/// Not-equal comparison between two different reverse iterators over the same container
template<typename Iter1, typename Iter2>
bool operator!=( const ReverseIterator<Iter1>& iter1,
				 const ReverseIterator<Iter2>& iter2 )
{
	return !(iter1 == iter2);
}
/// Less-than comparison between two different reverse iterators over the same container
template<typename Iter1, typename Iter2>
bool operator<( const ReverseIterator<Iter1>& iter1,
				const ReverseIterator<Iter2>& iter2 )
{
	if(iter1.base() > iter2.base()) return true;
	else return false;
}
/// Greater-than comparison between two different reverse iterators over the same container
template<typename Iter1, typename Iter2>
bool operator>( const ReverseIterator<Iter1>& iter1,
				const ReverseIterator<Iter2>& iter2 )
{
	if(iter1.base() < iter2.base()) return true;
	else return false;
}
/// Less-than or equal comparison between two different reverse iterators over the same container
template<typename Iter1, typename Iter2>
bool operator<=( const ReverseIterator<Iter1>& iter1,
				 const ReverseIterator<Iter2>& iter2 )
{
	if(iter1.base() >= iter2.base()) return true;
	else return false;
}
/// Greater-than or equal comparison between two different reverse iterators over the same container
template<typename Iter1, typename Iter2>
bool operator>=( const ReverseIterator<Iter1>& iter1,
				 const ReverseIterator<Iter2>& iter2 )
{
	if(iter1.base() <= iter2.base()) return true;
	else return false;
}


#endif

