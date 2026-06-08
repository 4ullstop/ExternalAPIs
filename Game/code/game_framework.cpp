#include "game_framework.h"

void CreateViewAndPerspective(game_camera* camera)
{
    //NOTE: Aspect ratio is not filled out and will need to be in the Win32 layer
#if 1
    m4 startingViewMatrix = Transpose(LookAtRH(camera->startEye,
					       camera->startAt,
					       camera->startUp));

#else
    m4 lookAt = LookAtRH(camera->startEye, camera->startAt, camera->startUp);

    m4 startingViewMatrix = Transpose(lookAt);
#endif


    camera->view = startingViewMatrix;
    camera->fovY = 2.0f * (r32)(atan(tan(ToRad(70) * 0.5f)) / camera->aspect.y);

#if 1
    camera->projection = Transpose(PerspectiveFovRH(
				       camera->fovY,
				       camera->aspect.x,
				       0.01f,
				       1000.0f)
	);

#else
    m4 persp = PerspectiveFovRH(camera->fovY,
				camera->aspect.x,
				0.01f,
				1000.0f);
    m4 transP = Transpose(persp);

    camera->projection = transP;
    
#endif    
}


