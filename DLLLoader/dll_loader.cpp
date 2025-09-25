/*
  This API requires that you implement your own function for filling out the information for the functions that will be loaded through the various created APIs, write the function in the API's code itself

  
  ******This is a bad explanation come back here and fix it when you have the mind to think about it more******
 */
#include "dll_loader.h"

internal void
Win32GetEXEFileName(win32_state_data* data)
{
    DWORD sizeOfFileName = GetModuleFileName(0, data->exeFileName, sizeof(data->exeFileName));
    data->onePastLastEXEFileNameSlash = data->exeFileName;

    for (char* scan = data->exeFileName; *scan; ++scan)
    {
	if (*scan == '\\')
	{
	    data->onePastLastEXEFileNameSlash= scan + 1;
	}
    }
}

internal i32
StringLength(char* string)
{
    i32 count = 0;
    while (*string++)
    {
	++count;
    }
    return(count);
}

internal void
CatStrings(size_t sourceACount, char* sourceA,
	   size_t sourceBCount, char* sourceB,
	   size_t destCount, char* dest)
{
    for (i32 index = 0; index < sourceACount; ++index)
    {
	*dest++ = *sourceA++;
    }
    for (i32 index = 0; index < sourceBCount; ++index)
    {
	*dest++ = *sourceB++;
    }
    *dest++ = 0;
}

void
Win32BuildEXEPathFileName(win32_state_data* data, char* fileName, i32 destCount, char* dest)
{
    CatStrings(data->onePastLastEXEFileNameSlash - data->exeFileName, data->exeFileName,
	       StringLength(fileName), fileName,
	       destCount, dest);
}

inline FILETIME
Win32GetLastWriteTime(char* fileName)
{
    FILETIME lastWriteTime = {};

    WIN32_FILE_ATTRIBUTE_DATA data;
    if (GetFileAttributesEx(fileName, GetFileExInfoStandard, &data))
    {
	lastWriteTime = data.ftLastWriteTime;
    }
    return(lastWriteTime);
}

//Returns failure code
#if 0
i32
Win32LoadDLLCode(char* sourceDLLName, i32 (*FillFunctions)(void*, win32_dll_code*), void* functionStruct)
{
    win32_state_data stateData = {};
    Win32GetEXEFileName(&stateData);
    
    char sourceDLLCodeFullPath[WIN32_STATE_FILE_NAME_COUNT];
    Win32BuildEXEPathFileName(&stateData, sourceDLLName, sizeof(sourceDLLCodeFullPath), sourceDLLCodeFullPath);

    char tempCodeDLLFullPath[WIN32_STATE_FILE_NAME_COUNT];
    Win32BuildEXEPathFileName(&stateData, "file_temp.dll", sizeof(tempCodeDLLFullPath), tempCodeDLLFullPath);

    char codeLockFullPath[WIN32_STATE_FILE_NAME_COUNT];
    Win32BuildEXEPathFileName(&stateData, "lock.tmp", sizeof(codeLockFullPath), codeLockFullPath);
    
    win32_dll_code win32Code = {};
    WIN32_FILE_ATTRIBUTE_DATA ignored;

    if (!GetFileAttributesEx(codeLockFullPath, GetFileExInfoStandard, &ignored))
    {
	win32Code.dllLastWriteTime = Win32GetLastWriteTime(sourceDLLCodeFullPath);
	CopyFile(sourceDLLName, tempCodeDLLFullPath, FALSE);
	win32Code.dllCode = LoadLibrary(tempCodeDLLFullPath);



	if (win32Code.dllCode)
	{
	    FillFunctions(functionStruct, &win32Code); //woah hello there void* function pointer this is so anonymous
	}
	if (!win32Code.isValid)
	{
	    functionStruct = 0;
	    return(1);
	}

    }
    else
    {
	return(2);
    }
    return(0);
}
#else
void
Win32LoadDLLCode(char* sourceDLLName, i32 (*FillFunctions)(void*, win32_dll_code*), void* functionStruct)
{
}
#endif
