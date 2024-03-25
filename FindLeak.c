#include    <stdio.h>
#include    <malloc.h>
#include    <string.h>
#include    "findLeak.h"

#undef        malloc
#undef        calloc
#undef         free

static leakMem * ptr_start = NULL;
static leakMem * ptr_next =  NULL;

// -----------------------------------------------------------

// Name: MyMalloc

// Desc: This is hidden to user. when user call malloc function then 
//       this function will be called.

void *MyMalloc (uint nSize, cchar* file, uint lineNumber)
{
    void * ptr = malloc (nSize);
    if (ptr != NULL) 
    {
        SubAddMemInfo(ptr, nSize, file, lineNumber);
    }
    return ptr;
}


// -----------------------------------------------------------

// Name: MyCalloc

// Desc: This is hidden to user. when user call calloc function then 
//       this function will be called.

void * MyCalloc (uint elements, uint nSize, const char * file, uint lineNumber)
{
    uint tSize;
    void * ptr = calloc(elements , nSize);
    if(ptr != NULL)
    {
        tSize = elements * nSize;
        SubAddMemInfo (ptr, tSize, file, lineNumber);
    }
    return ptr;
}



// -----------------------------------------------------------

// Name: SubAdd

// Desc: It's actually  Adding the Info.


void SubAdd(infoMem alloc_info)
{
    leakMem * mem_leak_info = NULL;
    mem_leak_info = (leakMem *) malloc (sizeof(leakMem));
    mem_leak_info->memData.addr = alloc_info.addr;
    mem_leak_info->memData.nSize = alloc_info.nSize;
    strcpy(mem_leak_info->memData.fileName, alloc_info.fileName); 
    mem_leak_info->memData.lineNumber = alloc_info.lineNumber;
    mem_leak_info->nxt = NULL;
    if (ptr_start == NULL)
    {
        ptr_start = mem_leak_info;
        ptr_next = ptr_start;
    }
    else {
        ptr_next->nxt = mem_leak_info;
        ptr_next = ptr_next->nxt;    
    }
}


// -----------------------------------------------------------

// Name: ResetInfo

// Desc: It erasing the memory using by List on the basis of info( pos)

void ResetInfo(uint pos)
{
    uint index = 0;
    leakMem * alloc_info, * temp;

    if(pos == 0)
    {
        leakMem * temp = ptr_start;
        ptr_start = ptr_start->nxt;
        free(temp);
    }
    else 
    {
        for(index = 0, alloc_info = ptr_start; index < pos; 
        alloc_info = alloc_info->nxt, ++index)
        {
            if(pos == index + 1)
            {
                temp = alloc_info->nxt;
                alloc_info->nxt =  temp->nxt;
                free(temp);
                break;
            }
        }
    }
}



// -----------------------------------------------------------

// Name: DeleteAll

// Desc: It deletes the all elements which resides on List


void DeleteAll()
{
    leakMem * temp = ptr_start;
    leakMem * alloc_info = ptr_start;
    while(alloc_info != NULL) 
    {
        alloc_info = alloc_info->nxt;
        free(temp);
        temp = alloc_info;
    }
}



// -----------------------------------------------------------

// Name: MyFree

// Desc:


void MyFree(void * mem_ref)
{
    uint loop;
   // if the allocated memory info is part of the list, removes it
    leakMem  *leak_info = ptr_start;
    /* check if allocate memory is in our list */
    for(loop = 0; leak_info != NULL; ++loop, leak_info = leak_info->nxt)
    {
        if ( leak_info->memData.addr == mem_ref )
        {
            ResetInfo(loop);
            break;
        }
    }
    free(mem_ref);
}


// -----------------------------------------------------------
// Name: SubAddMemInfo

// Desc: it also fill the the Info


void SubAddMemInfo (void * mem_ref, uint nSize, cchar * file, uint lineNumber)
{
    infoMem AllocInfo;

    /* fill up the structure with all info */

    memset( &AllocInfo, 0, sizeof ( AllocInfo ) );
    AllocInfo.addr     = mem_ref;
    AllocInfo.nSize = nSize;
    strncpy(AllocInfo.fileName, file, MAX_FILENAME_LENGTH);
    AllocInfo.lineNumber = lineNumber;

    /* SubAdd the above info to a list */
    SubAdd(AllocInfo);
}


// -----------------------------------------------------------

// Name: WriteMemLeak

// Desc: It writes information about Memory leaks in a file 
void WriteMemLeak(void)
{
    uint index;
    leakMem *leak_info;
    FILE * fp_write = fopen(OutFile, "wt");
    char info[1024];
    if(fp_write != NULL)
    {
        sprintf(info, "%s\n", "SUMMARY ABOUT MEMORY LEAKS OF YOUR SOURCE FILE ");
        fwrite(info, (strlen(info) + 1) , 1, fp_write);
        sprintf(info, "%s\n", "-----------------------------------");   
        fwrite(info, (strlen(info) + 1) , 1, fp_write);

        for(leak_info = ptr_start; leak_info != NULL; leak_info = leak_info->nxt)
        {
            sprintf(info, "Name of your Source File : %s\n", leak_info->memData.fileName);
            fwrite(info, (strlen(info) + 1) , 1, fp_write);
            sprintf(info, "Starting Address : %p\n", leak_info->memData.addr);
            fwrite(info, (strlen(info) + 1) , 1, fp_write);
            sprintf(info, " Total size Of memory Leak : %d bytes\n", leak_info->memData.nSize);            
            fwrite(info, (strlen(info) + 1) , 1, fp_write);
            sprintf(info, "Line Number for which no DeAllocation    : %d\n", leak_info->memData.lineNumber);
            fwrite(info, (strlen(info) + 1) , 1, fp_write);
            sprintf(info, "%s\n", "-----------------------------------");   
            fwrite(info, (strlen(info) + 1) , 1, fp_write);
            fwrite(info, (strlen(info) + 1) , 1, fp_write);
        }
    }  
    DeleteAll();
}
