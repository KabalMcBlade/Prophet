#pragma once

#include <random>
#include <functional>

#include <time.h>

#include "../Core/Architecture.h"
#include "../Core/BasicDefines.h"
#include "../Core/Assertions.h"
#include "../Core/BasicTypes.h"
#include "../Core/Alignment.h"
#include "../Core/MemCpy.h"

#include "MathHelper.h"


NAMESPACE_BEGIN

/// <summary>
/// Matrix 2D
/// </summary>
/// <typeparam name="T"></typeparam>
template<typename T, uint32 ROWS, uint32 COLUMNS>
class Matrix : public MathAligned<MEM_ALIGN>
{
	static_assert(ROWS > 0 && COLUMNS > 0, "Matrix 2D rows and column must be greater than 0");
	static_assert(std::numeric_limits<T>::is_integer || std::is_same<float, T>::value, "Matrix 2D can be only instantiated with integer or float");


public:
	INLINE Matrix()
	{
		memset(m_buffer, 0, sizeof(m_buffer));
	}

	INLINE Matrix(const Matrix& a)
	{
		Utils::MemCpy(&m_buffer[0][0], &a.m_buffer[0][0], sizeof(m_buffer));
	}

	INLINE void SetRandomUniformDistribution()
	{
		int32 seed = static_cast<int32>(time(0));
		std::default_random_engine random_engine(seed);

		std::uniform_real_distribution<T> uniform(-1, 1);

		for (uint32 row = 0; row < ROWS; ++row)
		{
			for (uint32 col = 0; col < COLUMNS; ++col)
			{
				m_buffer[row][col] = uniform(random_engine);
			}
		}
	}

	INLINE void SetValue(T _value)
	{
		for (uint32 row = 0; row < ROWS; ++row)
		{
			for (uint32 col = 0; col < COLUMNS; ++col)
			{
				m_buffer[row][col] = _value;
			}
		}
	}

	INLINE const uint32 GetRowsCount() const { return ROWS; };
	INLINE const uint32 GetColumnsCount() const { return COLUMNS; };
	INLINE const uint32 GetColumnOffsetCount() const { return static_cast<uint32>(Utils::Ceil(static_cast<float>(COLUMNS / SCALAR_COUNT))); };
	INLINE const uint32 GetTotalOffset() const { return ROWS * COLUMNS; };

	template<uint32 _ROW, uint32 _COL>
	INLINE void Set(const T& value)
	{
		if constexpr (_ROW < ROWS && _COL < COLUMNS)
		{
			m_buffer[_ROW][_COL] = value;
		}
	}

	template<uint32 _ROW, uint32 _COLUMN_OFFSET = 0>
	INLINE void SetRow(const typename Utils::SimdHelper<T>::Type& _value)
	{
		if constexpr (_ROW < ROWS && _COLUMN_OFFSET < Utils::Ceil(static_cast<float>(COLUMNS / SCALAR_COUNT)))
		{
			Utils::SimdHelper<T>::Get(&m_buffer[_ROW][_COLUMN_OFFSET * SCALAR_COUNT], _value);
		}
	}

	INLINE void SetRowAddress(const int32 _row, const uint32 _column_offset, const typename Utils::SimdHelper<T>::Type& _value)
	{
		if (_row < ROWS && _column_offset < Utils::Ceil(static_cast<float>(COLUMNS / SCALAR_COUNT)))
		{
			Utils::SimdHelper<T>::Get(&m_buffer[_row][_column_offset * SCALAR_COUNT], _value);
		}
	}

	template<uint32 _MATRIX_OFFSET>
	INLINE void SetAddress(const typename Utils::SimdHelper<T>::Type& _value)
	{
		if constexpr (_MATRIX_OFFSET < ROWS * COLUMNS)
		{
			ALIGNED T* ptr = &m_buffer[0][0];
			Utils::SimdHelper<T>::Get(&ptr[_MATRIX_OFFSET * SCALAR_COUNT], _value);
		}
	}

	INLINE void SetAddress(const uint32 _matrix_offset, const typename Utils::SimdHelper<T>::Type& _value)
	{
		if (_matrix_offset < ROWS * COLUMNS)
		{
			ALIGNED T* ptr = &m_buffer[0][0];
			Utils::SimdHelper<T>::Get(&ptr[_matrix_offset * SCALAR_COUNT], _value);
		}
	}

	template<uint32 _ROW, uint32 _COL>
	INLINE const T Get() const
	{
		if constexpr (_ROW < ROWS && _COL < COLUMNS)
		{
			return m_buffer[_ROW][_COL];
		}
		else
		{
			return static_cast<T>(0);
		}
	}

	template<uint32 _ROW, uint32 _COLUMN_OFFSET = 0>
	INLINE const typename Utils::SimdHelper<T>::Type GetRow() const
	{
		if constexpr (_ROW < ROWS && _COLUMN_OFFSET < Utils::Ceil(static_cast<float>(COLUMNS / SCALAR_COUNT)))
		{
			return Utils::SimdHelper<T>::Load(&m_buffer[_ROW][_COLUMN_OFFSET * SCALAR_COUNT]);
			
		}
		else
		{
			return Utils::SimdHelper<T>::Zero();
		}
	}

	template<uint32 _ROW, uint32 _COLUMN_OFFSET = 0>
	INLINE T const* GetRowAddress() const
	{
		if constexpr (_ROW < ROWS && _COLUMN_OFFSET < Utils::Ceil(static_cast<float>(COLUMNS / SCALAR_COUNT)))
		{
			return &m_buffer[_ROW][_COLUMN_OFFSET * SCALAR_COUNT];
		}
		else
		{
			return nullptr;
		}
	}

	template<uint32 _ROW>
	INLINE T const* GetRowAddress(const uint32 _column_offset = 0) const
	{
		if constexpr (_ROW >= ROWS)
		{
			return nullptr;
		}

		if (_column_offset < Utils::Ceil(static_cast<float>(COLUMNS / SCALAR_COUNT)))
		{
			return &m_buffer[_ROW][_column_offset * SCALAR_COUNT];
		}
		else
		{
			return nullptr;
		}
	}

	INLINE T const* GetRowAddress(const int32 _row, const uint32 _column_offset = 0) const
	{
		if (_row < ROWS && _column_offset < Utils::Ceil(static_cast<float>(COLUMNS / SCALAR_COUNT)))
		{
			return &m_buffer[_row][_column_offset * SCALAR_COUNT];
		}
		else
		{
			return nullptr;
		}
	}

	INLINE T const* GetAddress() const
	{
		return &m_buffer[0][0];
	}

	// debug purpose, no need to optimize
	void Print() const
	{
		for (uint32 i = 0; i < ROWS; ++i)
		{
			for (uint32 j = 0; j < COLUMNS; ++j)
			{
				std::cout << m_buffer[i][j] << " ";
			}
			std::cout << std::endl;
		}
		std::cout << std::endl;
	}

private:
	ALIGNED T m_buffer[ROWS][COLUMNS];
};




/// <summary>
/// Matrix 1D
/// </summary>
/// <typeparam name="T"></typeparam>
template<typename T, uint32 ROWS>
class Matrix<T, ROWS, 0> : public MathAligned<MEM_ALIGN>
{
	static_assert(ROWS > 0, "Matrix 1D rows must be greater than 0");
	static_assert(std::numeric_limits<T>::is_integer || std::is_same<float, T>::value, "Matrix 1D can be only instantiated with integer or float");

public:
	INLINE Matrix()
	{
		memset(m_buffer, 0, sizeof(m_buffer));
	}

	INLINE Matrix(const Matrix& a)
	{
		Utils::MemCpy(&m_buffer[0], &a.m_buffer[0], sizeof(m_buffer));
	}

	INLINE void SetValue(T _value)
	{
		for (uint32 row = 0; row < ROWS; ++row)
		{
			m_buffer[row]= _value;
		}
	}

	INLINE void SetRandomUniformDistribution()
	{
		int32 seed = static_cast<int32>(time(0));
		std::default_random_engine random_engine(seed);

		std::uniform_real_distribution<T> uniform(-1, 1);

		for (uint32 row = 0; row < ROWS; ++row)
		{
			m_buffer[row] = uniform(random_engine);
		}
	}

	INLINE const uint32 GetRowsCount() const { return ROWS; };
	INLINE const uint32 GetColumnsCount() const { return 0; };
	INLINE const uint32 GetColumnOffsetCount() const { return 0; };
	INLINE const uint32 GetTotalOffset() const { return ROWS; };

	template<uint32 _ROW>
	INLINE void Set(T value)
	{
		if constexpr (_ROW < ROWS)
		{
			m_buffer[_ROW] = value;
		}
	}

	template<uint32 _ROW_OFFSET = 0>
	INLINE void SetRow(const typename Utils::SimdHelper<T>::Type& _value)
	{
		if constexpr (_ROW_OFFSET < Utils::Ceil(static_cast<float>(ROWS / SCALAR_COUNT)))
		{
			Utils::SimdHelper<T>::Get(&m_buffer[_ROW_OFFSET * SCALAR_COUNT], _value);
		}
	}

	INLINE void SetRowAddress(const uint32 _row_offset, const typename Utils::SimdHelper<T>::Type& _value)
	{
		if (_row_offset < Utils::Ceil(static_cast<float>(ROWS / SCALAR_COUNT)))
		{
			Utils::SimdHelper<T>::Get(&m_buffer[_row_offset * SCALAR_COUNT], _value);
		}
	}

	template<uint32 _MATRIX_OFFSET>
	INLINE void SetAddress(const typename Utils::SimdHelper<T>::Type& _value)
	{
		if constexpr (_MATRIX_OFFSET < ROWS)
		{
			Utils::SimdHelper<T>::Get(&m_buffer[_MATRIX_OFFSET * SCALAR_COUNT], _value);
		}
	}

	INLINE void SetAddress(const uint32 _matrix_offset, const typename Utils::SimdHelper<T>::Type& _value)
	{
		if (_matrix_offset < ROWS)
		{
			Utils::SimdHelper<T>::Get(&m_buffer[_matrix_offset * SCALAR_COUNT], _value);
		}
	}

	template<uint32 _ROW>
	INLINE const T Get() const
	{
		if constexpr (_ROW >= ROWS)
		{
			return static_cast<T>(0);
		}

		return m_buffer[_ROW];
	}

	template<uint32 _ROW_OFFSET = 0>
	INLINE const typename Utils::SimdHelper<T>::Type GetRow() const
	{
		if constexpr (_ROW_OFFSET < Utils::Ceil(static_cast<float>(ROWS / SCALAR_COUNT)))
		{
			return Utils::SimdHelper<T>::Load(&m_buffer[_ROW_OFFSET * SCALAR_COUNT]);

		}
		else
		{
			return Utils::SimdHelper<T>::Zero();
		}
	}

	template<uint32 _ROW_OFFSET = 0>
	INLINE T const* GetRowAddress() const
	{
		if constexpr (_ROW_OFFSET < Utils::Ceil(static_cast<float>(ROWS / SCALAR_COUNT)))
		{
			return &m_buffer[_ROW_OFFSET * SCALAR_COUNT];
		}
		else
		{
			return nullptr;
		}
	}

	INLINE T const* GetRowAddress(const uint32 _row_offset = 0) const
	{
		if (_row_offset < Utils::Ceil(static_cast<float>(ROWS / SCALAR_COUNT)))
		{
			return &m_buffer[_row_offset * SCALAR_COUNT];
		}
		else
		{
			return nullptr;
		}
	}

	INLINE T const* GetAddress() const
	{
		return &m_buffer[0];
	}

	// debug purpose, no need to optimize
	void Print()
	{
		for (uint32 i = 0; i < ROWS; ++i)
		{
			std::cout << m_buffer[i] << " ";
		}
		std::cout << std::endl;
	}

private:
	ALIGNED T m_buffer[ROWS];
};


NAMESPACE_END