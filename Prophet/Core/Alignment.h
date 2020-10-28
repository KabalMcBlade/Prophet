#pragma once


#include "BasicDefines.h"


NAMESPACE_BEGIN

template<int32 Align> class MemoryAligned;
template<> class ALIGNED MemoryAligned<MEM_ALIGN> { };

template <int32 Align, typename T> class MemoryBase : public MemoryAligned<Align> { };

NAMESPACE_END