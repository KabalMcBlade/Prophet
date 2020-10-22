#pragma once

#include "BasicTypes.h"


#if _WIN32 || _WIN64
#if _WIN64
#define x64
#else
#define x86
#endif
#elif __GNUC__
#if __x86_64__ || __ppc64__
#define x64
#else
#define x86
#endif
#else
#define x86
#endif


// defines for easy namespace
#define USING_PROPHET using namespace prophet; 

#define OPTIMIZATION_OFF __pragma(optimize("",off))
#define OPTIMIZATION_ON __pragma(optimize("",on))

/// forces a function to be in lined
#define INLINE __forceinline

// tells the compiler to never inline a particular function
#define NO_INLINE  __declspec(noinline)

// Memory alignment
#define MEMORY_ALIGN(x)		__declspec(align(x))
#define ALIGN(x)			MEMORY_ALIGN(x)

//#define ALIGN_PLATFORM		ALIGN(MEMORY_ALIGNMENT_SIZE)

// these are the scalar amount useful for indices  when want to address a simple array as intrinsics
#define SCALAR_COUNT_128			4
#define SCALAR_COUNT_256			8
#define SCALAR_COUNT_512			16

#define MEM_ALIGN_16		16
#define MEM_ALIGN_32		32

#if ARCH & ARCH_AVX512_FLAG
#define SCALAR_COUNT	SCALAR_COUNT_512
#define MEM_ALIGN		MEM_ALIGN_32
#define ALIGNED			ALIGN(MEM_ALIGN)
#elif (ARCH & ARCH_AVX_FLAG)
#define SCALAR_COUNT	SCALAR_COUNT_256
#define MEM_ALIGN		MEM_ALIGN_32
#define ALIGNED			ALIGN(MEM_ALIGN)
#else
#define SCALAR_COUNT	SCALAR_COUNT_128
#define MEM_ALIGN		MEM_ALIGN_16
#define ALIGNED			ALIGN(MEM_ALIGN)
#endif


#define IS_ALIGNED(ptr, alignment)    (((prophet::uintPtr)ptr & (alignment - 1)) == 0)

