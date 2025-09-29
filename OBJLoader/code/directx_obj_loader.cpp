#include "directx_obj_loader.h"
#include "obj_loader.h"
#include "obj_loader.cpp"

//This function is the function we will load as a dll, bc it is specific to the operating system
void DirectXLoadOBJ(char* fileLocation, memory_arena* objLocationArena, program_memory* mainProgramMemory)
{
    obj* parsedOBJData = ParseOBJData(fileLocation, objLocationArena, mainProgramMemory);
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
//    ParseOBJData(tempFileLocation, &tempObjArena, &tempProgramMemory);
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

