#include <ai.h>

#include <cstring>
#include <string>
#include <sstream>



// ============================================================================

AI_PROCEDURAL_NODE_EXPORT_METHODS(MtoaTestProcProceduralMtd);


// FIXME move the parameters here !
node_parameters
{
    AiParameterBool  ("enableTest", true);
    AiParameterBool  ("renderCone", false);
    AiMetaDataSetBool(nentry, AtString(""), AtString("parallel_init"), true);
    
}

procedural_init
{
    return 1;
}

procedural_cleanup
{
    return 1;
}
procedural_num_nodes
{
    if (AiNodeGetBool(node, "enableTest"))
        return 1;

    return 0;
}

procedural_get_node
{
    AtNode *child = (AiNodeGetBool(node, "renderCone")) ? AiNode("cone", "yepeeTest", node) : AiNode("sphere", "yepeeTest", node);
    return child;  
}


node_loader
{
   if (i>0)
      return false;

   node->methods      = MtoaTestProcProceduralMtd;
   node->output_type  = AI_TYPE_NONE;
   node->name         = "mtoa_test_proc";
   node->node_type    = AI_NODE_SHAPE_PROCEDURAL;
   strcpy(node->version, AI_VERSION);
   return true;
}