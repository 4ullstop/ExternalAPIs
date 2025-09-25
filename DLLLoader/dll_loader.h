#if !defined(DLL_LOADER_H)

//Please please please go and find where these function exist so I'm only including the functions I need
#include <windows.h>

#define WIN32_STATE_FILE_NAME_COUNT MAX_PATH

struct win32_dll_code
{
    HMODULE dllCode;
    FILETIME dllLastWriteTime;

    bool32 isValid;
};

struct win32_state_data
{
    char exeFileName[WIN32_STATE_FILE_NAME_COUNT];
    char* onePastLastEXEFileNameSlash;
};

#define DLL_LOADER_H
#endif
