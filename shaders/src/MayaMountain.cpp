#include <ai.h>

#ifdef _MSC_VER
#define _USE_MATH_DEFINES
#endif
#include <cmath>

#include "MayaUtils.h"
#include "MayaNoiseUtils.h"

AI_SHADER_NODE_EXPORT_METHODS(MayaMountainMtd);

namespace
{
    enum MayaMountainParams
    {
        p_snowColor = 0,
        p_rockColor,
        p_amplitude,
        p_snowRoughness,
        p_rockRoughness,
        p_boundary,
        p_snowAltitude,
        p_snowDroppff,
        p_snowSlope,
        p_depthMax,

        p_uvCoord,
        MAYA_COLOR_BALANCE_ENUM
    };
}
node_parameters
{
   AtMatrix id = AiM4Identity();

   AiParameterRGB("snowColor", 1.0f, 1.0f, 1.0f);
   AiParameterRGB("rockColor", 0.262f, 0.012f, 0.0f);
   AiParameterFlt("amplitude", 1.0f);
   AiParameterFlt("snowRoughness", 0.707f);
   AiParameterFlt("rockRoughness", 0.5f);
   AiParameterFlt("boundary", 1.0f);
   AiParameterFlt("snowAltitude", 0.5f);
   AiParameterFlt("snowDropoff", 2.0f);
   AiParameterFlt("snowSlope", 0.8f);
   AiParameterFlt("depthMax", 20.0f);

   AiParameterVec2("uvCoord", 0.0f, 0.0f);

   AddMayaColorBalanceParams(params, nentry);

   AiMetaDataSetStr(nentry, NULL, "maya.name", "mountain");
   AiMetaDataSetInt(nentry, NULL, "maya.id", 0x52544D52);
}

node_initialize
{
}

node_update
{
    // Unconnected render attributes (uvCoords, normalCamera, etc)
    // should use globals as following Maya's behavior
    if (!AiNodeGetLink(node, "uvCoord"))
    {
        AtVector2 uv = AI_P2_ZERO;
        if (!AiNodeGetLink(node, "uvCoord.x")) uv.x = UV_GLOBALS;
        if (!AiNodeGetLink(node, "uvCoord.y")) uv.y = UV_GLOBALS;
        AiNodeSetVec2(node, "uvCoord", uv.x, uv.y);
    }
}

node_finish
{
}

shader_evaluate
{
    AtVector2 P;
    P = AiShaderEvalParamVec2(p_uvCoord);
    // Will be set to GLOBALS by update if unconnected
    if (P.x == UV_GLOBALS) P.x = sg->u;
    if (P.y == UV_GLOBALS) P.y = sg->v;

    if (!IsValidUV(P))
    {
       MayaDefaultColor(sg, node, p_defaultColor, sg->out.RGBA());
       return;
    }

    {
      AtRGB snowColor = AiShaderEvalParamRGB(p_snowColor);
      AtRGB rockColor = AiShaderEvalParamRGB(p_rockColor);
      float amplitude0 = AiShaderEvalParamFlt(p_amplitude);
      float rockFractal = AiShaderEvalParamFlt(p_rockRoughness);
      float border = AiShaderEvalParamFlt(p_boundary);
      float alt = AiShaderEvalParamFlt(p_snowAltitude);
      float drop = AiShaderEvalParamFlt(p_snowDroppff);
      float slopeValue = AiShaderEvalParamFlt(p_snowSlope);
      float maxRange = AiShaderEvalParamFlt(p_depthMax);

      bool snowPresent;		// the snow flag
      float fx;					// Frequencies
      float ofx, ofy;			// Offsets
      float ah, ch, amph;		// Elevation for the snow calculation
      float c, amp;				// Cumulative values
      unsigned int loop;				// Recursion counter
      unsigned int pfloor;				// Recursion level
      float slopex, slopey;		// Cumulative slope of Procedural Texture
      float iterate, a, amplitude;
      float slope[3];
      AtRGB outColor;

        iterate =  maxRange;
        pfloor = (unsigned int)iterate;

        // Initialize the frequencies and values
        //
        fx = 1.0f;
        ofx = 0.0f;
        ofy = 0.0f;
        slopex = 0.0f;
        slopey = 0.0f;
        ch = 0.0f;

        amplitude = amplitude0;
        amp = amplitude;
        amph = amp;
        for (loop = 0; loop < pfloor; loop++)
        {
            MayaNoiseUtils::DNoise2((double)(P.x * fx + ofx), (double)(P.y * fx + ofy), slope);
            ofx += 0.1f;
            ofy += 0.03f;
            slopex += slope[0] * amp;
            slopey += slope[1] * amp;
            fx *= 2.0f;
            amp *= border;

            ch += MayaNoiseUtils::noise2((P.x * fx + ofx), (P.y * fx + ofy)) * amph;
            amph *= rockFractal;
        }
        amp *= (iterate - (float)pfloor);
        MayaNoiseUtils::DNoise2((double)(P.x * fx + ofx), (double)(P.y * fx + ofy), slope);
        slopex += (slope[0] * amp);
        slopey += (slope[1] * amp);
        amph *= (iterate - (float)pfloor);
        ch += MayaNoiseUtils::noise2((P.x * fx + ofx), (P.y * fx + ofy)) * amph;
        c = slopex * slopex + slopey * slopey;
        a = 0.5f * c;
        ah = alt * drop;
        ah = (ah < 0.0f) ? 0.0f : ah;
        if (a > (ah + slopeValue)) snowPresent = false;
        else snowPresent = true;

        // compute output color and store it into datablock
        //
        if (!snowPresent)
        {
            outColor = rockColor;
        }
        else
        {
            outColor = snowColor;
        }

      sg->out.RGBA() = AtRGBA(outColor);
      MayaColorBalance(sg, node, p_defaultColor, sg->out.RGBA());
   }
}
