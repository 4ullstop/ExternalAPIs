#if !defined MEMORY_POOLS_DLL_INCLUDE_H
#include "memory_pools.h"

#if !defined ALLOW_WIN32
#define ALLOW_WIN32 1
#endif

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

/*
  This is kinda a poor work around for the time being
  It would be nice to find a good way to avoid using a windows specific alloc for page allocations
  Or find a good way to interchange them, (preferably better than the idea I currently have
  which would make use of void* types which are casted .......)

  But for now I'm just gonna define a macro as a compiler flag if I don't want to use Win32 specific code
  bc it's only used for this allocation which is something that is done at the beginning of the program
  likely only ever once

  To allow for backwards compatability, if the compiler flag does not exist, I create it and allow it to exist
  bc the code from the past worked completely fine (I say without having tested compilation...)

  Tanks for coming to my ted talk
 */
#if ALLOW_WIN32

//Real type
#define MEMORY_POOL_ALLOC(name) void name(LPVOID lpAddress, DWORD flAllocationType, DWORD flProtect, program_memory* memory)
typedef MEMORY_POOL_ALLOC(memory_pool_alloc);
MEMORY_POOL_ALLOC(PoolAllocStub)
{

}
global_variable memory_pool_alloc* PoolAlloc_ = PoolAllocStub;
#define PoolAlloc PoolAlloc_

#else
//DUMMY HEAD
#define MEMORY_POOL_ALLOC(name) void name(void)
typedef MEMORY_POOL_ALLOC(memory_pool_alloc);
MEMORY_POOL_ALLOC(PoolAllocStub)
{

}
global_variable memory_pool_alloc* PoolAlloc_ = PoolAllocStub;

#endif


#define MEMORY_POOL_INITIALIZE_ARENA(name) void name(memory_arena* arena, memory_index size, program_memory* memory, e_arena_type arenaType)
typedef MEMORY_POOL_INITIALIZE_ARENA(memory_pool_initialize_arena);
MEMORY_POOL_INITIALIZE_ARENA(PoolInitializeStub)
{

}
global_variable memory_pool_initialize_arena* InitializeArena_ = PoolInitializeStub;
#define InitializeArena InitializeArena_

#define MEMORY_POOL_INITIALIZE_ARENA2(name) void name(memory_arena* arena, memory_index size, u8* base)
typedef MEMORY_POOL_INITIALIZE_ARENA2(memory_pool_initialize_arena2);
MEMORY_POOL_INITIALIZE_ARENA2(PoolInitialize2Stub)
{

}
global_variable memory_pool_initialize_arena2* InitializeArena2_ = PoolInitialize2Stub;
#define InitializeArena2 InitializeArena2_

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

#define MEMORY_POOL_INIT_LISTED_MEMORY(name) void name(listed_memory* record, memory_arena* listArena, size_t sizeOfData)
typedef MEMORY_POOL_INIT_LISTED_MEMORY(memory_pool_init_listed_memory);
MEMORY_POOL_INIT_LISTED_MEMORY(InitListedMemoryStub)
{

}
global_variable memory_pool_init_listed_memory* InitListedMemory_ = InitListedMemoryStub;
#define InitListedMemory InitListedMemory_

#define MEMORY_POOL_ADD_LISTED_ITEM(name) void name(listed_memory* rec, void* data, size_t dataSize, listed_memory_node** recList)
typedef MEMORY_POOL_ADD_LISTED_ITEM(memory_pool_add_listed_item);
MEMORY_POOL_ADD_LISTED_ITEM(AddListedItemStub)
{

}
global_variable memory_pool_add_listed_item* AddListedItem_ = AddListedItemStub;
#define AddListedItem AddListedItem_

#define MEMORY_POOL_ADD_TO_END(name) void name(listed_memory* rec, void* data, size_t dataSize, listed_memory_node** recList)
typedef MEMORY_POOL_ADD_TO_END(memory_pool_add_to_end);
MEMORY_POOL_ADD_TO_END(AddToEndStub)
{

}
global_variable memory_pool_add_to_end* AddToEnd_ = AddToEndStub;
#define AddToEnd AddToEnd_

#define MEMORY_POOL_REMOVE_LISTED_ITEM(name) void name(listed_memory* rec, listed_memory_node** recList)
typedef MEMORY_POOL_REMOVE_LISTED_ITEM(memory_pool_remove_listed_item);
MEMORY_POOL_REMOVE_LISTED_ITEM(RemoveListedItemStub)
{

}
global_variable memory_pool_remove_listed_item* RemoveListedItem_ = RemoveListedItemStub;
#define RemoveListedItem RemoveListedItem_

#define MEMORY_POOL_REMOVE_SPECIFIC_NODE(name) void name(listed_memory* rec, listed_memory_node** recList, listed_memory_node* nodeToRemove)
typedef MEMORY_POOL_REMOVE_SPECIFIC_NODE(memory_pool_remove_specific_node);
MEMORY_POOL_REMOVE_SPECIFIC_NODE(RemoveSpecificNodeStub)
{

}
global_variable memory_pool_remove_specific_node* RemoveSpecificNode_ = RemoveSpecificNodeStub;
#define RemoveSpecificNode RemoveSpecificNode_

struct memory_pool_dll_code
{
    memory_pool_push_struct* PushStruct;
    memory_pool_push_array* PushArray;
    memory_pool_alloc* PoolAlloc;
    memory_pool_initialize_arena* InitArena;
    memory_pool_initialize_arena2* InitArena2;
    memory_pool_clear_arena* ClearArena;
    memory_pool_push_array_sized* PushArraySized;

    memory_pool_init_listed_memory* InitListedMemory;
    memory_pool_add_listed_item* AddListedItem;
    memory_pool_add_to_end* AddToEndOfList;
    memory_pool_remove_listed_item* RemoveListedItem;
    memory_pool_remove_specific_node* RemoveSpecificNode;
};

#define MEMORY_POOLS_DLL_INCLUDE_H
#endif
