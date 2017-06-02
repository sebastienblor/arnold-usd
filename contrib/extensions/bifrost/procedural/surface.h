#include "shape.h"
#include <bifrostprocessing/bifrostprocessing_surface.h>

struct SurfaceParameters : public Bifrost::Processing::SurfaceParameters {
    bool ignore_motion_blur = false;
    SurfaceParameters();
    SurfaceParameters(const AtNode* node);
    virtual Bifrost::API::String str() const override;
    void declare(AtList* params, AtNodeEntry* nentry);
};
