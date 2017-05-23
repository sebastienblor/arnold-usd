#pragma once

#include "bifrostprocessing_shape.h"

namespace Bifrost{
namespace Processing{

class Volume : public Shape{
public:
    Bifrost::API::String density_channel = "smoke";

    virtual Status initialize() override;
    virtual Bifrost::API::String str() const override;
};

}} // Bifrost::Processing
