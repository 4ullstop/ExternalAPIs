#if !defined OBJ_LOADER_H

#include "../../FileReader/file_reader.h"

struct obj
{
    r32* vertices;
    i32 vertexCount;
    i32 vertexLastIndex;
    u16* vertexIndices;
    
    r32* vertexNormals;
    i32 vertexNormalCount;
    i32 vertexNormalLastIndex;
    u16* vertexNormalIndices;
    
    r32* vertexTextureCoordinates;
    i32 vertexTextureCoordCount;
    i32 vertexTextureCoordLastIndex;
    u16* vertexTextureCoordIndices;
    
    i32* face;
    i32 faceCount;
    i32 faceLastIndex;
};

#define OBJ_LOADER_H
#endif
