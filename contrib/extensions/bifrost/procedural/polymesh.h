#include "surface.h"

struct PolymeshParameters : public SurfaceParameters{
    unsigned int subdivisions = 1;
    bool smoothing = true;

    AtArray* disp_map = nullptr;
    float disp_padding = 0;
    float disp_height = 0;
    float disp_zero_value = 0;
    bool disp_autobump = true;

    PolymeshParameters();
    PolymeshParameters(const AtNode* node);
    Bifrost::API::String str() const override;
    void declare(AtList* params, AtNodeEntry* nentry);

    AtNode* node() const;
};
