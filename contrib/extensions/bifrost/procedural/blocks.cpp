#include "blocks.h"
#include <ai.h>
#include <bifrostapi/bifrost_pointcomponent.h>
#include <bifrostapi/bifrost_visitor.h>
#include <bifrostapi/bifrost_layout.h>
#include "defs.h"
#include "utils.h"
#include "debug.h"
#include "tbb.h"
#include "sampler.h"

namespace{

class BlockVisitor : public Bifrost::API::Visitor{
public:
    BlockVisitor(std::vector<AtNode*>& nodes, float dx) : nodes(nodes), dx(dx){}
    void beginTile(const Bifrost::API::TileAccessor& accessor, const Bifrost::API::TreeIndex& index){
        Bifrost::API::TileCoord coord = accessor.tile(index).coord();
        AtNode* box = AiNode("box");
        amino::Math::vec3f p((coord.i-.5)*dx, (coord.j-.5)*dx, (coord.k-.5)*dx);
        AiNodeSetVec(box, "min", p.v[0], p.v[1], p.v[2]);
        AiNodeSetVec(box, "max", p.v[0]+5*dx, p.v[1]+5*dx, p.v[2]+5*dx);
        nodes.push_back(box);
    }
private:
    std::vector<AtNode*>& nodes;
    float dx;
};

}

BlocksParameters::BlocksParameters() : SurfaceParameters() {}
BlocksParameters::BlocksParameters(const AtNode* node) : SurfaceParameters(node) {}
void BlocksParameters::declare(AtList *params, AtNodeEntry *nentry){
    SurfaceParameters::declare(params, nentry);
    AiParameterStr("field_channel", distance_channel.c_str());
}

std::vector<AtNode*>* BlocksParameters::nodes() const{
    Bifrost::Processing::Surface surface(*this);
    //DUMP(this->str());
    report(surface.status());
    if(!surface.status()){
        return nullptr;
    }
    Bifrost::API::VoxelComponent voxels(surface.voxels());
    std::vector<AtNode*>* out = new std::vector<AtNode*>();
    std::vector<AtNode*>& nodes = *out;

    Bifrost::API::Layout layout(voxels.layout());
    BlockVisitor visitor(nodes, layout.voxelScale());
    layout.traverse(visitor, Bifrost::API::TraversalMode::BreadthFirst, layout.maxDepth(), layout.maxDepth());
    return out;
}

AI_PROCEDURAL_NODE_EXPORT_METHODS(BifrostBlocksMtds)

node_parameters
{
    BlocksParameters().declare(params,nentry);
}
procedural_init
{
    return (*user_ptr = BlocksParameters(node).nodes()) != nullptr;
}
procedural_cleanup
{
    if(user_ptr) delete static_cast< std::vector<AtNode*>* >(user_ptr);
    return true;
}
procedural_num_nodes
{
    return user_ptr? static_cast< std::vector<AtNode*>* >(user_ptr)->size() : 0;
}
procedural_get_node
{
    std::vector<AtNode*>* nodes = user_ptr? static_cast< std::vector<AtNode*>* >(user_ptr) : nullptr;
    return (nodes && i >= 0 && i < (int) nodes->size())? (*nodes)[i] : nullptr;
}
