#include <windows.h> //you won't need all of this at some point, slim it down for compile times
#include "win32_framework.h"


#include "../../MemoryPools/code/memory_pool_dll_include.h"
#include "../../FileReader/file_reader.h"
#include "../../FileReader/file_reader.cpp"
#include "D:/ExternalCustomAPIs/OBJLoader/code/obj_parser_dll_include.h"

global_variable program_memory memory;
global_variable memory_arena tempArena;
global_variable memory_arena staticArena;

global_variable thread_context blankThread;

//The assumption is that the user will use my memory library, this is a pretty bad assumption
#if 0
void Win32InitFramework()
{
    HMODULE memoryPoolLibrary = LoadLibrary("../../MemoryPools/code/memory_pools.dll");

    if(memoryPoolLibrary)
    {
	memoryPoolCode.PushStruct = (memory_pool_push_struct*)GetProcAddress(memoryPoolLibrary, "PushStruct");
	memoryPoolCode.PushArray = (memory_pool_push_array*)GetProcAddress(memoryPoolLibrary, "PushArray");
	memoryPoolCode.PoolAlloc = (memory_pool_alloc*)GetProcAddress(memoryPoolLibrary, "PoolAlloc");
	memoryPoolCode.InitArena = (memory_pool_initialize_arena*)GetProcAddress(memoryPoolLibrary, "InitializeArena");
	memoryPoolCode.ClearArena = (memory_pool_clear_arena*)GetProcAddress(memoryPoolLibrary, "ClearArena");
	memoryPoolCode.PushArraySized = (memory_pool_push_array_sized*)GetProcAddress(memoryPoolLibrary, "PushArraySized");
    }

    //NOTE: This is probably an unecessary size, when you have the time measure what you need and only use that

    memory.transientStorageSize = Megabytes(1);
    memory.permanentStorageSize = Megabytes(4);

    memoryPoolCode.PoolAlloc(0, MEM_RESERVE|MEM_COMMIT, PAGE_READWRITE, &memory);

    size_t staticArenaAllocSize = Megabytes(4);
    size_t tempArenaAllocSize = Megabytes(1);

    memory.permanentArenaBase = (u8*)memory.permanentStorage;
    memory.transientArenaBase = (u8*)memory.transientStorage;
    
    memoryPoolCode.InitArena(&staticArena, staticArenaAllocSize, &memory, e_arena_type::permanent);
    memoryPoolCode.InitArena(&tempArena, tempArenaAllocSize, &memory, e_arena_type::transient);
}
#endif
/*
  In order to use the game framework accordingly, we need to convert some properties for DX11 to
  work

  This means this function will need to be called for the user to have the camera functions work properly

  This is intended to be done in the user's Win32 layer
  
*/
void Win32UpdateCameraFP(dx_camera* camera)
{
    //Update constantBufferData according to the constantBufferData in our game_camera, then submit to pipeline  
}

void Win32SetMouseStates(game_input* newInput, game_input* oldInput)
{
    for (u8 i = 0; i < 5; i++)
    {
	game_button_state* nMouse = &newInput->mouseButtons[i];
	game_button_state* oMouse = &oldInput->mouseButtons[i];

	if(nMouse->released || oMouse->released)
	{
	    oMouse->endedDown = false;
	    nMouse->endedDown = false;
	    oMouse->released = false;
	    nMouse->released = false;
	}

	if (nMouse->started || oMouse->started)
	{
	    oMouse->started = false;
	    nMouse->started = false;
	}
    }
}

void Win32ProcessKeyboardMessage(game_button_state* newState, game_button_state* oldState, bool32 isDown)
{
    if (newState->endedDown != isDown)
    {
	newState->endedDown = isDown;

	newState->started = isDown;
	++newState->halfTransitionCount;
    }
    newState->wasDown = isDown;

    if (oldState->released)
    {
	oldState->released = false;
    }

    if (oldState->endedDown)
    {
	newState->started = false;
    }

    if (!newState->endedDown && oldState->endedDown)
    {
	newState->released = true;
    }
}

void Win32ProcessMouseMessage(game_button_state* nMouse, game_button_state* oMouse, bool32 down)
{
    if (down)
    {
	nMouse->started = true;
	oMouse->started = true;
	nMouse->endedDown = true;
	oMouse->endedDown = true;
    }
    else
    {
	nMouse->released = true;
	oMouse->released = true;
    }

}

void Win32ProcessPendingMessages(game_controller_input* keyboardController, game_controller_input* oldKeyboardController, game_input* newInput, game_input* oldInput, mouse_movements* mouse, memory_pool_dll_code* memoryPoolCode, memory_arena* perFrameArena, program_state* programState)
{
    Win32SetMouseStates(newInput, oldInput);

    MSG msg;
    while (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
    {
	switch(msg.message)
	{
	case WM_ACTIVATE:
	{

	} break;
	case WM_QUIT:
	{
	    programState->running = false;
	} break;
	case WM_SIZE:
	{

	} break;
	case WM_MOUSEACTIVATE:
	{

	} break;
	case WM_INPUT:
	case WM_SYSKEYDOWN:
	case WM_SYSKEYUP:
	case WM_KEYUP:
	case WM_KEYDOWN:
	{
	    UINT dwSize = 0;

	    u32 VKCode = (u32)msg.wParam;
	    bool32 wasDown = ((msg.lParam & (1 << 30)) != 0);
	    bool32 isDown = ((msg.lParam & (1 << 31)) == 0);

	    if (wasDown != isDown)
	    {
		if (VKCode == 'W')
		{
		    Win32ProcessKeyboardMessage(&keyboardController->moveForward,
						&oldKeyboardController->moveForward, isDown);
		}
		else if (VKCode == 'A')
		{
		    Win32ProcessKeyboardMessage(&keyboardController->moveLeft,
						&oldKeyboardController->moveLeft, isDown);
		}
		else if (VKCode == 'S')
		{
		    Win32ProcessKeyboardMessage(&keyboardController->moveBackward,
						&oldKeyboardController->moveBackward, isDown);
		}
		else if (VKCode == 'D')
		{
		    Win32ProcessKeyboardMessage(&keyboardController->moveRight,
						&oldKeyboardController->moveRight, isDown);
		}
		else if (VKCode == 'Q')
		{
		    Win32ProcessKeyboardMessage(&keyboardController->moveDown,
						&oldKeyboardController->moveDown, isDown);		    
		}
		else if (VKCode == 'E')
		{
		    Win32ProcessKeyboardMessage(&keyboardController->moveUp,
						&oldKeyboardController->moveUp, isDown);
		}
		else if (VKCode == 'F')
		{
		    Win32ProcessKeyboardMessage(&keyboardController->testKey,
						&oldKeyboardController->testKey, isDown);
		}
		else if (VKCode == VK_ESCAPE)
		{
		    //running = false;
		    //need to figure this out when time comes
		}
	    }

	    //Mouse inputs (using raw data for unbounded mouse movements)
	    GetRawInputData((HRAWINPUT)msg.lParam, RID_INPUT, NULL, &dwSize, sizeof(RAWINPUTHEADER));
	    LPBYTE lpb = (LPBYTE)memoryPoolCode->PushStruct(perFrameArena, (sizeof(BYTE)) * dwSize);
	    GetRawInputData((HRAWINPUT)msg.lParam, RID_INPUT, lpb, &dwSize, sizeof(RAWINPUTHEADER));

	    RAWINPUT* raw = (RAWINPUT*)lpb;

	    if (raw->header.dwType == RIM_TYPEMOUSE)
	    {
		mouse->loc.x = (r32)raw->data.mouse.lLastX;
		mouse->loc.y = (r32)raw->data.mouse.lLastY;

		bool32 isMouseDown = false;

		if (raw->data.mouse.usButtonFlags == RI_MOUSE_MIDDLE_BUTTON_DOWN)
		{
		    Win32ProcessMouseMessage(&newInput->mouseButtons[e_mouse_buttons::middle_mouse],
					     &oldInput->mouseButtons[e_mouse_buttons::middle_mouse],
					     true);
		}
		else if (raw->data.mouse.usButtonFlags == RI_MOUSE_MIDDLE_BUTTON_UP)
		{
		    Win32ProcessMouseMessage(&newInput->mouseButtons[e_mouse_buttons::middle_mouse],
					     &oldInput->mouseButtons[e_mouse_buttons::middle_mouse],
					     false);
		}

		if (raw->data.mouse.usButtonFlags == RI_MOUSE_LEFT_BUTTON_DOWN)
		{
		    Win32ProcessMouseMessage(&newInput->mouseButtons[e_mouse_buttons::left_mouse],
					     &oldInput->mouseButtons[e_mouse_buttons::left_mouse],
					     true);
		}
		else if (raw->data.mouse.usButtonFlags == RI_MOUSE_LEFT_BUTTON_UP)
		{
		    Win32ProcessMouseMessage(&newInput->mouseButtons[e_mouse_buttons::left_mouse],
					     &oldInput->mouseButtons[e_mouse_buttons::left_mouse],
					     false);
		}

		if (raw->data.mouse.usButtonFlags == RI_MOUSE_RIGHT_BUTTON_DOWN)
		{
		    Win32ProcessMouseMessage(&newInput->mouseButtons[e_mouse_buttons::right_mouse],
					     &oldInput->mouseButtons[e_mouse_buttons::right_mouse],
					     true);
		}
		else if (raw->data.mouse.usButtonFlags == RI_MOUSE_RIGHT_BUTTON_UP)
		{
		    Win32ProcessMouseMessage(&newInput->mouseButtons[e_mouse_buttons::right_mouse],
					     &oldInput->mouseButtons[e_mouse_buttons::right_mouse],
					     false);
		}
	    }
	    
	} break;
	default:
	{
	    TranslateMessage(&msg);
	    DispatchMessage(&msg);
	} break;
	}

    }
	    
}

obj_conversion ConvertGameOBJToDXOBJ(obj* currObj, memory_arena* arena, memory_pool_dll_code* memoryPoolCode)
{
    obj_conversion result;

    HRESULT hr = {};

    result.objVertsSize = sizeof(vertex_position_color) * (currObj->vertexCount);
    result.objVerts =
	(vertex_position_color*)memoryPoolCode->PushArraySized(arena, result.objVertsSize);

    size_t indexSize = sizeof(u16) * currObj->faceLastIndex;
    result.indices = (u16*)memoryPoolCode->PushArraySized(arena, indexSize);

    //When you get to it you will have to figure out how to load in textures
    v3 testColors[] =
    {
	{0, 0, 0}, //0 Black
	{1, 0, 0}, //1 Red
	{0, 1, 0}, //2 Green
	{0, 0, 1}, //3 Blue
	{1, 0, 1}, //4 Magenta
	{0, 1, 1}, //5 Cyan
	{1, 1, 0}, //6 Yellow
	{1, 1, 1}, //7 White
    };

    for (i32 i = 0, j = 0; j < currObj->vertexCount; i += 3, j++)
    {
	result.objVerts[j].pos.x = currObj->vertices[i];
	result.objVerts[j].pos.y = currObj->vertices[i + 2];
	result.objVerts[j].pos.z = currObj->vertices[i + 1];

	DirectX::XMFLOAT3 vertColor = {testColors[j].x, testColors[j].y, testColors[j].z};
	result.objVerts[j].color = vertColor;
    }
    for (i32 i = 0; i < currObj->faceLastIndex; i++)
    {
	result.indices[i] = currObj->vertexIndices[i] - 1;
    }
    result.indexCount = currObj->faceLastIndex;
    return(result);
}
