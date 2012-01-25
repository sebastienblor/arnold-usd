#include "ShaderTranslators.h"
#include "scene/MayaScene.h"
#include "render/RenderOptions.h"
#include "render/RenderSession.h"

#include <ai_msg.h>
#include <ai_nodes.h>
#include <ai_ray.h>

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

#include <maya/MColor.h>
#include <maya/MTransformationMatrix.h>
#include <maya/MEulerRotation.h>
#include <maya/MAngle.h>

#include <maya/MRenderUtil.h>

#include <string>
#include <fstream>

static bool SortFloatArray(AtArray *a, unsigned int *shuffle=NULL)
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

static void ShuffleArray(AtArray *a, unsigned int *shuffle, int arnoldType)
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
      int tmp;
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
      unsigned int tmp;
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
      int tmp;
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

// Sky
//
AtNode*  CSkyShaderTranslator::CreateArnoldNodes()
{
   return AddArnoldNode("sky");
}

void CSkyShaderTranslator::Export(AtNode* shader)
{


   MFnDependencyNode trNode(m_dagPath.transform());

   MTransformationMatrix tmatrix(m_dagPath.inclusiveMatrix());
   MEulerRotation erotate = tmatrix.eulerRotation();
   AiNodeSetFlt(shader, "X_angle", static_cast<float>(-MAngle(erotate[0]).asDegrees()));
   AiNodeSetFlt(shader, "Y_angle", static_cast<float>(MAngle(erotate[1]).asDegrees()));
   AiNodeSetFlt(shader, "Z_angle", static_cast<float>(-MAngle(erotate[2]).asDegrees()));

   double scale[3];
   tmatrix.getScale(scale, MSpace::kTransform);
   AiMsgDebug("Sky scale %f %f %f", scale[0], scale[1], scale[2]);
   // Invert in Z to account for the env sphere being viewed from inside
   AiNodeSetVec(shader, "X", 1.0f/static_cast<float>(scale[0]), 0.0f, 0.0f);
   AiNodeSetVec(shader, "Y", 0.0f, 1.0f/static_cast<float>(scale[1]), 0.0f);
   AiNodeSetVec(shader, "Z", 0.0f, 0.0f, -1.0f/static_cast<float>(scale[2]));

   ProcessParameter(shader, "color",     AI_TYPE_RGB);
   ProcessParameter(shader, "format",    AI_TYPE_ENUM);
   ProcessParameter(shader, "intensity", AI_TYPE_FLOAT);

   int visibility = ComputeVisibility();
   AiNodeSetBool(shader, "opaque_alpha", (int)(visibility & AI_RAY_CAMERA));
   AiNodeSetInt(shader, "visibility", visibility);
}

// Lambert
//
AtNode*  CLambertTranslator::CreateArnoldNodes()
{
   return AddArnoldNode("lambert");
}

void CLambertTranslator::Export(AtNode* shader)
{
   // FIXME: correct metadata for these 3 parameters would make a custom translator unnecessary
   ProcessParameter(shader, "Kd", AI_TYPE_FLOAT, FindMayaObjectPlug("diffuse"));
   ProcessParameter(shader, "Kd_color", AI_TYPE_RGB, FindMayaObjectPlug("color"));
   ProcessParameter(shader, "opacity", AI_TYPE_RGB, FindMayaObjectPlug("outMatteOpacity"));

   MPlugArray connections;

   MPlug plug = FindMayaObjectPlug("normalCamera");

   plug.connectedTo(connections, true, false);
   if (connections.length() > 0)
   {
      AtNode* m_fnNode = ExportNode(connections[0]);

      if (m_fnNode != NULL)
         AiNodeLink(m_fnNode, "@before", shader);
   }
}

// File
//
AtNode*  CFileTranslator::CreateArnoldNodes()
{
   return AddArnoldNode("MayaFile");
}

void CFileTranslator::Export(AtNode* shader)
{
   MPlugArray connections;

   MPlug plug = FindMayaObjectPlug("uvCoord");

   plug.connectedTo(connections, true, false);

   if (connections.length() != 0)
   {
      MObject srcObj = connections[0].node();
      MFnDependencyNode srcNodeFn(srcObj);
      if (srcNodeFn.typeName() == "place2dTexture")
      {
         // until multiple outputs are supporte, place2d outputs are added to
         // inputs on the file node itself
         // FIXME do this with a translator
         ProcessParameter(shader, "coverage", AI_TYPE_POINT2, srcNodeFn.findPlug("coverage"));
         ProcessParameter(shader, "rotateFrame", AI_TYPE_FLOAT, srcNodeFn.findPlug("rotateFrame"));
         ProcessParameter(shader, "translateFrame", AI_TYPE_POINT2, srcNodeFn.findPlug("translateFrame"));
         ProcessParameter(shader, "mirrorU", AI_TYPE_BOOLEAN, srcNodeFn.findPlug("mirrorU"));
         ProcessParameter(shader, "mirrorV", AI_TYPE_BOOLEAN, srcNodeFn.findPlug("mirrorV"));
         ProcessParameter(shader, "wrapU", AI_TYPE_BOOLEAN, srcNodeFn.findPlug("wrapU"));
         ProcessParameter(shader, "wrapV", AI_TYPE_BOOLEAN, srcNodeFn.findPlug("wrapV"));
         ProcessParameter(shader, "stagger", AI_TYPE_BOOLEAN, srcNodeFn.findPlug("stagger"));
         ProcessParameter(shader, "repeatUV", AI_TYPE_POINT2, srcNodeFn.findPlug("repeatUV"));
         ProcessParameter(shader, "rotateUV", AI_TYPE_FLOAT, srcNodeFn.findPlug("rotateUV"));
         ProcessParameter(shader, "offsetUV", AI_TYPE_POINT2, srcNodeFn.findPlug("offset"));
         ProcessParameter(shader, "noiseUV", AI_TYPE_POINT2, srcNodeFn.findPlug("noiseUV"));
      }
   }
   
   if (NULL == ProcessParameter(shader, "filename", AI_TYPE_STRING, FindMayaObjectPlug("fileTextureName")))
   {
      MString filename; 
      MString resolvedFilename; 
      MString frameNumber("0"); 
      MStatus status; 
      frameNumber += GetExportFrame() + FindMayaObjectPlug("frameOffset").asInt(); 
      MRenderUtil::exactFileTextureName(m_object, filename); 
      resolvedFilename = MRenderUtil::exactFileTextureName(filename, FindMayaObjectPlug("useFrameExtension").asBool(), frameNumber, &status); 
      if (status == MStatus::kSuccess) 
      { 
         // Cancels above resolution since it ruins tokens
         resolvedFilename = filename; 
      } 
      else 
      { 
         resolvedFilename = FindMayaObjectPlug("fileTextureName").asString(); 
      } 
      
      // FIXME really inconvenient, a CRenderOptions instance should be stored in session 
      // or that class eliminated completely 
      CRenderOptions renderOptions; 
      renderOptions.SetArnoldRenderOptions(GetArnoldRenderOptions()); 
      renderOptions.GetFromMaya(); 
      if(renderOptions.useExistingTiledTextures()) 
      { 
         MString tx_filename(resolvedFilename.substring(0, resolvedFilename.rindexW(".")) + MString("tx")); 
         std::ifstream ifile(tx_filename.asChar()); 
         if(ifile.is_open()) 
         { 
            resolvedFilename = tx_filename; 
         } 
      }
      
      AiNodeSetStr(shader, "filename", resolvedFilename.asChar()); 
   }


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
AtNode*  CBump2DTranslator::CreateArnoldNodes()
{
   return AddArnoldNode("bump2d");
}

void CBump2DTranslator::Export(AtNode* shader)
{
   ProcessParameter(shader, "bump_map", AI_TYPE_FLOAT, FindMayaObjectPlug("bumpValue"));
   ProcessParameter(shader, "bump_height", AI_TYPE_FLOAT, FindMayaObjectPlug("bumpDepth"));
}

// Bump3d
//
AtNode*  CBump3DTranslator::CreateArnoldNodes()
{
   return AddArnoldNode("bump3d");
}

void CBump3DTranslator::Export(AtNode* shader)
{
   ProcessParameter(shader, "bump_map", AI_TYPE_FLOAT, FindMayaObjectPlug("bumpValue"));
   ProcessParameter(shader, "bump_height", AI_TYPE_FLOAT, FindMayaObjectPlug("bumpDepth"));
}

// SamplerInfo
//
AtNode* CSamplerInfoTranslator::CreateArnoldNodes()
{
   if (m_outputAttr == "facingRatio")
   {
      return AddArnoldNode("MayaFacingRatio");
   }
   else if (m_outputAttr == "flippedNormal")
   {
      return AddArnoldNode("MayaFlippedNormal");
   }
   else
      return NULL;
}

void CSamplerInfoTranslator::Export(AtNode* shader)
{}

// PlusMinusAverage
//
AtNode* CPlusMinusAverageTranslator::CreateArnoldNodes()
{
   if (m_outputAttr == "output1D")
   {
      return AddArnoldNode("MayaPlusMinusAverage1D");
   }
   else if (m_outputAttr == "output2D")
   {
      return AddArnoldNode("MayaPlusMinusAverage2D");
   }
   else if (m_outputAttr == "output3D")
   {
      return AddArnoldNode("MayaPlusMinusAverage3D");
   }
   else
      return NULL;
}

void CPlusMinusAverageTranslator::Export(AtNode* shader)
{
   CShaderTranslator::Export(shader);
}

// ParticleSamplerInfo
//
AtNode* CParticleSamplerInfoTranslator::CreateArnoldNodes()
{
   if (
         m_outputAttr == "outColor" ||
         m_outputAttr == "outIncandescence" ||
         m_outputAttr == "outTransparency"||
         m_outputAttr == "rgbPP" ||
         m_outputAttr == "incandescensePP" ||
         m_outputAttr == "incandescense"
      )
   {
      return AddArnoldNode("userDataColor");
   }

   else if(
         m_outputAttr == "acceleration" ||
         m_outputAttr == "force" ||
         m_outputAttr == "position" ||
         m_outputAttr == "worldPosition" ||
         m_outputAttr == "birthPosition" ||
         m_outputAttr == "birthWorldPosition" ||
         m_outputAttr == "velocity" ||
         m_outputAttr == "worldVelocity" ||
         m_outputAttr == "userVector1PP" ||
         m_outputAttr == "userVector2PP" ||
         m_outputAttr == "userVector3PP" ||
         m_outputAttr == "userVector4PP" ||
         m_outputAttr == "userVector5PP"
         )
   {
      return AddArnoldNode("userDataVector");
   }
   else if (
         m_outputAttr == "ageNormalized" ||
         m_outputAttr == "colorRed" ||
         m_outputAttr == "colorGreen" ||
         m_outputAttr == "colorBlue" ||
         m_outputAttr == "opacityPP" ||
         m_outputAttr == "opacity" ||
         m_outputAttr == "particleId" ||
         m_outputAttr == "lifespanPP" ||
         m_outputAttr == "lifespan" ||
         m_outputAttr == "age" ||
         m_outputAttr == "birthTime" ||
         m_outputAttr == "parentU" ||
         m_outputAttr == "parentV" ||
         m_outputAttr == "collisionU" ||
         m_outputAttr == "collisionV" ||
         m_outputAttr == "radiusPP" ||
         m_outputAttr == "radius" ||
         m_outputAttr == "userScalar1PP" ||
         m_outputAttr == "userScalar2PP" ||
         m_outputAttr == "userScalar3PP" ||
         m_outputAttr == "userScalar4PP" ||
         m_outputAttr == "userScalar5PP"
         )
   {
      return AddArnoldNode("userDataFloat");
   }
   else
      return NULL;
}

void CParticleSamplerInfoTranslator::Export(AtNode* shader)
{
   if (m_outputAttr == "outColor" || m_outputAttr == "rgbPP")
   {
      AiNodeSetStr(shader, "colorAttrName", "rgbPP");
   }
   else if (m_outputAttr == "outTransparency" )
   {
      AiNodeSetStr(shader, "colorAttrName", "opacityPP");
   }
   else if ( m_outputAttr == "opacityPP" || m_outputAttr == "opacity")
   {
      AiNodeSetStr(shader, "floatAttrName" , "opacityPP");
   }
   else if (m_outputAttr == "outIncandescence" || m_outputAttr == "incandescensePP" || m_outputAttr == "incandescense" )
   {
      AiNodeSetStr(shader, "colorAttrName", "incandescencePP");
   }
   else if (m_outputAttr == "lifespanPP" || m_outputAttr == "lifespan")
   {
      AiNodeSetStr(shader, "floatAttrName", "lifespanPP");
   }
   else if (m_outputAttr == "radiusPP" || m_outputAttr == "radius")
   {
      AiNodeSetStr(shader, "floatAttrName", "radiusPP");
   }

   else if(
            m_outputAttr == "acceleration" ||
            m_outputAttr == "force" ||
            m_outputAttr == "position" ||
            m_outputAttr == "worldPosition" ||
            m_outputAttr == "birthPosition" ||
            m_outputAttr == "birthWorldPosition" ||
            m_outputAttr == "velocity" ||
            m_outputAttr == "worldVelocity" ||
            m_outputAttr == "userVector1PP" ||
            m_outputAttr == "userVector2PP" ||
            m_outputAttr == "userVector3PP" ||
            m_outputAttr == "userVector4PP" ||
            m_outputAttr == "userVector5PP"
            )
      {
         AiNodeSetStr(shader, "vectorAttrName", m_outputAttr.asChar());
      }
   else if (
            m_outputAttr == "ageNormalized" ||
            m_outputAttr == "colorRed" ||
            m_outputAttr == "colorGreen" ||
            m_outputAttr == "colorBlue" ||
            m_outputAttr == "particleId" ||
            m_outputAttr == "age" ||
            m_outputAttr == "birthTime" ||
            m_outputAttr == "parentU" ||
            m_outputAttr == "parentV" ||
            m_outputAttr == "collisionU" ||
            m_outputAttr == "collisionV" ||
            m_outputAttr == "userScalar1PP" ||
            m_outputAttr == "userScalar2PP" ||
            m_outputAttr == "userScalar3PP" ||
            m_outputAttr == "userScalar4PP" ||
            m_outputAttr == "userScalar5PP"
            )
      {
         AiNodeSetStr(shader, "floatAttrName", m_outputAttr.asChar());
      }
}

// RemapValue
//
AtNode* CRemapValueTranslator::CreateArnoldNodes()
{
   if (m_outputAttr == "outValue")
   {
      return AddArnoldNode("MayaRemapValueToValue");
   }
   else if (m_outputAttr == "outColor")
   {
      return AddArnoldNode("MayaRemapValueToColor");
   }
   else
      return NULL;
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

      MObject opos = GetMayaObjectAttribute("value_Position");
      MObject oval = GetMayaObjectAttribute("value_FloatValue");
      MObject ointerp = GetMayaObjectAttribute("value_Interp");

      // FIXME: make inputValue the name of the parameter on the MayaRemapValue shader or set metadata
      ProcessParameter(shader, "input", AI_TYPE_FLOAT, FindMayaObjectPlug("inputValue"));
      ProcessParameter(shader, "inputMin", AI_TYPE_FLOAT);
      ProcessParameter(shader, "inputMax", AI_TYPE_FLOAT);
      ProcessParameter(shader, "outputMin", AI_TYPE_FLOAT);
      ProcessParameter(shader, "outputMax", AI_TYPE_FLOAT);

      // Note: this doesn't handle connection coming in individual elements
      attr = FindMayaObjectPlug("value");
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
         unsigned int* shuffle = new unsigned int[positions->nelements];
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

      MObject opos = GetMayaObjectAttribute("color_Position");
      MObject oval = GetMayaObjectAttribute("color_Color");
      MObject ointerp = GetMayaObjectAttribute("color_Interp");

      // FIXME: make inputValue the name of the parameter on the MayaRemapValue shader
      ProcessParameter(shader, "input", AI_TYPE_FLOAT, FindMayaObjectPlug("inputValue"));

      ProcessParameter(shader, "inputMin", AI_TYPE_FLOAT);
      ProcessParameter(shader, "inputMax", AI_TYPE_FLOAT);
      ProcessParameter(shader, "outputMin", AI_TYPE_FLOAT);
      ProcessParameter(shader, "outputMax", AI_TYPE_FLOAT);

      // Note: this doesn't handle connection coming in individual elements
      attr = FindMayaObjectPlug("color");
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
         unsigned int* shuffle = new unsigned int[positions->nelements];
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
AtNode* CRemapColorTranslator::CreateArnoldNodes()
{
   return AddArnoldNode("MayaRemapColor");
}

void CRemapColorTranslator::Export(AtNode* shader)
{
   static const char *remapInterpolationStrings[] =
   {
    "none",
    "linear",
    "smooth",
    "spline"
   };

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
   ProcessParameter(shader, "input", AI_TYPE_RGB, FindMayaObjectPlug("color"));
   ProcessParameter(shader, "inputMin", AI_TYPE_FLOAT);
   ProcessParameter(shader, "inputMax", AI_TYPE_FLOAT);
   ProcessParameter(shader, "outputMin", AI_TYPE_FLOAT);
   ProcessParameter(shader, "outputMax", AI_TYPE_FLOAT);

   for (int ci=0; ci<3; ++ci)
   {
      MObject opos = GetMayaObjectAttribute(posNames[ci*2]);
      MObject oval = GetMayaObjectAttribute(valNames[ci*2]);
      MObject ointerp = GetMayaObjectAttribute(interpNames[ci*2]);

      // Note: this doesn't handle connection coming in individual elements
      attr = FindMayaObjectPlug(plugNames[ci]);
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
         unsigned int* shuffle = new unsigned int[positions->nelements];
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

// Projection
//
AtNode*  CProjectionTranslator::CreateArnoldNodes()
{
   return AddArnoldNode("MayaProjection");
}

void CProjectionTranslator::Export(AtNode* shader)
{
   MStatus status;

   CShaderTranslator::Export(shader);

   MPlug typePlug = FindMayaObjectPlug("projType");
   MPlug camPlug = FindMayaObjectPlug("linkedCamera");
   MPlugArray connections;
   camPlug.connectedTo(connections, true, false);
   if (connections.length() >= 1 && typePlug.asInt() == 8)
   {
      MObject camObj = connections[0].node();
      MFnCamera cam(camObj);
      ProcessParameter(shader, "linkedCamera", AI_TYPE_NODE);
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
AtNode*  CRampTranslator::CreateArnoldNodes()
{
   return AddArnoldNode("MayaRamp");
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
   AiNodeSetBool(shader, "alphaIsLuminance", AtBoolean(true));
   ProcessParameter(shader, "invert", AI_TYPE_BOOLEAN);
   ProcessParameter(shader, "uvCoord", AI_TYPE_POINT2);

   MPlug plug, elem, pos, col;

   MObject opos = GetMayaObjectAttribute("position");
   MObject ocol = GetMayaObjectAttribute("color");
   plug = FindMayaObjectPlug("colorEntryList");
   unsigned int numElements = plug.numElements();
   // Limited to 16 connections
   if (numElements > 16)
   {
      MString warning;
      warning.format("ramp node '^1s' has more than 8 inputs, only the first 8 will be handled", GetMayaNodeName());
      MGlobal::displayWarning(warning);
      numElements = 16;
   }
   AiNodeSetUInt(shader, "numEntries", numElements);

   // Loop on color entries (position, color)
   char aiAttr[64];
   for (unsigned int i=0; i<numElements; ++i)
   {
      elem = plug.elementByPhysicalIndex(i);
      pos = elem.child(opos);
      col = elem.child(ocol);
      sprintf(aiAttr, "position%u", i);
      ProcessParameter(shader, aiAttr, AI_TYPE_FLOAT, pos);
      sprintf(aiAttr, "color%u", i);
      ProcessParameter(shader, aiAttr, AI_TYPE_RGB, col);
   }
}

// Place2DTexture

AtNode*  CPlace2DTextureTranslator::CreateArnoldNodes()
{
   return AddArnoldNode("MayaPlace2DTexture");
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
   ProcessParameter(shader, "offsetUV", AI_TYPE_POINT2, FindMayaObjectPlug("offset"));
   ProcessParameter(shader, "noiseUV", AI_TYPE_POINT2);
}

// LayeredTexture
//
AtNode*  CLayeredTextureTranslator::CreateArnoldNodes()
{
   return AddArnoldNode("MayaLayeredTexture");
}

void CLayeredTextureTranslator::Export(AtNode* shader)
{
   MPlug attr, elem, color, alpha, blendMode, isVisible;
   MPlugArray connections;
   MObject colorSrc, alphaSrc;
   bool colorConnectedToAlpha;
   char aiAttr[64];

   attr = m_fnNode.findPlug("inputs");
   unsigned int numElements = attr.numElements();
   if (numElements > 8)
   {
      AiMsgWarning("[mtoa] [translator %s] layeredTexture node has more than 8 inputs, only the first 8 will be handled", GetTranslatorName().asChar());
      numElements = 8;
   }

   AiNodeSetUInt(shader, "numInputs", numElements);

   ProcessParameter(shader, "alphaIsLuminance", AI_TYPE_BOOLEAN);
   
   MObject colorAttr = m_fnNode.attribute("color");
   MObject alphaAttr = m_fnNode.attribute("alpha");
   MObject blendModeAttr = m_fnNode.attribute("blendMode");
   MObject isVisibleAttr = m_fnNode.attribute("isVisible");

   for (unsigned int i = 0; i < numElements; ++i)
   {
      elem = attr.elementByPhysicalIndex(i);

      color = elem.child(colorAttr);
      alpha = elem.child(alphaAttr);
      blendMode = elem.child(blendModeAttr);
      isVisible = elem.child(isVisibleAttr);

      sprintf(aiAttr, "color%u", i);
      ProcessParameter(shader, aiAttr, AI_TYPE_RGBA, color);

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
      else
      {
         sprintf(aiAttr, "alpha%u", i);
         ProcessParameter(shader, aiAttr, AI_TYPE_FLOAT, alpha);
      }

      sprintf(aiAttr, "blendMode%u", i);
      ProcessParameter(shader, aiAttr, AI_TYPE_ENUM, blendMode);

      sprintf(aiAttr, "visible%u", i);
      ProcessParameter(shader, aiAttr, AI_TYPE_BOOLEAN, isVisible);
   }
}

// LayeredShader
//
AtNode*  CLayeredShaderTranslator::CreateArnoldNodes()
{
   return AddArnoldNode("MayaLayeredShader");
}

void CLayeredShaderTranslator::Export(AtNode* shader)
{
   MPlug attr, elem, color, transp;
   MPlugArray connections;
   MObject colorSrc, transpSrc;
   bool useTransparency;
   // char mayaAttr[64];
   char aiAttr[64];

   ProcessParameter(shader, "compositingFlag", AI_TYPE_ENUM);

   attr = m_fnNode.findPlug("inputs");
   unsigned int numElements = attr.numElements();
   if (numElements > 8)
   {
      AiMsgWarning("[mtoa] [translator %s] LayeredShader node has more than 8 inputs, only the first 8 will be handled", GetTranslatorName().asChar());
      numElements = 8;
   }

   AiNodeSetUInt(shader, "numInputs", numElements);

   MObject colorAttr = m_fnNode.attribute("color");
   MObject transpAttr = m_fnNode.attribute("transparency");

   for (unsigned int i = 0; i < numElements; ++i)
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

      // sprintf(mayaAttr, "inputs[%u].color", elem.logicalIndex());
      sprintf(aiAttr, "color%u", i);
      ProcessParameter(shader, aiAttr, AI_TYPE_RGB, color);

      sprintf(aiAttr, "useTransparency%u", i);
      AiNodeSetBool(shader, aiAttr, useTransparency ? TRUE : FALSE);

      if (useTransparency)
      {
         // sprintf(mayaAttr, "inputs[%u].transparency", elem.logicalIndex());
         sprintf(aiAttr, "transparency%u", i);
         ProcessParameter(shader, aiAttr, AI_TYPE_RGB, transp);
      }
   }
}


// Anim Nodes
//
AtNode*  CAnimCurveTranslator::CreateArnoldNodes()
{
   return AddArnoldNode("anim_float");
}

void CAnimCurveTranslator::Export(AtNode* shader)
{
   MFnAnimCurve fnCurve(GetMayaObject());
   MStatus status;
   float value = (float) fnCurve.evaluate(MAnimControl::currentTime(), &status);

   if (RequiresMotionData())
   {
      AtArray* values = AiArrayAllocate(1, GetNumMotionSteps(), AI_TYPE_FLOAT);
      AiArraySetFlt(values, 0, value);
      AiNodeSetArray(shader, "values", values);
   }
   else
   {
      AiNodeSetFlt(shader, "values", value);
   }
}

void CAnimCurveTranslator::ExportMotion(AtNode* shader, unsigned int step)
{
   MFnAnimCurve fnCurve(GetMayaObject());
   MStatus status;
   float value = (float) fnCurve.evaluate(MAnimControl::currentTime(), &status);
   AtArray* values = AiNodeGetArray(shader, "values");
   AiArraySetFlt(values, step, value);
}
