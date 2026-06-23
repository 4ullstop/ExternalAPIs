#if !defined GAME_FRAMEWORK_H

#undef ALLOW_WIN32
#define ALLOW_WIN32 0

#include "../../Math/forty_math_fast.h"
#include "../../Types/game_types.h"
#include "../../OBJLoader/code/obj_loader.h"
#include "../../MemoryPools/code/memory_pool_dll_include.h"
#include "../../OBJLoader/code/obj_parser_dll_include.h"

struct alignas(16) game_camera
{
    m4 world;
    m4 view;
    m4 projection;

    v4 startEye;
    v4 startAt;
    v4 startUp;

    v4 aspect;    

    v4 front;
    v4 right;
    v4 up;
    v4 position;

    v4 worldUp;

    v4 eye;
    v4 targetPos;


    v4 targetQRot;

    v4 positionTo;

    v4 qRotationTo;
    v4 currQRot;
    
    v4 viewCenter;
    
    r32 pitch, yaw;
    r32 xChange, yChange;
    r32 fovY, movementSpeed;
    r32 targetZoom, currZoom;
    r32 zoomTo;
};

//A struct that needs to be filled out by the user
struct framework_arenas
{
    memory_arena* setupArena;
    memory_arena* perFrameArena;
    memory_arena* spawnedObjectArena;
};


#define GAME_FRAMEWORK_H
#endif
