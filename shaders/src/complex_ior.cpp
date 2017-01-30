/// Complex Fresnel shader is provided by Chaos Group Ltd. and licensed
/// under the MIT License.
///
/// Author:  "Chaos Group Ltd."
/// Source:  "https://docs.chaosgroup.com/display/OSLShaders/Complex+Fresnel+shader"
/// License: "https://docs.chaosgroup.com/display/OSLShaders/License"
///
/// Ported by Peter Horvath, Solid Angle SL 2016

/*
Copyright (c) 2016 Chaos Software

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

/// The artistic mode is based on the paper Artist Friendly Metallic Fresnel by Ole Gulbrandsen (Framestore)
/// http://jcgt.org/published/0003/04/03/

/*
Copyright (c) 2014, Ole Gulbrandsen
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.
    * Neither the name of the <organization> nor the
      names of its contributors may be used to endorse or promote products
      derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL <COPYRIGHT HOLDER> BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE. 
*/

#include <ai.h>

#ifndef SHADER_PREFIX
#define SHADER_PREFIX ""
#endif

AI_SHADER_NODE_EXPORT_METHODS(ComplexIORMethods);

namespace {

enum Params
{
    p_preset = 0,
    p_mode,
    p_reflectivity,
    p_edgetint,
    p_n,
    p_k,
};

static const char* enum_material[] =
{
    "custom",
    "aluminium",
    "copper",
    "gold",
    "iron",
    "lead",
    "magnesium",
    "mercury",
    "nickel",
    "platinum",
    "silver",
    "sodium",
    NULL
};

enum Material
{
    MATERIAL_CUSTOM = 0,
    MATERIAL_ALUMINIUM,
    MATERIAL_COPPER,
    MATERIAL_GOLD,
    MATERIAL_IRON,
    MATERIAL_LEAD,
    MATERIAL_MAGNESIUM,
    MATERIAL_MERCURY,
    MATERIAL_NICKEL,
    MATERIAL_PLATINUM,
    MATERIAL_SILVER,
    MATERIAL_SODIUM,
};

static const char* enum_mode[] =
{
    "artistic",
    "physical",
    NULL
};

enum Mode
{
    MODE_ARTISTIC = 0,
    MODE_PHYSICAL,
};

struct ShaderData
{
    int mode;
    bool is_preset;
    AtVector n;
    AtVector k;
};

struct Preset
{
    AtVector n;
    AtVector k;

    Preset(float nx, float ny, float nz, 
           float kx, float ky, float kz)
    {
        n.x = nx;
        n.y = ny;
        n.z = nz;

        k.x = kx;
        k.y = ky;
        k.z = kz;
    }
};

// Presets are taken from http://refractiveindex.info
// for the red/green/blue wavelengths (f.e. 0.65, 0.55 and 0.45 micrometers).
static const Preset presets[] = {
    Preset(1.3424f, 0.96526f, 0.61692f,
           7.4737f, 6.3996f, 5.3036f),    // aluminium (Al)
    Preset(0.27105f, 0.67693f, 1.3164f,
           3.6092f, 2.6247f, 2.2921f),    // copper (Cu)
    Preset(0.18309f, 0.42203f, 1.3730f,
           3.4241f, 2.3461f, 1.7706f),    // gold (Au)
    Preset(2.9114f, 2.9497f, 2.5845f,
           3.0893f, 2.9318f, 2.7670f),    // iron (Fe)
    Preset(1.91f, 1.83f, 1.44f,
           3.51f, 3.4f, 3.18f),           // lead (Pb)
    Preset(0.44156f, 0.31501f, 0.21917f,
           6.0633f, 5.0363f, 4.0295f),    // magnesium (Mg)
    Preset(2.0733f, 1.5523f, 1.0606f,
           5.3383f, 4.6510f, 3.8628f),    // mercury (Hg)
    Preset(2.0592f, 1.8179f, 1.6210f,
           3.7574f, 3.2740f, 2.6932f),    // nickel (Ni)
    Preset(2.3757f, 2.0847f, 1.8453f,
           4.2654f, 3.7153f, 3.1365f),    // platinum (Pt)
    Preset(0.15943f, 0.14512f, 0.13547f,
           3.9290f, 3.19f, 2.3809f),     // silver (Ag)
    Preset(0.042564f, 0.051964f, 0.077088f,
           2.7655f, 2.2872f, 1.7494f),    // sodium (Na)
};

inline float n_min(float r)
{
    return (1.0f - r) / (1.0f + r);
}

inline float n_max(float r)
{
    return (1.0f + sqrtf(r)) / (1.0f - sqrtf(r)); 
}

inline float get_n(float r, float g)
{
    return n_min(r)*g + (1.0f-g)*n_max(r);
}

inline float get_k2(float r, float n)
{
    float nr = (n+1.0f)*(n+1.0f)*r - (n-1.0f)*(n-1.0f);
    return nr / (1.0f-r);
}

inline float get_r(float n, float k)
{
    return ((n-1.0f)*(n-1.0f) + k*k) / ((n+1.0f)*(n+1.0f) + k*k);
}

inline float get_g(float n, float k)
{
    float r = get_r(n,k);
    return (n_max(r) - n) / (n_max(r) - n_min(r));
}

inline float fresnel(float n, float k2, float c) 
{
    float c2 = c * c;
    float n2k2 = n * n + k2;
    float f1 = n2k2 + c2;
    float f2 = 2.0f * n * c;
    float f3 = n2k2 * c2 + 1.0f;

    // rs_num = n*n + k*k - 2*n*c + c*c
    float rs_num = f1 - f2;
    // rs_den = n*n + k*k + 2*n*c + c*c
    float rs_den = f1 + f2;
    float rs = rs_num / rs_den;
	
    // rp_num = (n*n + k*k)*c*c - 2*n*c + 1;
    float rp_num = f3 - f2;
    // rp_den = (n*n + k*k)*c*c + 2*n*c + 1;
    float rp_den = f3 + f2;
    float rp = rp_num / rp_den;
	
    return CLAMP(0.5f*(rs+rp), 0.0f, 1.0f);
}

};

node_parameters
{
    AiParameterEnum("material", MATERIAL_CUSTOM, enum_material);
    AiParameterEnum("mode", MODE_ARTISTIC, enum_mode);
    AiParameterRGB("reflectivity", 0.925952f, 0.720887f, 0.504154f);
    AiParameterRGB("edgetint", 0.995524f, 0.957415f, 0.822776f);
    AiParameterVec("eta", 0.27105f, 0.67693f, 1.3164f);
    AiParameterVec("k", 3.6092f, 2.6247f, 2.2921f);

    AiMetaDataSetBool(mds, "material", "linkable", false);
    AiMetaDataSetBool(mds, "mode", "linkable", false);
    AiMetaDataSetBool(mds, "eta", "linkable", false);
    AiMetaDataSetBool(mds, "k", "linkable", false);
    AiMetaDataSetBool(mds, "reflectivity", "always_linear", true);
    AiMetaDataSetBool(mds, "edgetint", "always_linear", true);
}

node_initialize
{
    ShaderData *data = new ShaderData();
    AiNodeSetLocalData(node, data);
}

node_update
{
    ShaderData *data = reinterpret_cast<ShaderData*>(AiNodeGetLocalData(node));

    static const AtString mode("mode");
    data->mode = AiNodeGetInt(node, mode);

    static const AtString material("material");
    int material_index = AiNodeGetInt(node, material);
    data->is_preset = material_index != MATERIAL_CUSTOM;
    if (data->is_preset)
    {
        const Preset& preset = presets[material_index - 1];
        data->n = preset.n;
        data->k = preset.k;
    }
    else
    {
        static const AtString n("eta");
        data->n = AiNodeGetVec(node, n);
        static const AtString k("k");
        data->k = AiNodeGetVec(node, k);
    }
}

node_finish
{
    ShaderData *data = reinterpret_cast<ShaderData*>(AiNodeGetLocalData(node));
    delete data;
}

shader_evaluate
{
    ShaderData *data = reinterpret_cast<ShaderData*>(AiNodeGetLocalData(node));

    // define n (refractive index) and k (extinction coefficient) values
    AtVector n, k2;
    if (!data->is_preset && data->mode == MODE_ARTISTIC)
    {
        const AtRGB& r = AiShaderEvalParamRGB(p_reflectivity);
        const AtRGB& g = AiShaderEvalParamRGB(p_edgetint);
        n = AiVector(get_n(r.r, g.r), get_n(r.g, g.g), get_n(r.b, g.b));
        k2 = AiVector(get_k2(r.r, n.x), get_k2(r.g, n.y), get_k2(r.b, n.z));
    }
    else
    {
        n = data->n;
        k2 = data->k * data->k;
    }

    // calculate the fresnel effect
    float cos_theta = ABS(AiV3Dot(sg->Rd, sg->N));
    sg->out.RGB.r = fresnel(n.x, k2.x, cos_theta);
    sg->out.RGB.g = fresnel(n.y, k2.y, cos_theta);
    sg->out.RGB.b = fresnel(n.z, k2.z, cos_theta);
}


