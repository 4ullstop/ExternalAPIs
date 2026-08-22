#include "obj_loader.h"
#include "../../FileReader/file_reader.cpp" 
#include <stdio.h>
#include <stdlib.h>
#include "../../MemoryPools/code/memory_pool_dll_include.h"
#include <libloaderapi.h>
#include "../../Types/intrinsics.h"
#include "parser_shared.cpp"

#define RUN_PERFORMANCE_TIMER 1

#if RUN_PERFORMANCE_TIMER
#include <windows.h>
#endif

/*
  Currently, it is intended that the OBJ file you load in is created and exported through blender and it
  must be ensured that the faces are triangulated or else the algorithm won't work.
 */

/*
  The things to fix bc they are janky/piggy/bananacakes:
   - Not a huge fan of all of the while loops it takes to parse the file for multiple things
   
   Reach goal(s):
   - Create the ability to load in different OS besides windows
   - Create the ability to read data that is different than what the Blender OBJ exporter defaults to
   (i.e. the different ways in which faces are loaded in, what to do when there is no data for certain
   data formats and so on...)
   - Materials on the object? 


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
   - Redo the way faces are read in
   - Throw into Direct3D
 */


global_variable i64 perfCountFrequency;
#define INITIALIZE_GAME_MEMORY 0

enum data_format
{
    e_comment = 35,
    e_face = 102,
    e_vertex = 118,
    e_vertex_normal = 228,
    e_mat = 232,
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

void ParseFaceValues(char* rowString, memory_arena* objArena, obj* result, i32 itemsPerRow, memory_pool_dll_code* memoryPoolCode, i32 materialIndex, i32* currentFace)
{
    find_string_value_data blockString = {};
    blockString.i = 2;
    blockString.start = blockString.i;
    
    for (i32 i = 0; i < itemsPerRow; i++)
    {
	FindNextValueStr(rowString, objArena, &blockString, memoryPoolCode);
	
	find_string_value_data integerString = {};
	//This needs to happen 3 times
	i32 newStartLocation = 0;

	//Again this only accounts for the v/vt/vn face set up and not the others
	i32 storageIndex = result->faceLastIndex;
	i32 integer = FindIntFromFaceValue(&newStartLocation, &blockString.newString[newStartLocation]);
	result->vertexIndices[storageIndex] = (u16)integer;

	result->faces[*currentFace].e[i] = integer;
	if (result->hasMaterials)
	{
	    Assert(itemsPerRow < 4); //did you triangulate the mesh?
	    result->facesToMaterialIndices[*currentFace] = materialIndex;
	}
	
	integer = FindIntFromFaceValue(&newStartLocation, &blockString.newString[newStartLocation]);
	result->vertexTextureCoordIndices[storageIndex] = (u16)integer;

	integer = FindIntFromFaceValue(&newStartLocation, &blockString.newString[newStartLocation]);
	result->vertexNormalIndices[storageIndex] = (u16)integer;	

	result->faceLastIndex++;

	blockString.start = blockString.i;
    }
    *currentFace = *currentFace + 1;
}

internal char*
CopyMatName(char* name, memory_arena* tempArena, memory_pool_dll_code* memoryPoolCode)
{
    i32 i = 0;

    while (name[i] != '\0')
    {
	i++;
    }
    char* result = (char*)memoryPoolCode->PushArraySized(tempArena, (sizeof(char) * i));
    
    for (i32 k = 0; k < i; k++)
    {
	result[k] = name[k];
    }

    return(result);
}

struct face_per_material_t_data
{
    i32 facesPerMaterial;
};

obj* ParseOBJData(char* fileLocation, memory_arena* tempArena, memory_arena* staticArena, program_memory* mainProgramMemory, memory_pool_dll_code* memoryPoolCode)
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

#if 0    
    HMODULE memoryPoolLibrary = LoadLibrary("D:/ExternalCustomAPIs/MemoryPools/dll/memory_pools.dll");

    DWORD lastError = {};
    lastError = GetLastError();    
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
#endif
    
    //Intializing the "pre-allocated" (via main program) memory pool for testing purposes
#if INITIALIZE_GAME_MEMORY
    mainProgramMemory->permanentStorageSize = Megabytes(64);
    mainProgramMemory->transientStorageSize = 0;
    memoryPoolCode.PoolAlloc(0, MEM_RESERVE|MEM_COMMIT, PAGE_READWRITE, mainProgramMemory);
    memoryPoolCode.InitializeArena(tempArena, mainProgramMemory->permanentStorageSize, (u8*)mainProgramMemory->permanentStorage);
#endif    

    thread_context blankThread = {};
    debug_read_file_result fileResult = DEBUGPlatformReadEntireFile(&blankThread, fileLocation);

    char* charFileResults = (char*)fileResult.contents;
    
    obj tester = {};
    
    result = (obj*)memoryPoolCode->PushStruct(staticArena, sizeof(tester));
    
    char* parsee = charFileResults;

    i32 i = 0;

    //Determine the amount of verts, faces, normals etc... for allocation purposes

    //I don't like this but I'm too lazy to figure out a better solution atm

    //Create a linked list here to determine the material names and faces that exist with each other
    //Then use this list in the main while loop to determine the amount of v4I to allocate



    i32 numOfMaterials = 0;
    bool32 ontoMat = false;
    i32 facePerMaterial = 0;
    bool32 initList = false;
    listed_memory* facePerMaterialList = 0;
    listed_memory_node* facePerMaterialNodes = 0;
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
	    case e_mat:
	    {
		numOfMaterials++;
		if (!ontoMat)
		{
		    ontoMat = true;
		}
		else
		{
		    //do stuffs w/listed materials
		    facePerMaterial = 0;
		}
	    };
	    default:
	    {
		
	    } break;
	    }
	    if (ontoMat)
	    {
		facePerMaterial++;
	    }
	}
	
    }

    if (numOfMaterials > 0)
    {
	result->hasMaterials = true;
    }
    i = 0;



    result->vertices = (r32*)memoryPoolCode->PushArraySized(tempArena, (sizeof(r32) * result->vertexCount) * 3);
    result->vertexNormals = (r32*)memoryPoolCode->PushArraySized(tempArena, (sizeof(r32) * result->vertexNormalCount) * 3);
#if 0
    result->vertexTextureCoordinates = (r32*)memoryPoolCode->PushArraySized(tempArena, (sizeof(r32) * result->vertexTextureCoordCount) * 2);
#endif
    result->vertexTextureCoordinates = (v2*)memoryPoolCode->PushArraySized(tempArena, sizeof(v2) * result->vertexTextureCoordCount);
    result->face = (i32*)memoryPoolCode->PushArraySized(tempArena, (sizeof(i32) * result->faceCount) * 3);    

    result->facesToMaterialIndices = (i32*)memoryPoolCode->PushArraySized(tempArena, (sizeof(i32) * result->faceCount));
    result->faces = (v4I*)memoryPoolCode->PushArraySized(tempArena, (sizeof(v4I) * result->faceCount));
    if (numOfMaterials > 0)
    {
	result->materialNames = (char**)memoryPoolCode->PushArraySized(tempArena, (sizeof(char*) * numOfMaterials));
    }
    i32 materialIndex = -1;



    i32 currentFace = 0;
    bool32 faceMemoryInitialized = false;
    while (parsee[i] != NULL)
    {
	if (parsee[i] == '\n')
	{
	    //Find next line
	    //Figure out the proper designation for the value of the line (v, f, vt...)

	    i32 dataFormat = DetermineDataFormat(&parsee[i + 1]);

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
	    
	    char* rowData = (char*)memoryPoolCode->PushArray(tempArena, rowDataLen, parsee);

	    for (i32 k = i + 1, f = 0; k < j; k++, f++)
	    {
		rowData[f] = parsee[k];
	    }

	    switch (dataFormat)
	    {
	    case e_comment:
	    {
		//It was a comment
		i32 foo = 0;

	    } break;
	    case e_face:
	    {
		if (!faceMemoryInitialized)
		{
		    result->vertexIndices = (u16*)memoryPoolCode->PushArraySized(tempArena,
										(sizeof(i32) * result->faceCount)
										* itemsPerRow);
		    result->vertexNormalIndices = (u16*)memoryPoolCode->PushArraySized(tempArena,
										      (sizeof(i32) *
										       result->faceCount) *
										      itemsPerRow);
		    result->vertexTextureCoordIndices = (u16*)memoryPoolCode->PushArraySized(tempArena,
											    (sizeof(i32) *
											    result->faceCount) *
											    itemsPerRow);
		    faceMemoryInitialized = true;
		}
		
		ParseFaceValues(rowData,
				tempArena,
				result,
				itemsPerRow,
				memoryPoolCode,
				materialIndex,
				&currentFace);
	    } break;
	    case e_vertex:
	    {
		ParseFloatMembers(rowData,
				  tempArena,
				  result->vertices,
				  &result->vertexLastIndex,
				  itemsPerRow,
				  memoryPoolCode);
	    } break;
	    case e_vertex_normal:
	    {
		ParseFloatMembers(rowData,
				  tempArena,
				  result->vertexNormals,
				  &result->vertexNormalLastIndex,
				  itemsPerRow,
				  memoryPoolCode);
	    } break;

	    case e_mat:
	    {
		find_string_value_data blockString = {};
		blockString.i = 6;
		blockString.start = blockString.i;
		FindNextValueStr(rowData, tempArena, &blockString, memoryPoolCode);
		//Find the material in the material format after getting it's name
		//Count faces for material?
		materialIndex++;

		result->materialNames[materialIndex] = CopyMatName(blockString.newString,
								     tempArena,
								     memoryPoolCode);

	    } break;

	    case e_vertex_texture_coords:
	    {
		i32 index = 0;
		ParseFloatMembers(rowData,
				  tempArena,
				  result->vertexTextureCoordinates[result->vertexTextureCoordLastIndex++].e,
				  &index,
				  itemsPerRow,
				  memoryPoolCode);
	    } break;
	    default:
	    {
		//Error here
	    } break;
	    }	    

	}
	i++;
	PoolClear(tempArena);
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
