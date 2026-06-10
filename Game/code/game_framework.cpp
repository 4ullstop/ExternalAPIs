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
