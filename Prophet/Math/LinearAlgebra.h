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

		const T* addr = _matrix.GetAddress();

		const typename Utils::SimdHelper<T>::Type mulBuffer = Utils::SimdHelper<T>::Load(_value);

		Matrix<T, ROWS, COLUMNS> m(_matrix);

		for (uint32 i = 0, j = 0; i < steps; ++i, j += SCALAR_COUNT)
		{
			const typename Utils::SimdHelper<T>::Type buffer = Utils::SimdHelper<T>::Load(&addr[j]);
			const typename Utils::SimdHelper<T>::Type result = Utils::SimdHelper<T>::Mul(buffer, mulBuffer);

			m.SetAddress(i, result);
		}

		return m;
	}

	template<typename T, uint32 ROWS, uint32 COLUMNS>
	static INLINE Matrix<T, ROWS, COLUMNS> Multiply(const Matrix<T, ROWS, COLUMNS>& _matrixA, const Matrix<T, ROWS, COLUMNS>& _matrixB)
	{
		AssertReturnValue((_matrixA.GetRowsCount() == _matrixB.GetRowsCount()) && (_matrixA.GetColumnsCount() == _matrixB.GetColumnsCount()), _matrixA, "Matrices must share the same number of rows and columns");

		constexpr uint32 left = (ROWS * COLUMNS) % SCALAR_COUNT;
		constexpr uint32 steps = ((ROWS * COLUMNS) / SCALAR_COUNT) + (left != 0);

		const T* addrA = _matrixA.GetAddress();
		const T* addrB = _matrixB.GetAddress();

		Matrix<T, ROWS, COLUMNS> m;

		for (uint32 i = 0, j = 0; i < steps; ++i, j += SCALAR_COUNT)
		{
			const typename Utils::SimdHelper<T>::Type bufferA = Utils::SimdHelper<T>::Load(&addrA[j]);
			const typename Utils::SimdHelper<T>::Type bufferB = Utils::SimdHelper<T>::Load(&addrB[j]);
			const typename Utils::SimdHelper<T>::Type result = Utils::SimdHelper<T>::Mul(bufferA, bufferB);

			m.SetAddress(i, result);
		}

		return m;
	}



	template<typename T, uint32 ROWS, uint32 COLUMNS_SRC, uint32 COLUMNS_DST>
	static INLINE Matrix<T, ROWS, COLUMNS_DST> Multiply(const Matrix<T, ROWS, COLUMNS_SRC>& _matrixA, const Matrix<T, ROWS, COLUMNS_DST>& _matrixB)
	{
		static_assert(COLUMNS_DST <= COLUMNS_SRC, "Destination columns count as well as the second matrix parameter has to be less or equal to the source columns count, the first matrix parameter");
		AssertReturnValue(_matrixA.GetRowsCount() == _matrixB.GetRowsCount(), _matrixB, "Matrices must share the same number of rows in order to be allowed to used in Hadamard product");

		constexpr uint32 left = (ROWS * COLUMNS_DST) % SCALAR_COUNT;
		constexpr uint32 steps = ((ROWS * COLUMNS_DST) / SCALAR_COUNT) + (left != 0);

		const T* addrA = _matrixA.GetAddress();
		const T* addrB = _matrixB.GetAddress();

		Matrix<T, ROWS, COLUMNS_DST> m;
		m.SetRandomUniformDistribution();

		for (uint32 i = 0, j = 0; i < steps; ++i, j += SCALAR_COUNT)
		{
			const typename Utils::SimdHelper<T>::Type bufferA = Utils::SimdHelper<T>::Load(&addrA[j]);
			const typename Utils::SimdHelper<T>::Type bufferB = Utils::SimdHelper<T>::Load(&addrB[j]);
			const typename Utils::SimdHelper<T>::Type result = Utils::SimdHelper<T>::Mul(bufferA, bufferB);

			m.SetAddress(i, result);
		}

		return m;
	}
}

NAMESPACE_END