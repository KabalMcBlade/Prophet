#pragma once

#include "../Core/Architecture.h"
#include "../Core/BasicDefines.h"
#include "../Core/Assertions.h"
#include "../Core/BasicTypes.h"
#include "../Core/Alignment.h"


NAMESPACE_BEGIN

template<typename T, uint32 ROWS, uint32 COLUMNS>
class Matrix : public MathAligned<MEM_ALIGN>
{
	static_assert(ROWS > 0, "Matrix rows must be greater than 0");
	static_assert(std::numeric_limits<T>::is_integer || std::is_floating_point<T>::value, "Matrix can be only instantiated with integer, float or double");

public:
	INLINE Matrix()
	{
		memset(m_buffer, 0, sizeof(m_buffer));
	}

	INLINE void SetRandomUniformDistribution()
	{
#if ARCH & ARCH_AVX512_FLAG
		// 512
#elif (ARCH & ARCH_AVX_FLAG)
		// 256
#else
		// 128
#endif
	}

	INLINE uint32 GetRowsCount() const { return ROWS; };
	INLINE uint32 GetColumnsCount() const { return COLUMNS; };

private:
	using Type = typename std::conditional<std::is_same<int, T>::value, __int,
		typename std::conditional<std::is_same<float, T>::value, __float,
			__double
		>::type
	>::type;

	//Type m_buffer[Ceil(ROWS / SCALAR_COUNT) % SCALAR_COUNT][Ceil(COLUMNS / SCALAR_COUNT) % SCALAR_COUNT];
	Type m_buffer[ROWS][COLUMNS];
	//ALIGNED T m_buffer[ROWS][COLUMNS];
};



template<typename T, uint32 ROWS>
class Matrix<T, ROWS, 0> : public MathAligned<MEM_ALIGN>
{
	static_assert(ROWS > 0, "Matrix rows must be greater than 0");
	static_assert(std::numeric_limits<T>::is_integer || std::is_floating_point<T>::value, "Matrix can be only instantiated with integer, float or double");

public:
	INLINE Matrix()
	{
		memset(m_buffer, 0, sizeof(m_buffer));
	}

	INLINE void SetRandomUniformDistribution()
	{
#if ARCH & ARCH_AVX512_FLAG
		// 512
#elif (ARCH & ARCH_AVX_FLAG)
		// 256
#else
		// 128
#endif
	}

	INLINE uint32 GetRowsCount() const { return ROWS; };
	INLINE uint32 GetColumnsCount() const { return 0; };

private:
	using Type = typename std::conditional<std::is_same<int, T>::value, __int,
		typename std::conditional<std::is_same<float, T>::value, __float,
		__double
		>::type
	>::type;

	//Type m_buffer[Ceil(ROWS / SCALAR_COUNT) % SCALAR_COUNT][Ceil(COLUMNS / SCALAR_COUNT) % SCALAR_COUNT];
	Type m_buffer[ROWS];
};


NAMESPACE_END