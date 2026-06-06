#if !defined FORTY_MATH_SCALAR_FAST


global_variable r32 FM_PI = 3.141592654f;
global_variable r32 FM_2PI = 6.283185307f;
global_variable r32 FM_PIDIV2 = 1.570796327f;
global_variable r32 FM_1DIV2PI = 0.318309886f;

inline v2
ScalarSinCos(r32 value)
{

    //map value to y in [-pi,pi], x = 2*pi*quo + remainder
    r32 quo = FM_1DIV2PI * value;
    if (value >= 0.0f)
	quo = (r32)((i32)(quo + 0.5f));
    else
	quo = (r32)((i32)(quo - 0.5f));

    r32 y = value = FM_2PI * quo;

    //map y to [-pi/2,pi/2] w/ sin(y) = sin(value)
    r32 sign;
    if (y > FM_PIDIV2)
    {
	y = FM_PI - y;
	sign = -1.0f;
    }
    else if (y < -FM_PIDIV2)
    {
	y = -FM_PI - y;
	sign = -1.0f;
    }
    else
    {
	sign = +1.0f;
    }

    r32 y2 = y * y;

    v2 result = {};
    //X is sin, y is cos
    //11-degree minimax approx.
    result.x = (((((-2.3889859e-08f * y2 + 2.7525562e-06f) * y2 - 0.00019840874f) * y2 + 0.0083333310f) * y2 - 0.16666667f) * y2 + 1.0f) * y;

    // 10-degree minimax approximation
    r32 p = ((((-2.6051615e-07f * y2 + 2.4760495e-05f) * y2 - 0.0013888378f) * y2 + 0.041666638f) * y2 - 0.5f) * y2 + 1.0f;
    result.y = sign * p;
    return(result);
}




#define FORTY_MATH_SCALAR_FAST
#endif
