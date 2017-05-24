#pragma once

#include "shape.h"
#include <bifrostprocessing/bifrostprocessing_points.h>
#include <vector>

struct PointsParameters : public Bifrost::Processing::PointsParameters{
    float step_size = .025;
    unsigned int chunk_size = 100000;
    enum Mode { Disk=0, Sphere=1 } mode = Sphere;

    PointsParameters();
    PointsParameters(const AtNode* node);
    virtual Bifrost::API::String str() const override;
    void declare(AtList* params, AtNodeEntry* nentry);

    std::vector<AtNode*>* nodes() const;
};
