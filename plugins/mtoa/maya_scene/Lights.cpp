
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
#include <maya/MSelectionList.h>
#include <maya/MTransformationMatrix.h>
#include <maya/MVector.h>

#include <vector>

void CMayaScene::ExportLightFilters(AtNode* light, MString filterNames)
{
   MSelectionList list;
   MObject        node;

   char* filterList = const_cast<char*>(filterNames.asChar());
   char* filterName = strtok(filterList, ":");
   
   std::vector<AtNode*> filters;

   while (filterName != NULL)
   {
      list.clear();
      list.add(filterName);

      if (list.length() > 0)
      {
         list.getDependNode(0, node);

         AtNode* filter = ExportShader(node);

         filters.push_back(filter);

         filterName = strtok(NULL, ":");
      }
   }

   AiNodeSetArray(light, "filters", AiArrayConvert(filters.size(), 1, AI_TYPE_POINTER, &filters[0], TRUE));
}

void CMayaScene::ExportLightData(AtNode* light, const MDagPath& dagPath, bool mb, bool custom)
{
   MColor color;
   AtMatrix matrix;
   MFnDagNode fnDagNode(dagPath);
   MFnLight fnLight(dagPath);

   AiNodeSetStr(light, "name", fnDagNode.partialPathName().asChar());

   color = fnLight.color();
   AiNodeSetRGB(light, "color", color.r, color.g, color.b);
   AiNodeSetFlt(light, "intensity", fnLight.intensity());

   AiNodeSetBool(light, "cast_shadows", fnLight.useRayTraceShadows());
   AiNodeSetInt(light, "samples", fnDagNode.findPlug("shadowRays").asInt());

   AiNodeSetBool(light, "affect_diffuse", fnLight.lightDiffuse());
   AiNodeSetBool(light, "affect_specular", fnLight.lightSpecular());

   // Check if custom attributes have been created, ignore them otherwise
   MStatus status;
   fnDagNode.findPlug("bounces", &status);
   bool customAttributes = (status == MS::kSuccess);

   if (customAttributes)
   {
      AiNodeSetInt(light, "sss_samples", fnDagNode.findPlug("sss_samples").asInt());

      AiNodeSetInt(light, "bounces", fnDagNode.findPlug("bounces").asInt());
      AiNodeSetFlt(light, "bounce_factor", fnDagNode.findPlug("bounce_factor").asFloat());

      MString filters = fnDagNode.findPlug("light_filters").asString();

      if (filters != "")
         ExportLightFilters(light, filters);
   }

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

   AtNode* light = AiNodeLookUpByName(fnDagNode.partialPathName().asChar());

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
   MFnDagNode fnDagNode(dagPath);

   bool mb = m_motionBlurData.enabled && m_fnArnoldRenderOptions->findPlug("mb_lights_enable").asBool();

   // Check if custom attributes have been created, ignore them otherwise
   MStatus status;
   fnDagNode.findPlug("bounces", &status);
   bool customAttributes = (status == MS::kSuccess);

   if (dagPath.hasFn(MFn::kAmbientLight))
   {
      if (step == 0)
      {
         light = AiNode("ambient_light");

         ExportLightData(light, dagPath, mb, customAttributes);
      }
   }
   else if (dagPath.hasFn(MFn::kDirectionalLight))
   {
      if (step == 0)
      {
         MFnDirectionalLight fnLight(dagPath);

         light = AiNode("distant_light");

         ExportLightData(light, dagPath, mb, customAttributes);

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

         ExportLightData(light, dagPath, mb, customAttributes);

         AiNodeSetFlt(light, "radius", fnLight.shadowRadius());

         if (customAttributes)
         {
            AiNodeSetBool(light, "affect_volumetrics", fnDagNode.findPlug("affect_volumetrics").asBool());
            AiNodeSetBool(light, "cast_volumetric_shadows", fnDagNode.findPlug("cast_volumetric_shadows").asBool());
         }
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

         ExportLightData(light, dagPath, mb, customAttributes);

         AiNodeSetFlt(light, "radius", fnLight.shadowRadius());
         AiNodeSetFlt(light, "cone_angle", (fnLight.coneAngle() + fnLight.penumbraAngle()) * AI_RTOD);
         AiNodeSetFlt(light, "penumbra_angle", fabsf(fnLight.penumbraAngle()) * AI_RTOD);
         AiNodeSetFlt(light, "cosine_power", fnLight.dropOff());

         if (customAttributes)
         {
            AiNodeSetBool(light, "affect_volumetrics", fnDagNode.findPlug("affect_volumetrics").asBool());
            AiNodeSetBool(light, "cast_volumetric_shadows", fnDagNode.findPlug("cast_volumetric_shadows").asBool());

            AiNodeSetFlt(light, "aspect_ratio", fnDagNode.findPlug("aspect_ratio").asFloat());
            AiNodeSetFlt(light, "lens_radius", fnDagNode.findPlug("lens_radius").asFloat());
         }
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

         ExportLightData(light, dagPath, mb, customAttributes);

         AtPoint vertices[4];
         
         AiV3Create(vertices[0], 1, 1, 0);
         AiV3Create(vertices[1], 1, -1, 0);
         AiV3Create(vertices[2], -1, -1, 0);
         AiV3Create(vertices[3], -1, 1, 0);

         AiNodeSetArray(light, "vertices", AiArrayConvert(4, 1, AI_TYPE_POINT, vertices, true));

         if (customAttributes)
         {
            AiNodeSetBool(light, "affect_volumetrics", fnDagNode.findPlug("affect_volumetrics").asBool());
            AiNodeSetBool(light, "cast_volumetric_shadows", fnDagNode.findPlug("cast_volumetric_shadows").asBool());

            AiNodeSetInt(light, "sidedness", fnDagNode.findPlug("sidedness").asInt());
            AiNodeSetBool(light, "solid_angle", fnDagNode.findPlug("solid_angle").asBool());
         }
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
