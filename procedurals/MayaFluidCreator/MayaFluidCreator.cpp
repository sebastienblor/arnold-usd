#include <ai.h>
#include <cstring>
#include <cstdio>

static int Init(AtNode* mynode, void** user_ptr)
{
   return true;
}

static int Cleanup(void* user_ptr)
{
   return true;
}

static int NumNodes(void* user_ptr)
{
   return 0;
}

static AtNode* GetNode(void* user_ptr, int i)
{
   return 0;
}


// DSO hook
#ifdef __cplusplus
extern "C"
{
#endif

AI_EXPORT_LIB int ProcLoader(AtProcVtable *vtable)
{
   vtable->Init = Init;
   vtable->Cleanup = Cleanup;
   vtable->NumNodes = NumNodes;
   vtable->GetNode = GetNode;

   sprintf(vtable->version, AI_VERSION);
   return 1;
}

#ifdef __cplusplus
}
#endif
