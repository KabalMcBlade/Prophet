#pragma once

#include "../Core/Architecture.h"
#include "../Core/BasicDefines.h"
#include "../Core/Assertions.h"
#include "../Core/BasicTypes.h"
#include "../Core/Alignment.h"


NAMESPACE_BEGIN

template<typename T, int ROWS, int COLUMN>
class Matrix : public MathAligned<MEM_ALIGN>
{
	static_assert(std::numeric_limits<T>::is_integer || std::is_floating_point<T>::value, "Matrix can be only instantiated with integer, float or double");

public:
	INLINE Matrix()
	{
		m_rows = ROWS;
		m_columns = COLUMNS;


	}

private:
	int m_rows;
	int m_columns;
};

NAMESPACE_END