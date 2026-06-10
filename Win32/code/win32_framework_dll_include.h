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
global_variable win32_init_framework* Win32InitFramework_ = Win32InitFramework;
#define Win32InitFramework Win32InitFrameworkStub_
#endif

struct win32_framework_dll_code
{
    win32_process_pending_messages* Win32ProcessPendingMessages;
};

#define WIN32_FRAMEWORK_DLL_INCLUDE_H
#endif
