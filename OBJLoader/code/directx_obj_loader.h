#if !defined DIRECTX_OBJ_LOADER_H
//Create directxtypedefs for values associated with direct x that are needed for loading into directx
#include "../../Types/direct_x_typedefs.h"
#include <d3d11.h>

struct direct_x_loaded_buffers
{
    ID3D11Buffer* vertexBuffer;
    ID3D11Buffer* indexBuffer;
    i32 indexCount;
};

#define DIRECTX_OBJ_LOADER_H
#endif
