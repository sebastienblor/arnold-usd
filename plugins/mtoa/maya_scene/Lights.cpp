
#include "MayaScene.h"

#include <ai_constants.h>
#include <ai_msg.h>
#include <ai_nodes.h>

#include <maya/MColor.h>
#include <maya/MDagPath.h>
#include <maya/MFloatVector.h>
#include <maya/MFnAmbientLight.h>
#include <maya/MFnAreaLight.h>
#include <maya/MFnDirectionalLight.h>
#include <maya/MFnPointLight.h>
#include <maya/MFnSpotLight.h>
#include <maya/MMatrix.h>
#include <maya/MPlug.h>
#include <maya/MTransformationMatrix.h>
#include <maya/MVector.h>

void CMayaScene::ExportLightData(AtNode* light, const MDagPath& dagPath, bool mb)
{
   MColor color;
   AtMatrix matrix;
   MFnDagNode fnDagNode(dagPath);
   MFnLight fnLight(dagPath);

   AiNodeSetStr(light, "name", fnDagNode.name().asChar());

   color = fnLight.color();
   AiNodeSetRGB(light, "color", color.r, color.g, color.b);
   AiNodeSetFlt(light, "intensity", fnLight.intensity());

   AiNodeSetBool(light, "cast_shadows", fnLight.useRayTraceShadows());
   AiNodeSetInt(light, "samples", fnDagNode.findPlug("shadowRays").asInt());

   AiNodeSetBool(light, "affect_diffuse", fnLight.lightDiffuse());
   AiNodeSetBool(light, "affect_specular", fnLight.lightSpecular());

   GetMatrix(matrix, dagPath);

   if (mb)
   {
      AtArray* matrices = AiArrayAllocate(1, m_motionBlurData.motion_steps, AI_TYPE_MATRIX);
      AiArraySetMtx(matrices, 0, matrix);
      AiNodeSetArray(light, "matrix", matrices);
   }
   else
   {
      AiNodeSetMatrix(light, "matrix", matrix);
   }
}

void CMayaScene::ExportLightMBData(const MDagPath& dagPath, AtUInt step)
{
   AtMatrix matrix;
   MFnDagNode fnDagNode(dagPath);

   AtNode* light = AiNodeLookUpByName(fnDagNode.name().asChar());

   GetMatrix(matrix, dagPath);

   AtArray* matrices = AiNodeGetArray(light, "matrix");
   AiArraySetMtx(matrices, step, matrix);
}

void CMayaScene::ExportLight(const MDagPath& dagPath, AtUInt step)
{
   MTransformationMatrix lightWorldMatrix;
   AtMatrix matrix;
   MFloatVector vector;
   AtNode* light = NULL;

   bool mb = m_motionBlurData.enabled && m_fnArnoldRenderOptions->findPlug("mb_lights_enable").asBool();

   if (dagPath.hasFn(MFn::kAmbientLight))
   {
      if (step == 0)
      {
         light = AiNode("ambient_light");

         ExportLightData(light, dagPath, mb);
      }
   }
   else if (dagPath.hasFn(MFn::kDirectionalLight))
   {
      if (step == 0)
      {
         MFnDirectionalLight fnLight(dagPath);

         light = AiNode("distant_light");

         ExportLightData(light, dagPath, mb);

         AiNodeSetFlt(light, "angle", fnLight.shadowAngle());
      }
      else if (mb)
      {
         ExportLightMBData(dagPath, step);
      }
   }
   else if (dagPath.hasFn(MFn::kPointLight))
   {
      if (step == 0)
      {
         MFnPointLight fnLight(dagPath);

         light = AiNode("point_light");

         ExportLightData(light, dagPath, mb);

         AiNodeSetFlt(light, "radius", fnLight.shadowRadius());
      }
      else if (mb)
      {
         ExportLightMBData(dagPath, step);
      }
   }
   else if (dagPath.hasFn(MFn::kSpotLight))
   {
      if (step == 0)
      {
         MFnSpotLight fnLight(dagPath);

         light = AiNode("spot_light");

         ExportLightData(light, dagPath, mb);

         AiNodeSetFlt(light, "radius", fnLight.shadowRadius());
         AiNodeSetFlt(light, "cone_angle", (fnLight.coneAngle() + fnLight.penumbraAngle()) * AI_RTOD);
         AiNodeSetFlt(light, "penumbra_angle", fabsf(fnLight.penumbraAngle()) * AI_RTOD);
      }
      else if (mb)
      {
         ExportLightMBData(dagPath, step);
      }
   }
   else if (dagPath.hasFn(MFn::kAreaLight))
   {
      if (step == 0)
      {
         MFnAreaLight fnLight(dagPath);

         light = AiNode("quad_light");

         ExportLightData(light, dagPath, mb);

         AtPoint vertices[4];
         
         AiV3Create(vertices[0], 1, 1, 0);
         AiV3Create(vertices[1], 1, -1, 0);
         AiV3Create(vertices[2], -1, -1, 0);
         AiV3Create(vertices[3], -1, 1, 0);

         AiNodeSetArray(light, "vertices", AiArrayConvert(4, 1, AI_TYPE_POINT, vertices, true));

         AiNodeSetInt(light, "sidedness", 1);
         //AiNodeSetBool(light, "solid_angle", true);
      }
      else if (mb)
      {
         ExportLightMBData(dagPath, step);
      }
   }
   else
   {
      if (step == 0)
      {
         MFnLight fnLight(dagPath);
         AiMsgError("[mtoa] ERROR: Unknown light type (%s).\n", fnLight.typeName().asChar());
      }
   }
}
