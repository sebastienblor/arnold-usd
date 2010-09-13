
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
#include <maya/MAngle.h>
#include <maya/MFnNumericData.h>
#include <maya/MFnMatrixData.h>
#include <maya/MFnCamera.h>

#include <string>
#include <string.h>

static const char *g_remapInterpolationStrings[] =
{
   "none",
   "linear",
   "smooth",
   "spline"
};

static bool SortFloatArray(AtArray *a, AtUInt *shuffle = NULL)
{
   bool modified = false;

   if (a && a->nelements > 0)
   {
      float p0, p1;
      int tmp;

      bool swapped = true;
      AtUInt32 n = a->nelements;

      if (shuffle)
      {
         for (AtUInt32 i = 0; (i < n); ++i)
         {
            shuffle[i] = i;
         }
      }

      while (swapped)
      {
         swapped = false;
         n -= 1;
         for (AtUInt32 i = 0; (i < n); ++i)
         {
            p0 = AiArrayGetFlt(a, i);
            p1 = AiArrayGetFlt(a, i + 1);
            if (p0 > p1)
            {
               swapped = true;
               modified = true;

               AiArraySetFlt(a, i, p1);
               AiArraySetFlt(a, i + 1, p0);

               if (shuffle)
               {
                  tmp = shuffle[i];
                  shuffle[i] = shuffle[i + 1];
                  shuffle[i + 1] = tmp;
               }
            }
         }
      }
   }

   return modified;
}

static void ShuffleArray(AtArray *a, AtUInt *shuffle, int arnoldType)
{
   if (!a || !shuffle)
   {
      return;
   }

   if (arnoldType == AI_TYPE_FLOAT)
   {
      float tmp;
      for (AtUInt32 i = 0; (i < a->nelements); ++i)
      {
         if (shuffle[i] < i)
            continue;
         tmp = AiArrayGetFlt(a, i);
         AiArraySetFlt(a, i, AiArrayGetFlt(a, shuffle[i]));
         AiArraySetFlt(a, shuffle[i], tmp);
      }
   }
   else if (arnoldType == AI_TYPE_RGB)
   {
      AtRGB tmp;
      for (AtUInt32 i = 0; (i < a->nelements); ++i)
      {
         if (shuffle[i] < i)
            continue;
         tmp = AiArrayGetRGB(a, i);
         AiArraySetRGB(a, i, AiArrayGetRGB(a, shuffle[i]));
         AiArraySetRGB(a, shuffle[i], tmp);
      }
   }
   else if (arnoldType == AI_TYPE_RGBA)
   {
      AtRGBA tmp;
      for (AtUInt32 i = 0; (i < a->nelements); ++i)
      {
         if (shuffle[i] < i)
            continue;
         tmp = AiArrayGetRGBA(a, i);
         AiArraySetRGBA(a, i, AiArrayGetRGBA(a, shuffle[i]));
         AiArraySetRGBA(a, shuffle[i], tmp);
      }
   }
   else if (arnoldType == AI_TYPE_STRING)
   {
      std::string tmp0, tmp1;
      for (AtUInt32 i = 0; (i < a->nelements); ++i)
      {
         if (shuffle[i] < i)
            continue;
         tmp0 = AiArrayGetStr(a, i);
         tmp1 = AiArrayGetStr(a, shuffle[i]);
         AiArraySetStr(a, i, tmp1.c_str());
         AiArraySetStr(a, shuffle[i], tmp0.c_str());
      }
   }
   else if (arnoldType == AI_TYPE_BYTE)
   {
      AtByte tmp;
      for (AtUInt32 i = 0; (i < a->nelements); ++i)
      {
         if (shuffle[i] < i)
            continue;
         tmp = AiArrayGetByte(a, i);
         AiArraySetByte(a, i, AiArrayGetByte(a, shuffle[i]));
         AiArraySetByte(a, shuffle[i], tmp);
      }
   }
   else if (arnoldType == AI_TYPE_INT)
   {
      AtInt tmp;
      for (AtUInt32 i = 0; (i < a->nelements); ++i)
      {
         if (shuffle[i] < i)
            continue;
         tmp = AiArrayGetByte(a, i);
         AiArraySetByte(a, i, AiArrayGetByte(a, shuffle[i]));
         AiArraySetByte(a, shuffle[i], tmp);
      }
   }
   else if (arnoldType == AI_TYPE_UINT)
   {
      AtUInt tmp;
      for (AtUInt32 i = 0; (i < a->nelements); ++i)
      {
         if (shuffle[i] < i)
            continue;
         tmp = AiArrayGetByte(a, i);
         AiArraySetByte(a, i, AiArrayGetByte(a, shuffle[i]));
         AiArraySetByte(a, shuffle[i], tmp);
      }
   }
   else if (arnoldType == AI_TYPE_BOOLEAN)
   {
      bool tmp;
      for (AtUInt32 i = 0; (i < a->nelements); ++i)
      {
         if (shuffle[i] < i)
            continue;
         tmp = (AiArrayGetBool(a, i) == TRUE);
         AiArraySetBool(a, i, AiArrayGetBool(a, shuffle[i]));
         AiArraySetBool(a, shuffle[i], tmp);
      }
   }
   else if (arnoldType == AI_TYPE_VECTOR)
   {
      AtVector tmp;
      for (AtUInt32 i = 0; (i < a->nelements); ++i)
      {
         if (shuffle[i] < i)
            continue;
         tmp = AiArrayGetVec(a, i);
         AiArraySetVec(a, i, AiArrayGetVec(a, shuffle[i]));
         AiArraySetVec(a, shuffle[i], tmp);
      }
   }
   else if (arnoldType == AI_TYPE_POINT)
   {
      AtPoint tmp;
      for (AtUInt32 i = 0; (i < a->nelements); ++i)
      {
         if (shuffle[i] < i)
            continue;
         tmp = AiArrayGetPnt(a, i);
         AiArraySetPnt(a, i, AiArrayGetPnt(a, shuffle[i]));
         AiArraySetPnt(a, shuffle[i], tmp);
      }
   }
   else if (arnoldType == AI_TYPE_POINT2)
   {
      AtPoint2 tmp;
      for (AtUInt32 i = 0; (i < a->nelements); ++i)
      {
         if (shuffle[i] < i)
            continue;
         tmp = AiArrayGetPnt2(a, i);
         AiArraySetPnt2(a, i, AiArrayGetPnt2(a, shuffle[i]));
         AiArraySetPnt2(a, shuffle[i], tmp);
      }
   }
   else if (arnoldType == AI_TYPE_POINTER)
   {
      void *tmp;
      for (AtUInt32 i = 0; (i < a->nelements); ++i)
      {
         if (shuffle[i] < i)
            continue;
         tmp = AiArrayGetPtr(a, i);
         AiArraySetPtr(a, i, AiArrayGetPtr(a, shuffle[i]));
         AiArraySetPtr(a, shuffle[i], tmp);
      }
   }
   else if (arnoldType == AI_TYPE_MATRIX)
   {
      AtMatrix tmp0;
      AtMatrix tmp1;
      for (AtUInt32 i = 0; (i < a->nelements); ++i)
      {
         if (shuffle[i] < i)
            continue;
         AiArrayGetMtx(a, i, tmp0);
         AiArrayGetMtx(a, shuffle[i], tmp1);
         AiArraySetMtx(a, i, tmp1);
         AiArraySetMtx(a, shuffle[i], tmp0);
      }
   }
   else if (arnoldType == AI_TYPE_ENUM)
   {
      AtInt tmp;
      for (AtUInt32 i = 0; (i < a->nelements); ++i)
      {
         if (shuffle[i] < i)
            continue;
         tmp = AiArrayGetInt(a, i);
         AiArraySetInt(a, i, AiArrayGetInt(a, shuffle[i]));
         AiArraySetInt(a, shuffle[i], tmp);
      }
   }
}

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
      case AI_TYPE_POINT2:
         {
            float x, y;
            MObject numObj = plug.asMObject();
            MFnNumericData numData(numObj);
            numData.getData2Float(x, y);
            AiNodeSetPnt2(arnoldShader, arnoldAttrib, x, y);
         }
         break;
      case AI_TYPE_MATRIX:
         {
            AtMatrix am;
            MObject matObj = plug.asMObject();
            MFnMatrixData matData(matObj);
            MMatrix mm = matData.matrix();
            ConvertMatrix(am, mm);
            AiNodeSetMatrix(arnoldShader, arnoldAttrib, am);
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

      // Maya's X Y and Z Vectors
      AiNodeSetVec(shader, "X", 1.0f, 0.0f, 0.0f);
      AiNodeSetVec(shader, "Y", 0.0f, 1.0f, 0.0f);
      AiNodeSetVec(shader, "Z", 0.0f, 0.0f, -1.0f);

      MFnDagNode nodeDagNode(mayaShader);
      MDagPath nodeDagPath;
      nodeDagNode.getPath(nodeDagPath);
      MFnDependencyNode trNode(nodeDagPath.transform());

      MPlug plug   = trNode.findPlug("rotateX");
      MAngle angle;
      plug.getValue(angle);
      AiNodeSetFlt(shader, "X_angle", static_cast<float>(-angle.asDegrees()));

      plug = trNode.findPlug("rotateY");
      plug.getValue(angle);
      AiNodeSetFlt(shader, "Y_angle", static_cast<float>(angle.asDegrees()));

      plug = trNode.findPlug("rotateZ");
      plug.getValue(angle);
      AiNodeSetFlt(shader, "Z_angle", static_cast<float>(-angle.asDegrees()));

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
      MPlugArray connections;

      MPlug plug = node.findPlug("uvCoord");

      plug.connectedTo(connections, true, false);

      if (connections.length() != 0)
      {
         MObject srcObj = connections[0].node();
         MFnDependencyNode srcNode(srcObj);

         if (srcNode.typeName() == "place2dTexture")
         {
            shader = AiNode("MayaFile");
            AiNodeSetStr(shader, "filename", node.findPlug("fileTextureName").asString().asChar());

            ProcessShaderParameter(srcNode, "coverage", shader, "coverage", AI_TYPE_POINT2);
            ProcessShaderParameter(srcNode, "rotateFrame", shader, "rotateFrame", AI_TYPE_FLOAT);
            ProcessShaderParameter(srcNode, "translateFrame", shader, "translateFrame", AI_TYPE_POINT2);
            ProcessShaderParameter(srcNode, "mirrorU", shader, "mirrorU", AI_TYPE_BOOLEAN);
            ProcessShaderParameter(srcNode, "mirrorV", shader, "mirrorV", AI_TYPE_BOOLEAN);
            ProcessShaderParameter(srcNode, "wrapU", shader, "wrapU", AI_TYPE_BOOLEAN);
            ProcessShaderParameter(srcNode, "wrapV", shader, "wrapV", AI_TYPE_BOOLEAN);
            ProcessShaderParameter(srcNode, "stagger", shader, "stagger", AI_TYPE_BOOLEAN);
            ProcessShaderParameter(srcNode, "repeatUV", shader, "repeatUV", AI_TYPE_POINT2);
            ProcessShaderParameter(srcNode, "rotateUV", shader, "rotateUV", AI_TYPE_FLOAT);
            ProcessShaderParameter(srcNode, "offset", shader, "offsetUV", AI_TYPE_POINT2);
            ProcessShaderParameter(srcNode, "noiseUV", shader, "noiseUV", AI_TYPE_POINT2);
            ProcessShaderParameter(node, "colorGain", shader, "colorGain", AI_TYPE_RGB);
            ProcessShaderParameter(node, "colorOffset", shader, "colorOffset", AI_TYPE_RGB);
            ProcessShaderParameter(node, "alphaGain", shader, "alphaGain", AI_TYPE_FLOAT);
            ProcessShaderParameter(node, "alphaOffset", shader, "alphaOffset", AI_TYPE_FLOAT);
            ProcessShaderParameter(node, "alphaIsLuminance", shader, "alphaIsLuminance", AI_TYPE_BOOLEAN);
            ProcessShaderParameter(node, "invert", shader, "invert", AI_TYPE_BOOLEAN);
            ProcessShaderParameter(node, "defaultColor", shader, "defaultColor", AI_TYPE_RGB);
         }
      }

      if (!shader)
      {
         shader = AiNode("image");
         AiNodeSetStr(shader, "filename", node.findPlug("fileTextureName").asString().asChar());
      }

      AiNodeSetStr(shader, "name", node.name().asChar());
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
   else if (node.typeName() == "remapValue")
   {
      if (attrName == "outValue")
      {
         MPlug attr, elem, pos, val, interp;

         MObject opos = node.attribute("value_Position");
         MObject oval = node.attribute("value_FloatValue");
         MObject ointerp = node.attribute("value_Interp");

         shader = AiNode("MayaRemapValueToValue");

         ProcessShaderParameter(node, "inputValue", shader, "input", AI_TYPE_FLOAT);
         ProcessShaderParameter(node, "inputMin", shader, "inputMin", AI_TYPE_FLOAT);
         ProcessShaderParameter(node, "inputMax", shader, "inputMax", AI_TYPE_FLOAT);
         ProcessShaderParameter(node, "outputMin", shader, "outputMin", AI_TYPE_FLOAT);
         ProcessShaderParameter(node, "outputMax", shader, "outputMax", AI_TYPE_FLOAT);

         // Note: this doesn't handle connection coming in individual elements
         attr = node.findPlug("value");
         AtArray *positions = AiArrayAllocate(attr.numElements(), 1, AI_TYPE_FLOAT);
         AtArray *values = AiArrayAllocate(attr.numElements(), 1, AI_TYPE_FLOAT);
         AtArray *interps = AiArrayAllocate(attr.numElements(), 1, AI_TYPE_STRING);
         for (unsigned int i=0; i<attr.numElements(); ++i)
         {
            elem = attr.elementByPhysicalIndex(i);
            pos = elem.child(opos);
            val = elem.child(oval);
            interp = elem.child(ointerp);
            AiArraySetFlt(positions, i, pos.asFloat());
            AiArraySetFlt(values, i, val.asFloat());
            AiArraySetStr(interps, i, g_remapInterpolationStrings[interp.asInt()]);
         }
         // Need to sort the arrays (maya has the excellent idea not to do it)
         if (positions->nelements > 1)
         {
            AtUInt* shuffle = new AtUInt[positions->nelements];
            if (SortFloatArray(positions, shuffle))
            {
               ShuffleArray(values, shuffle, AI_TYPE_FLOAT);
               ShuffleArray(interps, shuffle, AI_TYPE_STRING);
            }
            delete[] shuffle;
         }
         AiNodeSetArray(shader, "positions", positions);
         AiNodeSetArray(shader, "values", values);
         AiNodeSetArray(shader, "interpolations", interps);
      }
      else if (attrName == "outColor")
      {
         MPlug attr, elem, pos, val, interp;

         MObject opos = node.attribute("color_Position");
         MObject oval = node.attribute("color_Color");
         MObject ointerp = node.attribute("color_Interp");

         shader = AiNode("MayaRemapValueToColor");

         ProcessShaderParameter(node, "inputValue", shader, "input", AI_TYPE_FLOAT);
         ProcessShaderParameter(node, "inputMin", shader, "inputMin", AI_TYPE_FLOAT);
         ProcessShaderParameter(node, "inputMax", shader, "inputMax", AI_TYPE_FLOAT);
         ProcessShaderParameter(node, "outputMin", shader, "outputMin", AI_TYPE_FLOAT);
         ProcessShaderParameter(node, "outputMax", shader, "outputMax", AI_TYPE_FLOAT);

         // Note: this doesn't handle connection coming in individual elements
         attr = node.findPlug("color");
         AtArray *positions = AiArrayAllocate(attr.numElements(), 1, AI_TYPE_FLOAT);
         AtArray *values = AiArrayAllocate(attr.numElements(), 1, AI_TYPE_RGB);
         AtArray *interps = AiArrayAllocate(attr.numElements(), 1, AI_TYPE_STRING);
         for (unsigned int i=0; i<attr.numElements(); ++i)
         {
            elem = attr.elementByPhysicalIndex(i);
            pos = elem.child(opos);
            val = elem.child(oval);
            interp = elem.child(ointerp);
            AiArraySetFlt(positions, i, pos.asFloat());
            AtRGB value;
            value.r = val.child(0).asFloat();
            value.g = val.child(1).asFloat();
            value.b = val.child(2).asFloat();
            AiArraySetRGB(values, i, value);
            AiArraySetStr(interps, i, g_remapInterpolationStrings[interp.asInt()]);
         }
         // Need to sort the arrays (maya has the excellent idea not to do it)
         if (positions->nelements > 1)
         {
            AtUInt* shuffle = new AtUInt[positions->nelements];
            if (SortFloatArray(positions, shuffle))
            {
               ShuffleArray(values, shuffle, AI_TYPE_RGB);
               ShuffleArray(interps, shuffle, AI_TYPE_STRING);
            }
            delete[] shuffle;
         }
         AiNodeSetArray(shader, "positions", positions);
         AiNodeSetArray(shader, "values", values);
         AiNodeSetArray(shader, "interpolations", interps);
      }
      if (shader != 0)
      {
         MString name = node.name() + "_" + attrName;
         AiNodeSetStr(shader, "name", name.asChar());
      }
   }
   else if (node.typeName() == "remapColor")
   {
      if (attrName == "outColor")
      {
         shader = AiNode("MayaRemapColor");

         MPlug attr, elem, pos, val, interp;

         const char *plugNames[3] = {"red", "green", "blue"};
         const char *posNames[6] = {"red_Position",   "rPositions",
                                    "green_Position", "gPositions",
                                    "blue_Position",  "bPositions"};
         const char *valNames[6] = {"red_FloatValue",   "rValues",
                                    "green_FloatValue", "gValues",
                                    "blue_FloatValue",  "bValues"};
         const char *interpNames[6] = {"red_Interp",   "rInterpolations",
                                       "green_Interp", "gInterpolations",
                                       "blue_Interp",  "bInterpolations"};

         ProcessShaderParameter(node, "color", shader, "input", AI_TYPE_RGB);
         ProcessShaderParameter(node, "inputMin", shader, "inputMin", AI_TYPE_FLOAT);
         ProcessShaderParameter(node, "inputMax", shader, "inputMax", AI_TYPE_FLOAT);
         ProcessShaderParameter(node, "outputMin", shader, "outputMin", AI_TYPE_FLOAT);
         ProcessShaderParameter(node, "outputMax", shader, "outputMax", AI_TYPE_FLOAT);

         for (int ci=0; ci<3; ++ci)
         {
            MObject opos = node.attribute(posNames[ci*2]);
            MObject oval = node.attribute(valNames[ci*2]);
            MObject ointerp = node.attribute(interpNames[ci*2]);

            // Note: this doesn't handle connection coming in individual elements
            attr = node.findPlug(plugNames[ci]);
            AtArray *positions = AiArrayAllocate(attr.numElements(), 1, AI_TYPE_FLOAT);
            AtArray *values = AiArrayAllocate(attr.numElements(), 1, AI_TYPE_FLOAT);
            AtArray *interps = AiArrayAllocate(attr.numElements(), 1, AI_TYPE_STRING);
            for (unsigned int i=0; i<attr.numElements(); ++i)
            {
               elem = attr.elementByPhysicalIndex(i);
               pos = elem.child(opos);
               val = elem.child(oval);
               interp = elem.child(ointerp);
               AiArraySetFlt(positions, i, pos.asFloat());
               AiArraySetFlt(values, i, val.asFloat());
               AiArraySetStr(interps, i, g_remapInterpolationStrings[interp.asInt()]);
            }
            // Need to sort array (maya has the excellent idea not to do it)
            if (positions->nelements > 1)
            {
               AtUInt* shuffle = new AtUInt[positions->nelements];
               if (SortFloatArray(positions, shuffle))
               {
                  ShuffleArray(values, shuffle, AI_TYPE_FLOAT);
                  ShuffleArray(interps, shuffle, AI_TYPE_STRING);
               }
               delete[] shuffle;
            }
            AiNodeSetArray(shader, posNames[ci*2 + 1], positions);
            AiNodeSetArray(shader, valNames[ci*2 + 1], values);
            AiNodeSetArray(shader, interpNames[ci*2 + 1], interps);
         }
      }
      if (shader != 0)
      {
         MString name = node.name() + "_" + attrName;
         AiNodeSetStr(shader, "name", name.asChar());
      }
   }
   else if (node.typeName() == "projection")
   {
      shader = AiNode("MayaProjection");

      AiNodeSetStr(shader, "name", node.name().asChar());

      ProcessShaderParameter(node, "projType", shader, "type", AI_TYPE_INT);
      ProcessShaderParameter(node, "uAngle", shader, "uAngle", AI_TYPE_FLOAT);
      ProcessShaderParameter(node, "vAngle", shader, "vAngle", AI_TYPE_FLOAT);
      ProcessShaderParameter(node, "local", shader, "local", AI_TYPE_BOOLEAN);
      ProcessShaderParameter(node, "wrap", shader, "wrap", AI_TYPE_BOOLEAN);
      ProcessShaderParameter(node, "invert", shader, "invert", AI_TYPE_BOOLEAN);
      ProcessShaderParameter(node, "colorGain", shader, "colorGain", AI_TYPE_RGB);
      ProcessShaderParameter(node, "colorOffset", shader, "colorOffset", AI_TYPE_RGB);
      ProcessShaderParameter(node, "defaultColor", shader, "defaultColor", AI_TYPE_RGB);
      ProcessShaderParameter(node, "alphaGain", shader, "alphaGain", AI_TYPE_FLOAT);
      ProcessShaderParameter(node, "alphaOffset", shader, "alphaOffset", AI_TYPE_FLOAT);
      ProcessShaderParameter(node, "image", shader, "image", AI_TYPE_RGBA);

      // alphaIsLuminance?

      // placementMatrix
      MPlug plug = node.findPlug("placementMatrix");
      // should follow connections here also
      // temporarily just read the value
      AtMatrix ipm;
      MObject matObj = plug.asMObject();
      MFnMatrixData matData(matObj);
      MMatrix mm = matData.matrix();
      ConvertMatrix(ipm, mm);
      AiNodeSetMatrix(shader, "mappingCoordinate", ipm);

      ProcessShaderParameter(node, "fitType", shader, "fitType", AI_TYPE_INT);
      ProcessShaderParameter(node, "fitFill", shader, "fillType", AI_TYPE_INT);

      plug = node.findPlug("linkedCamera");
      MPlugArray connections;
      plug.connectedTo(connections, true, false);
      if (connections.length() >= 1)
      {
         MObject camObj = connections[0].node();

         MFnCamera cam(camObj);

         MDagPath camPath;
         cam.getPath(camPath);

         AiNodeSetFlt(shader, "cameraNearPlane", static_cast<float>(cam.nearClippingPlane()));
         AiNodeSetFlt(shader, "cameraHorizontalFOV", static_cast<float>(cam.horizontalFieldOfView()));
         AiNodeSetFlt(shader, "cameraAspectRatio", static_cast<float>(cam.aspectRatio()));

         MMatrix trans = camPath.inclusiveMatrixInverse();
         AtMatrix atrans;
         ConvertMatrix(atrans, trans);
         AiNodeSetMatrix(shader, "cameraInverseMatrix", atrans);
      }
      else
      {
         // no linked camera, fit type needs to be None ?
         AiNodeSetInt(shader, "fitType", 0);
      }
   }
   else if (node.typeName() == "ramp")
   {
      shader = AiNode("MayaRamp");

      AiNodeSetStr(shader, "name", node.name().asChar());
      ProcessShaderParameter(node, "type", shader, "type", AI_TYPE_INT);
      ProcessShaderParameter(node, "interpolation", shader, "interpolation", AI_TYPE_INT);
      ProcessShaderParameter(node, "uWave", shader, "uWave", AI_TYPE_FLOAT);
      ProcessShaderParameter(node, "vWave", shader, "vWave", AI_TYPE_FLOAT);
      ProcessShaderParameter(node, "noise", shader, "noise", AI_TYPE_FLOAT);
      ProcessShaderParameter(node, "noiseFreq", shader, "noiseFreq", AI_TYPE_FLOAT);
      ProcessShaderParameter(node, "colorGain", shader, "colorGain", AI_TYPE_RGB);
      ProcessShaderParameter(node, "colorOffset", shader, "colorOffset", AI_TYPE_RGB);
      ProcessShaderParameter(node, "defaultColor", shader, "defaultColor", AI_TYPE_RGB);
      ProcessShaderParameter(node, "alphaGain", shader, "alphaGain", AI_TYPE_FLOAT);
      ProcessShaderParameter(node, "alphaOffset", shader, "alphaOffset", AI_TYPE_FLOAT);
      ProcessShaderParameter(node, "invert", shader, "invert", AI_TYPE_BOOLEAN);

      MPlug plug, elem, pos, col;
      MPlugArray connections;
      AtNode *placement = NULL;

      plug = node.findPlug("uvCoord");
      plug.connectedTo(connections, true, false);

      if (connections.length() != 0)
      {
         MObject srcObj = connections[0].node();
         MFnDependencyNode srcNode(srcObj);

         if (srcNode.typeName() == "place2dTexture")
         {
            placement = AiNode("MayaPlace2dTexture");

            AiNodeSetStr(placement, "name", srcNode.name().asChar());
            ProcessShaderParameter(srcNode, "coverage", placement, "coverage", AI_TYPE_POINT2);
            ProcessShaderParameter(srcNode, "rotateFrame", placement, "rotateFrame", AI_TYPE_FLOAT);
            ProcessShaderParameter(srcNode, "translateFrame", placement, "translateFrame", AI_TYPE_POINT2);
            ProcessShaderParameter(srcNode, "mirrorU", placement, "mirrorU", AI_TYPE_BOOLEAN);
            ProcessShaderParameter(srcNode, "mirrorV", placement, "mirrorV", AI_TYPE_BOOLEAN);
            ProcessShaderParameter(srcNode, "wrapU", placement, "wrapU", AI_TYPE_BOOLEAN);
            ProcessShaderParameter(srcNode, "wrapV", placement, "wrapV", AI_TYPE_BOOLEAN);
            ProcessShaderParameter(srcNode, "stagger", placement, "stagger", AI_TYPE_BOOLEAN);
            ProcessShaderParameter(srcNode, "repeatUV", placement, "repeatUV", AI_TYPE_POINT2);
            ProcessShaderParameter(srcNode, "rotateUV", placement, "rotateUV", AI_TYPE_FLOAT);
            ProcessShaderParameter(srcNode, "offset", placement, "offsetUV", AI_TYPE_POINT2);
            ProcessShaderParameter(srcNode, "noiseUV", placement, "noiseUV", AI_TYPE_POINT2);
         }
      }
      if (placement != NULL)
      {
         AiNodeSetBool(shader, "overrideUV", true);
         AiNodeLink(placement, "uv", shader);
      }
      else
      {
         AiNodeSetBool(shader, "overrideUV", false);
      }

      MObject opos = node.attribute("position");
      MObject ocol = node.attribute("color");
      plug = node.findPlug("colorEntryList");
      AtArray *positions = AiArrayAllocate(plug.numElements(), 1, AI_TYPE_FLOAT);
      AtArray *colors = AiArrayAllocate(plug.numElements(), 1, AI_TYPE_RGB);
      // Connections on individual array element are not handled
      for (unsigned int i=0; i<plug.numElements(); ++i)
      {
         elem = plug.elementByPhysicalIndex(i);
         pos = elem.child(opos);
         col = elem.child(ocol);
         AtRGB v;
         v.r = col.child(0).asFloat();
         v.g = col.child(1).asFloat();
         v.b = col.child(2).asFloat();
         AiArraySetFlt(positions, i, pos.asFloat());
         AiArraySetRGB(colors, i, v);
      }
      // Sort position array
      if (positions->nelements > 1)
      {
         AtUInt* shuffle = new AtUInt[positions->nelements];
         if (SortFloatArray(positions, shuffle))
         {
            ShuffleArray(colors, shuffle, AI_TYPE_RGB);
         }
         delete[] shuffle;
      }
      AiNodeSetArray(shader, "positions", positions);
      AiNodeSetArray(shader, "colors", colors);
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
