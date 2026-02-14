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


typedef struct inst_buffer_struct_
{
    DirectX::XMFLOAT4 instancePosition;
    bool32 renderWholeCube;
} inst_buffer_struct;

typedef struct inst_buffer_struct_2_
{
    r32 voxelWidth, voxelHeight, voxelResolution;
    DirectX::XMFLOAT4 boundingBoxExtent;
} inst_buffer_struct_2;

#define DIRECT_X_TYPEDEFS_H
#endif
