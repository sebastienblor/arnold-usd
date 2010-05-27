
#include "MayaScene.h"

#include <ai_msg.h>
#include <ai_nodes.h>
#include <ai_ray.h>

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
      case AI_TYPE_RGBA:
         {
            // Is the source parameter RGB or RGBA?
            if (plug.numChildren() == 4)
            {
               AiNodeSetRGBA(arnoldShader, arnoldAttrib, plug.child(0).asFloat(), plug.child(1).asFloat(), plug.child(2).asFloat(), plug.child(3).asFloat());
            }
            else
            {
               // For RGB source parameter, set alpha value to 1
               AiNodeSetRGBA(arnoldShader, arnoldAttrib, plug.child(0).asFloat(), plug.child(1).asFloat(), plug.child(2).asFloat(), 1);
            }
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
      MString attrName = connections[0].partialName(false, false, false, false, false, true);

      AtNode* node = ExportShader(connections[0].node(), attrName);

      if (node != NULL)
         AiNodeLink(node, arnoldAttrib, arnoldShader);
   }
}

MObject CMayaScene::GetNodeShader(MObject dagNode, int instanceNum)
{
   MPlugArray        connections;
   MFnDependencyNode fnDGNode(dagNode);

   // Find the "instObjGroups" attribute. Follow that plug to see where it is connected.
   MPlug plug(dagNode, fnDGNode.attribute("instObjGroups"));

   plug.elementByLogicalIndex(instanceNum).connectedTo(connections, false, true);

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

         if (strcmp(AiParamGetName(paramEntry), "name"))
         {
            SHADER_PARAM(AiParamGetName(paramEntry), AiParamGetType(paramEntry));
         }
      }

      MPlugArray connections;

      MPlug plug = node.findPlug("normalCamera");

      plug.connectedTo(connections, true, false);
      if (connections.length() > 0)
      {
         MString attrName = connections[0].partialName(false, false, false, false, false, true);

         AtNode* bump = ExportShader(connections[0].node(), attrName);

         if (bump != NULL)
            AiNodeLink(bump, "@before", shader);
      }
   }

   return shader;
}

AtNode* CMayaScene::ExportShader(MObject mayaShader, const MString &attrName)
{
   // First check if this shader has already been processed
   for (std::vector<CShaderData>::const_iterator it = m_processedShaders.begin(); (it != m_processedShaders.end()); ++it)
   {
      if (it->mayaShader == mayaShader && it->attrName == attrName)
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
      shader = AiNode("sky");

      AiNodeSetStr(shader, "name", node.name().asChar());

      ProcessShaderParameter(node, "X", shader, "X", AI_TYPE_VECTOR);
      ProcessShaderParameter(node, "X_angle", shader, "X_angle", AI_TYPE_FLOAT);
      ProcessShaderParameter(node, "Y", shader, "Y", AI_TYPE_VECTOR);
      ProcessShaderParameter(node, "Y_angle", shader, "Y_angle", AI_TYPE_FLOAT);
      ProcessShaderParameter(node, "Z", shader, "Z", AI_TYPE_VECTOR);
      ProcessShaderParameter(node, "Z_angle", shader, "Z_angle", AI_TYPE_FLOAT);
      ProcessShaderParameter(node, "color", shader, "color", AI_TYPE_RGB);
      ProcessShaderParameter(node, "format", shader, "format", AI_TYPE_ENUM);;
      ProcessShaderParameter(node, "intensity", shader, "intensity", AI_TYPE_FLOAT);
      AiNodeSetBool(shader, "opaque_alpha", 1);

      AtInt visibility = 65535;

      if (!node.findPlug("casts_shadows").asBool())
         visibility &= ~AI_RAY_SHADOW;

      if (!node.findPlug("primary_visibility").asBool())
      {
         visibility &= ~AI_RAY_CAMERA;
         AiNodeSetBool(shader, "opaque_alpha", 0);
      }

      if (!node.findPlug("visible_in_reflections").asBool())
         visibility &= ~AI_RAY_REFLECTED;

      if (!node.findPlug("visible_in_refractions").asBool())
         visibility &= ~AI_RAY_REFRACTED;

      if (!node.findPlug("diffuse_visibility").asBool())
         visibility &= ~AI_RAY_DIFFUSE;

      if (!node.findPlug("glossy_visibility").asBool())
         visibility &= ~AI_RAY_GLOSSY;

      AiNodeSetInt(shader, "visibility", visibility);
   }
   else if (node.typeName() == "ArnoldHairShader")
   {
      shader = ExportArnoldShader(mayaShader, "hair");
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

      MPlugArray connections;

      MPlug plug = fnShader.findPlug("normalCamera");

      plug.connectedTo(connections, true, false);
      if (connections.length() > 0)
      {
         MString attrName = connections[0].partialName(false, false, false, false, false, true);

         AtNode* node = ExportShader(connections[0].node(), attrName);

         if (node != NULL)
            AiNodeLink(node, "@before", shader);
      }
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
      
      AiNodeSetStr(shader, "filename", node.findPlug("fileTextureName").asString().asChar());
   }
   else if (node.typeName() == "multiplyDivide")
   {
      shader = ExportArnoldShader(mayaShader, "MayaMultiplyDivide");
   }
   else if (node.typeName() == "reverse")
   {
      shader = ExportArnoldShader(mayaShader, "MayaReverse");
   }
   else if (node.typeName() == "clamp")
   {
      shader = ExportArnoldShader(mayaShader, "MayaClamp");
   }
   else if (node.typeName() == "gammaCorrect")
   {
      shader = ExportArnoldShader(mayaShader, "MayaGammaCorrect");
   }
   else if (node.typeName() == "condition")
   {
      shader = AiNode("MayaCondition");
   
      AiNodeSetStr(shader, "name", node.name().asChar());

      ProcessShaderParameter(node, "operation", shader, "operation", AI_TYPE_ENUM);
      ProcessShaderParameter(node, "firstTerm", shader, "first_term", AI_TYPE_FLOAT);
      ProcessShaderParameter(node, "secondTerm", shader, "second_term", AI_TYPE_FLOAT);
      ProcessShaderParameter(node, "colorIfTrue", shader, "color_if_true", AI_TYPE_RGB);
      ProcessShaderParameter(node, "colorIfFalse", shader, "color_if_false", AI_TYPE_RGB);
   }
   else if (node.typeName() == "blendColors")
   {
      shader = ExportArnoldShader(mayaShader, "MayaBlendColors");
   }
   else if (node.typeName() == "bump2d")
   {
      shader = AiNode("bump2d");
   
      AiNodeSetStr(shader, "name", node.name().asChar());

      ProcessShaderParameter(node, "bumpValue", shader, "bump_map", AI_TYPE_FLOAT);
      ProcessShaderParameter(node, "bumpDepth", shader, "bump_height", AI_TYPE_FLOAT);
   }
   else if (node.typeName() == "samplerInfo")
   {
      if (attrName == "facingRatio")
      {
         shader = AiNode("MayaFacingRatio");
      }
      else if (attrName == "flippedNormal")
      {
         shader = AiNode("MayaFlippedNormal");
      }
      if (shader != 0)
      {
         MString name = node.name() + "_" + attrName;
         AiNodeSetStr(shader, "name", name.asChar());
      }
   }
   else if (node.typeName() == "plusMinusAverage")
   {
      if (attrName == "output1D")
      {
         shader = AiNode("MayaPlusMinusAverage1D");

         MPlug elem, attr;

         attr = node.findPlug("operation");
         AiNodeSetInt(shader, "operation", attr.asInt());

         attr = node.findPlug("input1D");
         AtArray *values = AiArrayAllocate(attr.numElements(), 1, AI_TYPE_FLOAT);
         for (unsigned int i=0; i<attr.numElements(); ++i)
         {
            elem = attr.elementByPhysicalIndex(i);
            // This could actually be a connection but Arnold does not support
            // array element connections for now
            AiArraySetFlt(values, i, elem.asFloat());
         }
         AiNodeSetArray(shader, "values", values);

      }
      else if (attrName == "output2D")
      {
         shader = AiNode("MayaPlusMinusAverage2D");

         MObject oinx = node.attribute("input2Dx");
         MObject oiny = node.attribute("input2Dy");

         MPlug attr, elem, inx, iny;
         AtPoint2 value;

         attr = node.findPlug("operation");
         AiNodeSetInt(shader, "operation", attr.asInt());

         attr = node.findPlug("input2D");
         AtArray *values = AiArrayAllocate(attr.numElements(), 1, AI_TYPE_POINT2);
         for (unsigned int i=0; i<attr.numElements(); ++i)
         {
            elem = attr.elementByPhysicalIndex(i);
            // This could actually be a connection but Arnold does not support
            // array element connections for now
            inx = elem.child(oinx);
            iny = elem.child(oiny);
            value.x = inx.asFloat();
            value.y = iny.asFloat();
            AiArraySetPnt2(values, i, value);
         }
         AiNodeSetArray(shader, "values", values);

      }
      else if (attrName == "output3D")
      {
         shader = AiNode("MayaPlusMinusAverage3D");

         MObject oinx = node.attribute("input3Dx");
         MObject oiny = node.attribute("input3Dy");
         MObject oinz = node.attribute("input3Dz");

         MPlug attr, elem, inx, iny, inz;
         AtRGB value;

         attr = node.findPlug("operation");
         AiNodeSetInt(shader, "operation", attr.asInt());

         attr = node.findPlug("input3D");
         AtArray *values = AiArrayAllocate(attr.numElements(), 1, AI_TYPE_RGB);
         for (unsigned int i=0; i<attr.numElements(); ++i)
         {
            elem = attr.elementByPhysicalIndex(i);
            // This could actually be a connection but Arnold does not support
            // array element connections for now
            inx = elem.child(oinx);
            iny = elem.child(oiny);
            inz = elem.child(oinz);
            value.r = inx.asFloat();
            value.g = iny.asFloat();
            value.b = inz.asFloat();
            AiArraySetRGB(values, i, value);
         }
         AiNodeSetArray(shader, "values", values);

      }
      if (shader != 0)
      {
         MString name = node.name() + "_" + attrName;
         AiNodeSetStr(shader, "name", name.asChar());
      }
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
      data.attrName     = attrName;

      m_processedShaders.push_back(data);
   }

   return shader;
}
