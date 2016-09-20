#include <ai.h>

#ifdef _MSC_VER
#define _USE_MATH_DEFINES
#endif
#include <cmath>

#include "MayaUtils.h"

#define MAYA_CLAMP(x,a,b)	((x) < (a) ? (a) : (x) > (b) ? (b) : (x))

#define MAYA_CHEAP_SIN(r,x)	(r) = (x); \
				(r) = ((r) < 0.0f) ? \
					(1.0f + (r) - floorf((r))) : \
					((r) - floorf((r))); \
				if ((r) < 0.5f) { \
					(r) = ((r) < 0.25f) ? \
						(0.25f - (r)) : \
						((r) - 0.25f);  \
					(r) = 1.0f - 16.0f*(r)*(r); \
					} \
				else { \
					(r) = ((r) < 0.75f) ? \
						(0.75f - (r)) : \
						((r) - 0.75f);  \
					(r) = 1.0f - 16.0f*(r)*(r); \
					(r) = -(r); \
					}

#define ROCK_CYCLOID(r, x) \
	if (((r)=(x), ((r)=(r)-(int)(r)) < 0.0)) (r) += 0.5; else (r) -= 0.5; \
	(r) = -4.0*(r)*(r)+1.0;


AI_SHADER_NODE_EXPORT_METHODS(MayaRockMtd);

namespace
{
   enum MayaRockParams
   {
      p_color1 = 0,
      p_color2,
      p_grainSize,
      p_diffusion,
      p_mixRatio,
      
      p_wrap,
      p_local,
      p_placementMatrix,
      MAYA_COLOR_BALANCE_ENUM
   };
}

node_parameters
{
   AtMatrix id;
   AiM4Identity(id);

   AiParameterRGB("color1", 0.235f, 0.0f, 0.0f);
   AiParameterRGB("color2", 1.0f, 1.0f, 1.0f);
   AiParameterFLT("grainSize", 0.01f);
   AiParameterFLT("diffusion", 1.0f);
   AiParameterFLT("mixRatio", 0.5f);

   AiParameterBOOL("wrap", true);
   AiParameterBOOL("local", false);
   AiParameterMTX("placementMatrix", id);
   AddMayaColorBalanceParams(params, mds);

   AiMetaDataSetStr(mds, NULL, "maya.name", "rock");
   AiMetaDataSetInt(mds, NULL, "maya.id", 0x52544D52);
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
   AtMatrix *placementMatrix = AiShaderEvalParamMtx(p_placementMatrix);
   bool local = AiShaderEvalParamBool(p_local);
   bool wrap = AiShaderEvalParamBool(p_wrap);
   
   AtPoint P;

   AtPoint tmpPts;
   bool usePref = SetRefererencePoints(sg, tmpPts);

   AiM4PointByMatrixMult(&P, *placementMatrix, (local ? &(sg->Po) : &(sg->P)));
   
   if (wrap || ((-1.0f <= P.x && P.x <= 1.0f) &&
                (-1.0f <= P.y && P.y <= 1.0f) &&
                (-1.0f <= P.z && P.z <= 1.0f)))
   {
      AtRGB color1 = AiShaderEvalParamRGB(p_color1);
      AtRGB color2 = AiShaderEvalParamRGB(p_color2);
      float grainSize = AiShaderEvalParamFlt(p_grainSize);
      float diffusion = AiShaderEvalParamFlt(p_diffusion);
      float mixRatio = AiShaderEvalParamFlt(p_mixRatio);

      int cnt;
      float rock;
      float tex[3], tx2[3], fra[3], inv[3];
      float cosIntY, cosIntZ;
      float cosNxtY, cosNxtZ;
      float size;
      double index;

      static const float RockArray[100 + 1 /* "+1" for linux-x86. */] = {
          0.251059f, 0.208948f, 0.940928f, 0.422546f, 0.395893f,
          0.382565f, 0.231731f, 0.535547f, 0.533210f, 0.862949f,
          0.962365f, 0.315418f, 0.735979f, 0.654669f, 0.050290f,
          0.453805f, 0.806458f, 0.655780f, 0.811851f, 0.829877f,
          0.079188f, 0.204543f, 0.680214f, 0.651889f, 0.976463f,
          0.085075f, 0.203873f, 0.231494f, 0.014263f, 0.156578f,
          0.014036f, 0.703928f, 0.089309f, 0.571295f, 0.957362f,
          0.286090f, 0.143671f, 0.825377f, 0.857297f, 0.350239f,
          0.253979f, 0.286101f, 0.827189f, 0.551713f, 0.957706f,
          0.015494f, 0.849222f, 0.993476f, 0.167158f, 0.587348f,
          0.545666f, 0.337887f, 0.368854f, 0.610192f, 0.997154f,
          0.426896f, 0.326217f, 0.027302f, 0.853496f, 0.276501f,
          0.139256f, 0.185820f, 0.286338f, 0.841148f, 0.036720f,
          0.635994f, 0.952952f, 0.160629f, 0.532699f, 0.639396f,
          0.683071f, 0.008548f, 0.747579f, 0.198267f, 0.125615f,
          0.950704f, 0.527485f, 0.849188f, 0.861940f, 0.427578f,
          0.079964f, 0.540884f, 0.222840f, 0.621933f, 0.837634f,
          0.201585f, 0.156817f, 0.125407f, 0.155357f, 0.425212f,
          0.359167f, 0.832446f, 0.807281f, 0.412272f, 0.560455f,
          0.762489f, 0.473624f, 0.501293f, 0.417249f, 0.228637f,
          0.228637f
      };


      for (cnt = 0; cnt < 3; cnt++) {
          fra[cnt] = fabsf(P[cnt] + 100.0f) / grainSize;
          tex[cnt] = (float)(int)fra[cnt];
          fra[cnt] -= tex[cnt];
          tx2[cnt] = (tex[cnt] + 1)*grainSize;
          tex[cnt] *= grainSize;
          inv[cnt] = 1.0f - fra[cnt];
      }

      MAYA_CHEAP_SIN(cosIntY, 0.25f + tex[1] * 2371.5f)
      MAYA_CHEAP_SIN(cosNxtY, 0.25f + tx2[1] * 2371.5f)
      MAYA_CHEAP_SIN(cosIntZ, 0.80f + tex[2] * 3482.2f)
      MAYA_CHEAP_SIN(cosNxtZ, 0.80f + tx2[2] * 3482.2f)

      rock = 0.0f;
      ROCK_CYCLOID(index, (double)(tex[0] + cosIntY + cosIntZ) * 2355.3)
      rock += RockArray[(int)(100.0f*index)] * inv[0] * inv[1] * inv[2];
      ROCK_CYCLOID(index, (double)(tex[0] + cosIntY + cosNxtZ) * 2355.3)
      rock += RockArray[(int)(100.0f*index)] * inv[0] * inv[1] * fra[2];
      ROCK_CYCLOID(index, (double)(tex[0] + cosNxtY + cosIntZ) * 2355.3)
      rock += RockArray[(int)(100.0f*index)] * inv[0] * fra[1] * inv[2];
      ROCK_CYCLOID(index, (double)(tex[0] + cosNxtY + cosNxtZ) * 2355.3)
      rock += RockArray[(int)(100.0f*index)] * inv[0] * fra[1] * fra[2];
      ROCK_CYCLOID(index, (double)(tx2[0] + cosIntY + cosIntZ) * 2355.3)
      rock += RockArray[(int)(100.0f*index)] * fra[0] * inv[1] * inv[2];
      ROCK_CYCLOID(index, (double)(tx2[0] + cosIntY + cosNxtZ) * 2355.3)
      rock += RockArray[(int)(100.0f*index)] * fra[0] * inv[1] * fra[2];
      ROCK_CYCLOID(index, (double)(tx2[0] + cosNxtY + cosIntZ) * 2355.3)
      rock += RockArray[(int)(100.0f*index)] * fra[0] * fra[1] * inv[2];
      ROCK_CYCLOID(index, (double)(tx2[0] + cosNxtY + cosNxtZ) * 2355.3)
      rock += RockArray[(int)(100.0f*index)] * fra[0] * fra[1] * fra[2];

      size = 0.5f * MAYA_CLAMP(0.01f, 1.0E-08f, 1.0f) / grainSize;
      rock = (rock - mixRatio) / (diffusion + size);
      rock /= 1.0f - 2.0f * fabsf(mixRatio - 0.5f);
      rock = MAYA_CLAMP(rock, -0.5f, 0.5f);
      size = 1.0f - 0.5f * size;
      if (size < 0.0f)
          size = 0.0f;
      mixRatio = 1.0f - mixRatio;
      rock = size * (0.5f + rock - mixRatio) + mixRatio;

      AtRGB outColor;
      outColor.r = rock * (color1.r - color2.r) + color2.r;
      outColor.g = rock * (color1.g - color2.g) + color2.g;
      outColor.b = rock * (color1.b - color2.b) + color2.b;

      AiRGBtoRGBA(outColor, sg->out.RGBA);
      MayaColorBalance(sg, node, p_defaultColor, sg->out.RGBA);
      
   }
   else
   {
      MayaDefaultColor(sg, node, p_defaultColor, sg->out.RGBA);
   }
   if (usePref) RestorePoints(sg, tmpPts);
}
