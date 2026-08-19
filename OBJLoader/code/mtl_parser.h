#if !defined MTL_PARSER_H


#include <stdio.h>
#include <stdlib.h>

#undef ALLOW_WIN32
#define ALLOW_WIN32 0
#include "../../MemoryPools/code/memory_pool_dll_include.h"

#include "../../Types/intrinsics.h"
#include "../../FileReader/bmp_reader.cpp"
#include "../../Math/forty_math.h"
#include "parser_shared.cpp"

struct mtl
{
    v3 ambient;
    v3 diffuse;
    v3 emmisive;
    v3 specular;
    r32 specExp;

    r32 opacityDensity;
    r32 dissolve;
    i32 illumination;

    char* name;
};

#define MTL_PARSER_H
#endif

