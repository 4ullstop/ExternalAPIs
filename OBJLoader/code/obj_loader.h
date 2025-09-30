#if !defined OBJ_LOADER_H

#include "../../FileReader/file_reader.h"

struct obj
{
    r32* vertices;
    i32 vertexCount;
    i32 vertexLastIndex;
    i32* vertexIndices;
    
    r32* vertexNormals;
    i32 vertexNormalCount;
    i32 vertexNormalLastIndex;
    i32* vertexNormalIndices;
    
    r32* vertexTextureCoordinates;
    i32 vertexTextureCoordCount;
    i32 vertexTextureCoordLastIndex;
    i32* vertexTextureCoordIndices;
    
    i32* face;
    i32 faceCount;
    i32 faceLastIndex;
};

#define OBJ_LOADER_H
#endif
