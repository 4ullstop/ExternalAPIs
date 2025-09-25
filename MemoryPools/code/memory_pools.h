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
};

#define MEMORY_POOLS_H
#endif
