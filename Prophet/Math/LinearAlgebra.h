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

namespace LinearAlgebra
{
	template<typename T, uint32 ROWS, uint32 COLUMNS>
	static INLINE Matrix<T, ROWS, COLUMNS> Multiply(const Matrix<T, ROWS, COLUMNS>& _matrix, T _value)
	{
		constexpr uint32 left = (ROWS * COLUMNS) % SCALAR_COUNT;
		constexpr uint32 steps = ((ROWS * COLUMNS) / SCALAR_COUNT) + (left != 0);

		const T* fullValuesAddr = _matrix.GetAddress();

		const typename Utils::SimdHelper<T>::Type mulBuffer = Utils::SimdHelper<T>::Load(_value);

		Matrix<T, ROWS, COLUMNS> m(_matrix);

		for (uint32 i = 0, j = 0; i < steps; ++i, j += SCALAR_COUNT)
		{
			const typename Utils::SimdHelper<T>::Type buffer = Utils::SimdHelper<T>::Load(&fullValuesAddr[j]);
			const typename Utils::SimdHelper<T>::Type result = Utils::SimdHelper<T>::Mul(buffer, mulBuffer);

			m.SetAddress(i, result);
		}

		return m;
	}
}

NAMESPACE_END