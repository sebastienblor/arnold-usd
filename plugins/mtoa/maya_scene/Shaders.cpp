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

#include <maya/MRenderUtil.h>

#include <string>
#include <cstring>

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

AtNode* CMayaScene::ExportArnoldShader(MObject mayaShader, MString arnoldShader)
{
   AtNode* shader = NULL;
   MFnDependencyNode node(mayaShader);

   const AtNodeEntry* nodeEntry = AiNodeEntryLookUp(arnoldShader.asChar());

   if (nodeEntry != NULL)
   {
      AtNode* shader = AiNode(arnoldShader.asChar());

      AiNodeSetStr(shader, "name", node.name().asChar());

      AtInt numParams = AiNodeEntryGetNumParams(nodeEntry);

      for (AtInt J = 0; (J < numParams); ++J)
      {
         const AtParamEntry* paramEntry = AiNodeEntryGetParameter(nodeEntry, J);
      
         if (!strncmp(AiParamGetName(paramEntry), "aov_", 4))
         {
            CRenderSession *renderSession = CRenderSession::GetInstance();
            const CRenderOptions *renderOptions = renderSession->RenderOptions();

            // do not check type for now
            MString aovName = AiParamGetName(paramEntry) + 4;

            if (renderOptions->FindAOV(aovName) != size_t(-1))
            {
               AiNodeSetStr(shader, AiParamGetName(paramEntry), aovName.asChar());
            }
            else
            {
               AiNodeSetStr(shader, AiParamGetName(paramEntry), "");
            }
         }
         else if (strcmp(AiParamGetName(paramEntry), "name"))
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

   AtInt nodeId = node.typeId().id();
   std::map<int, CreatorFunction>::iterator dependTransIt = s_dependTranslators.find(nodeId);
   if (dependTransIt != s_dependTranslators.end())
   {
      if (mayaShader.hasFn(MFn::kDagNode))
      {
         CDagTranslator* translator;
         MDagPath dagPath;
         MDagPath::getAPathTo(mayaShader, dagPath);
         translator = (CDagTranslator*)dependTransIt->second();
         translator->Init(dagPath, this, attrName);
         // FIXME: currently shaders are only exported for step = 0
         shader = translator->DoExport(0);
         m_processedTranslators[MObjectHandle(mayaShader)] = translator;
      }
      else
      {
         CNodeTranslator* translator;
         translator = (CNodeTranslator*)dependTransIt->second();
         translator->Init(mayaShader, this, attrName);
         // FIXME: currently shaders are only exported for step = 0
         shader = translator->DoExport(0);
         m_processedTranslators[MObjectHandle(mayaShader)] = translator;
      }
   }
   else
      AiMsgWarning("[mtoa] Shader type not supported: %s", node.typeName().asChar());

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

// Sky
//
AtNode* CSkyShaderTranslator::Export()
{
   AtNode* shader = AiNode("sky");
   AiNodeSetStr(shader, "name", m_fnNode.name().asChar());
   Update(shader);
   return shader;
}

void CSkyShaderTranslator::Update(AtNode* shader)
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
AtNode* CLambertTranslator::Export()
{
   AtNode* shader = AiNode("lambert");
   AiNodeSetStr(shader, "name", m_fnNode.name().asChar());
   Update(shader);
   return shader;
}

void CLambertTranslator::Update(AtNode* shader)
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
AtNode* CSurfaceShaderTranslator::Export()
{
   AtNode* shader = AiNode("flat");
   AiNodeSetStr(shader, "name", m_fnNode.name().asChar());
   Update(shader);
   return shader;
}

void CSurfaceShaderTranslator::Update(AtNode* shader)
{
   ProcessParameter(shader, "outColor", "color", AI_TYPE_RGB);
   ProcessParameter(shader, "outMatteOpacity", "opacity", AI_TYPE_RGB);
}

// File
//
AtNode* CFileTranslator::Export()
{
   AtNode* shader = AiNode("MayaFile");
   AiNodeSetStr(shader, "name", m_fnNode.name().asChar());
   Update(shader);
   return shader;
}

void CFileTranslator::Update(AtNode* shader)
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
AtNode* CBump2dTranslator::Export()
{
   AtNode* shader = AiNode("bump2d");
   AiNodeSetStr(shader, "name", m_fnNode.name().asChar());
   Update(shader);
   return shader;
}

void CBump2dTranslator::Update(AtNode* shader)
{
   ProcessParameter(shader, "bumpValue", "bump_map", AI_TYPE_FLOAT);
   ProcessParameter(shader, "bumpDepth", "bump_height", AI_TYPE_FLOAT);
}

// Bump3d
//
AtNode* CBump3dTranslator::Export()
{
   AtNode* shader = AiNode("bump3d");
   AiNodeSetStr(shader, "name", m_fnNode.name().asChar());
   Update(shader);
   return shader;
}

void CBump3dTranslator::Update(AtNode* shader)
{
   ProcessParameter(shader, "bumpValue", "bump_map", AI_TYPE_FLOAT);
   ProcessParameter(shader, "bumpDepth", "bump_height", AI_TYPE_FLOAT);
}

// SamplerInfo
//
AtNode* CSamplerInfoTranslator::Export()
{
   AtNode* shader = NULL;
   if (m_outputAttr == "facingRatio")
   {
      shader = AiNode("MayaFacingRatio");
   }
   else if (m_outputAttr == "flippedNormal")
   {
      shader = AiNode("MayaFlippedNormal");
   }
   if (shader != NULL)
   {
      MString name = m_fnNode.name() + "_" + m_outputAttr;
      AiNodeSetStr(shader, "name", name.asChar());
   }

   return shader;
}

void CSamplerInfoTranslator::Update(AtNode* shader)
{}

// PlusMinusAverage
//
AtNode* CPlusMinusAverageTranslator::Export()
{
   AtNode* shader = NULL;
   if (m_outputAttr == "output1D")
   {
      shader = AiNode("MayaPlusMinusAverage1D");
   }
   else if (m_outputAttr == "output2D")
   {
      shader = AiNode("MayaPlusMinusAverage2D");
   }
   else if (m_outputAttr == "output3D")
   {
      shader = AiNode("MayaPlusMinusAverage3D");
   }
   if (shader != NULL)
   {
      MString name = m_fnNode.name() + "_" + m_outputAttr;
      AiNodeSetStr(shader, "name", name.asChar());
      Update(shader);
   }

   return shader;
}

void CPlusMinusAverageTranslator::Update(AtNode* shader)
{
   if (m_outputAttr == "output1D")
   {
      MPlug elem, attr;

      attr = m_fnNode.findPlug("operation");
      AiNodeSetInt(shader, "operation", attr.asInt());

      attr = m_fnNode.findPlug("input1D");
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
   else if (m_outputAttr == "output2D")
   {
      MObject oinx = m_fnNode.attribute("input2Dx");
      MObject oiny = m_fnNode.attribute("input2Dy");

      MPlug attr, elem, inx, iny;
      AtPoint2 value;

      attr = m_fnNode.findPlug("operation");
      AiNodeSetInt(shader, "operation", attr.asInt());

      attr = m_fnNode.findPlug("input2D");
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
   else if (m_outputAttr == "output3D")
   {
      MObject oinx = m_fnNode.attribute("input3Dx");
      MObject oiny = m_fnNode.attribute("input3Dy");
      MObject oinz = m_fnNode.attribute("input3Dz");

      MPlug attr, elem, inx, iny, inz;
      AtRGB value;

      attr = m_fnNode.findPlug("operation");
      AiNodeSetInt(shader, "operation", attr.asInt());

      attr = m_fnNode.findPlug("input3D");
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
}

// RemapValue
//
AtNode* CRemapValueTranslator::Export()
{
   AtNode* shader = NULL;
   if (m_outputAttr == "outValue")
   {
      shader = AiNode("MayaRemapValueToValue");
   }
   else if (m_outputAttr == "outColor")
   {
      shader = AiNode("MayaRemapValueToColor");
   }
   if (shader != NULL)
   {
      MString name = m_fnNode.name() + "_" + m_outputAttr;
      AiNodeSetStr(shader, "name", name.asChar());
      Update(shader);
   }

   return shader;
}

void CRemapValueTranslator::Update(AtNode* shader)
{
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
}

// Remap Color
//
AtNode* CRemapColorTranslator::Export()
{
   AtNode* shader = NULL;
   if (m_outputAttr == "outColor")
   {
      //FIXME: missing AiNode()!
   }
   if (shader != NULL)
   {
      MString name = m_fnNode.name() + "_" + m_outputAttr;
      AiNodeSetStr(shader, "name", name.asChar());
      Update(shader);
   }

   return shader;
}

void CRemapColorTranslator::Update(AtNode* shader)
{
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
}

// Projection
//
AtNode* CProjectionTranslator::Export()
{
   AtNode* shader = AiNode("MayaProjection");
   AiNodeSetStr(shader, "name", m_fnNode.name().asChar());
   Update(shader);
   return shader;
}

void CProjectionTranslator::Update(AtNode* shader)
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
AtNode* CRampTranslator::Export()
{
   AtNode* shader = AiNode("MayaRamp");
   AiNodeSetStr(shader, "name", m_fnNode.name().asChar());
   Update(shader);
   return shader;
}

void CRampTranslator::Update(AtNode* shader)
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

   MPlug plug, elem, pos, col;
   MPlugArray connections;

   if (ProcessParameter(shader, "uv", AI_TYPE_POINT2) != NULL)
   {
      AiNodeSetBool(shader, "overrideUV", true);
   }
   else
   {
      AiNodeSetBool(shader, "overrideUV", false);
   }
   /*
    * AtNode *shader = NULL;
   plug = m_fnNode.findPlug("uvCoord");
   plug.connectedTo(connections, true, false);

   if (connections.length() != 0)
   {
      MObject srcObj = connections[0].node();
      MFnDependencyNode srcNode(srcObj);

      if (srcNode.typeName() == "place2dTexture")
      {
         shader = AiNode("MayaPlace2dTexture");

         AiNodeSetStr(shader, "name", srcNode.name().asChar());
         ProcessParameter("coverage", shader, "coverage", AI_TYPE_POINT2);
         ProcessParameter("rotateFrame", shader, "rotateFrame", AI_TYPE_FLOAT);
         ProcessParameter("translateFrame", shader, "translateFrame", AI_TYPE_POINT2);
         ProcessParameter("mirrorU", shader, "mirrorU", AI_TYPE_BOOLEAN);
         ProcessParameter("mirrorV", shader, "mirrorV", AI_TYPE_BOOLEAN);
         ProcessParameter("wrapU", shader, "wrapU", AI_TYPE_BOOLEAN);
         ProcessParameter("wrapV", shader, "wrapV", AI_TYPE_BOOLEAN);
         ProcessParameter("stagger", shader, "stagger", AI_TYPE_BOOLEAN);
         ProcessParameter("repeatUV", shader, "repeatUV", AI_TYPE_POINT2);
         ProcessParameter("rotateUV", shader, "rotateUV", AI_TYPE_FLOAT);
         ProcessParameter("offset", shader, "offsetUV", AI_TYPE_POINT2);
         ProcessParameter("noiseUV", shader, "noiseUV", AI_TYPE_POINT2);
      }
   }
   if (shader != NULL)
   {
      AiNodeSetBool(shader, "overrideUV", true);
      AiNodeLink(shader, "uv", shader);
   }
   else
   {
      AiNodeSetBool(shader, "overrideUV", false);
   }
   */

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

AtNode* CPlace2dTextureTranslator::Export()
{
   AtNode* shader = AiNode("MayaPlace2dTexture");
   AiNodeSetStr(shader, "name", m_fnNode.name().asChar());
   Update(shader);
   return shader;
}

void CPlace2dTextureTranslator::Update(AtNode* shader)
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
   ProcessParameter(shader, "offsetUV", AI_TYPE_POINT2);
   ProcessParameter(shader, "noiseUV", AI_TYPE_POINT2);
}
