#if !defined GAME_FRAMEWORK_H

#include "../../Math/forty_math_fast.h"
#include "../../Types/game_types.h"

struct game_camera
{
    m4 world;
    m4 view;
    m4 projection;

    v4 startEye;
    v4 startAt;
    v4 startUp;

    r32 fovY;

    v2 aspect;

    v4 front;
    v4 right;
    v4 up;
    v4 position;

    v4 worldUp;

    r32 pitch, yaw;
    r32 xChange, yChange;
};

#define GAME_FRAMEWORK_H
#endif
