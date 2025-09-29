#include "obj_loader.h"
#include "../../FileReader/file_reader.cpp"
#include <stdio.h>
#include <stdlib.h>
#include "../../MemoryPools/code/memory_pool_dll_include.h"
#include <libloaderapi.h>
#include "../../Types/intrinsics.h"

#define RUN_PERFORMANCE_TIMER 1

#if RUN_PERFORMANCE_TIMER
#include <windows.h>
#endif

/*
  Yeah... this is bad lmao

  The things to fix bc they are janky/piggy/bananacakes:
   - 3 times through the vertex indices loops, could be more than 3 vertex indices
   - Not a huge fan of all of the while loops it takes to parse the file for multiple things
   
   Reach goal:
   - Create the ability to load in different OS besides windows


   YOUR INSTRUCTION SET:
   SSE4.1, SSE4.2 AVX2 (64 bit)
 */

/*
  How can you optimize this:
   - Uhhhhhhh, not quite as many loops probably
   - Find a better way to search through char*
   - Use intrinsics
   - Bit shifting?
   - Finally use multiple threads, but thats like the last resort
 */

/*
  Surprisingly, this isn't that slow...
  Order of importance:
   - Throw into Direct3D
 */

global_variable memory_pool_dll_code memoryPoolCode;
global_variable i64 perfCountFrequency;
#define INITIALIZE_GAME_MEMORY 1

enum data_format
{
    e_comment = 35,
    e_face = 102,
    e_vertex = 118,
    e_vertex_normal = 228,
    e_vertex_texture_coords = 234,
};

/*
  At some point make our timer it's own library when I feel like it but rn just doing some tests so it don't rly matter
 */



inline LARGE_INTEGER
Win32GetWallClock(void)
{
    LARGE_INTEGER result;
    QueryPerformanceCounter(&result);
    return(result);
}

inline r32
Win32GetSecondsElapsed(LARGE_INTEGER start, LARGE_INTEGER end)
{
    r32 result = ((r32)(end.QuadPart - start.QuadPart) / (r32)perfCountFrequency);
    return(result);
}


struct find_string_value_data
{
    char* newString;
    i32 stringLength;

    i32 i;
    i32 start;
};

void FindNextValueStr(char* rowString, memory_arena* objArena, find_string_value_data* stringData)
{
    stringData->newString = 0;
    while(rowString[++stringData->i] != ' ')
    {
	if (rowString[stringData->i] == NULL)
	{
	    break;
	}
    }

    stringData->stringLength = stringData->i - stringData->start;

    stringData->newString = (char*)memoryPoolCode.PushArraySized(objArena, sizeof(char*) * stringData->stringLength);

    for (i32 k = stringData->start, f = 0; k < stringData->i; k++)
    {
	if (rowString[k] != ' ')
	{
	    stringData->newString[f] = rowString[k];
	    f++;
	}
    }
}

i32 DetermineDataFormat(char* string)
{
    i32 result = (i32)string[0];
    if (!(string[1] == ' '))
    {
	result += (i32)string[1];
    }
    return(result);
}

void ParseFloatMembers(char* rowString, memory_arena* objArena, r32* storageArray, i32* storageIndex, i32 itemsPerRow)
{
    find_string_value_data stringData = {};
    stringData.i = 2;
    stringData.start = stringData.i;
    for (i32 i = 0; i < itemsPerRow; i++)
    {
	FindNextValueStr(rowString, objArena, &stringData);
	r32 convertedFloatValue = (r32)atof(stringData.newString);
	storageArray[(*storageIndex)++] = convertedFloatValue;
	stringData.start = stringData.i;
    }
}

i32 FindIntFromFaceValue(i32* startLocation, char* stringValue)
{
    i32 i = 0, place = 1, j = 0, result = 0;

    while (stringValue[++i] != '/')
    {
	if (stringValue[i] == NULL)
	{
	    break;
	}
	place *= 10;
    }

    while (stringValue[j] != '/')
    {
	if (stringValue[j] == NULL)
	{
	    break;
	}
	i32 intVal = stringValue[j] - '0';
	intVal *= place;
	result += intVal;
	place /= 10;
	j++;
    }
    *startLocation = *startLocation + (j + 1);
    return(result);
}

void ParseFaceValues(char* rowString, memory_arena* objArena, i32* storageArray, i32* storageIndex, i32 itemsPerRow)
{
    find_string_value_data blockString = {};
    blockString.i = 2;
    blockString.start = blockString.i;
    for (i32 i = 0; i < itemsPerRow; i++)
    {
	//This doesn't seem to work well for this portion
	FindNextValueStr(rowString, objArena, &blockString);
	
	find_string_value_data integerString = {};
	//This needs to happen 3 times
	i32 newStartLocation = 0;
	for (i32 j = 0; j < 3; j++)
	{
	    //Update the position at which to check the string for new values
	    i32 integer = FindIntFromFaceValue(&newStartLocation, &blockString.newString[newStartLocation]);
	    storageArray[(*storageIndex)++] = integer;
	}
	blockString.start = blockString.i;
    }
}

obj* ParseOBJData(char* fileLocation, memory_arena* objLocationArena, program_memory* mainProgramMemory)
{
    //Replace this with paramter

    /*
      It is asssumed that this location has already been initialized within the main program
      
     */
    
    //A million years from now when you decide to optimize this
    /*
      Open up multiple threads to search for the information after first parse?
      Read each part of the obj file in the separate thread and run it together

      Needs to be specific to the operating system used (win32), so we can
      put it into the proper format for the API

      
     */

    obj* result = 0;
    
    //Yes this is specific to win32, fix it later...
    HMODULE memoryPoolLibrary = LoadLibrary("../../MemoryPools/dll/memory_pools.dll");

    if (memoryPoolLibrary)
    {
	memoryPoolCode.PushStruct = (memory_pool_push_struct*)GetProcAddress(memoryPoolLibrary, "PushStruct");
	memoryPoolCode.PushArray = (memory_pool_push_array*)GetProcAddress(memoryPoolLibrary, "PushArray");
	memoryPoolCode.PoolAlloc = (memory_pool_alloc*)GetProcAddress(memoryPoolLibrary, "PoolAlloc");
	memoryPoolCode.InitializeArena = (memory_pool_initialize_arena*)GetProcAddress(memoryPoolLibrary, "InitializeArena");
	memoryPoolCode.ClearArena = (memory_pool_clear_arena*)GetProcAddress(memoryPoolLibrary, "ClearArena");
	memoryPoolCode.PushArraySized = (memory_pool_push_array_sized*)GetProcAddress(memoryPoolLibrary, "PushArraySized");
    }

    thread_context blankThread = {};
    debug_read_file_result fileResult = DEBUGPlatformReadEntireFile(&blankThread, fileLocation);

    char* charFileResults = (char*)fileResult.contents;
    
    program_memory memory = {};
    memory.permanentStorageSize = Megabytes(10);
    memory.transientStorageSize = 0;
    memoryPoolCode.PoolAlloc(0, MEM_RESERVE|MEM_COMMIT, PAGE_READWRITE, &memory);
    memory_arena objArena = {};
    memoryPoolCode.InitializeArena(&objArena, memory.permanentStorageSize, (u8*)memory.permanentStorage);

    
    //Intializing the "pre-allocated" (via main program) memory pool for testing purposes
#if INITIALIZE_GAME_MEMORY
    mainProgramMemory->permanentStorageSize = Megabytes(64);
    mainProgramMemory->transientStorageSize = 0;
    memoryPoolCode.PoolAlloc(0, MEM_RESERVE|MEM_COMMIT, PAGE_READWRITE, mainProgramMemory);
    memoryPoolCode.InitializeArena(objLocationArena, mainProgramMemory->permanentStorageSize, (u8*)mainProgramMemory->permanentStorage);
#endif    

    obj tester = {};
    
    result = (obj*)memoryPoolCode.PushStruct(objLocationArena, sizeof(tester));
    
    char* parsee = charFileResults;

    i32 i = 0;

    //Determine the amount of verts, faces, normals etc... for allocation purposes

    //I don't like this but I'm too lazy to figure out a better solution atm
    while (parsee[++i] != NULL)
    {
	if (parsee[i] == '\n')
	{
	    i32 dataFormat = DetermineDataFormat(&parsee[i + 1]);

	    switch(dataFormat)
	    {
	    case e_face:
	    {
		result->faceCount++;
	    } break;
	    case e_vertex:
	    {
		result->vertexCount++;
	    } break;
	    case e_vertex_normal:
	    {
		result->vertexNormalCount++;
	    } break;
	    case e_vertex_texture_coords:
	    {
		result->vertexTextureCoordCount++;
	    } break;
	    default:
	    {
		
	    } break;
	    }
	}
    }

    i = 0;


#if 1    
    result->vertexIndices = (r32*)memoryPoolCode.PushArraySized(objLocationArena, (sizeof(r32) * result->vertexCount) * 3);
    result->vertexNormals = (r32*)memoryPoolCode.PushArraySized(objLocationArena, (sizeof(r32) * result->vertexNormalCount) * 3);
    result->vertexTextureCoordinates = (r32*)memoryPoolCode.PushArraySized(objLocationArena, (sizeof(r32) * result->vertexTextureCoordCount) * 2);
    result->face = (i32*)memoryPoolCode.PushArraySized(objLocationArena, (sizeof(i32) * result->faceCount) * 3);    

#else    
    result->vertexIndices = (r32*)memoryPoolCode.PushArray(objLocationArena, result->vertexCount, result->vertexIndices);
    result->vertexNormals = (r32*)memoryPoolCode.PushArray(objLocationArena, result->vertexNormalCount, result->vertexNormals);
    result->vertexTextureCoordinates = (r32*)memoryPoolCode.PushArray(objLocationArena, result->vertexTextureCoordCount, result->vertexTextureCoordinates);
    result->face = (i32*)memoryPoolCode.PushArray(objLocationArena, result->faceCount, result->face);

#endif
    
    while (parsee[i] != NULL)
    {
	if (parsee[i] == '\n')
	{
	    //Find next line
	    //Figure out the proper designation for the value of the line (v, f, vt...)

	    i32 desVal = DetermineDataFormat(&parsee[i + 1]);

	    i32 j = i + 1;
	    i32 itemsPerRow = 0;
	    while (parsee[++j] != '\n')
	    {
		if (parsee[j] == NULL)
		{
		    break;
		}
		if (parsee[j] == ' ')
		{
		    itemsPerRow++;
		}
	    }

	    

	    i32 rowDataLen = j - i;
	    
	    char* rowData = (char*)memoryPoolCode.PushArray(&objArena, rowDataLen, parsee);

	    for (i32 k = i + 1, f = 0; k < j; k++, f++)
	    {
		rowData[f] = parsee[k];
	    }

	    switch (desVal)
	    {
	    case e_comment:
	    {
		//It was a comment
		i32 foo = 0;

	    } break;
	    case e_face:
	    {
		ParseFaceValues(rowData,
				&objArena,
				result->face,
				&result->faceLastIndex,
				itemsPerRow);
	    } break;
	    case e_vertex:
	    {
		ParseFloatMembers(rowData,
				  &objArena,
				  result->vertexIndices,
				  &result->vertexLastIndex,
				  itemsPerRow);
	    } break;
	    case e_vertex_normal:
	    {
		ParseFloatMembers(rowData,
				  &objArena,
				  result->vertexNormals,
				  &result->vertexNormalLastIndex,
				  itemsPerRow);
	    } break;
	    case e_vertex_texture_coords:
	    {
		ParseFloatMembers(rowData,
				  &objArena,
				  result->vertexTextureCoordinates,
				  &result->vertexTextureCoordLastIndex,
				  itemsPerRow);
	    } break;
	    default:
	    {
		//Error here
	    } break;
	    }	    

	}
	i++;
	PoolClear(&objArena);
    }

    return(result);
}

/*
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
    ParseOBJData(tempFileLocation, &tempObjArena, &tempProgramMemory);

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

*/
