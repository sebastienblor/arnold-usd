#include <ai.h>

#include <cstdio>

extern const AtNodeMethods* HairPhysicalShaderDiffuseColorAdapterMtd;
extern const AtNodeMethods* HairPhysicalShaderSpecularWeightAdapterMtd;
extern const AtNodeMethods* HairPhysicalShaderSpecularGlossAdapterMtd;
extern const AtNodeMethods* HairPhysicalShaderTransmissionWeightAdapterMtd;
extern const AtNodeMethods* HairPhysicalShaderTransmissionSpreadAdapterMtd;

enum {
    SHADER_HAIRPHYSICALSHADER_DIFFUSECOLORADAPTER = 0,
    SHADER_HAIRPHYSICALSHADER_SPECULARWEIGHTADAPTER,
    SHADER_HAIRPHYSICALSHADER_SPECULARGLOSSADAPTER,
    SHADER_HAIRPHYSICALSHADER_TRANSMISSIONWEIGHTADAPTER,
    SHADER_HAIRPHYSICALSHADER_TRANSMISSIONSPREADADAPTER
};

node_loader
{
    switch (i)
    {
    case SHADER_HAIRPHYSICALSHADER_DIFFUSECOLORADAPTER:
        node->methods      = HairPhysicalShaderDiffuseColorAdapterMtd;
        node->output_type  = AI_TYPE_RGB;
        node->name         = "HairPhysicalShaderDiffuseColorAdapter";
        node->node_type    = AI_NODE_SHADER;
        break;

    case SHADER_HAIRPHYSICALSHADER_SPECULARWEIGHTADAPTER:
        node->methods      = HairPhysicalShaderSpecularWeightAdapterMtd;
        node->output_type  = AI_TYPE_FLOAT;
        node->name         = "HairPhysicalShaderSpecularWeightAdapter";
        node->node_type    = AI_NODE_SHADER;
        break;

    case SHADER_HAIRPHYSICALSHADER_SPECULARGLOSSADAPTER:
        node->methods      = HairPhysicalShaderSpecularGlossAdapterMtd;
        node->output_type  = AI_TYPE_FLOAT;
        node->name         = "HairPhysicalShaderSpecularGlossAdapter";
        node->node_type    = AI_NODE_SHADER;
        break;

    case SHADER_HAIRPHYSICALSHADER_TRANSMISSIONWEIGHTADAPTER:
        node->methods      = HairPhysicalShaderTransmissionWeightAdapterMtd;
        node->output_type  = AI_TYPE_FLOAT;
        node->name         = "HairPhysicalShaderTransmissionWeightAdapter";
        node->node_type    = AI_NODE_SHADER;
        break;

    case SHADER_HAIRPHYSICALSHADER_TRANSMISSIONSPREADADAPTER:
        node->methods      = HairPhysicalShaderTransmissionSpreadAdapterMtd;
        node->output_type  = AI_TYPE_FLOAT;
        node->name         = "HairPhysicalShaderTransmissionSpreadAdapter";
        node->node_type    = AI_NODE_SHADER;
        break;

    default:
        return false;
    }

    sprintf(node->version, AI_VERSION);
    return true;
}
