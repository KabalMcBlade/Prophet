#pragma once

#include "../Core/BasicTypes.h"

NAMESPACE_BEGIN

namespace Utils
{
	template <typename T, typename std::enable_if<std::is_arithmetic_v<T>>::type* = nullptr>
	constexpr T Abs(const T _value)
	{
		return (_value < (T)0 ? -_value : _value);
	}

	template <typename T, typename std::enable_if<std::is_arithmetic_v<T>>::type* = nullptr>
	constexpr T Min(const T _a, const T _b) 
	{
		return (_a <= _b ? _a : _b);
	}

	template <typename T, typename std::enable_if<std::is_arithmetic_v<T>>::type* = nullptr>
	constexpr T Max(const T _a, const T _b)
	{
		return (_a >= _b ? _a : _b);
	}
	
	template <typename T, typename std::enable_if<std::is_floating_point<T>::value>::type* = nullptr>
	static INLINE constexpr T Round(const T _value)
	{
		return (T)static_cast<int64>(_value + (_value >= (T)0 ? (T)0.5 : (T)-0.5));
	}

	template <typename T, typename std::enable_if<std::is_floating_point<T>::value>::type* = nullptr>
	static INLINE constexpr T Floor(const T _value)
	{
		const T castValue = (T)static_cast<int64>(_value);
		return (_value >= (T)0 ? castValue : (_value == castValue ? _value : castValue - (T)1));
	}
	
	template <typename T, typename std::enable_if<std::is_floating_point<T>::value>::type* = nullptr>
	static INLINE constexpr T Ceil(const T _value)
	{
		const T castValue = (T)static_cast<int64>(_value);
		return (_value < (T)0 ? castValue : (_value == castValue ? _value : castValue + (T)1));
	}
}

NAMESPACE_END