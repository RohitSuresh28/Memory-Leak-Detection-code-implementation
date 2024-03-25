#define   uint          unsigned int 
#define   cchar         const char  
#define  OutFile       "/home/rohit/MemLeakInfo.txt"   // Just Suppose

#define  MAX_FILENAME_LENGTH    256
#define  calloc(objs, nSize)    MyCalloc (objs, nSize, __FILE__, __LINE__)
#define  malloc(nSize)          MyMalloc (nSize, __FILE__, __LINE__)
#define  free(rMem)             MyFree(rMem)

// This structure is keeping info about memory leak

struct InfoMem
{
    void *addr;
    uint nSize;
    char fileName[MAX_FILENAME_LENGTH];
    uint lineNumber;
};
typedef struct InfoMem infoMem;

//This is link list of InfoMem which keeps a List of memory Leak in a source file

struct LeakMem
{
    infoMem memData;
    struct LeakMem *nxt;
};

typedef struct LeakMem leakMem;

void WriteMemLeak(void);

void SubAddMemInfo(void *rMem, uint nSize,  cchar  *file, uint lno);

void SubAdd(infoMem alloc_info);

void ResetInfo(uint pos); //erase

void DeleteAll(void); //clear(void);

void *MyMalloc(uint size, cchar *file, uint line);

void *MyCalloc(uint elements, uint size, cchar * file, uint lno);

void  MyFree(void * mem_ref);
