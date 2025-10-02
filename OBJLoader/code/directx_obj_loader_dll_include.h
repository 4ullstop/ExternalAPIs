#if !defined DIRECTX_OBJ_LOADER_DLL_INCLUDE_H
#include "directx_obj_loader.h"

//Will need to include the id3d11device when you get the chance in the parameter list here
#define DIRECT_X_LOAD_OBJ(name) void name(char* fileLocation, memory_arena* objLocationArena, program_memory* mainProgramMemory, ID3D11Device* device, direct_x_loaded_buffers* loadedBuffers)
typedef DIRECT_X_LOAD_OBJ(direct_x_load_obj);
DIRECT_X_LOAD_OBJ(DirectXLoadOBJStub)
{

}
global_variable direct_x_load_obj* DirectXLoadOBJ_ = DirectXLoadOBJStub;
#define DirectXLoadOBJ DirectXLoadOBJ_

struct direct_x_load_obj_code
{
    direct_x_load_obj* DirectXLoadOBJ;
};

#define DIRECTX_OBJ_LOADER_DLL_INCLUDE_H
#endif
