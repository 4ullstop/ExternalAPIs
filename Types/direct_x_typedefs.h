#if !defined DIRECT_X_TYPEDEFS_H

#include <DirectXMath.h>
#include "typedefs.h"
#include <d3d11.h>

typedef struct constant_buffer_struct_
{
    DirectX::XMFLOAT4X4 world;
    DirectX::XMFLOAT4X4 view;
    DirectX::XMFLOAT4X4 projection;    
} constant_buffer_struct;

typedef struct vertex_position_color_
{
    DirectX::XMFLOAT3 pos;
    DirectX::XMFLOAT3 color;    
} vertex_position_color;

struct cube_buffers
{
    ID3D11Buffer* vertexBuffer;
    ID3D11Buffer* indexBuffer;
    i32 indexCount;
};

#define DIRECT_X_TYPEDEFS_H
#endif
