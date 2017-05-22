#include <ai.h>
#include "debug.h"

AI_VOLUME_NODE_EXPORT_METHODS(BifrostImplicitMtds)

node_parameters
{
}

volume_create
{
    PROFILER("initialize implicit");
    return false;
}

volume_cleanup
{
    return true;
}

volume_sample
{
    return false;
}

volume_gradient
{
    return false;
}

volume_ray_extents
{

}

volume_update
{
    return false;
}
