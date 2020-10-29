#pragma once

#include "Architecture.h"
#include "BasicDefines.h"
#include "Assertions.h"
#include "BasicTypes.h"


NAMESPACE_BEGIN




namespace Utils
{
	enum ESwizzle : uint32
	{
		X = 0,
		Y = 1,
		Z = 2,
		W = 3
	};

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

#pragma region SWIZZLE METHOD
		template<unsigned MASK>
		static INLINE Type Shuffle(Type _value)
		{
#if (ARCH & ARCH_AVX512_FLAG)
			if constexpr (std::is_same<int, T>::value)
			{
				return _mm512_shuffle_epi32(_value, _mask);
			}
			else
			{
				return _mm512_shuffle_ps(_value, _mask);
			}
#elif ARCH & ARCH_AVX2_FLAG
			if constexpr (std::is_same<int, T>::value)
			{
				return _mm256_shuffle_epi32(_value, _mask);
			}
			else
			{
				return _mm256_shuffle_ps(_value, _mask);
			}
#elif ARCH & ARCH_AVX_FLAG
			if constexpr (std::is_same<int, T>::value)
			{
				const __m256 lhv = _mm256_castsi256_ps(_value);
				const __m256 rhv = _mm256_castsi256_ps(_value);
				const __m256 shf = _mm256_shuffle_ps(lhv, rhv, MASK);
				return  _mm256_castps_si256(shf);
			}
			else
			{
				return _mm256_shuffle_ps(_value, _mask);
			}
#else
			if constexpr (std::is_same<int, T>::value)
			{
				const __m128 lhv = _mm_castsi128_ps(_value);
				const __m128 rhv = _mm_castsi128_ps(_value);
				const __m128 shf = _mm_shuffle_ps(lhv, rhv, MASK);
				return  _mm_castps_si128(shf);
			}
			else
			{
				return _mm_shuffle_ps(_value, _value, MASK);
			}
#endif
		}

		template<ESwizzle _X, ESwizzle _Y, ESwizzle _Z, ESwizzle _W>
		static constexpr INLINE Type Swizzle(Type _v)
		{
			static_assert(_X <= 3, "X parameter out of range (0 to 3)");
			static_assert(_Y <= 3, "Y parameter out of range (0 to 3)");
			static_assert(_Z <= 3, "Z parameter out of range (0 to 3)");
			static_assert(_W <= 3, "W parameter out of range (0 to 3)");

			return Shuffle<_MM_SHUFFLE(_W, _Z, _Y, _X)>(_v);
		}
#pragma endregion

		static INLINE Type Zero()
		{
#if (ARCH & ARCH_AVX512_FLAG)
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

#if (ARCH & ARCH_AVX512_FLAG)
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
#if (ARCH & ARCH_AVX512_FLAG)
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


		static INLINE void Get(T* _output, Type _value)
		{
#if (ARCH & ARCH_AVX512_FLAG)
			if constexpr (std::is_same<int, T>::value)
			{
				_mm512_stream_si512(_output, _value);
			}
			else
			{
				_mm512_stream_ps(_output, _value);
			}
#elif (ARCH & ARCH_AVX_FLAG)
			if constexpr (std::is_same<int, T>::value)
			{
				_mm256_stream_si256((__m256i*) & _output[0], _value);
			}
			else
			{
				_mm256_stream_ps(_output, _value);
			}
#else
			if constexpr (std::is_same<int, T>::value)
			{
				_mm_stream_si128((__m128i*) & _output[0], _value);
			}
			else
			{
				_mm_stream_ps(_output, _value);
			}
#endif
			_mm_mfence();
		}


		static INLINE Type Load(T const* _addr)
		{
#if (ARCH & ARCH_AVX512_FLAG)
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
#if (ARCH & ARCH_AVX512_FLAG)
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
#if (ARCH & ARCH_AVX512_FLAG)
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

		static INLINE Type Dot(Type _a, Type _b)
		{
#if (ARCH & ARCH_AVX512_FLAG)
			if constexpr (std::is_same<int, T>::value)
			{
				return _mm512_dot_pi(_a, _b);
			}
			else
			{
				return _mm512_dot_ps(_a, _b);
			}
#elif (ARCH & ARCH_AVX_FLAG)
			if constexpr (std::is_same<int, T>::value)
			{
				return _mm256_dot_pi(_a, _b);
			}
			else
			{
				return _mm256_dot_ps(_a, _b);
			}
#else
			if constexpr (std::is_same<int, T>::value)
			{
				return _mm_dot_pi(_a, _b);
			}
			else
			{
				return _mm_dot_ps(_a, _b);
			}
#endif
		}


		static INLINE T ExtractX(Type _value)
		{
#if (ARCH & ARCH_AVX512_FLAG)
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
				return _mm_cvtss_si32(_value);
			}
			else
			{
				return _mm_cvtss_f32(_value);
			}
#endif
		}

	private:
#pragma region Helper methods

#pragma endregion 
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

#pragma region Dot Product Methods

#if (ARCH & ARCH_AVX512_FLAG)
		static INLINE __m512i _mm512_dot_pi(const __m128i _a, const __m128i _b)
		{
			const __m512i mul = _mm512_mullo_epi32(_a, _b);
			const __m512i add = _mm512_add_epi32(mul, Swizzle<Y, X, W, Z>(mul));
			const __m512i res = _mm512_add_epi32(add, Swizzle<W, Z, Y, X>(add));
			return res;
		}

		static INLINE __m512 _mm512_dot_ps(const __m128 _a, const __m128 _b)
		{
			const __m128 mul = _mm512_mul_ps(_a, _b);
			const __m128 add = _mm512_add_ps(mul, Swizzle<Y, X, W, Z>(mul));
			const __m128 res = _mm512_add_ps(add, Swizzle<W, Z, Y, X>(add));
			return res;
		}
#endif

#if (ARCH & ARCH_AVX_FLAG)
		static INLINE __m256i _mm256_dot_pi(const __m128i _a, const __m128i _b)
		{
			const __m256i mul = _mm256_mullo_epi32(_a, _b);
			const __m256i add = _mm256_add_epi32(mul, Swizzle<Y, X, W, Z>(mul));
			const __m256i res = _mm256_add_epi32(add, Swizzle<W, Z, Y, X>(add));
			return res;
		}

		static INLINE __m256 _mm256_dot_ps(const __m128 _a, const __m128 _b)
		{
			const __m128 mul = _mm256_mul_ps(_a, _b);
			const __m128 add = _mm256_add_ps(mul, Swizzle<Y, X, W, Z>(mul));
			const __m128 res = _mm256_add_ps(add, Swizzle<W, Z, Y, X>(add));
			return res;
		}
#endif

		static INLINE __m128i _mm_dot_pi(const __m128i _a, const __m128i _b)
		{
#if ARCH & ARCH_SSE3_FLAG
			const __m128i mul = _mm_mul_pi(_a, _b);
			const __m128i add = _mm_hadd_epi32(mul, mul);
			const __m128i res = _mm_hadd_epi32(add, add);
			return res;
#else
			const __m128i mul = _mm_mul_pi(_a, _b);
			const __m128i add = _mm_add_pi32(mul, Swizzle<Y, X, W, Z>(mul));
			const __m128i res = _mm_add_pi32(add, Swizzle<W, Z, Y, X>(add));
			return res;
#endif
		}

		static INLINE __m128 _mm_dot_ps(const __m128 _a, const __m128 _b)
		{
#if ARCH & ARCH_SSE41_FLAG
			return _mm_dp_ps(_a, _b, 0xff); // 1111 1111 -> all values are computed and the result is saved to the whole register
#elif ARCH & ARCH_SSE3_FLAG
			const __m128 mul = _mm_mul_ps(_a, _b);
			const __m128 add = _mm_hadd_ps(mul, mul);
			const __m128 res = _mm_hadd_ps(add, add);
			return res;
#else
			const __m128 mul = _mm_mul_ps(_a, _b);
			const __m128 add = _mm_add_ps(mul, Swizzle<Y, X, W, Z>(mul));
			const __m128 res = _mm_add_ps(add, Swizzle<W, Z, Y, X>(add));
			return res;
#endif
		}
#pragma endregion
	};
}

NAMESPACE_END