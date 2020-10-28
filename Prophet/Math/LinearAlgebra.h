#pragma once

#include <random>

#include <time.h>

#include "../Core/Architecture.h"
#include "../Core/BasicDefines.h"
#include "../Core/Assertions.h"
#include "../Core/BasicTypes.h"
#include "../Core/Alignment.h"
#include "../Core/SimdHelper.h"

#include "../Tensor/Matrix.h"
#include "../Tensor/Vector.h"


NAMESPACE_BEGIN

namespace LinearAlgebra
{
	template<typename T, uint32 ROWS, uint32 COLUMNS>
	static INLINE Matrix<T, ROWS, COLUMNS> Multiply(const Matrix<T, ROWS, COLUMNS>& _matrix, T _value)
	{
		constexpr uint32 left = (ROWS * COLUMNS) % SCALAR_COUNT;
		constexpr uint32 steps = ((ROWS * COLUMNS) / SCALAR_COUNT) + (left != 0);

		const T* addr = _matrix;

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
		AssertReturnValue((_matrixA.Rows() == _matrixB.Rows()) && (_matrixA.Columns() == _matrixB.Columns()), _matrixA, "Matrices must share the same number of rows and columns");

		constexpr uint32 left = (ROWS * COLUMNS) % SCALAR_COUNT;
		constexpr uint32 steps = ((ROWS * COLUMNS) / SCALAR_COUNT) + (left != 0);

		const T* addrA = _matrixA;
		const T* addrB = _matrixB;

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
		AssertReturnValue(_matrixA.Rows() == _matrixB.Rows(), _matrixB, "Matrices must share the same number of rows in order to be allowed to used in Hadamard product");

		constexpr uint32 left = (ROWS * COLUMNS_DST) % SCALAR_COUNT;
		constexpr uint32 steps = ((ROWS * COLUMNS_DST) / SCALAR_COUNT) + (left != 0);

		const T* addrA = _matrixA;
		const T* addrB = _matrixB;

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

	template<typename T, uint32 COUNT>
	static INLINE Vector<T, COUNT> Multiply(const Vector<T, COUNT>& _vector, T _value)
	{
		constexpr uint32 left = COUNT % SCALAR_COUNT;
		constexpr uint32 steps = (COUNT / SCALAR_COUNT) + (left != 0);

		const T* addr = _vector;

		const typename Utils::SimdHelper<T>::Type mulBuffer = Utils::SimdHelper<T>::Load(_value);

		Vector<T, COUNT> v(_vector);

		for (uint32 i = 0, j = 0; i < steps; ++i, j += SCALAR_COUNT)
		{
			const typename Utils::SimdHelper<T>::Type buffer = Utils::SimdHelper<T>::Load(&addr[j]);
			const typename Utils::SimdHelper<T>::Type result = Utils::SimdHelper<T>::Mul(buffer, mulBuffer);

			v.SetAddress(i, result);
		}

		return v;
	}

	template<typename T, uint32 COUNT>
	static INLINE Vector<T, COUNT> Multiply(const Vector<T, COUNT>& _vectorA, const Vector<T, COUNT>& _vectorB)
	{
		constexpr uint32 left = COUNT % SCALAR_COUNT;
		constexpr uint32 steps = (COUNT / SCALAR_COUNT) + (left != 0);

		const T* addrA = _vectorA;
		const T* addrB = _vectorB;

		Vector<T, COUNT> v;

		for (uint32 i = 0, j = 0; i < steps; ++i, j += SCALAR_COUNT)
		{
			const typename Utils::SimdHelper<T>::Type bufferA = Utils::SimdHelper<T>::Load(&addrA[j]);
			const typename Utils::SimdHelper<T>::Type bufferB = Utils::SimdHelper<T>::Load(&addrB[j]);
			const typename Utils::SimdHelper<T>::Type result = Utils::SimdHelper<T>::Mul(bufferA, bufferB);

			v.SetAddress(i, result);
		}

		return v;
	}
}

NAMESPACE_END