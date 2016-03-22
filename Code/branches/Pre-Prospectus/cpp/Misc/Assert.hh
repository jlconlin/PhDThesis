// Date Last Altered: $Date: 2006-11-15 20:51:22 -0500 (Wed, 15 Nov 2006) $
// Revision Number: $Revision: 39 $
//----------------------------------*-C++-*----------------------------------//
/// \file Assert.hh
/// \author Geoffrey Furnish
/// \date December 6, 1993
/// \brief This file provides an Assert, Insist, and DIE macros.
///
/// The purpose of this file is to provide Assert_link , Insist, and DIE macros.
/// The Assert macro provides assertions for sanity tests that may be compiled
/// out.  The Insist macros provides similar functionality to the assert macro
/// but should never be compiled out.  Die is a placeholder for functions that
/// have not yet been implemented.
//---------------------------------------------------------------------------//
// Header file for Assert macro, and friends.

#ifndef __ds_Assert_hh__
#define __ds_Assert_hh_
#include <iostream>
#include <cstdlib>

//---------------------------------------------------------------------------//
/// The Assert macro causes the code to abort if condition \a a is false.
//
/// The Assert macro is intended to be used for validating preconditions which
/// must be true in order for following code to be correct, etc. For example,
/// \code Assert( x > 0 ); y = sqrt(x); \endcode If the Assertion fails, the code should
/// just bomb. Philosophically, it should be used to ferret out bugs in
/// preceding code, making sure that prior results are within reasonable
/// bounds before proceeding to use those results in further computation, etc.
//---------------------------------------------------------------------------//
#ifdef NOASSERT
#define Assert(a)
#else
#define Assert(a) if (!(a)) \
{ \
	std::cerr << std::endl << __FILE__ << ':' << __LINE__ << " Assertion " << #a << " failed in " << __FILE__ \
    << " at line " << __LINE__ << std::endl << std::flush; exit(1); \
}
#endif


//---------------------------------------------------------------------------//
/// The Insist macro echoes message \a b if condition \a a is false.
//
/// The Insist macro is akin to the Assert macro, but it provides the
/// opportunity to specify an instructive message. The idea here is that you 
/// should use Insist for checking things which are more or less under user 
/// control. If the user makes a poor choice, we "insist" that it be 
/// corrected, providing a corrective hint. 
/// \note 
/// We provide a way to eliminate Assertions, but not insistings. The 
/// idea is that Assert is used to perform sanity checks during program 
/// development, which you might want to eliminate during production runs for 
/// performance sake. Insist is used for things which really really must be 
/// true, such as "the file must've been opened", etc. So, use Assert for 
/// things which you want taken out of production codes (like, the check might 
/// inhibit inlining or something like that), but use Insist for those things 
/// you want checked even in a production code. 
//---------------------------------------------------------------------------//
#define Insist(a,b) if (!(a)) \
{ \
	std::cerr << std::endl << __FILE__ << ':' << __LINE__ << " Insisting that " << #a << " , in " << __FILE__; \
    std::cerr << " at line " << __LINE__ << std::endl << std::flush; \
    throw(b); \
}

#define InsistNoThrow(a) if (!(a)) \
{ \
	std::cerr << std::endl << __FILE__ << ':' << __LINE__ << " Insisting that " << #a << " , in " << __FILE__; \
    std::cerr << " at line " << __LINE__ << std::endl << std::flush; \
}


//---------------------------------------------------------------------------// 
/// The DIE macro is a temporary place holder.
//
/// The DIE macro is for putting into function bodies which are only place 
/// holders until they can be actually implemented. This will encourage that 
/// process. 
//---------------------------------------------------------------------------//
#define DIE \
{ \
	std::cerr << std::endl << __FILE__; \
    std::cerr << ':' << __LINE__ << " Function containing line " << __LINE__ << " in file " \
			  << __FILE__ << " not implemented yet.\n"; \
	std::abort(); \
}

#endif
//---------------------------------------------------------------------------// 
// end of ds++/Assert.hh 
//---------------------------------------------------------------------------//
