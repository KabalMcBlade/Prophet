#pragma once

#include <ostream>

#include "Array.h"


NAMESPACE_BEGIN


namespace __private
{
	template<typename T>
	class Array<T, 1>
	{
		static_assert(std::numeric_limits<T>::is_integer || std::is_same<float, T>::value, "Scalar can be only integer or float");

	public:
		typedef T Type;

		//
		// CONSTRUCTORS
		INLINE Array() : m_data(static_cast<T>(0)) {}
		INLINE Array(Array const& _other) : m_data(_other) {}
		INLINE Array(T _value) : m_data(_value) {}

		//
		// MEMBER DATA
		ALIGNED Type m_data;

		//
		// ACCESSORS
		INLINE constexpr unsigned Rank() const { return 0; }
		INLINE constexpr unsigned Dim() const { return 0; }
	
		//
		// OPERATORS
		INLINE operator T const& () const { return m_data; }
		INLINE operator T& () { return m_data; }
		INLINE friend std::ostream& operator<<(std::ostream& _os, const Array<T>& _other)
		{
			_os << _other;
			return _os;
		}
	};
}

template <typename T>
using Scalar = __private::Array<T, 1>;



NAMESPACE_END