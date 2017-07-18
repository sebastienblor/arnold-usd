#include <ai.h>

#ifdef _MSC_VER
#define _USE_MATH_DEFINES
#endif
#include <cmath>

#include "MayaUtils.h"
#include "MayaNoiseUtils.h"

AI_SHADER_NODE_EXPORT_METHODS(MayaCraterMtd);

namespace
{
   enum MayaCraterParams
   {
      p_shaker = 0,
      p_channel1,
      p_channel2,
      p_channel3,
      p_melt,
      p_balance,
      p_frequency,
      p_normDepth,
      p_normMelt,
      p_normBalance,
      p_normFrequency,

      p_wrap,
      p_local,
      p_placementMatrix,
      MAYA_COLOR_BALANCE_ENUM
   };
   struct MayaCraterData {
      bool placementMatrixLinked;
   };
}

node_parameters
{
   AtMatrix id = AiM4Identity();

   AiParameterFlt("shaker", 1.5f);
   AiParameterRGB("channel1", 1.0f, 0.0f, 0.0f);
   AiParameterRGB("channel2", 0.0f, 1.0f, 0.0f);
   AiParameterRGB("channel3", 0.0f, 0.0f, 1.0f);
   AiParameterFlt("melt", 0.0f);
   AiParameterFlt("balance", 0.0f);
   AiParameterFlt("frequency", 1.5f);

   AiParameterFlt("normDepth", 5.0f);
   AiParameterFlt("normMelt", 0.0f);
   AiParameterFlt("normBalance", 1.0f);
   AiParameterFlt("normFrequency", 1.0f);
   
   AiParameterBool("wrap", true);
   AiParameterBool("local", false);
   AiParameterMtx("placementMatrix", id);
   AddMayaColorBalanceParams(params, nentry);

   AiMetaDataSetStr(nentry, NULL, "maya.name", "crater");
   AiMetaDataSetInt(nentry, NULL, "maya.id", 0x52544D52);
   AiMetaDataSetStr(nentry, NULL, "_synonym", "mayaCrater");

}

node_initialize
{
   AiNodeSetLocalData(node, new MayaCraterData());   
}

node_update
{
   MayaCraterData* data =(MayaCraterData*)AiNodeGetLocalData(node);
   data->placementMatrixLinked = AiNodeIsLinked(node, "placementMatrix");
}

node_finish
{
   delete (MayaCraterData*)AiNodeGetLocalData(node);
}

shader_evaluate
{
   MayaCraterData* data =(MayaCraterData*)AiNodeGetLocalData(node);
   AtMatrix *placementMatrix = AiShaderEvalParamMtx(p_placementMatrix);
   bool local = AiShaderEvalParamBool(p_local);
   bool wrap = AiShaderEvalParamBool(p_wrap);
   
   AtVector P;

   AtVector tmpPts;
   bool usePref = SetRefererencePoints(sg, tmpPts);

   P = AiM4PointByMatrixMult(*placementMatrix, (local ? sg->Po : sg->P));
   if (data->placementMatrixLinked)
      delete placementMatrix;
   
   if (wrap || ((-1.0f <= P.x && P.x <= 1.0f) &&
                (-1.0f <= P.y && P.y <= 1.0f) &&
                (-1.0f <= P.z && P.z <= 1.0f)))
   {
      AtRGB channel1 = AiShaderEvalParamRGB(p_channel1);
      AtRGB channel2 = AiShaderEvalParamRGB(p_channel2);
      AtRGB channel3 = AiShaderEvalParamRGB(p_channel3);
      float shaker = AiShaderEvalParamFlt(p_shaker);
      float melt = 2.0f * AiShaderEvalParamFlt(p_melt);
      float balance = AiShaderEvalParamFlt(p_balance);
      float frequency = AiShaderEvalParamFlt(p_frequency);

      //float blend;
      float sampleSize;
      float coeff, m1, m2, p1, p2, p3;

//       AtVector normal = (MAYA_CONNECTED(&parms->normalCamera)) ?
//           *mi_eval_vector(&parms->normalCamera) :
//           state->normal;
// 
//       TEX3D_FILTERED_SIZE(size)

      sampleSize = melt;
      if (sampleSize < AI_EPSILON)
          sampleSize = AI_EPSILON;

      /* Scalar recurrence. */
      float crater = MayaNoiseUtils::recurrenceGetValue3d(sampleSize, shaker, &P);

      coeff = 1.0f / (balance + 1.0f);
      m1 = sinf(crater*frequency);
      m1 *= m1;
      m2 = m1 - 1.0f;
      /* m1 = b/(b+1) * sin(c*f)^8 */
      m1 *= m1; m1 *= m1 * balance * coeff;
      /* m2 = 1/(b+1) * (1 - cos(c*f)^8) */
      m2 *= m2; m2 *= m2; m2 = (1.0f - m2) * coeff;

      p3 = m1 + m2;
      p1 = p3 - 1.0f; p1 *= p1;	    /* (m1 + m2 - 1)^2                  */
      p2 = -2.0f*p3*(p3 - 1.0f);	/* -2 * (m1 + m2 - 1) * (m1 + m2)   */
      p3 *= p3;			            /* (m1 + m2)^2                      */

      /* Compute result color. */
      AtRGB outColor;
      outColor.r = p1*channel1.r + p2*channel2.r + p3*channel3.r;
      outColor.g = p1*channel1.g + p2*channel2.g + p3*channel3.g;
      outColor.b = p1*channel1.b + p2*channel2.b + p3*channel3.b;
      if (outColor.r < 0.0f) {
          outColor.r = -outColor.r;
          if (outColor.r > 1.0f)
              outColor.r = 1.0f;
      }
      else
          if (outColor.r > 1.0f) {
              outColor.r = 2.0f - outColor.r;
              if (outColor.r < 0.0f)
                  outColor.r = 0.0f;
          }
      if (outColor.g < 0.0f) {
          outColor.g = -outColor.g;
          if (outColor.g > 1.0f)
              outColor.g = 1.0f;
      }
      else
          if (outColor.g > 1.0f) {
              outColor.g = 2.0f - outColor.g;
              if (outColor.g < 0.0f)
                  outColor.g = 0.0f;
          }
      if (outColor.b < 0.0f) {
          outColor.b = -outColor.b;
          if (outColor.b > 1.0f)
              outColor.b = 1.0f;
      }
      else
          if (outColor.b > 1.0f) {
              outColor.b = 2.0f - outColor.b;
              if (outColor.b < 0.0f)
                  outColor.b = 0.0f;
          }

      sg->out.RGBA() = AtRGBA(outColor);
      MayaColorBalance(sg, node, p_defaultColor, sg->out.RGBA());

      melt = 2.0f * AiShaderEvalParamFlt(p_normMelt);

      frequency = AiShaderEvalParamFlt(p_normFrequency);
      balance = AiShaderEvalParamFlt(p_normBalance);
      AtVector inNormal = sg->N;

      sampleSize = melt;
      if (sampleSize < AI_EPSILON)
          sampleSize = AI_EPSILON;

      /* Vector recurrence. */
      //sg->N = MayaNoiseUtils::recurrenceGetVector3d(sampleSize, normDepth, &P);

      sg->N = AiV3Normalize(sg->N);

      coeff = 1.0f / (balance + 1.0f);
      m1 = sinf(crater*frequency);
      m1 *= m1;
      m2 = m1 - 1.0f;
      /* m1 = b/(b+1) * sin(c*f)^8 */
      m1 *= m1; m1 *= m1 * balance * coeff;
      /* m2 = 1/(b+1) * (1 - cos(c*f)^8) */
      m2 *= m2; m2 *= m2; m2 = (1.0f - m2) * coeff;

      /* Computation must be carried out in camera space. */
      //sg->N = AiShaderGlobalsTransformPoint(sg, sg->N, 1);

      sg->N = AiV3Normalize(sg->N);

      /* Compute result normal. */
      sg->N.x = (m1 + m2) * sg->N.x + inNormal.x;
      sg->N.y = (m1 + m2) * sg->N.y + inNormal.y;
      sg->N.z = (m1 + m2) * sg->N.z + inNormal.z;

      sg->N = AiShaderGlobalsTransformPoint(sg, sg->N, 0);
      AiV3Normalize(sg->N); 

      if (!AiV3IsFinite(sg->N))
          sg->N = inNormal;
      else
      {
          sg->Nf = sg->N;
          AiFaceViewer(sg);
      }
   }
   else
   {
      MayaDefaultColor(sg, node, p_defaultColor, sg->out.RGBA());
   }
   if (usePref) RestorePoints(sg, tmpPts);
}
