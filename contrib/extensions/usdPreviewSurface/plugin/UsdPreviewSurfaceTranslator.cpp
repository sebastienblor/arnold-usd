#include "UsdPreviewSurfaceTranslator.h"

#include <maya/MRenderLineArray.h>
#include <maya/MRenderLine.h>
#include <maya/MDagPathArray.h>
#include <maya/MPointArray.h>
#include <maya/MHairSystem.h>
#include <maya/MFnPfxGeometry.h>
#include <maya/MRampAttribute.h>
#include <maya/MFnNurbsCurve.h>
#include <maya/MAnimControl.h>
#include <maya/MSelectionList.h>
#include <maya/MDGModifier.h>
#include <maya/MDGMessage.h>
#include <maya/MEventMessage.h>

#include <algorithm>
#include <string>

AtNode* CUsdPreviewSurfaceTranslator::CreateArnoldNodes()
{

   return AddArnoldNode("standard_surface");
}

// Translating Usd preview surface.
// Note that the code below should be similar to what is done in the arnold-usd repository
// when a previewSurface shader primitive is translated to arnold
void CUsdPreviewSurfaceTranslator::Export( AtNode *node )
{
   ProcessParameter(node, "base_color", AI_TYPE_RGB, "diffuseColor");
   AiNodeSetFlt(node, "base", 1.f); // scalar multiplier, set it to 1

   ProcessParameter(node, "emission_color", AI_TYPE_RGB, "emissiveColor");
   AiNodeSetFlt(node, "emission", 1.f); // scalar multiplier, set it to 1

   MPlug useSpecularWorkflow = FindMayaPlug("useSpecularWorkflow");
   if (useSpecularWorkflow.isNull() || useSpecularWorkflow.asBool())
   {
      ProcessParameter(node, "specular_color", AI_TYPE_RGB, "specularColor");
      // this is actually not correct. In USD, this is apparently the
      // fresnel 0° "front-facing" specular color. Specular is considered
      // to be always white for grazing angles

   } else
   {
      // metallic workflow, set the specular color to white and use the
      // metalness
      AiNodeSetRGB(node, "specular_color", 1.f, 1.f, 1.f);
      ProcessParameter(node, "metalness", AI_TYPE_FLOAT, "metallic");
   }

   ProcessParameter(node, "specular_roughness", AI_TYPE_FLOAT, "roughness");
   ProcessParameter(node, "specular_IOR", AI_TYPE_FLOAT, "ior");
   ProcessParameter(node, "coat", AI_TYPE_FLOAT, "clearcoat");
   ProcessParameter(node, "coat_roughness", AI_TYPE_FLOAT, "clearcoatRoughness");
   ProcessParameter(node, "opacity", AI_TYPE_RGB, "opacity");
   
   MPlug normalPlug = FindMayaPlug("normal");
   AiNodeUnlink(node, "normal");
   if (!normalPlug.isNull())
   {
      MPlugArray connections;
      normalPlug.connectedTo(connections, true, false);
      if (connections.length() > 0)
      {
         AtNode *bump = ExportConnectedNode(connections[0]);
         if (bump)
            AiNodeLink(bump, "normal", node);
      }
   }
}

