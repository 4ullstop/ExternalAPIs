#if !defined(GAME_FRAMEWORK_DLL_INCLUDE)

#include "game_framework.h"

#define GAME_CREATE_VIEW_AND_PERSPECTIVE(name) void name(game_camera* camera)
typedef GAME_CREATE_VIEW_AND_PERSPECTIVE(game_create_view_and_perspective);
GAME_CREATE_VIEW_AND_PERSPECTIVE(GameCreateViewAndPerspectiveStub)
{

}
global_variable game_create_view_and_perspective* GameCreateViewAndPerspective_ = GameCreateViewAndPerspectiveStub;
#define GameCreateViewAndPerspective GameCreateViewAndPerspective_

#define GAME_UPDATE_CAMERA(name) void name(game_camera* camera)
typedef GAME_UPDATE_CAMERA(game_update_camera);
GAME_UPDATE_CAMERA(GameUpdateCameraStub)
{

}
global_variable game_update_camera* GameUpdateCamera_ = GameUpdateCameraStub;
#define GameUpdateCamera GameUpdateCamera_

struct game_framework_dll_code
{
    game_create_view_and_perspective* GameCreateViewAndPerspective;
    game_update_camera* GameUpdateCamera;
};
    
#define GAME_FRAMEWORK_DLL_INCLUDE
#endif
