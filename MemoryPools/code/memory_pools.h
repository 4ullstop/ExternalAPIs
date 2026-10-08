#if !defined MEMORY_POOLS_H

#include "../../Types/typedefs.h"

struct memory_arena
{
    memory_index size;
    u8* base;
    memory_index used;
};

enum e_arena_type
{
    permanent,
    transient,
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

    u8* permanentArenaBase;
    u8* transientArenaBase;
};

struct listed_memory_node
{
    listed_memory_node* next;
    listed_memory_node* prev;
    
    u8 set;
    //Push a piece of data of a specified size (constant)
    void* data;
};

struct listed_memory
{
    size_t dataSize;
    size_t nodeSize;
    
    i32 numOfItems;
    u32 totalListSize;
    
    listed_memory_node* freeNodes;
    listed_memory_node* nodeArray;
};


void* MemCpy(void* dest, void* src, size_t n)
{
    if (dest == nullptr) return(nullptr);
    u8* uDest = (u8*)dest;
    u8* uSrc = (u8*)src;

    for (i32 i = 0; i < n; i++)
    {
	uDest[i] = uSrc[i];
    }

    return(dest);
}


#define MEMORY_POOLS_H
#endif
