#include "memory_pools.h"
#include <windows.h>

/*
  Might be useful to make this a DLL this way you can just rip the way that you created it before
 */



void InitializeArena(memory_arena* arena, memory_index size, u8* base)
{
    arena->size = size;
    arena->base = base;
    arena->used = 0;
}

//This function allocates and sets the locations of the permanent and trasient storage 
void PoolAlloc(LPVOID lpAddress, DWORD flAllocationType, DWORD flProtect, program_memory* memory)
{
    memory->totalSize = memory->permanentStorageSize + memory->transientStorageSize;
    size_t size = (size_t)memory->totalSize;
    
    memory->memoryBlock = VirtualAlloc(lpAddress, size, flAllocationType, flProtect);

    if (memory->memoryBlock)
    {
	memory->permanentStorage = memory->memoryBlock;
	memory->transientStorage = ((u8*) memory->permanentStorage + memory->permanentStorageSize);
    }
}

void* PushSize_(memory_arena* arena, memory_index size)
{
    Assert((arena->used + size) <= arena->size);
    void* result = arena->base + arena->used;
    arena->used += size;

    return(result);
}

void* PushStruct(memory_arena* arena, memory_index size)
{
    return(PushSize_(arena, size));
}

void* PushArray(memory_arena* arena, i32 count, void* type)
{
    return(PushSize_(arena, (count)*sizeof(type)));
}

void* PushArraySized(memory_arena* arena, size_t size)
{
    return(PushSize_(arena, size));
}

void ClearArena(memory_arena* arena)
{
    arena->used = 0;
}
