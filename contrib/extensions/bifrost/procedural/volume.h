#pragma once

#include "shape.h"
#include <bifrostprocessing/bifrostprocessing_volume.h>
#include "sampler.h"
#include <ai_bbox.h>

class Volume;

struct VolumeParameters : public Bifrost::Processing::VolumeParameters{
    VolumeParameters();
    VolumeParameters(const AtNode* node);

    virtual Bifrost::API::String str() const override;
    void declare(AtList* params, AtNodeEntry* nentry);

    Volume* volume() const;
};

class Volume{
public:
    Volume(const VolumeParameters& params);
    inline const Bifrost::Processing::Status& status(){ return volume.status(); }
    bool sample(const AtString& channel, const AtVector& pos, uint16_t tid, AtParamValue *value, uint8_t *type, int interp);
    AtBBox bbox() const;

private:
    Bifrost::Processing::Volume volume;
    std::vector<ComponentSampler> samplers;
};

