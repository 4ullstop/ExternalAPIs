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

#define MEMORY_POOLS_H
#endif
