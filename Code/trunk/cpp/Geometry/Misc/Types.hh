// Date Last Altered: $Date: 2008-08-04 22:55:41 -0600 (Mon, 04 Aug 2008) $
// Revision Number: $Revision: 344 $
//--------------------------------------------*-C++-*------------------------------------//
/*! \file Types.hh
 *  \author Greg Davidson
 *	 \date August 15, 2007
 *
 *  \brief Provides aliases for the basic primitive numerical types.
 *
 *  This file provides type definitions for the basic primitive numerical types.  The advantage
 *  of providing these type definitions is that they may be altered on
 *  different platforms to provide identical code behavior, regardless of the machine. */

#ifndef TYPES_HH
#define TYPES_HH

// ****** Signed Integral Types ******
typedef char Byte;   ///< One byte signed integral type.
typedef short Int2;  ///< Two byte signed integral type.
typedef int Int4;    ///< Four byte signed integral type.
// Don't use long long unless really needed.  This type is not portable.
//typedef long long Int8;            

// ****** Unsigned Integral Types ******
typedef unsigned char UnsignedByte;     ///< One byte unsigned integral type
typedef unsigned short UnsignedInt2;    ///< Two byte unsigned integral type
typedef unsigned int UnsignedInt4;      ///< Four byte unsigned integral type 
// Don't use unsigned long long unless really needed.  This type is not portable.
//typedef unsigned long long UnsignedInt8;   

// ****** Floating-point types ******
typedef float Real4;          ///< Four byte floating type
typedef double Real8;         ///< Eight byte floating type
typedef long double Real12;   ///< Twelve byte floating type

#endif

