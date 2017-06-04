#pragma once

#include "surface.h"
#include <ai_bbox.h>
#include "sampler.h"
#include <bifrostprocessing/bifrostprocessing_intersector.h>

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
    inline const Bifrost::Processing::Intersector& intersector() const{ return _intersector; }
    inline float padding() const{ return _padding; }
    inline bool hasMotion() const{ return _hasMotion; }
    int depth(const AtVector& p);
private:
    Bifrost::Processing::Surface _surface;
    Sampler _sampler;
    Bifrost::Processing::Intersector _intersector;
    float _padding;
    bool _hasMotion;
    Bifrost::API::TileAccessor _accessor;
    float invDx;
    int maxDepth;
};

