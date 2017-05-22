#include <bifrostapi/bifrost_voxelcomponent.h>
#include <bifrostprocessing/bifrostprocessing_surface.h>
#include <ai_array.h>
#include <ai_nodes.h>

class Surface : public Bifrost::Processing::Surface{
public:
    Surface();
    Surface(const AtNode* node);
    virtual Bifrost::API::String str() const override;
    void declare(AtList* params, AtNodeEntry* nentry);
};

class Polymesh : public Surface{
public:
    unsigned int subdivisions = 1;
    bool smoothing = true;

    AtArray* disp_map = nullptr;
    float disp_padding = 0;
    float disp_height = 0;
    float disp_zero_value = 0;
    bool disp_autobump = true;

    Polymesh();
    Polymesh(const AtNode* node);
    virtual Bifrost::API::String str() const override;
    void declare(AtList *params, AtNodeEntry *nentry);
};
