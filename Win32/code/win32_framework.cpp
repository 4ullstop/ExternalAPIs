#include <windows.h> //you won't need all of this at some point, slim it down for compile times
#include "win32_framework.h"


#include "../../MemoryPools/code/memory_pool_dll_include.h"
#include "../../FileReader/file_reader.h"
#include "../../FileReader/file_reader.cpp"
#include "D:/ExternalCustomAPIs/OBJLoader/code/obj_parser_dll_include.h"

//REMOVE THIS
#include <stdio.h>

#if 0
global_variable program_memory memory;
global_variable memory_arena tempArena;
global_variable memory_arena staticArena;
#endif

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


	++newState->halfTransitionCount;
    }
    newState->wasDown = isDown;



    if (isDown && newState->endedDown)
    {
	newState->started = true;
    }


    if (oldState->released)
    {
	oldState->released = false;
	newState->released = false;
    }    

    
    if (oldState->endedDown)
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
	case WM_CLOSE:
	case WM_DESTROY:
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
	{
	    UINT dwSize = 0;
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
		else if (VKCode == '1')
		{
		    Win32ProcessKeyboardMessage(&keyboardController->one,
						&oldKeyboardController->one, isDown);
		    
		}
		else if (VKCode == '2')
		{
		    Win32ProcessKeyboardMessage(&keyboardController->two,
						&oldKeyboardController->two, isDown);
		}
		else if (VKCode == '3')
		{
		    Win32ProcessKeyboardMessage(&keyboardController->three,
						&oldKeyboardController->three, isDown);
		}
		else if (VKCode == '4')
		{
		    Win32ProcessKeyboardMessage(&keyboardController->four,
						&oldKeyboardController->four, isDown);
		}
		else if (VKCode == '5')
		{
		    Win32ProcessKeyboardMessage(&keyboardController->five,
						&oldKeyboardController->five, isDown);
		}
		else if (VKCode == '6')
		{
		    Win32ProcessKeyboardMessage(&keyboardController->six,
						&oldKeyboardController->six, isDown);
		}
		else if (VKCode == '7')
		{
		    Win32ProcessKeyboardMessage(&keyboardController->seven,
						&oldKeyboardController->seven, isDown);		    
		}
		else if (VKCode == VK_ESCAPE)
		{
		    //running = false;
		    //need to figure this out when time comes
		    programState->running = false;
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
    obj_conversion result = {};

    HRESULT hr = {};

    if (currObj->hasMaterials)
    {
	result.materialIndices = (u32*)memoryPoolCode->PushArraySized(arena, (sizeof(u32) * currObj->faceCount));
    }

    v3* tempVerts = (v3*)memoryPoolCode->PushArraySized(arena, sizeof(v3) * currObj->vertexCount);
    
    for (i32 i = 0, j = 0; j < currObj->vertexCount; i +=3, j++)
    {
	tempVerts[j].x = currObj->vertices[i];
	tempVerts[j].y = currObj->vertices[i + 2];
	tempVerts[j].z = currObj->vertices[i + 1];	
    }

    v2* tempTexCoord = (v2*)memoryPoolCode->PushArraySized(arena, sizeof(v2) * currObj->vertexTextureCoordLastIndex);
    
    for (i32 i = 0; i < currObj->vertexTextureCoordLastIndex; i++)
    {
	tempTexCoord[i] = currObj->vertexTextureCoordinates[i];
    }

    //v/vt/vn
    vertex_position_color* preEvalList =
	(vertex_position_color*)memoryPoolCode->PushArraySized(arena,
							       sizeof(vertex_position_color) *
							       currObj->faceLastIndex);

    v4I* keys = (v4I*)memoryPoolCode->PushArraySized(arena, sizeof(v4I) * currObj->faceLastIndex);

    i32* preEvalIndices = (i32*)memoryPoolCode->PushArraySized(arena, sizeof(i32) * currObj->faceLastIndex);


    i32 totalNumOfVerts = 0;
    i32 totalNumOfIndices = 0;
    for (i32 i = 0, f = 0, k = 0; i < currObj->faceLastIndex; i++)
    {
	i32 posIndex = currObj->vertexIndices[i] - 1;
	i32 textureCoordIndex = currObj->vertexTextureCoordIndices[i] - 1;
	i32 vertexNormalIndex = currObj->vertexNormalIndices[i] - 1;

	keys[i].e[0] = posIndex;
	keys[i].e[1] = textureCoordIndex;
	keys[i].e[2] = vertexNormalIndex;
	keys[i].e[3] = 0;

	bool32 keyFound = false;
	i32 repeatedIndex = 0;
	for (i32 j = 0; j < i; j++)
	{
	    if (keys[i] == keys[j])
	    {
		//vertex already exists, don't add to vert list, properly index
		keyFound = true;
		repeatedIndex = preEvalIndices[j];
		break;
	    }
	}
	
	if (!keyFound)
	{
	    preEvalList[f].pos.x = tempVerts[posIndex].x;
	    preEvalList[f].pos.y = tempVerts[posIndex].y;
	    preEvalList[f].pos.z = tempVerts[posIndex].z;

	    DirectX::XMFLOAT3 vertColor = {1.0f, 0.0f, 1.0f};
	    preEvalList[f].color = vertColor;
	    
	    preEvalList[f].texCoord.x = tempTexCoord[textureCoordIndex].x;
	    preEvalList[f].texCoord.y = tempTexCoord[textureCoordIndex].y;	    
	    
	    preEvalIndices[k] = f;
	    f++;
	    k++;
	}
	else
	{
	    preEvalIndices[k] = repeatedIndex;
	    k++;
	}

	totalNumOfVerts = f;
	totalNumOfIndices = k;
    }
    //Finally allocate based on totalNum variables and assign

    result.objVertsSize = sizeof(vertex_position_color) * totalNumOfVerts;
    result.objVerts = (vertex_position_color*)memoryPoolCode->PushArraySized(arena, result.objVertsSize);
    result.indexCount = totalNumOfIndices;
    result.indices = (u16*)memoryPoolCode->PushArraySized(arena, sizeof(u16) * totalNumOfIndices);


    
    for (i32 i = 0; i < totalNumOfVerts; i++)
    {
	result.objVerts[i] = preEvalList[i];
    }

    for (i32 i = 0; i < totalNumOfIndices; i++)
    {
	result.indices[i] = (u16)preEvalIndices[i];
    }


    result.hasMaterials = currObj->hasMaterials;
    if (currObj->hasMaterials)
    {
	for (i32 i = 0; i < currObj->faceCount; i++)
	{
	    result.materialIndices[i] = (u32)currObj->facesToMaterialIndices[i];
	}
	result.faceCount = currObj->faceCount;

	size_t materialSize = sizeof(material_properties) * currObj->numOfMaterials;
	
	result.materialProperties = (material_properties*)memoryPoolCode->PushArraySized(arena, materialSize);

	result.numOfMaterials = currObj->numOfMaterials;
	DirectX::XMVECTOR tempCol = {};
	for (i32 i = 0; i < currObj->numOfMaterials; i++)
	{
	    tempCol = DirectX::XMVectorSet(currObj->materials[i].diffuse.x,
					   currObj->materials[i].diffuse.y,
					   currObj->materials[i].diffuse.z,
					   1.0f);
	    DirectX::XMStoreFloat4(&result.materialProperties[i].matColor, tempCol);
	}
    }      


    return(result);
}

internal FILETIME
Win32GetLastWriteTime(char* filename)
{
    FILETIME lastWriteTime = {};
    WIN32_FILE_ATTRIBUTE_DATA data;
    if (GetFileAttributesEx(filename, GetFileExInfoStandard, &data))
    {
	lastWriteTime = data.ftLastWriteTime;
    }
    return(lastWriteTime);
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

win32_game_code Win32LoadGameCode(game_code_path_cluster* paths, char* updateFuncName, char* initFuncName, memory_pool_dll_code* memoryPoolCode, memory_arena* arena)
{
    win32_game_code result = {};
    WIN32_FILE_ATTRIBUTE_DATA ignored;
    DWORD lastError = 0;

    if (!paths->funcNameInit)
    {
	paths->updateFuncName = (char*)memoryPoolCode->PushStruct(arena, sizeof(StringLength(updateFuncName)));
	paths->updateFuncName = updateFuncName;

	paths->initFuncName = (char*)memoryPoolCode->PushStruct(arena, sizeof(StringLength(initFuncName)));
	paths->initFuncName = initFuncName;
	paths->funcNameInit = true;
    }
    
    if ((!GetFileAttributesEx(paths->gameCodeLockFullPath, GetFileExInfoStandard, &ignored)) &&
	(paths->updateFuncName && paths->initFuncName))
    {
	result.dllLastWriteTime = Win32GetLastWriteTime(paths->sourceGameCodeDLLFullPath);
	CopyFile(paths->sourceGameCodeDLLFullPath, paths->tempGameCodeDLLFullPath, FALSE);
	result.gameCodeDLL = LoadLibrary(paths->tempGameCodeDLLFullPath);
	if (result.gameCodeDLL)
	{
	    result.GameUpdate = (void*)GetProcAddress(result.gameCodeDLL, paths->updateFuncName);
	    result.GameInitialize = (void*)GetProcAddress(result.gameCodeDLL, paths->initFuncName);
	    result.isValid = (result.GameUpdate && result.GameInitialize);
	}
    }

    if (!result.isValid)
    {
	result.GameUpdate = 0;
	result.GameInitialize = 0;
    }
    return(result);
}

internal void Win32GetEXEFilename(program_state* state)
{
    DWORD sizeOfFilename = GetModuleFileName(0, state->exeFilename, sizeof(state->exeFilename));
    state->onePastExeFilenameSlash = state->exeFilename;
    for (char* scan = state->exeFilename; *scan; ++scan)
    {
	if (*scan == '\\')
	{
	    state->onePastExeFilenameSlash = scan + 1;
	}
    }
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
    *dest = 0;
}

internal void Win32BuildEXEPathFilename(program_state* state, char* filename, i32 destCount, char* dest)
{
    CatStrings(state->onePastExeFilenameSlash - state->exeFilename, state->exeFilename,
	       StringLength(filename), filename,
	       destCount, dest);
}

game_code_path_cluster Win32GameCodeSetup(char* layerDll, char* tempDll, char* lockDll, program_state* state)
{
    Win32GetEXEFilename(state);

    game_code_path_cluster result = {};
    
    Win32BuildEXEPathFilename(state, layerDll, sizeof(result.sourceGameCodeDLLFullPath),
			      result.sourceGameCodeDLLFullPath);
    
    Win32BuildEXEPathFilename(state, tempDll, sizeof(result.tempGameCodeDLLFullPath),
			      result.tempGameCodeDLLFullPath);

    Win32BuildEXEPathFilename(state, lockDll, sizeof(result.gameCodeLockFullPath),
			      result.gameCodeLockFullPath);

    return(result);
}

internal void
Win32UnloadGameCode(win32_game_code* gameCode)
{
    if (gameCode->gameCodeDLL)
    {
	FreeLibrary(gameCode->gameCodeDLL);
	gameCode->gameCodeDLL = 0;
    }
    gameCode->isValid = false;
    gameCode->GameUpdate = 0;
    gameCode->GameInitialize = 0;
}

i32 CheckAndLoadGameCode(game_code_path_cluster* paths, win32_game_code* gameCode, memory_pool_dll_code* memoryPoolCode, memory_arena* arena)
{
    FILETIME newDLLWriteTime = Win32GetLastWriteTime(paths->sourceGameCodeDLLFullPath);
    if (CompareFileTime(&newDLLWriteTime, &gameCode->dllLastWriteTime) != 0)
    {
	Win32UnloadGameCode(gameCode);

	*gameCode = Win32LoadGameCode(paths, nullptr, nullptr, memoryPoolCode, arena);
    }
    //Function returns the load counter
    return(0);
}

internal draw_buffers
CreateBuffersFromOBJ(obj* objToInit, memory_arena* tempArena, ID3D11Device* d3dDevice, memory_pool_dll_code* memoryPoolCode)
{
    HRESULT hr = {};
    draw_buffers result = {};

    obj_conversion convertedObj = convertedObj = ConvertGameOBJToDXOBJ(objToInit, tempArena, memoryPoolCode);

    D3D11_BUFFER_DESC vertexDesc;
    vertexDesc.Usage = D3D11_USAGE_DEFAULT;
    vertexDesc.ByteWidth = convertedObj.objVertsSize;
    vertexDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    vertexDesc.CPUAccessFlags = 0;
    vertexDesc.MiscFlags = 0;

    D3D11_SUBRESOURCE_DATA vertexData;
    ZeroMemory(&vertexData, sizeof(D3D11_SUBRESOURCE_DATA));
    vertexData.pSysMem = convertedObj.objVerts;
    vertexData.SysMemPitch = 0;
    vertexData.SysMemSlicePitch = 0;
    
    hr = d3dDevice->CreateBuffer(
	&vertexDesc,
	&vertexData,
	&result.vertexBuffer);

    CD3D11_BUFFER_DESC indexDesc(
	sizeof(u16) * convertedObj.indexCount,
	D3D11_BIND_INDEX_BUFFER);

    D3D11_SUBRESOURCE_DATA indexData;
    ZeroMemory(&indexData, sizeof(D3D11_SUBRESOURCE_DATA));
    indexData.pSysMem = convertedObj.indices;
    indexData.SysMemPitch = 0;
    indexData.SysMemSlicePitch = 0;

    hr = d3dDevice->CreateBuffer(
	&indexDesc,
	&indexData,
	&result.indexBuffer);

    result.indexCount = convertedObj.indexCount;

 

    if (convertedObj.hasMaterials)
    {
	

	D3D11_BUFFER_DESC matDesc = {};
	matDesc.Usage = D3D11_USAGE_IMMUTABLE;
	matDesc.ByteWidth = sizeof(u32) * convertedObj.faceCount;
	matDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	matDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
	matDesc.StructureByteStride = sizeof(u32);

	//It might be worth allocating this automatically, and then filling with
	//blank colors such that we can tell that a material is missing
	D3D11_SUBRESOURCE_DATA initData = {};
	initData.pSysMem = convertedObj.materialIndices;

	
	
	DWORD lastError = {};

	
	hr = d3dDevice->CreateBuffer(&matDesc, &initData, &result.materialBuffer);

	hr = d3dDevice->CreateShaderResourceView(result.materialBuffer, nullptr, &result.materialResourceView);

	D3D11_BUFFER_DESC matAtDesc = {};
	matAtDesc.Usage = D3D11_USAGE_DEFAULT;
	matAtDesc.ByteWidth = sizeof(material_properties) * convertedObj.numOfMaterials;
	matAtDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	matAtDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
	matAtDesc.StructureByteStride = sizeof(material_properties);

	D3D11_SUBRESOURCE_DATA matInitData = {};
	matInitData.pSysMem = convertedObj.materialProperties;

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Format = DXGI_FORMAT_UNKNOWN;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
	srvDesc.Buffer.FirstElement = 0;
	srvDesc.Buffer.NumElements = convertedObj.numOfMaterials;

	hr = d3dDevice->CreateBuffer(&matAtDesc, &matInitData, &result.materialPropertiesBuffer);
	hr = d3dDevice->CreateShaderResourceView(result.materialPropertiesBuffer, &srvDesc, &result.materialPropertiesView);
	
    }
    else
    {
	result.materialBuffer = nullptr;
	result.materialResourceView = nullptr;
	    
    }
    result.faceCount = convertedObj.faceCount;
    result.hasMaterials = convertedObj.hasMaterials;


    
    
    return(result);
}

texture_buffers*
Win32CreateTextureBuffers(game_loaded_textures* gameLoadedTextures, memory_arena* arena, memory_pool_dll_code* memoryPoolCode, ID3D11Device* d3dDevice)
{
    texture_buffers* result = 0;
    result =
	(texture_buffers*)memoryPoolCode->PushArraySized(arena,
							 sizeof(texture_buffers) *
							 gameLoadedTextures->numOfTextures);

    HRESULT hr = {};
    for (i32 i = 0; i < gameLoadedTextures->numOfTextures; i++)
    {
	i32 width = gameLoadedTextures->textures[i].bmpTextureData.width;
	i32 height = gameLoadedTextures->textures[i].bmpTextureData.height;
	
	D3D11_TEXTURE2D_DESC textureDesc = {};
	textureDesc.Width = width;
	textureDesc.Height = height;
	textureDesc.MipLevels = 1;
	textureDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.SampleDesc.Quality = 0;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = 0;
	textureDesc.ArraySize = 1;

	D3D11_SUBRESOURCE_DATA textureData = {};
	textureData.pSysMem = gameLoadedTextures->textures[i].bmpTextureData.pixels;
	textureData.SysMemPitch = width * sizeof(u32);

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Format = textureDesc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = 1;
	
	hr = d3dDevice->CreateTexture2D(&textureDesc, &textureData, &result[i].textureData);
	hr = d3dDevice->CreateShaderResourceView(result[i].textureData, &srvDesc, &result[i].textureResourceView);
    }
    return(result);
}

void Win32CreateSpawnableBuffers(game_loaded_objs* gameObjs, game_loaded_textures* gameTextures, win32_spawnable_objs* win32Objs, memory_arena* objArena, memory_arena* tempArena, memory_pool_dll_code* memoryPoolCode, ID3D11Device* d3dDevice)
{
    win32Objs->objDrawnBuffers =
	(draw_buffers*)memoryPoolCode->PushArraySized(objArena, (sizeof(draw_buffers)
								* gameObjs->spawnedObjMemory->numOfItems));

    obj* objsToSpawn = gameObjs->staticLoadedObjs;
    draw_buffers* win32DrawnBuffers = win32Objs->objDrawnBuffers;

    for (i32 i = 0; i < gameObjs->objFileNum; i++)
    {
	Assert(win32DrawnBuffers);
	Assert(objsToSpawn);

	//If you start experiencing issues where meshes start to dissapear bc of overwritten data or something,
	//look here and change tempArena to objArena
	*(win32DrawnBuffers) = CreateBuffersFromOBJ(objsToSpawn, tempArena, d3dDevice, memoryPoolCode);
	win32DrawnBuffers++;
	objsToSpawn++;
    }
    win32Objs->textureBuffers = Win32CreateTextureBuffers(gameTextures, objArena, memoryPoolCode, d3dDevice);
}

DirectX::XMVECTOR FromV4ToXMVECTOR(v4 v)
{
    DirectX::XMVECTOR result = DirectX::XMVectorSet(v.x, v.y, v.z, v.w);
    return(result);
}

DirectX::XMMATRIX FromM4ToXMMATRIX(m4 m)
{
    DirectX::XMMATRIX result = {};

    //try to see if you can use intrinsics here!!
    result.r[0] = FromV4ToXMVECTOR(m.r[0]);
    result.r[1] = FromV4ToXMVECTOR(m.r[1]);
    result.r[2] = FromV4ToXMVECTOR(m.r[2]);
    result.r[3] = FromV4ToXMVECTOR(m.r[3]);

    return(result);
} 

void ConvertGameCameraDataToWin32(dx_camera* dxCam, game_camera_data* gCamData)
{
    DirectX::XMStoreFloat4x4(&dxCam->constantBufferData.world,
			     FromM4ToXMMATRIX(gCamData->world));

    DirectX::XMStoreFloat4x4(&dxCam->constantBufferData.view,
			     FromM4ToXMMATRIX(gCamData->view));

    DirectX::XMStoreFloat4x4(&dxCam->constantBufferData.projection,
			     FromM4ToXMMATRIX(gCamData->projection));
}

