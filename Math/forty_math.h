#if !defined (FORTY_MATH_H)

struct v2I
{
    i32 x, y;
};

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

    inline v2 &operator*=(r32 a);
    inline v2 &operator+=(v2 a);
    inline v2 &operator-=(v2 a);    
};



inline v2
operator*(r32 a, v2 b)
{
    v2 result;
    result.x = a * b.x;
    result.y = a * b.y;


    return(result);
}

inline v2
operator*(v2 b, r32 a)
{
    v2 result;
    result.x = a * b.x;
    result.y = a * b.y;


    return(result);
}

inline v2
operator/(v2 a, v2 b)
{
    v2 result;
    result.x = a.x / b.x;
    result.y = a.y / b.y;
    return(result);
}

inline v2
operator/(v2 a, r32 b)
{
    v2 result;
    result.x = a.x / b;
    result.y = a.y / b;
    return(result);
}

inline v2
operator-(v2 a)
{
    v2 result;
    result.x = -a.x;
    result.y = -a.y;


    return(result);
}

inline v2
operator+(v2 a, v2 b)
{
    v2 result;
    result.x = a.x + b.x;
    result.y = a.y + b.y;


    return(result);
}

inline v2
operator-(v2 a, v2 b)
{
    v2 result;
    result.x = a.x - b.x;
    result.y = a.y - b.y;


    return(result);
}

inline v2 &v2::
operator*=(r32 a)
{
    *this = a * *this;
    return(*this);
}

inline v2 &v2::
operator+=(v2 a)
{
    *this = *this + a;
    return(*this);
}

inline v2 &v2::
operator-=(v2 a)
{
    *this = *this - a;
    return(*this);
}

inline bool32
operator==(v2 a, v2 b)
{
    bool32 result = (a.x == b.x) && (a.y == b.y);
    return(result);
}

inline bool32
operator!=(v2 a, v2 b)
{
    bool32 result = !(a == b);
    return(result);
}

internal r32
Dot(v2 a, v2 b)
{
    r32 result;

    result = (a.x)*(b.x) + (a.y)*(b.y);
    return(result);
}

inline r32
Magnitude(v2 a)
{
    r32 result = 0.0f;
    result = (r32)sqrt(pow(a.x, 2) + pow(a.y, 2));
    return(result);
}

internal v2
Normalize(v2 a)
{
    v2 result = {};
    result = a / Magnitude(a);
    return(result);
}

/*
***********************v3*************************
 */

struct v3
{
    union
    {
	struct
	{
	    r32 x, y, z;
	};
	r32 e[3];
    };

    inline v3 &operator*=(r32 a);
    inline v3 &operator+=(v3 a);
    inline v3 &operator-=(v3 a);
    inline v3 &operator/=(r32 a);
};

inline v3
operator*(r32 a, v3 b)
{
    v3 result;
    result.x = a * b.x;
    result.y = a * b.y;
    result.z = a * b.z;

    return(result);
}

inline v3
operator*(v3 b, r32 a)
{
    v3 result;
    result.x = a * b.x;
    result.y = a * b.y;
    result.z = a * b.z;

    return(result);
}

inline v3
operator/(v3 a, v3 b)
{
    v3 result;
    result.x = a.x / b.x;
    result.y = a.y / b.y;
    result.z = a.z / b.z;
    return(result);
}

inline v3
operator-(v3 a)
{
    v3 result;
    result.x = -a.x;
    result.y = -a.y;
    result.z = -a.z;

    return(result);
}

inline v3
operator+(v3 a, v3 b)
{
    v3 result;
    result.x = a.x + b.x;
    result.y = a.y + b.y;
    result.z = a.z + b.z;

    return(result);
}

inline v3
operator+(v3 a, r32 b)
{
    v3 result = {};
    result.x = a.x + b;
    result.y = a.y + b;
    result.z = a.z + b;

    return(result);
}

inline v3
operator+(r32 b, v3 a)
{
    v3 result = a + b;
    return(result);
}

inline v3
operator-(v3 a, v3 b)
{
    v3 result;
    result.x = a.x - b.x;
    result.y = a.y - b.y;
    result.z = a.z - b.z;

    return(result);
}

inline v3
operator-(v3 a, r32 b)
{
    v3 result;
    result.x = a.x - b;
    result.y = a.y - b;
    result.z = a.z - b;

    return(result);
}

inline v3
operator/(v3 a, r32 b)
{
    v3 result = {};
    result.x = a.x / b;
    result.y = a.y / b;
    result.z = a.z / b;

    return(result);
}

inline v3 &v3::
operator*=(r32 a)
{
    *this = a * *this;
    return(*this);
}

inline v3 &v3::
operator+=(v3 a)
{
    *this = *this + a;
    return(*this);
}

inline v3 &v3::
operator-=(v3 a)
{
    *this = *this - a;
    return(*this);
}

inline v3 &v3::
operator/=(r32 a)
{
    *this = *this / a;
    return(*this);
}

internal r32
Dot(v3 a, v3 b)
{
    r32 result;

    result = ((a.x)*(b.x) + (a.y)*(b.y) + (a.z)*(b.z));
    return(result);
}

internal v3
Cross(v3 a, v3 b)
{
    r32 x, y, z;
    x = ((a.y) * (b.z)) - ((a.z) * (b.y));
    y = ((a.z) * (b.x)) - ((a.y) * (b.z));
    z = ((a.x)* (b.y)) - ((a.y) * (b.x));
    v3 result = v3{x, y, z};
    return(result);
}

inline r32
Magnitude(v3 a)
{
    r32 result = 0.0f;
    result = (r32)sqrt(pow(a.x, 2) + pow(a.y, 2) + pow(a.z, 2));
    return(result);
}

internal v3
Normalize(v3 a)
{
    v3 result = {};
    result = a / Magnitude(a);
    return(result);
}

/*
********v4********
 */

struct v4
{
    union
    {
	struct
	{
	    r32 x, y, z, w;
	};
	r32 e[4];
    };
    inline v4 &operator*=(r32 a);
    inline v4 &operator+=(v4 a);
    inline v4 &operator-=(v4 a);    
};


inline v4
operator*(r32 a, v4 b)
{
    v4 result;
    result.x = a * b.x;
    result.y = a * b.y;
    result.z = a * b.z;
    result.w = a * b.w;

    return(result);
}

inline v4
operator*(v4 b, r32 a)
{
    v4 result;

    result = a * b;
    
    
    return(result);
}

inline v4
operator/(v4 a, v4 b)
{
    v4 result;
    result.x = a.x / b.x;
    result.y = a.y / b.y;
    result.z = a.z / b.z;
    result.w = a.w / b.w;
    return(result);
}

inline v4
operator-(v4 a)
{
    v4 result;
    result.x = -a.x;
    result.y = -a.y;
    result.z = -a.z;
    result.w = -a.w;
    
    return(result);
}

inline v4
operator+(v4 a, v4 b)
{
    v4 result;
    result.x = a.x + b.x;
    result.y = a.y + b.y;
    result.z = a.z + b.z;
    result.w = a.w + b.w;
    
    return(result);
}

inline v4
operator+(v4 a, r32 b)
{
    v4 result;
    result.x = a.x + b;
    result.y = a.y + b;
    result.z = a.z + b;
    result.w = a.w + b;

    return(result);
}

inline v4
operator+(r32 b, v4 a)
{
    v4 result = a + b;
    return(result);
}

inline v4
operator-(v4 a, v4 b)
{
    v4 result;
    result.x = a.x - b.x;
    result.y = a.y - b.y;
    result.z = a.z - b.z;
    result.w = a.w - b.w;

    return(result);
}

inline v4
operator/(v4 a, r32 b)
{
    v4 result = {};
    result.x = a.x / b;
    result.y = a.y / b;
    result.z = a.z / b;
    result.w = a.w / b;

    return(result);
}

inline v4 &v4::
operator*=(r32 a)
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

internal r32
DotV4(v4 a, v4 b)
{
    r32 result;

    result = ((a.x)*(b.x) + (a.y)*(b.y) + (a.z)*(b.z), (a.w)*(b.w));
    return(result);
}

internal v4
Cross(v4 a, v4 b)
{
    r32 x, y, z, w;
    x = ((a.y) * (b.z)) - ((a.z) * (b.y));
    y = ((a.z) * (b.x)) - ((a.y) * (b.z));
    z = ((a.x)* (b.y)) - ((a.y) * (b.x));

    
    v4 result = v4{x, y, z, 0.0f};
    return(result);
}

/*
*************************M2*************************
 */

struct m2
{
    union
    {
	struct
	{
	    v2 r1, r2;
	};
	v2 r[2];

	struct
	{
	    r32 m11, m12;
	    r32 m21, m22;
	};
	r32 e[2][2];
    };
    inline m2 &operator*=(m2);
    inline m2 &operator*=(r32);
    inline m2 &operator+=(m2);
    inline m2 &operator-=(m2);        
};

inline v2
column(m2 m, i32 i)
{
    v2 result = {};
    result = v2{m.e[0][i], m.e[1][i]};
    return(result);
}

inline m2
operator+(m2 a, m2 b)
{
    m2 result = {};
    result.r1 = a.r1 + b.r1;
    result.r2 = a.r2 + b.r2;
    return(result);
}

inline m2
operator-(m2 a, m2 b)
{
    m2 result = {};
    result.r1 = a.r1 - b.r1;
    result.r2 = a.r2 - b.r2;
    return(result);
}

inline m2
operator*(m2 a, m2 b)
{
    m2 result = {};
    
    for (i32 i = 0; i < 2; i++)
    {
	for (i32 j = 0; j < 2; j++)
	{
	    result.e[i][j] = Dot(a.r[i], column(b, j));
	}
    }

    return(result);
}

inline m2
operator*(m2 a, r32 b)
{
    m2 result = {};
    for (i32 i = 0; i < 2; i++)
    {
	result.r[i] = a.r[i] * b;
    }
    return(result);
}

inline m2 &m2::
operator*=(m2 a)
{
    *this = *this * a;
    return(*this);
}

inline m2 &m2::
operator*=(r32 a)
{
    *this = *this * a;
    return(*this);
}

inline m2 &m2::
operator+=(m2 a)
{
    *this = *this + a;
    return(*this);
}

inline m2 &m2::
operator-=(m2 a)
{
    *this = *this - a;
    return(*this);
}

internal r32
DetM2(m2 m)
{
    r32 result = 0.0f;
    /*
      00 01
      10 11
     */

    r32 a = m.m11 * m.m22;
    r32 b = m.m21 * m.m12;
    result = a - b;
    return(result);
}

internal m2
Transpose(m2 m)
{
    //Columns become rows
    m2 result = {};
    for (i32 i = 0; i < 2; i++)
    {
	for (i32 j = 0; j < 2; j++)
	{
	    result.e[j][i] = m.e[i][j];
	}
    }
    return(result);
}

internal v2
TransformVec(v2 v, m2 m)
{
    v2 result = {};
    result.x = v.x * m.r1.x + v.y * m.r2.x;
    result.y = v.x * m.r2.y + v.y * m.r2.y;

    return(result);
}

/*
*************************M3*************************
 */

struct m3
{
    union
    {
	struct
	{
	    v3 r1, r2, r3;
	};
	v3 r[3];

	struct
	{
	    r32 m11, m12, m13;
	    r32 m21, m22, m23;
	    r32 m31, m32, m33;
	};
	r32 e[3][3];
    };

    inline m3 &operator*=(m3);
    inline m3 &operator*=(r32);
    inline m3 &operator+=(m3);
    inline m3 &operator-=(m3);    
};


inline v3
column(m3 m, i32 i)
{
    v3 result = {};
    result = v3{m.e[0][i], m.e[1][i], m.e[2][i]};
    return(result);
}

inline m3
operator+(m3 a, m3 b)
{
    m3 result = {};
    result.r1 = a.r1 + b.r1;
    result.r2 = a.r2 + b.r2;
    result.r3 = a.r3 + b.r3;
    return(result);
}

inline m3
operator-(m3 a, m3 b)
{
    m3 result = {};
    result.r1 = a.r1 - b.r1;
    result.r2 = a.r2 - b.r2;
    result.r3 = a.r3 - b.r3;
    return(result);
}

inline m3
operator*(m3 a, m3 b)
{
    m3 result = {};
    
    for (i32 i = 0; i < 3; i++)
    {
	for (i32 j = 0; j < 3; j++)
	{
	    result.e[i][j] = Dot(a.r[i], column(b, j));
	}
    }

    return(result);
}

inline m3
operator*(m3 a, r32 b)
{
    m3 result = {};
    for (i32 i = 0; i < 3; i++)
    {
	result.r[i] = a.r[i] * b;
    }
    return(result);
}

inline m3 &m3::
operator*=(m3 a)
{
    *this = *this * a;
    return(*this);
}

inline m3 &m3::
operator*=(r32 a)
{
    *this = *this * a;
    return(*this);
}

inline m3 &m3::
operator+=(m3 a)
{
    *this = *this + a;
    return(*this);
}

inline m3 &m3::
operator-=(m3 a)
{
    *this = *this - a;
    return(*this);
}

struct mat_array
{
    r32 a[4][4];
};

internal mat_array
GetMinorMatrix(i32 matrixStride, v2 currMinor, r32 m[][4])
{
    mat_array result = {};

    for (i32 i = 0, l = 0; i < matrixStride; i++)
    {
	for (i32 j = 0, f = 0; j < matrixStride; j++)
	{
	    if ((i != currMinor.x) && (j != currMinor.y))
	    {
		result.a[l][f] = m[i][j];
		if (f < matrixStride - 2)
		    f++;
		else
		    l++; 
	    }
	}
    }

    return(result);
}

//When you are brave enough translate to SIMD
internal r32
GetDet(i32 matrixStride, r32 m[][4])
{
    r32 result = 0.0f;
    if (matrixStride == 1)
    {
	return(m[0][0]);
    }
    

    v2 a11, a12, a13, a14;
    a11 = v2{0, 0};
    a12 = v2{0, 1};
    a13 = v2{0, 2};
    a14 = v2{0, 3};

    v2 minor[4] = {a11, a12, a13, a14};

    i32 sign = -1;
    for (i32 k = 0; k < matrixStride; k++)
    {
	v2 currMinor = minor[k];

	r32 currDetMat[4][4];
	i32 addedValues = 0;

#if 0	
	for (i32 i = 0, l = 0; i < matrixStride; i++)
	{
	    for (i32 j = 0, f = 0; j < matrixStride; j++)
	    {
		v2 ij = v2{(r32)i, (r32)j};
		if ((ij.x != currMinor.x) && (ij.y != currMinor.y))
		{
		    currDetMat[l][f] = m[i][j];

		    if (f < matrixStride - 2)
			f++;
		    else
			l++;
		}
	    }
	}

#else
	mat_array a = GetMinorMatrix(matrixStride, currMinor, m);
	for (i32 i = 0; i < matrixStride; i++)
	{
	    for (i32 j = 0; j < matrixStride; j++)
	    {
		currDetMat[i][j] = a.a[i][j];
	    }
	}

#endif	
	i32 e1, e2;
	e1 = (i32)currMinor.e[0];
	e2 = (i32)currMinor.e[1];

	sign *= -1;
	result += (m[e1][e2]) * (GetDet(matrixStride - 1, currDetMat)) * sign;

    }
    return(result);
}


internal r32
Det(m3 m)
{
    r32 result = 0.0f;
    
    i32 matrixStride = 3;
    r32 newMat[4][4] = {};

    for (i32 i = 0; i < 4; i++)
    {
	for (i32 j = 0; j < 4; j++)
	{
	    newMat[i][j] = (j > (matrixStride - 1)) || (i > (matrixStride - 1)) ? 0 : m.e[i][j];
	}
    }
    
    result = GetDet(matrixStride, newMat);
    return(result);
}

internal m3
Transpose(m3 m)
{
    m3 result = {};
    for (i32 i = 0; i < 3; i++)
    {
	for (i32 j = 0; j < 3; j++)
	{
	    result.e[j][i] = m.e[i][j];
	}
    }
    return(result);
}

internal v3
TransformVec(v3 v, m3 m)
{
    v3 result = {};
    result.x = Dot(v, column(m, 0));
    result.y = Dot(v, column(m, 1));
    result.z = Dot(v, column(m, 2));

    return(result);
}

//You were working here
internal mat_array
Adjoint(r32 m[][4], i32 matrixStride)
{
    mat_array result;
    
    for (i32 i = 0; i < matrixStride; i++)
    {
	for (i32 j = 0; j < matrixStride; j++)
	{
	    v2 currMinor = v2{(r32)i, (r32)j};
	    mat_array a = GetMinorMatrix(matrixStride - 1, currMinor, m);
	    result.a[i][j];
	}
    }
}

/*
*************************M4*************************
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

internal m3
FromM4ToM3(m4 m)
{
    m3 result = {};
    for (i32 i = 0; i < 3; i++)
    {
	for (i32 j = 0; j < 3; j++)
	{
	    result.e[i][j] = m.e[i][j];
	}
    }
    return(result);
}

inline v4
column(m4 m, i32 i)
{
    v4 result = {};
    result = v4{m.e[0][i], m.e[1][i], m.e[2][i], m.e[3][i]};
    return(result);
}

inline m4
operator+(m4 a, m4 b)
{
    m4 result = {};
    result.r1 = a.r1 + b.r1;
    result.r2 = a.r2 + b.r2;
    result.r3 = a.r3 + b.r3;
    result.r4 = a.r4 + b.r4;
    return(result);
}

inline m4
operator-(m4 a, m4 b)
{
    m4 result = {};
    result.r1 = a.r1 - b.r1;
    result.r2 = a.r2 - b.r2;
    result.r3 = a.r3 - b.r3;
    result.r4 = a.r4 - b.r4;
    return(result);
}

inline m4
operator*(m4 a, m4 b)
{
    m4 result = {};
    
    for (i32 row = 0; row < 4; row++)
    {
	for (i32 col = 0; col < 4; col++)
	{
	    result.e[row][col] = (a.e[row][0] * b.e[0][col] +
				  a.e[row][1] * b.e[1][col] +
				  a.e[row][2] * b.e[2][col] +
				  a.e[row][3] * b.e[3][col]);
	}
    }

    return(result);
}

inline m4
operator*(m4 a, r32 b)
{
    m4 result = {};
    for (i32 i = 0; i < 4; i++)
    {
	result.r[i] = a.r[i] * b;
    }
    return(result);
}

inline m4 &m4::
operator*=(m4 a)
{
    *this = *this * a;
    return(*this);
}

inline m4 &m4::
operator*=(r32 a)
{
    *this = *this * a;
    return(*this);
}

inline m4 &m4::
operator+=(m4 a)
{
    *this = *this + a;
    return(*this);
}

inline m4 &m4::
operator-=(m4 a)
{
    *this = *this - a;
    return(*this);
}

internal r32
Dot(v4 a, v4 b)
{
    r32 result;
    result = ((a.x)*(b.x) + (a.y)*(b.y) + (a.z)*(b.z) + (a.w)*(b.w));
    return(result);
}

inline r32
Magnitude(v4 a)
{
    r32 result = 0.0f;
    result = (r32)sqrt(pow(a.x, 2) + pow(a.y, 2) + pow(a.z, 2) + pow(a.w, 2));
    return(result);
}

internal v4
Normalize(v4 a)
{
    v4 result = {};
    result = a / Magnitude(a);
    return(result);
}

internal v4
TransformVec(v4 v, m4 m)
{
    v4 result = {};

#if 1
    result.x = Dot(v, column(m, 0));
    result.y = Dot(v, column(m, 1));
    result.z = Dot(v, column(m, 2));
    result.w = Dot(v, column(m, 3));    
#else
    result.x = Dot(v, m.r1);
    result.y = Dot(v, m.r2);
    result.z = Dot(v, m.r3);
    result.w = Dot(v, m.r4);    
#endif
    return(result);
}

internal r32
Det(m4 m)
{
    r32 result = 0.0f;
    i32 matrixStride = 4;
    r32 newMat[4][4] = {};
    
    for (i32 i = 0; i < 4; i++)
    {
	for (i32 j = 0; j < 4; j++)
	{
	    newMat[i][j] = (j > (matrixStride -1)) || (i > (matrixStride - 1)) ? 0 : m.e[i][j];
	}
    }
    result = GetDet(matrixStride, newMat);
    return(result);
}

internal m4
Transpose(m4 m)
{
    m4 result = {};
    for (i32 i = 0; i < 4; i++)
    {
	for (i32 j = 0; j < 4; j++)
	{
	    result.e[j][i] = m.e[i][j];
	}
    }
    return(result);
}

internal m3
MinorToM3(mat_array m)
{
    m3 result = {};

    i32 matrixStride = 3;
    for (i32 i = 0; i < matrixStride; i++)
    {
	for (i32 j = 0; j < matrixStride; j++)
	{
	    result.e[i][j] = m.a[i][j];
	}
    }

    return(result);
}

internal m2
GetMinorMatrix(i32 matrixStride, v2 currMinor, m2 m)
{
    m2 result = {};
    i32 skipR, skipC;
    skipR = (i32)currMinor.y;
    skipC = (i32)currMinor.x;

    i32 outR = 0;
    for (i32 i = 0; i < matrixStride; i++)
    {
	if (i == skipR) continue;

	i32 outC = 0;
	for (i32 j = 0; j < matrixStride; j++)
	{
	    if (j == skipC) continue;

	    result.e[outR][outC] = m.e[i][j];
	    outC++;
	}
    }
    outR++;
    return(result);
}


internal m3
GetMinorMatrix(i32 matrixStride, v2 currMinor, m4 m)
{
    i32 skipR, skipC;
    skipR = (i32)currMinor.x;
    skipC = (i32)currMinor.y;

    m3 result = {};
    i32 outR = 0;
    
    for (i32 i = 0; i < matrixStride; i++)
    {
	if (i == skipR) continue;

	i32 outC = 0;
	for (i32 j = 0; j < matrixStride; j++)
	{
	    if (j == skipC) continue;

	    result.e[outR][outC] = m.e[i][j];
	    outC++;
	}
	outR++;
    }
    return(result);
}


internal m4 
Adjoint(m4 m)
{
    m4 result = {};

    i32 matrixStride = 4;
    
    for (i32 i = 0; i < matrixStride; i++)
    {
	for (i32 j = 0; j < matrixStride; j++)
	{
	    v2 currMinor = v2{(r32)i, (r32)j};
	    m3 minor = GetMinorMatrix(matrixStride, currMinor, m);
	    r32 det = Det(minor);

	    i32 ij = (i32)pow(-1, (i + j));
	    
	    result.e[i][j] = det * ij;
	}
    }

    result = Transpose(result);
    return(result);
}

internal m4
Inverse(m4 m)
{
    m4 result = {};

    // (1 / det) * adjoint
    r32 div = 1.0f / Det(m);


    result = Adjoint(m) * div;
    return(result);
}

#define FORTY_MATH_H
#endif
