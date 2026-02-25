#include "memory_pools.h"
#include <windows.h>

/*
  Might be useful to make this a DLL this way you can just rip the way that you created it before
 */


/*
  The memory should be set up as so:
   - There should be one large allocation up front
   - Inside of this large allocation, there are smaller chunks known as arenas
     - These arenas contain the data necessary for whatever function is required of them

   This way we only really need one allocation, the rest is just subdividing up the chunk
   into relevant portions

   Below is a diagram of what the memory should look like according to the description above
  
  
     a1        a2          a3            a4         a5    a6
  [000000] [00000000] [00000000000] [00000000000] [0000] [000]
  [00000000000000000000000000000000000000000000000000000000000]
  [------------------program memory---------------------------]

  a1: arena1
  a2: arena2
  a3: arena3
  a4: arena4
  a5: arena5
  a6: arena6
  
 */


void InitializeArena2(memory_arena* arena, memory_index size, u8* base)
{
    arena->size = size;
    arena->base = base;
    arena->used = 0;
}

void InitializeArena(memory_arena* arena, memory_index size, program_memory* memory, e_arena_type arenaType)
{
    //Figure out how to keep the memory aligned to boundaries, could be useful for future implementation
    //This will simply require that you figure out the padding required to the next boundary and then addding it:
    //arena->base = memory->currentArenaBase + padding;

    arena->size = size;
    arena->base = arenaType ? (u8*)memory->transientArenaBase : (u8*)memory->permanentArenaBase;
    arena->used = 0;

    if (arenaType)
	memory->transientArenaBase += size;
    else
	memory->permanentArenaBase += size;
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

//    memory->currentArenaBase = (u8*)memory->permanentStorage;
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

//Advanced memory pool code

//find a way to return just the listed memory nodes as an array, but have the nodes have "allocated" data
//associated with the returns

listed_memory_node*
PushListedMemoryNodes(memory_arena* arena, size_t sizeOfData, u32 numOfItems)
{
    
}

//for the love of god make sure your arenas are working as you think they are, I don't think they are
void InitializeListedMemory(listed_memory* record, memory_arena* listArena, size_t sizeOfData)
{
    //Push the data here
    size_t listSize = listArena->size - sizeof(listed_memory_node);
    u32 listCount = 0;

    record->dataSize = sizeOfData;
    size_t singleNodeSize = sizeof(listed_memory_node) + sizeOfData;
    listCount = (u32)(listSize / singleNodeSize);
    record->totalListSize = listCount;
    //Push again according to the size of the data we are inputting


    void* pushedSize = PushArraySized(listArena, (singleNodeSize * listCount) - singleNodeSize);
    size_t used = 0;
    size_t base = (u8*)pushedSize;
    
    for (u32 i = 0; i < listCount - 1; i++)
    {
	void* nodeSize;
	void* dataSize;
    }
    
    record->freeNodes =
	record->nodeArray =
	(listed_memory_node*)PushArraySized(listArena, (singleNodeSize * listCount) - singleNodeSize);
//since I'm already allocating larger than what is needed, find a way to subdivide such that,
    //we allocate not only the node but divide that memory up to include the data you want to
    //allocate as well

    //allocate our node data here, then we can pair the two
    for (size_t i = 0; i < listCount - 1; i++)
    {
	//So rather than setting next to array + 1, we cast to u8* and increment based on size of node + size of data
	//Might need to create special getter function for getting values of the linked list

	record->nodeArray[i].next = &record->nodeArray[i + 1];
	record->nodeArray[listSize - 1].next = 0;
    }

    
}

void* ListAlloc(listed_memory* rec)
{
    listed_memory_node* result = 0;
    rec->numOfItems = rec->numOfItems + 1;
    if ((rec == 0) || (rec->freeNodes == 0))
    {
	return(result);
    }
    result = rec->freeNodes;
    rec->freeNodes = rec->freeNodes->next;
    return(result);
}

void ListFree(listed_memory* rec, void* ptr)
{
    if ((rec == 0) || (ptr == 0))
    {
	return;
    }

    rec->numOfItems = rec->numOfItems - 1;
    
    listed_memory_node* freed = (listed_memory_node*)ptr;
    freed->next = rec->freeNodes;
    rec->freeNodes = freed;    
}

void RemoveListedItem(listed_memory* rec, listed_memory_node** recList)
{
    if (recList == 0)
    {
	return;
    }

    listed_memory_node* temp = *recList;
    *recList = (*recList)->next;

    ListFree(rec, temp);
}

void AddListedItem(listed_memory* rec, void* data, size_t dataSize, listed_memory_node** recList)
{
    Assert(dataSize == rec->dataSize);

    listed_memory_node* newNode = (listed_memory_node*)ListAlloc(rec);
    //You must push the data here
//    newNode->data = data;

    newNode->data = data;

    if (recList == 0)
    {
	*recList = newNode;
    }
    else
    {
	newNode->next = *recList;
	*recList = newNode;
    }
}


