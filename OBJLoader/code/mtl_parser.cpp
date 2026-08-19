#include "mtl_parser.h"
#include "../../FileReader/file_reader.cpp"

/*
   welcome to the most cowabunga oriented programmed code you've seen in your life
 */

#define INITIALIZE_GAME_MEMORY 1

enum mtl_data_format
{
    e_dissolve = 100,
    e_amb_col = 172,
    e_diffuse = 175,
    e_emmisive_exp = 176,
    e_opacity_density = 183,
    e_spec = 190,
    e_spec_exp = 193,
    e_new_mat = 211,    
    e_illum = 213,    
};

void
ClearArray(i32* array)
{
    for (i32 i = 0; i < ArrayCount(array); i++)
    {
	array[i] = 0;
    }
}

bool32 DoesDataFormatExist(i32 format)
{
    return((format == e_dissolve) ||
	   (format == e_amb_col) ||
	   (format == e_diffuse) ||
	   (format == e_emmisive_exp) ||
	   (format == e_opacity_density) ||
	   (format == e_spec) ||
	   (format == e_spec_exp) ||
	   (format == e_illum) ||
	   (format == e_new_mat));
}

internal char*
GetMaterialName(char* rowString, memory_arena* tempArena, memory_arena* staticArena, memory_pool_dll_code* memoryPoolCode)
{
    find_string_value_data stringData = {};
    stringData.i = 7;
    stringData.start = stringData.i;
    
    FindNextValueStr(rowString, tempArena, &stringData, memoryPoolCode);
    char tempName[256];

    i32 len = 0;
    for (i32 i = 0; stringData.newString[i] != '\n'; i++)
    {
	tempName[i] = stringData.newString[i];
	len = i;
    }
    char* result = (char*)memoryPoolCode->PushArraySized(staticArena, (sizeof(char*) * len));
    for (i32 i = 0; i < len + 1; i++)
    {
	result[i] = tempName[i];
    }
    return(result);
}

mtl* ParseMTLData(char* fileLocation, i32* outNumOfMaterials, memory_arena* tempArena, memory_arena* staticArena, program_memory* pg, memory_pool_dll_code* memoryPoolCode)
{

    mtl* result = 0;
    
    thread_context blank = {};
    debug_read_file_result fileResult = DEBUGPlatformReadEntireFile(&blank, fileLocation);
    if (fileResult.contentsSize <= 0)
    {
	return(nullptr);
    }

    char* charFileResults = (char*)fileResult.contents;
    char* parsee = charFileResults;

    i32 i = 0;
    i32 numOfMaterials = 0;
    while (parsee[++i] != NULL)
    {
	if (parsee[i] == '\n')
	{
	    i32 dataFormat = DetermineDataFormat(&parsee[i + 1]);
	    if (dataFormat == e_new_mat)
	    {
		//Get first material name func, use below in switch statement as well

		numOfMaterials++;
	    }
	}
    }

    *outNumOfMaterials = numOfMaterials;
    if (numOfMaterials <= 0)
    {
	return(nullptr);
    }
    
    result = (mtl*)memoryPoolCode->PushStruct(staticArena, sizeof(mtl) * numOfMaterials);
    i32 currMatIndex = -1;

    i = 0;

    char rowData[48];
    
    while (parsee[i] != NULL)
    {
	if (parsee[i] == '\n')
	{
	    i32 dataFormat = DetermineDataFormat(&parsee[i + 1]);

	    i32 j = i;
	    i32 k = 0;
	    if (parsee[++j] == NULL)
		break;
	    while (parsee[++j] != '\n')
	    {
		rowData[++k] = parsee[j];
	    }
	    rowData[k + 1] = ' ';
	    
	    if (DoesDataFormatExist(dataFormat))
	    {
		switch(dataFormat)
		{
		case e_dissolve:
		{
		    i32 storageOff = -1;
		    ParseFloatMembers(rowData,
				      tempArena,
				      &result[currMatIndex].dissolve,
				      &storageOff,
				      1,
				      memoryPoolCode);		    
		} break;
		case e_amb_col:
		{
		    i32 storageOn = 0;
		    ParseFloatMembers(rowData,
				      tempArena,
				      result[currMatIndex].ambient.e,
				      &storageOn,
				      3,
				      memoryPoolCode);
		} break;
		case e_diffuse:
		{
		    i32 storageOn = 0;
		    ParseFloatMembers(rowData,
				      tempArena,
				      result[currMatIndex].diffuse.e,
				      &storageOn,
				      3,
				      memoryPoolCode);
		} break;
		case e_emmisive_exp:
		{
		    i32 storageOn = 0;
		    ParseFloatMembers(rowData,
				      tempArena,
				      result[currMatIndex].emmisive.e,
				      &storageOn,
				      3,
				      memoryPoolCode);
		} break;
		case e_opacity_density:
		{
		    i32 storageOff = -1;
		    ParseFloatMembers(rowData,
				      tempArena,
				      &result[currMatIndex].opacityDensity,
				      &storageOff,
				      1,
				      memoryPoolCode);
		} break;
		case e_spec:
		{
		    i32 storageOn = 0;
		    ParseFloatMembers(rowData,
				      tempArena,
				      result[currMatIndex].specular.e,
				      &storageOn,
				      3,
				      memoryPoolCode);
		} break;
		case e_spec_exp:
		{

		    i32 storageOff = -1;
		    ParseFloatMembers(rowData,
				      tempArena,
				      &result[currMatIndex].specExp,				      
				      &storageOff,
				      1,
				      memoryPoolCode);

		} break;
		case e_new_mat:
		{
		    currMatIndex++;
		    result->name = GetMaterialName(&parsee[i], tempArena, staticArena, memoryPoolCode);		    
		} break;
		case e_illum:
		{
		    find_string_value_data blockString = {};
		    blockString.i = 5;
		    blockString.start = blockString.i;

		    FindNextValueStr(rowData, tempArena, &blockString, memoryPoolCode);
		    i32 newStart = 0;
		    i32 intVal = FindIntFromFaceValue(&newStart, &blockString.newString[newStart]);

		    result->illumination = intVal;
		} break;
		default:
		{

		} break;
		}
	    }
		    
	}
	i++;
    }

    return(result); 
}
    
