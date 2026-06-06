#if !defined WIN32_FRAMEWORK_H
#include "../../Types/typedefs.h"
#include "../../Types/direct_x_typedefs.h"

#include "math.h"
#include "../../Math/forty_math.h"

//define types inside of the framework

struct mouse_movements
{
    v2 loc;
};

enum e_mouse_buttons
{
    middle_mouse,
    left_mouse,
    right_mouse,
    middle_mouse_up,
    middle_mouse_down,
};

struct game_button_state
{
    i32 halfTransitionCount;
    bool32 endedDown;
    bool32 wasDown;
    bool32 started;
    bool32 held;
    bool32 released;

    i32 heldTime;
};

struct game_controller_input
{
    bool32 isAnalog;
    bool32 isConnected;

    bool32 started;
    bool32 inputPreviousFrame;
    
    union
    {
	game_button_state buttons[8];
	struct
	{
	    game_button_state moveForward;
	    game_button_state moveBackward;
	    game_button_state moveRight;
	    game_button_state moveLeft;
	    game_button_state moveUp;
	    game_button_state moveDown;
	    
	    game_button_state testKey;

	    game_button_state terminator;
	};
    };
};

struct game_input
{
    game_button_state mouseButtons[5];
    i32 mouseXUnbounded, mouseYUnbounded, mouseZUnbounded;

    i32 mouseXBounded, mouseYBounded, mouseZBounded;

    r32 dTime;
    game_controller_input controllers[5];
};

struct dx_camera
{
    constant_buffer_struct constantBufferData;

    
}

#define WIN32_FRAMEWORK_H
#endif
