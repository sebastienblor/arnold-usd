#include <ai.h>
#include "debug.h"

AI_PROCEDURAL_NODE_EXPORT_METHODS(BifrostPointsMtds)

node_parameters
{
}

procedural_init
{
    PROFILER("initialize points");
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
