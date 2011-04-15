#include "Shaders.h"
#include "MayaScene.h"
#include "NodeTranslator.h"
#include "render/RenderOptions.h"
#include "render/RenderSession.h"

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
#include <maya/MFnAttribute.h>

#include <maya/MRenderUtil.h>

#include <string>
#include <cstring>


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

void CMayaScene::ProcessShaderParameter(MFnDependencyNode shader, const char* param, AtNode* arnoldShader, const char* arnoldAttrib, int arnoldAttribType, int element)
{
   MPlugArray connections;

   MPlug plug = shader.findPlug(param);
   if (element >= 0)
      plug = plug.elementByPhysicalIndex(element);
   plug.connectedTo(connections, true, false);
   if (connections.length() == 0)
   {
      switch(arnoldAttribType)
      {
      case AI_TYPE_RGB:
         {
            bool compConnected = false;
            for (unsigned int i=0; i < 3; i++)
            {
               plug.child(i).connectedTo(connections, true, false);
               if (connections.length() > 0)
               {
                  compConnected = true;
                  MString attrName = connections[0].partialName(false, false, false, false, false, true);
                  MString compAttrName(arnoldAttrib);
                  switch(i)
                  {
                  case 0:
                     compAttrName += ".r";
                     break;
                  case 1:
                     compAttrName += ".g";
                     break;
                  case 2:
                     compAttrName += ".b";
                     break;
                  }
                  AtNode* node = ExportShader(connections[0].node(), attrName);
                  if (node != NULL)
                     AiNodeLink(node, compAttrName.asChar(), arnoldShader);
               }
            }
            if (!compConnected)
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
               bool compConnected = false;
               for (unsigned int i=0; i < 3; i++)
               {
                  plug.child(i).connectedTo(connections, true, false);
                  if (connections.length() > 0)
                  {
                     compConnected = true;
                     MString attrName = connections[0].partialName(false, false, false, false, false, true);
                     MString compAttrName(arnoldAttrib);
                     switch(i)
                     {
                     case 0:
                        compAttrName += ".r";
                        break;
                     case 1:
                        compAttrName += ".g";
                        break;
                     case 2:
                        compAttrName += ".b";
                        break;
                     }
                     AtNode* node = ExportShader(connections[0].node(), attrName);
                     if (node != NULL)
                        AiNodeLink(node, compAttrName.asChar(), arnoldShader);
                  }
               }
               if (!compConnected)
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
            bool compConnected = false;
            for (unsigned int i=0; i < 3; i++)
            {
               plug.child(i).connectedTo(connections, true, false);
               if (connections.length() > 0)
               {
                  compConnected = true;
                  MString attrName = connections[0].partialName(false, false, false, false, false, true);
                  MString compAttrName(arnoldAttrib);
                  switch(i)
                  {
                  case 0:
                     compAttrName += ".x";
                     break;
                  case 1:
                     compAttrName += ".y";
                     break;
                  case 2:
                     compAttrName += ".z";
                     break;
                  }
                  AtNode* node = ExportShader(connections[0].node(), attrName);
                  if (node != NULL)
                     AiNodeLink(node, compAttrName.asChar(), arnoldShader);
               }
            }
            if (!compConnected)
               AiNodeSetVec(arnoldShader, arnoldAttrib, plug.child(0).asFloat(), plug.child(1).asFloat(), plug.child(2).asFloat());
         }
         break;
      case AI_TYPE_POINT:
         {
            bool compConnected = false;
            for (unsigned int i=0; i < 3; i++)
            {
               plug.child(i).connectedTo(connections, true, false);
               if (connections.length() > 0)
               {
                  compConnected = true;
                  MString attrName = connections[0].partialName(false, false, false, false, false, true);
                  MString compAttrName(arnoldAttrib);
                  switch(i)
                  {
                  case 0:
                     compAttrName += ".x";
                     break;
                  case 1:
                     compAttrName += ".y";
                     break;
                  case 2:
                     compAttrName += ".z";
                     break;
                  }
                  AtNode* node = ExportShader(connections[0].node(), attrName);
                  if (node != NULL)
                     AiNodeLink(node, compAttrName.asChar(), arnoldShader);
               }
            }
            if (!compConnected)
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

// Sky
//
const char* CSkyShaderTranslator::GetArnoldNodeType()
{
   return "sky";
}

void CSkyShaderTranslator::Export(AtNode* shader)
{
   // Maya's X Y and Z Vectors
   AiNodeSetVec(shader, "X", 1.0f, 0.0f, 0.0f);
   AiNodeSetVec(shader, "Y", 0.0f, 1.0f, 0.0f);
   AiNodeSetVec(shader, "Z", 0.0f, 0.0f, -1.0f);

   MDagPath nodeDagPath;
   m_fnNode.getPath(nodeDagPath);
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

   ProcessParameter(shader, "color", AI_TYPE_RGB);
   ProcessParameter(shader, "format", AI_TYPE_ENUM);
   ProcessParameter(shader, "intensity", AI_TYPE_FLOAT);

   AtInt visibility = ComputeVisibility();
   AiNodeSetBool(shader, "opaque_alpha", (AtInt)(visibility & AI_RAY_CAMERA));
   AiNodeSetInt(shader, "visibility", visibility);
}

// Lambert
//
const char* CLambertTranslator::GetArnoldNodeType()
{
   return "lambert";
}

void CLambertTranslator::Export(AtNode* shader)
{
   ProcessParameter(shader, "diffuse", "Kd", AI_TYPE_FLOAT);
   ProcessParameter(shader, "color", "Kd_color", AI_TYPE_RGB);
   ProcessParameter(shader, "outMatteOpacity", "opacity", AI_TYPE_RGB);

   MPlugArray connections;

   MPlug plug = m_fnNode.findPlug("normalCamera");

   plug.connectedTo(connections, true, false);
   if (connections.length() > 0)
   {
      MString m_outputAttr = connections[0].partialName(false, false, false, false, false, true);

      AtNode* m_fnNode = m_scene->ExportShader(connections[0].node(), m_outputAttr);

      if (m_fnNode != NULL)
         AiNodeLink(m_fnNode, "@before", shader);
   }
}

// SurfaceShader
//
const char* CSurfaceShaderTranslator::GetArnoldNodeType()
{
   return "flat";
}

void CSurfaceShaderTranslator::Export(AtNode* shader)
{
   ProcessParameter(shader, "outColor", "color", AI_TYPE_RGB);
   ProcessParameter(shader, "outMatteOpacity", "opacity", AI_TYPE_RGB);
}

// File
//
const char* CFileTranslator::GetArnoldNodeType()
{
   return "MayaFile";
}

void CFileTranslator::Export(AtNode* shader)
{
   MPlugArray connections;

   MPlug plug = m_fnNode.findPlug("uvCoord");

   plug.connectedTo(connections, true, false);

   if (connections.length() != 0)
   {
      MObject srcObj = connections[0].node();
      MFnDependencyNode srcNode(srcObj);

      if (srcNode.typeName() == "place2dTexture")
      {
         // until multiple outputs are supporte, place2d outputs are added to
         // inputs on the file node itself
         m_scene->ProcessShaderParameter(srcNode, "coverage", shader, "coverage", AI_TYPE_POINT2);
         m_scene->ProcessShaderParameter(srcNode, "rotateFrame", shader, "rotateFrame", AI_TYPE_FLOAT);
         m_scene->ProcessShaderParameter(srcNode, "translateFrame", shader, "translateFrame", AI_TYPE_POINT2);
         m_scene->ProcessShaderParameter(srcNode, "mirrorU", shader, "mirrorU", AI_TYPE_BOOLEAN);
         m_scene->ProcessShaderParameter(srcNode, "mirrorV", shader, "mirrorV", AI_TYPE_BOOLEAN);
         m_scene->ProcessShaderParameter(srcNode, "wrapU", shader, "wrapU", AI_TYPE_BOOLEAN);
         m_scene->ProcessShaderParameter(srcNode, "wrapV", shader, "wrapV", AI_TYPE_BOOLEAN);
         m_scene->ProcessShaderParameter(srcNode, "stagger", shader, "stagger", AI_TYPE_BOOLEAN);
         m_scene->ProcessShaderParameter(srcNode, "repeatUV", shader, "repeatUV", AI_TYPE_POINT2);
         m_scene->ProcessShaderParameter(srcNode, "rotateUV", shader, "rotateUV", AI_TYPE_FLOAT);
         m_scene->ProcessShaderParameter(srcNode, "offset", shader, "offsetUV", AI_TYPE_POINT2);
         m_scene->ProcessShaderParameter(srcNode, "noiseUV", shader, "noiseUV", AI_TYPE_POINT2);
      }
   }
   MString filename;
   MString resolvedFilename;
   MString frameNumber("0");
   MStatus status;
   frameNumber += m_scene->GetCurrentFrame() + m_fnNode.findPlug("frameOffset").asInt();
   MRenderUtil::exactFileTextureName(m_object, filename);
   resolvedFilename = MRenderUtil::exactFileTextureName(filename, m_fnNode.findPlug("useFrameExtension").asBool(), frameNumber, &status);
   if (status == MStatus::kSuccess)
   {
      resolvedFilename = filename;

      /*
      // FIXME : detect when to do this second resolution since it is ruining tokens
      // by returing an empty string while also returning kSuccess
      resolvedFilename = MRenderUtil::exactFileTextureName(filename, m_fnNode.findPlug("useFrameExtension").asBool(), frameNumber, &status);
      if (status != MStatus::kSuccess)
      {
         cout << "failed 2nd resolution" << endl;
         resolvedFilename = filename;
      }
      */
   }
   else
   {
      resolvedFilename = m_fnNode.findPlug("filename").asString();
   }
   AiNodeSetStr(shader, "filename", resolvedFilename.asChar());

   ProcessParameter(shader, "colorGain", AI_TYPE_RGB);
   ProcessParameter(shader, "colorOffset", AI_TYPE_RGB);
   ProcessParameter(shader, "alphaGain", AI_TYPE_FLOAT);
   ProcessParameter(shader, "alphaOffset", AI_TYPE_FLOAT);
   ProcessParameter(shader, "alphaIsLuminance", AI_TYPE_BOOLEAN);
   ProcessParameter(shader, "invert", AI_TYPE_BOOLEAN);
   ProcessParameter(shader, "defaultColor", AI_TYPE_RGB);
}

// Bump2d
//
const char* CBump2DTranslator::GetArnoldNodeType()
{
   return "bump2d";
}

void CBump2DTranslator::Export(AtNode* shader)
{
   ProcessParameter(shader, "bumpValue", "bump_map", AI_TYPE_FLOAT);
   ProcessParameter(shader, "bumpDepth", "bump_height", AI_TYPE_FLOAT);
}

// Bump3d
//
const char* CBump3DTranslator::GetArnoldNodeType()
{
   return "bump3d";
}

void CBump3DTranslator::Export(AtNode* shader)
{
   ProcessParameter(shader, "bumpValue", "bump_map", AI_TYPE_FLOAT);
   ProcessParameter(shader, "bumpDepth", "bump_height", AI_TYPE_FLOAT);
}

// SamplerInfo
//
const char* CSamplerInfoTranslator::GetArnoldNodeType()
{
   if (m_outputAttr == "facingRatio")
   {
      return "MayaFacingRatio";
   }
   else if (m_outputAttr == "flippedNormal")
   {
      return "MayaFlippedNormal";
   }
   else
      return "";
}

void CSamplerInfoTranslator::Export(AtNode* shader)
{}

// PlusMinusAverage
//
const char* CPlusMinusAverageTranslator::GetArnoldNodeType()
{
   if (m_outputAttr == "output1D")
   {
      return "MayaPlusMinusAverage1D";
   }
   else if (m_outputAttr == "output2D")
   {
      return "MayaPlusMinusAverage2D";
   }
   else if (m_outputAttr == "output3D")
   {
      return "MayaPlusMinusAverage3D";
   }
   else
      return "";
}

void CPlusMinusAverageTranslator::Export(AtNode* shader)
{
   if (m_outputAttr == "output1D")
   {
      MPlug attr, elem;
      MPlugArray connections;
      char mayaAttr[64];
      char aiAttr[64];

      attr = m_fnNode.findPlug("operation");
      AiNodeSetInt(shader, "operation", attr.asInt());

      attr = m_fnNode.findPlug("input1D");

      AtUInt numElements = attr.numElements();
      if (numElements > 8)
      {
         MGlobal::displayWarning("[mtoa] plusMinusAverage node has more than 8 inputs, only the first 8 will be handled");
         numElements = 8;
      }

      AiNodeSetUInt(shader, "numInputs", numElements);

      for (unsigned int i=0; i<numElements; ++i)
      {
         elem = attr.elementByPhysicalIndex(i);

         connections.clear();
         elem.connectedTo(connections, true, false);
         if (connections.length() > 0)
         {
            sprintf(mayaAttr, "inputs1D[%u]", elem.logicalIndex());
            sprintf(aiAttr, "value%u", i);
            ProcessParameter(shader, mayaAttr, aiAttr, AI_TYPE_FLOAT);
         }
      }
   }
   else if (m_outputAttr == "output2D")
   {
      MPlug attr, elem;
      MPlugArray connections;
      char mayaAttr[64];
      char aiAttr[64];

      attr = m_fnNode.findPlug("operation");
      AiNodeSetInt(shader, "operation", attr.asInt());

      attr = m_fnNode.findPlug("input2D");

      AtUInt numElements = attr.numElements();
      if (numElements > 8)
      {
         MGlobal::displayWarning("[mtoa] plusMinusAverage node has more than 8 inputs, only the first 8 will be handled");
         numElements = 8;
      }

      AiNodeSetUInt(shader, "numInputs", numElements);

      for (unsigned int i=0; i<numElements; ++i)
      {
         elem = attr.elementByPhysicalIndex(i);

         connections.clear();
         elem.connectedTo(connections, true, false);
         if (connections.length() > 0)
         {
            sprintf(mayaAttr, "inputs2D[%u]", elem.logicalIndex());
            sprintf(aiAttr, "value%u", i);
            ProcessParameter(shader, mayaAttr, aiAttr, AI_TYPE_POINT2);
         }
      }
   }
   else if (m_outputAttr == "output3D")
   {
      MPlug attr, elem;
      MPlugArray connections;
      char mayaAttr[64];
      char aiAttr[64];

      attr = m_fnNode.findPlug("operation");
      AiNodeSetInt(shader, "operation", attr.asInt());

      attr = m_fnNode.findPlug("input3D");

      AtUInt numElements = attr.numElements();
      if (numElements > 8)
      {
         MGlobal::displayWarning("[mtoa] plusMinusAverage node has more than 8 inputs, only the first 8 will be handled");
         numElements = 8;
      }

      AiNodeSetUInt(shader, "numInputs", numElements);

      for (unsigned int i=0; i<numElements; ++i)
      {
         elem = attr.elementByPhysicalIndex(i);

         connections.clear();
         elem.connectedTo(connections, true, false);
         if (connections.length() > 0)
         {
            sprintf(mayaAttr, "inputs3D[%u]", elem.logicalIndex());
            sprintf(aiAttr, "value%u", i);
            ProcessParameter(shader, mayaAttr, aiAttr, AI_TYPE_POINT);
         }
      }
   }
}

// RemapValue
//
const char* CRemapValueTranslator::GetArnoldNodeType()
{
   if (m_outputAttr == "outValue")
   {
      return "MayaRemapValueToValue";
   }
   else if (m_outputAttr == "outColor")
   {
      return "MayaRemapValueToColor";
   }
   else
      return "";
}

void CRemapValueTranslator::Export(AtNode* shader)
{
   static const char *remapInterpolationStrings[] =
   {
      "none",
      "linear",
      "smooth",
      "spline"
   };

   if (m_outputAttr == "outValue")
   {
      MPlug attr, elem, pos, val, interp;

      MObject opos = m_fnNode.attribute("value_Position");
      MObject oval = m_fnNode.attribute("value_FloatValue");
      MObject ointerp = m_fnNode.attribute("value_Interp");

      // FIXME: make inputValue the name of the parameter on the MayaRemapValue shader
      ProcessParameter(shader, "inputValue", "input", AI_TYPE_FLOAT);
      ProcessParameter(shader, "inputMin", AI_TYPE_FLOAT);
      ProcessParameter(shader, "inputMax", AI_TYPE_FLOAT);
      ProcessParameter(shader, "outputMin", AI_TYPE_FLOAT);
      ProcessParameter(shader, "outputMax", AI_TYPE_FLOAT);

      // Note: this doesn't handle connection coming in individual elements
      attr = m_fnNode.findPlug("value");
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
         AiArraySetStr(interps, i, remapInterpolationStrings[interp.asInt()]);
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
   else if (m_outputAttr == "outColor")
   {
      MPlug attr, elem, pos, val, interp;

      MObject opos = m_fnNode.attribute("color_Position");
      MObject oval = m_fnNode.attribute("color_Color");
      MObject ointerp = m_fnNode.attribute("color_Interp");

      // FIXME: make inputValue the name of the parameter on the MayaRemapValue shader
      ProcessParameter(shader, "inputValue", "input", AI_TYPE_FLOAT);
      ProcessParameter(shader, "inputMin", AI_TYPE_FLOAT);
      ProcessParameter(shader, "inputMax", AI_TYPE_FLOAT);
      ProcessParameter(shader, "outputMin", AI_TYPE_FLOAT);
      ProcessParameter(shader, "outputMax", AI_TYPE_FLOAT);

      // Note: this doesn't handle connection coming in individual elements
      attr = m_fnNode.findPlug("color");
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
         AiArraySetStr(interps, i, remapInterpolationStrings[interp.asInt()]);
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
}

// Remap Color
//
const char* CRemapColorTranslator::GetArnoldNodeType()
{
   if (m_outputAttr == "outColor")
   {
      //FIXME: missing AiNode()!
   }
   return "";
}

void CRemapColorTranslator::Export(AtNode* shader)
{
#if NULL // Disable: Arnold node does not exist
   if (m_outputAttr == "outColor")
   {
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

      // FIXME: change shader parameter name to match maya
      ProcessParameter(shader, "color", "input", AI_TYPE_RGB);
      ProcessParameter(shader, "inputMin", AI_TYPE_FLOAT);
      ProcessParameter(shader, "inputMax", AI_TYPE_FLOAT);
      ProcessParameter(shader, "outputMin", AI_TYPE_FLOAT);
      ProcessParameter(shader, "outputMax", AI_TYPE_FLOAT);

      for (int ci=0; ci<3; ++ci)
      {
         MObject opos = m_fnNode.attribute(posNames[ci*2]);
         MObject oval = m_fnNode.attribute(valNames[ci*2]);
         MObject ointerp = m_fnNode.attribute(interpNames[ci*2]);

         // Note: this doesn't handle connection coming in individual elements
         attr = m_fnNode.findPlug(plugNames[ci]);
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
            AiArraySetStr(interps, i, remapInterpolationStrings[interp.asInt()]);
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
#endif // NULL
}

// Projection
//
const char* CProjectionTranslator::GetArnoldNodeType()
{
   return "MayaProjection";
}

void CProjectionTranslator::Export(AtNode* shader)
{
   // FIXME: change shader parameter name to match maya
   ProcessParameter(shader, "projType", "type", AI_TYPE_INT);
   ProcessParameter(shader, "uAngle", AI_TYPE_FLOAT);
   ProcessParameter(shader, "vAngle", AI_TYPE_FLOAT);
   ProcessParameter(shader, "local", AI_TYPE_BOOLEAN);
   ProcessParameter(shader, "wrap", AI_TYPE_BOOLEAN);
   ProcessParameter(shader, "invert", AI_TYPE_BOOLEAN);
   ProcessParameter(shader, "colorGain", AI_TYPE_RGB);
   ProcessParameter(shader, "colorOffset", AI_TYPE_RGB);
   ProcessParameter(shader, "defaultColor", AI_TYPE_RGB);
   ProcessParameter(shader, "alphaGain", AI_TYPE_FLOAT);
   ProcessParameter(shader, "alphaOffset", AI_TYPE_FLOAT);
   ProcessParameter(shader, "image", AI_TYPE_RGBA);

   // alphaIsLuminance?

   // shaderMatrix
   MPlug plug = m_fnNode.findPlug("shaderMatrix");
   // should follow connections here also
   // temporarily just read the value
   AtMatrix ipm;
   MObject matObj = plug.asMObject();
   MFnMatrixData matData(matObj);
   MMatrix mm = matData.matrix();
   ConvertMatrix(ipm, mm);
   AiNodeSetMatrix(shader, "mappingCoordinate", ipm);

   ProcessParameter(shader, "fitType", AI_TYPE_INT);
   // FIXME: change shader parameter name to match maya
   ProcessParameter(shader, "fitFill", "fillType", AI_TYPE_INT);

   MPlug typePlug = m_fnNode.findPlug("projType");
   plug = m_fnNode.findPlug("linkedCamera");
   MPlugArray connections;
   plug.connectedTo(connections, true, false);
   if (connections.length() >= 1 && typePlug.asInt() == 8)
   {
      MObject camObj = connections[0].node();

      MFnCamera cam(camObj);

      MDagPath camPath;
      cam.getPath(camPath);

      AiNodeSetStr(shader, "cameraName", cam.name().asChar());
      AiNodeSetFlt(shader, "cameraNearPlane", static_cast<float>(cam.nearClippingPlane()));
      AiNodeSetFlt(shader, "cameraHorizontalFOV", static_cast<float>(cam.horizontalFieldOfView()));
      AiNodeSetFlt(shader, "cameraAspectRatio", static_cast<float>(cam.aspectRatio()));
   }
   else
   {
      // no linked camera, fit type needs to be None ?
      AiNodeSetInt(shader, "fitType", 0);
   }
}

// Ramp
//
const char* CRampTranslator::GetArnoldNodeType()
{
   return "MayaRamp";
}

void CRampTranslator::Export(AtNode* shader)
{
   ProcessParameter(shader, "type", AI_TYPE_INT);
   ProcessParameter(shader, "interpolation", AI_TYPE_INT);
   ProcessParameter(shader, "uWave", AI_TYPE_FLOAT);
   ProcessParameter(shader, "vWave", AI_TYPE_FLOAT);
   ProcessParameter(shader, "noise", AI_TYPE_FLOAT);
   ProcessParameter(shader, "noiseFreq", AI_TYPE_FLOAT);
   ProcessParameter(shader, "colorGain", AI_TYPE_RGB);
   ProcessParameter(shader, "colorOffset", AI_TYPE_RGB);
   ProcessParameter(shader, "defaultColor", AI_TYPE_RGB);
   ProcessParameter(shader, "alphaGain", AI_TYPE_FLOAT);
   ProcessParameter(shader, "alphaOffset", AI_TYPE_FLOAT);
   ProcessParameter(shader, "invert", AI_TYPE_BOOLEAN);
   ProcessParameter(shader, "uvCoord", AI_TYPE_POINT2);

   MPlug plug, elem, pos, col;
   MPlugArray connections;

   MObject opos = m_fnNode.attribute("position");
   MObject ocol = m_fnNode.attribute("color");
   plug = m_fnNode.findPlug("colorEntryList");
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

// Place2DTexture

const char* CPlace2DTextureTranslator::GetArnoldNodeType()
{
   return "MayaPlace2DTexture";
}

void CPlace2DTextureTranslator::Export(AtNode* shader)
{
   ProcessParameter(shader, "coverage", AI_TYPE_POINT2);
   ProcessParameter(shader, "rotateFrame", AI_TYPE_FLOAT);
   ProcessParameter(shader, "translateFrame", AI_TYPE_POINT2);
   ProcessParameter(shader, "mirrorU", AI_TYPE_BOOLEAN);
   ProcessParameter(shader, "mirrorV", AI_TYPE_BOOLEAN);
   ProcessParameter(shader, "wrapU", AI_TYPE_BOOLEAN);
   ProcessParameter(shader, "wrapV", AI_TYPE_BOOLEAN);
   ProcessParameter(shader, "stagger", AI_TYPE_BOOLEAN);
   ProcessParameter(shader, "repeatUV", AI_TYPE_POINT2);
   ProcessParameter(shader, "rotateUV", AI_TYPE_FLOAT);
   ProcessParameter(shader, "offset", "offsetUV", AI_TYPE_POINT2);
   ProcessParameter(shader, "noiseUV", AI_TYPE_POINT2);
}

// LayeredTexture
//
const char* CLayeredTextureTranslator::GetArnoldNodeType()
{
   return "MayaLayeredTexture";
}

void CLayeredTextureTranslator::Export(AtNode* shader)
{
   MPlug attr, elem, color, alpha, blendMode, isVisible;
   MPlugArray connections;
   MObject colorSrc, alphaSrc;
   bool colorConnectedToAlpha;
   char mayaAttr[64];
   char aiAttr[64];

   attr = m_fnNode.findPlug("inputs");
   AtUInt numElements = attr.numElements();
   if (numElements > 8)
   {
      MGlobal::displayWarning("[mtoa] layeredTexture node has more than 8 inputs, only the first 8 will be handled");
      numElements = 8;
   }

   AiNodeSetUInt(shader, "numInputs", numElements);

   MObject colorAttr = m_fnNode.attribute("color");
   MObject alphaAttr = m_fnNode.attribute("alpha");
   MObject blendModeAttr = m_fnNode.attribute("blendMode");
   MObject isVisibleAttr = m_fnNode.attribute("isVisible");

   for (AtUInt i = 0; i < numElements; ++i)
   {
      elem = attr.elementByPhysicalIndex(i);

      color = elem.child(colorAttr);
      alpha = elem.child(alphaAttr);
      blendMode = elem.child(blendModeAttr);
      isVisible = elem.child(isVisibleAttr);

      sprintf(mayaAttr, "inputs[%u].color", elem.logicalIndex());
      sprintf(aiAttr, "color%u", i);
      ProcessParameter(shader, mayaAttr, aiAttr, AI_TYPE_RGBA);

      // Alpha connection is only handled when 
      // The input in color and alpha is the same

      colorSrc = MObject::kNullObj;
      alphaSrc = MObject::kNullObj;

      color.connectedTo(connections, true, false);
      if (connections.length() > 0)
         colorSrc = connections[0].node();

      connections.clear();
      alpha.connectedTo(connections, true, false);
      if (connections.length() > 0)
         alphaSrc = connections[0].node();

      if (alphaSrc.isNull())
         colorConnectedToAlpha = false;
      else
         colorConnectedToAlpha = (colorSrc == alphaSrc);

      sprintf(aiAttr, "colorConnectedToAlpha%u", i);
      AiNodeSetBool(shader, aiAttr, colorConnectedToAlpha ? TRUE : FALSE);

      if (!colorConnectedToAlpha && alphaSrc.isNull())
      {
         // Export alpha value when it's not connected

         sprintf(aiAttr, "alpha%u", i);
         AiNodeSetFlt(shader, aiAttr, alpha.asFloat());
      }

      sprintf(mayaAttr, "inputs[%u].blendMode", elem.logicalIndex());
      sprintf(aiAttr, "blendMode%u", i);
      ProcessParameter(shader, mayaAttr, aiAttr, AI_TYPE_ENUM);

      sprintf(mayaAttr, "inputs[%u].isVisible", elem.logicalIndex());
      sprintf(aiAttr, "visible%u", i);
      ProcessParameter(shader, mayaAttr, aiAttr, AI_TYPE_BOOLEAN);
   }
}

// LayeredShader
//
const char* CLayeredShaderTranslator::GetArnoldNodeType()
{
   return "MayaLayeredShader";
}

void CLayeredShaderTranslator::Export(AtNode* shader)
{
   MPlug attr, elem, color, transp;
   MPlugArray connections;
   MObject colorSrc, transpSrc;
   bool useTransparency;
   char mayaAttr[64];
   char aiAttr[64];

   ProcessParameter(shader, "compositingFlag", AI_TYPE_ENUM);

   attr = m_fnNode.findPlug("inputs");
   AtUInt numElements = attr.numElements();
   if (numElements > 8)
   {
      MGlobal::displayWarning("[mtoa] layeredShader node has more than 8 inputs, only the first 8 will be handled");
      numElements = 8;
   }

   AiNodeSetUInt(shader, "numInputs", numElements);

   MObject colorAttr = m_fnNode.attribute("color");
   MObject transpAttr = m_fnNode.attribute("transparency");

   for (AtUInt i = 0; i < numElements; ++i)
   {
      elem = attr.elementByPhysicalIndex(i);

      color = elem.child(colorAttr);
      transp = elem.child(transpAttr);

      colorSrc = MObject::kNullObj;
      transpSrc = MObject::kNullObj;

      connections.clear();
      color.connectedTo(connections, true, false);
      if (connections.length() > 0)
         colorSrc = connections[0].node();

      connections.clear();
      transp.connectedTo(connections, true, false);
      if (connections.length() > 0)
         transpSrc = connections[0].node();

      if (transpSrc.isNull())
         useTransparency = true;
      else
         useTransparency = (colorSrc != transpSrc);

      sprintf(mayaAttr, "inputs[%u].color", elem.logicalIndex());
      sprintf(aiAttr, "color%u", i);
      ProcessParameter(shader, mayaAttr, aiAttr, AI_TYPE_RGB);

      sprintf(aiAttr, "useTransparency%u", i);
      AiNodeSetBool(shader, aiAttr, useTransparency ? TRUE : FALSE);

      if (useTransparency)
      {
         sprintf(mayaAttr, "inputs[%u].transparency", elem.logicalIndex());
         sprintf(aiAttr, "transparency%u", i);
         ProcessParameter(shader, mayaAttr, aiAttr, AI_TYPE_RGB);
      }
   }
}
