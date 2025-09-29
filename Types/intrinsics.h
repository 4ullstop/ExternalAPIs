#if !defined (INTRINSICS_H)
#include "math.h"
#include "typedefs.h"

inline i32
SignOf(i32 value)
{
    i32 result = (value >= 0) ? 1 : -1;
    return(result);
}

inline r32
SquareRoot(r32 real)
{
    r32 result = sqrtf(real);
    return(result);
}

inline r32
AbsoluteValue(r32 real)
{
    r32 result = (r32)fabs(real);
    return(result);
}

inline i32
RounR32ToI32(r32 real)
{
    i32 result = (i32)roundf(real);
    return(result);
}

inline u32
RoundR32ToU32(r32 real)
{
    u32 result = (u32)roundf(real);
    return(result);
}

inline i32
FloorR32ToI32(r32 real)
{
    i32 result = (i32)floorf(real);
    return(result);
}

inline i32
CeilR32ToI32(r32 real)
{
    i32 result = (i32)ceilf(real);
    return(result);
}

inline i32
TruncateR32ToI32(r32 real)
{
    i32 result = (i32)real;
    return(result);
}

inline r32
Sin(r32 angle)
{
    r32 result = sinf(angle);
    return(result);
}

inline r32
Cos(r32 angle)
{
    r32 result = cosf(angle);
    return(result);
}

inline r32
ATan2(r32 y, r32 x)
{
    r32 result = atan2f(y, x);
    return(result);
}

inline r32
RoundFloat(r32 value, i32 decimalPlaces)
{
    r32 result = 0;

    r32 shift = (r32)pow(10, decimalPlaces);

    result = value * shift;
    result = (r32)round(result);
    result /= shift;
    return(result);
}
#define INTRINSICS_H
#endif

