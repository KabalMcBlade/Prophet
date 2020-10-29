#pragma once


#include "Matrix.h"


NAMESPACE_BEGIN


namespace __private
{
#pragma region Variadic template aggregate
	template<size ...>
	struct AddAll : std::integral_constant< size, 0 > {};

	template<size X, size ... Xs>
	struct AddAll<X, Xs...> : std::integral_constant< size, X + AddAll<Xs...>::value > {};

	template<size ...>
	struct MulAll : std::integral_constant< size, 1 > {};

	template<size X, size ... Xs>
	struct MulAll<X, Xs...> : std::integral_constant< size, X* MulAll<Xs...>::value > {};
#pragma endregion

	template<typename T, unsigned PRIMARY_DIMENSION, unsigned ... OTHER_DIMENSIONS>
	class Array<T, PRIMARY_DIMENSION, OTHER_DIMENSIONS...>
	{
		static_assert(std::numeric_limits<T>::is_integer || std::is_same<float, T>::value, "Tensor can be only integer or float");

	public:
#pragma region Constants values
		static constexpr int32 kDimensionSize = PRIMARY_DIMENSION + AddAll< OTHER_DIMENSIONS... >::value;
		static constexpr int32 kDimensionShape = PRIMARY_DIMENSION * MulAll< OTHER_DIMENSIONS... >::value;
#pragma endregion

#pragma region Types
		typedef typename T Param;
		typedef typename Array<Param, OTHER_DIMENSIONS...>::Type OneDimensionDownArrayT;
		typedef OneDimensionDownArrayT Type[PRIMARY_DIMENSION];
#pragma endregion

#pragma region Constructors
		INLINE Array() = default;
		INLINE Array(Array const&) = default;
#pragma endregion

#pragma region Member variables
		ALIGNED Type m_data;
#pragma endregion

#pragma region Accessors
		INLINE constexpr unsigned Rank() const { return sizeof...(OTHER_DIMENSIONS) + PRIMARY_DIMENSION; }
#pragma endregion

#pragma region Operators
		INLINE const OneDimensionDownArrayT& operator[](unsigned _i) const { return m_data[_i]; }
		INLINE OneDimensionDownArrayT& operator[](unsigned _i) { return m_data[_i]; }
		INLINE operator Param const* () const { return GetBaseAddressConst(); }
		INLINE operator Param* () { return GetBaseAddress(); }

		INLINE friend std::ostream& operator<<(std::ostream& _os, const Array& _other)
		{
			Print(_os, _other, PRIMARY_DIMENSION, OTHER_DIMENSIONS...);
			return _os;
		}
#pragma endregion

#pragma region Public Methods
		INLINE void Clear()
		{
			memset(GetBaseAddress(), 0, sizeof(m_data));
		}

		INLINE void CopyFrom(const Array& _other)
		{
			Utils::MemCpy(GetBaseAddress(), _other, sizeof(m_data));
		}

		INLINE void SetRandomUniformDistribution()
		{
			std::random_device rd;
			std::default_random_engine generator(rd());

			Param dist[kDimensionShape];
			if constexpr (std::is_same<float, T>::value)
			{
				std::uniform_real_distribution<T> uniform(-1.0f, 1.0f);
				for (uint32 i = 0; i < kDimensionShape; ++i)
				{
					dist[i] = uniform(generator);
				}
			}
			else
			{
				std::uniform_int_distribution<T> uniform(-1, 1);
				for (uint32 i = 0; i < kDimensionShape; ++i)
				{
					dist[i] = uniform(generator);
				}
			}

			constexpr uint32 left = kDimensionShape % SCALAR_COUNT;
			constexpr uint32 steps = (kDimensionShape / SCALAR_COUNT) + (left != 0);

			for (uint32 i = 0, j = 0; i < steps; ++i, j += SCALAR_COUNT)
			{
				const typename Utils::SimdHelper<T>::Type buffer = Utils::SimdHelper<T>::Load(&dist[j]);
				SetAddress(i, buffer);
			}
		}

		INLINE void SetAddress(const uint32 _offset, const typename Utils::SimdHelper<T>::Type& _value)
		{
			if (_offset < kDimensionShape)
			{
				ALIGNED T* ptr = GetBaseAddress();
				Utils::SimdHelper<T>::Get(&ptr[_offset * SCALAR_COUNT], _value);
			}
		}
#pragma endregion

	private:
#pragma region Private members
		mutable Param const* m_baseAddressConst = nullptr;
		Param* m_baseAddress = nullptr;
#pragma endregion

#pragma region Print methods
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
#pragma endregion

#pragma region Get base address methods
		INLINE Param* GetBaseAddress()
		{
			if (m_baseAddress)
			{
				return m_baseAddress;
			}
			else
			{
				m_baseAddress = GetBaseAddress(m_data, PRIMARY_DIMENSION, OTHER_DIMENSIONS...);
				return m_baseAddress;
			}
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
#pragma endregion

#pragma region Get base address methods (CONST VERSION)
		INLINE Param const* GetBaseAddressConst() const
		{
			if (m_baseAddressConst)
			{
				return m_baseAddressConst;
			}
			else
			{
				m_baseAddressConst = GetBaseAddressConst(m_data, PRIMARY_DIMENSION, OTHER_DIMENSIONS...);
				return m_baseAddressConst;
			}
		}

		INLINE Array::Param const* GetBaseAddressConst(const Array::Param& _value) const
		{
			return &_value;
		}

		template <typename BEGIN, typename... REST>
		INLINE Array::Param const* GetBaseAddressConst(const  Array::Param* _other, const BEGIN& _begin, const REST&... _rest) const
		{
			const auto& data = _other[0];
			return GetBaseAddressConst(data);
		}

		template <typename T, typename BEGIN, typename... REST>
		INLINE Array::Param const* GetBaseAddressConst(const T& _other, const BEGIN& _begin, const REST&... _rest) const
		{
			const auto& data = _other[0];
			return GetBaseAddressConst(data, _rest...);
		}

		template <typename BEGIN, typename... REST>
		INLINE Array::Param const* GetBaseAddressConst(const OneDimensionDownArrayT& _other, const BEGIN& _begin, const REST&... _rest) const
		{
			const auto& data = _other[0];
			return GetBaseAddressConst(data, _rest...);
		}

		template <typename BEGIN, typename... REST>
		INLINE Array::Param const* GetBaseAddressConst(const Array& _other, const BEGIN& _begin, const REST&... _rest) const
		{
			const auto& data = _other[0];
			return GetBaseAddressConst(data, _rest...);
		}
#pragma endregion
	};
}

template <typename T, unsigned PRIMARY_DIMENSION, unsigned ... OTHER_DIMENSION>
using Tensor = __private::Array<T, PRIMARY_DIMENSION, OTHER_DIMENSION...>;


NAMESPACE_END