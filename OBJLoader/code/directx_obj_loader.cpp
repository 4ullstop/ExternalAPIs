#include "directx_obj_loader.h"
#include "obj_loader.h"
#include "obj_loader.cpp"

//This function is the function we will load as a dll, bc it is specific to the operating system
void DirectXLoadOBJ(char* fileLocation, memory_arena* objLocationArena, program_memory* mainProgramMemory)
{
    obj* parsedOBJData = ParseOBJData(fileLocation, objLocationArena, mainProgramMemory);


    
    vertex_position_color* objVerts =
	(vertex_position_color*)memoryPoolCode.PushArraySized(objLocationArena,
							      sizeof(vertex_position_color) * parsedOBJData->vertexCount);

    for (i32 i = 0, j = 0; i < (parsedOBJData->vertexCount * 3); i += 3, j++)
    {
	objVerts[j].pos.x = parsedOBJData->vertices[i];
	objVerts[j].pos.y = parsedOBJData->vertices[i + 1];
	objVerts[j].pos.z = parsedOBJData->vertices[i + 2];
    }

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

