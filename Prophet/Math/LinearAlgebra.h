#pragma once

#include <random>

#include <time.h>

#include "../Core/Architecture.h"
#include "../Core/BasicDefines.h"
#include "../Core/Assertions.h"
#include "../Core/BasicTypes.h"
#include "../Core/Alignment.h"
#include "../Core/SimdHelper.h"

#include "Matrix.h"
#include "Vector.h"


NAMESPACE_BEGIN

template<typename T, uint32 ROWS, uint32 COLUMNS>
static INLINE Matrix<T, ROWS, COLUMNS> Multiply(const Matrix<T, ROWS, COLUMNS>& _matrix, T _value)
{
	constexpr uint32 left = (ROWS * COLUMNS) % SCALAR_COUNT;
	constexpr uint32 steps = ((ROWS * COLUMNS) / SCALAR_COUNT) + (left != 0);

	const float* fullValuesAddr = _matrix.GetFullAddress();

	Utils::SimdHelper<float>::Type buffer[steps];
	Utils::SimdHelper<float>::Type multiplierBuffer[steps];

	for (uint32 i = 0; i < steps; ++i)
	{
		buffer[i] = Utils::SimdHelper<T>::Load(&fullValuesAddr[i]);
		multiplierBuffer[i] = Utils::SimdHelper<T>::Load(_value);
	}

	/*
	for (uint32 i = 0; i < steps; ++i)
	{
		Utils::SimdHelper<T>::Mul(buffer[i], multiplierBuffer[i]);
	}
	*/
}

NAMESPACE_END