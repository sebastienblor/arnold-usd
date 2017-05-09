#include <ai.h>
#include "debug.h"

AI_PROCEDURAL_NODE_EXPORT_METHODS(BifrostImplicitMtds)

node_parameters
{
}

procedural_init
{
    PROFILER("initialize implicit");
    return false;
}

procedural_cleanup
{
    return true;
}

procedural_num_nodes
{
    return 1;
}

procedural_get_node
{
    return NULL;
}
