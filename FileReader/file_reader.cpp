#include "file_reader.h"
//At some point only include the files you actually need
#include <windows.h>

DEBUG_PLATFORM_FREE_FILE_MEMORY(DEBUGPlatformFreeFileMemory)
{
    VirtualFree(memory, 0, MEM_RELEASE);
}

DEBUG_PLATFORM_READ_ENTIRE_FILE(DEBUGPlatformReadEntireFile)
{
    debug_read_file_result result = {};

    //This might try to create a file if it doesn't exist, might want to fix it so it doesn't
    HANDLE fileHandle = CreateFile(fileName,
				   GENERIC_READ,
				   FILE_SHARE_READ,
				   0,
				   OPEN_EXISTING,
				   0,
				   0);

    if (fileHandle != INVALID_HANDLE_VALUE)
    {
	LARGE_INTEGER fileSize;
	if (GetFileSizeEx(fileHandle, &fileSize))
	{
	    u32 fileSize32 = SafeTruncateU64(fileSize.QuadPart);	    

	    result.contents = VirtualAlloc(0, fileSize32, MEM_RESERVE|MEM_COMMIT, PAGE_READWRITE);
	    if (result.contents)
	    {
		DWORD bytesRead;
		if (ReadFile(fileHandle,
			     result.contents,
			     fileSize32,
			     &bytesRead,
			     0) &&
		    (fileSize32 == bytesRead))
		{
		    result.contentsSize = fileSize32;
		}
		else
		{
		    DEBUGPlatformFreeFileMemory(thread, result.contents);
		    result.contents = 0;
		}
	    }
	    else
	    {

	    }
	}
	else
	{

	}
	CloseHandle(fileHandle);
    }
    else
    {
	
    }
    return(result);
}

DEBUG_PLATFORM_WRITE_ENTIRE_FILE(DEBUGPlatformWriteEntireFile)
{
    bool32 result = 0;
    HANDLE fileHandle = CreateFile(fileName,
				   GENERIC_WRITE,
				   0,
				   0,
				   CREATE_ALWAYS,
				   0,
				   0);
    if (fileHandle != INVALID_HANDLE_VALUE)
    {
	DWORD bytesWritten;
	if (WriteFile(fileHandle, memory, memorySize, &bytesWritten, 0))
	{
	    result = (bytesWritten == memorySize);
	}
	else
	{

	}
	CloseHandle(fileHandle);
    }
    return(result);
}
