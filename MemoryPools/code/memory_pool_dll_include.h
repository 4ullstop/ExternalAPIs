#if !defined MEMORY_POOLS_DLL_INCLUDE_H
#include "memory_pools.h"

#define MEMORY_POOL_PUSH_STRUCT(name) void* name(memory_arena* arena, memory_index size)
typedef MEMORY_POOL_PUSH_STRUCT(memory_pool_push_struct);
MEMORY_POOL_PUSH_STRUCT(MemoryPoolPushStructStub)
{
    return(0);
}
global_variable memory_pool_push_struct* MemoryPoolPushStruct_ = MemoryPoolPushStructStub;
#define MemoryPoolPushStruct MemoryPoolPushStruct_

#define MEMORY_POOL_PUSH_ARRAY(name) void* name(memory_arena* arena, i32 count, void* type)
typedef MEMORY_POOL_PUSH_ARRAY(memory_pool_push_array);
MEMORY_POOL_PUSH_ARRAY(MemoryPoolPushArrayStub)
{
    return(0);
}
global_variable memory_pool_push_array* MemoryPoolPushArray_ = MemoryPoolPushArrayStub;
#define MemoryPoolPushArray MemoryPoolPushArray_

#define MEMORY_POOL_ALLOC(name) void name(LPVOID lpAddress, DWORD flAllocationType, DWORD flProtect, program_memory* memory)
typedef MEMORY_POOL_ALLOC(memory_pool_alloc);
MEMORY_POOL_ALLOC(PoolAllocStub)
{

}
global_variable memory_pool_alloc* PoolAlloc_ = PoolAllocStub;
#define PoolAlloc PoolAlloc_

#define MEMORY_POOL_INITIALIZE_ARENA(name) void name(memory_arena* arena, memory_index size, u8* base)
typedef MEMORY_POOL_INITIALIZE_ARENA(memory_pool_initialize_arena);
MEMORY_POOL_INITIALIZE_ARENA(PoolInitializeStub)
{

}
global_variable memory_pool_initialize_arena* InitializeArena_ = PoolInitializeStub;
#define InitializeArena InitilaizeArena_

#define MEMORY_POOL_CLEAR_ARENA(name) void name(memory_arena* arena)
typedef MEMORY_POOL_CLEAR_ARENA(memory_pool_clear_arena);
MEMORY_POOL_CLEAR_ARENA(PoolClearArenaStub)
{

}
global_variable memory_pool_clear_arena* PoolClear_ = PoolClearArenaStub;
#define PoolClear PoolClear_

#define MEMORY_POOL_PUSH_ARRAY_SIZED(name) void* name(memory_arena* arena, size_t size)
typedef MEMORY_POOL_PUSH_ARRAY_SIZED(memory_pool_push_array_sized);
MEMORY_POOL_PUSH_ARRAY_SIZED(PoolPushArraySizedStub)
{
    return(0);
}
global_variable memory_pool_push_array_sized* PushArraySized_ = PoolPushArraySizedStub;
#define PushArraySized PushArraySized_

struct memory_pool_dll_code
{
    memory_pool_push_struct* PushStruct;
    memory_pool_push_array* PushArray;
    memory_pool_alloc* PoolAlloc;
    memory_pool_initialize_arena* InitializeArena;
    memory_pool_clear_arena* ClearArena;
    memory_pool_push_array_sized* PushArraySized;
};

#define MEMORY_POOLS_DLL_INCLUDE_H
#endif
