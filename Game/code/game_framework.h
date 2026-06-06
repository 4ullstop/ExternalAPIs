#if !defined GAME_FRAMEWORK_H

#include "../../Math/forty_math_fast.h"

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
};

#define GAME_FRAMEWORK_H
#endif
