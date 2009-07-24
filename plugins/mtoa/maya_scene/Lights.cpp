
#include "MayaScene.h"

#include <ai_constants.h>
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
#include <maya/MPlug.h>
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
      MFnAmbientLight fnLight(dagPath);

      light = AiNode("ambient_light");

      AiNodeSetStr(light, "name", fnDagNode.name().asChar());

      color = fnLight.color();
      AiNodeSetRGB(light, "color", color.r, color.g, color.b);
      AiNodeSetFlt(light, "intensity", fnLight.intensity());

      AiNodeSetBool(light, "cast_shadows", fnLight.useRayTraceShadows());
      AiNodeSetInt(light, "samples", fnDagNode.findPlug("shadowRays").asInt());

      AiNodeSetBool(light, "affect_diffuse", fnLight.lightDiffuse());
      AiNodeSetBool(light, "affect_specular", fnLight.lightSpecular());
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
      AiNodeSetFlt(light, "angle", fnLight.shadowAngle());
      AiNodeSetInt(light, "samples", fnDagNode.findPlug("shadowRays").asInt());

      AiNodeSetBool(light, "affect_diffuse", fnLight.lightDiffuse());
      AiNodeSetBool(light, "affect_specular", fnLight.lightSpecular());
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
      AiNodeSetArray(light, "position", AiArrayConvert(1, 1, AI_TYPE_POINT, &vector, TRUE));

      AiNodeSetBool(light, "cast_shadows", fnLight.useRayTraceShadows());
      AiNodeSetFlt(light, "radius", fnLight.shadowRadius());
      AiNodeSetInt(light, "samples", fnDagNode.findPlug("shadowRays").asInt());

      AiNodeSetBool(light, "affect_diffuse", fnLight.lightDiffuse());
      AiNodeSetBool(light, "affect_specular", fnLight.lightSpecular());
   }
   else if (dagPath.hasFn(MFn::kSpotLight))
   {
      MFnSpotLight fnLight(dagPath);

      light = AiNode("spot_light");

      AiNodeSetStr(light, "name", fnDagNode.name().asChar());

      color = fnLight.color();
      AiNodeSetRGB(light, "color", color.r, color.g, color.b);
      AiNodeSetFlt(light, "intensity", fnLight.intensity());

      lightWorldMatrix = dagPath.inclusiveMatrix();
      vector           = lightWorldMatrix.translation(MSpace::kWorld);
      AiNodeSetArray(light, "position", AiArrayConvert(1, 1, AI_TYPE_POINT, &vector, TRUE));

      vector += fnLight.lightDirection(0, MSpace::kWorld);
      AiNodeSetArray(light, "look_at", AiArrayConvert(1, 1, AI_TYPE_POINT, &vector, TRUE));

      AiNodeSetBool(light, "cast_shadows", fnLight.useRayTraceShadows());
      AiNodeSetFlt(light, "radius", fnLight.shadowRadius());
      AiNodeSetInt(light, "samples", fnDagNode.findPlug("shadowRays").asInt());

      AiNodeSetFlt(light, "cone_angle", (fnLight.coneAngle() + fnLight.penumbraAngle()) * AI_RTOD);
      AiNodeSetFlt(light, "penumbra_angle", fabsf(fnLight.penumbraAngle()) * AI_RTOD);

      AiNodeSetBool(light, "affect_diffuse", fnLight.lightDiffuse());
      AiNodeSetBool(light, "affect_specular", fnLight.lightSpecular());
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
