#include "ShaderTranslators.h"
#include "scene/MayaScene.h"
#include "render/RenderOptions.h"
#include "render/RenderSession.h"
#include "platform/Platform.h"
#include "utils/MakeTx.h"

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
   AiNodeSetVec(shader, "Z", 0.0f, 0.0f, 1.0f/static_cast<float>(scale[2]));

   ProcessParameter(shader, "color",     AI_TYPE_RGB);
   ProcessParameter(shader, "format",    AI_TYPE_ENUM);
   ProcessParameter(shader, "intensity", AI_TYPE_FLOAT);

   AtByte visibility = ComputeVisibility();
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
   ProcessParameter(shader, "Kd", AI_TYPE_FLOAT, "diffuse");
   ProcessParameter(shader, "Kd_color", AI_TYPE_RGB, "color");

   // Custom translation
   MPlugArray connections;
   MPlug plug;

   // Transparency to opacity
   plug = FindMayaPlug("transparency");
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
   
   ExportBump(shader);
}

// Physical Sky
//
AtNode*  CPhysicalSkyTranslator::CreateArnoldNodes()
{
   return ProcessAOVOutput(AddArnoldNode("physical_sky"));
}

void CPhysicalSkyTranslator::Export(AtNode* shader)
{
   // All physical sky attributes are not linkable in Arnold
   MStatus status;
   
   MPlug plug = FindMayaPlug("turbidity", &status);
   ProcessConstantParameter(shader, "turbidity", AI_TYPE_FLOAT, plug);
   
   plug = FindMayaPlug("ground_albedo", &status);
   AiNodeSetRGB(shader, "ground_albedo", plug.child(0).asFloat(), plug.child(1).asFloat(), plug.child(2).asFloat());
   
   plug = FindMayaPlug("use_degrees", &status);
   ProcessConstantParameter(shader, "use_degrees", AI_TYPE_BOOLEAN, plug);
   
   plug = FindMayaPlug("elevation", &status);
   ProcessConstantParameter(shader, "elevation", AI_TYPE_FLOAT, plug);
   
   plug = FindMayaPlug("azimuth", &status);
   ProcessConstantParameter(shader, "azimuth", AI_TYPE_FLOAT, plug);
   
   plug = FindMayaPlug("sun_direction", &status);
   AiNodeSetVec(shader, "sun_direction", plug.child(0).asFloat(), plug.child(1).asFloat(), plug.child(2).asFloat());
   
   plug = FindMayaPlug("enable_sun", &status);
   ProcessConstantParameter(shader, "enable_sun", AI_TYPE_BOOLEAN, plug);
   
   plug = FindMayaPlug("intensity", &status);
   ProcessConstantParameter(shader, "intensity", AI_TYPE_FLOAT, plug);
   
   plug = FindMayaPlug("sky_tint", &status);
   AiNodeSetRGB(shader, "sky_tint", plug.child(0).asFloat(), plug.child(1).asFloat(), plug.child(2).asFloat());
   
   plug = FindMayaPlug("sun_tint", &status);
   AiNodeSetRGB(shader, "sun_tint", plug.child(0).asFloat(), plug.child(1).asFloat(), plug.child(2).asFloat());
   
   plug = FindMayaPlug("sun_size", &status);
   ProcessConstantParameter(shader, "sun_size", AI_TYPE_FLOAT, plug);
   
   plug = FindMayaPlug("X", &status);
   AiNodeSetVec(shader, "X", plug.child(0).asFloat(), plug.child(1).asFloat(), plug.child(2).asFloat());
   
   plug = FindMayaPlug("Y", &status);
   AiNodeSetVec(shader, "Y", plug.child(0).asFloat(), plug.child(1).asFloat(), plug.child(2).asFloat());
   
   plug = FindMayaPlug("Z", &status);
   AiNodeSetVec(shader, "Z", plug.child(0).asFloat(), plug.child(1).asFloat(), plug.child(2).asFloat());
}

// File
//
AtNode*  CFileTranslator::CreateArnoldNodes()
{
   return ProcessAOVOutput(AddArnoldNode("MayaFile"));
}

bool StringHasOnlyNumbersAndMinus(const std::string& str)
{
   static const char validCharacters[10] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'}; // do we have to add - ?
   for (std::string::const_iterator it = str.begin(); it != str.end(); ++it)
   {
      for (int i = 0; i < 10; ++i)
      {
         if (*it == validCharacters[i])
            return true;
      }
   }
   return false;
}

void CFileTranslator::Export(AtNode* shader)
{
   MPlugArray connections;

   MPlug plug = FindMayaPlug("uvCoord");

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
         srcNodeFn.findPlug("uvCoord").connectedTo(connections, true, false);
         if (connections.length() > 0)
         {
            MFnDependencyNode uvcNodeFn(connections[0].node());
            if (uvcNodeFn.typeName() == "uvChooser")
               AiNodeSetStr(shader, "uvSetName", uvcNodeFn.findPlug("uvSets").elementByPhysicalIndex(0).asString().asChar());
         }
      }
   }
   
   if (NULL == ProcessParameter(shader, "filename", AI_TYPE_STRING, "fileTextureName"))
   {
      MString resolvedFilename; 
      MString frameNumber("0"); 
      MStatus status; 
      frameNumber += GetExportFrame() + FindMayaPlug("frameOffset").asInt();
      int tilingMode = FindMayaPlug("uvTilingMode").asInt();
      if (tilingMode == 0)
      {
         MString filename; 
         MRenderUtil::exactFileTextureName(GetMayaObject(), filename);
         resolvedFilename = MRenderUtil::exactFileTextureName(filename, FindMayaPlug("useFrameExtension").asBool(), frameNumber, &status);
         if (status == MStatus::kSuccess) 
         { 
            // Cancels above resolution since it ruins tokens
            resolvedFilename = filename; 
         } 
         else 
         { 
            resolvedFilename = FindMayaPlug("fileTextureName").asString();
         }
      }
      else
      {
         resolvedFilename = FindMayaPlug("fileTextureNamePattern").asString();
         if (resolvedFilename.length() == 0)
         {
            resolvedFilename = FindMayaPlug("computedFileTextureNamePattern").asString();
         }
      }
      
      // FIXME really inconvenient, a CRenderOptions instance should be stored in session 
      // or that class eliminated completely 
      CRenderOptions renderOptions; 
      renderOptions.SetArnoldRenderOptions(GetArnoldRenderOptions()); 
      renderOptions.GetFromMaya(); 

      // Do not call makeTx if we're doing swatch rendering or material view
      int sessionMode = m_session->GetSessionMode();
      if (sessionMode != MTOA_SESSION_MATERIALVIEW && sessionMode != MTOA_SESSION_SWATCH &&
         sessionMode != MTOA_SESSION_UNDEFINED)
      {
         if (renderOptions.autoTx() && FindMayaPlug("aiAutoTx").asBool())
         {
            MString colorSpace = FindMayaPlug("colorSpace").asString();
            int createdFiles = 0;
            int skippedFiles = 0;
            int errorFiles = 0;
            makeTx(resolvedFilename, colorSpace, &createdFiles, &skippedFiles, &errorFiles);

            if (createdFiles + skippedFiles + errorFiles == 0)
            {               
               // no file has been found
               // let's try with the search paths
               const MStringArray &searchPaths = m_session->GetTextureSearchPaths();
               for (unsigned int i = 0; i < searchPaths.length(); ++i)
               {
                  MString searchFilename = searchPaths[i] + resolvedFilename;
                  makeTx(searchFilename, colorSpace, &createdFiles, &skippedFiles, &errorFiles);

                  if (createdFiles + skippedFiles + errorFiles > 0) break; // textures have been found with this search path. Let's stop looking for them
               }
            }
         }
      }      
      if(renderOptions.useExistingTiledTextures()) 
      {
         MString txFilename(resolvedFilename.substring(0, resolvedFilename.rindexW(".")) + MString("tx"));
         MStringArray expandedFilenames = expandFilename(txFilename);

         if(expandedFilenames.length() == 0)
         {
            // No file was found for this filename
            // we should check in the search paths
            const MStringArray &searchPaths = m_session->GetTextureSearchPaths();
         
            for (unsigned int i = 0; i < searchPaths.length(); ++i)
            {

               MString searchFilename = searchPaths[i] + txFilename;
               expandedFilenames = expandFilename(searchFilename);
               
               // we found the texture, stop searching
               if (expandedFilenames.length() > 0) break;
            }
         }
         // if expandedFilenames.length >= 1 then we're OK ?
         if (expandedFilenames.length() > 0)
         {
            resolvedFilename = txFilename;
         }
      }
      m_session->FormatTexturePath(resolvedFilename);
      AiNodeSetStr(shader, "filename", resolvedFilename.asChar()); 
   }

   ProcessParameter(shader, "mipBias", AI_TYPE_INT);
   AiNodeSetInt(shader, "filter", FindMayaPlug("aiFilter").asInt());
   AiNodeSetBool(shader, "useDefaultColor", FindMayaPlug("aiUseDefaultColor").asBool());

   ProcessParameter(shader, "colorGain", AI_TYPE_RGB);
   ProcessParameter(shader, "colorOffset", AI_TYPE_RGB);
   ProcessParameter(shader, "alphaGain", AI_TYPE_FLOAT);
   ProcessParameter(shader, "alphaOffset", AI_TYPE_FLOAT);
   ProcessParameter(shader, "alphaIsLuminance", AI_TYPE_BOOLEAN);
   ProcessParameter(shader, "invert", AI_TYPE_BOOLEAN);
   ProcessParameter(shader, "defaultColor", AI_TYPE_RGB);

   plug = FindMayaPlug("exposure");
   if (plug.isNull())
      AiNodeSetFlt(shader, "exposure", 0.0f);
   else
      ProcessParameter(shader, "exposure", AI_TYPE_FLOAT, plug);
}

void CFileTranslator::NodeInitializer(CAbTranslator context)
{

   CExtensionAttrHelper helper(context.maya, "MayaFile");
   helper.MakeInput("mipBias");
   helper.MakeInput("filter");
   helper.MakeInput("useDefaultColor");

   CAttrData data;
   data.defaultValue.BOOL = true;
   data.name = "aiAutoTx";
   data.shortName = "autotx";
   helper.MakeInputBoolean(data);
}

// Bump2d
//
void CBump2DTranslator::NodeInitializer(CAbTranslator context)
{
   CExtensionAttrHelper helper(context.maya, "bump2d");
   
   CAttrData data;
   data.defaultValue.BOOL = true;
   data.name = "aiFlipR";
   data.shortName = "flip_r";
   helper.MakeInputBoolean(data);
   
   data.name = "aiFlipG";
   data.shortName = "flip_g";
   helper.MakeInputBoolean(data);
   
   data.defaultValue.BOOL = false;
   data.name = "aiSwapTangents";
   data.shortName = "swap_tangents";
   helper.MakeInputBoolean(data);
   
   data.defaultValue.BOOL = true;
   data.name = "aiUseDerivatives";
   data.shortName = "use_derivatives";
   helper.MakeInputBoolean(data);
   
#ifdef MTOA_ENABLE_GAMMA
   data.name = "aiGammaCorrect";
   data.shortName = "gamma_correct";
   helper.MakeInputBoolean(data);
#endif
}

AtNode*  CBump2DTranslator::CreateArnoldNodes()
{
   return AddArnoldNode("mayaBump2D");
}

void CBump2DTranslator::Export(AtNode* shader)
{
   ProcessParameter(shader, "bump_map", AI_TYPE_FLOAT, "bumpValue");
   ProcessParameter(shader, "bump_height", AI_TYPE_FLOAT, "bumpDepth");
   MStatus status;
   MPlug plug = FindMayaPlug("bumpInterp", &status);
   if (status && !plug.isNull())
   {
      int useAs = plug.asShort();
      AiNodeSetInt(shader, "use_as", useAs);
      if (useAs > 0)
      {         
         AtNode* n = AiNodeGetLink(shader, "bump_map");
         if (n != 0)
            AiNodeLink(n, "normal_map", shader);
      }
   }
   ProcessParameter(shader, "flip_r", AI_TYPE_BOOLEAN, "aiFlipR");
   ProcessParameter(shader, "flip_g", AI_TYPE_BOOLEAN, "aiFlipG");
   ProcessParameter(shader, "swap_tangents", AI_TYPE_BOOLEAN, "aiSwapTangents");
   ProcessParameter(shader, "use_derivatives", AI_TYPE_BOOLEAN, "aiUseDerivatives");
#ifdef MTOA_ENABLE_GAMMA
   ProcessParameter(shader, "gamma_correct", AI_TYPE_BOOLEAN, "aiGammaCorrect");
#else
   AiNodeSetFlt(shader, "gamma_correct", 1.f);
#endif
   MPlugArray connections;
   plug = FindMayaPlug("normalCamera");

   ExportBump(shader);
}

// Bump3d
//
AtNode*  CBump3DTranslator::CreateArnoldNodes()
{
   return  ProcessAOVOutput(AddArnoldNode("bump3d"));
}

void CBump3DTranslator::Export(AtNode* shader)
{
   ProcessParameter(shader, "bump_map", AI_TYPE_FLOAT, "bumpValue");
   ProcessParameter(shader, "bump_height", AI_TYPE_FLOAT, "bumpDepth");   
}

// SamplerInfo
//
AtNode* CSamplerInfoTranslator::CreateArnoldNodes()
{
   MString outputAttr = GetMayaAttributeName();
   AtNode* shader = NULL;
   if (outputAttr == "facingRatio" || outputAttr == "flippedNormal")
   {
      shader = AddArnoldNode("MayaSamplerInfo1D");
   }
   else if (outputAttr == "uvCoord" || outputAttr == "pixelCenter")
   {
      shader = AddArnoldNode("MayaSamplerInfo2D");
   }
   else if (outputAttr == "pointWorld" || outputAttr == "pointObj" || outputAttr == "pointCamera" ||
            outputAttr == "normalCamera" || outputAttr == "rayDirection" ||
            outputAttr == "tangentUCamera" ||outputAttr == "tangentVCamera")
   {
      shader = AddArnoldNode("MayaSamplerInfo3D");
   }
   else
   {
      AiMsgError("[mtoa] [translator %s] invalid output attribute requested: %s", GetTranslatorName().asChar(), outputAttr.asChar());
      return NULL;
   }
   AtEnum modeEnum = AiParamGetEnum(AiNodeEntryLookUpParameter(AiNodeGetNodeEntry(shader), "mode"));
   AiNodeSetInt(shader, "mode", AiEnumGetValue(modeEnum, outputAttr.asChar()));
   return ProcessAOVOutput(shader);
}

void CSamplerInfoTranslator::Export(AtNode* shader)
{}

// PlusMinusAverage
//
AtNode* CPlusMinusAverageTranslator::CreateArnoldNodes()
{
   MString outputAttr = GetMayaAttributeName();

   if (outputAttr == "output1D")
   {
      return ProcessAOVOutput(AddArnoldNode("MayaPlusMinusAverage1D"));
   }
   else if (outputAttr == "output2D")
   {
      return ProcessAOVOutput(AddArnoldNode("MayaPlusMinusAverage2D"));
   }
   else if (outputAttr == "output3D")
   {
      return ProcessAOVOutput(AddArnoldNode("MayaPlusMinusAverage3D"));
   }
   else
   {
      AiMsgError("[mtoa] [translator %s] invalid output attribute requested: %s", GetTranslatorName().asChar(), outputAttr.asChar());
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
   MString outputAttr = GetMayaAttributeName();

   if (
         outputAttr == "outColor" ||
         outputAttr == "outIncandescence" ||
         outputAttr == "outTransparency"||
         outputAttr == "rgbPP" ||
         outputAttr == "incandescensePP" ||
         outputAttr == "incandescense"
      )
   {
      return AddArnoldNode("userDataColor");
   }

   else if(
         outputAttr == "acceleration" ||
         outputAttr == "force" ||
         outputAttr == "position" ||
         outputAttr == "worldPosition" ||
         outputAttr == "birthPosition" ||
         outputAttr == "birthWorldPosition" ||
         outputAttr == "velocity" ||
         outputAttr == "worldVelocity" ||
         outputAttr == "userVector1PP" ||
         outputAttr == "userVector2PP" ||
         outputAttr == "userVector3PP" ||
         outputAttr == "userVector4PP" ||
         outputAttr == "userVector5PP"
         )
   {
      return AddArnoldNode("userDataVector");
   }
   else if (
         outputAttr == "ageNormalized" ||
         outputAttr == "colorRed" ||
         outputAttr == "colorGreen" ||
         outputAttr == "colorBlue" ||
         outputAttr == "opacityPP" ||
         outputAttr == "opacity" ||
         outputAttr == "particleId" ||
         outputAttr == "lifespanPP" ||
         outputAttr == "lifespan" ||
         outputAttr == "age" ||
         outputAttr == "birthTime" ||
         outputAttr == "parentU" ||
         outputAttr == "parentV" ||
         outputAttr == "collisionU" ||
         outputAttr == "collisionV" ||
         outputAttr == "radiusPP" ||
         outputAttr == "radius" ||
         outputAttr == "userScalar1PP" ||
         outputAttr == "userScalar2PP" ||
         outputAttr == "userScalar3PP" ||
         outputAttr == "userScalar4PP" ||
         outputAttr == "userScalar5PP"
         )
   {
      return AddArnoldNode("userDataFloat");
   }
   else
      return NULL;
}

void CParticleSamplerInfoTranslator::Export(AtNode* shader)
{
   MString outputAttr = GetMayaAttributeName();

   if (outputAttr == "outColor" || outputAttr == "rgbPP")
   {
      AiNodeSetStr(shader, "colorAttrName", "rgbPP");
   }
   else if (outputAttr == "outTransparency" )
   {
      AiNodeSetStr(shader, "colorAttrName", "opacityPP");
   }
   else if ( outputAttr == "opacityPP" || outputAttr == "opacity")
   {
      AiNodeSetStr(shader, "floatAttrName" , "opacityPP");
   }
   else if (outputAttr == "outIncandescence" || outputAttr == "incandescensePP" || outputAttr == "incandescense" )
   {
      AiNodeSetStr(shader, "colorAttrName", "incandescencePP");
   }
   else if (outputAttr == "lifespanPP" || outputAttr == "lifespan")
   {
      AiNodeSetStr(shader, "floatAttrName", "lifespanPP");
   }
   else if (outputAttr == "radiusPP" || outputAttr == "radius")
   {
      AiNodeSetStr(shader, "floatAttrName", "radiusPP");
   }

   else if(
            outputAttr == "acceleration" ||
            outputAttr == "force" ||
            outputAttr == "position" ||
            outputAttr == "worldPosition" ||
            outputAttr == "birthPosition" ||
            outputAttr == "birthWorldPosition" ||
            outputAttr == "velocity" ||
            outputAttr == "worldVelocity" ||
            outputAttr == "userVector1PP" ||
            outputAttr == "userVector2PP" ||
            outputAttr == "userVector3PP" ||
            outputAttr == "userVector4PP" ||
            outputAttr == "userVector5PP"
            )
      {
         AiNodeSetStr(shader, "vectorAttrName", outputAttr.asChar());
      }
   else if (
            outputAttr == "ageNormalized" ||
            outputAttr == "colorRed" ||
            outputAttr == "colorGreen" ||
            outputAttr == "colorBlue" ||
            outputAttr == "particleId" ||
            outputAttr == "age" ||
            outputAttr == "birthTime" ||
            outputAttr == "parentU" ||
            outputAttr == "parentV" ||
            outputAttr == "collisionU" ||
            outputAttr == "collisionV" ||
            outputAttr == "userScalar1PP" ||
            outputAttr == "userScalar2PP" ||
            outputAttr == "userScalar3PP" ||
            outputAttr == "userScalar4PP" ||
            outputAttr == "userScalar5PP"
            )
      {
         AiNodeSetStr(shader, "floatAttrName", outputAttr.asChar());
      }
}

// RemapValue
//
AtNode* CRemapValueTranslator::CreateArnoldNodes()
{
   MString outputAttr = GetMayaAttributeName();

   if (outputAttr == "outValue")
   {
      return ProcessAOVOutput(AddArnoldNode("MayaRemapValueToValue"));
   }
   else if (outputAttr == "outColor")
   {
      return ProcessAOVOutput(AddArnoldNode("MayaRemapValueToColor"));
   }
   else
   {
      AiMsgError("[mtoa] [translator %s] invalid output attribute requested: %s", GetTranslatorName().asChar(), outputAttr.asChar());
      return NULL;
   }
}

void CRemapValueTranslator::Export(AtNode* shader)
{
   MString outputAttr = GetMayaAttributeName();

   if (outputAttr == "outValue")
   {
      MPlug attr, elem, pos, val, interp;

      MObject opos = GetMayaObjectAttribute("value_Position");
      MObject oval = GetMayaObjectAttribute("value_FloatValue");
      MObject ointerp = GetMayaObjectAttribute("value_Interp");

      // FIXME: make inputValue the name of the parameter on the MayaRemapValue shader or set metadata
      ProcessParameter(shader, "input", AI_TYPE_FLOAT, "inputValue");
      ProcessParameter(shader, "inputMin", AI_TYPE_FLOAT);
      ProcessParameter(shader, "inputMax", AI_TYPE_FLOAT);
      ProcessParameter(shader, "outputMin", AI_TYPE_FLOAT);
      ProcessParameter(shader, "outputMax", AI_TYPE_FLOAT);
      
      attr = FindMayaPlug("value");
      unsigned int numElements = attr.numElements();
      AtArray* positions = InitArrayParameter(AI_TYPE_FLOAT, numElements);
      AtArray* values = InitArrayParameter(AI_TYPE_FLOAT, numElements);
      AtArray* interpolations = InitArrayParameter(AI_TYPE_INT, numElements);
      for (unsigned int i=0; i<numElements; ++i)
      {
         elem = attr.elementByPhysicalIndex(i);
         pos = elem.child(opos);
         val = elem.child(oval);
         interp = elem.child(ointerp);
         
         ProcessArrayParameterElement(shader, positions, "positions", pos, AI_TYPE_FLOAT, i);
         ProcessArrayParameterElement(shader, values, "values", val, AI_TYPE_FLOAT, i);
         ProcessArrayParameterElement(shader, interpolations, "interpolations", interp, AI_TYPE_INT, i);
      }
      SetArrayParameter(shader, "positions", positions);
      SetArrayParameter(shader, "values", values);
      SetArrayParameter(shader, "interpolations", interpolations);
   }
   else if (outputAttr == "outColor")
   {
      MPlug attr, elem, pos, val, interp;

      MObject opos = GetMayaObjectAttribute("color_Position");
      MObject oval = GetMayaObjectAttribute("color_Color");
      MObject ointerp = GetMayaObjectAttribute("color_Interp");

      // FIXME: make inputValue the name of the parameter on the MayaRemapValue shader
      ProcessParameter(shader, "input", AI_TYPE_FLOAT, "inputValue");

      ProcessParameter(shader, "inputMin", AI_TYPE_FLOAT);
      ProcessParameter(shader, "inputMax", AI_TYPE_FLOAT);
      ProcessParameter(shader, "outputMin", AI_TYPE_FLOAT);
      ProcessParameter(shader, "outputMax", AI_TYPE_FLOAT);

      attr = FindMayaPlug("color");
      unsigned int numElements = attr.numElements();
      AtArray* positions = InitArrayParameter(AI_TYPE_FLOAT, numElements);
      AtArray* values = InitArrayParameter(AI_TYPE_RGB, numElements);
      AtArray* interpolations = InitArrayParameter(AI_TYPE_INT, numElements);
      for (unsigned int i=0; i<numElements; ++i)
      {
         elem = attr.elementByPhysicalIndex(i);
         pos = elem.child(opos);
         val = elem.child(oval);
         interp = elem.child(ointerp);
         
         ProcessArrayParameterElement(shader, positions, "positions", pos, AI_TYPE_FLOAT, i);
         ProcessArrayParameterElement(shader, values, "values", val, AI_TYPE_RGB, i);
         ProcessArrayParameterElement(shader, interpolations, "interpolations", interp, AI_TYPE_INT, i);
      }
      SetArrayParameter(shader, "positions", positions);
      SetArrayParameter(shader, "values", values);
      SetArrayParameter(shader, "interpolations", interpolations);
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
   ProcessParameter(shader, "input", AI_TYPE_RGB, "color");
   ProcessParameter(shader, "inputMin", AI_TYPE_FLOAT);
   ProcessParameter(shader, "inputMax", AI_TYPE_FLOAT);
   ProcessParameter(shader, "outputMin", AI_TYPE_FLOAT);
   ProcessParameter(shader, "outputMax", AI_TYPE_FLOAT);

   for (int ci=0; ci<3; ++ci)
   {
      MObject opos = GetMayaObjectAttribute(posNames[ci*2]);
      MObject oval = GetMayaObjectAttribute(valNames[ci*2]);
      MObject ointerp = GetMayaObjectAttribute(interpNames[ci*2]);
      
      attr = FindMayaPlug(plugNames[ci]);
      unsigned int numElements = attr.numElements();
      AtArray* positions = InitArrayParameter(AI_TYPE_FLOAT, numElements);
      AtArray* values = InitArrayParameter(AI_TYPE_FLOAT, numElements);
      AtArray* interpolations = InitArrayParameter(AI_TYPE_INT, numElements);
      for (unsigned int i=0; i<numElements; ++i)
      {
         elem = attr.elementByPhysicalIndex(i);
         pos = elem.child(opos);
         val = elem.child(oval);
         interp = elem.child(ointerp);
         
         ProcessArrayParameterElement(shader, positions, posNames[ci*2 + 1], pos, AI_TYPE_FLOAT, i);
         ProcessArrayParameterElement(shader, values, valNames[ci*2 + 1], val, AI_TYPE_FLOAT, i);
         ProcessArrayParameterElement(shader, interpolations, interpNames[ci*2 + 1], interp, AI_TYPE_INT, i);
      }
      SetArrayParameter(shader, posNames[ci*2 + 1], positions);
      SetArrayParameter(shader, valNames[ci*2 + 1], values);
      SetArrayParameter(shader, interpNames[ci*2 + 1], interpolations);
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
   ProcessParameter(shader, "input", AI_TYPE_RGB, "color");
   ProcessParameter(shader, "inputMin", AI_TYPE_FLOAT);
   ProcessParameter(shader, "inputMax", AI_TYPE_FLOAT);
   ProcessParameter(shader, "outputMin", AI_TYPE_FLOAT);
   ProcessParameter(shader, "outputMax", AI_TYPE_FLOAT);

   for (int ci=0; ci<3; ++ci)
   {
      MObject opos = GetMayaObjectAttribute(posNames[ci*2]);
      MObject oval = GetMayaObjectAttribute(valNames[ci*2]);
      MObject ointerp = GetMayaObjectAttribute(interpNames[ci*2]);

      attr = FindMayaPlug(plugNames[ci]);
      unsigned int numElements = attr.numElements();
      AtArray* positions = InitArrayParameter(AI_TYPE_FLOAT, numElements);
      AtArray* values = InitArrayParameter(AI_TYPE_FLOAT, numElements);
      AtArray* interpolations = InitArrayParameter(AI_TYPE_INT, numElements);
      for (unsigned int i=0; i<numElements; ++i)
      {
         elem = attr.elementByPhysicalIndex(i);
         pos = elem.child(opos);
         val = elem.child(oval);
         interp = elem.child(ointerp);
         
         ProcessArrayParameterElement(shader, positions, posNames[ci*2 + 1], pos, AI_TYPE_FLOAT, i);
         ProcessArrayParameterElement(shader, values, valNames[ci*2 + 1], val, AI_TYPE_FLOAT, i);
         ProcessArrayParameterElement(shader, interpolations, interpNames[ci*2 + 1], interp, AI_TYPE_INT, i);
      }
      SetArrayParameter(shader, posNames[ci*2 + 1], positions);
      SetArrayParameter(shader, valNames[ci*2 + 1], values);
      SetArrayParameter(shader, interpNames[ci*2 + 1], interpolations);
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

   MPlug typePlug = FindMayaPlug("projType");
   MPlug camPlug = FindMayaPlug("linkedCamera");
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
   ProcessParameter(shader, "useReferenceObject", AI_TYPE_BOOLEAN, "aiUseReferenceObject");
}

void ProjectionTranslatorNodeInitializer(CAbTranslator context)
{
   CExtensionAttrHelper helper("projection");
   
   CAttrData data;
   
   data.defaultValue.BOOL = true;
   data.name = "aiUseReferenceObject";
   data.shortName = "ai_use_reference_object";
   helper.MakeInputBoolean(data);
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
   plug = FindMayaPlug("colorEntryList");
   unsigned int numElements = plug.numElements();

   // Loop on color entries (position, color)

   AtArray* position = InitArrayParameter(AI_TYPE_FLOAT, numElements);
   AtArray* color = InitArrayParameter(AI_TYPE_RGB, numElements);
   
   for (unsigned int i=0; i<numElements; ++i)
   {
      elem = plug.elementByPhysicalIndex(i);
      pos = elem.child(opos);
      col = elem.child(ocol);
      
      ProcessArrayParameterElement(shader, position, "position", pos, AI_TYPE_FLOAT, i);
      ProcessArrayParameterElement(shader, color, "color", col, AI_TYPE_RGB, i);
   }
   SetArrayParameter(shader, "position", position);
   SetArrayParameter(shader, "color", color);
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
   ProcessParameter(shader, "offsetUV", AI_TYPE_POINT2, "offset");
   ProcessParameter(shader, "noiseUV", AI_TYPE_POINT2);

   MFnDependencyNode fnNode(GetMayaObject());
   MPlugArray connections;
   fnNode.findPlug("uvCoord").connectedTo(connections, true, false);
   if (connections.length() > 0)
   {
      MFnDependencyNode uvcNodeFn(connections[0].node());
      if (uvcNodeFn.typeName() == "uvChooser")
         AiNodeSetStr(shader, "uvSetName", uvcNodeFn.findPlug("uvSets").elementByPhysicalIndex(0).asString().asChar());
   }
}

// LayeredTexture
//
AtNode*  CLayeredTextureTranslator::CreateArnoldNodes()
{
   return ProcessAOVOutput(AddArnoldNode("MayaLayeredTexture"));
}

void CLayeredTextureTranslator::Export(AtNode* shader)
{
   MPlug attr, elem, color, alpha, blendMode, isVisible;
   MPlugArray connections;
   MObject colorSrc, alphaSrc;
   bool colorConnectedToAlpha;
   char aiAttr[64];
   
   MFnDependencyNode fnNode(GetMayaObject());

   attr = fnNode.findPlug("inputs");
   unsigned int numElements = attr.numElements();
   if (numElements > 16)
   {
      AiMsgWarning("[mtoa] [translator %s] layeredTexture node has more than 16 inputs, only the first 16 will be handled", GetTranslatorName().asChar());
      numElements = 16;
   }

   AiNodeSetUInt(shader, "numInputs", numElements);

   ProcessParameter(shader, "alphaIsLuminance", AI_TYPE_BOOLEAN);
   
   MObject colorAttr = fnNode.attribute("color");
   MObject alphaAttr = fnNode.attribute("alpha");
   MObject blendModeAttr = fnNode.attribute("blendMode");
   MObject isVisibleAttr = fnNode.attribute("isVisible");

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
      AiNodeSetBool(shader, aiAttr, colorConnectedToAlpha ? true : false);

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
   return ProcessAOVOutput(AddArnoldNode("MayaLayeredShader"));
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
   
   MFnDependencyNode fnNode(GetMayaObject());
   attr = fnNode.findPlug("inputs");
   unsigned int numElements = attr.numElements();
   if (numElements > 16)
   {
      AiMsgWarning("[mtoa] [translator %s] LayeredShader node has more than 16 inputs, only the first 16 will be handled", GetTranslatorName().asChar());
      numElements = 16;
   }

   AiNodeSetUInt(shader, "numInputs", numElements);

   MObject colorAttr = fnNode.attribute("color");
   MObject transpAttr = fnNode.attribute("transparency");

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
      AiNodeSetBool(shader, aiAttr, useTransparency ? true : false);

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


// Displacement
//
AtNode*  CDisplacementTranslator::CreateArnoldNodes()
{
   MPlugArray connections;

   MPlug plug = FindMayaPlug("vectorDisplacement");

   plug.connectedTo(connections, true, false);

   if (connections.length() != 0)
   {
      return AddArnoldNode("MayaVectorDisplacement");
   }
   else
   {
      return AddArnoldNode("MayaNormalDisplacement");
   }
}

void CDisplacementTranslator::Export(AtNode* shader)
{
   ProcessParameter(shader, "displacement", AI_TYPE_FLOAT);
   ProcessParameter(shader, "vectorDisplacement", AI_TYPE_VECTOR);
   ProcessParameter(shader, "scale", AI_TYPE_FLOAT);
   ProcessParameter(shader, "vectorEncoding", AI_TYPE_INT);
   ProcessParameter(shader, "vectorSpace", AI_TYPE_INT);
   ProcessParameter(shader, "tangent", AI_TYPE_VECTOR);
   if (AiNodeIs(shader, "MayaNormalDisplacement"))
      ProcessParameter(shader, "zeroValue", AI_TYPE_FLOAT, "aiDisplacementZeroValue");
}

void DisplacementTranslatorNodeInitializer(CAbTranslator context)
{
   CExtensionAttrHelper helper("displacementShader");
   
   CAttrData data;
   
   data.defaultValue.FLT = 0.f;
   data.name = "aiDisplacementPadding";
   data.shortName = "ai_displacement_padding";
   helper.MakeInputFloat(data);
   
   data.defaultValue.FLT = 0.f;
   data.name = "aiDisplacementZeroValue";
   data.shortName = "ai_displacement_zero_value";
   helper.MakeInputFloat(data);
   
   data.defaultValue.BOOL = true;
   data.name = "aiDisplacementAutoBump";
   data.shortName = "ai_displacement_auto_bump";
   helper.MakeInputBoolean(data);
}

void CMayaBlinnTranslator::Export(AtNode* shader)
{
   ProcessParameter(shader, "Kd", AI_TYPE_FLOAT, "diffuse");
   ProcessParameter(shader, "Kd_color", AI_TYPE_RGB, "color");
   
   ProcessParameter(shader, "Ks", AI_TYPE_FLOAT, "specularRollOff");
   ProcessParameter(shader, "specular_roughness", AI_TYPE_FLOAT, "eccentricity");
   ProcessParameter(shader, "Ks_color", AI_TYPE_RGB, "specularColor");
   
   ProcessParameter(shader, "Kr", AI_TYPE_FLOAT, "reflectivity");
   ProcessParameter(shader, "Kr_color", AI_TYPE_RGB, "reflectedColor");
   
   AiNodeSetFlt(shader, "emission", 1.f);
   ProcessParameter(shader, "emission_color", AI_TYPE_RGB, "incandescence");
}

AtNode* CMayaBlinnTranslator::CreateArnoldNodes()
{
   return ProcessAOVOutput(AddArnoldNode("standard"));
}

void CMayaPhongTranslator::Export(AtNode* shader)
{
   ProcessParameter(shader, "Kd", AI_TYPE_FLOAT, "diffuse");
   ProcessParameter(shader, "Kd_color", AI_TYPE_RGB, "color");
   
   AiNodeSetFlt(shader, "Ks", 1.f);
   MPlug cosinePowerPlug = FindMayaPlug("cosinePower");
   float rougness = sqrtf(1.0f / (0.454f * cosinePowerPlug.asFloat() + 3.357f));
   AiNodeSetFlt(shader, "specular_roughness", rougness);
   ProcessParameter(shader, "Ks_color", AI_TYPE_RGB, "specularColor");
   
   ProcessParameter(shader, "Kr", AI_TYPE_FLOAT, "reflectivity");
   ProcessParameter(shader, "Kr_color", AI_TYPE_RGB, "reflectedColor");
   
   AiNodeSetFlt(shader, "emission", 1.f);
   ProcessParameter(shader, "emission_color", AI_TYPE_RGB, "incandescence");
}

AtNode* CMayaPhongTranslator::CreateArnoldNodes()
{
   return ProcessAOVOutput(AddArnoldNode("standard"));
}

void CAiHairTranslator::NodeInitializer(CAbTranslator context)
{
   CExtensionAttrHelper helper("aiHair");
   
   CAttrData data;

   data.name = "aiEnableMatte";
   data.shortName = "ai_enable_matte";
   data.defaultValue.BOOL = false;
   helper.MakeInputBoolean(data);

   data.name = "aiMatteColor";
   data.shortName = "ai_matte_color";
   data.defaultValue.RGB = AI_RGB_BLACK;
   helper.MakeInputRGB(data);
   
   data.name = "aiMatteColorA";
   data.shortName = "ai_matte_color_a";
   data.hasMin = true;
   data.min.FLT = 0.f;
   data.hasMax = true;
   data.max.FLT = 1.0;
   data.defaultValue.FLT = 0.0f;
   helper.MakeInputFloat(data);   
}

AtNode* CAiHairTranslator::CreateArnoldNodes()
{
   return AddArnoldNode("hair");
}

AtNode* CAiImageTranslator::CreateArnoldNodes()
{
   return AddArnoldNode("image");
}

void CAiImageTranslator::Export(AtNode* image)
{
   CShaderTranslator::Export(image);
   if (AiNodeGetLink(image, "filename") == 0)
   {
      CRenderOptions renderOptions; 
      renderOptions.SetArnoldRenderOptions(GetArnoldRenderOptions()); 
      renderOptions.GetFromMaya(); 
      MString filename(AiNodeGetStr(image, "filename"));
      filename = filename.expandEnvironmentVariablesAndTilde();

      // do not call makeTx if we're doing a swatch rendering or material view
      int sessionMode = m_session->GetSessionMode();
      if (sessionMode != MTOA_SESSION_MATERIALVIEW && sessionMode != MTOA_SESSION_SWATCH &&
         sessionMode != MTOA_SESSION_UNDEFINED)
      {
         if (renderOptions.autoTx() && FindMayaPlug("autoTx").asBool())
         {
            MString colorSpace = FindMayaPlug("colorSpace").asString();
            int createdFiles = 0;
            int skippedFiles = 0;
            int errorFiles = 0;
            makeTx(filename, colorSpace, &createdFiles, &skippedFiles, &errorFiles);
            if (createdFiles + skippedFiles + errorFiles == 0)
            {
               // no file has been found
               // let's try with the search paths
               const MStringArray &searchPaths = m_session->GetTextureSearchPaths();
               for (unsigned int i = 0; i < searchPaths.length(); ++i)
               {
                  MString searchFilename = searchPaths[i] + filename;
                  makeTx(searchFilename, colorSpace, &createdFiles, &skippedFiles, &errorFiles);
                  if (createdFiles + skippedFiles + errorFiles > 0) break; // textures have been found with this search path. Let's stop looking for them
               }
            }
         }
      }

      // FIXME : we should append the .tx to the full filename without removing the original extension

      if(renderOptions.useExistingTiledTextures()) 
      {
         MString txFilename(filename.substring(0, filename.rindexW(".")) + MString("tx"));
         MStringArray expandedFilenames = expandFilename(txFilename);

         if(expandedFilenames.length() == 0)
         {
            // No file was found for this filename
            // we should check in the search paths
            const MStringArray &searchPaths = m_session->GetTextureSearchPaths();
         
            for (unsigned int i = 0; i < searchPaths.length(); ++i)
            {
               MString searchFilename = searchPaths[i] + "/" + txFilename;
               expandedFilenames = expandFilename(searchFilename);

               // we found the texture, stop searching
               if (expandedFilenames.length() > 0) break;
            }
         }
         // if expandedFilenames.length >= 1 then we're OK ?
         if (expandedFilenames.length() > 0)
         {
            filename = txFilename;
         }

      }
      m_session->FormatTexturePath(filename);
      AiNodeSetStr(image, "filename", filename.asChar());
   }
}

void CAiImageTranslator::NodeInitializer(CAbTranslator context)
{
   CExtensionAttrHelper helper(context.maya, "image");
   CAttrData data;
   data.defaultValue.BOOL = true;
   data.name = "autoTx";
   data.shortName = "autotx";
   helper.MakeInputBoolean(data);

   data.defaultValue.BOOL = false;
   data.name = "colorManagementConfigFileEnabled";
   data.shortName = "cmcf";
   helper.MakeInputBoolean(data);

   data.defaultValue.STR = "";
   data.name = "colorManagementConfigFilePath";
   data.shortName = "cmcp";
   helper.MakeInputString(data);

   data.defaultValue.BOOL = false;
   data.name = "colorManagementEnabled";
   data.shortName = "cme";
   helper.MakeInputBoolean(data);

   data.defaultValue.INT = 0;
   data.name = "colorProfile";
   data.shortName = "cp";
   helper.MakeInputInt(data);

   data.defaultValue.STR = "";
   data.name = "colorSpace";
   data.shortName = "cs";
   helper.MakeInputString(data);

   data.defaultValue.STR = "";
   data.name = "workingSpace";
   data.shortName = "ws";
   helper.MakeInputString(data);

/* 
   // In case we need to have an attribute named imageName
   // so that it's recognized to update color spaces
   
   data.defaultValue.STR = "";
   data.name = "imageName";
   data.shortName = "in";
   helper.MakeInputString(data);
*/
   data.defaultValue.BOOL = false;
   data.name = "ignoreColorSpaceFileRules";
   data.shortName = "ifr";
   helper.MakeInputBoolean(data);
}

CMayaShadingSwitchTranslator::CMayaShadingSwitchTranslator(const char* nodeType, int paramType) : m_nodeType(nodeType), m_paramType(paramType)
{

}

void CMayaShadingSwitchTranslator::Export(AtNode* shadingSwitch)
{
   ProcessParameter(shadingSwitch, "default", m_paramType, "default");
   std::vector<AtNode*> inputs;
   std::vector<AtNode*> shapes;

   MFnDependencyNode dnode(GetMayaObject());

   MPlug inputPlug = dnode.findPlug("input");
   MIntArray existingIndices;
   inputPlug.getExistingArrayAttributeIndices(existingIndices);
   if (existingIndices.length() == 0)
      return;
   for (unsigned int i = 0; i < existingIndices.length(); ++i)
   {
      MPlug currentInputPlug = inputPlug.elementByLogicalIndex(existingIndices[i]);      
      MPlug shapePlug = currentInputPlug.child(1);
      MPlugArray conns;
      shapePlug.connectedTo(conns, true, false);
      if (conns.length() == 0)
         continue;
      MPlug inputShapePlug = conns[0];
      MPlug shaderPlug = currentInputPlug.child(0);      
      shaderPlug.connectedTo(conns, true, false);
      if (conns.length() == 0)
         continue;
      MPlug inputShaderPlug = conns[0];
      AtNode* shader = ExportNode(inputShaderPlug);
      if (shader == 0)
         continue;
      AtNode* shape = ExportNode(inputShapePlug);
      if (shape == 0)
         continue;
      inputs.push_back(shader);
      shapes.push_back(shape);
   }
   if (inputs.size() == 0)
      return;
   AiNodeSetArray(shadingSwitch, "inputs", AiArrayConvert((unsigned int)inputs.size(), 1, AI_TYPE_NODE, &inputs[0]));
   AiNodeSetArray(shadingSwitch, "shapes", AiArrayConvert((unsigned int)shapes.size(), 1, AI_TYPE_NODE, &shapes[0]));
}

AtNode* CMayaShadingSwitchTranslator::CreateArnoldNodes()
{
   return AddArnoldNode(m_nodeType.c_str());
}

void* CreateSingleShadingSwitchTranslator()
{
   return new CMayaShadingSwitchTranslator("MayaSingleShadingSwitch", AI_TYPE_FLOAT);
}

void* CreateDoubleShadingSwitchTranslator()
{
   return new CMayaShadingSwitchTranslator("MayaDoubleShadingSwitch", AI_TYPE_POINT2);
}

void* CreateTripleShadingSwitchTranslator()
{
   return new CMayaShadingSwitchTranslator("MayaTripleShadingSwitch", AI_TYPE_RGB);
}

void* CreateQuadShadingSwitchTranslator()
{
   return new CMayaShadingSwitchTranslator("MayaQuadShadingSwitch", AI_TYPE_RGBA);
}
