// Date Last Altered: $Date: 2007-01-06 16:37:40 -0500 (Sat, 06 Jan 2007) $
// Revision Number: $Revision: 71 $
//--------------------------------------------*-C++-*------------------------------------//
/// \file: Iterator.hh
/// \author Greg Davidson
/// \date November 6, 2006
/// 
/// \brief This file provides the interface and implementation of the Iterator class
///
/// This file provides the interface and implementation of the Iterator class.
/// Iterators are wrappers for STL iterators with range checking  provided.
/// Additionally, these iterators prevent comparisons between Iterators that belong to
/// unrelated classes but share implementation details (i.e., we
/// don't want to be able to compare iterators between zone-centered fields and
/// node-centered fields).

#ifndef ITERATOR_HH
#define ITERATOR_HH

#include <iterator>
#include <boost/utility.hpp>
#include "IteratorExcept.hh"

using std::iterator_traits;
using std::iterator;
using boost::addressof;

/// \brief Provides a type-safe iterator with optional checking
///
/// Iterators are wrappers for STL iterators with range checking  provided.
/// Additionally, these iterators prevent comparisons between Iterators that belong to
/// unrelated classes but share implementation details (i.e., we
/// don't want to be able to compare iterators between zone-centered fields and
/// node-centered fields).
template<typename Cont, typename Iter = typename Cont::iterator, typename Type = Cont>
class Iterator : public iterator< typename iterator_traits<Iter>::iterator_category,
								  typename iterator_traits<Iter>::value_type,
								  typename iterator_traits<Iter>::difference_type,
								  typename iterator_traits<Iter>::pointer,
								  typename iterator_traits<Iter>::reference >
{
public:
	/// Type of unchecked-iterators we are using
	typedef Iter iterator_type;

	/// Default constructor.
	Iterator() : mContainer(), mIter() {  }
	/// Constructor taking a container and an iterator into that container
    Iterator(const Cont& cont_in, const Iter& x) throw(IteratorOutOfRange)
			: mContainer(addressof(cont_in)), mIter(x)
	{  
#ifndef NITERCHK
		if(mIter > mContainer->end() || mIter < mContainer->begin()-1)
			throw(IteratorOutOfRange(__LINE__, __FILE__));
#endif
	}

	/// Copy Constructor
	Iterator(const Iterator<Cont, Iter, Type>& iter_in)
			: mContainer(iter_in.mContainer), mIter(iter_in.mIter)
	{ }
	
	/// Constructor taking an Iterator
    //template<typename Cont_2, typename Iter_2, typename Type_2>
	template<typename Iter_2>
    Iterator(const Iterator<Cont, Iter_2, Type>& iter_in)
			: mContainer(addressof(iter_in.container())),  mIter(iter_in.iterator())
	{ }

	/// Copy Operator
	const Iterator& operator=(const Iterator<Cont, Iter, Type>& iter_in)
	{
		mContainer = iter_in.mContainer;
		mIter = iter_in.mIter;

		return *this;
	}
	
	/// Return the unchecked iterator
    Iter iterator() const { return mIter; }
	/// Return a reference to the container
    const Cont& container() const { return *mContainer; } 

	/// Dereference operator
    typename Iter::reference operator*() const throw(IteratorOutOfRange)
	{
#ifndef NITERCHK
		if(mIter == mContainer->begin()-1 || mIter == mContainer->end())
			throw(IteratorOutOfRange(__LINE__, __FILE__));
#endif
		return *mIter;
	}
	
	/// Pointer to member operator
    typename Iter::pointer operator->() const throw(IteratorOutOfRange)
	{
#ifndef NITERCHK
		if(mIter == mContainer->begin()-1 || mIter == mContainer->end())
			throw(IteratorOutOfRange(__LINE__, __FILE__));
#endif 
		return &(*mIter);
	}

	/// Bracket operator
    typename Iter::reference operator[](typename Iter::difference_type n) const throw(IteratorOutOfRange)
	{
#ifndef NITERCHK
		if(mIter+n < mContainer->begin() || mIter+n >= mContainer->end())
			throw(IteratorOutOfRange(__LINE__, __FILE__));
#endif
		return mIter[n];
	}

	/// Increment prefix operator
    Iterator<Cont, Iter, Type>& operator++() throw(IteratorOutOfRange)
	{
#ifndef NITERCHK	
		if(mIter == mContainer->end()) throw(IteratorOutOfRange(__LINE__, __FILE__));
#endif
		++mIter;
		return *this;
	}
	/// Increment postfix operator
	Iterator<Cont, Iter, Type> operator++(int) throw(IteratorOutOfRange)
	{
		Iterator<Cont, Iter, Type> tmp = *this;
		++mIter;
		return tmp;
	}
    /// Decrement prefix operator
    Iterator<Cont, Iter, Type>& operator--() throw(IteratorOutOfRange)
	{
#ifndef NITERCHK
		if(mIter == mContainer->begin()-1) throw(IteratorOutOfRange(__LINE__, __FILE__));
#endif
		--mIter;
		return *this;
	}
	/// Decrement postfix operator
    Iterator<Cont, Iter, Type> operator--(int) throw(IteratorOutOfRange)
	{
		Iterator<Cont, Iter, Type> tmp = *this;
		--mIter;
		return tmp;
	}
	/// Addition operator
    Iterator<Cont, Iter, Type> operator+(typename Iter::difference_type n) const
		throw(IteratorOutOfRange)
	{
		return Iterator<Cont, Iter, Type>(*mContainer, mIter+n);
	}
    /// Plus-equal operator
    Iterator<Cont, Iter, Type>& operator+=(typename Iter::difference_type n) const
		throw(IteratorOutOfRange)
	{
#ifndef NITERCHK
		if(mIter+n < mContainer->begin() || mIter+n > mContainer->end())
			throw(IteratorOutOfRange(__LINE__, __FILE__));
#endif
		mIter += n;
		return *this;
	}
	/// Subtraction operator
    Iterator<Cont, Iter, Type> operator-(typename Iter::difference_type n) const
		throw(IteratorOutOfRange)
	{
		return Iterator<Cont, Iter, Type>(*mContainer, mIter-n);
	}
	/// Minus-equal operator
    Iterator<Cont, Iter, Type>& operator-=(typename Iter::difference_type n) const
		throw(IteratorOutOfRange)
	{
#ifndef NITERCHK
		if(mIter-n < mContainer->begin() || mIter-n > mContainer->end())
			throw(IteratorOutOfRange(__LINE__, __FILE__));
#endif
		mIter -= n;
		return *this;
	}
	/// Difference operator
    template<typename Iter_2>
    typename Iter::difference_type operator-(const Iterator<Cont, Iter_2, Type>& iter_in) const
	{
		return mIter - iter_in.iterator();
	}

private:
    const Cont* mContainer; ///< Pointer to the container
    Iter mIter; ///< Store the container's unchecked iterator
};

// ******* Here's where we start to differ with Iterators.  We only allow comparisons between
// iterators that share the Cont, and not just the Impl.

/// Plus operator
template<typename Cont, typename Iter, typename Type>
Iterator<Cont, Iter, Type> operator+(typename Iter::difference_type n,
									 const Iterator<Cont, Iter, Type>& iter)
{
	return iter+n;
}

/// Subtraction operator
template<typename Cont, typename Iter, typename Type>
Iterator<Cont, Iter, Type> operator-(typename Iter::difference_type n,
									 const Iterator<Cont, Iter, Type>& iter)
{
	return iter-n;
}

// ******* Comparison operators ******
/// Equal comparison between two different iterators over the same container
template<typename Cont, typename Iter1, typename Iter2, typename Type>
bool operator==(const Iterator<Cont, Iter1, Type>& iter1,
				const Iterator<Cont, Iter2, Type>& iter2)
{
	if( &(iter1.container()) == &(iter2.container()) && iter1.iterator() == iter2.iterator())
		return true;
	else return false;
}

/// Not-equal comparison between two different iterators over the same container.
template<typename Cont, typename Iter1, typename Iter2, typename Type>
bool operator!=(const Iterator<Cont, Iter1, Type>& iter1,
				const Iterator<Cont, Iter2, Type>& iter2)
{
	return !(iter1==iter2);
}
/// Less-than comparison between two different iterators over the same container.
template<typename Cont, typename Iter1, typename Iter2, typename Type>
bool operator<(const Iterator<Cont, Iter1, Type>& iter1,
			   const Iterator<Cont, Iter2, Type>& iter2)
{
	if( iter1.iterator() < iter2.iterator())
		return true;
	else return false;
}
/// Greater-than comparison between two different iterators over the same container.
template<typename Cont, typename Iter1, typename Iter2, typename Type>
bool operator>(const Iterator<Cont, Iter1, Type>& iter1,
			   const Iterator<Cont, Iter2, Type>& iter2)
{
	if( iter1.iterator() > iter2.iterator())
		return true;
	else return false;
}
/// Less-than or equal Comparison between two different iterators over the same container.
template<typename Cont, typename Iter1, typename Iter2, typename Type>
bool operator<=(const Iterator<Cont, Iter1, Type>& iter1,
				const Iterator<Cont, Iter2, Type>& iter2)
{
	if( iter1.iterator() <= iter2.iterator())
		return true;
	else return false;
}
/// Greater-than or equal Comparison between two different iterators over the same container.
template<typename Cont, typename Iter1, typename Iter2, typename Type>
bool operator>=(const Iterator<Cont, Iter1, Type>& iter1, const Iterator<Cont, Iter2, Type>& iter2)
{
	if( iter1.iterator() >= iter2.iterator())
		return true;
	else return false;
}


#endif
