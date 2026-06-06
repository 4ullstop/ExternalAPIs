#include "game_framework.h"

internal void
CreateViewAndPerspective(game_camera* camera)
{
    //NOTE: Aspect ratio is not filled out and will need to be in the Win32 layer
    m4 startingViewMatrix = Transpose(LookAtRH(camera->startEye,
					       camera->startAt,
					       camera->startUp));

    camera->view = startingViewMatrix;
    camera->fovY = 2.0f * (r32)(atan(tan(ToRadians(70) * 0.5f)) / camera->aspect.y);
    camera->projection = Transpose(PerspectiveFovRH(
				       camera->fovY,
				       aspect.x,
				       0.01f,
				       1000.0f)
	);
}


