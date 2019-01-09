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

class Guard
{
public:

    Guard(){
        AiCritSecInitRecursive(&critical_section);
    }
    virtual ~Guard() {
        AiCritSecClose(&critical_section);
    }

    void enter() {
        AiCritSecEnter(&critical_section);
    }
    void leave() {
        AiCritSecLeave(&critical_section);
    }

private:
    AtCritSec critical_section;
};

Guard guard;

AI_PROCEDURAL_NODE_EXPORT_METHODS(XgArnoldProceduralMtd);

node_parameters
{
   AiParameterStr("data", "");
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

   guard.enter();
   ProceduralWrapper* ud = new ProceduralWrapper( new Procedural(), false /* Won't do cleanup */ );
   if( !ud )
   {
      guard.leave();
      return 0;
   }

   *user_ptr = (void*)ud;

   bool result = ud->Init( node, true ); // "true" means that the procedural parent must be set
   guard.leave();

   return result;
}

// Cleanup
procedural_cleanup
{
   // AiMsgDebug("[xgArnoldProcedural] Cleanup()");

   guard.enter();
   ProceduralWrapper* ud = (ProceduralWrapper*)user_ptr;
   if( !ud )
   {
         guard.leave();
         return 0;
   }
   int ret = ud->Cleanup();
   delete ud;
   guard.leave();
   return ret;
}

// Get number of nodes
procedural_num_nodes
{
   // AiMsgDebug("[xgArnoldProcedural] NumNodes()");

   guard.enter();
   ProceduralWrapper* ud = (ProceduralWrapper*)user_ptr;
   if( !ud )
   {
      guard.leave();
      return 0;
   }
   int numnodes = ud->NumNodes();
   guard.leave();
   return numnodes;
}

// Get the i_th node
procedural_get_node
{
   // AiMsgDebug("[xgArnoldProcedural] GetNode()");

   guard.enter();
   ProceduralWrapper* ud = (ProceduralWrapper*)user_ptr;
   if( !ud )
   {
      guard.leave();
      return 0;
   }
   AtNode *this_node = ud->GetNode(i);
   guard.leave();
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

