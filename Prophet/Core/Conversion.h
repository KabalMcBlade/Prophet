#pragma once

#include "Architecture.h"
#include "BasicDefines.h"
#include "Assertions.h"
#include "BasicTypes.h"


NAMESPACE_BEGIN


ALIGNED struct Float
{
	union
	{
		float m_floats[SCALAR_COUNT];
		__float m_vector;
	};

	INLINE operator __float() const { return m_vector; }
	INLINE operator const float* () const { return m_floats; }
};


ALIGNED struct UInt
{
	union
	{
		uint32 m_uint[SCALAR_COUNT];
		__float m_vector;
	};

	INLINE operator __float() const { return m_vector; }
	INLINE operator __int() const 
	{
#if ARCH & ARCH_AVX512_FLAG
		return _mm512_castps_si512(m_vector);
#elif (ARCH & ARCH_AVX_FLAG)
		return _mm256_castps_si256(m_vector);
#else
		return _mm_castps_si128(m_vector);
#endif

	}

	INLINE operator __double() const
	{
#if ARCH & ARCH_AVX512_FLAG
		return _mm512_castps_pd(m_vector);
#elif (ARCH & ARCH_AVX_FLAG)
		return _mm256_castps_pd(m_vector);
#else
		return _mm_castps_pd(m_vector);
#endif
	}
};


NAMESPACE_END