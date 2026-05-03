#if !defined (FORTY_DIRECTX_MATH_H)

#include "forty_math.h"

internal DirectX::XMVECTOR
FromV3ToXMVECTOR(v3 vec)
{
    DirectX::XMVECTOR result = DirectX::XMVectorSet(vec.x, vec.y, vec.z, 0.0f);
    return(result);
}

internal v3
FromXMVECTORToV3(DirectX::XMVECTOR vec)
{
    v3 result = {};
    result.x = DirectX::XMVectorGetX(vec);
    result.y = DirectX::XMVectorGetY(vec);
    result.z = DirectX::XMVectorGetZ(vec);

    return(result);
}

internal v4
FromXMVECTORToV4(DirectX::XMVECTOR vec)
{
    v4 result = {};
    result.x = DirectX::XMVectorGetX(vec);
    result.y = DirectX::XMVectorGetY(vec);
    result.z = DirectX::XMVectorGetZ(vec);
    result.w = DirectX::XMVectorGetW(vec);

    return(result);
}

internal DirectX::XMVECTOR
FromV4ToXMVECTOR(v4 v)
{
    DirectX::XMVECTOR result = DirectX::XMVectorSet(v.x, v.y, v.z, v.w);
    return(result);
}

internal m4
FromXMMATRIXToM4(DirectX::XMMATRIX m)
{
    m4 result = {};

    result.r1 = FromXMVECTORToV4(m.r[0]);
    result.r2 = FromXMVECTORToV4(m.r[1]);
    result.r3 = FromXMVECTORToV4(m.r[2]);
    result.r4 = FromXMVECTORToV4(m.r[3]);

    return(result);
}

internal DirectX::XMMATRIX
FromM4ToXMMATRIX(m4 m)
{
    DirectX::XMMATRIX result = {};
    result.r[0] = FromV4ToXMVECTOR(m.r1);
    result.r[1] = FromV4ToXMVECTOR(m.r2);
    result.r[2] = FromV4ToXMVECTOR(m.r3);
    result.r[3] = FromV4ToXMVECTOR(m.r4);

    return(result);
}

#define FORTY_DIRECTX_MATH_H
#endif
