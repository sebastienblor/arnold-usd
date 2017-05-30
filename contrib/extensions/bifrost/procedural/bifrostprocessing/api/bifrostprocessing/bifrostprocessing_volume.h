#pragma once
#include <bifrostprocessing/bifrostprocessing_shape.h>

namespace Bifrost{
namespace Processing{

struct VolumeParameters : public ShapeParameters{
    Bifrost::API::String density_channel = "density";
    float smooth = 0;
    unsigned int smooth_iterations = 1;

    VolumeParameters() : ShapeParameters() {}
    virtual Bifrost::API::String str() const;
};

class Volume : public Shape {
public:
    Volume(const VolumeParameters& params);
private:
    Bifrost::API::String density_channel;
};


}} // Bifrost::Processing
