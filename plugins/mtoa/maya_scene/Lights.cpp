
#include "MayaScene.h"

#include <ai_msg.h>
#include <ai_nodes.h>

#include <maya/MColor.h>
#include <maya/MDagPath.h>
#include <maya/MFloatVector.h>
#include <maya/MFnAmbientLight.h>
#include <maya/MFnDirectionalLight.h>
#include <maya/MFnPointLight.h>
#include <maya/MFnSpotLight.h>
#include <maya/MMatrix.h>
#include <maya/MTransformationMatrix.h>
#include <maya/MVector.h>

void CMayaScene::ExportLight(const MDagPath& dagPath)
{

   MTransformationMatrix   lightWorldMatrix;
   MFloatVector            vector;
   MColor                  color;
   AtNode*                 light = NULL;
   MFnDagNode              fnDagNode(dagPath);

   if (dagPath.hasFn(MFn::kAmbientLight))
   {
   }
   else if (dagPath.hasFn(MFn::kDirectionalLight))
   {
      MFnDirectionalLight fnLight(dagPath);

      light = AiNode("distant_light");

      AiNodeSetStr(light, "name", fnDagNode.name().asChar());

      color = fnLight.color();

      AiNodeSetRGB(light, "color", color.r, color.g, color.b);
      AiNodeSetFlt(light, "intensity", fnLight.intensity());

      vector = fnLight.lightDirection(0, MSpace::kWorld);

      AiNodeSetVec(light, "direction", vector.x, vector.y, vector.z);
      AiNodeSetBool(light, "cast_shadows", fnLight.useRayTraceShadows());
   }
   else if (dagPath.hasFn(MFn::kPointLight))
   {
      MFnPointLight fnLight(dagPath);

      light = AiNode("point_light");

      AiNodeSetStr(light, "name", fnDagNode.name().asChar());

      color = fnLight.color();

      AiNodeSetRGB(light, "color", color.r, color.g, color.b);
      AiNodeSetFlt(light, "intensity", fnLight.intensity());

      lightWorldMatrix = dagPath.inclusiveMatrix();
      vector           = lightWorldMatrix.translation(MSpace::kWorld);

      AiNodeSetVec(light, "position", vector.x, vector.y, vector.z);

      AiNodeSetBool(light, "cast_shadows", fnLight.useRayTraceShadows());
   }
   else if (dagPath.hasFn(MFn::kSpotLight))
   {
   }
   else if (dagPath.hasFn(MFn::kAreaLight))
   {
   }
   else
   {
      MFnLight fnLight(dagPath);

      AiMsgError("[mtoa] ERROR: Unknown light type (%s).\n", fnLight.typeName().asChar());
   }

}  // ExportLight()
