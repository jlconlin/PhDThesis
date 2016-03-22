//======================================================================
//
// C version of MCNP5 routines for random number generation
//
//   - These routine require the use of 'unsigned long long' integers,
//     as specified in the ANSI-C99 standard - 64-bit integers.
//
//   - These routines were developed & tested with the gcc and 
//     g++ compiler. No special options are needed to compile & test.
//
//   - For other C/C++ compilers, some tweaking may be needed. 
//     Be sure to run & examine the tests.
//
//   - NOTE: These routines are not thread-safe. 
//           (OpenMP threading will be coming soon.)
//
//   - To mix these C routines with Fortran-90 compiled
//     with the Absoft compiler, use these options when
//     compiling the fortran:
//        f90 -YEXT_NAMES=LCS -YEXT_SFX=_ -YCFRL=1  ... 
//
//   - To mix these C routines with Fortran-90 compiled
//     with the g95 compiler, use these options when
//     compiling the fortran:
//        g95 -funderscoring -fno-second-underscore ... 
//
//   - To mix these C routines with Fortran-90 compiled
//     with the IBM xlf compiler, use these options when
//     compiling the fortran:
//        xlf -qextname ...
//
//
// Author:  FB Brown, 2005-12-02
//======================================================================

//=======================================================================
// Description:
//  mcnp_random.F90 -- random number generation routines
//=======================================================================
//  This module contains:
//
//   * Constants for the RN generator, including initial RN seed for the
//     problem & the current RN seed
//
//   * MCNP interface routines:
//     - random number function:           rang()
//     - RN initialization for problem:    RN_init_problem
//     - RN initialization for particle:   RN_init_particle
//     - get info on RN parameters:        RN_query
//     - get RN seed for n-th history:     RN_query_first
//     - set new RN parameters:            RN_set
//     - skip-ahead in the RN sequence:    RN_skip_ahead
//     - Unit tests:        RN_test_basic, RN_test_skip, RN_test_mixed
//
//   * For interfacing with the rest of MCNP, arguments to/from these
//     routines will have types of I8 or I4.
//     Any args which are to hold random seeds, multipliers,
//     skip-distance will be type I8, so that 63 bits can be held without
//     truncation.
//
// Revisions:
// * 10-04-2001 - F Brown, initial mcnp version
// * 06-06-2002 - F Brown, mods for extended generators
// * 12-21-2004 - F Brown, added 3 of LeCuyer's 63-bit mult. RNGs
// * 01-29-2005 - J Sweezy, Modify to use mcnp modules prior to automatic
//                io unit numbers.
// * 12-02-2005 - F Brown, mods for consistency with C version
//=======================================================================

// #ifdef __cplusplus
// extern "C" {
// #endif

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include "mcnp_random.h"


// function names to match what g95 expects
// #define  rang               rang_
// #define  RN_init_problem    rn_init_problem_
// #define  RN_init_particle   rn_init_particle_
// #define  RN_set             rn_set_
// #define  RN_query           rn_query_
// #define  RN_query_first     rn_query_first_
// #define  RN_update_stats    rn_update_stats_
// #define  RN_test_basic      rn_test_basic_
// #define  RN_test_skip       rn_test_skip_
// #define  RN_test_mixed      rn_test_mixed_

  //---------------------------------------------------
  // Kinds for LONG INTEGERS (64-bit) & REAL*8 (64-bit)
  //---------------------------------------------------
#define	 LONG  long long
#define	ULONG  unsigned long long
#define	 REAL  double

  //-------------------------------------
  // Public interface for functions
  //-------------------------------------
//REAL   rang(void);
//ULONG  RN_skip_ahead( ULONG* seed, LONG* nskip );
//void   RN_init_problem( int*   new_standard_gen,
//                        ULONG* new_seed,
//                        ULONG* new_stride,
//                        ULONG* new_part1,
//                        int*   print_info );
//void   RN_init_particle( ULONG* nps );
//void   RN_set( char* key, ULONG* value );
//ULONG  RN_query( char* key );
//ULONG  RN_query_first( ULONG* nps );
//void   RN_update_stats( void );
//void   RN_test_basic( int* new_gen );
//void   RN_test_skip(  int* new_gen );
//void   RN_test_mixed( int* new_gen );

  //-------------------------------------
  // Constants for standard RN generators
  //-------------------------------------
  typedef struct {
    int          index;
    ULONG        mult;       // generator (multiplier)
    ULONG        add;        // additive constant
    int          log2mod;    // log2 of modulus, must be <64
    ULONG        stride;     // stride for particle skip-ahead
    ULONG        initseed;   // default seed for problem
    char         name[9];
  } RN_GEN;

  //  parameters for standard generators
  static const int      n_RN_GEN = 7;
  static const RN_GEN   standard_generator[7] = {
    1,      19073486328125ULL, 0ULL, 48, 152917ULL, 19073486328125ULL, "mcnp std",
    2, 9219741426499971445ULL, 1ULL, 63, 152917ULL,              1ULL, "LEcuyer1",
    3, 2806196910506780709ULL, 1ULL, 63, 152917ULL,              1ULL, "LEcuyer2",
    4, 3249286849523012805ULL, 1ULL, 63, 152917ULL,              1ULL, "LEcuyer3",
    5, 3512401965023503517ULL, 0ULL, 63, 152917ULL,              1ULL, "LEcuyer4",
    6, 2444805353187672469ULL, 0ULL, 63, 152917ULL,              1ULL, "LEcuyer5",
    7, 1987591058829310733ULL, 0ULL, 63, 152917ULL,              1ULL, "LEcuyer6" 
  };

  //-----------------------------------------------------------------
  //   * Linear multiplicative congruential RN algorithm:
  //
  //            RN_SEED = RN_SEED*RN_MULT + RN_ADD  mod RN_MOD
  //
  //   * Default values listed below will be used, unless overridden
  //-----------------------------------------------------------------
  static int    RN_INDEX   = 1;
  static ULONG  RN_MULT    =  19073486328125ULL;
  static ULONG  RN_ADD     =               0ULL;
  static int    RN_BITS    = 48;
  static ULONG  RN_STRIDE  =          152917ULL;
  static ULONG  RN_SEED0   =  19073486328125ULL;
  static ULONG  RN_MOD     = 281474976710656ULL;
  static ULONG  RN_MASK    = 281474976710655ULL;
  static ULONG  RN_PERIOD  =  70368744177664ULL;
  static REAL   RN_NORM    = 1.0/281474976710656.;

  //------------------------------------
  // Private data for a single particle
  //------------------------------------
  static ULONG  RN_SEED    = 19073486328125ULL; // current seed
  static ULONG  RN_COUNT   = 0;	                // current counter
  static ULONG  RN_NPS     = 0;		        // current particle number

  //------------------------------------------
  // Shared data, to collect info on RN usage
  //------------------------------------------
  static ULONG  RN_COUNT_TOTAL   = 0;     // total RN count all particles
  static ULONG  RN_COUNT_STRIDE  = 0;     // count for stride exceeded
  static ULONG  RN_COUNT_MAX     = 0;     // max RN count all particles
  static ULONG  RN_COUNT_MAX_NPS = 0;     // part index for max count

  //----------------------------------------------------------------------
  // reference data:  seeds for case of init.seed = 1,
  //                  seed numbers for index 1-5, 123456-123460
  //----------------------------------------------------------------------
  static const ULONG  RN_CHECK[7][10] = { 
    // ***** 1 ***** mcnp standard gen *****
         19073486328125ULL,      29763723208841ULL,     187205367447973ULL, 
        131230026111313ULL,     264374031214925ULL,     260251000190209ULL,
        106001385730621ULL,     232883458246025ULL,      97934850615973ULL,
        163056893025873ULL,
    // ***** 2 *****
    9219741426499971446ULL,  666764808255707375ULL, 4935109208453540924ULL,
    7076815037777023853ULL, 5594070487082964434ULL, 7069484152921594561ULL,
    8424485724631982902ULL,   19322398608391599ULL, 8639759691969673212ULL,
    8181315819375227437ULL,
    // ***** 3 *****
    2806196910506780710ULL, 6924308458965941631ULL, 7093833571386932060ULL,
    4133560638274335821ULL,  678653069250352930ULL, 6431942287813238977ULL,
    4489310252323546086ULL, 2001863356968247359ULL,  966581798125502748ULL,
    1984113134431471885ULL,
    // ***** 4 *****
    3249286849523012806ULL, 4366192626284999775ULL, 4334967208229239068ULL,
    6386614828577350285ULL, 6651454004113087106ULL, 2732760390316414145ULL,
    2067727651689204870ULL, 2707840203503213343ULL, 6009142246302485212ULL,
    6678916955629521741ULL,
    // ***** 5 *****
    3512401965023503517ULL, 5461769869401032777ULL, 1468184805722937541ULL,
    5160872062372652241ULL, 6637647758174943277ULL,  794206257475890433ULL,
    4662153896835267997ULL, 6075201270501039433ULL,  889694366662031813ULL,
    7299299962545529297ULL,
    // ***** 6 *****
    2444805353187672469ULL,  316616515307798713ULL, 4805819485453690029ULL,
    7073529708596135345ULL, 3727902566206144773ULL, 1142015043749161729ULL,
    8632479219692570773ULL, 2795453530630165433ULL, 5678973088636679085ULL,
    3491041423396061361ULL,
    // ***** 7 *****
    1987591058829310733ULL, 5032889449041854121ULL, 4423612208294109589ULL,
    3020985922691845009ULL, 5159892747138367837ULL, 8387642107983542529ULL,
    8488178996095934477ULL,  708540881389133737ULL, 3643160883363532437ULL,
    4752976516470772881ULL
  };

//----------------------------------------------------------------------
//
  REAL	RNG::rang( void ) {
    // MCNP random number generator
    //
    // ***************************************
    // ***** modifies RN_SEED & RN_COUNT *****
    // ***************************************
	RN_SEED   = (RN_MULT*RN_SEED + RN_ADD) & RN_MASK;
        RN_COUNT += 1;

        return  (REAL) (RN_SEED*RN_NORM);
  }

//----------------------------------------------------------------------
//
  ULONG   RNG::RN_skip_ahead( ULONG* s, LONG* n ) {
    //  skip ahead n RNs:   RN_SEED*RN_MULT^n mod RN_MOD
    ULONG seed  = *s;
    LONG  nskip = *n;
    while( nskip<0 )  nskip += RN_PERIOD;      // add period till >0
    nskip = nskip & RN_MASK;                   // mod RN_MOD
    ULONG  gen=1,  g=RN_MULT, gp, inc=0, c=RN_ADD, rn;
    // get gen=RN_MULT^n,  in log2(n) ops, not n ops !
    for( ; nskip; nskip>>=1 ) {  
      if( nskip&1 ) {
        gen =  gen*g      & RN_MASK;
        inc = (inc*g + c) & RN_MASK;
      }
      c  = g*c+c & RN_MASK;
      g  = g*g   & RN_MASK;
    }
    rn = (gen*seed + inc) & RN_MASK;
  
    return (ULONG) rn;
  }
//----------------------------------------------------------------------
//
  void RNG::RN_init_problem( int*   new_standard_gen,
                        ULONG* new_seed,
                        ULONG* new_stride,
                        ULONG* new_part1,
                        int*   print_info ) {
    // * initialize MCNP random number parameters for problem,
    //   based on user input.  This routine should be called
    //   only from the main thread, if OMP threading is being used.
    //
    // * for initial & continue runs, these args should be set:
    //     new_standard_gen - index of built-in standard RN generator,
    //                        from RAND gen=   (or dbcn(14)
    //     new_seed   - from RAND seed=        (or dbcn(1))
    //     output     - logical, print RN seed & mult if true
    //
    //     new_stride - from RAND stride=      (or dbcn(13))
    //     new_part1  - from RAND hist=        (or dbcn(8))
    //
    // * for continue runs only, these should also be set:
    //     new_count_total   - from "rnr"   at end of previous run
    //     new_count_stride  - from nrnh(1) at end of previous run
    //     new_count_max     - from nrnh(2) at end of previous run
    //     new_count_max_nps - from nrnh(3) at end of previous run
    //
    // * check on size of long-ints & long-int arithmetic
    // * check the multiplier
    // * advance the base seed for the problem
    // * set the initial particle seed
    // * initialize the counters for RN stats

    if( *new_standard_gen<1 || *new_standard_gen>n_RN_GEN ) {
      printf("***** error: bad new_standard_gen\n");
      exit(1);
    }
    // set defaults, override if input supplied: seed, mult, stride
    RN_INDEX   = *new_standard_gen;
    RN_MULT    = standard_generator[RN_INDEX-1].mult;
    RN_ADD     = standard_generator[RN_INDEX-1].add;
    RN_STRIDE  = standard_generator[RN_INDEX-1].stride;
    RN_SEED0   = standard_generator[RN_INDEX-1].initseed;
    RN_BITS    = standard_generator[RN_INDEX-1].log2mod;
    RN_MOD     = 1ULL<<RN_BITS;
    RN_MASK    = (~0ULL) >> (64-RN_BITS);
    RN_NORM    = 1./(REAL)RN_MOD;
    RN_PERIOD  = (RN_ADD==0ULL)?  1ULL<<(RN_BITS-2) : 1ULL<<RN_BITS;
    if( *new_seed>0 ) {
      RN_SEED0 = *new_seed;
    }
    if( *new_stride>0 ) {
      RN_STRIDE = *new_stride;
    }
    RN_COUNT_TOTAL   = 0;
    RN_COUNT_STRIDE  = 0;
    RN_COUNT_MAX     = 0;
    RN_COUNT_MAX_NPS = 0;
    if( *print_info ) {
      printf( "\n%s\n%s%20d%s\n%s%20llu%s\n%s%20llu%s\n"
              "%s%20llu%s\n%s%20d%s\n%s%20llu%s\n%s\n\n",
          " ***************************************************",
          " * Random Number Generator  = ", RN_INDEX,       " *",
          " * Random Number Seed       = ", RN_SEED0,       " *",
          " * Random Number Multiplier = ", RN_MULT,        " *",
          " * Random Number Adder      = ", RN_ADD,         " *",
          " * Random Number Bits Used  = ", RN_BITS,        " *",
          " * Random Number Stride     = ", RN_STRIDE,      " *",
          " ***************************************************");
      printf(" comment. using random number generator %d,"
             " initial seed = %llu\n", RN_INDEX, RN_SEED0 );
    }
    // double-check on number of bits in a long long unsigned int
    if( sizeof(RN_SEED)<8 ) {
      printf("***** RN_init_problem ERROR:"
             " <64 bits in long-int, can-t generate RN-s\n");
      exit(1);
    }
    ULONG itemp1, itemp2, itemp3, itemp4;
    itemp1 =  (ULONG) pow(5.e0,25);
    itemp2 =  (ULONG) pow(5.e0,19);
    itemp3 = ((ULONG) pow(2.e0,62-1))<<1 + 1;
    itemp4 = itemp1*itemp2;
    if( itemp4&itemp3 != 8443747864978395601ULL ) {
      printf("***** RN_init_problem ERROR:"
             " can-t do 64-bit integer ops for RN-s\n");
      exit(1);
    }
    if( *new_part1>1 ) {
      LONG n = (*new_part1-1)*RN_STRIDE;
      LONG k =                RN_STRIDE;
      RN_SEED0     = RN_skip_ahead( &RN_SEED0, &n );
      ULONG itemp1 = RN_skip_ahead( &RN_SEED0, &k );
      if( *print_info ) {
        printf("\n%s\n%s\n%s%20llu%s\n%s%20llu%s\n%s%20llu%s\n%s\n\n",
          " ***************************************************",
          " * Random Number Seed will be advanced to that for *",
          " * previous particle number = ", *new_part1,     " *",
          " * New RN Seed for problem  = ", RN_SEED0,       " *",
          " * Next Random Number Seed  = ", itemp1,         " *",
          " ***************************************************");
        printf(" comment. advancing random number to particle %20llu,"
               " initial seed = %20llu\n", *new_part1, RN_SEED0 );
      }
    }
    // set the initial particle seed
    RN_SEED    = RN_SEED0;
    RN_COUNT   = 0;
    RN_NPS     = 0;
    return;
  }
//----------------------------------------------------------------------
//
  void	RNG::RN_init_particle( ULONG* nps ) {
    // initialize MCNP random number parameters for particle "nps"
    //
    //     * generate a new particle seed from the base seed
    //       & particle index
    //     * set the RN count to zero
        LONG  nskp = *nps * RN_STRIDE;
	RN_SEED  = RN_skip_ahead( &RN_SEED0, &nskp );
        RN_COUNT = 0;
        RN_NPS   = *nps;
  }
//----------------------------------------------------------------------
//
  void   RNG::RN_set( char* key, ULONG* value ) {
    if( !strcmp(key,"count_total")   ) { RN_COUNT_TOTAL   = *value; return; } 
    if( !strcmp(key,"count_stride")  ) { RN_COUNT_STRIDE  = *value; return; } 
    if( !strcmp(key,"count_max")     ) { RN_COUNT_MAX     = *value; return; } 
    if( !strcmp(key,"count_max_nps") ) { RN_COUNT_MAX_NPS = *value; return; } 
    if( !strcmp(key,"seed")          ) {
      RN_SEED0 = *value;
      RN_SEED  = RN_SEED0;
      RN_COUNT = 0;
      RN_NPS   = 0;
    } 
    return;
  }
//----------------------------------------------------------------------
//
  ULONG   RNG::RN_query( char* key ) {
    if( !strcmp(key,"seed")          ) return  RN_SEED; 
    if( !strcmp(key,"stride")        ) return  RN_STRIDE; 
    if( !strcmp(key,"mult")          ) return  RN_MULT; 
    if( !strcmp(key,"add")           ) return  RN_ADD; 
    if( !strcmp(key,"count")         ) return  RN_COUNT; 
    if( !strcmp(key,"period")        ) return  RN_PERIOD;
    if( !strcmp(key,"count_total")   ) return  RN_COUNT_TOTAL; 
    if( !strcmp(key,"count_stride")  ) return  RN_COUNT_STRIDE; 
    if( !strcmp(key,"count_max")     ) return  RN_COUNT_MAX; 
    if( !strcmp(key,"count_max_nps") ) return  RN_COUNT_MAX_NPS; 
    if( !strcmp(key,"first")         ) return  RN_SEED0;
    return 0ULL;
  }
//----------------------------------------------------------------------
//
  ULONG   RNG::RN_query_first( ULONG* nps ) {
    LONG n = *nps * RN_STRIDE;
    return  RN_skip_ahead( &RN_SEED0, &n); 
  }
//----------------------------------------------------------------------
//
  void  RNG::RN_update_stats( void ) {
    // update overall RN count info

    RN_COUNT_TOTAL += RN_COUNT;

    if( RN_COUNT>RN_COUNT_MAX ) {
      RN_COUNT_MAX     = RN_COUNT;
      RN_COUNT_MAX_NPS = RN_NPS;
    }

    if( RN_COUNT>RN_STRIDE ) {
      RN_COUNT_STRIDE += 1;
    }

    RN_COUNT = 0;
    RN_NPS   = 0;

    return;
  }
//----------------------------------------------------------------------
//
  void	RNG::RN_test_basic( int* new_gen ) {
    // test routine for basic random number generator
    //
    ULONG  seeds[10],  one=1ULL, z=0ULL;  
    int i,j, k=1;
    double s = 0.0;
  
    printf("\n ***** random number - basic test *****\n");
  
    // set seed 
    RN_init_problem( new_gen, &one, &z, &z, &k );
  
    // get the    5 seeds, then skip a few, get 5 more - directly
    for( i=0; i<5;      i++ ) { s += rang(); seeds[i] = RN_query("seed"); }
    for( i=5; i<123455; i++ ) { s += rang(); }
    for( i=5; i<10;     i++ ) { s += rang(); seeds[i] = RN_query("seed"); }
  
    // compare
    for( i=0; i<10; i++ ) {
      j = (i<5)? i+1 : i+123451;
      printf(" %6d  reference: %20llu  computed: %20llu\n",
               j, RN_CHECK[*new_gen-1][i], seeds[i] );
      if( seeds[i] != RN_CHECK[*new_gen-1][i] ) {
        printf(" ***** basic_test of RN generator failed\n");
      }
    }
  }
//----------------------------------------------------------------------
//
  void	RNG::RN_test_skip( int* new_gen ) {
    // test routine for basic random number generation & skip-ahead
    //
    ULONG  seeds[10],  n=1ULL, z=0ULL;  
    LONG j;
    int i, k=0;
  
    printf("\n ***** random number - skip test *****\n");

    // set the seed
    RN_init_problem( new_gen, &n,&z, &z, &k );

    // use the skp-ahead function to get the first 5 seeds, then 5 more
    for( i=0; i<10; i++ ) {
      j = (i<5)? i+1 : i+123451;
      seeds[i] = RN_skip_ahead( &n, &j );
    }
  
    // compare
    for( i=0; i<10; i++ ) {
      j = (i<5)? i+1 : i+123451;
      printf(" %6lld  reference: %20llu  computed: %20llu\n",
               j, RN_CHECK[*new_gen-1][i], seeds[i] );
      if( seeds[i] != RN_CHECK[*new_gen-1][i] ) {
        printf(" ***** skip_test of RN generator failed\n");
      }
    }
  }
//----------------------------------------------------------------------
//
  void RNG::RN_test_mixed( int* new_gen ) {
    // test routine -- print RN's 1-5 & 123456-123460, with reference vals
    ULONG  oldseed, s, n=0ULL, one=1ULL, j;  
    int     i, z=0;

    printf("\n ***** random number - mixed test *****\n");

    // set the seed & set stride to 1
    RN_init_problem( new_gen, &one, &one, &n, &z );
    RN_set("stride", &one );

    printf(" RN_MULT   = %20llu\n", RN_MULT);
    printf(" RN_ADD    = %20llu\n", RN_ADD );
    printf(" RN_MOD    = %20llu\n", RN_MOD );
    printf(" RN_PERIOD = %20llu\n", RN_PERIOD);
    printf(" RN_MASK   = %20llu\n", RN_MASK);
    printf(" RN_STRIDE = %20llu\n", RN_STRIDE);
    printf(" RN_NORM   = %20.14e\n\n",   RN_NORM);
    for( i=0; i<10; i++ ) {
      rang();
      j = (i<5)? i+1 : i+123451;
      RN_init_particle( &j );
      s = RN_query("seed");
      printf(" %6llu  reference: %20llu  computed: %20llu\n",
             j, RN_CHECK[RN_INDEX-1][i], s );
      if( s!=RN_CHECK[RN_INDEX-1][i] ) {
        printf(" ***** mixed_test of RN generator failed\n");
      }
    }
  }
//----------------------------------------------------------------------
// #ifdef __cplusplus
// }
// #endif
