#pragma once


#include "Vector.h"


NAMESPACE_BEGIN


namespace __private
{
	template<typename T, unsigned PRIMARY_DIMENSION, unsigned SECONDARY_DIMENSION>
	class Array<T, PRIMARY_DIMENSION, SECONDARY_DIMENSION>
	{
		static_assert(std::numeric_limits<T>::is_integer || std::is_same<float, T>::value, "Matrix can be only integer or float");

	public:
		typedef typename Array<T, SECONDARY_DIMENSION>::Type OneDimensionDownArrayT;
		typedef OneDimensionDownArrayT Type[PRIMARY_DIMENSION];

		//
		// CONSTRUCTORS
		INLINE Array() = default;
		INLINE Array(Array const&) = default;

		//
		// MEMBER DATA
		ALIGNED Type m_data;

		//
		// ACCESSORS
		INLINE constexpr unsigned Rank() const { return 2; }
		INLINE constexpr unsigned Rows() const { return PRIMARY_DIMENSION; }
		INLINE constexpr unsigned Columns() const { return SECONDARY_DIMENSION; }

		//
		// OPERATORS
		INLINE const OneDimensionDownArrayT& operator[](unsigned _i) const { return m_data[_i]; }
		INLINE OneDimensionDownArrayT& operator[](unsigned _i) { return m_data[_i]; }
		INLINE operator T const* () const { return &m_data[0][0]; }
		INLINE operator T* () { return &m_data[0][0]; }

		INLINE friend std::ostream& operator<<(std::ostream& _os, const Array& _other)
		{
			for (uint32 i = 0; i < PRIMARY_DIMENSION; ++i)
			{
				for (uint32 j = 0; j < SECONDARY_DIMENSION; ++j)
				{
					_os << _other[i][j] << " ";
				}
				std::cout << std::endl;
			}

			return _os;
		}

		//
		// METHODS
		INLINE void Clear()
		{
			memset(&m_data[0][0], 0, sizeof(m_data));
		}

		INLINE void CopyFrom(Array const& _other)
		{
			Utils::MemCpy(&m_data[0][0], &_other.m_data[0][0], sizeof(m_data));
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
					for (unsigned j = 0; j < SECONDARY_DIMENSION; ++j)
					{
						m_data[i][j] = uniform(generator);
					}
				}
			}
			else
			{
				std::uniform_int_distribution<T> uniform(-1, 1);

				for (unsigned i = 0; i < PRIMARY_DIMENSION; ++i)
				{
					for (unsigned j = 0; j < SECONDARY_DIMENSION; ++j)
					{
						m_data[i][j] = uniform(generator);
					}
				}
			}
		}

		INLINE void SetAddress(const uint32 _offset, const typename Utils::SimdHelper<T>::Type& _value)
		{
			if (_offset < PRIMARY_DIMENSION * SECONDARY_DIMENSION)
			{
				ALIGNED T* ptr = &m_data[0][0];
				Utils::SimdHelper<T>::Get(&ptr[_offset * SCALAR_COUNT], _value);
			}
		}
	};
}

template <typename T, unsigned PRIMARY_DIMENSION, unsigned SECONDARY_DIMENSION>
using Matrix = __private::Array<T, PRIMARY_DIMENSION, SECONDARY_DIMENSION>;


NAMESPACE_END