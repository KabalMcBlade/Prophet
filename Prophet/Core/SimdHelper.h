#pragma once

#include "Architecture.h"
#include "BasicDefines.h"
#include "Assertions.h"
#include "BasicTypes.h"


NAMESPACE_BEGIN

namespace Utils
{

	template<typename T>
	class SimdHelper
	{
		static_assert(std::numeric_limits<T>::is_integer || std::is_same<float, T>::value, "SimdHelper can be only instantiated with integer or float");

	public:
		using Type = typename std::conditional<std::is_same<int, T>::value, __int,
			typename std::conditional<std::is_same<float, T>::value, __float,
			__double
			>::type
		>::type;

	public:
		static INLINE Type Zero()
		{
#if ARCH & ARCH_AVX512_FLAG
			if constexpr (std::is_same<int, T>::value)
			{
				return _mm512_setzero_si512();
			}
			else
			{
				return _mm512_setzero_ps();
			}
#elif (ARCH & ARCH_AVX_FLAG)
			if constexpr (std::is_same<int, T>::value)
			{
				return _mm256_setzero_si256();
			}
			else
			{
				return _mm256_setzero_ps();
			}
#else
			if constexpr (std::is_same<int, T>::value)
			{
				return _mm_setzero_si128();
			}
			else
			{
				return _mm_setzero_ps();
			}
#endif
		}

		template<unsigned _INDEX>
		static INLINE T GetValueByIndex(Type _value)
		{
			static_assert(_INDEX <= SCALAR_COUNT, "Index out of range");

#if ARCH & ARCH_AVX512_FLAG
			if constexpr (std::is_same<int, T>::value)
			{
				return _mm512_cvtsi512_si32(_mm512_alignr_epi32(_value, _value, _INDEX));
			}
			else
			{
				return _mm512_cvtss_f32(_mm512_alignr_epi32(_mm512_castps_si512(_value), _mm512_castps_si512(_value), _INDEX));
			}
#elif (ARCH & ARCH_AVX_FLAG)
			if constexpr (std::is_same<int, T>::value)
			{
				return _mm256_extract_epi32(_value, _INDEX);
			}
			else
			{
				return _mm256_extract_ps<_INDEX>(_value);
			}
#else
			if constexpr (std::is_same<int, T>::value)
			{
				return _mm_extract_pi<_INDEX>(_value);
			}
			else
			{
				return _mm_extract_ps<_INDEX>(_value);
			}
#endif
		}

		template<>
		static INLINE T GetValueByIndex<0>(Type _value)
		{
#if ARCH & ARCH_AVX512_FLAG
			if constexpr (std::is_same<int, T>::value)
			{
				return _mm512_cvtsi512_si32(_value);
			}
			else
			{
				return _mm512_cvtss_f32(_value);
			}
#elif (ARCH & ARCH_AVX_FLAG)
			if constexpr (std::is_same<int, T>::value)
			{
				return _mm256_cvtsi256_si32(_value);
			}
			else
			{
				return _mm256_cvtss_f32(_value);
			}
#else
			if constexpr (std::is_same<int, T>::value)
			{
				return _mm_cvtsi128_si32(_value);
			}
			else
			{
				return _mm_cvtss_f32(_value);
			}
#endif
		}

		static INLINE Type Load(T const* _addr)
		{
#if ARCH & ARCH_AVX512_FLAG
			if constexpr (std::is_same<int, T>::value)
			{
				return _mm512_load_si512((__m512i const*)(&(_addr[0])));
		}
			else
			{
				return _mm512_load_ps(_addr);
			}
#elif (ARCH & ARCH_AVX_FLAG)
			if constexpr (std::is_same<int, T>::value)
			{
				return _mm256_load_si256((__m256i const*)(&(_addr[0])));
			}
			else
			{
				return _mm256_load_ps(_addr);
			}
#else
			if constexpr (std::is_same<int, T>::value)
			{
				return _mm_load_si128((__m128i const*)(&(_addr[0])));
			}
			else
			{
				return _mm_load_ps(_addr);
			}
#endif
		}

		static INLINE Type Load(T _value)
		{
#if ARCH & ARCH_AVX512_FLAG
			if constexpr (std::is_same<int, T>::value)
			{
				return _mm512_set1_epi32(_value);
			}
			else
			{
				return _mm512_set1_ps(_value);
			}
#elif (ARCH & ARCH_AVX_FLAG)
			if constexpr (std::is_same<int, T>::value)
			{
				return _mm256_set1_epi32(_value);
			}
			else
			{
				return _mm256_set1_ps(_value);
			}
#else
			if constexpr (std::is_same<int, T>::value)
			{
				return _mm_set1_epi32(_value);
			}
			else 
			{
				return _mm_set1_ps(_value);
			}
#endif
		}

		static INLINE Type Mul(Type _a, Type _b)
		{
#if ARCH & ARCH_AVX512_FLAG
			if constexpr (std::is_same<int, T>::value)
			{
				return _mm512_mullo_epi32(_a, _b);
			}
			else
			{
				return _mm512_mul_ps(_a, _b);
			}
#elif (ARCH & ARCH_AVX_FLAG)
			if constexpr (std::is_same<int, T>::value)
			{
				return _mm256_mullo_epi32(_a, _b);
			}
			else
			{
				return _mm256_mul_ps(_a, _b);
			}
#else
			if constexpr (std::is_same<int, T>::value)
			{
				return _mm_mul_pi(_a, _b);
			}
			else
			{
				return _mm_mul_ps(_a, _b);
			}
#endif
		}

	private:
		static INLINE __m128i _mm_mul_pi(const __m128i& _a, const __m128i& _b)
		{
#if ARCH & ARCH_SSE41_FLAG
			return _mm_mullo_epi32(_a, _b);
#else
			const __m128i tmp1 = _mm_mul_epu32(_a, _b); // multiply _a[2], _b[0]
			const __m128i tmp2 = _mm_mul_epu32(_mm_srli_si128(_a, 4), _mm_srli_si128(_b, 4)); // multiply _a[3], _b[1]
			return _mm_unpacklo_epi32(_mm_shuffle_epi32(tmp1, _MM_SHUFFLE(0, 0, 2, 0)), _mm_shuffle_epi32(tmp2, _MM_SHUFFLE(0, 0, 2, 0))); /* shuffle results to [63..0] and pack */
#endif
		}

		template<unsigned _INDEX>
		static INLINE float _mm_extract_ps(const __m128& _value)
		{
			static_assert(_INDEX <= SCALAR_COUNT, "Index out of range");

#if ARCH & ARCH_SSE41_FLAG
			float r;
			_MM_EXTRACT_FLOAT(r, _value, _INDEX);	// TO DO: Check if I have to invert the Imm8 index value, it depends how I will load in the matrix, I'll check later!
			return r;
#else
			return _mm_cvtss_f32(_mm_shuffle_ps(_value, _value, _MM_SHUFFLE(_INDEX, _INDEX, _INDEX, _INDEX)));
#endif
		}

		template<unsigned _INDEX>
		static INLINE int _mm_extract_pi(const __m128i& _value)
		{
			static_assert(_INDEX <= SCALAR_COUNT, "Index out of range");

#if ARCH & ARCH_SSE41_FLAG
			return _mm_extract_epi32(_value, _INDEX);	// TO DO: Check if I have to invert the Imm8 index value, it depends how I will load in the matrix, I'll check later!
#else
			return _mm_cvtss_i32(_mm_shuffle_epi32(_value, _MM_SHUFFLE(_INDEX, _INDEX, _INDEX, _INDEX)));
#endif
		}


#if (ARCH & ARCH_AVX_FLAG)
		template<unsigned _INDEX>
		static INLINE float _mm256_extract_ps(__m256 _value)
		{
			static_assert(_INDEX <= SCALAR_COUNT, "Index out of range");

			if constexpr (_INDEX == 7)
			{
				const __m128 hi = _mm256_extractf128_ps(_value, 1);
				return _mm_cvtss_f32(_mm_shuffle_ps(hi, hi, 3));
			}
			else if constexpr (_INDEX == 6)
			{
				const __m128 hi = _mm256_extractf128_ps(_value, 1);
				return _mm_cvtss_f32(_mm_movehl_ps(hi, hi));
			}
			else if constexpr (_INDEX == 5)
			{
				const __m128 hi = _mm256_extractf128_ps(_value, 1);
				return _mm_cvtss_f32(_mm_shuffle_ps(hi, hi, 1));
			}
			else if constexpr (_INDEX == 4)
			{
				return _mm_cvtss_f32(_mm256_extractf128_ps(_value, 1));
			}
			else if constexpr (_INDEX == 3)
			{
				const __m128 lo = _mm256_extractf128_ps(_value, 0);
				return _mm_cvtss_f32(_mm_shuffle_ps(lo, lo, 3));
			}
			else if constexpr (_INDEX == 2)
			{
				const __m128 lo = _mm256_extractf128_ps(_value, 0);
				return _mm_cvtss_f32(_mm_movehl_ps(lo, lo));
			}
			else if constexpr (_INDEX == 1)
			{
				const __m128 lo = _mm256_extractf128_ps(_value, 0);
				return _mm_cvtss_f32(_mm_shuffle_ps(lo, lo, 1));
			}
			else
			{
				return _mm_cvtss_f32(_mm256_extractf128_ps(_value, 0));
			}


			/*
			 ALIGNED float buffer[8]; 
			_mm256_store_ps(buffer, _value);

			float v0 = _mm_cvtss_f32(_mm256_castps256_ps128(_value));
			float v1 = buffer[1];
			float v2 = buffer[2];
			...
			*/
		}
#endif

	};
}

NAMESPACE_END