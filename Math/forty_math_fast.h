#if !defined (FORTY_MATH_FAST_H)
#include <xmmintrin.h>
#include <emmintrin.h>
#include <tmmintrin.h>
#include "../Types/typedefs.h"
#include "forty_math.h"
/*
  MAJOR NOTE: USE OF THIS FILE REQUIRES THE "USE_FORTY_MATH_FAST" COMPILER FLAG
  Code will not compile without it and will cause redefinition issues of v4 type  
 */


#define Min(a, b) ((a < b) ? (a) : (b))
#define Max(a, b) ((a > b) ? (a) : (b))

#if 0
struct v2
{
    union
    {
	struct
	{
	    r32 x, y;
	};
	r32 e[2];

    };
};
#endif

#include "forty_math_scalar_fast.h"
//Using __vectorcall to pass the parameters as SIMD registers directly
//To ensure that this would work on other platforms, you will need to define
//it according to the platform specific version of __vectorcall

typedef __m128 m128;

#if defined(_MSC_VER)
#define FCALL __vectorcall
#elif defined(__clang__) || defined(__GNUC__)
#define FCALL
#else
#define FCALL
#endif

#define NO_INTRINSICS 0
//#define ARM 0
#define SSE 1

//Determine our instruction set level
#if SSE

#if defined(__SSE4_2) || (defined(_M_IX86FP) && _M_IX86_FP >= 2 && defined(__AVX__))
#define SSE_4 1
#endif

#if defined(__SSE4_1__)
#define SSE_4 1
#endif

#if defined(__SSSE3__)
#define SSE_3 1
#endif

#if defined(__SSE3__) || (defined(_M_IX86_FP) && _M_IX86_FP >= 2 && defined(__AVX__))
#define SSE_3 1
#endif

#if defined(__SSE2__) || defined(_M_AMD64) || defined(_M_X64) || (defined(_M_IX86_FP) && _MIX86_FP >= 2)
#define SSE_2 1
#endif

#if defined(__SSE__) || (defined(_M_IX86_FP) && _M_IX86_FP >= 1)
#define SSE_1 1
#endif

#endif

#define FM_PERMUTE_PS(v, c) _mm_shuffle_ps((v), (v), c)

#define FM_FMADD_PS(a, b, c) _mm_add_ps(_mm_mul_ps((a), (b)), (c))
#define FM_FNMADD_PS(a, b, c) _mm_sub_ps((c), _mm_mul_ps((a), (b)))





/*
************************V4*************************************
 */

struct alignas(16) v4I
{
    union
    {
	struct
	{
	    i32 x, y, z, w;
	};
	i32 e[4];
	__m128 smv;
	__m128i iSmv;
    };
};

inline bool32
operator==(v4I a, v4I b)
{
    bool32 result =
    ((a.e[0] == b.e[0]) &&
     (a.e[1] == b.e[1]) &&
     (a.e[2] == b.e[2]) &&
     (a.e[3] == b.e[3]));
    return(result);
}

struct alignas(16) v4u32
{
    union
    {
	struct
	{
	    u32 x, y, z, w;
	};
	u32 e[4];
	__m128 smv;
	__m128i iSmv;
    };
};


//infinity
global_variable v4I FM_INF  = v4I{0x7F800000, 0x7F800000, 0x7F800000, 0x7F800000};
global_variable v4I FM_QNaN = v4I{0x7FC00000, 0x7FC00000, 0x7FC00000, 0x7FC00000};

struct alignas(16) v4
{
    union
    {
	struct
	{
	    r32 x, y, z, w;
	};
	r32 e[4];
	__m128 smv;
	__m128i iSmv;
    };
    inline v4 &operator*=(v4 a);
    inline v4 &operator+=(v4 a);
    inline v4 &operator-=(v4 a);    
};

global_variable v4 FM_IDENTITY_R0 = v4{1.0f, 0.0f, 0.0f, 0.0f};
global_variable v4 FM_IDENTITY_R1 = v4{0.0f, 1.0f, 0.0f, 0.0f};
global_variable v4 FM_IDENTITY_R2 = v4{0.0f, 0.0f, 1.0f, 0.0f};
global_variable v4 FM_IDENTITY_R3 = v4{0.0f, 0.0f, 0.0f, 1.0f};

global_variable v4 FM_NEG_IDENTITY_R0 = v4{-1.0f, 0.0f, 0.0f, 0.0f};
global_variable v4 FM_NEG_IDENTITY_R1 = v4{0.0f, -1.0f, 0.0f, 0.0f};
global_variable v4 FM_NEG_IDENTITY_R2 = v4{0.0f, 0.0f, -1.0f, 0.0f};
global_variable v4 FM_NEG_IDENTITY_R3 = v4{0.0f, 0.0f, 0.0f, -1.0f};

global_variable u32 FM_SELECT_0 = 0x00000000;
global_variable u32 FM_SELECT_1 = 0xFFFFFFFF;

global_variable v4u32 FM_SELECT0101 = {FM_SELECT_0, FM_SELECT_1, FM_SELECT_0, FM_SELECT_1};
global_variable v4u32 FM_SELECT1010 = {FM_SELECT_1, FM_SELECT_0, FM_SELECT_1, FM_SELECT_0};
global_variable v4u32 FM_SELECT1000 = {FM_SELECT_1, FM_SELECT_0, FM_SELECT_0, FM_SELECT_0};
global_variable v4u32 FM_SELECT1100 = {FM_SELECT_1, FM_SELECT_1, FM_SELECT_0, FM_SELECT_0};
global_variable v4u32 FM_SELECT1110 = {FM_SELECT_1, FM_SELECT_1, FM_SELECT_1, FM_SELECT_0};
global_variable v4u32 FM_SELECT1011 = {FM_SELECT_1, FM_SELECT_0, FM_SELECT_1, FM_SELECT_1};

global_variable v4u32 FM_MASK_X = {0xFFFFFFFF, 0x00000000, 0x00000000, 0x00000000};
global_variable v4u32 FM_MASK_Y = {0x00000000, 0xFFFFFFFF, 0x00000000, 0x00000000};
global_variable v4u32 FM_MASK_Z = {0x00000000, 0x00000000, 0xFFFFFFFF, 0x00000000};
global_variable v4u32 FM_MASK_W = {0x00000000, 0x00000000, 0x00000000, 0xFFFFFFFF};

global_variable v4u32 FM_MASK_XY = {0xFFFFFFFF, 0xFFFFFFFF, 0x00000000, 0x00000000};

global_variable v4 FM_NEGATE_Y = {-1.0f, 1.0f, 1.0f, 1.0f};
global_variable v4 FM_NEGATE_X = {1.0f, -1.0f, 1.0f, 1.0f};
global_variable v4 FM_NEGATE_Z = {1.0f, 1.0f, -1.0f, 1.0f};
global_variable v4 FM_NEGATE_W = {1.0f, 1.0f, 1.0f, -1.0f};

global_variable v4u32 FM_MASK_3 = {0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x00000000};

global_variable v4I FM_ABS_MASK = {0x7FFFFFFF, 0x7FFFFFFF, 0x7FFFFFFF, 0x7FFFFFFF};

global_variable v4 FM_ZERO = {0.0f, 0.0f, 0.0f, 0.0f};
global_variable v4 FM_ONE = {1.0f, 1.0f, 1.0f, 1.0f};

global_variable v4 FM_RECIPROCAL_TWO_PI = {FM_1DIV2PI, FM_1DIV2PI, FM_1DIV2PI, FM_1DIV2PI};
global_variable v4 FM_NO_FRACTION = {8388608.0f, 8388608.0f, 8388608.0f, 8388608.0f};
global_variable v4u32 FM_NEGATIVE_ZERO = {0x80000000, 0x80000000, 0x80000000, 0x80000000};
global_variable v4 FM_NEGATIVE_ONE = {-1.0f, -1.0f, -1.0f, -1.0f};

global_variable v4 FM_PI_V = {FM_PI, FM_PI, FM_PI, FM_PI};
global_variable v4 FM_TWO_PI = {FM_2PI, FM_2PI, FM_2PI, FM_2PI};
global_variable v4 FM_HALFPI = {FM_PIDIV2, FM_PIDIV2, FM_PIDIV2, FM_PIDIV2};

global_variable v4 FM_ONEHALF = {0.5f, 0.5f, 0.5f, 0.5f};

global_variable v4 FM_SIN_COEFFICIENTS_0 = {-0.16666667f, +0.0083333310f, -0.00019840874f, +2.7525562e-06f};
global_variable v4 FM_SIN_COEFFICIENTS_1 ={-2.3889859e-08f, -0.16665852f /*Est1*/, +0.0083139502f /*Est2*/, -0.00018524670f /*Est3*/};

global_variable v4 FM_COS_COEFFICIENTS_0 = {-0.5f, +0.041666638f, -0.0013888378f, +2.4760495e-05f};
global_variable v4 FM_COS_COEFFICIENTS_1 = {-2.6051615e-07f, -0.49992746f /*Est1*/, +0.041493919f /*Est2*/, -0.0012712436f /*Est3*/ };

global_variable v4 FM_TAN_COEFFICIENTS_0 = {1.0f, 0.333333333f, 0.133333333f, 5.396825397e-2f};
global_variable v4 FM_TAN_COEFFICIENTS_1  = {2.186948854e-2f, 8.863235530e-3f, 3.592128167e-3f, 1.455834485e-3f};

global_variable v4 FM_ATAN_COEFFICIENTS_0 = {-0.3333314528f, +0.1999355085f, -0.1420889944f, +0.1065626393f};
global_variable v4 FM_ATAN_COEFFICIENTS_1 = { { { -0.0752896400f, +0.0429096138f, -0.0161657367f, +0.0028662257f } } };

global_variable v4 FM_EXP_EST_1 = {-6.93147182e-1f, -6.93147182e-1f, -6.93147182e-1f, -6.93147182e-1f};
global_variable v4 FM_EXP_EST_2 = {+2.40226462e-1f, +2.40226462e-1f, +2.40226462e-1f, +2.40226462e-1f};
global_variable v4 FM_EXP_EST_3 = {-5.55036440e-2f, -5.55036440e-2f, -5.55036440e-2f, -5.55036440e-2f};
global_variable v4 FM_EXP_EST_4 = {+9.61597636e-3f, +9.61597636e-3f, +9.61597636e-3f, +9.61597636e-3f};
global_variable v4 FM_EXP_EST_5 = {-1.32823968e-3f, -1.32823968e-3f, -1.32823968e-3f, -1.32823968e-3f};
global_variable v4 FM_EXP_EST_6 = {+1.47491097e-4f, +1.47491097e-4f, +1.47491097e-4f, +1.47491097e-4f};
global_variable v4 FM_EXP_EST_7 = {-1.08635004e-5f, -1.08635004e-5f, -1.08635004e-5f, -1.08635004e-5f};

global_variable v4I FM_EXPONENT_BIAS = {127, 127, 127, 127};

global_variable v4I FM_253 = {253, 253, 253, 253};

global_variable v4I FM_MIN_NORMAL = {0x00800000, 0x00800000, 0x00800000, 0x00800000};
global_variable v4I FM_BIN_128 = {0x43000000, 0x43000000, 0x43000000, 0x43000000};

global_variable v4I FM_SUBNORMAL_EXPONENT = {-126, -126, -126, -126};
global_variable v4u32 FM_BINNEG_150 = {0xC3160000, 0xC3160000, 0xC3160000, 0xC3160000};

global_variable v4I FM_QNAN_TEST = {0x007FFFFF, 0x007FFFFF, 0x007FFFFF, 0x007FFFFF};

global_variable u32 FM_PERMUTE_0X = 0;
global_variable u32 FM_PERMUTE_0Y = 1;
global_variable u32 FM_PERMUTE_0Z = 2;
global_variable u32 FM_PERMUTE_0W = 3;
global_variable u32 FM_PERMUTE_1X = 4;
global_variable u32 FM_PERMUTE_1Y = 5;
global_variable u32 FM_PERMUTE_1Z = 6;
global_variable u32 FM_PERMUTE_1W = 7;


#define FMISINF(x) isinf(x)

r32 ToRad(r32 deg) { return deg * (FM_PI / 180.0f); }

u32 GetByteIndices(u32 index)
{
    if (index > 3) return 0x80808080;

    u32 baseByte = (index & 3) * 4;
    return(baseByte | ((baseByte + 1) << 8) | ((baseByte + 2) << 16) | ((baseByte + 3) << 24));
}

//Wow this function is crazy
#if 1
inline v4 FCALL
VectorPermute(v4 a, v4 b, u32 permuteX, u32 permuteY, u32 permuteZ, u32 permuteW)
{
    __m128i maskA = _mm_setr_epi32(
	GetByteIndices(permuteX),
	GetByteIndices(permuteY),
	GetByteIndices(permuteZ),
	GetByteIndices(permuteW)
	);

    __m128i maskB = _mm_setr_epi32(
	GetByteIndices(permuteX - 4),
	GetByteIndices(permuteY - 4),
	GetByteIndices(permuteZ - 4),
	GetByteIndices(permuteW - 4)
	);


    m128 shuffled1 = _mm_castsi128_ps(_mm_shuffle_epi8(_mm_castps_si128(a.smv), maskA));
    m128 shuffled2 = _mm_castsi128_ps(_mm_shuffle_epi8(_mm_castps_si128(b.smv), maskB));
    v4 result = {};
    result.smv = _mm_or_ps(shuffled1, shuffled2);
    return(result);
}
#else
inline v4u32 FCALL
VectorPermute(v4 a, v4 b, u32 permuteX, u32 permuteY, u32 permuteZ, u32 permuteW)
{
    u32 ta = 0;
    u32 tb = 0;
    
    u32* aPtr[2];
    aPtr[0] = &ta;
    aPtr[1] = &tb;
    
    v4u32 result;
    u32 i0 = permuteX & 3;
    u32 vi0 = permuteX >> 2;
    result.e[0] = aPtr[vi0][i0];

    u32 i1 = permuteY & 3;
    u32 vi1 = permuteY >> 2;
    result.e[1] = aPtr[vi1][i1];

    u32 i2 = permuteZ & 3;
    u32 vi2 = permuteZ >> 2;
    result.e[2] = aPtr[vi2][i2];

    u32 i3 = permuteW & 3;
    u32 vi3 = permuteW >> 2;
    result.e[3] = aPtr[vi3][i3];

    return(result);
}
#endif


inline v4 FCALL
SetV4(r32 x, r32 y, r32 z, r32 w)
{
#if NO_INTRINSICS
    v4 result = v4{x, y, z, w};
    return(result);
#elif defined(ARM)

#elif defined(SSE)
    v4 result;
    result.smv = _mm_set_ps(w, z, y, x);
    return(result);
#endif    
}

inline v4 FCALL
operator+(v4 a, v4 b)
{
#if NO_INTRINSICS
    v4 result =
    {
	(a.x + b.x),
	(a.y + b.y),
	(a.z + b.z),
	(a.w + b.w)
    };
    return(result);
#elif defined(ARM)
    
#elif defined(SSE)
    v4 result = {};
    result.smv = _mm_add_ps(a.smv, b.smv); //simple add function 
    return(result);
#endif    
}

inline v4 FCALL
operator-(v4 a, v4 b)
{
#if NO_INTRINSICS
    v4 result =
    {
	(a.x - b.x),
	(a.y - b.y),
	(a.z - b.z),
	(a.w - b.w)
    };
    return(result);
#elif defined(ARM)
    
#elif defined(SSE)
    v4 result = {};
    result.smv = _mm_sub_ps(a.smv, b.smv); //simple subtract function 
    return(result);
#endif        
}

inline v4 FCALL
operator/(v4 a, v4 b)
{
#if NO_INTRINSICS
    v4 result =
    {
	(a.x / b.x),
	(a.y / b.y),
	(a.z / b.z),
	(a.w / b.w)
    };
    return(result);
#elif defined(ARM)
    
#elif defined(SSE)
    v4 result = {};
    result.smv = _mm_div_ps(a.smv, b.smv); //simple divide function
    return(result);
#endif            
}

inline v4 FCALL
operator*(v4 a, v4 b)
{
#if NO_INTRINSICS
    v4 result =
    {
	(a.x * b.x),
	(a.y * b.y),
	(a.z * b.z),
	(a.w * b.w)
    };
    return(result);
#elif defined(ARM)
    
#elif defined(SSE)
    v4 result = {};
    result.smv = _mm_mul_ps(a.smv, b.smv); //simple multiply function
    return(result);
#endif            
}

inline v4 FCALL
operator*(v4 a, r32 s)
{
    v4 temp = {s, s, s, s};
    return(a * temp);
}

inline v4 &v4::
operator*=(v4 a)
{
    *this = a * *this;
    return(*this);
}

inline v4 &v4::
operator+=(v4 a)
{
    *this = *this + a;
    return(*this);
}

inline v4 &v4::
operator-=(v4 a)
{
    *this = *this - a;
    return(*this);
}

inline bool32
operator==(v4 a, v4 b)
{
    bool32 result =
    {
	(a.x == b.x) &&
	(a.y == b.y) &&
	(a.z == b.z) &&
	(a.w == b.w)
    };
    return(result);
}

inline bool32
operator!=(v4 a, v4 b)
{
    bool32 result =
    {
	(a.x != b.x) &&
	(a.y != b.y) &&
	(a.z != b.z) &&
	(a.w != b.w)
    };
    return(result);
}

inline v4 FCALL
VectorMultiplyAdd(v4 a, v4 b, v4 c)
{
#if NO_INTRINSICS
    v4 result =
    {
	a.e[0] * b.e[0] + c.e[0],
	a.e[1] * b.e[1] + c.e[1],
	a.e[2] * b.e[2] + c.e[2],
	a.e[3] * b.e[3] + c.e[3]
    };

    return(result);
#elif defined(ARM)

#elif defined(SSE)
    m128 r = FM_FMADD_PS(a.smv, b.smv, c.smv);
    v4 result = {};
    result.smv = r;
    return(result);
#endif    
}

inline v4 FCALL
ZeroVector()
{
#if NO_INTRINSICS
    v4 result = {0.0f, 0.0f, 0.0f, 0.0f};
    return(result);
#elif defined(ARM)

#elif defined(SSE)
    v4 result = {};
    result.smv = _mm_setzero_ps();
    return(result);
#endif    
}



inline v4 FCALL
DotV4(v4 a, v4 b)
{
#if NO_INTRINSICS    
    r32 result;
    result = ((a.x)*(b.x) + (a.y)*(b.y) + (a.z)*(b.z) + (a.w)*(b.w));
    return(result);
#elif defined(ARM)

#elif defined(SSE)

#if defined(SSE_4)
    v4 result = {};
    result.smv = _mm_dp_ps(a.smv, b.smv, 0xff);
    return(result);
#elif defined(SSE_3)
    v4 result = {};
    __m128 temp = _mm_mul_ps(a.smv, b.smv); //multiply packed floats
    temp = _mm_hadd_ps(temp, temp); //horizontal add adjacent pairs of single precision floats
    result.smv = (_mm_hadd_ps(temp, temp));
    return(result);
#elif defined(SSE_2) || defined(SSE_1)
    __m128 temp2 = b.smv;
    __m128 temp = _mm_mul_ps(a.smv, temp2);
    temp2 = _mm_shuffle_ps(temp2, temp, _MM_SHUFFLE(1, 0, 0, 0));
    temp2 = _mm_add_ps(temp2, temp);
    temp = _mm_shuffle_ps(temp, temp2, _MM_SHUFFLE(0, 3, 0, 0));
    temp = _mm_add_ps(temp, temp2);
    v4 result = {};
    result.smv = FM_PERMUTE_PS(temp, _MM_SHUFFLE(2, 2, 2, 2));
    return(result);
#endif
    
#endif    
}
/*
(((V2.vector4_f32[2] * V3.vector4_f32[3]) - (V2.vector4_f32[3] * V3.vector4_f32[2])) * V1.vector4_f32[1]) - (((V2.vector4_f32[1] * V3.vector4_f32[3]) - (V2.vector4_f32[3] * V3.vector4_f32[1])) * V1.vector4_f32[2]) + (((V2.vector4_f32[1] * V3.vector4_f32[2]) - (V2.vector4_f32[2] * V3.vector4_f32[1])) * V1.vector4_f32[3]),


(((V2.vector4_f32[3] * V3.vector4_f32[2]) - (V2.vector4_f32[2] * V3.vector4_f32[3])) * V1.vector4_f32[0]) - (((V2.vector4_f32[3] * V3.vector4_f32[0]) - (V2.vector4_f32[0] * V3.vector4_f32[3])) * V1.vector4_f32[2]) + (((V2.vector4_f32[2] * V3.vector4_f32[0]) - (V2.vector4_f32[0] * V3.vector4_f32[2])) * V1.vector4_f32[3]),


(((V2.vector4_f32[1] * V3.vector4_f32[3]) - (V2.vector4_f32[3] * V3.vector4_f32[1])) * V1.vector4_f32[0]) - (((V2.vector4_f32[0] * V3.vector4_f32[3]) - (V2.vector4_f32[3] * V3.vector4_f32[0])) * V1.vector4_f32[1]) + (((V2.vector4_f32[0] * V3.vector4_f32[1]) - (V2.vector4_f32[1] * V3.vector4_f32[0])) * V1.vector4_f32[3]),

(((V2.vector4_f32[2] * V3.vector4_f32[1]) - (V2.vector4_f32[1] * V3.vector4_f32[2])) * V1.vector4_f32[0]) - (((V2.vector4_f32[2] * V3.vector4_f32[0]) - (V2.vector4_f32[0] * V3.vector4_f32[2])) * V1.vector4_f32[1]) + (((V2.vector4_f32[1] * V3.vector4_f32[0]) - (V2.vector4_f32[0] * V3.vector4_f32[1])) * V1.vector4_f32[2]),
*/




inline v4 FCALL
Cross(v4 a, v4 b, v4 c)
{
#if NO_INTRINSICS
    v4 result = {{{
		(((b.e[2] * c.e[3]) - (b.e[3] * c.e[2])) * a.e[1]) - (((b.e[1] * c.e[3]) - (b.e[3] * c.e[1])) * a.e[2]) + (((b.e[1] * c.e[2]) - (b.e[2] * c.e[1])) * a.e[3]),
		(((b.e[3] * c.e[2]) - (b.e[2] * c.e[3])) * a.e[0]) - (((b.e[3] * c.e[0]) - (b.e[0] * c.e[3])) * a.e[2]) + (((b.e[2] * c.e[0]) - (b.e[0] * c.e[2])) * a.e[3]),
		(((b.e[1] * c.e[3]) - (b.e[3] * c.e[1])) * a.e[0]) - (((b.e[0] * c.e[3]) - (b.e[3] * c.e[0])) * a.e[1]) + (((b.e[0] * c.e[1]) - (b.e[1] * c.e[0])) * a.e[3]),
		(((b.e[2] * c.e[1]) - (b.e[1] * c.e[2])) * a.e[0]) - (((b.e[2] * c.e[0]) - (b.e[0] * c.e[2])) * a.e[1]) + (((b.e[1] * c.e[0]) - (b.e[0] * c.e[1])) * a.e[2])
	    }}};
#elif defined(ARM)

#elif defined(SSE)

    //a(zwyz) * b(wzwy)
    __m128 m128Result = FM_PERMUTE_PS(b.smv, _MM_SHUFFLE(2, 1, 3, 2));
    __m128 temp3 = FM_PERMUTE_PS(c.smv, _MM_SHUFFLE(1, 3, 2, 3));
    m128Result = _mm_mul_ps(m128Result, temp3);

    //-b(wzwy) * c(zwyz)
    __m128 temp2 = FM_PERMUTE_PS(b.smv, _MM_SHUFFLE(1, 3, 2, 3));
    temp3 = FM_PERMUTE_PS(temp3, _MM_SHUFFLE(1, 3, 0, 1));
    m128Result = FM_FNMADD_PS(temp2, temp3, m128Result);

//term1 * a(yxxx)
    __m128 temp1 = FM_PERMUTE_PS(a.smv, _MM_SHUFFLE(0, 0, 0, 1));
    m128Result = _mm_mul_ps(m128Result, temp1);

    
    //b(ywxz) * c(wxwx)
    temp2 = FM_PERMUTE_PS(b.smv, _MM_SHUFFLE(2, 0, 3, 1));
    temp3 = FM_PERMUTE_PS(c.smv, _MM_SHUFFLE(0, 3, 0, 3));
    temp3 = _mm_mul_ps(temp3, temp2);

    //-b(wxwx) * c(ywxz)
    temp2 = FM_PERMUTE_PS(temp2, _MM_SHUFFLE(2, 0, 3, 1));
    temp1 = FM_PERMUTE_PS(b.smv, _MM_SHUFFLE(0, 3, 0, 3));
    temp3 = FM_FNMADD_PS(temp2, temp1, temp3);

    //m128Result - temp * a(zzyy)
    temp1 = FM_PERMUTE_PS(a.smv, _MM_SHUFFLE(1, 1, 2, 2));
    m128Result = FM_FNMADD_PS(temp1, temp3, m128Result);


//b(yzxy) * c(zxyx)
    temp2 = FM_PERMUTE_PS(b.smv, _MM_SHUFFLE(1, 0, 2, 1));
    temp3 = FM_PERMUTE_PS(c.smv, _MM_SHUFFLE(0, 1, 0, 2));
    temp3 = _mm_mul_ps(temp3, temp2);

//-b(zxyx) * c(yzxy)
    temp2 = FM_PERMUTE_PS(temp2, _MM_SHUFFLE(2, 0, 2, 1));
    temp1 = FM_PERMUTE_PS(c.smv, _MM_SHUFFLE(1, 0, 2, 1));
    temp3 = FM_FNMADD_PS(temp1, temp2, temp3);

//result + term * a(wwwz)
    temp1 = FM_PERMUTE_PS(a.smv, _MM_SHUFFLE(2, 3, 3, 3));
    m128Result = FM_FMADD_PS(temp3, temp1, m128Result);
    v4 result = {};
    result.smv = m128Result;
    return(result); 

    
#endif    
}

/*
  NOTE: the following aren't defined bc we don't need them unless intrinsics aren't being used, fill them out when
  you feel necessary but remeber using them won't actually do anything 
 */
inline v4 FCALL
VecSqrt(v4 a)
{
#if NO_INTRINSICS


#elif defined(ARM)

#elif defined(SSE)
#if defined(SSE_4)

#elif defined(SSE_3)

#elif defined(SSE_2) || defined(SSE_1)
    
#endif    
    
#endif    
}


inline v4 FCALL
VecLenSq(v4 a)
{
    return(DotV4(a, a));
}


inline v4 FCALL
VecLen(v4 a)
{
#if NO_INTRINSICS
    v4 result = {};
    result = VecLenSq(a); //DEFINE THIS
    result = VecSqrt(result); //DEFINE THIS
    return(result);

#elif defined(ARM)

#elif defined(SSE)
#if defined(SSE_4)

#elif defined(SSE_3)

#elif defined(SSE_2) || defined(SSE_1)
    
#endif    
    
#endif    
}

/*
  END NOTE
 */

inline v4 FCALL
VecLenV3(v4 a)
{
#if NO_INTRINSICS
    
#elif defined(ARM)

#elif defined(SSE)
#if defined(SSE_4)
    v4 temp = {};
    temp.smv = _mm_dp_ps(a.smv, a.smv, 0x7f);
    temp.smv = _mm_sqrt_ps(temp.smv);
    return(temp);
#elif defined(SSE_3)
    v4 vLenSq = {};
    vLenSq.smv = _mm_mul_ps(a.smv, a.smv);
    vLenSq.smv = _mm_and_ps(vLenSq.smv, FM_MASK_3.smv);
    vLenSq.smv = _mm_hadd_ps(vLenSq.smv, vLenSq.smv);
    vLenSq.smv = _mm_hadd_ps(vLenSq.smv, vLenSq.smv);
    vLenSq.smv = _mm_sqrt_ps(vLenSq.smv);
    return(vLenSq);
#elif defined(SSE_2) || defined(SSE_1)
    v4 vLenSq = {};
    vLenSq.smv = _mm_mul_ps(a.smv, a.smv);
    v4 temp = {};
    temp.smv = FM_PERMUTE_PS(vLenSq.smv, _MM_SHUFFLE(1, 2, 1, 2));
    vLenSq.smv = _mm_add_ss(vLenSq.smv, temp.smv);
    temp.smv = FM_PERMUTE_PS(temp.smv, _MM_SHUFFLE(1, 1, 1, 1));
    vLenSq.smv = _mm_add_ss(vLenSq.smv, temp.smv);
    vLenSq.smv = FM_PERMUTE_PS(vLenSq.smv, _MM_SHUFFLE(0, 0, 0, 0));
    vLenSq.smv = _mm_sqrt_ps(vLenSq.smv);
    return(vLenSq);
#endif    
#endif    
}

inline v4 FCALL
VecLenV4(v4 v)
{
#if NO_INTRINSICS

#elif defined(ARM)

#elif defined(SSE)
    m128 lenSq = _mm_mul_ps(v.smv, v.smv);
    //temp has z and w
    m128 temp = FM_PERMUTE_PS(lenSq, _MM_SHUFFLE(3, 2, 3, 2));
    //x+z y+w
    lenSq = _mm_add_ps(lenSq, temp);
    //x+z x+z x+z y+w
    lenSq = FM_PERMUTE_PS(lenSq, _MM_SHUFFLE(1, 0, 0, 0));
    //??, ??, y+w y+w
    temp = _mm_shuffle_ps(temp, lenSq, _MM_SHUFFLE(3, 3, 0, 0));
    //??, ??, x_z y_w, ??
    lenSq = _mm_add_ps(lenSq, temp);
    lenSq = FM_PERMUTE_PS(lenSq, _MM_SHUFFLE(2, 2, 2, 2));
    lenSq = _mm_sqrt_ps(lenSq);

    v4 result = {};
    result.smv = lenSq;
    return(result);
#endif    
}

inline v4 FCALL
NegateVector(v4 a)
{
#if NO_INTRINSICS
    v4 result =
    {
	-v.e[0],
	-v.e[1],
	-v.e[2],
	-v.e[3],	
    };
    return(result);
#elif defined(ARM)
    
#elif defined(SSE)
    v4 z, result;
    z.smv = _mm_setzero_ps();
    result.smv = _mm_sub_ps(z.smv, a.smv);
    return(result);
#endif    
}

inline bool32 FCALL
Vector3Equal(v4 a, v4 b)
{
#if NO_INTRINSICS

    return (((a.e[0] == b.e[0]) && (a.e[1] == b.e[1]) && (a.e[2] == b.e[2]) && (a.e[3] == b.e[3])) != 0);
    
#elif defined(ARM)

#elif defined(SSE)
    v4 temp = {};
    temp.smv = _mm_cmpeq_ps(a.smv, b.smv);
    return(((_mm_movemask_ps(temp.smv) & 7) == 7) != 0);
#endif    
}

inline bool32 FCALL
Vector3IsInf(v4 a)
{
#if NO_INTRINSICS
    //requires use of a function in std which I don't wanna use so if you
    //aren't using intrinsics just deal w/ it

#elif defined(ARM)

#elif defined(SSE)
    m128 temp = _mm_and_ps(a.smv, FM_ABS_MASK.smv);
    //Compare to inf
    temp = _mm_cmpeq_ps(temp, FM_INF.smv);
    //If x y or z are inf sings are true
    return((_mm_movemask_ps(temp) & 7) != 0);
#endif    
}

inline v4 FCALL
CrossV3(v4 a, v4 b)
{
#if NO_INTRINSICS

    v4 result = 
    {
	(a.e[1] * b.e[2]) - (a.e[2] * b.e[1]),
	(a.e[2] * b.e[0]) - (a.e[0] * b.e[2]),
	(a.e[0] * b.e[1]) - (a.e[1] * b.e[0]),
	0.0f
    };

    return(result);

#elif defined(ARM)

#elif defined(SSE)

    v4 result;
    // y1,z1,x1,w1
    m128 temp1 = FM_PERMUTE_PS(a.smv, _MM_SHUFFLE(3, 0, 2, 1));
    // z2,x2,y2,w2
    m128 temp2 = FM_PERMUTE_PS(b.smv, _MM_SHUFFLE(3, 1, 0, 2));
    //Perform left operation
    result.smv = _mm_mul_ps(temp1, temp2);
    // z1,x1,y1,w1
    temp1 = FM_PERMUTE_PS(temp1, _MM_SHUFFLE(3, 0, 2, 1));
    // y2,z2,x2,w2    
    temp2 = FM_PERMUTE_PS(temp2, _MM_SHUFFLE(3, 1, 0, 2));
    //Perform right operation
    result.smv = FM_FNMADD_PS(temp1, temp2, result.smv);
    result.smv = _mm_and_ps(result.smv, FM_MASK_3.smv);
    return(result);
    
#endif    
}

inline v4 FCALL
VectorLerp(v4 a, v4 b, v4 t)
{
#if NO_INTRINSICS
    v4 length = b - a;
    v4 result = VectorMultiplyAdd(length, t, a);
#elif defined(ARM)

#elif defined(SSE)
    v4 len = {};
    len.smv = _mm_sub_ps(b.smv, a.smv);
    v4 result = {};
    result.smv = FM_FMADD_PS(len.smv, t.smv, a.smv);
    return(result);
#endif    
}

inline v4 FCALL
DotV3(v4 a, v4 b)
{
#if NO_INTRINSICS

    r32 fVal = a.e[0] * b.e[0] + a.e[1] * b.e[1] + a.e[2] * b.e[2];
    v4 result = {};
    result.e[0] =
	result.e[1] =
	result.e[2]
	result.e[3] = fVal;
    return(result);
    
#elif defined(ARM)

#elif defined(SSE)    

#if defined(SSE_4)

    v4 result = {};
    result.smv = _mm_dp_ps(a, b, 0x7f);
    return(result);
    
#elif defined(SSE_3)

    v4 temp, result;
    temp.smv = _mm_mul_ps(a.smv, b.smv);
    temp.smv = _mm_and_ps(temp.smv, FM_MASK_3.smv);
    temp.smv = _mm_hadd_ps(temp.smv, temp.smv);
    result.smv = _mm_hadd_ps(temp.smv, temp.smv);
    return(result);
    
#elif defined(SSE_2) || defined(SSE_1)
    v4 dot, temp, result;
    dot.smv = _mm_mul_ps(a.smv, b.smv);
    // x=Dot.vector4_f32[1], y=Dot.vector4_f32[2]
    temp.smv = FM_PERMUTE_PS(dot.smv, _MM_SHUFFLE(2, 1, 2, 1));
    // Result.vector4_f32[0] = x+y
    dot.smv = _mm_add_ss(dot.smv, temp.smv);
    // x=Dot.vector4_f32[2]
    temp.smv = FM_PERMUTE_PS(temp.smv, _MM_SHUFFLE(1, 1, 1, 1));
    // Result.vector4_f32[0] = (x+y)+z
    dot.smv = _mm_add_ss(dot.smv, temp.smv);
    //SPLAT x
    result.smv = FM_PERMUTE_PS(dot.smv, _MM_SHUFFLE(0, 0, 0, 0));
    return(result);
#endif    
    
#endif    
    
}

inline v4 FCALL
NormalizeV3(v4 a)
{
#if NO_INTRINSICS
    r32 len = 0.0f;
    v4 result = {};

    result = VecLenV3(a);
    len = result.e[0];
    if (len > 0)
    {
	len = 1.0f / len;
    }

    result.e[0] = a.e[0] * len;
    result.e[1] = a.e[1] * len;
    result.e[2] = a.e[2] * len;
    result.e[3] = a.e[3] * len;
    return(result);

#elif defined(ARM)

#elif defined(SSE)

#if defined(SSE_4)
    v4 result, zeroMask, lenSq;
    lenSq.smv = _mm_dp_ps(a.smv, a.smv, 0x7f);
    //Prep for division
    result.smv = _mm_sqrt_ps(lenSq.smv);
    //Create 0 w/ single instruction
    zeroMask.smv = _mm_setzero_ps();
    //Test for divide by 0
    zeroMask.smv = _mm_cmpneq_ps(zeroMask.smv, result.smv);
    //Failsafe on zero len planes
    //If len is inf set elements to 0
    lenSq.smv = _mm_cpmneq_ps(lenSq.smv, FM_INF.smv);
    //Divide to perform normalization
    result.smv = _mm_div_ps(a.smv, result.smv);
    //Any inf set to 0
    result.smv = _mm_and_ps(result.smv, zeroMask.smv);
    //Select qnana or result based on inf len
    v4 temp1, temp2;
    temp1.smv = _mm_andnot_ps(lenSq.smv, FM_QNaN.smv);
    temp2.smv = _mm_and_ps(result.smv, lenSq.smv);
    result.smv = _mm_or_ps(temp1.smv, temp2.smv);
    return(result);
#elif defined(SSE_3)

    v4 lenSq, result, zeroMask;
    //Perform dot product on x y and z only
    lenSq.smv = _mm_mul_ps(a.smv, a.smv);
    lenSq.smv = _mm_and_ps(lenSq.smv, FM_MASK_3.smv);
    lenSq.smv = _mm_hadd_ps(lenSq.smv, lenSq.smv);
    lenSq.smv = _mm_hadd_ps(lenSq.smv, lenSq.smv);
    //Prep for division
    result.smv = _mm_sqrt_ps(lenSq.smv);
    //Create 0 w/ one instruction
    zeroMask.smv = _mm_setzero_ps();//this could be a useful instruction
    //Test for divide by zero
    zeroMask.smv = _mm_cmpneq_ps(zeroMask.smv, result.smv);
    //Failsafe on 0 len planes
    //if len is inf set elements to 0
    lenSq.smv = _mm_cmpneq_ps(lenSq.smv, FM_INF.smv);
    //Divide to perform normalization
    result.smv = _mm_div_ps(a.smv, result.smv);
    //Any inf set to 0
    result.smv = _mm_and_ps(result.smv, zeroMask.smv);
    //Select qnan or inf len
    v4 temp1, temp2;
    temp1.smv = _mm_andnot_ps(lenSq.smv, FM_QNaN.smv);
    temp2.smv = _mm_and_ps(result.smv, lenSq.smv);
    result.smv = _mm_or_ps(temp1.smv, temp2.smv);
    return(result);
    
#elif defined(SSE_2) || defined(SSE_1)
    //perf dot on x y and z only
    v4 lenSq, temp, result, zeroMask;
    lenSq.smv = _mm_mul_ps(a.smv, a.smv);
    temp.smv = FM_PERMUTE_PS(lenSq.smv, _MM_SHUFFLE(2, 1, 2, 1));
    lenSq.smv = _mm_add_ss(lenSq.smv, temp.smv);
    temp.smv = FM_PERMUTE_PS(temp.smv, _MM_SHUFFLE(1, 1, 1, 1));
    lenSq.smv = _mm_add_ss(lenSq.smv, temp.smv);
    lenSq.smv = FM_PERMUTE_PS(lenSq.smv, _MM_SHUFFLE(0, 0, 0, 0));
    //Prep for division
    result.smv = _mm_sqrt_ps(lenSq.smv);
    //Create  0
    zeroMask.smv = _mm_setzero_ps();
    //Test for divide by 0
    zeroMask.smv = _mm_cmpneq_ps(zeroMask.smv, result.smv);
    //Failsafe on 0
    //If len is inf set to 0
    lenSq.smv = _mm_cmpneq_ps(lenSq.smv, FM_INF.smv);
    //Div to perform normalzation
    result.smv = _mm_div_ps(a.smv, result.smv);
    //Any inf set to 0
    result.smv = _mm_and_ps(result.smv, zeroMask.smv);
    //Select qnan or inf
    v4 temp1, temp2;
    temp1.smv = _mm_andnot_ps(lenSq.smv, FM_QNaN.smv);
    temp2.smv = _mm_and_ps(result.smv, lenSq.smv);
    result.smv = _mm_or_ps(temp1.smv, temp2.smv);
    return(result);
#endif

#endif    
}

inline v4 FCALL
NormalizeV4(v4 a)
{
#if NO_INTRINSICS
    r32 fLen;
    v4 result = {};

    result = VecLen(a); //DEFINE THIS 
    fLen = result.e[0];

    //prevent divide by 0
    if (fLen > 0)
	fLen = 1.0f / fLen;

    result.e[0] = a.e[0] * fLen;
    result.e[1] = a.e[1] * fLen;
    result.e[2] = a.e[2] * fLen;
    result.e[3] = a.e[3] * fLen;
    return(result);

#elif defined(ARM)

#elif defined(SSE)
#if defined(SSE_4)
    m128 lenSq = _mm_dp_ps(a.smv, a.smv, 0xff); //tis a multiply
    //prep for division
    m128 m128Result = _mm_sqrt_ps(lenSq);
    //Create zero w/ single instruction
    m128 zeroMask = _mm_setzero_ps();
    //test for a divide by 0
    zeroMask = _mm_cmpneq_ps(zeroMask, result); //compare all elements in two m128 for not equal
    //failsafe on zero
    //if len is infinite, set elements to zero
    lenSq = _mm_cmpneq_ps(lenSq, FM_INF);
    //divide to perform normalization
    m128Result = _mm_div_ps(a, m128Result);
    //Any that are inf set to 0
    m128Result = _mm_and_ps(m128Result, zeroMask); //bitwise and
    //select qnan or result based on inf len
    m128 temp1 = _mm_andnot_ps(lenSq, FM_QNaN);
    m128 temp2 = _mm_and_ps(m128Result, lenSq);
    m128Result = _mm_or_ps(temp1, temp2); //bitwise or
    v4 result = {};
    result.smv = m128Result;
    return(result);
#elif defined(SSE_3)
    m128 lenSq = _mm_mul_ps(a.smv, a.smv);
    //Perform dot product on x, y, z, and w
    lenSq = _mm_hadd_ps(lenSq, lenSq);
    lenSq = _mm_hadd_ps(lenSq, lenSq);    
    //prep for division
    v4 result = {};
    result.smv = _mm_sqrt_ps(lenSq);
    //Create zero w/ single instruction
    m128 zeroMask = _mm_setzero_ps();
    //Test for divide by zero
    zeroMask = _mm_cmpneq_ps(zeroMask, result.smv);
    //if len is inf set elements to zero
    lenSq = _mm_cmpneq_ps(lenSq, FM_INF.smv);
    //divide to perform normalization
    result.smv = _mm_div_ps(a.smv, result.smv);
    //Any that are info set to 0
    result.smv = _mm_and_ps(result.smv, zeroMask);

    //select qnan or result based on inf len
    m128 temp1 = _mm_andnot_ps(lenSq, FM_QNaN.smv);
    m128 temp2 = _mm_and_ps(result.smv, lenSq);
    result.smv = _mm_or_ps(temp1, temp2);
    return(result);
#elif defined(SSE_2) || defined(SSE_1)
    //perform dot product on x, y, z, and w
    m128 lenSq = _mm_mul_ps(a.smv, a.smv);
    //vtemp has z and w
    m128 temp = FM_PERMUTE_PS(lenSq, _MM_SHUFFLE(3, 2, 3, 2));
    //x+z y+w
    lenSq = _mm_add_ps(lenSq, temp);
    //x+z, x+z, x+z, y+w
    lenSq = FM_PERMUTE_PS(lenSq, _MM_SHUFFLE(1, 0, 0, 0));
    //??, ??, y+w, y+w
    temp = _mm_shuffle_ps(temp, lenSq, _MM_SHUFFLE(3, 3, 0, 0));
    //??, ??, x+z+y+w, ??
    lenSq = _mm_add_ps(lenSq, temp);
    //Splat the len
    lenSq = FM_PERMUTE_PS(lenSq, _MM_SHUFFLE(2, 2, 2, 2));
    //Prep for division
    v4 result = {};
    result.smv = _mm_sqrt_ps(lenSq);
    //Create zero w/ single instruction
    m128 zeroMask = _mm_setzero_ps();
    //Test for divide by zero
    zeroMask = _mm_cmpneq_ps(zeroMask, result.smv);
    //failsafe on zero if len is inf set elements to 0
    lenSq = _mm_cmpneq_ps(lenSq, FM_INF.smv);
    //Divide to perform normalization
    result.smv = _mm_div_ps(a.smv, result.smv);
    //Any that are info set to zero
    result.smv = _mm_and_ps(result.smv, zeroMask);
    //Select qnan or result based on inf len
    m128 temp1 = _mm_andnot_ps(lenSq, FM_QNaN.smv);
    m128 temp2 = _mm_and_ps(result.smv, lenSq);
    result.smv = _mm_or_ps(temp1, temp2);
    return(result);
#endif
    
#endif    
}

inline v4 FCALL
VectorSelect(v4 a, v4 b, v4u32 control)
{
#if NO_INTRINSICS
    v4u32 u32Result =
    {
	(((u32)a.e[0]) & ~((u32)control.e[0])) | (((u32)b.e[0]) & ((u32)control.e[0])),
	(((u32)a.e[1]) & ~((u32)control.e[1])) | (((u32)b.e[1]) & ((u32)control.e[1])),
	(((u32)a.e[2]) & ~((u32)control.e[2])) | (((u32)b.e[2]) & ((u32)control.e[2])),
	(((u32)a.e[3]) & ~((u32)control.e[3])) | (((u32)b.e[3]) & ((u32)control.e[3])),	
    };

    v4 result =
    {
	(r32)u32Result.e[0], (r32)u32Result.e[1], (r32)u32Result.e[2], (r32)u32Result.e[3]
    };
    return(result);
#elif defined(ARM)

#elif defined(SSE)
    v4 temp1, temp2, result;
    temp1.smv = _mm_andnot_ps(control.smv, a.smv);
    temp2.smv = _mm_and_ps(b.smv, control.smv);
    result.smv = _mm_or_ps(temp1.smv, temp2.smv);
    return(result);
#endif    
}

inline v4 FCALL
VectorSelect(v4 a, v4 b, v4 control)
{
    v4u32 temp = {};
    temp.smv = control.smv;
    return(VectorSelect(a, b, temp));
}

inline v4 FCALL
VectorSelect(v4u32 a, v4 b, v4u32 control)
{
    v4 tempA = {};
    tempA.smv = a.smv;
    return(VectorSelect(tempA, b, control));
}
    

inline v4 FCALL
SplatX(v4 v)
{
#if NO_INTRINSICS
    v4 result = {};
    result.e[0] =
	result.e[1] =
	result.e[2] =
	result.e[3] = v.e[0];

    return(result);
#elif defined(ARM)

#elif defined(SSE)
    v4 result = {};
    result.smv = FM_PERMUTE_PS(v.smv, _MM_SHUFFLE(0, 0, 0, 0));
    return(result);
#endif    
}

inline v4 FCALL
SplatY(v4 v)
{
#if NO_INTRINSICS
    v4 result = {};
    result.e[0] =
	result.e[1] =
	result.e[2] =
	result.e[3] = v.e[1];

    return(result);
#elif defined(ARM)

#elif defined(SSE)
    v4 result = {};
    result.smv = FM_PERMUTE_PS(v.smv, _MM_SHUFFLE(1, 1, 1, 1));
    return(result);
#endif    
}

inline v4 FCALL
SplatZ(v4 v)
{
#if NO_INTRINSICS
    v4 result = {};
    result.e[0] =
	result.e[1] =
	result.e[2] =
	result.e[3] = v.e[2];

    return(result);
#elif defined(ARM)

#elif defined(SSE)
    v4 result = {};
    result.smv = FM_PERMUTE_PS(v.smv, _MM_SHUFFLE(2, 2, 2, 2));
    return(result);
#endif    
}

inline v4 FCALL
SplatW(v4 v)
{
#if NO_INTRINSICS
    v4 result = {};
    result.e[0] =
	result.e[1] =
	result.e[2] =
	result.e[3] = v.e[3];

    return(result);
#elif defined(ARM)

#elif defined(SSE)
    v4 result = {};
    result.smv = FM_PERMUTE_PS(v.smv, _MM_SHUFFLE(3, 3, 3, 3));
    return(result);
#endif    
}

inline v4 FCALL
VectorTrueInt()
{
#if NO_INTRINSICS    
    v4u32 u32Result = {0xFFFFFFFU, 0xFFFFFFFU, 0xFFFFFFFU, 0xFFFFFFFU};

    v4 result = {};
    result.smv = u32Result.smv;
    return(result);
#elif defined(ARM)

#elif defined(SSE)    
    __m128i v = _mm_set1_epi32(-1);
    m128 mRes = _mm_castsi128_ps(v);
    v4 result = {};
    result.smv = mRes;
    return(result);
#endif    
}

inline v4 FCALL
VectorAndInt(v4 a, v4 b)
{
#if NO_INTRINSICS
    v4u32 result =
    {
	(u32)a.e[0] & (u32)b.e[0],
	(u32)a.e[1] & (u32)b.e[1],
	(u32)a.e[2] & (u32)b.e[2],
	(u32)a.e[3] & (u32)b.e[3]	
    };

    return(result);
#elif defined(ARM)

#elif defined(SSE)
    v4 result = {};
    result.smv = _mm_and_ps(a.smv, b.smv);
    return(result);
#endif
}

inline v4 FCALL
VectorEqualInt(v4 a, v4 b)
{
#if NO_INTRINSICS    
    v4u32 control =
    {
	((u32)a.e[0] == (u32)b.e[0]) ? 0xFFFFFFFF : 0,
	((u32)a.e[1] == (u32)b.e[1]) ? 0xFFFFFFFF : 0,
	((u32)a.e[2] == (u32)b.e[2]) ? 0xFFFFFFFF : 0,
	((u32)a.e[3] == (u32)b.e[3]) ? 0xFFFFFFFF : 0	
    };
    v4 result = {};
    result.smv = control.smv;
    return(result);
#elif defined(ARM)

#elif defined(SSE)
    __m128i v = _mm_cmpeq_epi32(_mm_castps_si128(a.smv), _mm_castps_si128(b.smv));
    m128 vres = _mm_castsi128_ps(v);
    v4 result = {};
    result.smv = vres;
    return(result);
#endif    
    
}

inline v4 FCALL
VectorOrInt(v4 a, v4 b)
{
#if NO_INTRINSICS
    v4u32 u32Res =
    {
	(u32)a.e[0] | (u32)b.e[0],
	(u32)a.e[1] | (u32)b.e[1],
	(u32)a.e[2] | (u32)b.e[2],
	(u32)a.e[3] | (u32)b.e[3]
    };

    v4 result = {};
    result.smv = u32Res.smv;
    return(result);
#elif defined(ARM)

#elif defined(SSE)
    __m128i v = _mm_or_si128(_mm_castps_si128(a.smv), _mm_castps_si128(b.smv));
    v4 result = {};
    result.smv = _mm_castsi128_ps(v);
    return(result);
#endif    
}

inline v4 FCALL
VectorEqual(v4 a, v4 b)
{
#if NO_INTRINSICS
    v4u32 control =
    {
	(a.e[0] == b.e[0]) ? 0xFFFFFFFF : 0,
	(a.e[1] == b.e[1]) ? 0xFFFFFFFF : 0,
	(a.e[2] == b.e[2]) ? 0xFFFFFFFF : 0,
	(a.e[3] == b.e[3]) ? 0xFFFFFFFF : 0
    };
    v4 result = {};
    result.smv = control.smv;
    return(result);
#elif defined(ARM)

#elif defined(SSE)
    v4 result = {};
    result.smv = _mm_cmpeq_ps(a.smv, b.smv);
    return(result);
#endif    
}

inline v4 FCALL
VectorIsInf(v4 a)
{
#if NO_INTRINSICS
    v4u32 control =
    {
	FMISINF(a.e[0]) ? 0xFFFFFFFU : 0,
	FMISINF(a.e[1]) ? 0xFFFFFFFU : 0,
	FMISINF(a.e[2]) ? 0xFFFFFFFU : 0,
	FMISINF(a.e[3]) ? 0xFFFFFFFU : 0
    };

    v4 result = {};
    result.smv = control.smv;
    return(result);
#elif defined(ARM)

#elif defined(SSE)
    m128 temp = _mm_and_ps(a.smv, FM_ABS_MASK.smv);
    temp = _mm_cmpeq_ps(temp, FM_INF.smv);
    v4 result = {};
    result.smv = temp;
    return(result);
#endif
}

inline v4 FCALL
VectorAbs(v4 v)
{
#if NO_INTRINSICS
    v4 result =
    {
	fabsf(v.e[0]),
	fabsf(v.e[1]),
	fabsf(v.e[2]),
	fabsf(v.e[3])
    };
    return(result);
#elif defined(ARM)

#elif defined(SSE)
    m128 temp = _mm_setzero_ps();
    temp = _mm_sub_ps(temp, v.smv);
    temp = _mm_max_ps(temp, v.smv);
    v4 result = {};
    result.smv = temp;
    return(result);
#endif    
}

//checked
inline v4 FCALL
VectorATan(v4 v)
{
#if NO_INTRINSICS
    v4 result =
    {
	atanf(v.e[0]),
	atanf(v.e[1]),
	atanf(v.e[2]),
	atanf(v.e[3])	
    };
    return(result);
#elif defined(ARM)

#elif defined(SSE)
    v4 absV = VectorAbs(v);
    m128 invV = _mm_div_ps(FM_ONE.smv, v.smv);
    m128 comp = _mm_cmpgt_ps(v.smv, FM_ONE.smv);
    m128 select0 = _mm_and_ps(comp, FM_ONE.smv);
    m128 select1 = _mm_andnot_ps(comp, FM_NEGATIVE_ONE.smv);
    m128 sign = _mm_or_ps(select0, select1);
    comp = _mm_cmple_ps(absV.smv, FM_ONE.smv);
    select0 = _mm_and_ps(comp, FM_ZERO.smv);
    select1 = _mm_andnot_ps(comp, sign);
    sign = _mm_or_ps(select0, select1);
    select0 = _mm_and_ps(comp, v.smv);
    select1 = _mm_andnot_ps(comp, invV);
    m128 x = _mm_or_ps(select0, select1);

    m128 x2 = _mm_mul_ps(x, x);

    v4 tc1 = FM_ATAN_COEFFICIENTS_1;
    m128 vConstantsB = FM_PERMUTE_PS(tc1.smv, _MM_SHUFFLE(3, 3, 3, 3));
    m128 vConstants = FM_PERMUTE_PS(tc1.smv, _MM_SHUFFLE(2, 2, 2, 2));
    m128 res = FM_FMADD_PS(vConstantsB, x2, vConstants);

    vConstants = FM_PERMUTE_PS(tc1.smv, _MM_SHUFFLE(1, 1, 1, 1));
    res = FM_FMADD_PS(res, x2, vConstants);

    vConstants = FM_PERMUTE_PS(tc1.smv, _MM_SHUFFLE(0, 0, 0, 0));
    res = FM_FMADD_PS(res, x2, vConstants);

    v4 tc0 = FM_ATAN_COEFFICIENTS_0;
    vConstants = FM_PERMUTE_PS(tc0.smv, _MM_SHUFFLE(3, 3, 3, 3));
    res = FM_FMADD_PS(res, x2, vConstants);

    vConstants = FM_PERMUTE_PS(tc0.smv, _MM_SHUFFLE(2, 2, 2, 2));
    res = FM_FMADD_PS(res, x2, vConstants);

    vConstants = FM_PERMUTE_PS(tc0.smv, _MM_SHUFFLE(1, 1, 1, 1));
    res = FM_FMADD_PS(res, x2, vConstants);
    
    vConstants = FM_PERMUTE_PS(tc0.smv, _MM_SHUFFLE(0, 0, 0, 0));
    res = FM_FMADD_PS(res, x2, vConstants);

    res = FM_FMADD_PS(res, x2, FM_ONE.smv);

    res = _mm_mul_ps(res, x);
    m128 res1 = _mm_mul_ps(sign, FM_HALFPI.smv);
    res1 = _mm_sub_ps(res1, res);

    comp = _mm_cmpeq_ps(sign, FM_ZERO.smv);
    select0 = _mm_and_ps(comp, res);
    select1 = _mm_andnot_ps(comp, res1);
    res = _mm_or_ps(select0, select1);
    v4 result = {};
    result.smv = res;
    return(result);
#endif    
}

inline v4 FCALL
Vector4Length(v4 v)
{
#if NO_INTRINSICS

#elif defined(ARM)

#elif defined(SSE)

#if defined(SSE_4)
    m128 temp = _mm_dp_ps(v.smv, v.smv, 0xff);
    v4 result = {};
    result.smv = _mm_sqrt_ps(temp);
    return(result);
#elif defined(SSE_3)
    m128 lenSq = _mm_mul_ps(v.smv, v.smv);
    lenSq = _mm_hadd_ps(lenSq, lenSq);
    lenSq = _mm_hadd_ps(lenSq, lenSq);
    lenSq = _mm_sqrt_ps(lenSq);
    v4 result = {};
    result.smv = lenSq;
    return(result);
#elif defined(SSE_2) || defined(SSE_1)
    m128 lenSq = _mm_mul_ps(v.smv, v.smv);
    m128 temp = FM_PERMUTE_PS(lenSq, _MM_SHUFFLE(3, 2, 3, 2));
    lenSq = _mm_add_ps(lenSq, temp);
    lenSq = FM_PERMUTE_PS(lenSq, _MM_SHUFFLE(1, 0, 0, 0));
    temp = _mm_shuffle_ps(temp, lenSq, _MM_SHUFFLE(3, 3, 0, 0));
    lenSq = _mm_add_ps(lenSq, temp);
    lenSq = FM_PERMUTE_PS(lenSq, _MM_SHUFFLE(2, 2, 2, 2));
    lenSq = _mm_sqrt_ps(lenSq);
    v4 result = {};
    result.smv = lenSq;
    return(result);
#endif    
    
#endif    
}

inline v4 FCALL
VectorATan2(v4 y, v4 x)
{
#if NO_INTRINSICS
    v4 result =
    {
	atan2f(y.e[0], x.e[0]),
	atan2f(y.e[1], x.e[1]),
	atan2f(y.e[2], x.e[2]),
	atan2f(y.e[3], x.e[3]),	
    };

    return(result);
#elif defined(ARM)

#elif defined(SSE)
    v4 aTan2Constants = {FM_PI, FM_PIDIV2, FM_PIDIV4, FM_PI * 3.0f / 4.0f};

    v4 zero = {0.0f, 0.0f, 0.0f, 0.0f};
    v4 aTanResultValid = VectorTrueInt();
    v4 pi = SplatX(aTan2Constants);
    v4 piOverTwo = SplatY(aTan2Constants);
    v4 piOverFour = SplatZ(aTan2Constants);
    v4 threePiOverFour = SplatW(aTan2Constants);

    v4 yEqualsZero = VectorEqual(y, zero);
    v4 xEqualsZero = VectorEqual(y, zero);
    v4 negativeZero = {};
    negativeZero.smv = FM_NEGATIVE_ZERO.smv;
    v4 xIsPositive = VectorAndInt(x, negativeZero);
    xIsPositive = VectorEqualInt(xIsPositive, zero);
    v4 yEqualsInf = VectorIsInf(y);
    v4 xEqualsInf = VectorIsInf(x);

    v4 ySign = VectorAndInt(y, negativeZero);
    pi = VectorOrInt(pi, ySign);
    piOverTwo = VectorOrInt(piOverTwo, ySign);
    piOverFour = VectorOrInt(piOverFour, ySign);
    threePiOverFour = VectorOrInt(threePiOverFour, ySign);



    v4 r1 = VectorSelect(pi, ySign, xIsPositive);
    v4 r2 = VectorSelect(aTanResultValid, piOverTwo, xEqualsZero);
    v4 r3 = VectorSelect(r2, r1, yEqualsZero);
    v4 r4 = VectorSelect(threePiOverFour, piOverFour, xIsPositive);
    v4 r5 = VectorSelect(piOverTwo, r4, xEqualsInf);

    v4 result = VectorSelect(r3, r5, yEqualsInf);
    aTanResultValid = VectorEqualInt(result, aTanResultValid);

    v4 v = y / x;
    v4 r0 = VectorATan(v);


    
    r1 = VectorSelect(pi, negativeZero, xIsPositive);
    r2 = r0 + r1;
    return(VectorSelect(result, r2, aTanResultValid));
#endif
}

inline v4 FCALL
VectorLess(v4 a, v4 b)
{
#if NO_INTRINSICS
    v4 control =
    {
	(a.e[0] < b.e[0]) ? 0xFFFFFFFF : 0,
	(a.e[1] < b.e[1]) ? 0xFFFFFFFF : 0,
	(a.e[2] < b.e[2]) ? 0xFFFFFFFF : 0,
	(a.e[3] < b.e[3]) ? 0xFFFFFFFF : 0
    };

    return(control);
#elif defined(ARM)

#elif defined(SSE)
    v4 result = {};
    result.smv = _mm_cmplt_ps(a.smv, b.smv);
    return(result);
#endif
}

inline v4 FCALL
MergeXY(v4 a, v4 b)
{
#if NO_INTRINSICS
    v4u32 u32Result =
    {
	(u32)a.e[0],
	(u32)a.e[0],
	(u32)a.e[1],
	(u32)a.e[1]
    };

    v4 result =
    {
	(r32)u32Result.e[0],
	(r32)u32Result.e[1],
	(r32)u32Result.e[2],
	(r32)u32Result.e[3],	
    };

    return(result);

#elif defined(ARM)

#elif defined(SSE)
    v4 result = {};
    result.smv = _mm_unpacklo_ps(a.smv, b.smv);
    return(result);
#endif    
}

inline v4 FCALL
MergeZW(v4 a, v4 b)
{
#if NO_INTRINSICS
    v4u32 u32Result =
    {
	(u32)a.e[2],
	(u32)a.e[2],
	(u32)a.e[3],
	(u32)a.e[3]
    };

    v4 result =
    {
	(r32)u32Result.e[0];
	(r32)u32Result.e[1];
	(r32)u32Result.e[2];
	(r32)u32Result.e[3];	
    };

    return(result);
#elif defined(ARM)

#elif defined(SSE)
    v4 result = {};
    result.smv = _mm_unpackhi_ps(a.smv, b.smv);
    return(result);

#endif
}

inline v4 FCALL VectorRound(v4 v)
{
#if NO_INTRINSICS
    
#elif defined(ARM)

#elif defined(SSE)
    m128 sign = _mm_and_ps(v.smv, FM_NEGATIVE_ZERO.smv);
    m128 sMagic = _mm_or_ps(FM_NO_FRACTION.smv, sign);
    m128 r1 = _mm_add_ps(v.smv, sMagic);
    r1 = _mm_sub_ps(r1, sMagic);
    m128 r2 = _mm_and_ps(v.smv, FM_ABS_MASK.smv);
    m128 mask = _mm_cmple_ps(r2, FM_NO_FRACTION.smv);
    r2 = _mm_andnot_ps(mask, v.smv);
    r1 = _mm_and_ps(r1, mask);
    v4 result = {};
    result.smv = _mm_xor_ps(r1, r2);
    return(result);
#endif    
}

inline v4 FCALL VectorModAngles(v4 angles)
{
#if NO_INTRINSICS
    
#elif defined(ARM)

#elif defined(SSE)
    v4 result = {};
    result.smv = _mm_mul_ps(angles.smv, FM_RECIPROCAL_TWO_PI.smv);
    result = VectorRound(result);
    result.smv = FM_FNMADD_PS(result.smv, FM_TWO_PI.smv, angles.smv);
    return(result);
#endif    
}

//Checked
inline v4 FCALL
VectorSin(v4 v)
{
#if NO_INTRINSICS
    v4 result =
    {
	sinf(v.e[0]),
	sinf(v.e[1]),
	sinf(v.e[2]),
	sinf(v.e[3])
    };
    return(result);
#elif defined(ARM)

#elif defined(SSE)
    v4 x = VectorModAngles(v);

    m128 sign = _mm_and_ps(x.smv, FM_NEGATIVE_ZERO.smv);
    m128 c = _mm_or_ps(FM_PI_V.smv, sign);
    m128 absx = _mm_andnot_ps(sign, x.smv);
    m128 rflx = _mm_sub_ps(c, x.smv);
    m128 comp = _mm_cmple_ps(absx, FM_HALFPI.smv);
    m128 select0 = _mm_and_ps(comp, x.smv);
    m128 select1 = _mm_andnot_ps(comp, rflx);
    x.smv = _mm_or_ps(select0, select1);

    m128 x2 = _mm_mul_ps(x.smv, x.smv);

    v4 sc1 = FM_SIN_COEFFICIENTS_1;
    m128 vConstantsB = FM_PERMUTE_PS(sc1.smv, _MM_SHUFFLE(0, 0, 0, 0));
    v4 sc0 = FM_SIN_COEFFICIENTS_0;
    m128 vConstants = FM_PERMUTE_PS(sc0.smv, _MM_SHUFFLE(3, 3, 3, 3));
    m128 result = FM_FMADD_PS(vConstantsB, x2, vConstants);

    vConstants = FM_PERMUTE_PS(sc0.smv, _MM_SHUFFLE(2, 2, 2, 2));
    result = FM_FMADD_PS(result, x2, vConstants);

    vConstants = FM_PERMUTE_PS(sc0.smv, _MM_SHUFFLE(1, 1, 1, 1));
    result = FM_FMADD_PS(result, x2, vConstants);

    vConstants = FM_PERMUTE_PS(sc0.smv, _MM_SHUFFLE(0, 0, 0, 0));
    result = FM_FMADD_PS(result, x2, vConstants);

    result = FM_FMADD_PS(result, x2, FM_ONE.smv);
    result = _mm_mul_ps(result, x.smv);

    v4 vRes = {};
    vRes.smv = result;
    return(vRes);
#endif    
}


inline void FCALL VectorSinCos
(
    v4* pSin,
    v4* pCos,
    v4 v
 )
{
#if NO_INTRINSICS
    v4 sin =
    {
	sinf(v.e[0]),
	sinf(v.e[1]),
	sinf(v.e[2]),
	sinf(v.e[3])
    };

    v4 cos =
    {
	cosf(v.e[0]),
	cosf(v.e[1]),
	cosf(v.e[2]),
	cosf(v.e[3])
    };

    *pSin = sin;
    *pCos = cos;
#elif defined(ARM)

#elif defined(SSE)
    v4 x = VectorModAngles(v);

    m128 sign = _mm_and_ps(x.smv, FM_NEGATIVE_ZERO.smv);
    m128 c = _mm_or_ps(FM_PI_V.smv, sign);
    m128 absx = _mm_andnot_ps(sign, x.smv);
    m128 rflx = _mm_sub_ps(c, x.smv);
    m128 comp = _mm_cmple_ps(absx, FM_HALFPI.smv);
    m128 select0 = _mm_and_ps(comp, x.smv);
    m128 select1 = _mm_andnot_ps(comp, rflx);
    x.smv = _mm_or_ps(select0, select1);
    select0 = _mm_and_ps(comp, FM_ONE.smv);
    select1 = _mm_andnot_ps(comp, FM_NEGATIVE_ONE.smv);
    sign = _mm_or_ps(select0, select1);

    m128 x2 = _mm_mul_ps(x.smv, x.smv);

    v4 sc1 = FM_SIN_COEFFICIENTS_1;
    m128 vConstantsB = FM_PERMUTE_PS(sc1.smv, _MM_SHUFFLE(0, 0, 0, 0));
    v4 sc0 = FM_SIN_COEFFICIENTS_0;
    m128 vConstants = FM_PERMUTE_PS(sc0.smv, _MM_SHUFFLE(3, 3, 3, 3));
    m128 result = FM_FMADD_PS(vConstantsB, x2, vConstants);

    vConstants = FM_PERMUTE_PS(sc0.smv, _MM_SHUFFLE(2, 2, 2, 2));
    result = FM_FMADD_PS(result, x2, vConstants);

    vConstants = FM_PERMUTE_PS(sc0.smv, _MM_SHUFFLE(1, 1, 1, 1));
    result = FM_FMADD_PS(result, x2, vConstants);

    vConstants = FM_PERMUTE_PS(sc0.smv, _MM_SHUFFLE(0, 0, 0, 0));
    result = FM_FMADD_PS(result, x2, vConstants);

    result = FM_FMADD_PS(result, x2, FM_ONE.smv);
    result = _mm_mul_ps(result, x.smv);
    v4 resV = {};
    resV.smv = result;
    *pSin = resV;

    v4 cc1 = FM_COS_COEFFICIENTS_1;
    vConstantsB = FM_PERMUTE_PS(cc1.smv, _MM_SHUFFLE(0, 0, 0, 0));
    v4 cc0 = FM_COS_COEFFICIENTS_0;
    vConstants = FM_PERMUTE_PS(cc0.smv, _MM_SHUFFLE(3, 3, 3, 3));
    result = FM_FMADD_PS(vConstantsB, x2, vConstants);

    vConstants = FM_PERMUTE_PS(cc0.smv, _MM_SHUFFLE(2, 2, 2, 2));
    result = FM_FMADD_PS(result, x2, vConstants);

    vConstants = FM_PERMUTE_PS(cc0.smv, _MM_SHUFFLE(1, 1, 1, 1));
    result = FM_FMADD_PS(result, x2, vConstants);

    vConstants = FM_PERMUTE_PS(cc0.smv, _MM_SHUFFLE(0, 0, 0, 0));
    result = FM_FMADD_PS(result, x2, vConstants);

    result = FM_FMADD_PS(result, x2, FM_ONE.smv);
    result = _mm_mul_ps(result, sign);

    resV.smv = result;
    *pCos = resV;
#endif    
}

inline v4 Lerp(v4 a, v4 b, r32 t)
{
#if NO_INTRINSICS
    v4 result = a + (b - a) * t;
#elif defined(ARM)

#elif defined(SSE)
    m128 L = _mm_sub_ps(b.smv, a.smv);
    m128 S = _mm_set_ps1(t);
    v4 result = {};
    result.smv = FM_FMADD_PS(L, S, a.smv);
    return(result);
#endif    
}

inline v4 FCALL
VectorExp(v4 v)
{
#if NO_INTRINSICS    
    v4 result =
    {
	exp2f(v.e[0]),
	exp2f(v.e[1]),
	exp2f(v.e[2]),
	exp2f(v.e[3]),	
    };
    return(result);

#elif defined(ARM)

#elif defined(SSE)
    __m128i itrunc = _mm_cvttps_epi32(v.smv);
    m128 ftrunc = _mm_cvtepi32_ps(itrunc);
    m128 y = _mm_sub_ps(v.smv, ftrunc);

    m128 poly = FM_FMADD_PS(FM_EXP_EST_7.smv, y, FM_EXP_EST_6.smv);
    poly = FM_FMADD_PS(poly, y, FM_EXP_EST_5.smv);
    poly = FM_FMADD_PS(poly, y, FM_EXP_EST_4.smv); 
    poly = FM_FMADD_PS(poly, y, FM_EXP_EST_3.smv);
    poly = FM_FMADD_PS(poly, y, FM_EXP_EST_2.smv);
    poly = FM_FMADD_PS(poly, y, FM_EXP_EST_1.smv);
    poly = FM_FMADD_PS(poly, y, FM_ONE.smv);    

    __m128i biased = _mm_add_epi32(itrunc, FM_EXPONENT_BIAS.iSmv);
    biased = _mm_slli_epi32(biased, 23);
    m128 result0 = _mm_div_ps(_mm_castsi128_ps(biased), poly);

    biased = _mm_add_epi32(itrunc, FM_253.iSmv);
    biased = _mm_slli_epi32(biased, 23);
    m128 result1 = _mm_mul_ps(FM_MIN_NORMAL.smv, result1);
    // Use selection to handle the cases
    //  if (V is NaN) -> QNaN;
    //  else if (V sign bit set)
    //      if (V > -150)
    //         if (V.exponent < -126) -> result1
    //         else -> result0
    //      else -> +0
    //  else
    //      if (V < 128) -> result0
    //      else -> +inf

    __m128i comp = _mm_cmplt_epi32(_mm_castps_si128(v.smv), FM_BIN_128.iSmv);
    __m128i select0 = _mm_and_si128(comp, _mm_castps_si128(result0));
    __m128i select1 = _mm_andnot_si128(comp, FM_INF.iSmv);
    __m128i result2 = _mm_or_si128(select0, select1);

    comp = _mm_cmplt_epi32(itrunc, FM_SUBNORMAL_EXPONENT.iSmv);
    select1 = _mm_and_si128(comp, _mm_castps_si128(result1));
    select0 = _mm_andnot_si128(comp, _mm_castps_si128(result0));
    __m128i result3 = _mm_or_si128(select0, select1);

    comp = _mm_cmplt_epi32(_mm_castps_si128(v.smv), FM_BINNEG_150.iSmv);
    select0 = _mm_and_si128(comp, result3);
    select1 = _mm_andnot_si128(comp, FM_ZERO.iSmv);
    __m128i result4 = _mm_or_si128(select0, select1);

    __m128i sign = _mm_and_si128(_mm_castps_si128(v.smv), FM_NEGATIVE_ZERO.iSmv);
    comp = _mm_cmpeq_epi32(sign, FM_NEGATIVE_ZERO.iSmv);
    select0 = _mm_and_si128(comp, result4);
    select1 = _mm_andnot_si128(comp, result2);
    __m128i result5 = _mm_or_si128(select0, select1);

    __m128i t0 = _mm_and_si128(_mm_castps_si128(v.smv), FM_QNAN_TEST.iSmv);
    __m128i t1 = _mm_and_si128(_mm_castps_si128(v.smv), FM_INF.iSmv);

    t0 = _mm_cmpeq_epi32(t0, FM_ZERO.iSmv);
    t1 = _mm_cmpeq_epi32(t1, FM_INF.iSmv);

    __m128i isNaN = _mm_andnot_si128(t0, t1);

    select0 = _mm_and_si128(isNaN, FM_QNaN.iSmv);
    select1 = _mm_andnot_si128(isNaN, result5);
    __m128i tResult = _mm_or_si128(select0, select1);

    v4 result = {};
    result.smv = _mm_castsi128_ps(tResult);
    return(result);
#endif
}


/*
****************************M4*******************************
 */

struct alignas(16) m4
{
    union
    {
	struct
	{
	    v4 r1, r2, r3, r4;
	};
	v4 r[4];
	struct
	{
	    m128 _r1, _r2, _r3, _r4;
	};
	m128 smv[4];
	
	struct
	{
	    r32 m11, m12, m13, m14;
	    r32 m21, m22, m23, m24;
	    r32 m31, m32, m33, m34;
	    r32 m41, m42, m43, m44;
	};

	r32 e[4][4];
    };

    inline m4 &operator*=(m4);
    inline m4 &operator*=(r32);
    inline m4 &operator+=(m4);
    inline m4 &operator-=(m4);
};

inline m4 FCALL
operator*(m4 a, m4 b)
{
#if NO_INTRINSICS
    m4 result = {};
    
    r32 x = a.e[0][0];
    r32 y = a.e[0][1];
    r32 z = a.e[0][2];
    r32 w = a.e[0][3];

    //First row
    result.e[0][0] = (b.m[0][0] * x) + (b.m[1][0] * y) + (b.m[2][0] * z) + (b.m[3][0] * w);
    result.e[0][1] = (b.m[0][1] * x) + (b.m[1][1] * y) + (b.m[2][1] * z) + (b.m[3][1] * w);
    result.e[0][2] = (b.m[0][2] * x) + (b.m[1][2] * y) + (b.m[2][2] * z) + (b.m[3][2] * w);
    result.e[0][3] = (b.m[0][3] * x) + (b.m[1][3] * y) + (b.m[2][3] * z) + (b.m[3][3] * w);

    //Second row
    x = a.e[1][0];
    y = a.e[1][1];
    z = a.e[1][2];
    w = a.e[1][3];
    result.e[1][0] = (b.m[0][0] * x) + (b.m[1][0] * y) + (b.m[2][0] * z) + (b.m[3][0] * w);
    result.e[1][1] = (b.m[0][1] * x) + (b.m[1][1] * y) + (b.m[2][1] * z) + (b.m[3][1] * w);
    result.e[1][2] = (b.m[0][2] * x) + (b.m[1][2] * y) + (b.m[2][2] * z) + (b.m[3][2] * w);
    result.e[1][3] = (b.m[0][3] * x) + (b.m[1][3] * y) + (b.m[2][3] * z) + (b.m[3][3] * w);

    //Third row
    x = a.e[2][0];
    y = a.e[2][1];
    z = a.e[2][2];
    w = a.e[2][3];
    result.e[2][0] = (b.m[0][0] * x) + (b.m[1][0] * y) + (b.m[2][0] * z) + (b.m[3][0] * w);
    result.e[2][1] = (b.m[0][1] * x) + (b.m[1][1] * y) + (b.m[2][1] * z) + (b.m[3][1] * w);
    result.e[2][2] = (b.m[0][2] * x) + (b.m[1][2] * y) + (b.m[2][2] * z) + (b.m[3][2] * w);
    result.e[2][3] = (b.m[0][3] * x) + (b.m[1][3] * y) + (b.m[2][3] * z) + (b.m[3][3] * w);    

    //Fourth row
    x = a.e[3][0];
    y = a.e[3][1];
    z = a.e[3][2];
    w = a.e[3][3];
    result.e[3][0] = (b.m[0][0] * x) + (b.m[1][0] * y) + (b.m[2][0] * z) + (b.m[3][0] * w);
    result.e[3][1] = (b.m[0][1] * x) + (b.m[1][1] * y) + (b.m[2][1] * z) + (b.m[3][1] * w);
    result.e[3][2] = (b.m[0][2] * x) + (b.m[1][2] * y) + (b.m[2][2] * z) + (b.m[3][2] * w);
    result.e[3][3] = (b.m[0][3] * x) + (b.m[1][3] * y) + (b.m[2][3] * z) + (b.m[3][3] * w);    

    return(result);
#elif defined(ARM)

#elif defined(SSE)
    m4 result;

    //Splat component x, y, z then w
    v4 vw, vx, vy, vz;
    vw = a.r[0];
    vx.smv = FM_PERMUTE_PS(vw.smv, _MM_SHUFFLE(0, 0, 0, 0));
    vy.smv = FM_PERMUTE_PS(vw.smv, _MM_SHUFFLE(1, 1, 1, 1));
    vz.smv = FM_PERMUTE_PS(vw.smv, _MM_SHUFFLE(2, 2, 2, 2));
    vw.smv = FM_PERMUTE_PS(vw.smv, _MM_SHUFFLE(3, 3, 3, 3));

    //Perform the operation on the first row
    vx.smv = _mm_mul_ps(vx.smv, b.smv[0]);
    vy.smv = _mm_mul_ps(vy.smv, b.smv[1]);
    vz.smv = _mm_mul_ps(vz.smv, b.smv[2]);
    vw.smv = _mm_mul_ps(vw.smv, b.smv[3]);
    //Perform binary add to reduce cumulative errors
    vx.smv = _mm_add_ps(vx.smv, vz.smv);
    vy.smv = _mm_add_ps(vy.smv, vw.smv);
    vx.smv = _mm_add_ps(vx.smv, vy.smv);
    result.smv[0] = vx.smv;

    //Second row
    vw = a.r[1];    
    vx.smv = FM_PERMUTE_PS(vw.smv, _MM_SHUFFLE(0, 0, 0, 0));
    vy.smv = FM_PERMUTE_PS(vw.smv, _MM_SHUFFLE(1, 1, 1, 1));
    vz.smv = FM_PERMUTE_PS(vw.smv, _MM_SHUFFLE(2, 2, 2, 2));
    vw.smv = FM_PERMUTE_PS(vw.smv, _MM_SHUFFLE(3, 3, 3, 3));
    
    vx.smv = _mm_mul_ps(vx.smv, b.smv[0]);
    vy.smv = _mm_mul_ps(vy.smv, b.smv[1]);
    vz.smv = _mm_mul_ps(vz.smv, b.smv[2]);
    vw.smv = _mm_mul_ps(vw.smv, b.smv[3]);
    //Perform binary add to reduce cumulative errors
    vx.smv = _mm_add_ps(vx.smv, vz.smv);
    vy.smv = _mm_add_ps(vy.smv, vw.smv);
    vx.smv = _mm_add_ps(vx.smv, vy.smv);
    result.smv[1] = vx.smv;

    //Third row
    vw = a.r[2];    
    vx.smv = FM_PERMUTE_PS(vw.smv, _MM_SHUFFLE(0, 0, 0, 0));
    vy.smv = FM_PERMUTE_PS(vw.smv, _MM_SHUFFLE(1, 1, 1, 1));
    vz.smv = FM_PERMUTE_PS(vw.smv, _MM_SHUFFLE(2, 2, 2, 2));
    vw.smv = FM_PERMUTE_PS(vw.smv, _MM_SHUFFLE(3, 3, 3, 3));
    
    vx.smv = _mm_mul_ps(vx.smv, b.smv[0]);
    vy.smv = _mm_mul_ps(vy.smv, b.smv[1]);
    vz.smv = _mm_mul_ps(vz.smv, b.smv[2]);
    vw.smv = _mm_mul_ps(vw.smv, b.smv[3]);
    //Perform binary add to reduce cumulative errors
    vx.smv = _mm_add_ps(vx.smv, vz.smv);
    vy.smv = _mm_add_ps(vy.smv, vw.smv);
    vx.smv = _mm_add_ps(vx.smv, vy.smv);
    result.smv[2] = vx.smv;

    //Fourth row
    vw = a.r[3];    
    vx.smv = FM_PERMUTE_PS(vw.smv, _MM_SHUFFLE(0, 0, 0, 0));
    vy.smv = FM_PERMUTE_PS(vw.smv, _MM_SHUFFLE(1, 1, 1, 1));
    vz.smv = FM_PERMUTE_PS(vw.smv, _MM_SHUFFLE(2, 2, 2, 2));
    vw.smv = FM_PERMUTE_PS(vw.smv, _MM_SHUFFLE(3, 3, 3, 3));
    
    vx.smv = _mm_mul_ps(vx.smv, b.smv[0]);
    vy.smv = _mm_mul_ps(vy.smv, b.smv[1]);
    vz.smv = _mm_mul_ps(vz.smv, b.smv[2]);
    vw.smv = _mm_mul_ps(vw.smv, b.smv[3]);
    //Perform binary add to reduce cumulative errors
    vx.smv = _mm_add_ps(vx.smv, vz.smv);
    vy.smv = _mm_add_ps(vy.smv, vw.smv);
    vx.smv = _mm_add_ps(vx.smv, vy.smv);
    result.smv[3] = vx.smv;

    return(result);
#endif    
}

inline v4 FCALL
Vector3Transform(v4 v, m4 m)
{
#if NO_INTRINSICS
    
#elif defined(ARM)

#elif defined(SSE)
    m128 mResult = FM_PERMUTE_PS(v.smv, _MM_SHUFFLE(2, 2, 2, 2));
    mResult = FM_FMADD_PS(mResult, m.r[2].smv, m.r[3].smv);
    m128 temp = FM_PERMUTE_PS(v.smv, _MM_SHUFFLE(1, 1, 1, 1));
    mResult = FM_FMADD_PS(temp, m.r[1].smv, mResult);
    temp = FM_PERMUTE_PS(v.smv, _MM_SHUFFLE(0, 0, 0, 0));
    mResult = FM_FMADD_PS(temp, m.r[0].smv, mResult);
    v4 vResult = {};
    vResult.smv = mResult;
    return(vResult);
#endif    
}

inline m4 FCALL
Identity(void)
{
    m4 m = {};
    m.r[0] = FM_IDENTITY_R0;
    m.r[1] = FM_IDENTITY_R1;
    m.r[2] = FM_IDENTITY_R2;
    m.r[3] = FM_IDENTITY_R3;
    return(m);
}

inline m4 FCALL
SetMatrix
(
    r32 m00, r32 m01, r32 m02, r32 m03,
    r32 m10, r32 m11, r32 m12, r32 m13,
    r32 m20, r32 m21, r32 m22, r32 m23,
    r32 m30, r32 m31, r32 m32, r32 m33
    )
{
    m4 m;
#if NO_INTRINSICS
    m.m[0][0] = m00; m.m[0][1] = m01; m.m[0][2] = m02; m.m[0][3] = m03;
    m.m[1][0] = m10; m.m[1][1] = m11; m.m[1][2] = m12; m.m[1][3] = m13;
    m.m[2][0] = m20; m.m[2][1] = m21; m.m[2][2] = m22; m.m[2][3] = m23;
    m.m[3][0] = m30; m.m[3][1] = m31; m.m[3][2] = m32; m.m[3][3] = m33;    
#else
    m.r[0] = SetV4(m00, m01, m02, m03);
    m.r[1] = SetV4(m10, m11, m12, m13);
    m.r[2] = SetV4(m20, m21, m22, m23);
    m.r[3] = SetV4(m30, m31, m32, m33);    
#endif
    return(m);
}

inline m4 FCALL
TranslationM4(r32 offsetX, r32 offsetY, r32 offsetZ)
{
#if NO_INTRINSICS
    m4 m = {};
    m.m[0][0] = 1.0f;
    m.m[0][1] = 0.0f;
    m.m[0][2] = 0.0f;
    m.m[0][3] = 0.0f;

    m.m[1][0] = 0.0f;
    m.m[1][1] = 1.0f;
    m.m[1][2] = 0.0f;
    m.m[1][3] = 0.0f;

    m.m[2][0] = 0.0f;
    m.m[2][1] = 0.0f;
    m.m[2][2] = 1.0f;
    m.m[2][3] = 0.0f;

    m.m[3][0] = offsetX;
    m.m[3][1] = offsetY;
    m.m[3][2] = offsetZ;
    m.m[3][3] = 1.0f;

    return(m);
#elif defined(SSE) || defined(ARM)
    m4 m = {};

    m.r[0] = FM_IDENTITY_R0;
    m.r[1] = FM_IDENTITY_R1;
    m.r[2] = FM_IDENTITY_R2;
    m.r[3] = SetV4(offsetX, offsetY, offsetZ, 1.f);
    return(m);
#endif    
}

inline m4 FCALL
TranslationM4(v4 offset)
{
#if NO_INTRINSICS
    m4 m = {};
    m.m[0][0] = 1.0f;
    m.m[0][1] = 0.0f;
    m.m[0][2] = 0.0f;
    m.m[0][3] = 0.0f;

    m.m[1][0] = 0.0f;
    m.m[1][1] = 1.0f;
    m.m[1][2] = 0.0f;
    m.m[1][3] = 0.0f;

    m.m[2][0] = 0.0f;
    m.m[2][1] = 0.0f;
    m.m[2][2] = 1.0f;
    m.m[2][3] = 0.0f;

    m.m[3][0] = v.e[0];
    m.m[3][1] = v.e[1];
    m.m[3][2] = v.e[2];
    m.m[3][3] = 1.0f;

    return(m);
#elif defined(SSE) || defined(ARM)
    m4 m = {};

    m.r[0] = FM_IDENTITY_R0;
    m.r[1] = FM_IDENTITY_R1;
    m.r[2] = FM_IDENTITY_R2;
    m.r[3] = VectorSelect(FM_IDENTITY_R3, offset, FM_SELECT1110);
    return(m);
#endif    
}

inline m4 FCALL
ScaleM4(r32 x, r32 y, r32 z)
{
#if NO_INTRINSICS

    m4 m = {};
    m.e[0][0] = x;
    m.e[0][1] = 0.0f;
    m.e[0][2] = 0.0f;
    m.e[0][3] = 0.0f;    
    
    m.e[1][0] = 0.0f;
    m.e[1][1] = y;
    m.e[1][2] = 0.0f;
    m.e[1][3] = 0.0f;

    m.e[2][0] = 0.0f;
    m.e[2][1] = 0.0f;
    m.e[2][2] = z;
    m.e[2][3] = 0.0f;

    m.e[3][0] = 0.0f;
    m.e[3][1] = 0.0f;
    m.e[3][2] = 0.0f;
    m.e[3][3] = 1.0f;

    return(m);
#elif defined(ARM)

#elif defined(SSE)
    m4 m = {};
    m.r[0].smv = _mm_set_ps(0, 0, 0, x);
    m.r[1].smv = _mm_set_ps(0, 0, y, 0);
    m.r[0].smv = _mm_set_ps(0, z, 0, 0);
    m.r[0] = FM_IDENTITY_R3;
    return(m);
#endif    
}

inline m4 FCALL
ScaleM4(v4 scale)
{
#if NO_INTRINSICS

    m4 m = {};
    m.e[0][0] = scale.e[0];
    m.e[0][1] = 0.0f;
    m.e[0][2] = 0.0f;
    m.e[0][3] = 0.0f;    
    
    m.e[1][0] = 0.0f;
    m.e[1][1] = scale.e[1];
    m.e[1][2] = 0.0f;
    m.e[1][3] = 0.0f;

    m.e[2][0] = 0.0f;
    m.e[2][1] = 0.0f;
    m.e[2][2] = scale.e[2];
    m.e[2][3] = 0.0f;

    m.e[3][0] = 0.0f;
    m.e[3][1] = 0.0f;
    m.e[3][2] = 0.0f;
    m.e[3][3] = 1.0f;

    return(m);

#elif defined(ARM)

#elif defined(SSE)

    m4 m = {};
    m.r[0].smv = _mm_and_ps(scale.smv, FM_MASK_X.smv);
    m.r[1].smv = _mm_and_ps(scale.smv, FM_MASK_Y.smv);
    m.r[0].smv = _mm_and_ps(scale.smv, FM_MASK_Z.smv);
    m.r[0] = FM_IDENTITY_R3;
    return(m);	
    
#endif    
}

inline m4 FCALL
RotX(r32 angle)
{
#if NO_INTRINSICS
    v2 sResult = ScalarSinCos(angle);
    r32 fSinAngle = sResult.x;
    r32 fCosAngle = sResult.y;
    
    m4 m = {};

    m.e[0][0] = 1.0f;
    m.e[0][1] = 0.0f;
    m.e[0][2] = 0.0f;
    m.e[0][3] = 0.0f;    
    
    m.e[1][0] = 0.0f;
    m.e[1][1] = fCosAngle;
    m.e[1][2] = fSinAngle;
    m.e[1][3] = 0.0f;

    m.e[2][0] = 0.0f;
    m.e[2][1] = -fSinAngle;
    m.e[2][2] = fCosAngle;
    m.e[2][3] = 0.0f;

    m.e[3][0] = 0.0f;
    m.e[3][1] = 0.0f;
    m.e[3][2] = 0.0f;
    m.e[3][3] = 1.0f;

    return(m);

#elif defined(ARM)

#elif defined(SSE)
    v2 sResult = ScalarSinCos(angle);
    r32 fSinAngle = sResult.x;
    r32 fCosAngle = sResult.y;

    v4 vSin, vCos;
    vSin.smv = _mm_set_ss(fSinAngle);
    vCos.smv = _mm_set_ss(fCosAngle);
    //x = 0, y= sin, z = cos, w = 0
    vCos.smv = _mm_shuffle_ps(vCos.smv, vSin.smv, _MM_SHUFFLE(3, 0, 0, 3));
    m4 m = {};
    m.r[0] - FM_IDENTITY_R0;
    m.r[1] = vCos;
    //x = 0, y = sin, z = cos, w = 0
    vCos.smv = FM_PERMUTE_PS(vCos.smv, _MM_SHUFFLE(3, 1, 2, 0));
    //x = 0, y = -sin, z = cos, w = 0
    vCos.smv = _mm_mul_ps(vCos.smv, FM_NEGATE_Y.smv);
    m.r[2] = vCos;
    m.r[3] = FM_IDENTITY_R3;

    return(m);
#endif
}


inline m4 FCALL
RotY(r32 angle)
{
#if NO_INTRINSICS
    v2 sResult = ScalarSinCos(angle);
    r32 fSinAngle = sResult.x;
    r32 fCosAngle = sResult.y;
    
    m4 m = {};

    m.e[0][0] = fCosAngle;
    m.e[0][1] = 0.0f;
    m.e[0][2] = -fSinAngle;
    m.e[0][3] = 0.0f;    
    
    m.e[1][0] = 0.0f;
    m.e[1][1] = 1.0f;
    m.e[1][2] = 0.0f;
    m.e[1][3] = 0.0f;

    m.e[2][0] = fSinAngle;
    m.e[2][1] = 0.0f;
    m.e[2][2] = fCosAngle;
    m.e[2][3] = 0.0f;

    m.e[3][0] = 0.0f;
    m.e[3][1] = 0.0f;
    m.e[3][2] = 0.0f;
    m.e[3][3] = 1.0f;

    return(m);

#elif defined(ARM)

#elif defined(SSE)
    v2 sResult = ScalarSinCos(angle);
    r32 fSinAngle = sResult.x;
    r32 fCosAngle = sResult.y;

    v4 vSin, vCos;
    vSin.smv = _mm_set_ss(fSinAngle);
    vCos.smv = _mm_set_ss(fCosAngle);
    //x = sin, y= 0, z = cos, w = 0
    vSin.smv = _mm_shuffle_ps(vSin.smv, vCos.smv, _MM_SHUFFLE(3, 0, 3, 0));
    m4 m = {};
    m.r[2] = vSin;
    m.r[1] = FM_IDENTITY_R1;
    //x = cos, y = 0, z = sin, w = 0
    vSin.smv = FM_PERMUTE_PS(vSin.smv, _MM_SHUFFLE(3, 0, 1, 2));
    //x = cos, y = 0, z = -sin, w = 0
    vSin.smv = _mm_mul_ps(vSin.smv, FM_NEGATE_Z.smv);
    m.r[0] = vSin;
    m.r[3] = FM_IDENTITY_R3;
    return(m);
#endif
}

inline m4 FCALL
RotZ(r32 angle)
{
#if NO_INTRINSICS
    v2 sResult = ScalarSinCos(angle);
    r32 fSinAngle = sResult.x;
    r32 fCosAngle = sResult.y;
    
    m4 m = {};

    m.e[0][0] = fCosAngle;
    m.e[0][1] = fSinAngle;
    m.e[0][2] = 0.0f;
    m.e[0][3] = 0.0f;    
    
    m.e[1][0] = -fSinAngle;
    m.e[1][1] = fCosAngle;
    m.e[1][2] = 0.0f;
    m.e[1][3] = 0.0f;

    m.e[2][0] = 0.0f;
    m.e[2][1] = 0.0f;
    m.e[2][2] = 1.0f;
    m.e[2][3] = 0.0f;

    m.e[3][0] = 0.0f;
    m.e[3][1] = 0.0f;
    m.e[3][2] = 0.0f;
    m.e[3][3] = 1.0f;

    return(m);

#elif defined(ARM)

#elif defined(SSE)
    v2 sResult = ScalarSinCos(angle);
    r32 fSinAngle = sResult.x;
    r32 fCosAngle = sResult.y;

    v4 vSin, vCos;
    vSin.smv = _mm_set_ss(fSinAngle);
    vCos.smv = _mm_set_ss(fCosAngle);

    //x = cos, y = sin, z = 0, w = 0
    vCos.smv = _mm_unpacklo_ps(vCos.smv, vSin.smv);

    m4 m = {};
    m.r[0] = vCos;
    //x = sin, y = cos, z = 0, w = 0
    vCos.smv = FM_PERMUTE_PS(vCos.smv, _MM_SHUFFLE(3, 2, 0, 1));
    //x = cos, y = -sin, z = 0, w = 0
    vCos.smv = _mm_mul_ps(vCos.smv, FM_NEGATE_X.smv);
    m.r[1] = vCos;
    m.r[2] = FM_IDENTITY_R2;
    m.r[3] = FM_IDENTITY_R3;
    return(m);

#endif
}

inline m4 FCALL
Transpose(m4 m)
{
#if NO_INTRINSICS
    // Original matrix:
    //
    //     m00m01m02m03
    //     m10m11m12m13
    //     m20m21m22m23
    //     m30m31m32m33

    m4 p  = {};
    p.r[0] = MergeXY(m.r[0], m.r[2]); //m00m20m01m21
    p.r[1] = MergeXY(m.r[1], m.r[3]); //m10m30m11m31
    p.r[2] = MergeZW(m.r[0], m.r[2]); //m02m22m03m23
    p.r[3] = MergeZW(m.r[1], m.r[3]); //m12m32m13m33

    m4 mt = {};
    mt.r[0] = MergeXY(p.r[0], p.r[1]); //m00m10,20,30
    mt.r[1] = MergeZW(p.r[0], p.r[1]); //m01m11m21m31
    mt.r[2] = MergeXY(p.r[2], p.r[3]); //m02m12m22m32
    mt.r[3] = MergeZW(p.r[2], p.r[3]); //m03m13m23m33

    return(mt);

#elif defined (ARM)

#elif defined (SSE)
// Interleave the top two rows
    // temp1 = [m00, m10, m01, m11]
    __m128 temp1 = _mm_unpacklo_ps(m.r[0].smv, m.r[1].smv);
    // temp3 = [m02, m12, m03, m13]
    __m128 temp3 = _mm_unpackhi_ps(m.r[0].smv, m.r[1].smv);

    // Interleave the bottom two rows
    // temp2 = [m20, m30, m21, m31]
    __m128 temp2 = _mm_unpacklo_ps(m.r[2].smv, m.r[3].smv);
    // temp4 = [m22, m32, m23, m33]
    __m128 temp4 = _mm_unpackhi_ps(m.r[2].smv, m.r[3].smv);

    m4 result;
    // Combine the blocks into final rows
    result.r[0].smv = _mm_movelh_ps(temp1, temp2); // m00, m10, m20, m30
    result.r[1].smv = _mm_movehl_ps(temp2, temp1); // m01, m11, m21, m31
    result.r[2].smv = _mm_movelh_ps(temp3, temp4); // m02, m12, m22, m32
    result.r[3].smv = _mm_movehl_ps(temp4, temp3); // m03, m13, m23, m33

    return(result);
#endif    
}

inline m4 FCALL
LookToLH(v4 eyePos, v4 eyeDir, v4 upDir)
{
    Assert(!Vector3Equal(eyeDir, ZeroVector()));
    Assert(!Vector3IsInf(eyeDir));
    Assert(!Vector3Equal(upDir, ZeroVector()));
    Assert(!Vector3IsInf(upDir));
    
    v4 r2 = NormalizeV3(eyeDir);
    v4 r0 = CrossV3(upDir, r2);
    r0 = NormalizeV3(r0);

    v4 r1 = CrossV3(r2, r0);
    v4 negEyePos = NegateVector(eyePos);

    v4 d0 = DotV3(r0, negEyePos);
    v4 d1 = DotV3(r1, negEyePos);
    v4 d2 = DotV3(r2, negEyePos);

    m4 m = {};
    m.r[0] = VectorSelect(d0, r0, FM_SELECT1110);
    m.r[1] = VectorSelect(d1, r1, FM_SELECT1110);
    m.r[2] = VectorSelect(d2, r2, FM_SELECT1110);
    m.r[3] = FM_IDENTITY_R3;
    
    m = Transpose(m);
    return(m);
}

inline m4 FCALL
LookAtRH(v4 eyePos, v4 focusPos, v4 upDir)
{
    v4 eyeDir = eyePos - focusPos;
    return(LookToLH(eyePos, eyeDir, upDir));
}

inline m4 FCALL
PerspectiveFovRH(r32 fovY, r32 aspectR, r32 nearZ, r32 farZ)
{
    Assert(nearZ > 0.f && farZ > 0.f);
    Assert(!ScalarNearEqual(fovY, 0.0f, 0.00001f * 2.0f));
    Assert(!ScalarNearEqual(aspectR, 0.0f, 0.00001f));
    Assert(!ScalarNearEqual(farZ, nearZ, 0.00001f));

#if NO_INTRINSICS



    r32 sinFov, cosFov;
    v2 sResult = ScalarSinCos(0.5f * fovY);
    sinFov = sResult.x;
    cosFov = sResult.y;

    r32 height = cosFov / sinFov;
    r32 width = height / aspectR;
    r32 fRange = farZ / (nearZ - farZ);

    m4 m = {};

    m.e[0][0] = width;
    m.e[0][1] = 0.0f;
    m.e[0][2] = 0.0f;
    m.e[0][3] = 0.0f;    
    
    m.e[1][0] = 0.0f;
    m.e[1][1] = height;
    m.e[1][2] = 0.0f;
    m.e[1][3] = 0.0f;

    m.e[2][0] = 0.0f;
    m.e[2][1] = 0.0f;
    m.e[2][2] = fRange;
    m.e[2][3] = -1.0f;

    m.e[3][0] = 0.0f;
    m.e[3][1] = 0.0f;
    m.e[3][2] = fRange * nearZ;
    m.e[3][3] = 0.0f;

    return(m);    
#elif defined(ARM)
    
#elif defined(SSE)
    r32 sinFov, cosFov;
    v2 sResult = ScalarSinCos(0.5f * fovY);
    sinFov = sResult.x;
    cosFov = sResult.y;
    r32 fRange = farZ / (nearZ - farZ);
    r32 height = cosFov /sinFov;
    r32 width = height / aspectR;
    
    m4 m = {};
    m.r[0].smv = _mm_set_ss(width);

    m.r[1].smv = _mm_set_ss(height);
    m.r[1].smv = _mm_shuffle_ps(m.r[1].smv, m.r[1].smv, _MM_SHUFFLE(1, 1, 0, 1));

    m.r[2].smv = _mm_set_ps(-1.0f, fRange, 0.0f, 0.0f);
    m.r[3].smv = _mm_set_ps(0.0f, fRange * nearZ, 0.0f, 0.0f);
    
    
    return(m);
#endif    

    
}

inline m4 MatrixRotationQuaternion(v4 q)
{

#if NO_INTRINSICS
    r32 qx = q.x;
    r32 qxx = qx * qx;

    r32 qy = q.y;
    r32 qyy = qy * qy;

    r32 qz = q.z;
    r32 qzz = qz * qz;

    r32 qw = q.w;

    m4 m = {};

    m.e[0][0] = 1.f - 2.f * qyy - 2.f * qzz;
    m.e[0][1] = 2.f * qx * qy + 2.f * qz * qw;
    m.e[0][2] = 2.f * qx * qz - 2.f * qy * qw;
    m.e[0][3] = 0.f;

    m.e[1][0] = 2.f * qx * qy - 2.f * qz * qw;
    m.e[1][1] = 1.f - 2.f * qxx - 2.f * qzz;
    m.e[1][2] = 2.f * qy * qz + 2.f * qx * qw;
    m.e[1][3] = 0.f;

    m.e[2][0] = 2.f * qx * qz + 2.f * qy * qw;
    m.e[2][1] = 2.f * qy * qz - 2.f * qx * qw;
    m.e[2][2] = 1.f - 2.f * qxx - 2.f * qyy;
    m.e[2][3] = 0.f;

    m.e[3][0] = 0.f;
    m.e[3][1] = 0.f;
    m.e[3][2] = 0.f;
    m.e[3][3] = 1.0f;
    return(m);
#elif defined(ARM)

#elif defined(SSE)

    v4 constant1110 = v4{1.0f, 1.0f, 1.0f, 0.0f};
    
    m128 q0 = _mm_add_ps(q.smv, q.smv);
    m128 q1 = _mm_mul_ps(q.smv, q0);

    m128 v0 = FM_PERMUTE_PS(q1, _MM_SHUFFLE(3, 0, 0, 1));
    v0 = _mm_and_ps(v0, FM_MASK_3.smv);
    m128 v1 = FM_PERMUTE_PS(q1, _MM_SHUFFLE(3, 1, 2, 2));
    v1 = _mm_and_ps(v1, FM_MASK_3.smv);
    
    // This is DirectXMath's "R0" (The diagonal vector)
    m128 R0 = _mm_sub_ps(constant1110.smv, v0);
    R0 = _mm_sub_ps(R0, v1);

    v0 = FM_PERMUTE_PS(q.smv, _MM_SHUFFLE(3, 1, 0, 0));
    v1 = FM_PERMUTE_PS(q0, _MM_SHUFFLE(3, 2, 1, 2));
    v0 = _mm_mul_ps(v0, v1);

    v1 = FM_PERMUTE_PS(q.smv, _MM_SHUFFLE(3, 3, 3, 3));
    m128 v2 = FM_PERMUTE_PS(q0, _MM_SHUFFLE(3, 0, 2, 1));
    v1 = _mm_mul_ps(v1, v2);



    m128 r1 = _mm_add_ps(v0, v1);
    m128 r2 = _mm_sub_ps(v0, v1);

    v0 = _mm_shuffle_ps(r1, r2, _MM_SHUFFLE(1, 0, 2, 1));
    v0 = FM_PERMUTE_PS(v0, _MM_SHUFFLE(1, 3, 2, 0));
    v1 = _mm_shuffle_ps(r1, r2, _MM_SHUFFLE(2, 2, 0, 0));
    v1 = FM_PERMUTE_PS(v1, _MM_SHUFFLE(2, 0, 2, 0));

    q1 = _mm_shuffle_ps(R0, v0, _MM_SHUFFLE(1, 0, 3, 0));
    q1 = FM_PERMUTE_PS(q1, _MM_SHUFFLE(1, 3, 2, 0));

    m4 m = {};
    m.r[0].smv = q1;

    q1 = _mm_shuffle_ps(R0, v0, _MM_SHUFFLE(3, 2, 3, 1));
    q1 = FM_PERMUTE_PS(q1, _MM_SHUFFLE(1, 3, 0, 2));
    m.r[1].smv = q1;

    q1 = _mm_shuffle_ps(v1, R0, _MM_SHUFFLE(3, 2, 1, 0));
    m.r[2].smv = q1;
    m.r[3] = FM_IDENTITY_R3;
    return(m);
    

#endif    
    
}

inline m4 FCALL
Inverse(m4 m, v4* pDet)
{
#if NO_INTRINSICS

#elif defined(ARM)

#elif defined(SSE)
    m128 temp1 = _mm_shuffle_ps(m.r[0].smv, m.r[1].smv, _MM_SHUFFLE(1, 0, 1, 0));
    m128 temp2 = _mm_shuffle_ps(m.r[0].smv, m.r[1].smv, _MM_SHUFFLE(3, 2, 3, 2));
    m128 temp3 = _mm_shuffle_ps(m.r[2].smv, m.r[3].smv, _MM_SHUFFLE(1, 0, 1, 0));
    m128 temp4 = _mm_shuffle_ps(m.r[2].smv, m.r[3].smv, _MM_SHUFFLE(3, 2, 3, 2));

    m4 mt = {};
    mt.r[0].smv = _mm_shuffle_ps(temp1, temp2, _MM_SHUFFLE(2, 0, 2, 0));
    mt.r[1].smv = _mm_shuffle_ps(temp1, temp2, _MM_SHUFFLE(3, 1, 3, 1));
    mt.r[2].smv = _mm_shuffle_ps(temp3, temp4, _MM_SHUFFLE(2, 0, 2, 0));
    mt.r[3].smv = _mm_shuffle_ps(temp3, temp4, _MM_SHUFFLE(3, 1, 3, 1));

    m128 v00 = FM_PERMUTE_PS(mt.r[2].smv, _MM_SHUFFLE(1, 1, 0, 0));
    m128 v10 = FM_PERMUTE_PS(mt.r[3].smv, _MM_SHUFFLE(3, 2, 3, 2));
    m128 v01 = FM_PERMUTE_PS(mt.r[0].smv, _MM_SHUFFLE(1, 1, 0, 0));
    m128 v11 = FM_PERMUTE_PS(mt.r[1].smv, _MM_SHUFFLE(3, 2, 3, 2));
    m128 v02 = _mm_shuffle_ps(mt.r[2].smv, mt.r[0].smv, _MM_SHUFFLE(2, 0, 2, 0));
    m128 v12 = _mm_shuffle_ps(mt.r[3].smv, mt.r[1].smv, _MM_SHUFFLE(3, 1, 3, 1));

    m128 d0 = _mm_mul_ps(v00, v10);
    m128 d1 = _mm_mul_ps(v01, v11);
    m128 d2 = _mm_mul_ps(v02, v12);

    v00 = FM_PERMUTE_PS(mt.r[2].smv, _MM_SHUFFLE(3, 2, 3, 2));
    v10 = FM_PERMUTE_PS(mt.r[3].smv, _MM_SHUFFLE(1, 1, 0, 0));
    v01 = FM_PERMUTE_PS(mt.r[0].smv, _MM_SHUFFLE(3, 2, 3, 2));
    v11 = FM_PERMUTE_PS(mt.r[1].smv, _MM_SHUFFLE(2, 0, 2, 0));
    v02 = _mm_shuffle_ps(mt.r[2].smv, mt.r[0].smv, _MM_SHUFFLE(3, 1, 3, 1));
    v12 = _mm_shuffle_ps(mt.r[3].smv, mt.r[1].smv, _MM_SHUFFLE(2, 0, 2, 0));

    d0 = FM_FNMADD_PS(v00, v10, d0);
    d1 = FM_FNMADD_PS(v01, v11, d1);
    d2 = FM_FNMADD_PS(v02, v12, d2);

    //v11 = d0y d0w d2y d2y
    v11 = _mm_shuffle_ps(d0, d2, _MM_SHUFFLE(1, 1, 3, 1));
    v00 = FM_PERMUTE_PS(mt.r[1].smv, _MM_SHUFFLE(1, 0, 2, 1));
    v10 = _mm_shuffle_ps(v11, d0, _MM_SHUFFLE(0, 3, 0, 2));
    v01 = FM_PERMUTE_PS(mt.r[0].smv, _MM_SHUFFLE(0, 1, 0, 2));
    v11 = _mm_shuffle_ps(v11, d0, _MM_SHUFFLE(2, 1, 2, 1));

    //v13 = d1y d1w d2w d2w
    m128 v13 = _mm_shuffle_ps(d1, d2, _MM_SHUFFLE(3, 3, 3, 1));
    v02 = FM_PERMUTE_PS(mt.r[3].smv, _MM_SHUFFLE(1, 0, 2, 1));
    v12 = _mm_shuffle_ps(v13, d1, _MM_SHUFFLE(0, 3, 0, 2));
    m128 v03 = FM_PERMUTE_PS(mt.r[2].smv, _MM_SHUFFLE(0, 1, 0, 2));
    v13 = _mm_shuffle_ps(v13, d1, _MM_SHUFFLE(2, 1, 2, 1));

    m128 c0 = _mm_mul_ps(v00, v10);
    m128 c2 = _mm_mul_ps(v01, v11);
    m128 c4 = _mm_mul_ps(v02, v12);
    m128 c6 = _mm_mul_ps(v03, v13);

    //v11 =d0x, d0y, d2x, d2x
    v11 = _mm_shuffle_ps(d0, d2, _MM_SHUFFLE(0, 0, 1, 0));
    v00 = FM_PERMUTE_PS(mt.r[1].smv, _MM_SHUFFLE(2, 1, 3, 2));
    v10 = _mm_shuffle_ps(d0, v11, _MM_SHUFFLE(2, 1, 0, 3));
    v01 = FM_PERMUTE_PS(mt.r[0].smv, _MM_SHUFFLE(1, 3, 2, 3));
    v11 = _mm_shuffle_ps(d0, v11, _MM_SHUFFLE(0, 2, 1 ,2));

    //v13 d1x d1y d2z d2z
    v13 = _mm_shuffle_ps(d1, d2, _MM_SHUFFLE(2, 2, 1, 0));
    v02 = FM_PERMUTE_PS(mt.r[3].smv, _MM_SHUFFLE(2, 1, 3, 2));
    v12 = _mm_shuffle_ps(d1, v13, _MM_SHUFFLE(2, 1, 0, 3));
    v03 = FM_PERMUTE_PS(mt.r[2].smv, _MM_SHUFFLE(1, 3, 2, 3));
    v13 = _mm_shuffle_ps(d1, v13, _MM_SHUFFLE(0, 2, 1, 2));

    c0 = FM_FNMADD_PS(v00, v10, c0);
    c2 = FM_FNMADD_PS(v01, v11, c2);
    c4 = FM_FNMADD_PS(v02, v12, c4);
    c6 = FM_FNMADD_PS(v03, v13, c6);

    v00 = FM_PERMUTE_PS(mt.r[1].smv, _MM_SHUFFLE(0, 3, 0, 3));
    //v10 = d0z d0z d2x d2y
    v10 = _mm_shuffle_ps(d0, d2, _MM_SHUFFLE(1, 0, 2, 2));
    v10 = FM_PERMUTE_PS(v10, _MM_SHUFFLE(0, 2, 3, 0));
    v01 = FM_PERMUTE_PS(mt.r[0].smv, _MM_SHUFFLE(2, 0, 3, 1));
    //v11 d0x d0w d2x d2y
    v11 = _mm_shuffle_ps(d0, d2, _MM_SHUFFLE(1, 0, 3, 0));
    v11 = FM_PERMUTE_PS(v11, _MM_SHUFFLE(2, 1, 0, 3));
    v02 = FM_PERMUTE_PS(mt.r[3].smv, _MM_SHUFFLE(0, 3, 0, 3));
    //v12 = d1z d1z d2z d2w
    v12 = _mm_shuffle_ps(d1, d2, _MM_SHUFFLE(3, 2, 2, 1));
    v12 = FM_PERMUTE_PS(v12, _MM_SHUFFLE(0, 2, 3, 0));
    v03 = FM_PERMUTE_PS(mt.r[2].smv, _MM_SHUFFLE(2, 0, 3, 1));
    //v13 = d1x d1w d2z d2w
    v13 = _mm_shuffle_ps(d1, d2, _MM_SHUFFLE(3, 2, 3, 0));
    v13 = FM_PERMUTE_PS(v13, _MM_SHUFFLE(2, 1, 0 ,3));

    v00 = _mm_mul_ps(v00, v10);
    v01 = _mm_mul_ps(v01, v11);
    v02 = _mm_mul_ps(v02, v12);
    v03 = _mm_mul_ps(v03, v13);

    m128 c1 = _mm_sub_ps(c0, v00);
    c0 = _mm_add_ps(c0, v00);
    m128 c3 = _mm_add_ps(c2, v01);
    c2 = _mm_sub_ps(c2, v01);
    m128 c5 = _mm_sub_ps(c4, v02);
    c4 = _mm_add_ps(c4, v02);
    m128 c7 = _mm_add_ps(c6, v03);
    c6 = _mm_sub_ps(c6, v03);

    c0 = _mm_shuffle_ps(c0, c1, _MM_SHUFFLE(3, 1, 2, 0));
    c2 = _mm_shuffle_ps(c2, c3, _MM_SHUFFLE(3, 1, 2, 0));
    c4 = _mm_shuffle_ps(c4, c5, _MM_SHUFFLE(3, 1, 2, 0));
    c6 = _mm_shuffle_ps(c6, c7, _MM_SHUFFLE(3, 1, 2, 0));
    c0 = FM_PERMUTE_PS(c0, _MM_SHUFFLE(3, 2, 1, 0));
    c2 = FM_PERMUTE_PS(c2, _MM_SHUFFLE(3, 2, 1, 0));
    c4 = FM_PERMUTE_PS(c4, _MM_SHUFFLE(3, 2, 1, 0));
    c6 = FM_PERMUTE_PS(c6, _MM_SHUFFLE(3, 2, 1, 0));

    //Get the determinant
    v4 c0v = {};
    c0v.smv = c0;
    v4 temp = DotV4(c0v, mt.r[0]);
    if (pDet !=  nullptr)
	*pDet = temp;

    temp.smv = _mm_div_ps(FM_ONE.smv, temp.smv);
    m4 result = {};
    result.r[0].smv = _mm_mul_ps(c0, temp.smv);
    result.r[1].smv = _mm_mul_ps(c2, temp.smv);
    result.r[2].smv = _mm_mul_ps(c4, temp.smv);
    result.r[3].smv = _mm_mul_ps(c6, temp.smv);
    return(result);
#endif    
}

/*
************Quaternions***********
 */

inline v4 FCALL
QuaternionIdentity()
{
    v4 result = FM_IDENTITY_R3;
    return(result);
}

inline v4 FCALL QuaternionRotationNormal(v4 normalAxis, r32 angle)
{
#if NO_INTRINSICS
    v4 n = VectorSelect(FM_ONE, normalAxis, FM_SELECT1110);
    r32 sinV, cosV;

    v2 sResult = ScalarSinCos(0.5f * angle);
    sinV = sResult.x;
    cosV = sResult.y;

    v4 scale = v4{sinV, sinV, sinV, cosV};
    return(n * scale);
#elif defined(ARM)

#elif defined(SSE)
    m128 n = _mm_and_ps(normalAxis.smv, FM_MASK_3.smv);
    n = _mm_or_ps(n, FM_IDENTITY_R3.smv);
    m128 scale = _mm_set_ps1(0.5f * angle);
    v4 vSine = {};
    v4 vCosine = {};
    v4 vScale = {};
    vScale.smv = scale;
    VectorSinCos(&vSine, &vCosine, vScale);
    scale = _mm_and_ps(vSine.smv, FM_MASK_3.smv);
    vCosine.smv = _mm_and_ps(vCosine.smv, FM_MASK_W.smv);
    scale = _mm_or_ps(scale, vCosine.smv);
    n = _mm_mul_ps(n, scale);
    v4 result = {};
    result.smv = n;
    return(result);
#endif    
}
//YOU HAVE NOT CHECKED ANY OF THESE FUNCTIONS ^^^^^^^

inline v4 FCALL QuaternionRotationAxis(v4 axis, r32 angle)
{
    Assert(!Vector3Equal(axis, ZeroVector()));
    Assert(!Vector3IsInf(axis));

    v4 normal = NormalizeV4(axis);
    v4 q = QuaternionRotationNormal(normal, angle);
    return(q);
}

inline v4 FCALL QuaternionNormalize(v4 q)
{
    return NormalizeV4(q);
}

inline v4 FCALL QuaternionRotationMatrix(m4 m)
{
#if NO_INTRINSICS

    v4 q = {};
    r32 r22 = m.e[2][2];
    if (r22 <= 0.f) //x^2 + y^2 >= z^2 + w^2
    {
	r32 dif10 = m.e[1][1] - m.e[0][0];
	r32 omr22 = 1.f - r22;
	if (dif10 <= 0.f) //x^2 >= y^2
	{
	    r32 fourXSqr = omr22 - dif10;
	    r32 inv4x = 0.5f / sqrtf(fourXSqr);
	    q.e[0] = fourXSqr * inv4x;
	    q.e[1] = (m.e[0][1] + m.e[1][0]) * inv4x;
	    q.e[2] = (m.e[0][2] + m.e[2][0]) * inv4x;
	    q.e[3] = (m.e[1][2] - m.e[2][1]) * inv4x;

	}
	else //y^2 >= x^2
	{
	    r32 fourYSqr = omr22 + dif10;
	    r32 inv4y = 0.5f / sqrtf(fourYSqr);
	    q.e[0] = (m.e[0][1] + m.e[1][0]) * inv4y;
	    q.e[1] = fourYSqr * inv4y;
	    q.e[2] = (m.e[1][2] + m.e[2][1]) * inv4y;
	    q.e[3] = (m.e[2][0] - m.e[0][2]) * inv4y;
	}
    }
    else //z^2 + w^2 >= x^2 + y^2
    {
	r32 sum10 = m.e[1][1] + m.e[0][0];
	r32 opr22 = 1.f + r22;
	if (sum10 < 0.f) //z^2 >= w^2
	{
	    r32 fourZSqr = opr22 - sum10;
	    r32 inv4z = 0.5f / sqrtf(fourZSqr);
	    q.e[0] = (m.e[0][2] + m.e[2][0]) * inv4z;
	    q.e[1] = (m.e[1][2] + m.e[2][1]) * inv4z;
	    q.e[2] = fourZSqr * inv4z;
	    q.e[3] = (m.e[1][0] - m.e[0][1]) * inv4z;
	}
	else //w^2 >= z^2
	{
	    r32 fourWSqr = opr22 + sum10;
	    r32 inv4w = 0.5f / sqrtf(fourWSqr);
	    q.e[0] = (m.e[1][2] - m.e[2][1]) * inv4w;
	    q.e[1] = (m.e[2][0] - m.e[0][2]) * inv4w;
	    q.e[2] = (m.e[0][1] - m.e[1][0]) * inv4w;
	    q.e[3] = fourWSqr * inv4w;
	}
    }
    return(q);
    
#elif defined(ARM)

#elif defined(SSE)
#if 1
    v4 XMPMMP = {+1.0f, -1.0f, -1.0f, +1.0f};
    v4 XMMPMP = {-1.0f, +1.0f, -1.0f, +1.0f};
    v4 XMMMPP = {-1.0f, -1.0f, +1.0f, +1.0f};

    m128 r0 = m.r[0].smv;
    m128 r1 = m.r[1].smv;
    m128 r2 = m.r[2].smv;

    m128 r00 = FM_PERMUTE_PS(r0, _MM_SHUFFLE(0, 0, 0, 0));
    m128 r11 = FM_PERMUTE_PS(r1, _MM_SHUFFLE(1, 1, 1, 1));
    m128 r22 = FM_PERMUTE_PS(r2, _MM_SHUFFLE(2, 2, 2, 2));

    m128 r11mr00 = _mm_sub_ps(r11, r00);
    m128 x2gey2 = _mm_cmple_ps(r11mr00, FM_ZERO.smv);

    m128 r11pr00 = _mm_add_ps(r11, r00);
    m128 z2gew2 = _mm_cmple_ps(r11pr00, FM_ZERO.smv);

    m128 x2py2gez2pw2 = _mm_cmple_ps(r22, FM_ZERO.smv);

    m128 t0 = FM_FMADD_PS(XMPMMP.smv, r00, FM_ONE.smv);
    m128 t1 = _mm_mul_ps(XMMPMP.smv, r11);
    m128 t2 = FM_FMADD_PS(XMMMPP.smv, r22, t0);
    m128 x2y2z2w2 = _mm_add_ps(t1, t2);

    t0 = _mm_shuffle_ps(r0, r1, _MM_SHUFFLE(1, 2, 2, 1));
    t1 = _mm_shuffle_ps(r1, r2, _MM_SHUFFLE(1, 0, 0, 0));
    t1 = FM_PERMUTE_PS(t1, _MM_SHUFFLE(1, 3, 2, 0));
    m128 xyxzyz = _mm_add_ps(t0, t1);

    t0 = _mm_shuffle_ps(r2, r1, _MM_SHUFFLE(0, 0, 0, 1));
    t1 = _mm_shuffle_ps(r1, r0, _MM_SHUFFLE(1, 2, 2, 2));
    t1 = FM_PERMUTE_PS(t1, _MM_SHUFFLE(1, 3, 2, 0));
    m128 xwywzw = _mm_sub_ps(t0, t1);
    xwywzw = _mm_mul_ps(XMMPMP.smv, xwywzw);

    t0 = _mm_shuffle_ps(x2y2z2w2, xyxzyz, _MM_SHUFFLE(0, 0, 1, 0));
    t1 = _mm_shuffle_ps(x2y2z2w2, xwywzw, _MM_SHUFFLE(0, 2, 3, 2));
    t2 = _mm_shuffle_ps(xyxzyz, xwywzw, _MM_SHUFFLE(1, 0, 2, 1));

    m128 tensor0 = _mm_shuffle_ps(t0, t2, _MM_SHUFFLE(2, 0, 2, 0));
    m128 tensor1 = _mm_shuffle_ps(t0, t2, _MM_SHUFFLE(3, 1, 1, 2));
    m128 tensor2 = _mm_shuffle_ps(t2, t1, _MM_SHUFFLE(2, 0, 1, 0));
    m128 tensor3 = _mm_shuffle_ps(t2, t1, _MM_SHUFFLE(1, 2, 3, 2));

    t0 = _mm_and_ps(x2gey2, tensor0);
    t1 = _mm_andnot_ps(x2gey2, tensor1);
    t0 = _mm_or_ps(t0, t1);
    t1 = _mm_and_ps(z2gew2, tensor2);
    t2 = _mm_andnot_ps(z2gew2, tensor3);
    t1 = _mm_or_ps(t1, t2);
    t0 = _mm_and_ps(x2py2gez2pw2, t0);
    t1 = _mm_andnot_ps(x2py2gez2pw2, t1);
    t2 = _mm_or_ps(t0, t1);


    v4 tempT2 = {};
    tempT2.smv = t2;
    v4 tempT0 = {};
    tempT0 = Vector4Length(tempT2);
    v4 result = {};
    result.smv = _mm_div_ps(t2, tempT0.smv);
    return(result);
#else
// DirectXMath SSE implementation strategy uses sign control arrays to find the trace
    m128 r0 = m.r[0].smv;
    m128 r1 = m.r[1].smv;
    m128 r2 = m.r[2].smv;

    // Standard DirectXMath sign vector templates
    const m128 jXMPMMP = _mm_set_ps(1.0f, -1.0f, -1.0f, 1.0f);
    const m128 jXMMPMP = _mm_set_ps(1.0f, -1.0f, 1.0f, -1.0f);
    const m128 jXMMMPP = _mm_set_ps(1.0f, 1.0f, -1.0f, -1.0f);

    m128 r00 = _mm_shuffle_ps(r0, r0, _MM_SHUFFLE(0, 0, 0, 0));
    m128 r11 = _mm_shuffle_ps(r1, r1, _MM_SHUFFLE(1, 1, 1, 1));
    m128 r22 = _mm_shuffle_ps(r2, r2, _MM_SHUFFLE(2, 2, 2, 2));

    m128 r11mr00 = _mm_sub_ps(r11, r00);
    m128 x2gey2 = _mm_cmple_ps(r11mr00, _mm_setzero_ps());
    m128 r11pr00 = _mm_add_ps(r11, r00);
    m128 z2gew2 = _mm_cmple_ps(r11pr00, _mm_setzero_ps());
    m128 x2py2gez2pw2 = _mm_cmple_ps(r22, _mm_setzero_ps());

    m128 t0 = _mm_add_ps(_mm_mul_ps(jXMPMMP, r00), _mm_set1_ps(1.0f));
    m128 t1 = _mm_mul_ps(jXMMPMP, r11);
    m128 t2 = _mm_add_ps(_mm_mul_ps(jXMMMPP, r22), t0);
    m128 x2y2z2w2 = _mm_add_ps(t1, t2);

    // Reconstruct cross-product terms safely matching DirectXMath unpacking layouts
    m128 s0 = _mm_shuffle_ps(r0, r1, _MM_SHUFFLE(1, 2, 2, 1));
    m128 s1 = _mm_shuffle_ps(r1, r2, _MM_SHUFFLE(1, 0, 0, 0));
    s1 = _mm_shuffle_ps(s1, s1, _MM_SHUFFLE(1, 3, 2, 0));
    m128 xyxzyz = _mm_add_ps(s0, s1);

    m128 s2 = _mm_shuffle_ps(r0, r1, _MM_SHUFFLE(2, 0, 0, 2));
    s2 = _mm_shuffle_ps(s2, s2, _MM_SHUFFLE(1, 3, 0, 2));
    m128 s3 = _mm_shuffle_ps(r2, r2, _MM_SHUFFLE(0, 0, 1, 1));
    s3 = _mm_shuffle_ps(s3, s3, _MM_SHUFFLE(2, 0, 1, 3));
    m128 xwywzw = _mm_sub_ps(s2, s3);

    // Build the selection vectors
    m128 tensor0 = _mm_shuffle_ps(x2y2z2w2, xyxzyz, _MM_SHUFFLE(0, 0, 1, 0));
    tensor0 = _mm_shuffle_ps(tensor0, xwywzw, _MM_SHUFFLE(0, 1, 2, 0)); // row 0: x dominant

    m128 tensor1 = _mm_shuffle_ps(xyxzyz, x2y2z2w2, _MM_SHUFFLE(1, 1, 0, 0));
    tensor1 = _mm_shuffle_ps(tensor1, xwywzw, _MM_SHUFFLE(1, 2, 2, 0)); // row 1: y dominant

    m128 tensor2 = _mm_shuffle_ps(xyxzyz, xyxzyz, _MM_SHUFFLE(2, 2, 1, 1));
    tensor2 = _mm_shuffle_ps(tensor2, x2y2z2w2, _MM_SHUFFLE(2, 2, 0, 1));
    tensor2 = _mm_shuffle_ps(tensor2, xwywzw, _MM_SHUFFLE(2, 0, 2, 0)); // row 2: z dominant

    m128 tensor3 = _mm_shuffle_ps(xwywzw, xwywzw, _MM_SHUFFLE(1, 2, 0, 0));
    tensor3 = _mm_shuffle_ps(tensor3, x2y2z2w2, _MM_SHUFFLE(3, 3, 0, 2));
    tensor3 = _mm_shuffle_ps(tensor3, tensor3, _MM_SHUFFLE(2, 3, 1, 0)); // row 3: w dominant

    // Perform conditional mask selects matching your layout sequence
    t0 = _mm_or_ps(_mm_and_ps(x2gey2, tensor0), _mm_andnot_ps(x2gey2, tensor1));
    t1 = _mm_or_ps(_mm_and_ps(z2gew2, tensor2), _mm_andnot_ps(z2gew2, tensor3));
    t2 = _mm_or_ps(_mm_and_ps(x2py2gez2pw2, t0), _mm_andnot_ps(x2py2gez2pw2, t1));

    // Fix the normalization step: Unpack the primary squared length element 
    m128 lengthSqr = _mm_shuffle_ps(x2y2z2w2, x2y2z2w2, _MM_SHUFFLE(0, 0, 0, 0));
    t0 = _mm_or_ps(_mm_and_ps(x2gey2, lengthSqr), _mm_andnot_ps(x2gey2, _mm_shuffle_ps(x2y2z2w2, x2y2z2w2, _MM_SHUFFLE(1, 1, 1, 1))));
    t1 = _mm_or_ps(_mm_and_ps(z2gew2, _mm_shuffle_ps(x2y2z2w2, x2y2z2w2, _MM_SHUFFLE(2, 2, 2, 2))), _mm_andnot_ps(z2gew2, _mm_shuffle_ps(x2y2z2w2, x2y2z2w2, _MM_SHUFFLE(3, 3, 3, 3))));
    lengthSqr = _mm_or_ps(_mm_and_ps(x2py2gez2pw2, t0), _mm_andnot_ps(x2py2gez2pw2, t1));

    // Normalize precisely via square root 
    m128 length = _mm_sqrt_ps(lengthSqr);
    m128 doubledResult = _mm_div_ps(t2, length);
    
    
    v4 result;
    result.smv = _mm_mul_ps(doubledResult, _mm_set1_ps(0.5f));
    return(result);
#endif
#endif    
}

inline v4 FCALL
QuaternionMultiply(v4 a, v4 b)
{
#if NO_INTRINSICS

    v4 result  =
	{
            (b.e[3] * a.e[0]) + (b.e[0] * a.e[3]) + (b.e[1] * a.e[2]) - (b.e[2] * a.e[1]),
            (b.e[3] * a.e[1]) - (b.e[0] * a.e[2]) + (b.e[1] * a.e[3]) + (b.e[2] * a.e[0]),
            (b.e[3] * a.e[2]) + (b.e[0] * a.e[1]) - (b.e[1] * a.e[0]) + (b.e[2] * a.e[3]),
            (b.e[3] * a.e[3]) - (b.e[0] * a.e[0]) - (b.e[1] * a.e[1]) - (b.e[2] * a.e[2])
        };
    return(result);    
#elif defined(ARM)

#elif defined(SSE)
    v4 controlWZYX = {1.0f, -1.0f, 1.0f, -1.0f};
    v4 controlZWXY = {1.0f, 1.0f, -1.0f, -1.0f};
    v4 controlYXWZ = {-1.0f, 1.0f, 1.0f, -1.0f};
    m128 q2x = b.smv;
    m128 q2y = b.smv;
    m128 q2z = b.smv;
    v4 result = b;

    result.smv = FM_PERMUTE_PS(result.smv, _MM_SHUFFLE(3, 3, 3, 3));
    q2x = FM_PERMUTE_PS(q2x, _MM_SHUFFLE(0, 0, 0, 0));
    q2y = FM_PERMUTE_PS(q2y, _MM_SHUFFLE(1, 1, 1, 1));
    q2z = FM_PERMUTE_PS(q2z, _MM_SHUFFLE(2, 2, 2, 2));

    result.smv = _mm_mul_ps(result.smv, a.smv);
    m128 q1Shuffle = a.smv;
    q1Shuffle = FM_PERMUTE_PS(q1Shuffle, _MM_SHUFFLE(0, 1, 2, 3));
    q2x = _mm_mul_ps(q2x, q1Shuffle);
    q1Shuffle = FM_PERMUTE_PS(q1Shuffle, _MM_SHUFFLE(2, 3, 0, 1));
    result.smv = FM_FMADD_PS(q2x, controlWZYX.smv, result.smv);
    q2y = _mm_mul_ps(q2y, q1Shuffle);
    q1Shuffle = FM_PERMUTE_PS(q1Shuffle, _MM_SHUFFLE(0, 1, 2, 3));
    q2y = _mm_mul_ps(q2y, controlZWXY.smv);
    q2z = _mm_mul_ps(q2z, q1Shuffle);
    q2y = FM_FMADD_PS(q2z, controlYXWZ.smv, q2y);
    result.smv = _mm_add_ps(result.smv, q2y);
    return(result);
#endif    
}

inline v4 FCALL
QuaternionConjugate(v4 q)
{
#if NO_INTRINSICS
    v4 result =
    {
	-q.e[0],
	-q.e[1],
	-q.e[2],
	q.e[3]
    };

    return(result);
#elif defined(ARM)

#elif defined(SSE)
    v4 negativeOne3 = {-1.0f, -1.0f, -1.0f, 1.0f};
    v4 result = {};
    result.smv = _mm_mul_ps(q.smv, negativeOne3.smv);
    return(result);
#endif    
}

inline v4 FCALL
Vector3Rotate(v4 v, v4 rotQ)
{
    v4 a = VectorSelect(FM_SELECT1110, v, FM_SELECT1110);
    v4 q = QuaternionConjugate(rotQ);
    v4 result = QuaternionMultiply(q, a);
    return(QuaternionMultiply(result, rotQ));
}

inline v4 FCALL
QuaternionDot(v4 a, v4 b)
{
    return(DotV4(a, b));
}

//Checked
inline v4 FCALL
QuaternionSlerpV(v4 q0, v4 q1, v4 t)
{
#if NO_INTRINSICS

#elif defined(ARM)

#elif defined(SSE)
    v4 oneMinusEpsilon = {1.0f - 0.000001f, 1.0f - 0.000001f, 1.0f - 0.000001f, 1.0f - 0.000001f};
    v4u32 signMask2 = {0x80000000, 0x00000000, 0x00000000, 0x00000000};

    v4 cosOmega = QuaternionDot(q0, q1);

    v4 zero = {0.0f, 0.0f, 0.0f, 0.0f};
    v4 control = VectorLess(cosOmega, zero);
    v4 sign = VectorSelect(FM_ONE, FM_NEGATIVE_ONE, control);

    cosOmega.smv = _mm_mul_ps(cosOmega.smv, sign.smv);
    control = VectorLess(cosOmega, oneMinusEpsilon);

    v4 sinOmega = {};
    sinOmega.smv = _mm_mul_ps(cosOmega.smv, cosOmega.smv);
    sinOmega.smv = _mm_sub_ps(FM_ONE.smv, sinOmega.smv);
    sinOmega.smv = _mm_sqrt_ps(sinOmega.smv);

    v4 omega = VectorATan2(sinOmega, cosOmega);

    v4 v01 = {};
    v01.smv = FM_PERMUTE_PS(t.smv, _MM_SHUFFLE(2, 3, 0, 1));
    v01.smv = _mm_and_ps(v01.smv, FM_MASK_XY.smv);
    v01.smv = _mm_xor_ps(v01.smv, signMask2.smv);
    v01.smv = _mm_add_ps(FM_IDENTITY_R0.smv, v01.smv);
    
    v4 s0_slerp = {};
    s0_slerp.smv = _mm_mul_ps(v01.smv, omega.smv);
    s0_slerp = VectorSin(s0_slerp);
    s0_slerp.smv = _mm_div_ps(s0_slerp.smv, sinOmega.smv);

    v4 s0_lerp = v01;
    v4 s0 = VectorSelect(s0_lerp, s0_slerp, control);

    v4 s1 = SplatY(s0);
    s0 = SplatX(s0);

    s1.smv = _mm_mul_ps(s1.smv, sign.smv);
    v4 result = {};
    result.smv = _mm_mul_ps(q0.smv, s0.smv);
    s1.smv = _mm_mul_ps(s1.smv, q1.smv);
    result.smv = _mm_add_ps(result.smv, s1.smv);
    return(result);
    //Check to see if these functions work (including the ones like Sin and ATan2)
#endif    
}

inline m4 FCALL
BuildScalingMatrix(r32 x, r32 y, r32 z)
{
#if NO_INTRINSICS
    m4 m = {};

    m.e[0][0] = x;
    m.e[0][1] = 0.0f;
    m.e[0][2] = 0.0f;
    m.e[0][3] = 0.0f;    
    
    m.e[1][0] = 0.0f;
    m.e[1][1] = y;
    m.e[1][2] = 0.0f;
    m.e[1][3] = 0.0f;

    m.e[2][0] = 0.0f;
    m.e[2][1] = 0.0f;
    m.e[2][2] = z;
    m.e[2][3] = 0.0f;

    m.e[3][0] = 0.0f;
    m.e[3][1] = 0.0f;
    m.e[3][2] = 0.0f;
    m.e[3][3] = 1.0f;

    return(m);
#elif defined(ARM)

#elif defined(SSE)
    m4 m = {};

    m.r[0].smv = _mm_set_ps(0, 0, 0, x);
    m.r[1].smv = _mm_set_ps(0, 0, y, 0);
    m.r[2].smv = _mm_set_ps(0, z, 0, 0);
    m.r[3].smv = FM_IDENTITY_R3.smv;

    return(m);
#endif    
}

inline m4 FCALL
BuildScalingMatrix(v4 v)
{
    m4 result = BuildScalingMatrix(v.x, v.y, v.z);
    return(result);
}

inline m4 FCALL
BuildTranslationMatrix(r32 x, r32 y, r32 z)
{
#if NO_INTRINSICS    
    m4 m = {};

    m.e[0][0] = 1.0f;
    m.e[0][1] = 0.0f;
    m.e[0][2] = 0.0f;
    m.e[0][3] = 0.0f;

    m.e[1][0] = 0.0f;
    m.e[1][1] = 1.0f;
    m.e[1][2] = 0.0f;
    m.e[1][3] = 0.0f;

    m.e[2][0] = 0.0f;
    m.e[2][1] = 0.0f;
    m.e[2][2] = 1.0f;
    m.e[2][3] = 0.0f;

    m.e[3][0] = x;
    m.e[3][1] = y;
    m.e[3][2] = z;
    m.e[3][3] = 1.0f;


    return(m);
#elif defined(ARM)

#elif defined(SSE)
    m4 m = {};

    m.r[0] = FM_IDENTITY_R0;
    m.r[1] = FM_IDENTITY_R1;
    m.r[2] = FM_IDENTITY_R2;
    m.r[3] = {x, y, z, 1.f};
    return(m);

#endif    
}

inline v4 FCALL
QuaternionFromEuler(v4 angles)
{
#if NO_INTRINSICS
    r32 halfPitch = angles.e[0] * 0.5f;
    r32 cp = cosf(halfPitch);
    r32 sp = sinf(halfPitch);

    r32 halfYaw = angles.e[1] * 0.5f;
    r32 cy = cosf(halfYaw);
    r32 sy = sinf(halfYaw);

    r32 halfRoll = angles.e[2] * 0.5f;
    r32 cr = cosf(halfRoll);
    r32 sr = sinf(halfRoll);

    v4 result =
    {
	cr * sp * cy + sr * cp * sy,
	cr * cp * sy - sr * sp * cy,
	sr * cp * cy - cr * sp * sy,
	cr * cp * cy + sr * sp * sy
    };
    return(result);
#else
    v4 sign = {1.0f, -1.0f, -1.0f, 1.0f};

    v4 halfAngles = FM_ONEHALF * angles;
    v4 sinAngles, cosAngles;
    VectorSinCos(&sinAngles, &cosAngles, halfAngles);


    v4 P0, Y0, R0, P1, Y1, R1;
#if 0
    v4u32 temp = VectorPermute(sinAngles, cosAngles, FM_PERMUTE_0X, FM_PERMUTE_1X, FM_PERMUTE_1X, FM_PERMUTE_1X);    
    P0.iSmv = temp.iSmv;
    temp = VectorPermute(sinAngles, cosAngles, FM_PERMUTE_1Y, FM_PERMUTE_0Y, FM_PERMUTE_1Y, FM_PERMUTE_1Y);
    Y0.iSmv = temp.iSmv;
    temp = VectorPermute(sinAngles, cosAngles, FM_PERMUTE_1Z, FM_PERMUTE_1Z, FM_PERMUTE_0Z, FM_PERMUTE_1Z);
    R0.iSmv = temp.iSmv;
    temp = VectorPermute(cosAngles, sinAngles, FM_PERMUTE_0X, FM_PERMUTE_1X, FM_PERMUTE_1X, FM_PERMUTE_1X);
    P1.iSmv = temp.iSmv;
    temp = VectorPermute(cosAngles, sinAngles, FM_PERMUTE_1Y, FM_PERMUTE_0Y, FM_PERMUTE_1Y, FM_PERMUTE_1Y);
    Y1.iSmv = temp.iSmv;
    temp = VectorPermute(cosAngles, sinAngles, FM_PERMUTE_1Z, FM_PERMUTE_1Z, FM_PERMUTE_0Z, FM_PERMUTE_1Z);
    R1.iSmv = temp.iSmv;
#else
    P0 =
	VectorPermute(sinAngles, cosAngles, FM_PERMUTE_0X, FM_PERMUTE_1X, FM_PERMUTE_1X, FM_PERMUTE_1X);
    Y0 =
	VectorPermute(sinAngles, cosAngles, FM_PERMUTE_1Y, FM_PERMUTE_0Y, FM_PERMUTE_1Y, FM_PERMUTE_1Y);
    R0 =
	VectorPermute(sinAngles, cosAngles, FM_PERMUTE_1Z, FM_PERMUTE_1Z, FM_PERMUTE_0Z, FM_PERMUTE_1Z);
    P1 =
	VectorPermute(sinAngles, cosAngles, FM_PERMUTE_1X, FM_PERMUTE_0X, FM_PERMUTE_0X, FM_PERMUTE_0X);
    Y1 =
	VectorPermute(sinAngles, cosAngles, FM_PERMUTE_0Y, FM_PERMUTE_1Y, FM_PERMUTE_0Y, FM_PERMUTE_0Y);
    R1 =
	VectorPermute(sinAngles, cosAngles, FM_PERMUTE_0Z, FM_PERMUTE_0Z, FM_PERMUTE_1Z, FM_PERMUTE_0Z);
#endif    

    
    v4 q1 = P1 * sign;
    v4 q0 = P0 * Y0;

    q1 = q1 * Y1;
    q0 = q0 * R0;
    v4 q = VectorMultiplyAdd(q1, R1, q0);
    return(q);
#endif    
}

inline v4 FCALL
QuaternionFromEuler(r32 pitch, r32 yaw, r32 roll)
{
    v4 v = {pitch, yaw, roll, 0.0f};
    return(QuaternionFromEuler(v));
}

//this isn't sim'd
inline v4 FCALL
EulerFromQuaternion(v4 q)
{
    v4 result = {};
    r32 sinP = 2.0f * (q.w * q.x + q.y * q.z);
    r32 cosP = 1.0f - 2.0f * (q.x * q.x + q.y * q.y);
    result.x = atan2f(sinP, cosP);

    r32 sinY = 2.0f * (q.w * q.y - q.z * q.x);
    if (fabs(sinY) >= 1.0f)
    {
	result.y = (r32)copysign(FM_PIDIV2, sinY);
    }
    else
    {
	result.y = asinf(sinY);
    }

    r32 sinR = 2.0f * (q.w * q.z + q.x * q.y);
    r32 cosR = 1.0f - 2.0f * (q.y * q.y + q.z * q.z);
    result.z = atan2f(sinR, cosR);
    result.w = 0.0f;
    return(result);
}

inline m4 FCALL
BuildTranslationMatrix(v4 v)
{
    m4 result = BuildTranslationMatrix(v.x, v.y, v.z);
    return(result);
}


inline m4 FCALL
CreateModelMatrix(v4 scale, v4 rot, v4 trans)
{

    m4 scaleM = BuildScalingMatrix(scale);
    v4 normalizedRot = QuaternionNormalize(rot);
    m4 rotM = MatrixRotationQuaternion(normalizedRot);
    m4 translationM = BuildTranslationMatrix(trans);

#if 0
    m4 result = translationM * rotM * scaleM;
#else
    m4 result = (scaleM * rotM) * translationM;
#endif    
    
    return(result);
}

inline v4 FCALL
CreateQuaternionRotationFromVector(v4 forward, v4 up)
{
    v4 f = NormalizeV3(forward);

    v4 tenU = NormalizeV3(up);
    
    v4 r = NormalizeV3(CrossV3(f, tenU));
    v4 u = CrossV3(r, f);
    
    //VecLenV3 is not defined, plz define it
//    v4 mUCF = VecLenV3(uCf);
//    v4 r = uCf / mUCF;
    m4 lookAt = LookToLH(FM_ZERO, f, u);
    m4 rotMat = Transpose(lookAt);
    v4 result = QuaternionRotationMatrix(rotMat);


    return(result);
}

inline v4 FCALL
CreateQuaternionRotationFromVector(v4 forward)
{
    v4 up = v4{0.0f, 1.0f, 0.0f, 0.0f};
    return(CreateQuaternionRotationFromVector(forward, up));
}

//At some point convert to SIMD
inline r32 FCALL
DotProductToDegrees(r32 dotProduct, r32 lengthA, r32 lengthB)
{
    r32 cosTheta = dotProduct / (lengthA * lengthB);

    cosTheta = Max(-1.0f, Min(cosTheta, -1.0f));

    r32 angleRadians = (r32)acos(cosTheta);

    r32 result = (r32)RAD2DEG(angleRadians);
    return(result);
}

#define FORTY_MATH_FAST_H
#endif

