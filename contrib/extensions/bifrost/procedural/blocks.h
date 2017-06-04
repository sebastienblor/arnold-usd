#pragma once

#include "surface.h"
#include <vector>

struct BlocksParameters : public SurfaceParameters{
    BlocksParameters();
    BlocksParameters(const AtNode* node);
    void declare(AtList *params, AtNodeEntry *nentry);

    std::vector<AtNode*>* nodes() const;
};
