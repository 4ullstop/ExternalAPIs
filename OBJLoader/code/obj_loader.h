#if !defined OBJ_LOADER_H

#include "../../FileReader/file_reader.h"

struct obj
{
    r32* vertexIndices;
    i32 vertexCount;
    i32 vertexLastIndex;
    
    r32* vertexNormals;
    i32 vertexNormalCount;
    i32 vertexNormalLastIndex;
    
    r32* vertexTextureCoordinates;
    i32 vertexTextureCoordCount;
    i32 vertexTextureCoordLastIndex;
    
    i32* face;
    i32 faceCount;
    i32 faceLastIndex;
};

#define OBJ_LOADER_H
#endif
