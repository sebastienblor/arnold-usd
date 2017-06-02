#pragma once

#include "shape.h"
#include <bifrostprocessing/bifrostprocessing_volume.h>
#include "sampler.h"
#include <ai_bbox.h>

class Volume;

struct VolumeParameters : public Bifrost::Processing::VolumeParameters{
    float step_size = .1;
    bool ignore_motion_blur = false;

    VolumeParameters();
    VolumeParameters(const AtNode* node);

    virtual Bifrost::API::String str() const override;
    void declare(AtList* params, AtNodeEntry* nentry);

    Volume* volume() const;
};

class Volume{
public:
    Volume(const VolumeParameters& params);
    inline bool valid() const{ return !(!_volume.status()); }
    inline const Bifrost::Processing::Volume& volume() const{ return _volume; }
    inline const Sampler& sampler() const{ return _sampler; }
    inline bool hasMotion() const{ return _hasMotion; }
private:
    Bifrost::Processing::Volume _volume;
    Sampler _sampler;
    bool _hasMotion;
};

