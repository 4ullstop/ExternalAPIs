#include "obj_loader.h"
#include "../../FileReader/file_reader.cpp"
#include <stdio.h>
#include "../../MemoryPools/code/memory_pool_dll_include.h"
#include <libloaderapi.h>

/*
  The things to fix bc they are janky/piggy/bananacakes:
   - Rounding errors in StrToR32
   - 3 times through the vertex indices loops, could be more than 3 vertex indices
   - Not a huge fan of all of the while loops it takes to parse the file for multiple things
   
   Reach goal:
   - Create the ability to load in different OS besides windows
  
 */

global_variable memory_pool_dll_code memoryPoolCode;

#define INITIALIZE_GAME_MEMORY 1

enum data_format
{
    e_comment = 35,
    e_face = 102,
    e_vertex = 118,
    e_vertex_normal = 228,
    e_vertex_texture_coords = 234,
};


r32 StrToR32(char* character, i32 length)
{
    //Check for sign
    //Check for period
    //Change value if digit

    r32 result = 0.0f;

    i32 sign = 1;
    i32 i = 0;
    if (character[0] == '-')
    {
	sign = -1;
	i = 1;
    }

    //Start by calculating the value in the first loop of the value before the decimal
    //Then calculate the value in the second loop of the value after the decimal


    r32 place = 1;    
    while (character[++i] != '.')
    {
	if (character[i] == NULL)
	{
	    break;
	}
	place *= 10;
    }

    r32 resultBeforeDecimal = 0.0f;    
    if (i == 1)
    {
	i32 intVal = character[0] - '0';
	intVal *= (i32)place;
	resultBeforeDecimal += (r32)intVal;
    }
    

    for (i32 j = 1; j < i; j++)
    {
	
	i32 intVal = character[j] - '0';

	intVal *= (i32)place;

	resultBeforeDecimal += (r32)intVal;
	if (place == 0.0f)
	{
	    break;
	}
	place /= 10.0f;
    }

    place = 0.1f;

    //We are getting floating point rounding errors what do I do?
    r32 resultAfterDecimal = 0.0f;
    for (i32 j = i + 1; j < length; j++)
    {
	i32 intVal = character[j] - '0';
	r32 fVal = (r32)intVal;

	fVal *= place;

	resultAfterDecimal += fVal;
	
	place /= 10;
    }

    result = (resultBeforeDecimal + resultAfterDecimal) * sign; 
    
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

void ParseVertexData(char* vertexString, memory_arena* objArena, obj* objStorageLocation)
{
    //We can keep allocating the array every loop iteration and then just clear the entire pool at the end of the function


    //While there are only 3 vertex indices in this model, there could be more, this needs to be a different loop
    //such that we can run it as many times as there are indices
    find_string_value_data stringData = {};
    stringData.i = 2;
    stringData.start = stringData.i;
    for (i32 l = 0; l < 3; l++)
    {
	FindNextValueStr(vertexString, objArena, &stringData);
	r32 newValue = StrToR32(stringData.newString, stringData.stringLength);	

	//Still seg faulting on this i32
	objStorageLocation->vertexIndices[objStorageLocation->vertexLastIndex++] = newValue;

	stringData.start = stringData.i;
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

void ParseVertexNormals(char* vertexNormalString, memory_arena* objArena, obj* objStorageLocation)
{
    //This is repeated, find a way to make it not repeated plz...
    find_string_value_data stringData = {};
    stringData.i = 2;
    stringData.start = stringData.i;
    for (i32 i = 0; i < 3; i++)
    {
	FindNextValueStr(vertexNormalString, objArena, &stringData);
	r32 newValue = StrToR32(stringData.newString, stringData.stringLength);
	objStorageLocation->vertexNormals[objStorageLocation->vertexNormalLastIndex++] = newValue;
	stringData.start = stringData.i;
    }
}

obj* LoadOBJFile(char* fileLocation, memory_arena* objLocationArena, program_memory* mainProgramMemory)
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
	    while (parsee[++j] != '\n')
	    {
		if (parsee[j] == NULL)
		{
		    break;
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

	    } break;
	    case e_vertex:
	    {
		ParseVertexData(rowData, &objArena, result);
	    } break;
	    case e_vertex_normal:
	    {
		ParseVertexNormals(rowData, &objArena, result);
	    } break;
	    case e_vertex_texture_coords:
	    {
		
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

int main(void)
{
#if INITIALIZE_GAME_MEMORY
    memory_arena tempObjArena = {};
    program_memory tempProgramMemory = {};
    char* tempFileLocation = "../misc/OBJtester.obj";
#endif

    LoadOBJFile(tempFileLocation, &tempObjArena, &tempProgramMemory);
    return(0);
}

