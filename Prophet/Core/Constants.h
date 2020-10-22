#pragma once


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


NAMESPACE_END

#pragma warning(pop)