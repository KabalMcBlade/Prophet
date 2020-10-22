#pragma once

#include <random>
#include <functional>

#include <time.h>

#include "../Core/Architecture.h"
#include "../Core/BasicDefines.h"
#include "../Core/Assertions.h"
#include "../Core/BasicTypes.h"
#include "../Core/Alignment.h"


NAMESPACE_BEGIN

template<typename T, uint32 ROWS, uint32 COLUMNS>
class Matrix : public MathAligned<MEM_ALIGN>
{
	static_assert(ROWS > 0 && COLUMNS > 0, "Matrix 2D rows and column must be greater than 0");
	static_assert(std::numeric_limits<T>::is_integer || std::is_same<float, T>::value, "Matrix 2D can be only instantiated with integer or float");

private:
	template<int32 _BEGIN, int32 _END, uint32 _ROW, int32 _STEP = 1>
	struct Unroller
	{
		template<typename Lambda>
		INLINE static void Next(Lambda& func)
		{
			func(_ROW, _BEGIN);
			Unroller<_BEGIN + _STEP, _END, _STEP>::Next(func);
		}
	};

	template<int32 _END, uint32 _ROW, int32 _STEP>
	struct Unroller<_END, _END, _ROW, _STEP>
	{
		template<typename Lambda>
		static void Next(Lambda& func) {}
	};

public:
	INLINE Matrix()
	{
		memset(m_buffer, 0, sizeof(m_buffer));
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

	INLINE constexpr uint32 GetRowsCount() const { return ROWS; };
	INLINE constexpr uint32 GetColumnsCount() const { return COLUMNS; };
	INLINE constexpr uint32 GetTotalOffset() const { return (COLUMNS % SCALAR_COUNT) + 1; };

	template<uint32 _ROW, uint32 _COL>
	INLINE void Set(T value)
	{
		if constexpr (_ROW < ROWS || _COL < COLUMNS)
		{
			m_buffer[_ROW][_COL] = value;
		}
	}

	template<uint32 _ROW, uint32 _OFFSET = 0>
	INLINE void SetRow(const typename Utils::SimdHelper<T>::Type _value) const
	{
		if constexpr (_ROW >= ROWS || _OFFSET >= (COLUMNS % SCALAR_COUNT) + 1)
		{
			return;
		}

		//return Utils::SimdHelper<T>::Load(&m_buffer[_ROW][_OFFSET * SCALAR_COUNT]);
	}

	template<uint32 _ROW, uint32 _COL>
	INLINE const T GetScalar() const
	{
		if constexpr (_ROW >= ROWS || _COL >= COLUMNS)
		{
			return static_cast<T>(0);
		}

		return m_buffer[_ROW][_COL];
	}

	template<uint32 _ROW, uint32 _OFFSET = 0>
	INLINE const typename Utils::SimdHelper<T>::Type GetRow() const
	{
		if constexpr (_ROW >= ROWS || _OFFSET >= (COLUMNS % SCALAR_COUNT) + 1)
		{
			return Utils::SimdHelper<T>::Zero();
		}

		return Utils::SimdHelper<T>::Load(&m_buffer[_ROW][_OFFSET * SCALAR_COUNT]);
	}

	template<uint32 _ROW, uint32 _OFFSET = 0>
	INLINE T const* GetRowAddress() const
	{
		if constexpr (_ROW >= ROWS || _OFFSET >= (COLUMNS % SCALAR_COUNT) + 1)
		{
			return nullptr;
		}

		return &m_buffer[_ROW][_OFFSET * SCALAR_COUNT];
	}

	// slightly slower, but useful if the user does not know the full offset count, used in conjunction with IterateRow
	template<uint32 _ROW>
	INLINE T const* GetRowAddress(const uint32 _offset) const
	{
		if constexpr (_ROW >= ROWS)
		{
			return nullptr;
		}

		if (_offset >= (COLUMNS % SCALAR_COUNT) + 1)
		{
			return nullptr;
		}

		return &m_buffer[_ROW][_offset * SCALAR_COUNT];
	}

	// slightly slower, but useful if the user does not know the full offset count, used in conjunction with IterateRow
	INLINE T const* GetRowAddress(const int32 _row, const uint32 _offset) const
	{
		if (_row >= ROWS || _offset >= (COLUMNS % SCALAR_COUNT) + 1)
		{
			return nullptr;
		}

		return &m_buffer[_row][_offset * SCALAR_COUNT];
	}

	template<uint32 _ROW>
	INLINE T const* GetFullRowAddress() const
	{
		if constexpr (_ROW >= ROWS)
		{
			return nullptr;
		}

		return &m_buffer[_ROW][0];
	}

	INLINE T const* GetFullAddress() const
	{
		return &m_buffer[0][0];
	}

	template<uint32 _ROW>
	INLINE void IterateRow(std::function<void(const int32, const int32)> _lambda) const
	{
		if constexpr (_ROW >= ROWS)
		{
			return;
		}

		Unroller<0, (_ROW + 1) * ((COLUMNS % SCALAR_COUNT) + 1), _ROW>::Next(_lambda);
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

	INLINE constexpr uint32 GetRowsCount() const { return ROWS; };
	INLINE constexpr uint32 GetColumnsCount() const { return 0; };

	template<uint32 _ROW>
	INLINE void Set(T value)
	{
		if constexpr (_ROW < ROWS)
		{
			m_buffer[_ROW] = value;
		}
	}

	template<uint32 _ROW>
	INLINE const T GetScalar() const
	{
		if constexpr (_ROW >= ROWS)
		{
			return static_cast<T>(0);
		}

		return m_buffer[_ROW];
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