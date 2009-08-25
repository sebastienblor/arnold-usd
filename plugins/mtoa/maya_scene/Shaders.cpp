
#include "MayaScene.h"

#include <ai_msg.h>
#include <ai_nodes.h>

#include <maya/MColor.h>
#include <maya/MFnBlinnShader.h>
#include <maya/MFnLambertShader.h>
#include <maya/MFnPhongShader.h>
#include <maya/MFnStringData.h>
#include <maya/MPlug.h>
#include <maya/MPlugArray.h>

// This is a shortcut for Arnold shaders (parameter name is the same in Maya and Arnold)
#define SHADER_PARAM(name, type) ProcessShaderParameter(mayaShader, name, shader, name, type)

void CMayaScene::ProcessShaderParameter(MFnDependencyNode shader, const char* param, AtNode* arnoldShader, const char* arnoldAttrib, int arnoldAttribType)
{
   MPlugArray connections;

   MPlug plug = shader.findPlug(param);

   plug.connectedTo(connections, true, false);

   if (connections.length() == 0)
   {
      switch(arnoldAttribType)
      {
      case AI_TYPE_RGB:
         {
            AiNodeSetRGB(arnoldShader, arnoldAttrib, plug.child(0).asFloat(), plug.child(1).asFloat(), plug.child(2).asFloat());
         }
         break;
      case AI_TYPE_FLOAT:
         {
            AiNodeSetFlt(arnoldShader, arnoldAttrib, plug.asFloat());
         }
         break;
      case AI_TYPE_BOOLEAN:
         {
            AiNodeSetBool(arnoldShader, arnoldAttrib, plug.asBool());
         }
         break;
      case AI_TYPE_ENUM:
         {
            AiNodeSetInt(arnoldShader, arnoldAttrib, plug.asInt());
         }
         break;
      case AI_TYPE_INT:
         {
            AiNodeSetInt(arnoldShader, arnoldAttrib, plug.asInt());
         }
         break;
      case AI_TYPE_STRING:
         {
            AiNodeSetStr(arnoldShader, arnoldAttrib, plug.asString().asChar());
         }
         break;
      case AI_TYPE_VECTOR:
         {
            AiNodeSetVec(arnoldShader, arnoldAttrib, plug.child(0).asFloat(), plug.child(1).asFloat(), plug.child(2).asFloat());
         }
         break;
      case AI_TYPE_POINT:
         {
            AiNodeSetPnt(arnoldShader, arnoldAttrib, plug.child(0).asFloat(), plug.child(1).asFloat(), plug.child(2).asFloat());
         }
         break;
      }
   }
   else
   {
      AtNode* node = ExportShader(connections[0].node());

      if (node != NULL)
         AiNodeLink(node, arnoldAttrib, arnoldShader);
   }
}

MObject CMayaScene::GetNodeShader(MObject dagNode)
{
   MPlugArray        connections;
   MFnDependencyNode fnDGNode(dagNode);

   // Find the "instObjGroups" attribute. Follow that plug to see where it is connected.
   MPlug plug(dagNode, fnDGNode.attribute("instObjGroups"));

   plug.elementByLogicalIndex(0).connectedTo(connections, false, true);

   if (connections.length() != 1)
   {
      return MObject::kNullObj;
   }

   MObject shadingGroup(connections[0].node());

   fnDGNode.setObject(shadingGroup);

   MPlug shaderPlug(shadingGroup, fnDGNode.attribute("surfaceShader"));

   connections.clear();

   shaderPlug.connectedTo(connections, true, false);

   return connections[0].node();
}

AtNode* CMayaScene::ExportArnoldShader(MObject mayaShader, MString arnoldShader)
{
   AtNode* shader = NULL;
   MFnDependencyNode node(mayaShader);

   const AtNodeEntry* nodeEntry = AiNodeEntryLookUp(arnoldShader.asChar());

   if (nodeEntry != NULL)
   {
      shader = AiNode(arnoldShader.asChar());

      AiNodeSetStr(shader, "name", node.name().asChar());

      AtInt numParams = AiNodeEntryGetNumParams(nodeEntry);

      for (AtInt J = 0; (J < numParams); ++J)
      {
         const AtParamEntry* paramEntry = AiNodeEntryGetParameter(nodeEntry, J);

         if (AiParamGetName(paramEntry) != "name")
            SHADER_PARAM(AiParamGetName(paramEntry), AiParamGetType(paramEntry));
      }
   }

   return shader;
 }

AtNode* CMayaScene::ExportShader(MObject mayaShader)
{
   // First check if this shader has already been processed
   for (std::vector<CShaderData>::const_iterator it = m_processedShaders.begin(); (it != m_processedShaders.end()); ++it)
   {
      if (it->mayaShader == mayaShader)
      {
         return it->arnoldShader;
      }
   }

   AtNode* shader = NULL;

   MFnDependencyNode node(mayaShader);

   AiMsgDebug("[mtoa] Exporting shader: %s", node.name().asChar());

   if (node.typeName() == "ArnoldStandardShader")
   {
      shader = ExportArnoldShader(mayaShader, "standard");
   }
   else if (node.typeName() == "ArnoldUtilityShader")
   {
      shader = ExportArnoldShader(mayaShader, "utility");
   }
   else if (node.typeName() == "ArnoldAmbientOcclusionShader")
   {
      shader = ExportArnoldShader(mayaShader, "ambient_occlusion");
   }
   else if (node.typeName() == "ArnoldSkyShader")
   {
      shader = ExportArnoldShader(mayaShader, "sky");
   }
   else if (node.typeName() == "ArnoldFogShader")
   {
      shader = ExportArnoldShader(mayaShader, "fog");
   }
   else if (node.typeName() == "ArnoldVolumeScatteringShader")
   {
      shader = ExportArnoldShader(mayaShader, "volume_scattering");
   }
   else if (node.typeName() == "ArnoldWireframeShader")
   {
      shader = ExportArnoldShader(mayaShader, "wireframe");
   }
   else if (node.typeName() == "ArnoldBarndoorShader")
   {
      shader = ExportArnoldShader(mayaShader, "barndoor");
   }
   else if (node.typeName() == "ArnoldGoboShader")
   {
      shader = ExportArnoldShader(mayaShader, "gobo");
   }
   else if (node.typeName() == "ArnoldLightBlockerShader")
   {
      shader = ExportArnoldShader(mayaShader, "light_blocker");
   }
   else if (node.typeName() == "ArnoldLightDecayShader")
   {
      shader = ExportArnoldShader(mayaShader, "light_decay");
   }
   else if (node.typeName() == "lambert")
   {
      MFnLambertShader fnShader(mayaShader);

      shader = AiNode("lambert");
   
      AiNodeSetStr(shader, "name", fnShader.name().asChar());

      ProcessShaderParameter(node, "diffuse", shader, "Kd", AI_TYPE_FLOAT);
      ProcessShaderParameter(node, "color", shader, "Kd_color", AI_TYPE_RGB);
      ProcessShaderParameter(node, "outMatteOpacity", shader, "opacity", AI_TYPE_RGB);
   }
   else if (node.typeName() == "surfaceShader")
   {
      shader = AiNode("flat");
   
      AiNodeSetStr(shader, "name", node.name().asChar());

      ProcessShaderParameter(node, "outColor", shader, "color", AI_TYPE_RGB);
      ProcessShaderParameter(node, "outMatteOpacity", shader, "opacity", AI_TYPE_RGB);
   }
   else if (node.typeName() == "file")
   {
      shader = AiNode("image");
      
      AiNodeSetStr(shader, "image_map", node.findPlug("fileTextureName").asString().asChar());
   }
   else if (node.typeName() == "multiplyDivide")
   {
      shader = ExportArnoldShader(mayaShader, "MayaMultiplyDivide");
   }
   else
   {
      AiMsgWarning("[mtoa] Shader type not supported: %s", node.typeName().asChar());
   }

   if (shader)
   {
      CShaderData   data;

      data.mayaShader   = mayaShader;
      data.arnoldShader = shader;

      m_processedShaders.push_back(data);
   }

   return shader;
}
