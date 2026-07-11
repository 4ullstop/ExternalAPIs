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

    r32 totalYaw = camera->yaw;
    
    camera->front =
	{
	    (r32)(cos(camera->pitch) * sin(totalYaw)),
	    (r32)(sin(camera->pitch)),
	    (r32)(cos(totalYaw) * cos(camera->pitch)),
	};

    camera->front = NormalizeV3(camera->front);

    //now we want the w coord involved?
    camera->right = NormalizeV4(CrossV3(camera->front, camera->worldUp));
    camera->up = NormalizeV4(CrossV3(camera->right, camera->front));


    v4 target = {};

    if (camera->inheritRotation)
    {

	v4 frontTarget = Vector3Rotate(camera->front, camera->inheritedRotation);
	frontTarget = NormalizeV3(frontTarget);
	target = frontTarget + camera->position;
	camera->up = Vector3Rotate(camera->up, camera->inheritedRotation);
    }
    else
    {
	target = camera->front + camera->position;	
    }
 


    camera->view = Transpose(LookAtRH(camera->position, target, camera->up));
    camera->viewInv = Inverse(camera->view, nullptr);    
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

spawned_obj_info* SpawnNewOBJ(spawnable_obj_type type, transform startTrans, game_loaded_objs* loadedObjs, memory_pool_dll_code* memoryPoolCode, bool32 inheritsTransform, m4 inheritedMat)
{
    spawned_obj_info newInfo;
    newInfo.modelTransform = startTrans;
    m4 localMat = CreateModelMatrix(startTrans.scale, startTrans.rotation, startTrans.location);
    newInfo.inheritsTransform = inheritsTransform;
    if (newInfo.inheritsTransform)
    {
	newInfo.modelMatrix = localMat * inheritedMat;
	newInfo.localMatrix = localMat;
    }
    else
    {
	newInfo.modelMatrix = localMat;
    }
    newInfo.type = type;

    listed_memory_node* node = memoryPoolCode->AddListedItem(loadedObjs->spawnedObjMemory,
							     (void*)&newInfo,
							     sizeof(newInfo),
							     &loadedObjs->spawnedObjNodes);
    spawned_obj_info* outInfo = (spawned_obj_info*)node->data;
    return(outInfo);
}

void UpdateInternalTransformations(game_camera* camera)
{
    m4 rot = MatrixRotationQuaternion(camera->currQRot);
    m4 trans = TranslationM4(camera->position);

    m4 zoom = TranslationM4(0.0f, 0.0f, camera->currZoom);
    m4 cameraWorld = zoom * rot * trans;

    v4 det = {};
    m4 view = Inverse(cameraWorld, &det);

    camera->view = Transpose(cameraWorld);
    
}

void InitArcBallCamera(game_camera* camera)
{
    camera->eye =  {0.0f, 0.0f, 0.0f, 0.0f};
    camera->viewCenter = {0.0f, 0.0f, 0.0f, 0.0f};
    camera->up = {0.0f, 1.0f, 0.0f, 0.0f};

    v4 dir =  camera->viewCenter - camera->eye;
    v4 zAxis = NormalizeV3(dir);
    v4 xAxis = NormalizeV3(CrossV3(zAxis,
				   NormalizeV3(camera->up)));
    v4 yAxis = NormalizeV3(CrossV3(xAxis, zAxis));
    xAxis = NormalizeV3(CrossV3(zAxis, yAxis));
    camera->targetPos = camera->viewCenter;
    camera->targetZoom = 15.f;

    zAxis = zAxis * -1.0f;

    v4 w = {0.0f, 0.0f, 0.0f, 1.0f};
    m4 rotMat = {};
    rotMat.r[0] = xAxis;
    rotMat.r[1] = yAxis;
    rotMat.r[2] = zAxis;
    rotMat.r[3] = w;

    rotMat = Transpose(rotMat);
    camera->targetQRot = QuaternionNormalize(QuaternionRotationMatrix(rotMat));

    camera->positionTo = camera->position = camera->targetPos;
    camera->zoomTo = camera->currZoom = camera->targetZoom;
    camera->qRotationTo = camera->currQRot = camera->targetQRot;

    UpdateInternalTransformations(camera);
}
