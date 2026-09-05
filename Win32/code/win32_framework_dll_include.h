//define dll functions to be used here
/*
  _____________________________________________
  You must have the MemoryPoolDLL code initialized within your code base if not this code will not work
  _____________________________________________
  
 */
#if !defined WIN32_FRAMEWORK_DLL_INCLUDE_H
#include "win32_framework.h"

#define WIN32_FRAMEWORK_PROCESS_PENDING_MESSAGES(name) void name(game_controller_input* keyboardController, game_controller_input* oldKeyboardController, game_input* newInput, game_input* oldInput, mouse_movements* mouse, memory_pool_dll_code* memoryPoolCode, memory_arena* perFrameArena, program_state* programState)
typedef WIN32_FRAMEWORK_PROCESS_PENDING_MESSAGES(win32_process_pending_messages);
WIN32_FRAMEWORK_PROCESS_PENDING_MESSAGES(Win32ProcessPendingMessagesStub)
{

}
global_variable win32_process_pending_messages* Win32ProcessPendingMessages_ = Win32ProcessPendingMessagesStub;
#define Win32ProcessPendingMessages Win32ProcessPendingMessages_

#if 0
#define WIN32_INIT_FRAMEWORK(name) void name(void)
typedef WIN32_INIT_FRAMEWORK(win32_init_framework);
WIN32_INIT_FRAMEWORK(Win32InitFrameworkStub)
{
    
}
global_variable win32_init_framework* Win32InitFramework_ = Win32InitFrameworkStub;
#define Win32InitFramework Win32InitFramework_
#endif

#define WIN32_LOAD_GAME_CODE(name) win32_game_code name(game_code_path_cluster* cluster, char* updateFuncName, char* initFuncName, memory_pool_dll_code* memoryPoolCode, memory_arena* arena)
typedef WIN32_LOAD_GAME_CODE(win32_load_game_code);
WIN32_LOAD_GAME_CODE(Win32LoadGameCodeStub)
{
    win32_game_code result = {};
    return(result);
}
global_variable win32_load_game_code* Win32LoadGameCode_ = Win32LoadGameCodeStub;
#define Win32LoadGameCode Win32LoadGameCode_
    
#define WIN32_GET_EXE_FILENAME(name) void name(program_state* state)
typedef WIN32_GET_EXE_FILENAME(win32_get_exe_filename);
WIN32_GET_EXE_FILENAME(Win32GetEXEFilenameStub)
{
    
}
global_variable win32_get_exe_filename* Win32GetEXEFilename_ = Win32GetEXEFilenameStub;
#define Win32GetEXEFilename Win32GetEXEFilename_

#define WIN32_CHECK_AND_LOAD_GAME_CODE(name) i32 name(game_code_path_cluster* paths, win32_game_code* gameCode, memory_pool_dll_code* memoryPoolCode, memory_arena* arena)
typedef WIN32_CHECK_AND_LOAD_GAME_CODE(win32_check_and_load_game_code);
WIN32_CHECK_AND_LOAD_GAME_CODE(Win32CheckAndLoadGameCodeStub)
{
    return(0);
}
global_variable win32_check_and_load_game_code* Win32CheckAndLoadGameCode_ = Win32CheckAndLoadGameCodeStub;
#define Win32CheckAndLoadGameCode Win32CheckAndLoadGameCode_

#define WIN32_GAME_CODE_SETUP(name) game_code_path_cluster name(char* layerDll, char* tempDll, char* lockDll, program_state* state)
typedef WIN32_GAME_CODE_SETUP(win32_game_code_setup);
WIN32_GAME_CODE_SETUP(Win32GameCodeSetupStub)
{
    game_code_path_cluster result = {};
    return(result);
}
global_variable win32_game_code_setup* Win32GameCodeSetup_ = Win32GameCodeSetupStub;
#define Win32GameCodeSetup Win32GameCodeSetup_

#define WIN32_CREATE_SPAWNABLE_BUFFERS(name) void name(game_loaded_objs* gameObjs, game_loaded_textures* gameTextures, win32_spawnable_objs* win32Objs, memory_arena* objArena, memory_arena* tempArena, memory_pool_dll_code* memoryPoolCode, ID3D11Device* d3dDevice)
typedef WIN32_CREATE_SPAWNABLE_BUFFERS(win32_create_spawnable_buffers);
WIN32_CREATE_SPAWNABLE_BUFFERS(Win32CreateSpawnableBuffersStub)
{

}
global_variable win32_create_spawnable_buffers* Win32CreateSpawnableBuffers_ = Win32CreateSpawnableBuffersStub;
#define Win32CreateSpawnableBuffers Win32CreateSpawnableBuffers_

#define WIN32_FROM_V4_TO_XMVECTOR(name) DirectX::XMVECTOR name(v4 v)
typedef WIN32_FROM_V4_TO_XMVECTOR(win32_from_v4_to_xmvector);
WIN32_FROM_V4_TO_XMVECTOR(Win32FromV4ToXMVECTORStub)
{
    DirectX::XMVECTOR result = {};
    return(result);
}
global_variable win32_from_v4_to_xmvector* Win32FromV4ToXMVECTOR_ = Win32FromV4ToXMVECTORStub;
#define Win32FromV4ToXMVECTOR Win32FromV4ToXMVector_

#define WIN32_CONVERT_GAME_CAMERA_TO_WIN32(name) void name(dx_camera* dxCam, game_camera_data* gCamData)
typedef WIN32_CONVERT_GAME_CAMERA_TO_WIN32(win32_convert_game_camera_to_win32);
WIN32_CONVERT_GAME_CAMERA_TO_WIN32(Win32ConvertGameCameraToWin32Stub)
{

}
global_variable win32_convert_game_camera_to_win32* Win32ConvertGameCameraToWin32_ = Win32ConvertGameCameraToWin32Stub;
#define Win32ConvertGameCameraToWin32 Win32ConvertGameCameraToWin32_

#define WIN32_FROM_M4_TO_XMMATRIX(name) DirectX::XMMATRIX name(m4 m)
typedef WIN32_FROM_M4_TO_XMMATRIX(win32_from_m4_to_xmmatrix);
WIN32_FROM_M4_TO_XMMATRIX(Win32FromM4ToXMMATRIXStub)
{
    DirectX::XMMATRIX result = {};
    return(result);
}
global_variable win32_from_m4_to_xmmatrix* Win32FromM4ToXMMATRIX_ = Win32FromM4ToXMMATRIXStub;
#define Win32FromM4ToXMMATRIX Win32FromM4ToXMMATRIX_

#define WIN32_PROCESS_CONTROLLER_INPUTS(name) void name(game_input* newInput, game_input* oldInput)
typedef WIN32_PROCESS_CONTROLLER_INPUTS(win32_process_controller_inputs);
WIN32_PROCESS_CONTROLLER_INPUTS(Win32ProcessControllerInputsStub)
{
    
}
global_variable win32_process_controller_inputs* Win32ProcessControllerInputs_ = Win32ProcessControllerInputsStub;
#define Win32ProcessControllerInputs Win32ProcessControllerInputs_


#define WIN32_LOAD_XINPUT(name) void name(void)
typedef WIN32_LOAD_XINPUT(win32_load_xinput);
WIN32_LOAD_XINPUT(Win32LoadXInputStub)
{

}
global_variable win32_load_xinput* Win32LoadXInput_ = Win32LoadXInputStub;
#define Win32LoadXInput Win32LoadXInput_

struct win32_framework_dll_code
{
    win32_process_pending_messages* Win32ProcessPendingMessages;
    win32_load_game_code* Win32LoadGameCode;
    win32_game_code_setup* Win32GameCodeSetup;
    win32_check_and_load_game_code* Win32CheckAndLoadGameCode;
    win32_create_spawnable_buffers* Win32CreateSpawnableBuffers;
    win32_from_v4_to_xmvector* Win32FromV4ToXMVECTOR;
    win32_convert_game_camera_to_win32* Win32ConvertGameCameraToWin32;
    win32_from_m4_to_xmmatrix* Win32FromM4ToXMMATRIX;
    win32_process_controller_inputs* Win32ProcessControllerInputs;
    win32_load_xinput* Win32LoadXInput;
};

#define WIN32_FRAMEWORK_DLL_INCLUDE_H
#endif
