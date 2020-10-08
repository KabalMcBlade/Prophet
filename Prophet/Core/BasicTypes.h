#pragma once

#include <cstddef>	// for size_t
#include <cstdint>	// for everything else

#include <stdint.h>     // for u8, etc...
#include <math.h>

#include <cmath>
#include <limits>
#include <utility>      // for move, etc...

#ifdef _DEBUG
#include <iostream>
#endif

#include "Architecture.h"


// defines for easy namespace
#define NAMESPACE_BEGIN namespace prophet {
#define NAMESPACE_END };  


NAMESPACE_BEGIN


#ifdef max
#undef max
#endif //

#ifdef min
#undef min
#endif //


#define MATH_CONST extern const __declspec(selectany)


typedef std::int8_t     int8;
typedef std::int16_t    int16;
typedef std::int32_t    int32;
typedef std::int64_t    int64;

typedef std::uint8_t    uint8;
typedef std::uint16_t   uint16;
typedef std::uint32_t   uint32;
typedef std::uint64_t   uint64;

typedef std::size_t     size;

typedef std::intptr_t   intPtr;
typedef std::uintptr_t  uintPtr;


#if ARCH & ARCH_AVX512_FLAG
typedef __m512 __float;
typedef __m512i __int;
typedef __m512d __double;
#elif (ARCH & ARCH_AVX_FLAG)
typedef __m256 __float;
typedef __m256i __int;
typedef __m256d __double;
#else
typedef __m128 __float;
typedef __m128i __int;
typedef __m128d __double;
#endif

NAMESPACE_END