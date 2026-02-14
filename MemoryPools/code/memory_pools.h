#if !defined MEMORY_POOLS_H

#include "../../Types/typedefs.h"

struct memory_arena
{
    memory_index size;
    u8* base;
    memory_index used;
};


struct program_memory
{
    u64 totalSize;
    void* memoryBlock;
    
    bool32 isInitialized;

    u64 permanentStorageSize;
    void* permanentStorage;

    u64 transientStorageSize;
    void* transientStorage;

    u8* currentArenaBase;
};

struct listed_memory_node
{
    listed_memory_node* next;

    //Push a piece of data of a specified size (constant)
    void* data;
};

struct listed_memory
{
    size_t dataSize;
    
    listed_memory_node* freeNodes;
    listed_memory_node* nodeArray;
};

#define MEMORY_POOLS_H
#endif
