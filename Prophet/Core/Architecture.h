#pragma once

#if _MSC_VER < 1800
#   error "Prophet requires Visual C++ 12 - 2013 or higher"
#endif  //_MSC_VER


///////////////////////////////////////////////////////////////////////////////////
// Instruction sets

#define ARCH_IX86_FLAG		0x00000001
#define ARCH_SSE2_FLAG		0x00000002
#define ARCH_SSE3_FLAG		0x00000004
#define ARCH_SSSE3_FLAG		0x00000008
#define ARCH_SSE41_FLAG		0x00000010
#define ARCH_SSE42_FLAG		0x00000020
#define ARCH_AVX_FLAG		0x00000040
#define ARCH_AVX2_FLAG		0x00000080
#define ARCH_AVX512_FLAG	0x00000100 // Skylake-X core subset instruction


#define ARCH_X86       ARCH_IX86_FLAG
#define ARCH_SSE2      (ARCH_SSE2_FLAG | ARCH_X86)
#define ARCH_SSE3      (ARCH_SSE3_FLAG | ARCH_SSE2)
#define ARCH_SSSE3     (ARCH_SSSE3_FLAG | ARCH_SSE3)
#define ARCH_SSE41     (ARCH_SSE41_FLAG | ARCH_SSSE3)
#define ARCH_SSE42     (ARCH_SSE42_FLAG | ARCH_SSE41)
#define ARCH_AVX       (ARCH_AVX_FLAG | ARCH_SSE42)
#define ARCH_AVX2      (ARCH_AVX2_FLAG | ARCH_AVX)
#define ARCH_AVX512    (ARCH_AVX512_FLAG | ARCH_AVX2) // Skylake-X core subset instruction


#if defined(__AVX512BW__) || defined(__AVX512CD__) || defined(__AVX512DQ__) || defined(__AVX512F__) || defined(__AVX512VL__)
#define ARCH     ARCH_AVX512
#include <immintrin.h>
#elif defined(__AVX2__)
#define ARCH     ARCH_AVX2
#include <immintrin.h>
#elif defined(__AVX__)
#define ARCH     ARCH_AVX
#include <immintrin.h>
#elif defined(__SSE4_2__)
#define ARCH     ARCH_SSE42
#include <nmmintrin.h>
#elif defined(__SSE4_1__)
#define ARCH     ARCH_SSE41
#include <smmintrin.h>
#elif defined(__SSSE3__)
#define ARCH     ARCH_SSSE3
#include <tmmintrin.h>
#elif defined(__SSE3__)
#define ARCH     ARCH_SSE3
#include <pmmintrin.h>
#elif defined(__SSE2__) || defined(_M_X64)
#define ARCH     ARCH_SSE2
#include <emmintrin.h>
#elif defined(_M_IX86_FP)
#if _M_IX86_FP >= 2
#define ARCH     ARCH_SSE2
#include <emmintrin.h>
#else
#error "Prophet requires at least SSE2 Support"
#endif
#else
#error "Prophet requires at least SSE2 Support"
#endif