#define _USE_MATH_DEFINES
#include <cmath>

#ifndef M_PI
#define M_PI 3.1415926535897932384626433832795
#endif

#include "../common/XgArnoldExpand.h"
#include <ai.h>
#include <vector>
#include <string>

using namespace XGenArnold;

static AtMutex s_mutex;

AI_PROCEDURAL_NODE_EXPORT_METHODS(XgArnoldProceduralMtd);

node_parameters
{
   AiParameterStr("data", "");
   AiMetaDataSetBool(nentry, AtString(""), AtString("parallel_init"), true);
}

procedural_init
{
   //AiMsgInfo("[xgArnoldProcedural] Init()");

   // Isn't this a bug on arnold side ?
   // If this procedural node is disabled, we shouldn't even get here
   if (AiNodeIsDisabled(node))
   {
      *user_ptr = 0;
      return 1;
   }

   s_mutex.lock();
   ProceduralWrapper* ud = new ProceduralWrapper( new Procedural(), false /* Won't do cleanup */ );
   if( !ud )
   {
      s_mutex.unlock();
      return 0;
   }

   *user_ptr = (void*)ud;

   int result = ud->Init( node, true ); // "true" means that the procedural parent must be set
   s_mutex.unlock();

   return result;
}

// Cleanup
procedural_cleanup
{
   // AiMsgDebug("[xgArnoldProcedural] Cleanup()");

   ProceduralWrapper* ud = (ProceduralWrapper*)user_ptr;
   if( !ud )
      return 0;

   int ret = ud->Cleanup();
   delete ud;
   return ret;
}

// Get number of nodes
procedural_num_nodes
{
   // AiMsgDebug("[xgArnoldProcedural] NumNodes()");

   ProceduralWrapper* ud = (ProceduralWrapper*)user_ptr;
   if( !ud )
      return 0;

   int numnodes = ud->NumNodes();
   return numnodes;
}

// Get the i_th node
procedural_get_node
{
   // AiMsgDebug("[xgArnoldProcedural] GetNode()");

   ProceduralWrapper* ud = (ProceduralWrapper*)user_ptr;
   if( !ud )
      return 0;

   AtNode *this_node = ud->GetNode(i);
   return this_node;
}

// DSO hook
#ifdef __cplusplus
extern "C"
{
#endif


node_loader
{
   if (i>0)
      return false;

   node->methods      = XgArnoldProceduralMtd;
   node->output_type  = AI_TYPE_NONE;
   node->name         = "xgen_procedural";
   node->node_type    = AI_NODE_SHAPE_PROCEDURAL;
   strcpy(node->version, AI_VERSION);

   return true;
}

#ifdef __cplusplus
}
#endif

