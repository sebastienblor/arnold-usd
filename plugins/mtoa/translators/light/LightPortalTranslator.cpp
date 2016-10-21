#include "LightPortalTranslator.h"
#include <ai_nodes.h>
#include <ai_ray.h>

bool CLightPortalTranslator::RequiresMotionData()
{
   return false;
}
   
void* CLightPortalTranslator::creator()
{
   return new CLightPortalTranslator();
}
   
AtNode* CLightPortalTranslator::CreateArnoldNodes()
{
   return AddArnoldNode("quad_light");
}

void CLightPortalTranslator::Export(AtNode* portal)
{
   // Early out, light isn't visible so no point exporting anything else.
   if (!IsRenderable())
   {
      AiNodeSetDisabled(portal, true);
      //AiNodeSetFlt(GetArnoldNode(), "intensity",  0.0f);
      return;
   }
   AiNodeSetDisabled(portal, false);
   
   ExportMatrix(portal);
   AtPoint vertices[4];

   AiV3Create(vertices[3], 1, 1, 0);
   AiV3Create(vertices[0], 1, -1, 0);
   AiV3Create(vertices[1], -1, -1, 0);
   AiV3Create(vertices[2], -1, 1, 0);

   AiNodeSetArray(portal, "vertices", AiArrayConvert(4, 1, AI_TYPE_POINT, vertices));
   AiNodeSetBool(portal, "portal", true);

}
