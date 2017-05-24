#pragma once

#include "bifrostprocessing_shape.h"

namespace Bifrost{
namespace Processing{

struct VolumeParameters : public ShapeParameters {
    Bifrost::API::String density_channel = "smoke";
    virtual Bifrost::API::String str() const override;
};

class Volume : public Shape{
public:
    Volume(const VolumeParameters& params);
};

}} // Bifrost::Processing
