#if !defined DIRECT_X_TYPEDEFS_H
#include <DirectXMath.h>

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

#define DIRECT_X_TYPEDEFS_H
#endif
