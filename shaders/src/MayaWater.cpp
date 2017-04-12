#include <ai.h>

#ifdef _MSC_VER
#define _USE_MATH_DEFINES
#endif
#include <cmath>

#include "MayaUtils.h"
#include "MayaNoiseUtils.h"

AI_SHADER_NODE_EXPORT_METHODS(MayaWaterMtd);

namespace
{
    enum MayaWaterParams
    {
        p_numWaves = 0,
        p_waveTime,
        p_waveVelocity,
        p_waveAmplitude,

        p_waveFrequency,
        p_subWaveFrequency,
        p_smoothness,
        p_windUV,

        p_rippleTime,
        p_rippleFrequency,
        p_rippleAmplitude,
        p_dropSize,
        p_rippleOrigin,

        p_groupVelocity,
        p_phaseVelocity,
        p_spreadRate,
        p_spreadStart,

        p_reflectionBox,
        p_boxMin,
        p_boxMax,

        p_uvCoord,
        MAYA_COLOR_BALANCE_ENUM
    };

}
node_parameters
{
   AtMatrix id = AiM4Identity();

   AiParameterInt("numWaves", 8);
   AiParameterFlt("waveTime", 0.0f);
   AiParameterFlt("waveVelocity", 1.0f);
   AiParameterFlt("waveAmplitude", 0.05f);

   AiParameterFlt("waveFrequency", 4.0f);
   AiParameterFlt("subWaveFrequency", 0.125f);
   AiParameterFlt("smoothness", 2.0f);
   AiParameterVec2("windUV", 1.0f, 0.0f);

   AiParameterFlt("rippleTime", 0.0f);
   AiParameterFlt("rippleFrequency", 25.0f);
   AiParameterFlt("rippleAmplitude", 0.05f);
   AiParameterFlt("dropSize", 0.3f);
   AiParameterVec2("rippleOrigin", 0.5f, 0.5f);

   AiParameterFlt("groupVelocity", 1.0f);
   AiParameterFlt("phaseVelocity", 2.5f);
   AiParameterFlt("spreadRate", 0.3f);
   AiParameterFlt("spreadStart", 0.005f);


   AiParameterBool("reflectionBox", false);
   AiParameterVec2("boxMin", 0.0f, 0.0f);
   AiParameterVec2("boxMax", 1.0f, 1.0f);

   AiParameterVec2("uvCoord", 0.0f, 0.0f);

   AddMayaColorBalanceParams(params, nentry);

   AiMetaDataSetStr(nentry, NULL, "maya.name", "water");
   AiMetaDataSetInt(nentry, NULL, "maya.id", 0x52544D52);
   AiMetaDataSetStr(nentry, NULL, "_synonym", "mayaWater");

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
    AtVector2 uv = AiShaderEvalParamVec2(p_uvCoord);
    // Will be set to GLOBALS by update if unconnected
    if (uv.x == UV_GLOBALS) uv.x = sg->u;
    if (uv.y == UV_GLOBALS) uv.y = sg->v;

    if (!IsValidUV(uv))
    {
       MayaDefaultColor(sg, node, p_defaultColor, sg->out.RGBA());
       return;
    }

    float rippleTime = AiShaderEvalParamFlt(p_rippleTime);
    float rippleWaveNum = AiShaderEvalParamFlt(p_rippleFrequency);
    float groupVelocity = AiShaderEvalParamFlt(p_groupVelocity);
    float ripplePhaseVel = AiShaderEvalParamFlt(p_phaseVelocity);
    float spreadRate = AiShaderEvalParamFlt(p_spreadRate);
    float spreadOrigin = AiShaderEvalParamFlt(p_spreadStart);
    float rippleAmplitude = AiShaderEvalParamFlt(p_rippleAmplitude);
    float dropSize = AiShaderEvalParamFlt(p_dropSize);
    int numWaves = AiShaderEvalParamInt(p_numWaves);
    bool reflections = AiShaderEvalParamBool(p_reflectionBox);

    //Tboolean isLum = block.value(attrAlphaIsLuminance()).asBoolean();
    AtVector2 boxMin = AiShaderEvalParamVec2(p_boxMin);
    AtVector2 boxMax = AiShaderEvalParamVec2(p_boxMax);
    AtVector2 origin = AiShaderEvalParamVec2(p_rippleOrigin);

    float value = 0.5f;		// Water elevation to be returned

    // Compute the contribution from the concentric  water ripples
    //
    if (rippleTime > 0.0f)
    {
        // ripple precalculations
        float	sp = spreadRate * rippleTime;
        sp = sp * sp;
        sp += spreadOrigin * spreadOrigin;

        float initialSpread = (sp < 1e-20f) ? 1e-20f : sp;

        sp = spreadOrigin * rippleAmplitude / sqrtf(initialSpread);
        float arg = rippleTime * groupVelocity;
        arg = (arg < 1e-12f) ? 1e-12f : arg;
        sp *= dropSize / (dropSize + sqrtf(arg));

        float envelopeScale = sp;
        float envelope;		    // Wave packet shape
        float rad;				// Increments and distances in uv space
        float tu, tv;			// Temporary parameter values
        float spread, spread2;	// Spread of Gaussian envelope
        int umir, vmir;		    // Mirror looping variables
        float du, dv;

        // Compute the direct water ripple
        //
        du = uv[0] - origin[0];
        dv = uv[1] - origin[1];

        rad = sqrtf(du * du + dv * dv);
        arg = rad - rippleTime * groupVelocity;

        // Added to avoid a floating point exception
        //
        envelope = -(arg * arg) / initialSpread;
        if (envelope < -10.0f) envelope = 0.0f;
        else envelope = envelopeScale * expf(envelope);

        arg = AI_PITIMES2 * (rad * rippleWaveNum - rippleTime * ripplePhaseVel);
        value -= envelope * cosf(arg);

        if (reflections)
        {
            for (umir = -1; umir <= 1; umir = umir + 2)
            {
                if (umir > 0)	tu = boxMax[0] + (boxMax[0] - origin[0]);
                else			tu = boxMin[0] + (boxMin[0] - origin[0]);

                du = uv[0] - tu;
                for (vmir = -1; vmir <= 1; vmir = vmir + 2)
                {
                    if (vmir > 0)	tv = boxMax[1] + (boxMax[1] - origin[1]);
                    else			tv = boxMin[1] + (boxMin[1] - origin[1]);

                    dv = uv[1] - tv;
                    rad = sqrtf(du * du + dv * dv);
                    arg = rad - rippleTime * groupVelocity;

                    spread = spreadRate * rippleTime;
                    spread2 = spread * spread;
                    spread2 += spreadOrigin * spreadOrigin;
                    spread2 = (spread2 < AI_EPSILON) ?
                        AI_EPSILON : spread2;
                    spread = sqrtf(spread2);
                    envelope = (-arg * arg / spread2);

                    if (envelope < -10.0f)	envelope = 0.0f;
                    else envelope = rippleAmplitude * expf(envelope);

                    envelope *= spreadOrigin / spread;
                    arg = rippleTime * groupVelocity;
                    arg = (arg < AI_EPSILON) ? AI_EPSILON : arg;
                    envelope *= dropSize / (dropSize + sqrtf(arg));
                    arg = AI_PITIMES2 * (rad * rippleWaveNum -
                        rippleTime * ripplePhaseVel);
                    value -= envelope * cosf(arg);
                }
            }
        } // if reflections
    } // if rippleTime

    // Compute the contribution from the linear water waves
    // Loop for each wave to evaluate the effect of the planar waves
    //
    if (numWaves < 0) numWaves = 0;

    AtVector2 wind = AiShaderEvalParamVec2(p_windUV);
    float windNorm = sqrtf(wind[0] * wind[0] + wind[1] * wind[1]);
    if (windNorm < 0.000001f)
        windNorm = 0.000001f;
    wind[0] /= windNorm;
    wind[1] /= windNorm;

    float windWards;       // Wave component towards wind
    float sideWards;       // Wave component perpendicular to wind
    float sum;             // Sum for components 
    float wnm;             // Wave wavenumber
    float ri;              // Randomized index for wave generation
    float frequency;       // Intermediate frequency variable 
    float numberU, numberV;
    float angFreq;
    float amplitude;
    float arg;

    float	waveNumber = AiShaderEvalParamFlt(p_waveFrequency);
    float	waveAmplitude = AiShaderEvalParamFlt(p_waveAmplitude);
    float	waveVelocity = AiShaderEvalParamFlt(p_waveVelocity);
    float	waveTime = AiShaderEvalParamFlt(p_waveTime);
    float	subWaveNumber = AiShaderEvalParamFlt(p_subWaveFrequency);
    float	smoothness = AiShaderEvalParamFlt(p_smoothness);

    // local storage for re-entrant random num generation (48 bit field)
    unsigned short       fPreviousIteration[3];

    // to be compatible with the old srand (1) initialization
    fPreviousIteration[0] = 0x330E;
    fPreviousIteration[1] = 1;
    fPreviousIteration[2] = 0;

    // create amplitudes & wave numbers
    for (int i = 0; i < numWaves; i++)
    {
        ri = ((float)i + 0.5f * (float)MayaNoiseUtils::erand48_r(fPreviousIteration)) * subWaveNumber;
        frequency = powf(2.0f, ri);
        windWards = 0.75f + 0.25f * (float)MayaNoiseUtils::erand48_r(fPreviousIteration);
        sideWards = (float)MayaNoiseUtils::erand48_r(fPreviousIteration) - 0.5f;
        sum = sqrtf(windWards * windWards + sideWards * sideWards) + 0.001f;
        wnm = (frequency * waveNumber + 0.0000001f) * AI_PITIMES2;

        numberU = (wind[0] * windWards - wind[1] * sideWards) * wnm / sum;
        numberV = (wind[1] * windWards + wind[0] * sideWards) * wnm / sum;
        angFreq = waveVelocity * sqrtf(frequency) * wnm;
        angFreq *= waveTime;
        sum = windWards / sum;
        sum = (sum < 0.0f) ? -sum : sum;
        amplitude = sum * waveAmplitude / powf(frequency, smoothness);

        arg = uv[0] * numberU + uv[1] * numberV - angFreq;
        value -= amplitude * cosf(arg);
    }

    // Set the output color
    //
    AtRGB outColor;
    outColor.r = value;
    outColor.g = value;
    outColor.b = value;

    sg->out.RGBA() = AtRGBA(outColor);
    MayaColorBalance(sg, node, p_defaultColor, sg->out.RGBA());
}
