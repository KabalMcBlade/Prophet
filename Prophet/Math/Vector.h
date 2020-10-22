#pragma once

#include <random>

#include <time.h>

#include "../Core/Architecture.h"
#include "../Core/BasicDefines.h"
#include "../Core/Assertions.h"
#include "../Core/BasicTypes.h"
#include "../Core/Alignment.h"

#include "Matrix.h"


NAMESPACE_BEGIN

template<typename T, uint32 ROWS>
class Vector : public Matrix<T, ROWS, 0>
{
};


NAMESPACE_END