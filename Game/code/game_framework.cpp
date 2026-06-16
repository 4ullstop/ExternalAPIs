#include "game_framework.h"

void CreateViewAndPerspective(game_camera* camera)
{
    //NOTE: Aspect ratio is not filled out and will need to be in the Win32 layer
    m4 startingViewMatrix = Transpose(LookAtRH(camera->startEye,
					       camera->startAt,
					       camera->startUp));


    camera->worldUp = camera->startUp;

    camera->view = startingViewMatrix;
    camera->fovY = 2.0f * (r32)(atan(tan(ToRad(70) * 0.5f)) / camera->aspect.y);


    camera->world = {};
    camera->projection = Transpose(PerspectiveFovRH(
				       camera->fovY,
				       camera->aspect.x,
				       0.01f,
				       1000.0f)
	);
}


void GameUpdateCamera(game_camera* camera)
{
    camera->yaw += camera->xChange;
    camera->pitch += camera->yChange;

    //This isn't perfect but can be fixed later
    if (camera->pitch > 89.0f)
    {
	camera->pitch = 89.0f;
    }
    if (camera->pitch < -89.0f)
    {
	camera->pitch = -89.0f;
    }
    
    camera->front =
    {
	(r32)(cos(camera->pitch) * sin(camera->yaw)),
	(r32)(sin(camera->pitch)),
	(r32)(cos(camera->yaw) * cos(camera->pitch)),
    };

    camera->front = NormalizeV3(camera->front);
    //now we want the w coord involved?
    camera->right = NormalizeV4(CrossV3(camera->front, camera->worldUp));
    camera->up = NormalizeV4(CrossV3(camera->right, camera->front));

    camera->view = Transpose(LookAtRH(camera->position, (camera->front + camera->position), camera->up));
    
}

//Eventually the goal would be to look into a directory, find all of the .obj files and load them in
//This will be two separate actions, in the game framework all we care about is loading in
//the files that are given to this function, the platform framework will have functions to
//look into the directory
game_loaded_objs LoadGameOBJFiles(parse_obj_data_code* parseObjCode, framework_arenas* arenas, program_memory* pgMem, memory_pool_dll_code* memoryPoolCode, char** fileLocation, i32 num)
{
    game_loaded_objs result = {};

    result.objFileNum = num;

    result.staticLoadedObjs = (obj*)memoryPoolCode->PushArraySized(arenas->setupArena, (sizeof(obj) * result.objFileNum));
    result.types = (spawnable_obj_type*)memoryPoolCode->PushArraySized(arenas->setupArena,
										   (sizeof(spawnable_obj_type) *
										    result.objFileNum));


    for (i32 i = 0; i < result.objFileNum; i++)
    {
	result.staticLoadedObjs[i] = *parseObjCode->ParseOBJData(fileLocation[i], arenas->perFrameArena, arenas->setupArena, pgMem, memoryPoolCode);
	result.types[i] = (spawnable_obj_type)i;
    }

    result.spawnedObjMemory = (listed_memory*)memoryPoolCode->PushStruct(arenas->spawnedObjectArena, sizeof(listed_memory));

    result.spawnedObjMemory->numOfItems = 0;
    memoryPoolCode->InitListedMemory(result.spawnedObjMemory, arenas->spawnedObjectArena, sizeof(spawned_obj_info));

    return(result);
}

void SpawnNewOBJ(spawnable_obj_type type, v3 location, game_loaded_objs* loadedObjs, memory_pool_dll_code* memoryPoolCode)
{
    spawned_obj_info newInfo;
    newInfo.location = location;
    newInfo.type = type;

    memoryPoolCode->AddListedItem(loadedObjs->spawnedObjMemory,
				  (void*)&newInfo,
				  sizeof(newInfo),
				  &loadedObjs->spawnedObjNodes);
}
