//define dll functions to be used here
/*
  _____________________________________________
  You must have the MemoryPoolDLL code initialized within your code base if not this code will not work
  _____________________________________________
  
 */
#if !defined WIN32_FRAMEWORK_DLL_INCLUDE_H
#include "win32_framework.h"

#define WIN32_FRAMEWORK_PROCESS_KEYBOARD_MESSAGE(name) void name(game_button_state* newState, game_button_state* oldState, bool32 isDown)
typedef WIN32_FRAMEWORK_PROCESS_KEYBOARD_MESSAGE(win32_process_keyboard_message);
WIN32_FRAMEWORK_PROCESS_KEYBOARD_MESSAGE(Win32ProcessKeyboardMessageStub)
{
    return(0);
}
global_variable win32_process_keyboard_message* Win32ProcessKeyboardMessage_ = Win32ProcessKeyboardMessageStub;
#define Win32ProcessKeyboardMessage Win32ProcessKeyboardMessage_

#define WIN32_FRAMEWORK_PROCESS_MOUSE_MESSAGE(name) void name(game_button_state* nMouse, game_button_state* oMouse, bool32 down)
typedef WIN32_FRAMEWORK_PROCESS_MOUSE_MESSAGE(win32_process_mouse_message);
WIN32_FRAMEWORK_PROCESS_MOUSE_MESSAGE(Win32ProcessMouseMessageStub)
{
    return(0);
}
global_variable win32_process_mouse_message* Win32ProcessMouseMessage_ = Win32ProcessMouseMessageStub;
#define Win32ProcessMouseMessage Win32ProcessMouseMessage_

#define WIN32_FRAMEWORK_PROCESS_PENDING_MESSAGES(name) void name(game_controller_input* keyboardController, game_controller_input* oldKeyboardController, game_input* newInput, game_input* oldInput, mouse_movements* mouse, memory_pool_dll_code* memoryPoolCode, memory_arena* perFrameArena)
typedef WIN32_FRAMEWORK_PROCESS_PENDING_MESSAGES(win32_process_pending_messages);
WIN32_FRAMEWORK_PROCESS_PENDING_MESSAGES(Win32ProcessPendingMessagesStub)
{
    return(0);
}
global_variable win32_process_pending_messages* Win32ProcessPendingMessages_ = Win32ProcessPendingMessagesStub;
#define Win32ProcessPendingMessages Win32ProcessPendingMessages_

struct win32_framework_dll_code
{
    win32_process_keyboard_message* Win32ProcessKeyboardMessage;
    win32_process_mouse_message* Win32ProcessMouseMessage;
    win32_process_pending_messages* Win32ProcessPendingMessages;
};

#define WIN32_FRAMEWORK_DLL_INCLUDE_H
#endif
