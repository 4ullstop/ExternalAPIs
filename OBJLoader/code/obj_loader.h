#if !defined OBJ_LOADER_H


#include "../../FileReader/file_reader.h"
#include "mtl_parser.h"
#include "../../Math/forty_math_fast.h"

/*
  Upload material index to pixel shader as constant buffer according to mesh
  materialIndex[] = {0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2...}
  where the numbers in the list are the material indicies according to (ie): v4 color[numOfMaterials];

  hlsl:
  
  result = color[materialIndex[faceID]]
 */

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

    v4I* faces;
    i32* facesToMaterialIndices;
    
    bool32* renderFace;
    //Grab the verts of the face via: face[i + 6]

    //Temporary, not final, just need something for testing code
    r32* colors;

    mtl* materials;
    i32 numOfMaterials;
    bool32 hasMaterials;
    char** materialNames;
};



#define OBJ_LOADER_H
#endif
