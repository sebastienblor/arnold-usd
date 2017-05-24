#include "shape.h"
#include <bifrostprocessing/bifrostprocessing_volume.h>
#include "sampler.h"

class Volume;

struct VolumeParameters : public Bifrost::Processing::VolumeParameters{
    VolumeParameters();
    VolumeParameters(const AtNode* node);

    virtual Bifrost::API::String str() const override;
    void declare(AtList* params, AtNodeEntry* nentry);

    Volume* volume() const;
};

class Volume : public Bifrost::Processing::Volume {
public:
    Volume(const VolumeParameters& params);
    bool sample(const AtString& channel, const AtVector& pos, uint16_t tid, AtParamValue *value, uint8_t *type, int interp);

public:
    std::vector<ComponentSampler> samplers;
};
