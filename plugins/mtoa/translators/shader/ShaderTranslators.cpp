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
   return ProcessAOVOutput(AddArnoldNode("lambert"));
}

void CLambertTranslator::Export(AtNode* shader)
{
   // One to one translations
   ProcessParameter(shader, "Kd", AI_TYPE_FLOAT, FindMayaObjectPlug("diffuse"));
   ProcessParameter(shader, "Kd_color", AI_TYPE_RGB, FindMayaObjectPlug("color"));

   // Custom translation
   MPlugArray connections;
   MPlug plug;

   // Transparency to opacity
   plug = FindMayaObjectPlug("transparency");
   if (!plug.isNull())
   {
      // For IPR unlink first
      if (AiNodeIsLinked(shader, "opacity")) AiNodeUnlink(shader, "opacity");
      plug.connectedTo(connections, true, false);
      if (connections.length() > 0)
      {
         AtNode* inNode = ExportNode(connections[0]);
         // Need to reverse it
         if (inNode != NULL)
         {
            MString tag = GetMayaNodeName() + ".transparency";
            AtNode* reverseNode = AddArnoldNode("MayaReverse", tag.asChar());
            AiNodeLink(inNode, "input", reverseNode);
            AiNodeLink(reverseNode, "opacity", shader);
         }
      }
      else
      {

         AiNodeSetRGB(shader, "opacity",
                        1.0f - plug.child(0).asFloat(),
                        1.0f - plug.child(1).asFloat(),
                        1.0f - plug.child(2).asFloat());
      }
   }

   // Normal camera
   plug = FindMayaObjectPlug("normalCamera");
   if (!plug.isNull())
   {
      // For IPR unlink first
      if (AiNodeIsLinked(shader, "@before")) AiNodeUnlink(shader, "@before");
      plug.connectedTo(connections, true, false);
      if (connections.length() > 0)
      {
         AtNode* inNode = ExportNode(connections[0]);
         if (inNode != NULL)
         {
            AiNodeLink(inNode, "@before", shader);
         }
      }
   }
}

// File
//
AtNode*  CFileTranslator::CreateArnoldNodes()
{
   return ProcessAOVOutput(AddArnoldNode("MayaFile"));
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
   return  ProcessAOVOutput(AddArnoldNode("bump3d"));
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
      return ProcessAOVOutput(AddArnoldNode("MayaFacingRatio"));
   }
   else if (m_outputAttr == "flippedNormal")
   {
      return ProcessAOVOutput(AddArnoldNode("MayaFlippedNormal"));
   }
   else
   {
      AiMsgError("[mtoa] [translator %s] invalid output attribute requested: %s", GetTranslatorName().asChar(), m_outputAttr.asChar());
      return NULL;
   }
}

void CSamplerInfoTranslator::Export(AtNode* shader)
{}

// PlusMinusAverage
//
AtNode* CPlusMinusAverageTranslator::CreateArnoldNodes()
{
   if (m_outputAttr == "output1D")
   {
      return ProcessAOVOutput(AddArnoldNode("MayaPlusMinusAverage1D"));
   }
   else if (m_outputAttr == "output2D")
   {
      return ProcessAOVOutput(AddArnoldNode("MayaPlusMinusAverage2D"));
   }
   else if (m_outputAttr == "output3D")
   {
      return ProcessAOVOutput(AddArnoldNode("MayaPlusMinusAverage3D"));
   }
   else
   {
      AiMsgError("[mtoa] [translator %s] invalid output attribute requested: %s", GetTranslatorName().asChar(), m_outputAttr.asChar());
      return NULL;
   }
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
      return ProcessAOVOutput(AddArnoldNode("MayaRemapValueToValue"));
   }
   else if (m_outputAttr == "outColor")
   {
      return ProcessAOVOutput(AddArnoldNode("MayaRemapValueToColor"));
   }
   else
   {
      AiMsgError("[mtoa] [translator %s] invalid output attribute requested: %s", GetTranslatorName().asChar(), m_outputAttr.asChar());
      return NULL;
   }
}

void CRemapValueTranslator::Export(AtNode* shader)
{
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
      
      attr = FindMayaObjectPlug("value");
      unsigned int numElements = attr.numElements();
      InitArrayParameter(shader, "positions", AI_TYPE_FLOAT, numElements);
      InitArrayParameter(shader, "values", AI_TYPE_FLOAT, numElements);
      InitArrayParameter(shader, "interpolations", AI_TYPE_INT, numElements);
      for (unsigned int i=0; i<numElements; ++i)
      {
         elem = attr.elementByPhysicalIndex(i);
         pos = elem.child(opos);
         val = elem.child(oval);
         interp = elem.child(ointerp);
         
         ProcessArrayParameterElement(shader, "positions", pos, AI_TYPE_FLOAT, i);
         ProcessArrayParameterElement(shader, "values", val, AI_TYPE_FLOAT, i);
         ProcessArrayParameterElement(shader, "interpolations", interp, AI_TYPE_INT, i);
      }
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

      attr = FindMayaObjectPlug("color");
      unsigned int numElements = attr.numElements();
      InitArrayParameter(shader, "positions", AI_TYPE_FLOAT, numElements);
      InitArrayParameter(shader, "values", AI_TYPE_RGB, numElements);
      InitArrayParameter(shader, "interpolations", AI_TYPE_INT, numElements);
      for (unsigned int i=0; i<numElements; ++i)
      {
         elem = attr.elementByPhysicalIndex(i);
         pos = elem.child(opos);
         val = elem.child(oval);
         interp = elem.child(ointerp);
         
         ProcessArrayParameterElement(shader, "positions", pos, AI_TYPE_FLOAT, i);
         ProcessArrayParameterElement(shader, "values", val, AI_TYPE_RGB, i);
         ProcessArrayParameterElement(shader, "interpolations", interp, AI_TYPE_INT, i);
      }
   }
}

// Remap Color
//
AtNode* CRemapColorTranslator::CreateArnoldNodes()
{
   return ProcessAOVOutput(AddArnoldNode("MayaRemapColor"));
}

void CRemapColorTranslator::Export(AtNode* shader)
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
      
      attr = FindMayaObjectPlug(plugNames[ci]);
      unsigned int numElements = attr.numElements();
      InitArrayParameter(shader, posNames[ci*2 + 1], AI_TYPE_FLOAT, numElements);
      InitArrayParameter(shader, valNames[ci*2 + 1], AI_TYPE_FLOAT, numElements);
      InitArrayParameter(shader, interpNames[ci*2 + 1], AI_TYPE_INT, numElements);
      for (unsigned int i=0; i<numElements; ++i)
      {
         elem = attr.elementByPhysicalIndex(i);
         pos = elem.child(opos);
         val = elem.child(oval);
         interp = elem.child(ointerp);
         
         ProcessArrayParameterElement(shader, posNames[ci*2 + 1], pos, AI_TYPE_FLOAT, i);
         ProcessArrayParameterElement(shader, valNames[ci*2 + 1], val, AI_TYPE_FLOAT, i);
         ProcessArrayParameterElement(shader, interpNames[ci*2 + 1], interp, AI_TYPE_INT, i);
      }
   }
}

// Remap Hsv
//
AtNode* CRemapHsvTranslator::CreateArnoldNodes()
{
   return AddArnoldNode("MayaRemapHsv");
}

void CRemapHsvTranslator::Export(AtNode* shader)
{
   MPlug attr, elem, pos, val, interp;

   const char *plugNames[3] = {"hue", "saturation", "value"};
   const char *posNames[6] = {"hue_Position",   "hPositions",
                              "saturation_Position", "sPositions",
                              "value_Position",  "vPositions"};
   const char *valNames[6] = {"hue_FloatValue",   "hValues",
                              "saturation_FloatValue", "sValues",
                              "value_FloatValue",  "vValues"};
   const char *interpNames[6] = {"hue_Interp",   "hInterpolations",
                                 "saturation_Interp", "sInterpolations",
                                 "value_Interp",  "vInterpolations"};

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

      attr = FindMayaObjectPlug(plugNames[ci]);
      unsigned int numElements = attr.numElements();
      InitArrayParameter(shader, posNames[ci*2 + 1], AI_TYPE_FLOAT, numElements);
      InitArrayParameter(shader, valNames[ci*2 + 1], AI_TYPE_FLOAT, numElements);
      InitArrayParameter(shader, interpNames[ci*2 + 1], AI_TYPE_INT, numElements);
      for (unsigned int i=0; i<numElements; ++i)
      {
         elem = attr.elementByPhysicalIndex(i);
         pos = elem.child(opos);
         val = elem.child(oval);
         interp = elem.child(ointerp);
         
         ProcessArrayParameterElement(shader, posNames[ci*2 + 1], pos, AI_TYPE_FLOAT, i);
         ProcessArrayParameterElement(shader, valNames[ci*2 + 1], val, AI_TYPE_FLOAT, i);
         ProcessArrayParameterElement(shader, interpNames[ci*2 + 1], interp, AI_TYPE_INT, i);
      }
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
   return ProcessAOVOutput(AddArnoldNode("MayaRamp"));
}

void CRampTranslator::Export(AtNode* shader)
{
   ProcessParameter(shader, "type", AI_TYPE_INT);
   ProcessParameter(shader, "interpolation", AI_TYPE_INT);
   ProcessParameter(shader, "uWave", AI_TYPE_FLOAT);
   ProcessParameter(shader, "vWave", AI_TYPE_FLOAT);
   ProcessParameter(shader, "noise", AI_TYPE_FLOAT);
   ProcessParameter(shader, "noiseFreq", AI_TYPE_FLOAT);
   ProcessParameter(shader, "hueNoise", AI_TYPE_FLOAT);
   ProcessParameter(shader, "hueNoiseFreq", AI_TYPE_FLOAT);
   ProcessParameter(shader, "satNoise", AI_TYPE_FLOAT);
   ProcessParameter(shader, "satNoiseFreq", AI_TYPE_FLOAT);
   ProcessParameter(shader, "valNoise", AI_TYPE_FLOAT);
   ProcessParameter(shader, "valNoiseFreq", AI_TYPE_FLOAT);
   ProcessParameter(shader, "colorGain", AI_TYPE_RGB);
   ProcessParameter(shader, "colorOffset", AI_TYPE_RGB);
   ProcessParameter(shader, "defaultColor", AI_TYPE_RGB);
   ProcessParameter(shader, "alphaGain", AI_TYPE_FLOAT);
   ProcessParameter(shader, "alphaOffset", AI_TYPE_FLOAT);
   AiNodeSetBool(shader, "alphaIsLuminance", true);
   ProcessParameter(shader, "invert", AI_TYPE_BOOLEAN);
   ProcessParameter(shader, "uvCoord", AI_TYPE_POINT2);

   MPlug plug, elem, pos, col;

   MObject opos = GetMayaObjectAttribute("position");
   MObject ocol = GetMayaObjectAttribute("color");
   plug = FindMayaObjectPlug("colorEntryList");
   unsigned int numElements = plug.numElements();

   // Loop on color entries (position, color)

   InitArrayParameter(shader, "position", AI_TYPE_FLOAT, numElements);
   InitArrayParameter(shader, "color", AI_TYPE_RGB, numElements);
   
   for (unsigned int i=0; i<numElements; ++i)
   {
      elem = plug.elementByPhysicalIndex(i);
      pos = elem.child(opos);
      col = elem.child(ocol);
      
      ProcessArrayParameterElement(shader, "position", pos, AI_TYPE_FLOAT, i);
      ProcessArrayParameterElement(shader, "color", col, AI_TYPE_RGB, i);
   }
}

// Place2DTexture

AtNode*  CPlace2DTextureTranslator::CreateArnoldNodes()
{
   return ProcessAOVOutput(AddArnoldNode("MayaPlace2DTexture"));
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
   return ProcessAOVOutput(AddArnoldNode("MayaLayeredTexture"));
}

void CLayeredTextureTranslator::Export(AtNode* shader)
{
   ProcessParameter(shader, "alphaIsLuminance", AI_TYPE_BOOLEAN);

   MPlug inputs, elemt, color, alpha, mode, visible;
   inputs = FindMayaObjectPlug("inputs");
   MObject ocolor = GetMayaObjectAttribute("color");
   MObject oalpha = GetMayaObjectAttribute("alpha");
   MObject omode = GetMayaObjectAttribute("blendMode");
   MObject ovisible = GetMayaObjectAttribute("isVisible");
   unsigned int numElements = inputs.numElements();

   // Init shader array parameters

   InitArrayParameter(shader, "color", AI_TYPE_RGBA, numElements);
   InitArrayParameter(shader, "alpha", AI_TYPE_FLOAT, numElements);
   InitArrayParameter(shader, "blendMode", AI_TYPE_INT, numElements);
   InitArrayParameter(shader, "visible", AI_TYPE_BOOLEAN, numElements);

   // Loop on input entries

   for (unsigned int i=0; i<numElements; ++i)
   {
      elemt = inputs.elementByPhysicalIndex(i);
      color = elemt.child(ocolor);
      alpha = elemt.child(oalpha);
      mode = elemt.child(omode);
      visible = elemt.child(ovisible);

      ProcessArrayParameterElement(shader, "color", color, AI_TYPE_RGBA, i);
      ProcessArrayParameterElement(shader, "alpha", alpha, AI_TYPE_FLOAT, i);
      ProcessArrayParameterElement(shader, "blendMode", mode, AI_TYPE_INT, i);
      ProcessArrayParameterElement(shader, "visible", visible, AI_TYPE_BOOLEAN, i);
   }
}

// LayeredShader
//
AtNode*  CLayeredShaderTranslator::CreateArnoldNodes()
{
   return ProcessAOVOutput(AddArnoldNode("MayaLayeredShader"));
}

void CLayeredShaderTranslator::Export(AtNode* shader)
{
   ProcessParameter(shader, "compositingFlag", AI_TYPE_ENUM);

   MPlug inputs, elemt, color, trans;
   inputs = FindMayaObjectPlug("inputs");
   MObject ocol = GetMayaObjectAttribute("color");
   MObject otrs = GetMayaObjectAttribute("transparency");
   unsigned int numElements = inputs.numElements();

   // Init shader array parameters

   InitArrayParameter(shader, "color", AI_TYPE_RGBA, numElements);
   InitArrayParameter(shader, "transparency", AI_TYPE_RGB, numElements);

   // Loop on input entries

   for (unsigned int i=0; i<numElements; ++i)
   {
      elemt = inputs.elementByPhysicalIndex(i);
      color = elemt.child(ocol);
      trans = elemt.child(otrs);

      ProcessArrayParameterElement(shader, "color", color, AI_TYPE_RGBA, i);
      ProcessArrayParameterElement(shader, "transparency", trans, AI_TYPE_RGB, i);
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
