#pragma once

#include "../Core/BasicTypes.h"

NAMESPACE_BEGIN

namespace Utils
{
	template <typename T, typename std::enable_if<std::is_floating_point<T>::value>::type* = nullptr>
	static INLINE constexpr T Floor(T _value) 
	{
		const T castValue = (T)static_cast<int64>(_value);
		return (_value >= (T)0 ? castValue : (_value == castValue ? _value : castValue - (T)1));
	}
	
	template <typename T, typename std::enable_if<std::is_floating_point<T>::value>::type* = nullptr>
	static INLINE constexpr T Ceil(T _value)
	{
		const T castValue = (T)static_cast<int64>(_value);
		return (_value < (T)0 ? castValue : (_value == castValue ? _value : castValue + (T)1));
	}
}

NAMESPACE_END