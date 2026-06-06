#if !defined (FORTY_MATH_FAST_H)
#include <xmmintrin.h>
#include "../Types/typedefs.h"
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
    };
};

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
    };
};


//infinity
global_variable v4I FM_INF  = v4I{0x7F80000, 0x7F80000, 0x7F80000, 0x7F80000};
global_variable v4I FM_QNaN = v4I{0x7FC0000, 0x7FC0000, 0x7FC0000, 0x7FC0000};

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
    };
    inline v4 &operator*=(v4 a);
    inline v4 &operator+=(v4 a);
    inline v4 &operator-=(v4 a);    
};

global_variable v4 FM_IDENTITY_R0 = v4{1.0f, 0.0f, 0.0f, 0.0f};
global_variable v4 FM_IDENTITY_R1 = v4{0.0f, 1.0f, 0.0f, 0.0f};
global_variable v4 FM_IDENTITY_R2 = v4{0.0f, 0.0f, 1.0f, 0.0f};
global_variable v4 FM_IDENTITY_R3 = v4{0.0f, 0.0f, 0.0f, 1.0f};


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

global_variable v4 FM_NEGATE_Y = {-1.0f, 1.0f, 1.0f, 1.0f};
global_variable v4 FM_NEGATE_X = {1.0f, -1.0f, 1.0f, 1.0f};
global_variable v4 FM_NEGATE_Z = {1.0f, 1.0f, -1.0f, 1.0f};
global_variable v4 FM_NEGATE_W = {1.0f, 1.0f, 1.0f, -1.0f};

global_variable v4u32 FM_MASK_3 = {0xFFFFFFFF, 0xFFFFFFFF, 0x00000000, 0x00000000};

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
	(r32)u32Result.e[0];
	(r32)u32Result.e[1];
	(r32)u32Result.e[2];
	(r32)u32Result.e[3];	
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

/*
****************************M4*******************************
 */

struct m4
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

#elif (ARM)

#elif (SSE)
    v4 temp1, temp2, temp3, temp4;
    // x.x,x.y,y.x,y.y    
    temp1.smv = _mm_shuffle_ps(m.r[0].smv, m.r[1].smv, _MM_SHUFFLE(1, 0, 1, 0));
    // x.z,x.w,y.z,y.w    
    temp3.smv = _mm_shuffle_ps(m.r[0].smv, m.r[1].smv, _MM_SHUFFLE(3, 2, 3, 2));
    // z.x,z.y,w.x,w.y
    temp2.smv = _mm_shuffle_ps(m.r[2].smv, m.r[3].smv, _MM_SHUFFLE(1, 0, 1, 0));
    // z.z,z.w,w.z,w.w
    temp4.smv = _mm_shuffle_ps(m.r[2].smv, m.r[3].smv, _MM_SHUFFLE(3, 2, 3, 2));

    m4 result;
    // x.x,y.x,z.x,w.x    
    result.r[0].smv = _mm_shuffle_ps(temp1.smv, temp2.smv, _MM_SHUFFLE(2, 0, 2, 0));
    // x.y,y.y,z.y,w.y    
    result.r[1].smv = _mm_shuffle_ps(temp1.smv, temp2.smv, _MM_SHUFFLE(3, 1, 3, 1));
    // x.z,y.z,z.z,w.z
    result.r[2].smv = _mm_shuffle_ps(temp3.smv, temp4.smv, _MM_SHUFFLE(2, 0, 2, 0));
    // x.w,y.w,z.w,w.w
    result.r[3].smv = _mm_shuffle_ps(temp3.smv, temp4.smv, _MM_SHUFFLE(3, 1, 3, 1));
    return(result);
#endif    
}

inline m4 FCALL
LookToLH(v4 eyePos, v4 eyeDir, v4 upDir)
{
    
}

inline m4 FCALL
LookAtRH(v4 eyePos, v4 focusPos, v4 upDir)
{
    v4 eyeDir = focusPos - eyePos;
    return(LookToLH(eyePos, eyeDir, upDir));
}

#define FORTY_MATH_FAST_H
#endif
