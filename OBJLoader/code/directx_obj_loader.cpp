#include "directx_obj_loader.h"
#include "obj_loader.h"
#include "obj_loader.cpp"
#include <d3d11.h>

//This function is the function we will load as a dll, bc it is specific to the operating system

void DirectXLoadOBJ(char* fileLocation, memory_arena* objLocationArena, program_memory* mainProgramMemory, ID3D11Device* device, direct_x_loaded_buffers* loadedBuffers)
{

    obj* parsedOBJData = ParseOBJData(fileLocation, objLocationArena, mainProgramMemory);


    
    u32 objVertsSize = sizeof(vertex_position_color) * parsedOBJData->vertexCount;
    
    vertex_position_color* objVerts =
	(vertex_position_color*)memoryPoolCode.PushArraySized(objLocationArena, objVertsSize);


    for (i32 i = 0, j = 0; i < (parsedOBJData->vertexCount * 3); i += 3, j++)
    {
	objVerts[j].pos.x = parsedOBJData->vertices[i];
	objVerts[j].pos.y = parsedOBJData->vertices[i + 1];
	objVerts[j].pos.z = parsedOBJData->vertices[i + 2];
    }

    //Now we do the fun stuff where load this info into buffers
    //Do we want to load into buffers here? Or in the main program itself?

    //We'll do here for now...

    //In order to create a model, we have to bind it's info to a buffer for the gpu to use?
    HRESULT hr = {};
    
    CD3D11_BUFFER_DESC vertexDesc(
	objVertsSize,
	D3D11_BIND_VERTEX_BUFFER);

    D3D11_SUBRESOURCE_DATA vertexData;
    ZeroMemory(&vertexData, sizeof(D3D11_SUBRESOURCE_DATA));
    vertexData.pSysMem = objVerts;
    vertexData.SysMemPitch = 0;
    vertexData.SysMemSlicePitch = 0;

    hr = device->CreateBuffer(
	&vertexDesc,
	&vertexData,
	&loadedBuffers->vertexBuffer);
    //cubeBuffer->vertexBuffer is the storage location of the buffer, make this better, make it a return type?
    
    //Oh, I actually need things from the main program here that I don't have and don't want to waste time creating

    //The number of indices is similar to the number of ur mom
    CD3D11_BUFFER_DESC indexDesc(
	sizeof(u16) * parsedOBJData->faceLastIndex,
	D3D11_BIND_INDEX_BUFFER);

    D3D11_SUBRESOURCE_DATA indexData;
    ZeroMemory(&indexData, sizeof(D3D11_SUBRESOURCE_DATA));
    indexData.pSysMem = parsedOBJData->vertexIndices;
    indexData.SysMemPitch = 0;
    indexData.SysMemSlicePitch = 0;
    
    hr = device->CreateBuffer(
	&indexDesc,
	&indexData,
	&loadedBuffers->indexBuffer);

    loadedBuffers->indexCount = parsedOBJData->faceLastIndex;
}


#if 0
int main(void)
{
#if INITIALIZE_GAME_MEMORY
    memory_arena tempObjArena = {};
    program_memory tempProgramMemory = {};
    char* tempFileLocation = "../misc/cubetester2.obj";
#endif

#if RUN_PERFORMANCE_TIMER
    LARGE_INTEGER perfCountFrequencyResult;
    QueryPerformanceFrequency(&perfCountFrequencyResult);
    perfCountFrequency = perfCountFrequencyResult.QuadPart;
    LARGE_INTEGER startCounter = Win32GetWallClock();
#endif
//    DirectXLoadOBJ(tempFileLocation, &tempObjArena, &tempProgramMemory);
#if RUN_PERFORMANCE_TIMER
    LARGE_INTEGER endCounter = Win32GetWallClock();
    r32 msPerRun = (1000.0f * (Win32GetSecondsElapsed(startCounter, endCounter)));
    char textBuffer[256];
    sprintf_s(textBuffer, sizeof(textBuffer),
	      "Time Spend Running: %f\n",
	      msPerRun);
    
    OutputDebugString(textBuffer);
#endif    
    return(0);
}

#endif 
