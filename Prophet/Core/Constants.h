#pragma once


#include "Conversion.h"


#pragma warning(push)
#pragma warning(disable: 4838 4305)

NAMESPACE_BEGIN

#define PI 3.141592653589793238f
#define HALF_PI 1.5707963267948966192f

#define VALUE_DEGTORAD  0.017453292519943295769236907684886f    // Degrees to Radians (PI/180)
#define VALUE_RADTODEG  57.295779513082320876798154814105f      // Radians to Degrees (180/PI)
#define DEG_TO_RAD(x)   (x * VALUE_DEGTORAD)
#define RAD_TO_DEG(x)   (x * VALUE_RADTODEG)

#define SMALL_NUMBER	(1.e-8f)
#define BIG_NUMBER	(1.e+8f)


#define MINUS_INFINITE	(-std::numeric_limits<float>::infinity())
#define PLUS_INFINITE	(std::numeric_limits<float>::infinity())
#define EPSILON			(std::numeric_limits<float>::epsilon())


#define ZERO 0x00000000
#define SIGN_BIT 0x80000000
#define SIGN_BIT_NEG 0x7FFFFFF



// numerical
MATH_CONST Float kZero = { { { 0.0f, 0.0f, 0.0f, 0.0f } } };
MATH_CONST Float kOneVec4 = { { { 1.0f, 1.0f, 1.0f, 1.0f } } };
MATH_CONST Float kOneVec3 = { { { 1.0f, 1.0f, 1.0f, 0.0f } } };
MATH_CONST Float kMinusOneVec4 = { { { -1.0f, -1.0f, -1.0f, -1.0f } } };
MATH_CONST Float kMinusOneVec3 = { { { -1.0f, -1.0f, -1.0f, 0.0f } } };
MATH_CONST Float kOneHalfVec4 = { { { 0.5f, 0.5f, 0.5f, 0.5f } } };
MATH_CONST Float kOneHalfVec3 = { { { 0.5f, 0.5f, 0.5f, 0.0f } } };
MATH_CONST Float kIdentity0 = { { { 1.0f, 0.0f, 0.0f, 0.0f } } };
MATH_CONST Float kIdentity1 = { { { 0.0f, 1.0f, 0.0f, 0.0f } } };
MATH_CONST Float kIdentity2 = { { { 0.0f, 0.0f, 1.0f, 0.0f } } };
MATH_CONST Float kIdentity3 = { { { 0.0f, 0.0f, 0.0f, 1.0f } } };
MATH_CONST Float k255Vec4 = { { { 255.0f, 255.0f, 255.0f, 255.0f } } };
MATH_CONST Float k255Vec3 = { { { 255.0f, 255.0f, 255.0f, 0.0f } } };
MATH_CONST Float k255Over2Vec4 = { { { 127.5f, 127.5f, 127.5f, 127.5f } } };
MATH_CONST Float k255Over2Vec3 = { { { 127.5f, 127.5f, 127.5f, 0.0f } } };


MATH_CONST Float kMinusInf = { MINUS_INFINITE, MINUS_INFINITE, MINUS_INFINITE, MINUS_INFINITE };
MATH_CONST Float kPlusInf = { PLUS_INFINITE, PLUS_INFINITE, PLUS_INFINITE, PLUS_INFINITE };
MATH_CONST Float kSignBit = { SIGN_BIT, SIGN_BIT, SIGN_BIT, SIGN_BIT };
MATH_CONST Float kNotAll = { SIGN_BIT_NEG, SIGN_BIT_NEG, SIGN_BIT_NEG, SIGN_BIT_NEG };
MATH_CONST Float kSmallNumber = { SMALL_NUMBER, SMALL_NUMBER, SMALL_NUMBER, SMALL_NUMBER };
MATH_CONST Float kBigNumber = { BIG_NUMBER, BIG_NUMBER, BIG_NUMBER, BIG_NUMBER };
MATH_CONST Float kMaskSignZeroSignZero = { SIGN_BIT, ZERO, SIGN_BIT, ZERO };


// selection
MATH_CONST UInt kSelectX = { { { 0xFFFFFFFF, 0, 0, 0 } } };
MATH_CONST UInt kSelectY = { { { 0, 0xFFFFFFFF, 0, 0 } } };
MATH_CONST UInt kSelectZ = { { { 0, 0, 0xFFFFFFFF, 0 } } };
MATH_CONST UInt kSelectW = { { { 0, 0, 0, 0xFFFFFFFF } } };


NAMESPACE_END

#pragma warning(pop)