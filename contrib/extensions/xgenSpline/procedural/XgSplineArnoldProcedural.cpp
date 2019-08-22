#include <ai.h>


#include <SgCurve.h>
#include <XgSplineAPI.h>

#include <cstring>
#include <string>
#include <sstream>

#include "../common/XgSplineArnoldExpand.h"

using namespace XgArnoldInternal;

AI_PROCEDURAL_NODE_EXPORT_METHODS(XgSplineProceduralMtd);


// FIXME move the parameters here !
node_parameters
{
   AiParameterBool  ("face_camera", false);
   AiMetaDataSetBool(nentry, AtString(""), AtString("parallel_init"), true);
}

procedural_init
{
   if (AiNodeIsDisabled(node))
   {
      *user_ptr = 0;
      return 1;
   }
   
   XgSplineProcedural* proc = new XgSplineProcedural();
   if (!proc->Init(node, true)) // "true" because we must set the parent procedural
   {
      delete proc;
      proc = NULL;
   }
   *user_ptr = reinterpret_cast<void*>(proc);
   return proc ? 1 : 0;
}

procedural_cleanup
{
    delete reinterpret_cast<XgSplineProcedural*>(user_ptr);
    return 1;
}
procedural_num_nodes
{
    XgSplineProcedural* proc = reinterpret_cast<XgSplineProcedural*>(user_ptr);
    return proc ? proc->NumNodes() : 0;
}

procedural_get_node
{
    XgSplineProcedural* proc = reinterpret_cast<XgSplineProcedural*>(user_ptr);
    return proc ? proc->GetNode(i) : NULL;
}


node_loader
{
   if (i>0)
      return false;

   node->methods      = XgSplineProceduralMtd;
   node->output_type  = AI_TYPE_NONE;
   node->name         = "xgenProcedural"; // why isn't this called xgen_spline_procedural ??
   node->node_type    = AI_NODE_SHAPE_PROCEDURAL;
   strcpy(node->version, AI_VERSION);
   return true;
}
