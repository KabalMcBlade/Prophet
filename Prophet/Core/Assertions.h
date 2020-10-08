#pragma once

#include <cstdio>
#include <assert.h>     // for assert


#include "BasicDefines.h"


#ifdef _DEBUG

#define Assert( condition, format, ... ) \
    if( !(condition) ) { \
        fprintf (stderr, "%s(%u): " format "\n", __FILE__, __LINE__, __VA_ARGS__); \
    }
#define AssertVoid( condition, format, ... ) \
    if( !(condition) ) { \
        fprintf (stderr, "%s(%u): " format "\n", __FILE__, __LINE__, __VA_ARGS__); \
		return; \
    }
#define AssertValue( condition, return_value, format, ... ) \
    if( !(condition) ) { \
        fprintf (stderr, "%s(%u): " format "\n", __FILE__, __LINE__, __VA_ARGS__); \
		return return_value; \
    }

#else

#define Assert( condition, format, ... )
#define AssertVoid( condition, format, ... )
#define AssertValue( condition, return_value, format, ... )

#endif 


#define AssertReturnVoid( condition, format, ... )					AssertVoid( condition, format, __VA_ARGS__ )
#define AssertReturnValue( condition, return_value, format, ...  )	AssertValue( condition, return_value, format, __VA_ARGS__ )