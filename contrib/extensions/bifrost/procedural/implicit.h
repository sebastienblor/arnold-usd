#pragma once

#include "surface.h"
#include <ai_bbox.h>
#include "sampler.h"

class Implicit;

struct ImplicitParameters : public SurfaceParameters{
    ImplicitParameters() : SurfaceParameters(){}
    ImplicitParameters(const AtNode* node) : SurfaceParameters(node){}

    Implicit* implicit() const;
};

class Implicit{
public:
    Implicit(const ImplicitParameters& params);
    inline bool valid() const{ return !(!_surface.status()); }
    inline const Bifrost::Processing::Surface& surface() const{ return _surface; }
    inline const Sampler& sampler() const{ return _sampler; }
private:
    Bifrost::Processing::Surface _surface;
    Sampler _sampler;
};

