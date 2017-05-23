#include <ai.h>
#include "bifrost_shape.h"
#include <bifrostprocessing/bifrostprocessing_volume.h>
#include "defs.h"
#include "utils.h"
#include "debug.h"

namespace {

class Volume : public Bifrost::Processing::Volume{
public:
    Volume() : Bifrost::Processing::Volume(){}

    Volume(const AtNode* node) : Bifrost::Processing::Volume() {
        GET_SHAPE();
        GET_STR(density_channel);
    }

    virtual Bifrost::Processing::Status initialize() override{
        return report(Bifrost::Processing::Volume::initialize());
    }

    void declare(AtList* params, AtNodeEntry* nentry){
        PARAM_SHAPE(({"vorticity"}), ({"velocity"}));
        PARAM_STR(density_channel);
    }

    virtual Bifrost::API::String str() const override{
        std::stringstream ss;
        ss << Bifrost::Processing::Volume::str().c_str();
        DUMP_PARAM(density_channel);
        return ss.str().c_str();
    }

    bool sample(const AtString& channel, uint16_t tid, AtParamValue *value, uint8_t *type, int interp){
        return false;
    }
};

}

AI_VOLUME_NODE_EXPORT_METHODS(BifrostVolumeMtds)

node_parameters
{
    Volume().declare(params, nentry);
}

volume_create
{
    data->private_info = nullptr;
    Volume* volume = new Volume(node);
    if(!volume->initialize()){
        delete volume;
        return false;
    }
    DUMP(volume->str());
    data->private_info = volume;
    data->auto_step_size = .1;
    data->bbox.min.x = data->bbox.min.y = data->bbox.min.z = -100;
    data->bbox.max.x = data->bbox.max.y = data->bbox.max.z =  100;
    return true;
}

volume_cleanup
{
    if(data->private_info) delete (Volume*) data->private_info;
    data->private_info = nullptr;
    return true;
}

volume_sample
{
    if(!data->private_info) return false;
    Volume* volume = static_cast<Volume*>(data->private_info);
    return volume->sample(channel, sg->tid, value, type, interp);
    return false;
}

volume_ray_extents
{
    // TODO: compute extents
    AiVolumeAddIntersection(info, t0, t1);
}

volume_gradient
{
    return false;
}
volume_update
{
    return true;
}

