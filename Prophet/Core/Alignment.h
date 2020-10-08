#pragma once


#include "Constants.h"


NAMESPACE_BEGIN

template<int32 Align> class MathAligned;
template<> class ALIGNED MathAligned<MEM_ALIGN> { };

template <int32 Align, typename T> class MathBase : public MathAligned<Align> { };

NAMESPACE_END