//mcnp_random.h
//This is the header file for mcnp_random.c
#ifndef MCNP_RANDOM_H
#define MCNP_RANDOM_H
//---------------------------------------------------
// Kinds for LONG INTEGERS (64-bit) & REAL*8 (64-bit)
//---------------------------------------------------
#define	 LONG  long long
#define	ULONG  unsigned long long
#define	 REAL  double

class RNG {
//-------------------------------------
// Public interface for functions
//-------------------------------------
    public:
        REAL   rang(void);
        ULONG  RN_skip_ahead( ULONG* seed, LONG* nskip );
        void   RN_init_problem( int*   new_standard_gen,
                              ULONG* new_seed,
                              ULONG* new_stride,
                              ULONG* new_part1,
                              int*   print_info );
        void   RN_init_particle( ULONG* nps );
        void   RN_set( char* key, ULONG* value );
        ULONG  RN_query( char* key );
        ULONG  RN_query_first( ULONG* nps );
        void   RN_update_stats( void );
        void   RN_test_basic( int* new_gen );
        void   RN_test_skip(  int* new_gen );
        void   RN_test_mixed( int* new_gen );
};

#endif

