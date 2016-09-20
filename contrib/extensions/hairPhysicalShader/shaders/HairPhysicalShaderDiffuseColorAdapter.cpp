#include <ai.h>

AI_SHADER_NODE_EXPORT_METHODS(HairPhysicalShaderDiffuseColorAdapterMtd);

enum HairPhysicalShaderDiffuseColorAdapterParams
{
    p_colorD = 0,
    p_intensityD
};

node_parameters
{
    AiParameterRGB("colorD", 0.207f, 0.138f, 0.069f);
    AiParameterFLT("intensityD", 1.0f);

    AiMetaDataSetBool(mds, NULL, "maya.hide", true);
}

node_initialize
{
}

node_update
{
}

node_finish
{
}

shader_evaluate
{
    // Diffuse Component
    //      hairPhysicalShader: rootColorD, tipColorD, intensityD
    //      aiHair            : rootcolor, tipcolor, ambdiff
    // ambdiff is the blend factor of isotropic diffuse and Kajiya-Kay diffuse.
    // hairPhysicalShader is fully Kajiya-Kay diffuse (*). We set ambdiff to
    // 1.0f. intensityD is multiplied to rootColorD and tipColorD instead.
    //
    // * hairPhysicalShader is Lambert diffuse when the comment is written.
    //   But it's expected to change soon.
    // 
    // Fact: When we set intensityD to 0.0f, a semi-sphere should be black.
    //
    const AtRGB colorD     = AiShaderEvalParamRGB(p_colorD);
    const float intensityD = AiShaderEvalParamFlt(p_intensityD);
    sg->out.RGB = colorD * intensityD;
}
