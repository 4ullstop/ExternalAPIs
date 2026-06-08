#if !defined(GAME_FRAMEWORK_DLL_INCLUDE)

#include "game_framework.h"

#define GAME_CREATE_VIEW_AND_PERSPECTIVE(name) void name(game_camera* camera)
typedef GAME_CREATE_VIEW_AND_PERSPECTIVE(game_create_view_and_perspective);
GAME_CREATE_VIEW_AND_PERSPECTIVE(GameCreateViewAndPerspectiveStub)
{

}
global_variable game_create_view_and_perspective* GameCreateViewAndPerspective_ = GameCreateViewAndPerspectiveStub;
#define GameCreateViewAndPerspective GameCreateViewAndPerspective_


struct game_framework_dll_code
{
    game_create_view_and_perspective* GameCreateViewAndPerspective;
};
    
#define GAME_FRAMEWORK_DLL_INCLUDE
#endif
