#pragma once
#include <bifrostprocessing/bifrostprocessing_shape.h>

namespace Bifrost{
namespace Processing{

struct VolumeParameters : public ShapeParameters{
    Bifrost::API::String density_channel = "density";

    VolumeParameters() : ShapeParameters() {}
    virtual Bifrost::API::String str() const;
};

class Volume : public Shape {
public:
    Volume(const VolumeParameters& params);
};


}} // Bifrost::Processing
