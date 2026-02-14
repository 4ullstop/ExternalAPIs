#if !defined OBJ_PARSER_DLL_INCLUDE_H
#include "obj_loader.h"

#define PARSE_OBJ_DATA(name) obj* name(char* fileLocation, memory_arena* objLocationArena, program_memory* mainProgramMemory)
typedef PARSE_OBJ_DATA(parse_obj_data);
PARSE_OBJ_DATA(ParseOBJDataStub)
{
    return(NULL);
}
global_variable parse_obj_data* ParseOBJData_ = ParseOBJDataStub;
#define ParseOBJData ParseOBJData_

struct parse_obj_data_code
{
    parse_obj_data* ParseOBJData;
};

#define OBJ_PARSER_DLL_INCLUDE_H
#endif
