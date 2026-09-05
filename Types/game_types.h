#if !defined(GAME_TYPES_H)
#include "typedefs.h"
#include "../../OBJLoader/code/obj_loader.h"
#include "../../MemoryPools/code/memory_pool_dll_include.h"


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

    r32 leftStickAverageX;
    r32 leftStickAverageY;

    r32 rightStickAverageX;
    r32 rightStickAverageY;
    
    union
    {
	game_button_state buttons[21];
	struct
	{
	    game_button_state moveForward;
	    game_button_state moveBackward;
	    game_button_state moveRight;
	    game_button_state moveLeft;
	    game_button_state moveUp;
	    game_button_state moveDown;
	    
	    game_button_state testKey;

	    game_button_state one;
	    game_button_state two;
	    game_button_state three;
	    game_button_state four;
	    game_button_state five;
	    game_button_state six;
	    game_button_state seven;
	    
	    game_button_state back;
	    game_button_state start;

	    game_button_state lookRight;
	    game_button_state lookLeft;
	    game_button_state lookUp;
	    game_button_state lookDown;	    
	    
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

inline game_controller_input* GetController(game_input* input, u32 controllerIndex)
{
    game_controller_input* result = &input->controllers[controllerIndex];
    return(result);
}

enum spawnable_obj_type : i32
{
//    sot_ico = 0,
    sot_boat = 0,
	sot_mast = 1,
	sot_ref = 2,
	sot_windsock = 3,
	sot_axes = 4,
	sot_forward_arrow = 5,
	sot_texTest = 6,
	sot_ocean = 7,
	sot_wind_model_bottom = 8,
	sot_wind_model_top = 9,
	sot_marooned = 10,
	sot_wind_dir_cardinal = 11,	
};

enum texture_lookup
{
    tl_none = 0,
    tl_testing = 1,
    tl_wind_model = 2,
}; 

struct transform
{
    v4 location;
    v4 rotation;
    v4 scale;
};

struct spawned_obj_info
{
    spawnable_obj_type type;
    transform modelTransform;
    transform localTransform;
    m4 modelMatrix;
    m4 localMatrix;
    bool32 inheritsTransform;
    texture_lookup textureInfo;
    v3 rotations;
};

//OBJS loaded in the game that can be spawned
struct game_loaded_objs
{
    obj* staticLoadedObjs;
    i32 objFileNum;
    spawnable_obj_type* types;
    
    listed_memory* spawnedObjMemory;
    listed_memory_node* spawnedObjNodes;

    i32 queuedSpawningItems;
};

struct texture
{
    loaded_bitmap bmpTextureData;
};

struct game_loaded_textures
{
    texture* textures;
    i32 numOfTextures;
};

#define GAME_TYPES_H
#endif
