// Date Last Altered: $Date: 2008-08-04 22:55:41 -0600 (Mon, 04 Aug 2008) $
// Revision Number: $Revision: 344 $
//----------------------------------*-C++-*----------------------------------//
/*! \file Assert.hh
 *  \author Geoffrey Furnish
 *  \date December 6, 1993
 *
 *  \brief Provides the \c Assert, \c Insist, and \c DIE macros.
 *
 *  The purpose of this file is to provide the \c Assert, \c Insist, and \c DIE 
 *  macros.  The \c Assert macro provides assertions for sanity tests that may 
 *  be compiled out.  The \c Insist macros provides similar functionality to the 
 *  \c Assert macro but should never be compiled out.  \c DIE is a placeholder 
 *  for functions that have not yet been implemented. */

#ifndef ASSERT_HH
#define ASSERT_HH

#include <iostream>
#include <cstdlib>

/*! \brief Causes the code to abort if condition \a a is false.
 *
 *  The \c Assert macro is intended to be used for validating preconditions which
 *  must be true in order for the following code to be correct, etc. For example,
 *  \code 
 *      Assert( x > 0 ); 
 *      y = sqrt(x); 
 *  \endcode 
 *  If the assertion fails, the code should abort with a message stating that
 *  the assertion failed and giving the file and line of the assertion.
 *  Philosophically, it should be used to ferret out bugs in
 *  preceding code, making sure that prior results are within reasonable
 *  bounds before proceeding to use those results in further computation, etc. */
#ifdef NOASSERT
#define Assert(a)
#else
#define Assert(a) \
if (!(a)) \
{ \
   std::cerr << std::endl << __FILE__ << ':' << __LINE__ \
             << " Assertion " << #a << " failed in " << __FILE__ \
             << " at line " << __LINE__ << std::endl << std::flush; \
   exit(1); \
}
#endif


/*! \brief Echoes message \a b if condition \a a is false and 
 *         throws \a b.
 *
 *  The \c Insist macro is akin to the \c Assert macro, but it provides the
 *  opportunity to specify an instructive message. The idea here is that you 
 *  should use \c Insist for checking things which are more or less under user 
 *  control. If the user makes a poor choice, we "insist" that it be 
 *  corrected, providing a corrective hint. 
 *  \remarks  We provide a way to eliminate assertions, but not insistings. The 
 *            idea is that \c Assert is used to perform sanity checks during program 
 *            development, which you might want to eliminate during production runs for 
 *            performance sake. \c Insist is used for things which really really must be 
 *            true, such as "the file must've been opened", etc. So, use \c Assert for 
 *            things which you want taken out of production codes (like, the check might 
 *            inhibit inlining or something like that), but use \c Insist for those things 
 *            you want checked even in a production code. */
#define Insist(a,b) \
if (!(a)) \
{ \
   std::cerr << std::endl << __FILE__ << ':' << __LINE__ \
             << " Insisting that " << #a << " , in " << __FILE__ \
             << " at line " << __LINE__ << std::endl << std::flush; \
   throw(b); \
}

/*! \brief Echoes message \a b if condition \a a is false.
 *
 *  The \c InsistNoThrow macro is just like the \c Insist macro, with the exception 
 *  that it does not throw \a b after printing the error message.
 *  \remarks  We provide a way to eliminate assertions, but not insistings. The 
 *            idea is that \c Assert is used to perform sanity checks during program 
 *            development, which you might want to eliminate during production runs for 
 *            performance sake. \c InsistNoThrow is used for things which really really 
 *            must be true, such as "the file must've been opened", etc. So, use \c Assert for 
 *            things which you want taken out of production codes (like, the check might 
 *            inhibit inlining or something like that), but use \c InsistNoThrow for those things 
 *            you want checked even in a production code. */
#define InsistNoThrow(a) \
if (!(a)) \
{ \
   std::cerr << std::endl << __FILE__ << ':' << __LINE__ \
             << " Insisting that " << #a << " , in " << __FILE__; \
             << " at line " << __LINE__ << std::endl << std::flush; \
}


/*! \brief Temporary place holder.
 *
 *  The \c DIE macro is for putting into function bodies as a place holder until
 *  that function can actually be implemented. */
#define DIE \
{ \
	std::cerr << std::endl << __FILE__ << ':' << __LINE__ \
	          << " Function containing line " << __LINE__ << " in file " \
			    << __FILE__ << " not implemented yet." << std::endl; \
	std::abort(); \
}

#endif // End ASSERT_HH

