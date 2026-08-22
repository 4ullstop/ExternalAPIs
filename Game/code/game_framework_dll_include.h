#if !defined(GAME_FRAMEWORK_DLL_INCLUDE)

#include "game_framework.h"


#if defined(_MSC_VER)
#define GAME_CALL __vectorcall
#elif defined(__clang__) || defined(__GNUC__)
#define GAME_CALL __attribute__((vectorcall))
#else
#define GAME_CALL
#endif


#define GAME_CREATE_VIEW_AND_PERSPECTIVE(name) void GAME_CALL name(game_camera* camera)
typedef GAME_CREATE_VIEW_AND_PERSPECTIVE(game_create_view_and_perspective);
GAME_CREATE_VIEW_AND_PERSPECTIVE(GameCreateViewAndPerspectiveStub)
{

}
global_variable game_create_view_and_perspective* GameCreateViewAndPerspective_ = GameCreateViewAndPerspectiveStub;
#define GameCreateViewAndPerspective GameCreateViewAndPerspective_

#define GAME_UPDATE_CAMERA(name) void GAME_CALL name(game_camera* camera)
typedef GAME_UPDATE_CAMERA(game_update_camera);
GAME_UPDATE_CAMERA(GameUpdateCameraStub)
{

}
global_variable game_update_camera* GameUpdateCamera_ = GameUpdateCameraStub;
#define GameUpdateCamera GameUpdateCamera_

#define GAME_LOAD_OBJ_FILES(name) game_loaded_objs name(parse_obj_data_code* parseObjCode, framework_arenas* arenas, program_memory* pgMem, memory_pool_dll_code* memoryPoolCode, char** fileLocation, i32 num)
typedef GAME_LOAD_OBJ_FILES(game_load_obj_files);
GAME_LOAD_OBJ_FILES(GameLoadObjFilesStub)
{
    game_loaded_objs result = {};
    return(result);
}
global_variable game_load_obj_files* GameLoadObjFiles_ = GameLoadObjFilesStub;
#define GameLoadObjFiles GameLoadObjFiles_

#define GAME_SPAWN_NEW_OBJ(name) spawned_obj_info* name(spawnable_obj_type type, transform startTrans, game_loaded_objs* loadedObjs, memory_pool_dll_code* memoryPoolCode, bool32 inheritsTransform, m4 inheritedMat)
typedef GAME_SPAWN_NEW_OBJ(game_spawn_new_obj);
GAME_SPAWN_NEW_OBJ(GameSpawnNewObjStub)
{
    spawned_obj_info* temp = 0;
    return(temp);
}
global_variable game_spawn_new_obj* GameSpawnNewObj_ = GameSpawnNewObjStub;
#define GameSpawnNewObj GameSpawnNewObj_

#define GAME_LOAD_TEXTURES(name) game_loaded_textures name(char** fileLocation, memory_arena* arena, i32 num, debug_platform_read_entire_file* ReadEntireFile, memory_pool_dll_code* memoryPoolCode)
typedef GAME_LOAD_TEXTURES(game_load_textures);
GAME_LOAD_TEXTURES(GameLoadTexturesStub)
{
    game_loaded_textures temp = {};
    return(temp);
}
global_variable game_load_textures* GameLoadTextures_ = GameLoadTexturesStub;
#define GameLoadTextures GameLoadTextures_

struct game_framework_dll_code
{
    game_create_view_and_perspective* GameCreateViewAndPerspective;
    game_update_camera* GameUpdateCamera;
    game_load_obj_files* GameLoadOBJFiles;
    game_spawn_new_obj* GameSpawnNewOBJ;
    game_load_textures* GameLoadTextures;
};
    
#define GAME_FRAMEWORK_DLL_INCLUDE
#endif
