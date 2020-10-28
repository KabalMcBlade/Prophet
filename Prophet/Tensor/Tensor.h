#pragma once


#include "Matrix.h"


NAMESPACE_BEGIN


namespace __private
{
	template<typename T, unsigned PRIMARY_DIMENSION, unsigned ... OTHER_DIMENSIONS>
	class Array<T, PRIMARY_DIMENSION, OTHER_DIMENSIONS...>
	{
		static_assert(std::numeric_limits<T>::is_integer || std::is_same<float, T>::value, "Tensor can be only integer or float");

	public:
		typedef typename T Param;
		typedef typename Array<Param, OTHER_DIMENSIONS...>::Type OneDimensionDownArrayT;
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
		INLINE constexpr unsigned Rank() const { return sizeof...(OTHER_DIMENSIONS) + PRIMARY_DIMENSION; }

		//
		// OPERATORS
		INLINE const OneDimensionDownArrayT& operator[](unsigned _i) const { return m_data[_i]; }
		INLINE OneDimensionDownArrayT& operator[](unsigned _i) { return m_data[_i]; }
		INLINE operator Param const* () const { return GetBaseAddress(); }
		INLINE operator Param* () { return GetBaseAddress(); }

		INLINE friend std::ostream& operator<<(std::ostream& _os, const Array& _other)
		{
			Print(_os, _other, PRIMARY_DIMENSION, OTHER_DIMENSIONS...);
			return _os;
		}

		//
		// METHODS
		INLINE void Clear()
		{
			memset(GetBaseAddress(), 0, sizeof(m_data));
		}

		INLINE void CopyFrom(Array& _other)
		{
			Utils::MemCpy(GetBaseAddress(), _other, sizeof(m_data));
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
// 					for (unsigned j = 0; j < SECONDARY_DIMENSION; ++j)
// 					{
// 						m_data[i][j] = uniform(generator);
// 					}
				}
			}
			else
			{
				std::uniform_int_distribution<T> uniform(-1, 1);

				for (unsigned i = 0; i < PRIMARY_DIMENSION; ++i)
				{
// 					for (unsigned j = 0; j < SECONDARY_DIMENSION; ++j)
// 					{
// 						m_data[i][j] = uniform(generator);
// 					}
				}
			}
		}

	private:
		//
		// METHODS

		//
		// BEGIN PRINT FUNCTIONS
		static INLINE void Print(std::ostream& _os, const Array::Param& _value)
		{
			_os << _value << " ";
		}

		template <typename BEGIN, typename... REST>
		static INLINE void Print(std::ostream& _os, const Array::Param* _other, const BEGIN& _begin, const REST&... _rest)
		{
			for (unsigned i = 0; i < _begin; ++i)
			{
				const auto& data = _other[i];
				Print(_os, data);
			}
			_os << std::endl;
		}

		template <typename T, typename BEGIN, typename... REST>
		static INLINE void Print(std::ostream& _os, const T& _other, const BEGIN& _begin, const REST&... _rest)
		{
			for (unsigned i = 0; i < _begin; ++i)
			{
				const auto& data = _other[i];
				Print(_os, data, _rest...);
			}
			_os << std::endl;
		}

		template <typename BEGIN, typename... REST>
		static INLINE void Print(std::ostream& _os, const OneDimensionDownArrayT& _other, const BEGIN& _begin, const REST&... _rest)
		{
			for (unsigned i = 0; i < _begin; ++i)
			{
				const auto& data = _other[i];
				Print(_os, data, _rest...);
			}
			_os << std::endl;
		}

		template <typename BEGIN, typename... REST>
		static INLINE void Print(std::ostream& _os, const Array& _other, const BEGIN& _begin, const REST&... _rest)
		{
			for (unsigned i = 0; i < _begin; ++i)
			{
				const auto& data = _other[i];
				Print(_os, data, _rest...);
			}
			_os << std::endl;
		}
		// END PRINT FUNCTIONS
		//

		//
		// BEGIN GET BASE ADDRESS FUNCTIONS
		INLINE Param* GetBaseAddress()
		{
			return GetBaseAddress(m_data, PRIMARY_DIMENSION, OTHER_DIMENSIONS...);
		}

		INLINE Array::Param* GetBaseAddress(Array::Param& _value)
		{
			return &_value;
		}

		template <typename BEGIN, typename... REST>
		INLINE Array::Param* GetBaseAddress(Array::Param* _other, const BEGIN& _begin, const REST&... _rest)
		{
			auto& data = _other[0];
			return GetBaseAddress(data);
		}

		template <typename T, typename BEGIN, typename... REST>
		INLINE Array::Param* GetBaseAddress(T& _other, const BEGIN& _begin, const REST&... _rest)
		{
			auto& data = _other[0];
			return GetBaseAddress(data, _rest...);
		}

		template <typename BEGIN, typename... REST>
		INLINE Array::Param* GetBaseAddress(OneDimensionDownArrayT& _other, const BEGIN& _begin, const REST&... _rest)
		{
			auto& data = _other[0];
			return GetBaseAddress(data, _rest...);
		}

		template <typename BEGIN, typename... REST>
		INLINE Array::Param* GetBaseAddress(Array& _other, const BEGIN& _begin, const REST&... _rest)
		{
			auto& data = _other[0];
			return GetBaseAddress(data, _rest...);
		}
		// END GET BASE ADDRESS FUNCTIONS
		//
	};
}

template <typename T, unsigned PRIMARY_DIMENSION, unsigned ... OTHER_DIMENSION>
using Tensor = __private::Array<T, PRIMARY_DIMENSION, OTHER_DIMENSION...>;


NAMESPACE_END