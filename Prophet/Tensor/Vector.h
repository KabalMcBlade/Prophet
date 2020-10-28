#pragma once

#include <random>
#include <functional>
#include <time.h>

#include "Scalar.h"


NAMESPACE_BEGIN


namespace __private
{
	template<typename T, unsigned PRIMARY_DIMENSION>
	class Array<T, PRIMARY_DIMENSION>
	{
		static_assert(std::numeric_limits<T>::is_integer || std::is_same<float, T>::value, "Vector can be only integer or float");

	public:
		typedef T Type[PRIMARY_DIMENSION];

		//
		// CONSTRUCTORS
		INLINE Array() = default;
		INLINE Array(Array const&) = default;

		//
		// MEMBER DATA
		ALIGNED Type m_data;

		//
		// ACCESSORS
		INLINE constexpr unsigned Rank() const { return 1; }
		INLINE constexpr unsigned Dim() const { return PRIMARY_DIMENSION; }

		//
		// OPERATORS
		INLINE const T& operator[](unsigned _i) const { return m_data[_i]; }
		INLINE T& operator[](unsigned _i) { return m_data[_i]; }
		INLINE operator T const* () const { return &m_data[0]; }
		INLINE operator T* () { return &m_data[0]; }

		INLINE friend std::ostream& operator<<(std::ostream& _os, const Array<T, PRIMARY_DIMENSION>& _other)
		{
			for (uint32 i = 0; i < PRIMARY_DIMENSION; ++i)
			{
				_os << _other[i] << " ";
			}

			std::cout << std::endl;

			return _os;
		}

		//
		// METHODS
		INLINE void Clear()
		{
			memset(&m_data[0], 0, sizeof(m_data));
		}

		INLINE void CopyFrom(Array const& _other)
		{
			Utils::MemCpy(&m_data[0], &_other.m_data[0], sizeof(m_data));
		}

		INLINE void SetRandomUniformDistribution()
		{
			std::random_device rd;
			std::default_random_engine generator(rd());

			if constexpr (std::is_same<float, T>::value)
			{
				std::uniform_real_distribution<T> uniform(-1.0f, 1.0f);

				for (unsigned i = 0; i < PRIMARY_DIMENSION; ++i)
				{
					m_data[i] = uniform(generator);
				}
			}
			else
			{
				std::uniform_int_distribution<T> uniform(-1, 1);

				for (unsigned i = 0; i < PRIMARY_DIMENSION; ++i)
				{
					m_data[i] = uniform(generator);
				}
			}
		}

		INLINE void SetAddress(const uint32 _offset, const typename Utils::SimdHelper<T>::Type& _value)
		{
			if (_offset < PRIMARY_DIMENSION)
			{
				ALIGNED T* ptr = &m_data[0];
				Utils::SimdHelper<T>::Get(&m_data[_offset * SCALAR_COUNT], _value);
			}
		}
	};
}

template <typename T, unsigned PRIMARY_DIMENSION>
using Vector = __private::Array<T, PRIMARY_DIMENSION>;


NAMESPACE_END