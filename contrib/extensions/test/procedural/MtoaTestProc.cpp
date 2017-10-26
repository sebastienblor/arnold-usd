#include <ai.h>

#include <cstring>
#include <string>
#include <sstream>



// ============================================================================

AI_PROCEDURAL_NODE_EXPORT_METHODS(MtoaTestProcProceduralMtd);


// FIXME move the parameters here !
node_parameters
{
    AiParameterBool  ("enableTest", false);
    
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
    return 1;
}

procedural_get_node
{
    AtNode *sphere = AiNode("sphere");
    AiNodeSetStr(sphere, "name", "yepeeTest" );
    return sphere;  
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