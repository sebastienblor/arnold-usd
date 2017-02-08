/// Thin Film shader is provided by Chaos Group Ltd. and licensed
/// under the MIT License.
///
/// Author:  "Chaos Group Ltd."
/// Source:  "https://docs.chaosgroup.com/display/OSLShaders/Thin+Film+Shader"
/// License: "https://docs.chaosgroup.com/display/OSLShaders/License"
///
/// Ported by Peter Horvath, Solid Angle SL 2016

/*
Copyright (c) 2016 Chaos Software

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

/// The implementation is based on the article: Thin Film Interference for Computer Graphics
/// Source: http://www.gamedev.net/page/resources/_/technical/graphics-programming-and-theory/thin-film-interference-for-computer-graphics-r2962
/// Licensed under the GameDev.net Open License (GDOL): http://www.gamedev.net/page/resources/_/gdnethelp/gamedevnet-open-license-r2956

#include <ai.h>

#ifndef SHADER_PREFIX
#define SHADER_PREFIX ""
#endif

AI_SHADER_NODE_EXPORT_METHODS(ThinFilmMethods);

namespace {

/* Amplitude reflection coefficient (s-polarized) */
inline float rs(float n1, float n2, float cosI, float cosT) 
{
    return (n1 * cosI - n2 * cosT) / (n1 * cosI + n2 * cosT);
}

/* Amplitude reflection coefficient (p-polarized) */
inline float rp(float n1, float n2, float cosI, float cosT) 
{
    return (n2 * cosI - n1 * cosT) / (n1 * cosT + n2 * cosI);
}

/* Amplitude transmission coefficient (s-polarized) */
inline float ts(float n1, float n2, float cosI, float cosT) 
{
    return 2 * n1 * cosI / (n1 * cosI + n2 * cosT);
}

/* Amplitude transmission coefficient (p-polarized) */
inline float tp(float n1, float n2, float cosI, float cosT) 
{
    return 2 * n1 * cosI / (n1 * cosT + n2 * cosI);
}

// cosI is the cosine of the incident angle, that is, cos0 = dot(view angle, normal)
// lambda is the wavelength of the incident light (e.g. lambda = 510 for green)
inline float thinFilmReflectance(float cos0, float lambda, float thickness, float n0, float n1, float n2) 
{
    // compute the phase change term (constant)
    float d10 = (n1 > n0) ? 0.0f : AI_PI;
    float d12 = (n1 > n2) ? 0.0f : AI_PI;
    float delta = d10 + d12;
    
    // now, compute cos1, the cosine of the reflected angle
    float sin1 = AiSqr(n0 / n1) * (1.0f - AiSqr(cos0));
    if (sin1 > 1.0f) return 1.0f; // total internal reflection
    float cos1 = sqrtf(1.0f - sin1);
    
    // compute cos2, the cosine of the final transmitted angle, i.e. cos(theta_2)
    // we need this angle for the Fresnel terms at the bottom interface
    float sin2 = AiSqr(n0 / n2) * (1.0f - AiSqr(cos0));
    if (sin2 > 1.0f) return 1.0f; // total internal reflection
    float cos2 = sqrtf(1.0f - sin2);
    
    // get the reflection transmission amplitude Fresnel coefficients
    float alpha_s = rs(n1, n0, cos1, cos0) * rs(n1, n2, cos1, cos2); // rho_10 * rho_12 (s-polarized)
    float alpha_p = rp(n1, n0, cos1, cos0) * rp(n1, n2, cos1, cos2); // rho_10 * rho_12 (p-polarized)
    
    float beta_s = ts(n0, n1, cos0, cos1) * ts(n1, n2, cos1, cos2); // tau_01 * tau_12 (s-polarized)
    float beta_p = tp(n0, n1, cos0, cos1) * tp(n1, n2, cos1, cos2); // tau_01 * tau_12 (p-polarized)
        
    // compute the phase term (phi)
    float phi = (2.0f * AI_PI / lambda) * (2.0f * n1 * thickness * cos1) + delta;
        
    // finally, evaluate the transmitted intensity for the two possible polarizations
    float ts = AiSqr(beta_s) / (AiSqr(alpha_s) - 2.0f * alpha_s * cosf(phi) + 1.0f);
    float tp = AiSqr(beta_p) / (AiSqr(alpha_p) - 2.0f * alpha_p * cosf(phi) + 1.0f);
    
    // we need to take into account conservation of energy for transmission
    float beam_ratio = (n2 * cos2) / (n0 * cos0);
    
    // calculate the average transmitted intensity (if you know the polarization distribution of your
    // light source, you should specify it here. if you don't, a 50%/50% average is generally used)
    float t = beam_ratio * (ts + tp) / 2.0f;
    
    // and finally, derive the reflected intensity
    return 1.0f - t;
}

enum Params
{
    p_thickness_min = 0,
    p_thickness_max,
    p_thickness,
    p_ior_medium,
    p_ior_film,
    p_ior_internal
};

struct ShaderData
{
    float thickness_min;
    float thickness_max;
    float ior_medium;
    float ior_film;
    float ior_internal;
};

};

node_parameters
{
    AiParameterFlt("thickness_min", 250.0f);
    AiParameterFlt("thickness_max", 400.0f);
    AiParameterFlt("thickness", 0.0f);
    AiParameterFlt("ior_medium", 1.0f);
    AiParameterFlt("ior_film", 1.5f);
    AiParameterFlt("ior_internal", 1.0f);

    AiMetaDataSetBool(nentry, "thickness_min", "linkable", false);
    AiMetaDataSetBool(nentry, "thickness_max", "linkable", false);
    AiMetaDataSetBool(nentry, "ior_medium", "linkable", false);
    AiMetaDataSetBool(nentry, "ior_film", "linkable", false);
    AiMetaDataSetBool(nentry, "ior_internal", "linkable", false);
}

node_initialize
{
    ShaderData *data = new ShaderData();
    AiNodeSetLocalData(node, data);
}

node_update
{
    ShaderData *data = reinterpret_cast<ShaderData*>(AiNodeGetLocalData(node));

    static const AtString thickness_min("thickness_min");
    data->thickness_min = AiNodeGetFlt(node, thickness_min);
    static const AtString thickness_max("thickness_max");
    data->thickness_max = AiNodeGetFlt(node, thickness_max);
    static const AtString ior_medium("ior_medium");
    data->ior_medium = AiNodeGetFlt(node, ior_medium);
    static const AtString ior_film("ior_film");
    data->ior_film = AiNodeGetFlt(node, ior_film);
    static const AtString ior_internal("ior_internal");
    data->ior_internal = AiNodeGetFlt(node, ior_internal);
}

node_finish
{
    ShaderData *data = reinterpret_cast<ShaderData*>(AiNodeGetLocalData(node));
    delete data;
}

shader_evaluate
{
    ShaderData *data = reinterpret_cast<ShaderData*>(AiNodeGetLocalData(node));

    float thickness = AiShaderEvalParamFlt(p_thickness);

    float cos0 = std::abs(AiV3Dot(sg->Rd, sg->N));
    float thick = data->thickness_min * (1.0f - thickness) + data->thickness_max * thickness;
    sg->out.RGB().r = thinFilmReflectance(cos0, 650.0f, thick, data->ior_medium, data->ior_film, data->ior_internal);
    sg->out.RGB().g = thinFilmReflectance(cos0, 510.0f, thick, data->ior_medium, data->ior_film, data->ior_internal);
    sg->out.RGB().b = thinFilmReflectance(cos0, 475.0f, thick, data->ior_medium, data->ior_film, data->ior_internal);
}
