#if !defined PARSER_SHARED_CPP

i32 DetermineDataFormat(char* string)
{
    i32 result = (i32)string[0];
    if (!(string[1] == ' '))
    {
	result += (i32)string[1];
    }
    return(result);
}

struct find_string_value_data
{
    char* newString;
    i32 stringLength;

    i32 i;
    i32 start;
};

void FindNextValueStr(char* rowString, memory_arena* objArena, find_string_value_data* stringData, memory_pool_dll_code* memoryPoolCode)
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

    stringData->newString = (char*)memoryPoolCode->PushArraySized(objArena, sizeof(char*) * stringData->stringLength);

    for (i32 k = stringData->start, f = 0; k < stringData->i; k++)
    {
	if (rowString[k] != ' ')
	{
	    stringData->newString[f] = rowString[k];
	    f++;
	}
    }
}



void ParseFloatMembers(char* rowString, memory_arena* objArena, r32* storageArray, i32* storageIndex, i32 itemsPerRow, memory_pool_dll_code* memoryPoolCode)
{
    find_string_value_data stringData = {};
    stringData.i = 2;
    stringData.start = stringData.i;
    for (i32 i = 0; i < itemsPerRow; i++)
    {
	FindNextValueStr(rowString, objArena, &stringData, memoryPoolCode);
	r32 convertedFloatValue = (r32)atof(stringData.newString);
	if ((*storageIndex >= 0))
	    storageArray[(*storageIndex)++] = convertedFloatValue;
	else
	    *storageArray = convertedFloatValue;
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
#define PARSER_SHARED_CPP
#endif
