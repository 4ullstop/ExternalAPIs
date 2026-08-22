#if !defined WIN32_FRAMEWORK_H

#include "../../Types/typedefs.h"
#include "../../Types/direct_x_typedefs.h"


#include "math.h"
#include "../../Math/forty_math.h"
#include "../../Types/game_types.h"



struct dx_camera
{
    constant_buffer_struct constantBufferData;
    DirectX::XMVECTOR position;
    DirectX::XMVECTOR right;
    DirectX::XMVECTOR worldUp;
    DirectX::XMVECTOR up;
    DirectX::XMVECTOR front;
    r32 yaw, pitch, movementSpeed, turnSpeed;

    
    DirectX::XMVECTOR startEye;
    DirectX::XMVECTOR startAt;
    DirectX::XMVECTOR startUp;

    r32 fovY;

    v2 aspect;
    
    r32 targetZoom, currZoom, lag, zoomingTo;
    DirectX::XMMATRIX viewInverted;
    DirectX::XMVECTOR targetPos;
    DirectX::XMVECTOR positionTo;
    
    DirectX::XMVECTOR targetQRot;
    DirectX::XMVECTOR qRotationTo;
    DirectX::XMVECTOR currQRot;
    
    DirectX::XMVECTOR viewCenter;
    DirectX::XMVECTOR eye;
    DirectX::XMVECTOR upDir;    
};

struct game_camera_data
{
    m4 world;
    m4 view;
    m4 projection;
};

#define WIN32_STATE_FILE_NAME_COUNT MAX_PATH

struct program_state
{
    bool32 running;

    char exeFilename[WIN32_STATE_FILE_NAME_COUNT];
    char* onePastExeFilenameSlash;
};

struct obj_conversion
{
    vertex_position_color* objVerts;
    u16* indices;
    u32 objVertsSize;
    u32 indexCount;
    u32* materialIndices;
    u32 faceCount;
    bool32 hasMaterials;

    material_properties* materialProperties;

    i32 numOfMaterials;
};

struct win32_game_code
{
    HMODULE gameCodeDLL;
    FILETIME dllLastWriteTime;

    //Defining in vague terms, to be cast
    //according to the game code created by the user
    //in the game layer
    void* GameUpdate;
    void* GameInitialize;
    
    bool32 isValid;
};

struct game_code_path_cluster
{
    char sourceGameCodeDLLFullPath[WIN32_STATE_FILE_NAME_COUNT];
    char tempGameCodeDLLFullPath[WIN32_STATE_FILE_NAME_COUNT];
    char gameCodeLockFullPath[WIN32_STATE_FILE_NAME_COUNT];

    char* updateFuncName;
    char* initFuncName;

    bool32 funcNameInit;
};

struct draw_buffers
{
    ID3D11Buffer* indexBuffer;
    ID3D11Buffer* vertexBuffer;
    i32 indexCount;
    
    ID3D11Buffer* materialBuffer;
    ID3D11ShaderResourceView* materialResourceView;

    ID3D11Buffer* materialPropertiesBuffer;
    ID3D11ShaderResourceView* materialPropertiesView;
    i32 faceCount;

    ID3D11Buffer* objectProperties;
    ID3D11ShaderResourceView* objectPropertiesView;
    u32 hasMaterials;

    
};

struct texture_buffers
{
    ID3D11Texture2D* textureData;
    ID3D11ShaderResourceView* textureResourceView;
};

struct win32_spawnable_objs
{
    draw_buffers* objDrawnBuffers;
    texture_buffers* textureBuffers;
};


#define WIN32_FRAMEWORK_H
#endif
