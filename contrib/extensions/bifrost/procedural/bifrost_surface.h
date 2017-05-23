#include "bifrost_shape.h"
#include <bifrostprocessing/bifrostprocessing_surface.h>
#include <ai_params.h>
#include <ai_nodes.h>

class Surface : public Bifrost::Processing::Surface{
public:
    Surface();
    Surface(const AtNode* node);
    virtual Bifrost::Processing::Status initialize() override;
    virtual Bifrost::API::String str() const override;
    void declare(AtList* params, AtNodeEntry* nentry);
};
