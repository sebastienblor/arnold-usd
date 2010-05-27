
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

#include <string>


static const char *g_remapInterpolationStrings[] =
{
   "none",
   "linear",
   "smooth",
   "spline"
};

static bool SortFloatArray(AtArray *a, int *shuffle=0)
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
         for (int i=0; i<n; ++i)
         {
            shuffle[i] = i;
         }
      }

      while (swapped)
      {
         swapped = false;
         n -= 1;
         for (AtUInt32 i=0; i<n; ++i)
         {
            p0 = AiArrayGetFlt(a, i);
            p1 = AiArrayGetFlt(a, i+1);
            if (p0 > p1)
            {
               swapped = true;
               modified = true;

               AiArraySetFlt(a, i, p1);
               AiArraySetFlt(a, i+1, p0);

               if (shuffle)
               {
                  tmp = shuffle[i];
                  shuffle[i] = shuffle[i+1];
                  shuffle[i+1] = tmp;
               }
            }
         }
      }
   }
   
   return modified;
}

static void ShuffleArray(AtArray *a, int *shuffle, int arnoldType)
{
   if (!a || !shuffle)
   {
      return;
   }

   if (arnoldType == AI_TYPE_FLOAT)
   {
      float tmp;
      for (int i=0; i<a->nelements; ++i)
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
      for (int i=0; i<a->nelements; ++i)
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
      for (int i=0; i<a->nelements; ++i)
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
      for (int i=0; i<a->nelements; ++i)
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
      for (int i=0; i<a->nelements; ++i)
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
      for (int i=0; i<a->nelements; ++i)
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
      for (int i=0; i<a->nelements; ++i)
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
      for (int i=0; i<a->nelements; ++i)
      {
         if (shuffle[i] < i)
            continue;
         tmp = AiArrayGetBool(a, i);
         AiArraySetBool(a, i, AiArrayGetBool(a, shuffle[i]));
         AiArraySetBool(a, shuffle[i], tmp);
      }
   }
   else if (arnoldType == AI_TYPE_VECTOR)
   {
      AtVector tmp;
      for (int i=0; i<a->nelements; ++i)
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
      for (int i=0; i<a->nelements; ++i)
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
      for (int i=0; i<a->nelements; ++i)
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
      for (int i=0; i<a->nelements; ++i)
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
      for (int i=0; i<a->nelements; ++i)
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
      for (int i=0; i<a->nelements; ++i)
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
            int *shuffle = new int[positions->nelements];
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
            int *shuffle = new int[positions->nelements];
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
