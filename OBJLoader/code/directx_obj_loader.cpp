#include "directx_obj_loader.h"
#include "obj_loader.h"
#include "obj_loader.cpp"

//This function is the function we will load as a dll, bc it is specific to the operating system
void DirectXLoadOBJ(char* fileLocation, memory_arena* objLocationArena, program_memory* mainProgramMemory)
{
    obj* parsedOBJData = ParseOBJData(fileLocation, objLocationArena, mainProgramMemory);

    size_t objVertsSize = sizeof(vertex_position_color) * parsedOBJData->vertexCount;
    
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
    CD3D11_BUFFER_DESC vertexDesc(
	objVertsSize,
	D3D11_BIND_VERTEX_BUFFER);

    D3D11_SUBRESOURCE_DATA vertexData;
    ZeroMemory(&vertexData, sizeof(D3D11_SUBRESOURCE_DATA));
    vertexData.pSysMem = objVerts;
    vertexData.SysMemPitch = 0;
    vertexData.SysMemSlicePitch = 0;

    //Oh, I actually need things from the main program here that I don't have and don't want to waste time creating
}

int main(void)
{
#if INITIALIZE_GAME_MEMORY
    memory_arena tempObjArena = {};
    program_memory tempProgramMemory = {};
    char* tempFileLocation = "../misc/OBJtester.obj";
#endif

#if RUN_PERFORMANCE_TIMER
    LARGE_INTEGER perfCountFrequencyResult;
    QueryPerformanceFrequency(&perfCountFrequencyResult);
    perfCountFrequency = perfCountFrequencyResult.QuadPart;
    LARGE_INTEGER startCounter = Win32GetWallClock();
#endif
    DirectXLoadOBJ(tempFileLocation, &tempObjArena, &tempProgramMemory);
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

