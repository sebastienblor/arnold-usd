#include "ShaderTranslators.h"
#include "platform/Platform.h"
#include "../../common/UnorderedContainer.h"

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
#include <maya/MMatrix.h>

#include <maya/MColor.h>
#include <maya/MTransformationMatrix.h>
#include <maya/MEulerRotation.h>
#include <maya/MAngle.h>
#include <maya/MSelectionList.h>
#include <maya/MRenderUtil.h>

#include <string>
#include <fstream>
#include <iostream>

#include <utils/MayaUtils.h>


static const MStringArray SHADER_INVALID_COMPONENTS;
static const char* shaderRgbComp[3] = {"r", "g", "b"};
static const MStringArray SHADER_RGB_COMPONENTS(shaderRgbComp, 3);
static const char* shaderRgbaComp[4] = {"r", "g", "b", "a"};
static const MStringArray SHADER_RGBA_COMPONENTS(shaderRgbaComp, 4);
const char* shaderPoint2Comp[2] = {"x", "y"};
const MStringArray SHADER_VECTOR2_COMPONENTS(shaderPoint2Comp, 2);
const char* shaderVectorComp[3] = {"x", "y", "z"};
const MStringArray SHADER_VECTOR_COMPONENTS(shaderVectorComp, 3);

const MStringArray& GetShaderComponentNames(int arnoldParamType)
{
   MStringArray componentNames;
   switch (arnoldParamType)
   {
   case AI_TYPE_RGB:
      return SHADER_RGB_COMPONENTS;
   case AI_TYPE_RGBA:
      return SHADER_RGBA_COMPONENTS;
   case AI_TYPE_VECTOR:
      return SHADER_VECTOR_COMPONENTS;
   case AI_TYPE_VECTOR2:
      return SHADER_VECTOR2_COMPONENTS;
   // FIXME Arnold5 make sure we don't want any components with closures
   default:
      return SHADER_INVALID_COMPONENTS;
   }
}

// returns 0-3 or -1 if failed
int GetShaderFloatComponentIndex(const MPlug &plug)
{
   MStatus stat;
   MFnNumericAttribute nAttr(plug.attribute(), &stat);
   if (stat == MS::kSuccess && nAttr.unitType() == MFnNumericData::kFloat)
   {
      if (plug.isChild())
      {
         MPlug parentPlug = plug.parent();
         if (parentPlug.numChildren() > 3)
            // TODO: print warning
            return -1;
         for (unsigned int i=0; i < parentPlug.numChildren(); ++i)
         {
            if (parentPlug.child(i) == plug)
               return i;
         }
         return -1;
      }
      else if (plug.partialName(false, false, false, false, false, true) == OUT_ALPHA_NAME)
      {
         return 3;
      }
   }
   return -1;
}

MString GetShaderComponentName(int arnoldParamType, const MPlug &plug)
{
   const MStringArray names = GetShaderComponentNames(arnoldParamType);
   int index = GetShaderFloatComponentIndex(plug);

   if (index >= 0 && (unsigned int)index < names.length())
      return names[index];
   return "";
}


static void ShaderComputeLink(CShaderTranslator *translator, const MPlug &plug, AtNode *shader, const char *attr)
{
   AtNode *target = translator->ExportConnectedNode(plug);
   if (plug.isChild())
   {
      int outputType = AiNodeEntryGetOutputType(AiNodeGetNodeEntry(target));
      MString component = GetShaderComponentName(outputType,plug);
      AiNodeLinkOutput(target, component.asChar(), shader, attr);
   } else
      AiNodeLink(target, attr, shader);
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
   return AddArnoldNode("lambert");
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
         AtNode* inNode = ExportConnectedNode(connections[0]);
         // Need to reverse it
         if (inNode != NULL)
         {
            MString tag = "transparency";
            AtNode* reverseNode = GetArnoldNode(tag.asChar());
            if (reverseNode == NULL)
               reverseNode = AddArnoldNode("complement", tag.asChar());
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
   return AddArnoldNode("physical_sky");
}

void CPhysicalSkyTranslator::Export(AtNode* shader)
{
   // All physical sky attributes are not linkable in Arnold
   MStatus status;
   
   // We used to call ProcessConstantParameter, but now we can simply tag these attributes as Non-Linkable in the metadata instead

   ProcessParameter(shader, "turbidity", AI_TYPE_FLOAT, "turbidity");
   
   MPlug plug = FindMayaPlug("ground_albedo", &status);
   AiNodeSetRGB(shader, "ground_albedo", plug.child(0).asFloat(), plug.child(1).asFloat(), plug.child(2).asFloat());
   
   ProcessParameter(shader, "use_degrees", AI_TYPE_BOOLEAN, "use_degrees");
   ProcessParameter(shader, "elevation", AI_TYPE_FLOAT, "elevation");
   ProcessParameter(shader, "azimuth", AI_TYPE_FLOAT, "azimuth");
   
   plug = FindMayaPlug("sun_direction", &status);
   AiNodeSetVec(shader, "sun_direction", plug.child(0).asFloat(), plug.child(1).asFloat(), plug.child(2).asFloat());
   
   ProcessParameter(shader, "enable_sun", AI_TYPE_BOOLEAN, "enable_sun");
   ProcessParameter(shader, "intensity", AI_TYPE_FLOAT, "intensity");
   
   plug = FindMayaPlug("sky_tint", &status);
   AiNodeSetRGB(shader, "sky_tint", plug.child(0).asFloat(), plug.child(1).asFloat(), plug.child(2).asFloat());
   
   plug = FindMayaPlug("sun_tint", &status);
   AiNodeSetRGB(shader, "sun_tint", plug.child(0).asFloat(), plug.child(1).asFloat(), plug.child(2).asFloat());
   
   ProcessParameter(shader, "sun_size", AI_TYPE_FLOAT, "sun_size");
   
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
   m_hasColorCorrect = RequiresColorCorrect();
   m_hasUvTransform = RequiresUvTransform();
   
   AtNode *imageNode = AddArnoldNode("image");
   AtNode *colorCorrectNode = m_hasColorCorrect ? AddArnoldNode("color_correct", "cc") : NULL;
   AtNode *uvTransformNode = m_hasUvTransform ? AddArnoldNode("uv_transform", "uv") : NULL;
   
   if (colorCorrectNode)
      AiNodeLink(imageNode, "input", colorCorrectNode);
   
   if (uvTransformNode)
   {
      AiNodeLink((colorCorrectNode) ? colorCorrectNode : imageNode, "passthrough", uvTransformNode);
      return uvTransformNode;
   }

   // FIXME isolate selected in ARV is going to ignore color_correct & uv_transform

   return (colorCorrectNode) ? colorCorrectNode : imageNode;
}

void CFileTranslator::Export(AtNode* shader)
{
   MPlugArray connections;
   const CSessionOptions &options = GetSessionOptions();

   AtNode *colorCorrectNode = (m_hasColorCorrect) ? GetArnoldNode("cc") : NULL;
   AtNode *uvTransformNode = (m_hasUvTransform) ? GetArnoldNode("uv") : NULL;
   MPlug plug = FindMayaPlug("uvCoord");
   plug.connectedTo(connections, true, false);

   if (connections.length() != 0)
   {
      MObject srcObj = connections[0].node();
      MFnDependencyNode srcNodeFn(srcObj);

      if (srcNodeFn.typeName() == "place2dTexture")
      {
         srcNodeFn.findPlug("uvCoord", true).connectedTo(connections, true, false);
         AiNodeSetStr(shader, "uvset", "");
         if (connections.length() > 0)
         {
            MFnDependencyNode uvcNodeFn(connections[0].node());
            if (uvcNodeFn.typeName() == "uvChooser")
               AiNodeSetStr(shader, "uvset", uvcNodeFn.findPlug("uvSets", true).elementByPhysicalIndex(0).asString().asChar());
         }
         if (uvTransformNode)
         {
            // we need to set the UV controls in the uv_transform node
            AiNodeSetStr(uvTransformNode, "uvset", AiNodeGetStr(shader, "uvset"));
            if (connections.length() > 0)
               AiNodeResetParameter(shader, "uvset");

            ProcessParameter(uvTransformNode, "coverage", AI_TYPE_VECTOR2, srcNodeFn.findPlug("coverage", true));
            ProcessParameter(uvTransformNode, "mirror_u", AI_TYPE_BOOLEAN, srcNodeFn.findPlug("mirrorU", true));
            ProcessParameter(uvTransformNode, "mirror_v", AI_TYPE_BOOLEAN, srcNodeFn.findPlug("mirrorV", true));

            if (srcNodeFn.findPlug("wrapU", true).asBool())
               AiNodeSetStr(uvTransformNode, "wrap_frame_u", "periodic");
            else
               AiNodeSetStr(uvTransformNode, "wrap_frame_u", "color");
               

            if (srcNodeFn.findPlug("wrapV", true).asBool())
               AiNodeSetStr(uvTransformNode, "wrap_frame_v", "periodic");
            else
               AiNodeSetStr(uvTransformNode, "wrap_frame_v", "color");
            

            ProcessParameter(uvTransformNode, "wrap_frame_color", AI_TYPE_RGBA, "defaultColor");   
            if (!AiNodeIsLinked(uvTransformNode, "wrap_frame_color")) // Force a transparent alpha on the defaultColor
            {
               AtRGBA col = AiNodeGetRGBA(uvTransformNode, "wrap_frame_color");
               AiNodeSetRGBA(uvTransformNode, "wrap_frame_color", col.r, col.g, col.b, 0.f);
            }
            // if not linked, set alpha to zero
            ProcessParameter(uvTransformNode, "repeat", AI_TYPE_VECTOR2, srcNodeFn.findPlug("repeatUV", true));
            ProcessParameter(uvTransformNode, "offset", AI_TYPE_VECTOR2, srcNodeFn.findPlug("offset", true));

            float rotateFrame = srcNodeFn.findPlug("rotateFrame", true).asFloat();
            AiNodeSetFlt(uvTransformNode, "rotate_frame", rotateFrame * 180.f / AI_PI);
            //ProcessParameter(uvTransformNode, "rotate_frame", AI_TYPE_FLOAT, srcNodeFn.findPlug("rotateFrame"));
            ProcessParameter(uvTransformNode, "translate_frame", AI_TYPE_VECTOR2, srcNodeFn.findPlug("translateFrame", true));
            float rotateUV = srcNodeFn.findPlug("rotateUV", true).asFloat();
            AiNodeSetFlt(uvTransformNode, "rotate", rotateUV * 180.f / AI_PI);
            ProcessParameter(uvTransformNode, "stagger", AI_TYPE_BOOLEAN, srcNodeFn.findPlug("stagger", true));
            ProcessParameter(uvTransformNode, "noise", AI_TYPE_VECTOR2, srcNodeFn.findPlug("noiseUV", true));

         } else
         {
            if (srcNodeFn.findPlug("wrapU", true).asBool())
            {
               if (srcNodeFn.findPlug("mirrorU", true).asBool())
                  AiNodeSetStr(shader, "swrap", "mirror");
               else
                  AiNodeSetStr(shader, "swrap", "periodic");
               
            } else
               AiNodeSetStr(shader, "swrap", "missing");

            if (srcNodeFn.findPlug("wrapV", true).asBool())
            {
               if (srcNodeFn.findPlug("mirrorV", true).asBool())
                  AiNodeSetStr(shader, "twrap", "mirror");
               else
                  AiNodeSetStr(shader, "twrap", "periodic");
               
            } else
               AiNodeSetStr(shader, "twrap", "missing");
            
            MPlug repeatUVPlug = srcNodeFn.findPlug("repeatUV", true);
            if (!repeatUVPlug.isNull())
            {
               AtVector2 repeatUV = AtVector2(repeatUVPlug.child(0).asFloat(), repeatUVPlug.child(1).asFloat());
               AiNodeSetFlt(shader, "sscale", repeatUV.x);
               AiNodeSetFlt(shader, "tscale", repeatUV.y);
            }
            MPlug offsetUVPlug = srcNodeFn.findPlug("offset", true);
            if (!offsetUVPlug.isNull())
            {
               AiNodeSetFlt(shader, "soffset", offsetUVPlug.child(0).asFloat());
               AiNodeSetFlt(shader, "toffset", offsetUVPlug.child(1).asFloat());
            }
         }

      }
   }

   MString prevFilename = AiNodeGetStr(shader, "filename").c_str();
   
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

      options.FormatTexturePath(resolvedFilename);

      MString tokenStr = "<";
      int tokenIndex = resolvedFilename.indexW(tokenStr);
      if (tokenIndex >= 0)
      {
         // need to handle <f> tokens, in case they are combined with other (eventually arnold) tokens
         tokenStr = "<f>";
         tokenIndex = resolvedFilename.indexW(tokenStr);
         if (tokenIndex > 0)
         {
            // the MString frameNumber adds a '0' before the frame value.
            // Do we really want that ? doesn't make much sense....so well, removing it here
            int fileFrame = FindMayaPlug("useFrameExtension").asBool() ? FindMayaPlug("frameExtension").asInt() + FindMayaPlug("frameOffset").asInt() : (int)GetExportFrame();
            frameNumber = fileFrame;
            MString filenameExt = resolvedFilename.substringW(tokenIndex + 3, resolvedFilename.length() - 1);
            resolvedFilename = resolvedFilename.substringW(0, tokenIndex - 1) + frameNumber + filenameExt;
         }
         tokenStr = "<shapeName>";
         MString tokenOut = "<attr:name>";
         ReplaceFileToken(resolvedFilename, tokenStr, tokenOut);
         tokenStr = "<shapePath>";
         ReplaceFileToken(resolvedFilename, tokenStr, tokenOut);
         tokenStr = "<UDIM";
         tokenOut = "<udim";
         ReplaceFileToken(resolvedFilename, tokenStr, tokenOut);
         tokenStr = "<utile>"; 
         tokenOut = "<utile:1>";
         ReplaceFileToken(resolvedFilename, tokenStr, tokenOut);
         // #3413 <u> <v> tokens are expected to be 0-indexed. Should this be extended to other tiling modes ?
         if (tilingMode == 1) 
            tokenOut = "<utile>";
         tokenStr = "<u>";
         ReplaceFileToken(resolvedFilename, tokenStr, tokenOut);
         tokenStr = "<U>";
         ReplaceFileToken(resolvedFilename, tokenStr, tokenOut);
         tokenStr = "<vtile>";
         tokenOut = "<vtile:1>";
         ReplaceFileToken(resolvedFilename, tokenStr, tokenOut);
         
         // #3413 <u> <v> tokens are expected to be 0-indexed. Should this be extended to other tiling modes ?
         if (tilingMode == 1)
            tokenOut = "<vtile>";
         tokenStr = "<v>";
         ReplaceFileToken(resolvedFilename, tokenStr, tokenOut);
         tokenStr = "<V>";
         ReplaceFileToken(resolvedFilename, tokenStr, tokenOut);
      }

      MString colorSpace = FindMayaPlug("colorSpace").asString();
      
      // if the color space has changed, we'll need to re-generate TX anyway
      bool requestUpdateTx = (colorSpace != m_colorSpace);
      m_colorSpace = colorSpace; // setting current value for next time

      if (!requestUpdateTx)
      {
         // Color Space is the same, so let's check if the filename was modified
         int prevFilenameLength = prevFilename.length();

         if (prevFilenameLength > 0)
         {
            // compare against previous filename to see if we need to re-generate the TX
            if (prevFilenameLength > 3 && prevFilename.substring(prevFilenameLength - 3, prevFilenameLength - 1) == MString(".tx"))
            {
               // Previous Filename was .tx, either because of "use existing tx", 
               // or because it's explicitely targeting the .tx
               MString prevBasename = prevFilename.substring(0, prevFilenameLength - 4);

               int dotPos = resolvedFilename.rindexW(".");
               if (dotPos > 0)
               {
                  MString basename = resolvedFilename.substring(0, dotPos - 1);
                  
                  // Let's compare the basenames (without extension)
                  if (prevBasename != basename)
                  {
                     // the basename was modified, this needs an update of TX
                     requestUpdateTx = true;
                  } else
                  {
                     //basename hasn't changed. However, I'm probably setting it back to non-tx here
                     // so let's keep the previous one (where Use Tx was applied)
                     resolvedFilename = prevFilename;
                  }
               }
            } else
            {
               // if previous filename and new one are exactly identical, it's useless to update Tx
               requestUpdateTx = (prevFilename != resolvedFilename);
            }
         } else if (resolvedFilename.length() > 0)
         {
            requestUpdateTx = true;
         }
      }

      AiNodeSetStr(shader, "filename", resolvedFilename.asChar()); 

      // only set the color_space if the texture isn't a TX
      AiNodeSetStr(shader, "color_space", "");
      if (resolvedFilename.length() > 4)
      {
         MString extension = resolvedFilename.substring(resolvedFilename.length() - 3, resolvedFilename.length() - 1);

         if (extension != ".tx" && extension !=  ".TX")
            AiNodeSetStr(shader, "color_space", colorSpace.asChar());
      }

      if (requestUpdateTx) RequestTxUpdate();
   } 

   ProcessParameter(shader, "mipmap_bias", AI_TYPE_INT, "aiMipBias");
   AiNodeSetInt(shader, "filter", FindMayaPlug("aiFilter").asInt());
  
   // FIXME : in Maya File, the default color is also seen out of the UV range, when UV wrapping is disabled
   // In Arnold image node, the only choice we have is "black"
   AiNodeSetBool(shader, "ignore_missing_textures", FindMayaPlug("aiUseDefaultColor").asBool());
   ProcessParameter(shader, "missing_texture_color", AI_TYPE_RGBA, "defaultColor");
   if (!AiNodeIsLinked(shader, "missing_texture_color")) // Force a transparent alpha on the defaultColor
   {
      AtRGBA col = AiNodeGetRGBA(shader, "missing_texture_color");
      AiNodeSetRGBA(shader, "missing_texture_color", col.r, col.g, col.b, 0.f);
   }
   
   
   ProcessParameter(shader, "offset", AI_TYPE_RGB, "colorOffset");

   MPlug colorGainPlug = FindMayaPlug("colorGain");
   MPlug exposurePlug = FindMayaPlug("exposure");

   MPlugArray colorGainConnections, exposureConnections;
   if (!colorGainPlug.isNull())
      colorGainPlug.connectedTo(colorGainConnections, true, false);
   bool isColorGainConnected = (colorGainConnections.length() > 0);
   float exposure = 0.f;
   if(!exposurePlug.isNull())
   {
      exposurePlug.connectedTo(exposureConnections, true, false);
      exposure = exposurePlug.asFloat();
   }
   bool isExposureConnected = (exposureConnections.length() > 0);

   if (!isExposureConnected && (std::abs(exposure) < AI_EPSILON))
   {
      // exposure is left to zero. So we can just export colorGain
      ProcessParameter(shader, "multiply", AI_TYPE_RGB, "colorGain");
   } else if ((!isExposureConnected) && (!isColorGainConnected))
   {
      // no shading connections, just set the right value in multiply
      AtRGB colorGain = AtRGB(colorGainPlug.child(0).asFloat(), 
                              colorGainPlug.child(1).asFloat(), 
                              colorGainPlug.child(2).asFloat());
      colorGain *= powf(2.0f, exposurePlug.asFloat());
      AiNodeSetRGB(shader, "multiply", colorGain.r, colorGain.g, colorGain.b);
   } else
   {
      AtNode *compositeNode = GetArnoldNode("gain_exp");
      if (compositeNode == NULL)
         compositeNode = AddArnoldNode("color_correct", "gain_exp");

      AiNodeLink(compositeNode, "multiply", shader);
      AiNodeSetRGBA(compositeNode, "input", 1.f, 1.f, 1.f, 1.f);
      ProcessParameter(compositeNode, "multiply", AI_TYPE_RGB, "colorGain");
      ProcessParameter(compositeNode, "exposure", AI_TYPE_FLOAT, "exposure");
   }
   if (colorCorrectNode)
   {
      ProcessParameter(colorCorrectNode, "alpha_is_luminance", AI_TYPE_BOOLEAN, "alphaIsLuminance");
      // when "invert" is enabled, we want it to invert both the RGB and the alpha
      // this is done through 2 different attributes in color_correct
      ProcessParameter(colorCorrectNode, "invert", AI_TYPE_BOOLEAN, "invert");
      ProcessParameter(colorCorrectNode, "invert_alpha", AI_TYPE_BOOLEAN, "invert");
      ProcessParameter(colorCorrectNode, "alpha_multiply", AI_TYPE_FLOAT, "alphaGain");
      ProcessParameter(colorCorrectNode, "alpha_add", AI_TYPE_FLOAT, "alphaOffset");
   }
   

   /* Note that the following native file attributes are ignored :
      - filter type 
      - pre-filter 
      - effects filter
      - effects filter offset
      - effects invert
      - effects color remap
   */


}

void CFileTranslator::NodeChanged(MObject& node, MPlug& plug)
{
   MString plugName = plug.partialName(false, false, false, false, false, true);
   if ((plugName == "alphaGain" || plugName == "alphaOffset" || plugName == "alphaIsLuminance" || plugName == "invert") &&
      !RequiresColorCorrect())
      SetUpdateMode(AI_RECREATE_NODE);

   if ((plugName == "uvCoord") &&
      !RequiresUvTransform())
      SetUpdateMode(AI_RECREATE_NODE);

   if (plugName == "fileTextureName")
   {
      // We're receiving a signal from the filename parameter.
      // So we don't want to compare the filenames in the next export, we'll just want to update the TX.
      // In fact, the texture could have changed even though the filename is still the same (see  #3792)
      // We could rather store a flag for that, but for now we can reset the "previous" color space, 
      // as it will force a refresh
      m_colorSpace = "";
   }

   CShaderTranslator::NodeChanged(node, plug);
}

bool CFileTranslator::RequiresColorCorrect() const
{
   return ! (IsFloatAttrDefault(FindMayaPlug("alphaGain"), 1.f) &&
             IsFloatAttrDefault(FindMayaPlug("alphaOffset"), 0.f) &&
             IsBoolAttrDefault(FindMayaPlug("alphaIsLuminance"), false) &&
             IsBoolAttrDefault(FindMayaPlug("invert"), false));
   
}
bool CFileTranslator::RequiresUvTransform() const
{
   MPlugArray connections;
   MPlug plug = FindMayaPlug("uvCoord");
   plug.connectedTo(connections, true, false);

   if (connections.length() == 0)
      return false;

   MObject srcObj = connections[0].node();
   MFnDependencyNode srcNodeFn(srcObj);

   if (srcNodeFn.typeName() != "place2dTexture")
      return false;

   return !(IsBoolAttrDefault(srcNodeFn.findPlug("stagger", true), false) &&
            IsBoolAttrDefault(srcNodeFn.findPlug("mirrorU", true), false) &&
            IsBoolAttrDefault(srcNodeFn.findPlug("mirrorV", true), false) &&
            IsFloatAttrDefault(srcNodeFn.findPlug("rotateFrame", true), 0.f ) &&
            IsFloatAttrDefault(srcNodeFn.findPlug("rotateUV", true), 0.f ) &&
            IsVec2AttrDefault(srcNodeFn.findPlug("coverage", true), 1.f, 1.f ) &&
            IsVec2AttrDefault(srcNodeFn.findPlug("translateFrame", true), 0.f, 0.f ) &&
            IsVec2AttrDefault(srcNodeFn.findPlug("repeatUV", true), 1.f, 1.f ) &&
            IsVec2AttrDefault(srcNodeFn.findPlug("noiseUV", true), 0.f, 0.f ) );

}
void CFileTranslator::ReplaceFileToken(MString &filename, const MString &tokenIn, const MString &tokenOut)
{   
   int tokenIndex = filename.indexW(tokenIn);
   if ((tokenIndex) < 0) return; // token not found

   filename = filename.substringW(0, tokenIndex - 1) + 
      tokenOut + filename.substringW(tokenIndex + tokenIn.length(), filename.length() - 1);
}
void CFileTranslator::NodeInitializer(CAbTranslator context)
{

   CExtensionAttrHelper helper(context.maya, "image");

   helper.MakeInput("filter");

   CAttrData data;
   data.defaultValue.BOOL() = true;
   data.name = "aiAutoTx";
   data.shortName = "autotx";
   data.channelBox = false;
   data.keyable = false;       
   helper.MakeInputBoolean(data);

   data.defaultValue.INT() = 0;
   data.name = "aiMipBias";
   data.shortName = "ai_mipmap_bias";
   helper.MakeInputInt(data);

   data.defaultValue.BOOL() = true;
   data.name = "aiUseDefaultColor";
   data.shortName = "ai_ignore_missing_textures";
   helper.MakeInputBoolean(data);

}

// Bump2d
//
void CBump2DTranslator::NodeInitializer(CAbTranslator context)
{
   CExtensionAttrHelper helper(context.maya, "bump2d");
   
   CAttrData data;
   data.defaultValue.BOOL() = true;
   data.name = "aiFlipR";
   data.shortName = "flip_r";
   data.channelBox = false;
   data.keyable = false;       
   helper.MakeInputBoolean(data);
   
   data.name = "aiFlipG";
   data.shortName = "flip_g";
   helper.MakeInputBoolean(data);
   
   data.defaultValue.BOOL() = false;
   data.name = "aiSwapTangents";
   data.shortName = "swap_tangents";
   helper.MakeInputBoolean(data);
   
   data.defaultValue.BOOL() = true;
   data.name = "aiUseDerivatives";
   data.shortName = "use_derivatives";
   helper.MakeInputBoolean(data);
   
}

AtNode*  CBump2DTranslator::CreateArnoldNodes()
{
   MPlugArray connections;
   MPlug plug = FindMayaPlug("bumpValue");
   if (!plug.isNull())
   {
      plug.connectedTo(connections, true, false);
      if (connections.length() > 0)
      {
         MObject mayaNode = connections[0].node();
         if (!mayaNode.isNull())
         {
            MString typeName = MFnDependencyNode(mayaNode).typeName();
            if (typeName == "aiRoundCorners")
               return AddArnoldNode("add");
         }
      }
   }

   plug = FindMayaPlug("bumpInterp");
   if (!plug.isNull() && plug.asShort() > 0)
      return AddArnoldNode("normal_map");

   // check attribute useAs
   return AddArnoldNode("bump2d");
}

void CBump2DTranslator::Export(AtNode* shader)
{
   static const AtString normalMapStr("normal_map");
   static const AtString addStr("add");
   if (AiNodeIs(shader, normalMapStr))
   {
      // either Object-space or Tangent-space normal

      MPlug plug = FindMayaPlug("bumpInterp");
      if (!plug.isNull() && plug.asShort() == 1) 
      {
         // Tangent space normal
         AiNodeSetBool(shader, "tangent_space", true);
         AiNodeSetBool(shader, "color_to_signed", true); // remap normals from [0,1] to [-1,1]
         ProcessParameter(shader, "invert_x", AI_TYPE_BOOLEAN, "aiFlipR");
         ProcessParameter(shader, "invert_y", AI_TYPE_BOOLEAN, "aiFlipG");
         AiNodeResetParameter(shader, "invert_z");

         MPlug swapTangentPlug = FindMayaPlug("aiSwapTangents");
         if (!swapTangentPlug.isNull() && swapTangentPlug.asBool())
            AiNodeSetStr(shader, "order", "YXZ");
         else
            AiNodeResetParameter(shader, "order"); // XYZ

         AiNodeResetParameter(shader, "tangent");
         AiNodeResetParameter(shader, "normal");
         AiNodeResetParameter(shader, "strength");

         MPlug useDerivsPlug = FindMayaPlug("aiUseDerivatives");
         if (!useDerivsPlug.isNull() && !useDerivsPlug.asBool())
         {
            // do we really want to keep supporting this ??
            // Here the user decided to export derivatives but doesn't want to use them for this bump....
            // We can plug a state_vector returning dPdu in parameter tangent as a workaround.
            AtNode *tangentNode = GetArnoldNode("tangent");
            if (tangentNode == NULL)
               tangentNode = AddArnoldNode("state_vector", "tangent");

            AiNodeSetStr(tangentNode, "variable", "dPdu");
            AiNodeLink(tangentNode, "tangent", shader);
         }        

      } else
      {
         // Object space normal
         AiNodeSetBool(shader, "tangent_space", false);
         // note that normal isn't remapped in object space normal in maya
         AiNodeSetBool(shader, "color_to_signed", false);

         AiNodeResetParameter(shader, "tangent");
         AiNodeResetParameter(shader, "normal");
         AiNodeResetParameter(shader, "order");
         AiNodeResetParameter(shader, "invert_x");
         AiNodeResetParameter(shader, "invert_y");
         AiNodeResetParameter(shader, "invert_z");
         AiNodeResetParameter(shader, "strength");
      }

      // Bump value is a float parameter in maya bump, but it can be 
      // automagically interpreted as a vector for normal maps. 
      MPlugArray connections;
      plug = FindMayaPlug("bumpValue");
      plug.connectedTo(connections, true, false);
      AtNode *bumpMap = (connections.length() > 0) ? ExportConnectedNode(connections[0]) : NULL;

      if (bumpMap)
         AiNodeLink(bumpMap, "input", shader);
      else 
         AiNodeResetParameter(shader, "input");

   } else if (AiNodeIs(shader, addStr))
   {
      MPlugArray connections;
      MPlug plug = FindMayaPlug("bumpValue");
      plug.connectedTo(connections, true, false);
      AtNode *bumpMap = (connections.length() > 0) ? ExportConnectedNode(connections[0]) : NULL;

      if (bumpMap)
         AiNodeLink(bumpMap, "input1", shader);
      else 
         AiNodeResetParameter(shader, "input1");

   } else   
   {
      // Bump mode
      // just need to export bump_map & bump_height
      ProcessParameter(shader, "bump_map", AI_TYPE_FLOAT, "bumpValue");
      ProcessParameter(shader, "bump_height", AI_TYPE_FLOAT, "bumpDepth");      
   }
   // in any case, export the bump for arnold parameter "normal"
   ExportBump(shader);
}

void CBump2DTranslator::NodeChanged(MObject& node, MPlug& plug)
{
   // make sure we recreate the node in case "bumpInterp" changes, 
   // since it can change whether this node is exported as a bump2d or as a normal_map
   MString plugName =  plug.partialName(false, false, false, false, false, true);
   if (plugName == MString("bumpInterp"))
      SetUpdateMode(AI_RECREATE_NODE);

   CShaderTranslator::NodeChanged(node, plug);
}

/////////////////////////////////////////////
// Checkboard
//
AtNode*  CCheckerTranslator::CreateArnoldNodes()
{
   m_hasColorCorrect = RequiresColorCorrect();
   m_hasUvTransform = RequiresUvTransform();
   
   AtNode *checkerNode = AddArnoldNode("checkerboard");
   AtNode *colorCorrectNode = m_hasColorCorrect ? AddArnoldNode("color_correct", "cc") : NULL;
   AtNode *uvTransformNode = m_hasUvTransform ? AddArnoldNode("uv_transform", "uv") : NULL;
   
   if (colorCorrectNode)
      AiNodeLink(checkerNode, "input", colorCorrectNode);
   
   if (uvTransformNode)
   {
      AiNodeLink((colorCorrectNode) ? colorCorrectNode : checkerNode, "passthrough", uvTransformNode);
      return uvTransformNode;
   }
   // FIXME: Isolate selected in ARV is going to ignore color_correct and uv_transform
   return (colorCorrectNode) ? colorCorrectNode : checkerNode;
}

void CCheckerTranslator::Export(AtNode* shader)
{
   MPlugArray connections;
   
   AtNode *colorCorrectNode = (m_hasColorCorrect) ? GetArnoldNode("cc") : NULL;
   AtNode *uvTransformNode = (m_hasUvTransform) ? GetArnoldNode("uv") : NULL;
   MPlug plug = FindMayaPlug("uvCoord");
   plug.connectedTo(connections, true, false);

   if (connections.length() != 0)
   {
      MObject srcObj = connections[0].node();
      MFnDependencyNode srcNodeFn(srcObj);

      if (srcNodeFn.typeName() == "place2dTexture")
      {
         srcNodeFn.findPlug("uvCoord", true).connectedTo(connections, true, false);
         AiNodeSetStr(shader, "uvset", "");
         if (connections.length() > 0)
         {
            MFnDependencyNode uvcNodeFn(connections[0].node());
            if (uvcNodeFn.typeName() == "uvChooser")
               AiNodeSetStr(shader, "uvset", uvcNodeFn.findPlug("uvSets", true).elementByPhysicalIndex(0).asString().asChar());
         }
         if (uvTransformNode)
         {
            // we need to set the UV controls in the uv_transform node
            AiNodeSetStr(uvTransformNode, "uvset", AiNodeGetStr(shader, "uvset"));
            // if the uvset was set on the uv_transform node, the actual shader should now look at the default coordinates
            AiNodeSetStr(shader, "uvset", ""); 
            ProcessParameter(uvTransformNode, "coverage", AI_TYPE_VECTOR2, srcNodeFn.findPlug("coverage", true));
            ProcessParameter(uvTransformNode, "mirror_u", AI_TYPE_BOOLEAN, srcNodeFn.findPlug("mirrorU", true));
            ProcessParameter(uvTransformNode, "mirror_v", AI_TYPE_BOOLEAN, srcNodeFn.findPlug("mirrorV", true));

            if (srcNodeFn.findPlug("wrapU", true).asBool())
               AiNodeSetStr(uvTransformNode, "wrap_frame_u", "none");
            else
               AiNodeSetStr(uvTransformNode, "wrap_frame_u", "color");
               

            if (srcNodeFn.findPlug("wrapV", true).asBool())
               AiNodeSetStr(uvTransformNode, "wrap_frame_v", "none");
            else
               AiNodeSetStr(uvTransformNode, "wrap_frame_v", "color");
            

            ProcessParameter(uvTransformNode, "wrap_frame_color", AI_TYPE_RGBA, "defaultColor");   
            if (!AiNodeIsLinked(uvTransformNode, "wrap_frame_color")) // Force a transparent alpha on the defaultColor
            {
               AtRGBA col = AiNodeGetRGBA(uvTransformNode, "wrap_frame_color");
               AiNodeSetRGBA(uvTransformNode, "wrap_frame_color", col.r, col.g, col.b, 0.f);
            }
            // if not linked, set alpha to zero
            ProcessParameter(uvTransformNode, "repeat", AI_TYPE_VECTOR2, srcNodeFn.findPlug("repeatUV", true));
            ProcessParameter(uvTransformNode, "offset", AI_TYPE_VECTOR2, srcNodeFn.findPlug("offset", true));

            float rotateFrame = srcNodeFn.findPlug("rotateFrame", true).asFloat();
            AiNodeSetFlt(uvTransformNode, "rotate_frame", rotateFrame * 180.f / AI_PI);
            //ProcessParameter(uvTransformNode, "rotate_frame", AI_TYPE_FLOAT, srcNodeFn.findPlug("rotateFrame"));
            ProcessParameter(uvTransformNode, "translate_frame", AI_TYPE_VECTOR2, srcNodeFn.findPlug("translateFrame", true));
            float rotateUV = srcNodeFn.findPlug("rotateUV", true).asFloat();
            AiNodeSetFlt(uvTransformNode, "rotate", rotateUV * 180.f / AI_PI);
            ProcessParameter(uvTransformNode, "stagger", AI_TYPE_BOOLEAN, srcNodeFn.findPlug("stagger", true));
            ProcessParameter(uvTransformNode, "noise", AI_TYPE_VECTOR2, srcNodeFn.findPlug("noiseUV", true));

         } else
         {
            MPlug repeatUVPlug = srcNodeFn.findPlug("repeatUV", true);
            if (!repeatUVPlug.isNull())
            {
               AtVector2 repeatUV = AtVector2(repeatUVPlug.child(0).asFloat(), repeatUVPlug.child(1).asFloat());
               AiNodeSetFlt(shader, "u_frequency", repeatUV.x);
               AiNodeSetFlt(shader, "v_frequency", repeatUV.y);
            }
            MPlug offsetUVPlug = srcNodeFn.findPlug("offset", true);
            if (!offsetUVPlug.isNull())
            {
               AiNodeSetFlt(shader, "u_offset", offsetUVPlug.child(0).asFloat());
               AiNodeSetFlt(shader, "v_offset", offsetUVPlug.child(1).asFloat());
            }
         }

      }
   }
  
   ProcessParameter(shader, "color1", AI_TYPE_RGB, "color2");
   ProcessParameter(shader, "color2", AI_TYPE_RGB, "color1");
   ProcessParameter(shader, "contrast", AI_TYPE_FLOAT);
   ProcessParameter(shader, "filter_strength", AI_TYPE_FLOAT, "filter");
   ProcessParameter(shader, "filter_offset", AI_TYPE_FLOAT, "filterOffset");
   
   if (colorCorrectNode)
   {
      ProcessParameter(colorCorrectNode, "alpha_is_luminance", AI_TYPE_BOOLEAN, "alphaIsLuminance");
      // when "invert" is enabled, we want it to invert both the RGB and the alpha
      // this is done through 2 different attributes in color_correct
      ProcessParameter(colorCorrectNode, "invert", AI_TYPE_BOOLEAN, "invert");
      ProcessParameter(colorCorrectNode, "alpha_multiply", AI_TYPE_FLOAT, "alphaGain");
      ProcessParameter(colorCorrectNode, "alpha_add", AI_TYPE_FLOAT, "alphaOffset");
      ProcessParameter(colorCorrectNode, "multiply", AI_TYPE_RGB, "colorGain");
      ProcessParameter(colorCorrectNode, "add", AI_TYPE_RGB, "colorOffset");
   } 
}

void CCheckerTranslator::NodeChanged(MObject& node, MPlug& plug)
{
   MString plugName = plug.partialName(false, false, false, false, false, true);
   if ((plugName == "alphaGain" || plugName == "alphaOffset" || plugName == "alphaIsLuminance" || 
      plugName == "invert" || plugName == "colorGain" || plugName == "colorOffset") &&
      !RequiresColorCorrect())
      SetUpdateMode(AI_RECREATE_NODE);

if ((plugName == "uvCoord") &&
      !RequiresUvTransform())
      SetUpdateMode(AI_RECREATE_NODE);

   CShaderTranslator::NodeChanged(node, plug);
}

bool CCheckerTranslator::RequiresColorCorrect() const
{
   return ! (IsFloatAttrDefault(FindMayaPlug("alphaGain"), 1.f) &&
             IsFloatAttrDefault(FindMayaPlug("alphaOffset"), 0.f) &&
             IsBoolAttrDefault(FindMayaPlug("alphaIsLuminance"), false) &&
             IsBoolAttrDefault(FindMayaPlug("invert"), false) &&
             IsRGBAttrDefault(FindMayaPlug("colorGain"), 1.f, 1.f, 1.f) &&
             IsRGBAttrDefault(FindMayaPlug("colorOffset"), 0.f, 0.f, 0.f));
   
}
bool CCheckerTranslator::RequiresUvTransform() const
{
   MPlugArray connections;
   MPlug plug = FindMayaPlug("uvCoord");
   plug.connectedTo(connections, true, false);

   if (connections.length() == 0)
      return false;

   MObject srcObj = connections[0].node();
   MFnDependencyNode srcNodeFn(srcObj);

   if (srcNodeFn.typeName() != "place2dTexture")
      return false;

   return !(IsBoolAttrDefault(srcNodeFn.findPlug("stagger", true), false) &&
            IsBoolAttrDefault(srcNodeFn.findPlug("mirrorU", true), false) &&
            IsBoolAttrDefault(srcNodeFn.findPlug("mirrorV", true), false) &&
            IsFloatAttrDefault(srcNodeFn.findPlug("rotateFrame", true), 0.f ) &&
            IsFloatAttrDefault(srcNodeFn.findPlug("rotateUV", true), 0.f ) &&
            IsVec2AttrDefault(srcNodeFn.findPlug("coverage", true), 1.f, 1.f ) &&
            IsVec2AttrDefault(srcNodeFn.findPlug("translateFrame", true), 0.f, 0.f ) &&
            IsVec2AttrDefault(srcNodeFn.findPlug("noiseUV", true), 0.f, 0.f ) &&
            IsBoolAttrDefault(srcNodeFn.findPlug("wrapU", true), true ) &&
            IsBoolAttrDefault(srcNodeFn.findPlug("wrapV", true), true ));

}
void CCheckerTranslator::NodeInitializer(CAbTranslator context)
{
   CExtensionAttrHelper helper(context.maya, "checkerboard");
}


// Bump3d
//
AtNode*  CBump3DTranslator::CreateArnoldNodes()
{
   return  AddArnoldNode("bump3d");
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
   MString outputAttr = GetMayaOutputAttributeName();
   AtNode* shader = NULL;

   // 1D Conditions 
   if (outputAttr == "facingRatio")
   {
      shader = AddArnoldNode("facing_ratio");
   }
   else if (outputAttr == "flippedNormal")
   {
      shader = AddArnoldNode("compare");
      AtNode* dot_shader = AddArnoldNode("dot", "dot");
      AtNode* Nf_shader = AddArnoldNode("state_vector", "nf");
      AiNodeSetStr(Nf_shader, "variable", "Nf");
      AtNode* N_shader = AddArnoldNode("state_vector", "n");
      AiNodeSetStr(N_shader, "variable", "N");
      AiNodeLink(Nf_shader, "input1", dot_shader);
      AiNodeLink(N_shader, "input2", dot_shader);
      AiNodeSetStr(shader, "test", "<");
      AiNodeSetFlt(shader, "input2", 0.0f);
      AiNodeLink(dot_shader, "input1", shader);
   }

   // // 2D Conditions
   else if (outputAttr == "uvCoord" )
   {
      shader = AddArnoldNode("float_to_rgb");
      AtNode* u_shader = AddArnoldNode("state_float", "u");
      AiNodeSetStr(u_shader, "variable", "u");
      AtNode* v_shader = AddArnoldNode("state_float", "v");
      AiNodeSetStr(v_shader, "variable", "v");
      AiNodeLink(u_shader, "r", shader);
      AiNodeLink(v_shader, "g", shader);
   }
   else if (outputAttr == "pixelCenter")
   {
      shader = AddArnoldNode("float_to_rgb");
      AtNode* screenx_shader = AddArnoldNode("state_int", "x");
      AiNodeSetStr(screenx_shader, "variable", "x");
      AtNode* screeny_shader = AddArnoldNode("state_int", "y");
      AiNodeSetStr(screeny_shader, "variable", "y");
      AiNodeLink(screenx_shader, "r", shader);
      AiNodeLink(screeny_shader, "g", shader);
   }

   // // 3D Conditions 
   else if ( outputAttr == "pointWorld")
   {
      shader = AddArnoldNode("state_vector");
      AiNodeSetStr(shader, "variable", "P");
   }
   else if ( outputAttr == "pointObj")
   {
      shader = AddArnoldNode("state_vector");
      AiNodeSetStr(shader, "variable", "Po");
   }
   else if ( outputAttr == "pointCamera")
   {
      shader = AddArnoldNode("space_transform");
      AiNodeSetStr(shader, "type", "point");
      AiNodeSetStr(shader, "from", "world");
      AiNodeSetStr(shader, "to", "camera");
      AtNode* P_shader = AddArnoldNode("state_vector", "P");
      AiNodeSetStr(P_shader, "variable", "P");
      AiNodeLink(P_shader, "input", shader);
   }
   else if ( outputAttr == "normalCamera")
   {
      shader = AddArnoldNode("space_transform");
      AiNodeSetStr(shader, "type", "normal");
      AiNodeSetStr(shader, "from", "world");
      AiNodeSetStr(shader, "to", "camera");
      AtNode* N_shader = AddArnoldNode("state_vector", "N");
      AiNodeSetStr(N_shader, "variable", "N");
      AiNodeLink(N_shader, "input", shader);
   }
   else if ( outputAttr == "rayDirection")
   {
      shader = AddArnoldNode("space_transform");
      AiNodeSetStr(shader, "type", "vector");
      AiNodeSetStr(shader, "from", "world");
      AiNodeSetStr(shader, "to", "camera");
      AtNode* Rd_shader = AddArnoldNode("state_vector", "Rd");
      AiNodeSetStr(Rd_shader, "variable", "Rd");
      AiNodeLink(Rd_shader, "input", shader);
   }
   else if ( outputAttr == "tangentUCamera")
   {
      shader = AddArnoldNode("space_transform");
      AiNodeSetStr(shader, "type", "vector");
      AiNodeSetStr(shader, "from", "world");
      AiNodeSetStr(shader, "to", "camera");
      AiNodeSetBool(shader, "normalize", true);
      AtNode* tgtU_shader = AddArnoldNode("state_vector", "u");
      AiNodeSetStr(tgtU_shader, "variable", "dPdu");
      AiNodeLink(tgtU_shader, "input", shader);
   }
   else if ( outputAttr == "tangentVCamera")
   {
      shader = AddArnoldNode("space_transform");
      AiNodeSetStr(shader, "type", "vector");
      AiNodeSetStr(shader, "from", "world");
      AiNodeSetStr(shader, "to", "camera");
      AiNodeSetBool(shader, "normalize", true);
      AtNode* tgtV_shader = AddArnoldNode("state_vector", "v");
      AiNodeSetStr(tgtV_shader, "variable", "dPdv");
      AiNodeLink(tgtV_shader, "input", shader);
   }
   else
   {
      AiMsgError("[mtoa] [translator %s] invalid output attribute requested: %s", GetTranslatorName().asChar(), outputAttr.asChar());
      return NULL;
   }
   return shader;
}

void CSamplerInfoTranslator::Export(AtNode* shader)
{
}

// PlusMinusAverage
//
AtNode* CPlusMinusAverageTranslator::CreateArnoldNodes()
{
   MString outputAttr = GetMayaOutputAttributeName();

   if (outputAttr == "output1D")
      m_inputSize = 1;
   else if (outputAttr == "output2D")
      m_inputSize = 2;
   else if (outputAttr == "output3D")
      m_inputSize = 3;
   else
   {
      m_inputSize = 0;
      AiMsgError("[mtoa] [translator %s] invalid output attribute requested: %s", GetTranslatorName().asChar(), outputAttr.asChar());
      return NULL;
   }

   return AddArnoldNode("layer_rgba");
}

void CPlusMinusAverageTranslator::Export(AtNode* shader)
{
   int operation = FindMayaPlug("operation").asInt();
   std::string opStr;
   switch(operation)
   {
      default:
      case OP_NONE:
         opStr = "overwrite";
         break;
      case OP_PLUS:
         opStr = "plus";
         break;
      case OP_MINUS:
         opStr = "minus";
         break;   
      case OP_AVERAGE: 
         opStr = "plus";
         break;
   }
   MFnDependencyNode fnNode(GetMayaObject());

   char aiAttr[64];
   MPlugArray connections;
   unsigned int size = 1;
   for (unsigned int i = 1; i <= 8; ++i)
   {
      sprintf(aiAttr, "enable%u", i);
      AiNodeSetBool(shader, aiAttr, false);
   }

   if (m_inputSize == 1)
   {
      MPlug plug = FindMayaPlug("input1D");
            
      size = plug.numElements();
      if (size > 8)
      {
         AiMsgWarning("[mtoa.translator] %s : a maximum of 8 inputs is supported for PlusMayaAverage inputs", fnNode.name().asChar());
         size = 8;
      }
      if (operation == OP_NONE)
         size = 1;

      for (unsigned int i = 0; i < size; ++i)
      {
         sprintf(aiAttr, "enable%u", i+1);
         AiNodeSetBool(shader, aiAttr, true);
         
         sprintf(aiAttr, "input%u", i+1);
         // can be linked
         float val = plug[i].asFloat();
         AiNodeSetRGBA(shader, aiAttr, val, val, val, 1.f);
         plug[i].connectedTo(connections, true, false);
         if (connections.length() > 0)
            ShaderComputeLink(this, connections[0], shader, aiAttr);
         
         if (i > 0)
         {
            sprintf(aiAttr, "operation%u", i+1);
            AiNodeSetStr(shader, aiAttr, opStr.c_str());
         }
         
      }
   } else if (m_inputSize == 2)
   {
      MPlug plug = FindMayaPlug("input2D");
      MObject ox = fnNode.attribute("input2Dx");
      MObject oy = fnNode.attribute("input2Dy");
      size = plug.numElements();
      if (size > 8)
      {
         AiMsgWarning("[mtoa.translator] %s : a maximum of 8 inputs is supported for PlusMayaAverage inputs", fnNode.name().asChar());
         size = 8;
      }
      if (operation == OP_NONE)
         size = 1;
      
      for (unsigned int i = 0; i < size; ++i)
      {
         sprintf(aiAttr, "enable%u", i+1);
         AiNodeSetBool(shader, aiAttr, true);
         
         sprintf(aiAttr, "input%u", i+1);
         AiNodeSetRGBA(shader, aiAttr, plug[i].child(ox).asFloat(), plug[i].child(oy).asFloat(), 0.f, 1.f);

         connections.clear();
         plug[i].connectedTo(connections, true, false);
         if (connections.length() > 0)
            ShaderComputeLink(this, connections[0], shader, aiAttr);
         connections.clear();
         plug[i].child(ox).connectedTo(connections, true, false);
         if (connections.length() > 0)
         {
            std::string compAttr = aiAttr;
            compAttr += std::string(".r");
            ShaderComputeLink(this, connections[0], shader, compAttr.c_str());
         }
         connections.clear();
         plug[i].child(oy).connectedTo(connections, true, false);
         if (connections.length() > 0)
         {
            std::string compAttr = aiAttr;
            compAttr += std::string(".g");
            ShaderComputeLink(this, connections[0], shader, compAttr.c_str());
         }

         if (i > 0)
         {
            sprintf(aiAttr, "operation%u", i+1);
            AiNodeSetStr(shader, aiAttr, opStr.c_str());
         }
      }
   } else if (m_inputSize == 3)
   {
      MPlug plug = FindMayaPlug("input3D");
      MObject ox = fnNode.attribute("input3Dx");
      MObject oy = fnNode.attribute("input3Dy");
      MObject oz = fnNode.attribute("input3Dz");

      size = plug.numElements();
      if (size > 8)
      {
         AiMsgWarning("[mtoa.translator] %s : a maximum of 8 inputs is supported for PlusMayaAverage inputs", fnNode.name().asChar());
         size = 8;
      }
      if (operation == OP_NONE)
         size = 1;
      
      for (unsigned int i = 0; i < size; ++i)
      {
         sprintf(aiAttr, "enable%u", i+1);
         AiNodeSetBool(shader, aiAttr, true);
         
         sprintf(aiAttr, "input%u", i+1);
         AiNodeSetRGBA(shader, aiAttr, plug[i].child(ox).asFloat(), plug[i].child(oy).asFloat(),  plug[i].child(oz).asFloat(), 1.f);
         connections.clear();
         plug[i].connectedTo(connections, true, false);
         if (connections.length() > 0)
            ShaderComputeLink(this, connections[0], shader, aiAttr);

         connections.clear();
         plug[i].child(ox).connectedTo(connections, true, false);
         if (connections.length() > 0)
         {
            std::string compAttr = aiAttr;
            compAttr += std::string(".r");
            ShaderComputeLink(this, connections[0], shader, compAttr.c_str());
         }
         connections.clear();
         plug[i].child(oy).connectedTo(connections, true, false);
         if (connections.length() > 0)
         {
            std::string compAttr = aiAttr;
            compAttr += std::string(".g");
            ShaderComputeLink(this, connections[0], shader, compAttr.c_str());
         }
         connections.clear();
         plug[i].child(oz).connectedTo(connections, true, false);
         if (connections.length() > 0)
         {
            std::string compAttr = aiAttr;
            compAttr += std::string(".b");
            ShaderComputeLink(this, connections[0], shader, compAttr.c_str());
         }
         if (i > 0)
         {
            sprintf(aiAttr, "operation%u", i+1);
            AiNodeSetStr(shader, aiAttr, opStr.c_str());
         }
      }      

   }
   if (operation == OP_AVERAGE)
   {
      AiNodeUnlink(shader, "input8");
      AiNodeSetStr(shader, "operation8", "divide");
      AiNodeSetRGBA(shader, "input8", float(size), float(size), float(size), 1.f);
      AiNodeSetBool(shader, "enable8", true);
   }
   
  
}

// ParticleSamplerInfo
//
AtNode* CParticleSamplerInfoTranslator::CreateArnoldNodes()
{
   MString outputAttr = GetMayaOutputAttributeName();

   if (
         outputAttr == "outColor" ||
         outputAttr == "outIncandescence" ||
         outputAttr == "outTransparency"||
         outputAttr == "rgbPP" ||
         outputAttr == "incandescensePP" ||
         outputAttr == "incandescense" ||
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
      return AddArnoldNode("user_data_rgb");
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
      return AddArnoldNode("user_data_float");
   }
   else
      return NULL;
}

void CParticleSamplerInfoTranslator::Export(AtNode* shader)
{
   MString outputAttr = GetMayaOutputAttributeName();

   if (outputAttr == "outColor" || outputAttr == "rgbPP")
   {
      AiNodeSetStr(shader, "attribute", "rgbPP");
   }
   else if (outputAttr == "outTransparency" )
   {
      AiNodeSetStr(shader, "attribute", "opacityPP");
   }
   else if ( outputAttr == "opacityPP" || outputAttr == "opacity")
   {
      AiNodeSetStr(shader, "attribute" , "opacityPP");
   }
   else if (outputAttr == "outIncandescence" || outputAttr == "incandescensePP" || outputAttr == "incandescense" )
   {
      AiNodeSetStr(shader, "attribute", "incandescencePP");
   }
   else if (outputAttr == "lifespanPP" || outputAttr == "lifespan")
   {
      AiNodeSetStr(shader, "attribute", "lifespanPP");
   }
   else if (outputAttr == "radiusPP" || outputAttr == "radius")
   {
      AiNodeSetStr(shader, "attribute", "radiusPP");
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
            outputAttr == "userVector5PP" ||
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
         AiNodeSetStr(shader, "attribute", outputAttr.asChar());
      }
}

// RemapValue
//
AtNode* CRemapValueTranslator::CreateArnoldNodes()
{
   MString outputAttr = GetMayaOutputAttributeName();

   AtNode* remapRamp = AddArnoldNode((outputAttr == "outValue") ? "ramp_float" : "ramp_rgb");
   //AtNode* outRemapRange = NULL;
   if (!(IsFloatAttrDefault(FindMayaPlug("outputMin"), 0.f) &&
         IsFloatAttrDefault(FindMayaPlug("outputMax"), 1.f)))
      return AddArnoldNode("range", "outRemapRange");
   
   return remapRamp;
}

void CRemapValueTranslator::Export(AtNode* shader)
{
   MString outputAttr = GetMayaOutputAttributeName();
   MFnDependencyNode fnNode(GetMayaObject());
   MPlug attr, elem, pos, val, interp;

   AtNode* inRemapRange = NULL;
   if (!(IsFloatAttrDefault(FindMayaPlug("inputMin"), 0.f) &&
         IsFloatAttrDefault(FindMayaPlug("inputMax"), 1.f)))
   {
      inRemapRange = GetArnoldNode("inRemapRange");
      if (inRemapRange == NULL)
         inRemapRange = AddArnoldNode("range", "inRemapRange");
   }

   AtNode* remapRamp = GetArnoldNode();
   AtNode* outRemapRange = GetArnoldNode("outRemapRange");
   if (inRemapRange)
   {
      AiNodeLink(inRemapRange, "input", remapRamp);
      ProcessParameter(inRemapRange, "input_min", AI_TYPE_FLOAT,"inputMin");
      ProcessParameter(inRemapRange, "input_max" ,AI_TYPE_FLOAT, "inputMax");         
      ProcessParameter(inRemapRange, "input", AI_TYPE_FLOAT, "inputValue");
   } else
      ProcessParameter(remapRamp, "input", AI_TYPE_FLOAT, "inputValue");
   
   if (outRemapRange)
   {
      AiNodeLink(remapRamp, "input", outRemapRange);
      ProcessParameter(outRemapRange, "output_min" , AI_TYPE_FLOAT, "outputMin");
      ProcessParameter(outRemapRange, "output_max", AI_TYPE_FLOAT, "outputMax");
   }


   if (outputAttr == "outValue")
   {
      attr = FindMayaPlug("value");

      MObject opos = fnNode.attribute("value_Position");
      ProcessArrayParameter(remapRamp, "position", attr, AI_TYPE_FLOAT, &opos);

      MObject oval = fnNode.attribute("value_FloatValue");
      ProcessArrayParameter(remapRamp, "value", attr, AI_TYPE_FLOAT, &oval);

      MObject ointerp = fnNode.attribute("value_Interp");
      ProcessArrayParameter(remapRamp, "interpolation", attr, AI_TYPE_INT, &ointerp);
      
   }
   else
   {  
      attr = FindMayaPlug("color");

      MObject opos = fnNode.attribute("color_Position");
      ProcessArrayParameter(remapRamp, "position", attr, AI_TYPE_FLOAT, &opos);

      MObject oval = fnNode.attribute("color_Color");
      ProcessArrayParameter(remapRamp, "color", attr, AI_TYPE_RGB, &oval);

      MObject ointerp = fnNode.attribute("color_Interp");
      ProcessArrayParameter(remapRamp, "interpolation", attr, AI_TYPE_INT, &ointerp);
   }
}

void CRemapValueTranslator::NodeChanged(MObject& node, MPlug& plug)
{
   MString plugName = plug.partialName(false, false, false, false, false, true);
   if ((plugName == "outputMin" || plugName == "outputMax") && (GetArnoldNode("outRemapRange") == NULL))
      SetUpdateMode(AI_RECREATE_NODE);

   CShaderTranslator::NodeChanged(node, plug);
}
// Remap Color
//
AtNode* CRemapColorTranslator::CreateArnoldNodes()
{

   AtNode* inRemapRange = AddArnoldNode("range", "inRemapRange");
   AtNode* R_Ramp = AddArnoldNode("ramp_float", "R_ramp");
   AtNode* G_Ramp = AddArnoldNode("ramp_float", "G_ramp");
   AtNode* B_Ramp = AddArnoldNode("ramp_float", "B_ramp");

   AtNode* outRemapRange = AddArnoldNode("range");

   AiNodeLinkOutput(inRemapRange, "r", R_Ramp, "input");
   AiNodeLinkOutput(inRemapRange, "g", G_Ramp, "input");
   AiNodeLinkOutput(inRemapRange, "b", B_Ramp, "input");

   AiNodeLink(R_Ramp, "input.r", outRemapRange);
   AiNodeLink(G_Ramp, "input.g", outRemapRange);
   AiNodeLink(B_Ramp, "input.b", outRemapRange);

   return outRemapRange;
}

void CRemapColorTranslator::Export(AtNode* shader)
{
   MPlug attr, elem, pos, val, interp;

   AtNode* inRemapRange = GetArnoldNode("inRemapRange");
   AtNode* R_Ramp = GetArnoldNode("R_ramp");
   AtNode* G_Ramp = GetArnoldNode("G_ramp");
   AtNode* B_Ramp = GetArnoldNode("B_ramp");
   AtNode* outRemapRange = shader;

   ProcessParameter(inRemapRange, "input", AI_TYPE_RGB, "color");
   ProcessParameter(inRemapRange, "input_min", AI_TYPE_FLOAT,"inputMin");
   ProcessParameter(inRemapRange, "input_max", AI_TYPE_FLOAT,"inputMax");

   ProcessParameter(outRemapRange, "output_min", AI_TYPE_FLOAT,"outputMin");
   ProcessParameter(outRemapRange, "output_max", AI_TYPE_FLOAT,"outputMax");

   MFnDependencyNode fnNode(GetMayaObject());

   attr = FindMayaPlug("red");
   MObject rpos = fnNode.attribute("red_Position");
   ProcessArrayParameter(R_Ramp, "position", attr, AI_TYPE_FLOAT, &rpos);

   MObject rval = fnNode.attribute("red_FloatValue");
   ProcessArrayParameter(R_Ramp, "value", attr, AI_TYPE_FLOAT, &rval);

   MObject rinterp = fnNode.attribute("red_Interp");
   ProcessArrayParameter(R_Ramp,"interpolation", attr, AI_TYPE_INT, &rinterp);

// ####

   attr = FindMayaPlug("green");
   MObject gpos = fnNode.attribute("green_Position");
   ProcessArrayParameter(G_Ramp, "position", attr, AI_TYPE_FLOAT, &gpos);

   MObject gval = fnNode.attribute("green_FloatValue");
   ProcessArrayParameter(G_Ramp, "value", attr, AI_TYPE_FLOAT, &gval);

   MObject ginterp = fnNode.attribute("green_Interp");
   ProcessArrayParameter(G_Ramp,"interpolation", attr, AI_TYPE_INT, &ginterp);

// ####

   attr = FindMayaPlug("blue");
   MObject bpos = fnNode.attribute("blue_Position");
   ProcessArrayParameter(B_Ramp, "position", attr, AI_TYPE_FLOAT, &bpos);

   MObject bval = fnNode.attribute("blue_FloatValue");
   ProcessArrayParameter(B_Ramp, "value", attr, AI_TYPE_FLOAT, &bval);

   MObject binterp = fnNode.attribute("blue_Interp");
   ProcessArrayParameter(B_Ramp,"interpolation", attr, AI_TYPE_INT, &binterp);

}

// Remap Hsv
//
AtNode* CRemapHsvTranslator::CreateArnoldNodes()
{
   AtNode* RGBtoHSV = AddArnoldNode("color_convert", "RGBtoHSV");
   AtNode* inRemapRange = AddArnoldNode("range", "inRemapRange");
   AtNode* H_Ramp = AddArnoldNode("ramp_float", "H_ramp");
   AtNode* S_Ramp = AddArnoldNode("ramp_float", "S_ramp");
   AtNode* V_Ramp = AddArnoldNode("ramp_float", "V_ramp");
   AtNode* outRemapRange = AddArnoldNode("range", "outRemapRange");
   AtNode* HSVtoRGB = AddArnoldNode("color_convert", "");

   AiNodeLink(RGBtoHSV, "input", inRemapRange);
   AiNodeLinkOutput(inRemapRange, "r", H_Ramp, "input");
   AiNodeLinkOutput(inRemapRange, "g", S_Ramp, "input");
   AiNodeLinkOutput(inRemapRange, "b", V_Ramp, "input");

   AiNodeLink(H_Ramp, "input.r", outRemapRange);
   AiNodeLink(S_Ramp, "input.g", outRemapRange);
   AiNodeLink(V_Ramp, "input.b", outRemapRange);

   AiNodeLink(outRemapRange, "input", HSVtoRGB);
   AiNodeSetStr(HSVtoRGB, "from", "HSV");
   AiNodeSetStr(HSVtoRGB, "to", "RGB");
   

   return HSVtoRGB;
}

void CRemapHsvTranslator::Export(AtNode* shader)
{
   MPlug attr, elem, pos, val, interp;

   AtNode* RGBtoHSV = GetArnoldNode("RGBtoHSV");
   AtNode* inRemapRange = GetArnoldNode("inRemapRange");
   AtNode* H_Ramp = GetArnoldNode("H_ramp");
   AtNode* S_Ramp = GetArnoldNode("S_ramp");
   AtNode* V_Ramp = GetArnoldNode("V_ramp");
   AtNode* outRemapRange = GetArnoldNode("outRemapRange");
   //AtNode* HSVtoRGB = shader;

   ProcessParameter(RGBtoHSV, "input", AI_TYPE_RGB, "color");
   
   ProcessParameter(inRemapRange, "input_min", AI_TYPE_FLOAT,"inputMin");
   ProcessParameter(inRemapRange, "input_max", AI_TYPE_FLOAT,"inputMax");

   ProcessParameter(outRemapRange, "output_min", AI_TYPE_FLOAT,"outputMin");
   ProcessParameter(outRemapRange, "output_max", AI_TYPE_FLOAT,"outputMax");

   MFnDependencyNode fnNode(GetMayaObject());

   attr = FindMayaPlug("hue");
   MObject rpos = fnNode.attribute("hue_Position");
   ProcessArrayParameter(H_Ramp, "position", attr, AI_TYPE_FLOAT, &rpos);

   MObject rval = fnNode.attribute("hue_FloatValue");
   ProcessArrayParameter(H_Ramp, "value", attr, AI_TYPE_FLOAT, &rval);

   MObject rinterp = fnNode.attribute("hue_Interp");
   ProcessArrayParameter(H_Ramp,"interpolation", attr, AI_TYPE_INT, &rinterp);

// ####

   attr = FindMayaPlug("saturation");
   MObject gpos = fnNode.attribute("saturation_Position");
   ProcessArrayParameter(S_Ramp, "position", attr, AI_TYPE_FLOAT, &gpos);

   MObject gval = fnNode.attribute("saturation_FloatValue");
   ProcessArrayParameter(S_Ramp, "value", attr, AI_TYPE_FLOAT, &gval);

   MObject ginterp = fnNode.attribute("saturation_Interp");
   ProcessArrayParameter(S_Ramp,"interpolation", attr, AI_TYPE_INT, &ginterp);

// ####

   attr = FindMayaPlug("value");
   MObject bpos = fnNode.attribute("value_Position");
   ProcessArrayParameter(V_Ramp, "position", attr, AI_TYPE_FLOAT, &bpos);

   MObject bval = fnNode.attribute("value_FloatValue");
   ProcessArrayParameter(V_Ramp, "value", attr, AI_TYPE_FLOAT, &bval);

   MObject binterp = fnNode.attribute("value_Interp");
   ProcessArrayParameter(V_Ramp,"interpolation", attr, AI_TYPE_INT, &binterp);


}

// Projection
//
AtNode*  CProjectionTranslator::CreateArnoldNodes()
{  
   int type = FindMayaPlug("projType").asInt();
   AtNode *projection = NULL;
   if (type == PT_PERSPECTIVE)
      projection = AddArnoldNode("camera_projection"); 
   else if (type == PT_TRIPLANAR)
      projection = AddArnoldNode("triplanar");
   else if (type == PT_NONE)
      projection = AddArnoldNode("flat");
   else
      projection = AddArnoldNode("uv_projection");

   if (RequiresColorCorrect())
      return AddArnoldNode("color_correct", "cc");

   return projection;
}

void CProjectionTranslator::Export(AtNode* shader)
{
   if (AiNodeIs(shader, AtString("uv_projection")))
   {
      int projType = FindMayaPlug("projType").asInt();
      switch(projType)
      {
         default:
         break; // shouldn't be here
         case PT_PLANAR:
            AiNodeSetStr(shader, "projection_type", "planar");
            break;
         case PT_SPHERICAL:
            AiNodeSetStr(shader, "projection_type", "spherical");
            break;
         case PT_CYLINDRICAL:
            AiNodeSetStr(shader, "projection_type", "spherical");
            break;
         case PT_BALL:
            AiNodeSetStr(shader, "projection_type", "ball");
            break;
         case PT_CUBIC:
            AiNodeSetStr(shader, "projection_type", "cubic");
            break;
         case PT_CONCENTRIC:
            // error message, not supported            
         break;
      }
      ProcessParameter(shader, "projection_color", AI_TYPE_RGBA, "image");

      bool wrap = FindMayaPlug("wrap").asBool();
      AiNodeSetBool(shader, "clamp", !wrap);
      ProcessParameter(shader, "default_color", AI_TYPE_RGBA, "defaultColor");

      // Warning: the angles seem to appear as degrees in the Maya interface, 
      // but they're actually returned as radians!
      AiNodeSetFlt(shader, "u_angle", FindMayaPlug("uAngle").asFloat() * AI_RTOD);
      AiNodeSetFlt(shader, "v_angle", FindMayaPlug("vAngle").asFloat() * AI_RTOD);
      
      ProcessParameter(shader, "matrix", AI_TYPE_MATRIX, "placementMatrix");
      if (FindMayaPlug("local").asBool())
         AiNodeSetStr(shader, "coord_space", "object");
      else if (FindMayaPlug("aiUseReferenceObject").asBool())
         AiNodeSetStr(shader, "coord_space", "Pref");
      else
         AiNodeSetStr(shader, "coord_space", "world");

   } else if (AiNodeIs(shader, AtString("triplanar")))
   {
      /*
      AtNode *uv_transform = GetArnoldNode("uv");
      if (uv_transform == NULL)
         uv_transform = AddArnoldNode("uv_transform", "uv");

      AiNodeLink(uv_transform, "input", shader);
      */
      ProcessParameter(shader, "input", AI_TYPE_RGB, "image");
      
      ProcessParameter(shader, "matrix", AI_TYPE_MATRIX, "placementMatrix");
      if (FindMayaPlug("local").asBool())
         AiNodeSetStr(shader, "coord_space", "object");
      else if (FindMayaPlug("aiUseReferenceObject").asBool())
         AiNodeSetStr(shader, "coord_space", "Pref");
      else
         AiNodeSetStr(shader, "coord_space", "world");

      AiNodeSetVec(shader, "scale", 0.5f, 0.5f, 0.5f);
      //AiNodeSetBool(uv_transform, "flip_v", true);

   } else if (AiNodeIs(shader, AtString("camera_projection")))
   {
      MPlug camPlug = FindMayaPlug("linkedCamera");
      MPlugArray connections;
      camPlug.connectedTo(connections, true, false);
      float camAR = 1.f;

      if (connections.length() > 0)
      {
         AtNode *cam = ExportConnectedNode(connections[0]);
         AiNodeSetPtr(shader, "camera", cam);
         MObject camObj = connections[0].node();
         MFnCamera mfnCam(camObj);
         camAR = static_cast<float>(mfnCam.aspectRatio());
      }
      else
         AiNodeResetParameter(shader, "camera");
      /*
      if (FindMayaPlug("local").asBool())
         AiNodeSetStr(shader, "coord_space", "object");
      else if (FindMayaPlug("aiUseReferenceObject").asBool())
         AiNodeSetStr(shader, "coord_space", "Pref");
      else
         AiNodeSetStr(shader, "coord_space", "world");
      */

      AiNodeUnlink(shader, "projection_color");
      ProcessParameter(shader, "projection_color", AI_TYPE_RGBA, "image");
      ProcessParameter(shader, "offscreen_color", AI_TYPE_RGBA, "defaultColor");

      MSelectionList resList;
      resList.add(MString(":defaultResolution"));
      
      if (resList.length() > 0)
      {
         MObject resObject;
         resList.getDependNode(0,resObject);
         MFnDependencyNode fnRes(resObject);
         int width = 1;
         int height = 1;
         float outAR = 1.f;
         
         MStatus status;
         MPlug plug = fnRes.findPlug("width", true, &status);
         if (status == MS::kSuccess)
            width = plug.asInt();
         plug = fnRes.findPlug("height", true, &status);
         if (status == MS::kSuccess)
            height = plug.asInt();
         plug = fnRes.findPlug("deviceAspectRatio", true, &status);
         if (status == MS::kSuccess)
         {
            AiNodeSetFlt(shader, "aspect_ratio", plug.asFloat());
            outAR = 1.0f / (((float)height / width) * plug.asFloat());
         }
         
         float uScale = 1.f;
         float vScale = 1.f;

         int fitType = FindMayaPlug("fitType").asInt();
         int fitFill = FindMayaPlug("fitFill").asInt();
         float resAR = (float(width) / float(height));

         const float invOutAR = 1.0f / outAR;
         const float invCamAR = 1.0f / camAR;
         const float invResAR = 1.f / resAR;

         if (fitFill == FILL_FILL)
         {
            if (fitType == FIT_CAMERA_FILM_GATE)
            {
               vScale = resAR * invCamAR * invOutAR;
            } 
         }
         else if (fitFill == FILL_HORIZONTAL)
         {
            vScale = resAR * invOutAR;
         }
         else // FILL_VERTICAL
         {
            if (fitType == FIT_CAMERA_FILM_GATE)
            {
               vScale = resAR * invCamAR * invOutAR;
               uScale = invCamAR;
            }
            else
            {
               uScale = invResAR * outAR;
            }
         }

         if (std::abs(uScale - 1.f) > AI_EPSILON || std::abs(vScale - 1.f) > AI_EPSILON)
         {
            AtNode *uv_transform = GetArnoldNode("uv");
            if (uv_transform == NULL)
               uv_transform = AddArnoldNode("uv_transform", "uv");

            ProcessParameter(uv_transform, "passthrough", AI_TYPE_RGBA, "image");
            AiNodeSetVec2(uv_transform, "scale_frame", uScale, vScale);
            AiNodeLink(uv_transform, "projection_color", shader);
         } 
         
      }

   } else if (AiNodeIs(shader, AtString("flat")))
   {
      ProcessParameter(shader, "color", AI_TYPE_RGBA, "image");
   }
  
   AtNode *cc = GetArnoldNode("cc");
   if (cc)
   {
      AiNodeLink(shader, "input", cc);
      ProcessParameter(cc, "multiply", AI_TYPE_RGB, "colorGain");
      ProcessParameter(cc, "add", AI_TYPE_RGB, "colorOffset");
      ProcessParameter(cc, "alphaGain", AI_TYPE_FLOAT, "alphaGain");
      ProcessParameter(cc, "alphaOffset", AI_TYPE_FLOAT, "alphaOffset");
      ProcessParameter(cc, "invert", AI_TYPE_BOOLEAN, "invert");
   }

}

void CProjectionTranslator::NodeInitializer(CAbTranslator context)
{
   CExtensionAttrHelper helper("projection");
   
   CAttrData data;
   
   data.defaultValue.BOOL() = true;
   data.name = "aiUseReferenceObject";
   data.shortName = "ai_use_reference_object";
   data.channelBox = false;
   data.keyable = false;       
   helper.MakeInputBoolean(data);
}
void CProjectionTranslator::NodeChanged(MObject& node, MPlug& plug)
{
   MString plugName = plug.partialName(false, false, false, false, false, true);
   if (plugName == "projType")
      SetUpdateMode(AI_RECREATE_NODE);

   if ((plugName == "alphaGain" || plugName == "alphaOffset" || plugName == "colorGain" || 
        plugName == "colorOffset" || plugName == "invert" ) && GetArnoldNode("cc") == NULL)
      SetUpdateMode(AI_RECREATE_NODE);
 
   CShaderTranslator::NodeChanged(node, plug);  
}

bool CProjectionTranslator::RequiresColorCorrect() const
{
   return ! (IsFloatAttrDefault(FindMayaPlug("alphaGain"), 1.f) &&
             IsFloatAttrDefault(FindMayaPlug("alphaOffset"), 0.f) &&
             IsRGBAttrDefault(FindMayaPlug("colorGain"), 1.f, 1.f, 1.f) &&
             IsRGBAttrDefault(FindMayaPlug("colorOffset"), 0.f, 0.f, 0.f) &&
             IsBoolAttrDefault(FindMayaPlug("invert"), false));
}


// Place2DTexture

AtNode*  CPlace2DTextureTranslator::CreateArnoldNodes()
{
   AddArnoldNode("state_float", "u");
   AddArnoldNode("state_float", "v");
   return AddArnoldNode("uv_transform");
}

void CPlace2DTextureTranslator::Export(AtNode* shader)
{
   AtNode *state_u = GetArnoldNode("u");
   AtNode *state_v = GetArnoldNode("v");
   
   AiNodeSetRGBA(shader, "passthrough", 0.f, 0.f, 0.f, 1.f);
   AiNodeLink(state_u, "passthrough.r", shader);
   AiNodeLink(state_v, "passthrough.g", shader);
   AiNodeSetStr(state_u, "variable", "u");
   AiNodeSetStr(state_v, "variable", "v");

   MFnDependencyNode fnNode(GetMayaObject());
   MPlugArray connections;
   AiNodeSetStr(shader, "uvset", "");
   fnNode.findPlug("uvCoord", true).connectedTo(connections, true, false);
   if (connections.length() > 0)
   {
      MFnDependencyNode uvcNodeFn(connections[0].node());
      if (uvcNodeFn.typeName() == "uvChooser")
         AiNodeSetStr(shader, "uvset", uvcNodeFn.findPlug("uvSets", true).elementByPhysicalIndex(0).asString().asChar());
   }
   ProcessParameter(shader, "coverage", AI_TYPE_VECTOR2, fnNode.findPlug("coverage", true));
   ProcessParameter(shader, "mirror_u", AI_TYPE_BOOLEAN, fnNode.findPlug("mirrorU", true));
   ProcessParameter(shader, "mirror_v", AI_TYPE_BOOLEAN, fnNode.findPlug("mirrorV", true));

   if (fnNode.findPlug("wrapU", true).asBool())
      AiNodeSetStr(shader, "wrap_frame_u", "none");
   else
   {
      // 2d procedural texture shaders (grid, cloth, marble, etc...)
      // expect a value of -1000000 when the Uvs are out-of-bounds #3822
      AiNodeSetStr(shader, "wrap_frame_u", "color");
      AiNodeSetRGBA(shader, "wrap_frame_color", -1000000.f, -1000000.f, -1000000.f, 1.f);
   }
      

   if (fnNode.findPlug("wrapV", true).asBool())
      AiNodeSetStr(shader, "wrap_frame_v", "none");
   else
   {
      // 2d procedural texture shaders (grid, cloth, marble, etc...)
      // expect a value of -1000000 when the Uvs are out-of-bounds #3822
      AiNodeSetStr(shader, "wrap_frame_v", "color");
      AiNodeSetRGBA(shader, "wrap_frame_color", -1000000.f, -1000000.f, -1000000.f, 1.f);
   }
   
   // if not linked, set alpha to zero
   ProcessParameter(shader, "repeat", AI_TYPE_VECTOR2, fnNode.findPlug("repeatUV", true));
   ProcessParameter(shader, "offset", AI_TYPE_VECTOR2, fnNode.findPlug("offset", true));


   float rotateFrame = fnNode.findPlug("rotateFrame", true).asFloat();
   AiNodeSetFlt(shader, "rotate_frame", rotateFrame * 180.f / AI_PI);
   //ProcessParameter(shader, "rotate_frame", AI_TYPE_FLOAT, fnNode.findPlug("rotateFrame"));
   ProcessParameter(shader, "translate_frame", AI_TYPE_VECTOR2, fnNode.findPlug("translateFrame", true));
   float rotateUV = fnNode.findPlug("rotateUV", true).asFloat();
   AiNodeSetFlt(shader, "rotate", rotateUV * 180.f / AI_PI);
   ProcessParameter(shader, "stagger", AI_TYPE_BOOLEAN, fnNode.findPlug("stagger", true));
   ProcessParameter(shader, "noise", AI_TYPE_VECTOR2, fnNode.findPlug("noiseUV", true));
}

// LayeredTexture
//
AtNode*  CLayeredTextureTranslator::CreateArnoldNodes()
{
   AtNode *layer_rgba = AddArnoldNode("layer_rgba");
   if (FindMayaPlug("alphaIsLuminance").asBool())
      return AddArnoldNode("color_correct", "cc");

   return layer_rgba;
}

void CLayeredTextureTranslator::Export(AtNode* shader)
{
   MPlug attr, elem, color, alpha, blendMode, isVisible;
   MPlugArray connections;
   MObject colorSrc, alphaSrc;
   bool colorConnectedToAlpha;
   char aiAttr[64];
   
   AtNode *cc = GetArnoldNode("cc");
   if (cc)
   {
      AiNodeLink(shader, "input", cc);
      AiNodeSetBool(cc, "alpha_is_luminance", true);
   }

   MFnDependencyNode fnNode(GetMayaObject());

   attr = fnNode.findPlug("inputs", true);
   int numElements = attr.numElements();
   
   MObject colorAttr = fnNode.attribute("color");
   MObject alphaAttr = fnNode.attribute("alpha");
   MObject blendModeAttr = fnNode.attribute("blendMode");
   MObject isVisibleAttr = fnNode.attribute("isVisible");

   // first initialize all layers to be disabled.
   for (unsigned int i = 0; i < 8; ++i)
   {
      sprintf(aiAttr, "enable%u", i);
      AiNodeSetBool(shader, aiAttr, false);
   }

   int arnoldIndex = 1;
   int shader_index = 0;
   if (numElements == 0)
      return;

   AtNode *rootShader = shader;
   if (numElements > 8)
   {
      shader_index++;
      shader = GetArnoldNode("l1");
      if (shader == NULL)
         shader = AddArnoldNode("layer_rgba", "l1");
   }
   for (int i = numElements - 1; i >= 0 ; --i, ++arnoldIndex)
   {
      elem = attr.elementByPhysicalIndex(i);
      color = elem.child(colorAttr);
      alpha = elem.child(alphaAttr);
      blendMode = elem.child(blendModeAttr);
      isVisible = elem.child(isVisibleAttr);

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


      if (arnoldIndex > 8 )
      {
         // I already finished the whole layer(shader_index)
         // I need to link it to the first input of the next layer

         // I still  have (i+1) layers to compute, and the first layer of my next shader
         // will be used to chain the previous layer_rgba. Let's see if that amount if < 8 so that
         // I can use the original layer_rgba shader
         AtNode *prevShader = shader;
         if (i < 7)
         {
            shader = rootShader;
         } else
         {
            shader_index++;
            MString key = "l";
            key += shader_index;
            shader = GetArnoldNode(key.asChar());
            if (shader == NULL)
               shader = AddArnoldNode("layer_rgba", key.asChar());
         }
         // link the whole previous layer_rgba to input1 so that we can continue with the following ones
         AiNodeLink(prevShader, "input1", shader); 
         arnoldIndex = 2;
      }

      sprintf(aiAttr, "input%u", arnoldIndex);
      ProcessParameter(shader, aiAttr, AI_TYPE_RGBA, color);


      sprintf(aiAttr, "mix%u", arnoldIndex);
      if (!colorConnectedToAlpha && alphaSrc.isNull())
         AiNodeSetFlt(shader, aiAttr, alpha.asFloat());
      else
         ProcessParameter(shader, aiAttr, AI_TYPE_FLOAT, alpha);

      int mayaBlend = blendMode.asInt();
      MString arnoldBlend;
      switch (mayaBlend){
         // set the arnold blend mode based on the maya one 
         default:
         case BM_NONE:
            arnoldBlend = "overwrite";
            AiNodeUnlink(shader, aiAttr);
            AiNodeSetFlt(shader, aiAttr, 1.f); // none = over with mix = 1 ?
            break;
         case BM_OVER:
            arnoldBlend = "overwrite";
            break;
         case BM_IN:
            // FIXME this isn't rendering properly
            arnoldBlend = "in";
            break;
         case BM_OUT:
            arnoldBlend = "out";
            break;
         case BM_ADD:
            arnoldBlend = "plus";
            break;
         case BM_SUBTRACT:
            arnoldBlend = "minus";
            break;
         case BM_MULTIPLY:
            arnoldBlend = "multiply";
            break;
         case BM_DIFFERENCE:
            arnoldBlend = "difference";
            break;
         case BM_LIGHTEN:
            arnoldBlend = "max";
            break;
         case BM_DARKEN:
            arnoldBlend = "min";
            break;
         case BM_SATURATE:
            arnoldBlend = "hard_light";
            break;
         case BM_DESATURATE:
            arnoldBlend = "exclusion";
            break;
         case BM_ILLUMINATE:
            arnoldBlend = "geometric";
            break;
      }
      sprintf(aiAttr, "operation%u", arnoldIndex);
      AiNodeSetStr(shader, aiAttr, arnoldBlend.asChar());
      
      sprintf(aiAttr, "enable%u", arnoldIndex);
      ProcessParameter(shader, aiAttr, AI_TYPE_BOOLEAN, isVisible);

   }

}
void CLayeredTextureTranslator::NodeChanged(MObject& node, MPlug& plug)
{
   MString plugName = plug.partialName(false, false, false, false, false, true);
   if (plugName == "alphaIsLuminance")
      SetUpdateMode(AI_RECREATE_NODE);
 
   CShaderTranslator::NodeChanged(node, plug);  
}

// LayeredShader
//
AtNode*  CLayeredShaderTranslator::CreateArnoldNodes()
{
   return AddArnoldNode("maya_layered_shader");
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
   attr = fnNode.findPlug("inputs", true);
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
      ProcessParameter(shader, aiAttr, AI_TYPE_CLOSURE, color);

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
   if (!IsMotionBlurEnabled(MTOA_MBLUR_SHADER))
      return NULL;

   return AddArnoldNode("ramp_float");
}

void CAnimCurveTranslator::Export(AtNode* shader)
{
   if (shader == NULL)
      return;

   MFnAnimCurve fnCurve(GetMayaObject());
   MStatus status;
   float value = (float) fnCurve.evaluate(MAnimControl::currentTime(), &status);
   AiNodeSetStr(shader, "type", "time"); // we want time-driven ramps

   if (RequiresMotionData())
   {
      int numMotionSteps = GetNumMotionSteps();
      //int motionStep = GetMotionStep();

      // Note that here we're exporting the different keys as elements in the ramp
      AtArray* values = AiArrayAllocate(numMotionSteps, 1, AI_TYPE_FLOAT);
      AiArraySetFlt(values, GetMotionStep(), value);
      AiNodeSetArray(shader, "value", values);

      // For positions, we can set the values right away...
      AtArray* positions = AiArrayAllocate(numMotionSteps, 1, AI_TYPE_FLOAT);
      AtArray* interpolations = AiArrayAllocate(numMotionSteps, 1, AI_TYPE_INT);

      for (int i = 0; i < numMotionSteps; ++i)
      {
         AiArraySetFlt(positions, i, float(i) / float(numMotionSteps - 1.f));
         // 1=Linear interpolation. Can we set a single array element, or does ramp_float expect the 
         // same amount of elements for the 3 arrays ?
         AiArraySetInt(interpolations, i, 1);
      }
      
      AiNodeSetArray(shader, "position", positions);
      AiNodeSetArray(shader, "interpolation", interpolations);
   }
   else
   {      
      AiNodeSetFlt(shader, "value", value);
      AiNodeSetFlt(shader, "position", 0.f);
      AiNodeSetInt(shader, "interpolation", 1); // linear
   }
   
}

void CAnimCurveTranslator::ExportMotion(AtNode* shader)
{
   MFnAnimCurve fnCurve(GetMayaObject());
   MStatus status;
   float value = (float) fnCurve.evaluate(MAnimControl::currentTime(), &status);
   AtArray* values = AiNodeGetArray(shader, "value");
   AiArraySetFlt(values, GetMotionStep(), value);
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
      m_isVectorDisp = true;
      AtNode *vectorMap = AddArnoldNode("vector_map");
      
      // if space is world (0), we need to insert a space_transform node
      AtNode *spaceTransform = (FindMayaPlug("vectorSpace").asInt() == 0) ? AddArnoldNode("space_transform", "space_transform") : NULL;

      if (!IsFloatAttrDefault(FindMayaPlug("displacement"), 0.f))
      {
         // on top of the vector displacement, there will be a regular displacement along the normal,
         // thus the root shader should be add
         AtNode *add = AddArnoldNode("add", "add");
         AddArnoldNode("state_vector", "state_vector");
         AddArnoldNode("multiply", "multiply1");
         AddArnoldNode("multiply", "multiply2");
         return add;
      }
      if (spaceTransform)
         return spaceTransform;
      
      return vectorMap;
   }
   else
   {      
      m_isVectorDisp = false;
      return AddArnoldNode("range");
   }
}

void CDisplacementTranslator::Export(AtNode* shader)
{
   if (!m_isVectorDisp)
   {
      // simple displacement along the normal.
      // We just need the displacement input + scale + zeroValue
      ProcessParameter(shader, "input", AI_TYPE_FLOAT, "displacement");
      ProcessParameter(shader, "contrast", AI_TYPE_FLOAT, "scale");
      AiNodeSetFlt(shader, "contrast_pivot", 0.f);
      // Now to match the formulas I need output_min = - aiDisplacementZeroValue
      // and (output_max - output_min) = 1
      MPlug zeroValPlug = FindMayaPlug("aiDisplacementZeroValue");
      float zeroVal = zeroValPlug.asFloat();
      AiNodeSetFlt(shader, "output_min", - zeroVal);
      AiNodeSetFlt(shader, "output_max", 1 - zeroVal);
      return;
   }

   // vector displacement, let the fun begin !
   AtNode *vectorMap = GetArnoldNode();
   AtNode *spaceTransform = GetArnoldNode("space_transform");
   AtNode *add = GetArnoldNode("add");
  
   ProcessParameter(vectorMap, "input", AI_TYPE_VECTOR, "vectorDisplacement");
   ProcessParameter(vectorMap, "scale", AI_TYPE_FLOAT);
   ProcessParameter(vectorMap, "tangent", AI_TYPE_VECTOR);
   AiNodeSetBool(vectorMap, "color_to_signed", (FindMayaPlug("vectorEncoding").asInt() != 0));
   // vectorSpace = {WORLD, OBJECT, TANGENT}
   int vectorSpace = FindMayaPlug("vectorSpace").asInt();
   AiNodeSetBool(vectorMap, "tangent_space", (vectorSpace == 2)); 
   if (vectorSpace == 2)
      AiNodeSetStr(vectorMap, "order", "XZY");

   if (spaceTransform)
   {
      AiNodeSetStr(spaceTransform, "to", "object");  // object
      AiNodeSetStr(spaceTransform, "from", (vectorSpace == 0) ? "world" : "object");
      AiNodeSetStr(spaceTransform, "type", "vector");
      AiNodeLink(vectorMap, "input", spaceTransform);
   }

   if (add)
   {
      AtNode *stateVector = GetArnoldNode("state_vector");
      AtNode *multiply1 = GetArnoldNode("multiply1");
      AtNode *multiply2 = GetArnoldNode("multiply2");

      AiNodeLink((spaceTransform) ? spaceTransform : vectorMap, "input1", add);
      AiNodeLink(stateVector, "input1", multiply1);
      AiNodeSetStr(stateVector, "variable", "N");
      AiNodeLink(multiply1, "input2", add);
      
      float scale = FindMayaPlug("scale").asFloat();
      MPlugArray connections;
      MPlug displacementPlug = FindMayaPlug("displacement");
      displacementPlug.connectedTo(connections, true, false);
      if (connections.length() > 0)
      {
         // there's a link on "displacement" so we can't just multiply it with "scale"
         // so we link multiply2 to the input2 of multiply1, we link multiply2.input1 to the 
         // displacement link, and we set scale to its input2
         AiNodeLink(multiply2, "input2", multiply1);

         ShaderComputeLink(this, connections[0], multiply2, "input1");

         AiNodeSetRGB(multiply2, "input2", scale, scale, scale);
      }
      else
      {
         // no link on "displacement", se we don't need multiply2. We just multiply it with "scale"
         AiNodeUnlink(multiply1, "input2"); // it case it was linked before
         float displacement = displacementPlug.asFloat();
         displacement *= scale;
         AiNodeSetRGB(multiply1, "input2", displacement, displacement, displacement);
      }
   }
}
void CDisplacementTranslator::NodeChanged(MObject& node, MPlug& plug)
{
   MString plugName = plug.partialName(false, false, false, false, false, true);
   if (!m_isVectorDisp)
   {
      if (plugName == "vectorDisplacement")
         SetUpdateMode(AI_RECREATE_NODE);
   } else
   {
      if (plugName == "vectorSpace" || plugName == "displacement")
         SetUpdateMode(AI_RECREATE_NODE);
   }

   CShaderTranslator::NodeChanged(node, plug);

   MPlug disp = MFnDependencyNode(node).findPlug("displacement", true);
   MPlugArray connectedPlugs;
   disp.connectedTo(connectedPlugs,false,true);

   // For each shading engine connected to the displacement node
   for(unsigned int j = 0; j < connectedPlugs.length(); j++)
   {
      MPlug connection = connectedPlugs[j];
      MObject shadingEngine = connection.node();

      std::vector< CNodeTranslator * > translatorsToUpdate;
      
      MFnDependencyNode shadingEngineDNode(shadingEngine);
      MPlug dagSetMembersPlug = shadingEngineDNode.findPlug("dagSetMembers", true);
      const unsigned int numElements = dagSetMembersPlug.numElements();

      // For each geometry connected to the shading engine
      for(unsigned int i = 0; i < numElements; i++)
      {
         MPlug a = dagSetMembersPlug[i];
         MPlugArray connectedPlugs;
         a.connectedTo(connectedPlugs,true,false);

         if (connectedPlugs.length() > 0)
         {
            MPlug connection = connectedPlugs[0];
            MObject parent = connection.node();
            MFnDependencyNode parentDag(parent);
            
            MDagPath dagPath;
            MStatus status = MDagPath::getAPathTo(parent, dagPath);
            if (!status)
               continue;

            // we don't want to export additional geometries, just get the already-exported 
            // geometries associated to this dagPath
            CNodeTranslator *translator2 = GetTranslator(dagPath); 
            if (translator2 == NULL)
               continue;

            translator2->SetUpdateMode(AI_RECREATE_NODE);
            translator2->RequestUpdate();
         }
      }
   }
}
void DisplacementTranslatorNodeInitializer(CAbTranslator context)
{
   CExtensionAttrHelper helper("displacementShader");
   
   CAttrData data;
   
   data.defaultValue.FLT() = 0.f;
   data.name = "aiDisplacementPadding";
   data.shortName = "ai_displacement_padding";
   data.channelBox = false;
   data.keyable = false;       
   helper.MakeInputFloat(data);
   
   data.defaultValue.FLT() = 0.f;
   data.name = "aiDisplacementZeroValue";
   data.shortName = "ai_displacement_zero_value";
   helper.MakeInputFloat(data);
   
   data.defaultValue.BOOL() = true;
   data.name = "aiDisplacementAutoBump";
   data.shortName = "ai_displacement_auto_bump";
   helper.MakeInputBoolean(data);
}

void CMayaBlinnTranslator::Export(AtNode* shader)
{
   ProcessParameter(shader, "base", AI_TYPE_FLOAT, "diffuse");
   ProcessParameter(shader, "base_color", AI_TYPE_RGB, "color");
   
   ProcessParameter(shader, "specular", AI_TYPE_FLOAT, "specularRollOff");
   ProcessParameter(shader, "specular_roughness", AI_TYPE_FLOAT, "eccentricity");
   ProcessParameter(shader, "specular_color", AI_TYPE_RGB, "specularColor");
   
   ProcessParameter(shader, "coat", AI_TYPE_FLOAT, "reflectivity");
   ProcessParameter(shader, "coat_color", AI_TYPE_RGB, "reflectedColor");
   AiNodeSetFlt(shader, "coat_roughness", 0.f);

   AiNodeSetFlt(shader, "specular_IOR", 3.f);
   AiNodeSetFlt(shader, "coat_IOR", 3.f);
   
   MPlug transparencyPlug = FindMayaPlug("transparency");
   float colorR = transparencyPlug.child(0).asFloat();
   float colorG = transparencyPlug.child(1).asFloat();
   float colorB = transparencyPlug.child(2).asFloat();
   AiNodeSetRGB(shader, "opacity", 1.0f - colorR, 1.0f - colorG, 1.0f - colorB);
   
   AiNodeSetFlt(shader, "emission", 1.f);
   ProcessParameter(shader, "emission_color", AI_TYPE_RGB, "incandescence");
}

AtNode* CMayaBlinnTranslator::CreateArnoldNodes()
{
   return AddArnoldNode("standard_surface");
}

void CMayaPhongTranslator::Export(AtNode* shader)
{
   ProcessParameter(shader, "base", AI_TYPE_FLOAT, "diffuse");
   ProcessParameter(shader, "base_color", AI_TYPE_RGB, "color");
   
   AiNodeSetFlt(shader, "specular", 1.f);
   AiNodeSetFlt(shader, "specular_IOR", 3.f);
   AiNodeSetFlt(shader, "coat_IOR", 3.f);
   MPlug cosinePowerPlug = FindMayaPlug("cosinePower");
   float rougness = sqrtf(1.0f / (0.454f * cosinePowerPlug.asFloat() + 3.357f));
   AiNodeSetFlt(shader, "specular_roughness", rougness);
   ProcessParameter(shader, "specular_color", AI_TYPE_RGB, "specularColor");
   
   ProcessParameter(shader, "coat", AI_TYPE_FLOAT, "reflectivity");
   ProcessParameter(shader, "coat_color", AI_TYPE_RGB, "reflectedColor");
   AiNodeSetFlt(shader, "coat_roughness", 0.f);

   MPlug transparencyPlug = FindMayaPlug("transparency");
   float colorR = transparencyPlug.child(0).asFloat();
   float colorG = transparencyPlug.child(1).asFloat();
   float colorB = transparencyPlug.child(2).asFloat();
   AiNodeSetRGB(shader, "opacity", 1.0f - colorR, 1.0f - colorG, 1.0f - colorB);
   
   AiNodeSetFlt(shader, "emission", 1.f);
   ProcessParameter(shader, "emission_color", AI_TYPE_RGB, "incandescence");
}

AtNode* CMayaPhongTranslator::CreateArnoldNodes()
{
   return AddArnoldNode("standard_surface");
}

void CMayaPhongETranslator::Export(AtNode* shader)
{
   ProcessParameter(shader, "base", AI_TYPE_FLOAT, "diffuse");
   ProcessParameter(shader, "base_color", AI_TYPE_RGB, "color");

   AiNodeSetFlt(shader, "specular", 1.f);
   ProcessParameter(shader, "specular_roughness", AI_TYPE_FLOAT, "roughness");
   ProcessParameter(shader, "specular_color", AI_TYPE_RGB, "specularColor");

   ProcessParameter(shader, "coat", AI_TYPE_FLOAT, "reflectivity");
   ProcessParameter(shader, "coat_color", AI_TYPE_RGB, "reflectedColor");
   AiNodeSetFlt(shader, "coat_roughness", 0.f);
   
   AiNodeSetFlt(shader, "specular_IOR", 3.f);
   AiNodeSetFlt(shader, "coat_IOR", 3.f);
   
   MPlug transparencyPlug = FindMayaPlug("transparency");
   float colorR = transparencyPlug.child(0).asFloat();
   float colorG = transparencyPlug.child(1).asFloat();
   float colorB = transparencyPlug.child(2).asFloat();
   AiNodeSetRGB(shader, "opacity", 1.0f - colorR, 1.0f - colorG, 1.0f - colorB);

   AiNodeSetFlt(shader, "emission", 1.f);
   ProcessParameter(shader, "emission_color", AI_TYPE_RGB, "incandescence");
}

AtNode* CMayaPhongETranslator::CreateArnoldNodes()
{
    return AddArnoldNode("standard_surface");
}

void CMayaAnisotropicTranslator::Export(AtNode* shader)
{
   ProcessParameter(shader, "base", AI_TYPE_FLOAT, "diffuse");
   ProcessParameter(shader, "base_color", AI_TYPE_RGB, "color");

   AiNodeSetFlt(shader, "specular", 1.f);
   ProcessParameter(shader, "specular_color", AI_TYPE_RGB, "specularColor");

   ProcessParameter(shader, "coat", AI_TYPE_FLOAT, "reflectivity");
   ProcessParameter(shader, "coat_color", AI_TYPE_RGB, "reflectedColor");
   AiNodeSetFlt(shader, "coat_roughness", 0.f);

   MPlug transparencyPlug = FindMayaPlug("transparency");
   float colorR = transparencyPlug.child(0).asFloat();
   float colorG = transparencyPlug.child(1).asFloat();
   float colorB = transparencyPlug.child(2).asFloat();
   AiNodeSetRGB(shader, "opacity", 1.0f - colorR, 1.0f - colorG, 1.0f - colorB);

   AiNodeSetFlt(shader, "emission", 1.f);
   ProcessParameter(shader, "emission_color", AI_TYPE_RGB, "incandescence");

   // anisotropic parameters
   MPlug anglePlug = FindMayaPlug("angle");
   float angle = (anglePlug.asFloat()/360.0f);
   AiNodeSetFlt(shader, "specular_rotation", 1.0f - angle);

   MPlug spreadXPlug = FindMayaPlug("spreadX");
   MPlug spreadYPlug = FindMayaPlug("spreadY");
   MPlug roughnessPlug = FindMayaPlug("roughness");

   float spreadX = spreadXPlug.asFloat();
   float spreadY = spreadYPlug.asFloat();
   float roughness = roughnessPlug.asFloat();

   AiNodeSetFlt(shader, "specular_IOR", 3.f);
   AiNodeSetFlt(shader, "coat_IOR", 3.f);
   
   float spread = spreadY - spreadX;
   //float maximum = spreadY > spreadX ? spreadY : spreadX;

   if (spreadY > spreadX)
   {
      roughness = roughness - (spreadY * 0.01f) * roughness;
      if (spreadX < 3.0f)
         spread += (3.0f - spreadX) * spread;
   
      if (spread > 99.9999f)
         spread = 99.9999f;
   }
   else
   {
      roughness = roughness - (spreadX * 0.01f) * roughness;
      if (spreadY < 3.0f)
         spread += (3.0f - spreadY) * spread;
   
      if (spread < -99.9999f)
         spread = -99.9999f;
   }
   float anisotropy = 0.5f + spread * 0.005f;

   if (roughness > 1.0f)
      roughness = 1.0f;

   AiNodeSetFlt(shader, "specular_roughness", roughness);

   // converting now anisotropy from old standard shader to new standard_surface
   anisotropy = 2.f * std::abs(anisotropy -0.5f);
   AiNodeSetFlt(shader, "specular_anisotropy", anisotropy);
}

AtNode* CMayaAnisotropicTranslator::CreateArnoldNodes()
{
   return AddArnoldNode("standard_surface");
}

void CMayaBlendColorsTranslator::Export(AtNode* shader)
{
   // Note that inputs are inverted in Maya shader as compared to arnold
   ProcessParameter(shader, "input2", AI_TYPE_RGBA, "color1");
   ProcessParameter(shader, "input1", AI_TYPE_RGBA, "color2");
   ProcessParameter(shader, "mix", AI_TYPE_FLOAT, "blender");
   
}

AtNode* CMayaBlendColorsTranslator::CreateArnoldNodes()
{
   return AddArnoldNode("mix_rgba");
}

void CMayaClampTranslator::Export(AtNode* shader)
{
   ProcessParameter(shader, "input", AI_TYPE_RGB);
   AiNodeSetStr(shader, AtString("mode"), AtString("color"));
   ProcessParameter(shader, "min_color", AI_TYPE_RGB, "min");
   ProcessParameter(shader, "max_color", AI_TYPE_RGB, "max");
}

AtNode* CMayaClampTranslator::CreateArnoldNodes()
{
   return AddArnoldNode("clamp");
}

void CMayaRampShaderTranslator::Export(AtNode* shader)
{
   ProcessParameter(shader, "base", AI_TYPE_FLOAT, "diffuse");
   ProcessParameter(shader, "Ks", AI_TYPE_FLOAT, "specularity");
   ProcessParameter(shader, "specular_roughness", AI_TYPE_FLOAT, "eccentricity");

   MColor color;
   float reflectivity = 0.0f;

   MPlug plug = FindMayaPlug("color");
   MRampAttribute ramp(plug);
   ramp.getColorAtPosition(0, color);
   AiNodeSetRGB(shader, "base_color", color[0], color[1], color[2]);

   plug = FindMayaPlug("specularColor");
   ramp = MRampAttribute(plug);
   ramp.getColorAtPosition(0, color);
   AiNodeSetRGB(shader, "specular_color", color[0], color[1], color[2]);

   plug = FindMayaPlug("transparency");
   ramp = MRampAttribute(plug);
   ramp.getColorAtPosition(0, color);
   AiNodeSetRGB(shader, "opacity", 1.0f - color[0], 1.0f - color[1], 1.0f - color[2]);

   AiNodeSetFlt(shader, "emission", 1.f);

   plug = FindMayaPlug("incandescence");
   ramp = MRampAttribute(plug);
   ramp.getColorAtPosition(0, color);
   AiNodeSetRGB(shader, "emission_color", color[0], color[1], color[2]);

   plug = FindMayaPlug("reflectivity");
   ramp = MRampAttribute(plug);
   ramp.getValueAtPosition(0, reflectivity);
   AiNodeSetFlt(shader, "coat", reflectivity);

   plug = FindMayaPlug("environment");
   ramp = MRampAttribute(plug);
   ramp.getColorAtPosition(0, color);
   AiNodeSetRGB(shader, "coat_color", color[0], color[1], color[2]);
   AiNodeSetFlt(shader, "coat_roughness", 0.f);
}

AtNode* CMayaRampShaderTranslator::CreateArnoldNodes()
{
   return AddArnoldNode("standard_surface");
}


void CAiHairTranslator::NodeInitializer(CAbTranslator context)
{
   CExtensionAttrHelper helper("aiHair");
   
   CAttrData data;

   data.name = "aiEnableMatte";
   data.shortName = "ai_enable_matte";
   data.defaultValue.BOOL() = false;
   helper.MakeInputBoolean(data);

   data.name = "aiMatteColor";
   data.shortName = "ai_matte_color";
   data.defaultValue.RGB() = AI_RGB_BLACK;
   helper.MakeInputRGB(data);
   
   data.name = "aiMatteColorA";
   data.shortName = "ai_matte_color_a";
   data.hasMin = true;
   data.min.FLT() = 0.f;
   data.hasMax = true;
   data.max.FLT() = 1.0;
   data.defaultValue.FLT() = 0.0f;
   helper.MakeInputFloat(data);   
}

AtNode* CAiHairTranslator::CreateArnoldNodes()
{
   return ProcessAOVOutput(AddArnoldNode("hair"));
}

void CAiStandardHairTranslator::NodeInitializer(CAbTranslator context)
{
   CExtensionAttrHelper helper("aiStandardHair");
   
   CAttrData data;

   data.name = "aiEnableMatte";
   data.shortName = "ai_enable_matte";
   data.defaultValue.BOOL() = false;
   helper.MakeInputBoolean(data);

   data.name = "aiMatteColor";
   data.shortName = "ai_matte_color";
   data.defaultValue.RGB() = AI_RGB_BLACK;
   helper.MakeInputRGB(data);
   
   data.name = "aiMatteColorA";
   data.shortName = "ai_matte_color_a";
   data.hasMin = true;
   data.min.FLT() = 0.f;
   data.hasMax = true;
   data.max.FLT() = 1.0;
   data.defaultValue.FLT() = 0.0f;
   helper.MakeInputFloat(data);   
}

AtNode* CAiStandardHairTranslator::CreateArnoldNodes()
{
   return ProcessAOVOutput(AddArnoldNode("standard_hair"));
}

AtNode* CAiImageTranslator::CreateArnoldNodes()
{
   return AddArnoldNode("image");
}

void CAiImageTranslator::Export(AtNode* image)
{
   const CSessionOptions &options = GetSessionOptions();

   // keep the previous filename
   MString prevFilename = AiNodeGetStr(image, "filename").c_str();
   prevFilename = resolveFilePathForSequences(prevFilename, FindMayaPlug("frame").asInt());

   CShaderTranslator::Export(image);
   if (AiNodeGetLink(image, "filename") == 0)
   {
        MString filename(AiNodeGetStr(image, "filename"));
        filename = resolveFilePathForSequences(filename, FindMayaPlug("frame").asInt());
        options.FormatTexturePath(filename);

      MString colorSpace = FindMayaPlug("colorSpace").asString();

      // if the color space has changed, we need to regenerate the TX
      bool requestUpdateTx = (colorSpace != m_colorSpace);
      
      // storing color space for next export
      m_colorSpace = colorSpace;

      if (!requestUpdateTx)
      {
         // Color Space is the same, so let's check if the filename was modified
         int prevFilenameLength = prevFilename.length();

         if (prevFilenameLength > 0)
         {
            // seb 
            // compare against previous filename to see if we need to re-generate the TX
            if (prevFilenameLength > 3 && prevFilename.substring(prevFilenameLength - 3, prevFilenameLength - 1) == MString(".tx"))
            {
               // Previous Filename was .tx, either because of "use existing tx", 
               // or because it's explicitely targeting the .tx
               MString prevBasename = prevFilename.substring(0, prevFilenameLength - 4);

               int dotPos = filename.rindexW(".");
               if (dotPos > 0)
               {
                  MString basename = filename.substring(0, dotPos - 1);
                  // Let's compare the basenames (without extension)
                  if (prevBasename != basename)
                  {
                     // the basename was modified, this needs an update of TX
                     requestUpdateTx = true;
                  } else
                  {
                     //basename hasn't changed. However, I'm probably setting it back to non-tx here
                     // so let's keep the previous one (where Use Tx was applied)
                     filename = prevFilename;
                  }
               }
            } else
            {
               // if previous filename and new one are exactly identical, it's useless to update Tx
               requestUpdateTx = (prevFilename != filename);
            }
         } else if (filename.length() > 0)
         {
            requestUpdateTx = true;
         }
      }

      AiNodeSetStr(image, "filename", filename.asChar());

      // only set the color_space if the texture isn't a TX
      AiNodeSetStr(image, "color_space", "");
      if (filename.length() > 4)
      {
         MString extension = filename.substring(filename.length() - 3, filename.length() - 1);
         // set the color space only if texture isn't a TX
         if (extension != ".tx" && extension !=  ".TX")
            AiNodeSetStr(image, "color_space", colorSpace.asChar());         
      }

      // let Arnold Session know that image files have changed and it's necessary to update them
      if (requestUpdateTx) RequestTxUpdate();
   }
}

void CAiImageTranslator::NodeInitializer(CAbTranslator context)
{
   CExtensionAttrHelper helper(context.maya, "image");
   CAttrData data;
   data.defaultValue.BOOL() = true;
   data.name = "autoTx";
   data.shortName = "autotx";
   helper.MakeInputBoolean(data);

   data.defaultValue.BOOL() = false;
   data.name = "colorManagementConfigFileEnabled";
   data.shortName = "cmcf";
   helper.MakeInputBoolean(data);

   data.defaultValue.STR() = AtString("");
   data.name = "colorManagementConfigFilePath";
   data.shortName = "cmcp";
   helper.MakeInputString(data);

   data.defaultValue.BOOL() = false;
   data.name = "colorManagementEnabled";
   data.shortName = "cme";
   helper.MakeInputBoolean(data);

   data.defaultValue.INT() = 0;
   data.name = "colorProfile";
   data.shortName = "cp";
   helper.MakeInputInt(data);

   data.defaultValue.STR() = AtString("");
   data.name = "colorSpace";
   data.shortName = "cs";
   helper.MakeInputString(data);

   data.defaultValue.STR() = AtString("");
   data.name = "workingSpace";
   data.shortName = "ws";
   helper.MakeInputString(data);

   data.defaultValue.BOOL() = false;
   data.name = "useFrameExtension";
   data.shortName = "useFrameExtension";
   helper.MakeInputBoolean(data);

   data.defaultValue.FLT() = 0.f;
   data.name = "frame";
   data.shortName = "frame";
   helper.MakeInputFloat(data);

/* 
   // In case we need to have an attribute named imageName
   // so that it's recognized to update color spaces
   
   data.defaultValue.STR = "";
   data.name = "imageName";
   data.shortName = "in";
   helper.MakeInputString(data);
*/
   data.defaultValue.BOOL() = false;
   data.name = "ignoreColorSpaceFileRules";
   data.shortName = "ifr";
   helper.MakeInputBoolean(data);

   // This registers the flename attribute so that it appears in the filepath editor
   MString typeLabel;
   MGlobal::executeCommand("filePathEditor -query -typeLabel aiImage.filename", typeLabel);
   if (typeLabel != MString("Image"))
      MGlobal::executeCommand("filePathEditor -registerType aiImage.filename -typeLabel \"Image\"");
}



AtNode* CAiRaySwitchTranslator::CreateArnoldNodes()
{
   MFnDependencyNode dnode(GetMayaObject());

   MPlugArray conns;
   static MStringArray attributeNames;

   if (attributeNames.length() == 0)
   {
      attributeNames.append("camera");
      attributeNames.append("shadows");
      attributeNames.append("diffuse_reflection");
      attributeNames.append("specular_reflection");
      attributeNames.append("diffuse_transmission");
      attributeNames.append("specular_transmission");
      attributeNames.append("volume");
   }

   bool hasClosures = false;
   for (unsigned int i = 0; i < attributeNames.length(); ++i)
   {

      MPlug inputPlug = dnode.findPlug(attributeNames[i], true);
      if (inputPlug.isNull())
         continue;

      // check if this attribute is connected
      inputPlug.connectedTo(conns, true, false);
      if (conns.length() == 0)
         continue;

      // export the connected node
      AtNode *camNode = ExportConnectedNode(conns[0]);
      if (camNode == NULL)
         continue;

      // check if the output type of the arnold node is closure or not
      if (AiNodeEntryGetOutputType(AiNodeGetNodeEntry(camNode)) == AI_TYPE_CLOSURE)
      {
         hasClosures = true;
         break;
      }
   }

   if (hasClosures)
      return AddArnoldNode("ray_switch_shader");

   
   return AddArnoldNode("ray_switch_rgba");
}

void CAiRaySwitchTranslator::Export(AtNode* shader)
{
   static const AtString raySwitchShaderStr("ray_switch_shader");

   // attribute types is either RGBA or CLOSURE, same as the shader output
   int attrType = AiNodeEntryGetOutputType(AiNodeGetNodeEntry(shader));

   ProcessParameter(shader, "camera", attrType, "camera");
   ProcessParameter(shader, "shadow", attrType, "shadow");
   ProcessParameter(shader, "diffuse_reflection", attrType, "diffuseReflection");
   ProcessParameter(shader, "diffuse_transmission", attrType, "diffuseTransmission");
   ProcessParameter(shader, "specular_reflection", attrType, "specularReflection");
   ProcessParameter(shader, "specular_transmission", attrType,"specularTransmission");
   ProcessParameter(shader, "volume", attrType,  "volume");
}

void CAiRaySwitchTranslator::NodeInitializer(CAbTranslator context)
{
   CExtensionAttrHelper helper(context.maya, "ray_switch_shader");
   CAttrData data;

   data.defaultValue.RGBA() = AI_RGBA_ZERO;
   data.keyable = true;
   data.linkable = true;
   data.name = "camera";
   data.shortName = "camera";
   helper.MakeInputRGBA(data);

   data.name = "shadow";
   data.shortName = "shadow";
   helper.MakeInputRGBA(data);

   data.name = "diffuseReflection";
   data.shortName = "diffuse_reflection";
   helper.MakeInputRGBA(data);

   data.name = "diffuseTransmission";
   data.shortName = "diffuse_transmission";
   helper.MakeInputRGBA(data);

   data.name = "specularReflection";
   data.shortName = "specular_reflection";
   helper.MakeInputRGBA(data);

   data.name = "specularTransmission";
   data.shortName = "specular_transmission";
   helper.MakeInputRGBA(data);

   data.name = "volume";
   data.shortName = "volume";
   helper.MakeInputRGBA(data);
}

// MIX SHADER : presented as the "closure" version.
// But if someone uses it in the middle of the shading tree to mix between different shaders, 
// we switch to the "rgba" version
AtNode* CAiMixShaderTranslator::CreateArnoldNodes()
{
   MFnDependencyNode dnode(GetMayaObject());

   MPlugArray conns;
   static MStringArray attributeNames;

   if (attributeNames.length() == 0)
   {
      attributeNames.append("shader1");
      attributeNames.append("shader2");
   }

   bool hasRGB = false;
   for (unsigned int i = 0; i < attributeNames.length(); ++i)
   {
      MPlug inputPlug = dnode.findPlug(attributeNames[i], true);
      if (inputPlug.isNull())
         continue;

      // check if this attribute is connected
      inputPlug.connectedTo(conns, true, false);
      if (conns.length() == 0)
         continue;

      // export the connected node
      AtNode *camNode = ExportConnectedNode(conns[0]);
      if (camNode == NULL)
         continue;

      // check if the output type of the arnold node is closure or not
      if (AiNodeEntryGetOutputType(AiNodeGetNodeEntry(camNode)) != AI_TYPE_CLOSURE)
      {
         hasRGB = true;
         break;
      }
   }

   if (hasRGB)
      return AddArnoldNode("mix_rgba");
   
   return AddArnoldNode("mix_shader");
}

void CAiMixShaderTranslator::Export(AtNode* shader)
{
   static const AtString mixShaderStr("mix_shader");

   if (AiNodeIs(shader, mixShaderStr))
   {
      ProcessParameter(shader, "shader1", AI_TYPE_CLOSURE, "shader1");
      ProcessParameter(shader, "shader2", AI_TYPE_CLOSURE, "shader2");
      ProcessParameter(shader, "mode", AI_TYPE_INT, "mode");
   } else
   {
      ProcessParameter(shader, "input1", AI_TYPE_RGBA, "shader1");
      ProcessParameter(shader, "input2", AI_TYPE_RGBA, "shader2");
   }   
   ProcessParameter(shader, "mix", AI_TYPE_FLOAT, "mix");
}

void CAiMixShaderTranslator::NodeInitializer(CAbTranslator context)
{
}


// SWITCH SHADER

AtNode* CAiSwitchShaderTranslator::CreateArnoldNodes()
{
   MFnDependencyNode dnode(GetMayaObject());

   MPlugArray conns;
   static MStringArray attributeNames;

   if (attributeNames.length() == 0)
   {
      for (unsigned int i = 0; i < 20; ++i)
      {
         MString attrName = "input";
         attrName += (int)i;
         attributeNames.append(attrName);
      }
   }

   bool hasRGB = false;
   for (unsigned int i = 0; i < attributeNames.length(); ++i)
   {
      MPlug inputPlug = dnode.findPlug(attributeNames[i], true);
      if (inputPlug.isNull())
         continue;

      // check if this attribute is connected
      inputPlug.connectedTo(conns, true, false);
      if (conns.length() == 0)
         continue;

      // export the connected node
      AtNode *camNode = ExportConnectedNode(conns[0]);
      if (camNode == NULL)
         continue;

      // check if the output type of the arnold node is closure or not
      if (AiNodeEntryGetOutputType(AiNodeGetNodeEntry(camNode)) != AI_TYPE_CLOSURE)
      {
         hasRGB = true;
         break;
      }
   }

   if (hasRGB)
      return AddArnoldNode("switch_rgba");
   
   return AddArnoldNode("switch_shader");
}

void CAiSwitchShaderTranslator::Export(AtNode* shader)
{
   MFnDependencyNode dnode(GetMayaObject());
   MPlugArray conns;

   bool isRGBA = AiNodeIs(shader, AtString("switch_rgba"));

   for (unsigned int i = 0; i < 20; ++i)
   {
      MString attrName = "input";
      attrName += (int)i;

      MPlug inputPlug = dnode.findPlug(attrName, true);
      inputPlug.connectedTo(conns, true, false);
      if (conns.length() > 0)
         ProcessParameter(shader, attrName.asChar(), (isRGBA) ? AI_TYPE_RGBA : AI_TYPE_CLOSURE, attrName.asChar());
      else
         AiNodeResetParameter(shader, AtString(attrName.asChar()));
   }

   ProcessParameter(shader, "index", AI_TYPE_INT);
}

void CAiSwitchShaderTranslator::NodeInitializer(CAbTranslator context)
{
}

//////////////////////////////////////////////////////
// PASSTHROUGH SHADER

// MIX SHADER : presented as the "closure" version.
// But if someone uses it in the middle of the shading tree to mix between different shaders, 
// we switch to the "rgba" version
AtNode* CAiPassthroughTranslator::CreateArnoldNodes()
{
   return AddArnoldNode("passthrough");
}

void CAiPassthroughTranslator::Export(AtNode* shader)
{
   ProcessParameter(shader, "passthrough", AI_TYPE_CLOSURE, "passthrough");
   MFnDependencyNode dnode(GetMayaObject());
   MPlugArray conns;


   for (unsigned int i = 0; i < 20; ++i)
   {
      MString attrName = "eval";
      attrName += (int)i;

      MPlug inputPlug = dnode.findPlug(attrName, true);
      inputPlug.connectedTo(conns, true, false);
      if (conns.length() > 0)
         ProcessParameter(shader, attrName.asChar(), AI_TYPE_CLOSURE, attrName.asChar());
      else
         AiNodeResetParameter(shader, AtString(attrName.asChar()));
   }
   ExportBump(shader);
}

void CAiPassthroughTranslator::NodeInitializer(CAbTranslator context)
{
}


//////////////////////////////////////////////////////
AtNode* CAiAovWriteColorTranslator::CreateArnoldNodes()
{
   MFnDependencyNode dnode(GetMayaObject());
   MPlug inputPlug = dnode.findPlug("beauty", true);

   bool isClosure = false;
   if (!inputPlug.isNull())
   {
      MPlugArray conns;
      inputPlug.connectedTo(conns, true, false);
      if (conns.length() > 0)
      {
         // export the connected node
         AtNode *node = ExportConnectedNode(conns[0]);
         if (node && AiNodeEntryGetOutputType(AiNodeGetNodeEntry(node)) == AI_TYPE_CLOSURE)
            isClosure = true;
            
      }
   }

   if (isClosure)
      return AddArnoldNode("aov_write_rgb");

   return AddArnoldNode("MtoaWriteColor");
}

void CAiAovWriteColorTranslator::Export(AtNode* shader)
{
   static const AtString writeColorShader("aov_write_rgb");

   if (AiNodeIs(shader, writeColorShader))
      ProcessParameter(shader, "passthrough", AI_TYPE_CLOSURE, "beauty"); // Closure version of the shader
   else
      ProcessParameter(shader, "passthrough", AI_TYPE_RGBA, "beauty"); // RGBA version of the shader

   ProcessParameter(shader, "blend_opacity", AI_TYPE_BOOLEAN, "blend");
   ProcessParameter(shader, "aov_name", AI_TYPE_STRING,  "aov_name");
   ProcessParameter(shader, "aov_input", AI_TYPE_RGB, "input");

}

void CAiAovWriteColorTranslator::NodeInitializer(CAbTranslator context)
{
}

//////////////////////////////////////////////////////
AtNode* CAiAovWriteFloatTranslator::CreateArnoldNodes()
{
   MFnDependencyNode dnode(GetMayaObject());
   MPlug inputPlug = dnode.findPlug("beauty", true);

   bool isRGBA = false;
   if (!inputPlug.isNull())
   {
      MPlugArray conns;
      inputPlug.connectedTo(conns, true, false);
      if (conns.length() > 0)
      {

         // export the connected node
         AtNode *node = ExportConnectedNode(conns[0]);
         if (node && AiNodeEntryGetOutputType(AiNodeGetNodeEntry(node)) != AI_TYPE_CLOSURE)
            isRGBA = true;
            
      }
   }

   if (isRGBA)
      return AddArnoldNode("MtoaWriteFloat");

   
   return AddArnoldNode("aov_write_float");
}

void CAiAovWriteFloatTranslator::Export(AtNode* shader)
{
   static const AtString writeFloatShader("aov_write_float");

   if (AiNodeIs(shader, writeFloatShader))
      ProcessParameter(shader, "passthrough", AI_TYPE_CLOSURE, "beauty"); // Closure version of the shader
   else
      ProcessParameter(shader, "passthrough", AI_TYPE_RGBA, "beauty"); // RGBA version of the shader

   ProcessParameter(shader, "blend_opacity", AI_TYPE_BOOLEAN, "blend");
   ProcessParameter(shader, "aov_name", AI_TYPE_STRING,  "aov_name");
   ProcessParameter(shader, "aov_input", AI_TYPE_FLOAT, "input");

}

void CAiAovWriteFloatTranslator::NodeInitializer(CAbTranslator context)
{
}

/////////////////
CMayaShadingSwitchTranslator::CMayaShadingSwitchTranslator(const char* nodeType, int paramType) : m_nodeType(nodeType), m_paramType(paramType)
{

}

void CMayaShadingSwitchTranslator::Export(AtNode* shader)
{
   MFnDependencyNode dnode(GetMayaObject());

   unordered_map<std::string, std::vector<AtNode *> > shapeMap;

   MPlug inputPlug = dnode.findPlug("input", true);
   MIntArray existingIndices;
   inputPlug.getExistingArrayAttributeIndices(existingIndices);
   if (existingIndices.length() == 0)
      return;

   // First build a map for all the shapes and the corresponding shaders
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
      AtNode* shader = ExportConnectedNode(inputShaderPlug);
      if (shader == 0)
         continue;
      AtNode* shape = ExportConnectedNode(inputShapePlug);
      if (shape == 0)
         continue;

      shapeMap[AiNodeGetName(shader)].push_back(shape);
      
   }

   if (shapeMap.empty())
      return;

   unordered_map<std::string, std::vector<AtNode *> >::iterator it = shapeMap.begin();
   unordered_map<std::string, std::vector<AtNode *> >::iterator itEnd = shapeMap.end();

   int ind = 0;
   AtNode *previousSwitch = NULL;
      
   for (; it != itEnd; ++it)
   {
      const std::vector<AtNode*> &shapes = it->second;
      if (shapes.empty())
         continue;
      
      AtNode *shapeShader = AiNodeLookUpByName(it->first.c_str());
      if (shapeShader == NULL)
         continue;
      
      // Get a query_shape shader and set the list of shapes
      MString queryKey = "query";
      queryKey += (ind + 1);
      AtNode *query = GetArnoldNode(queryKey.asChar());
      if (query == NULL)
         query = AddArnoldNode("query_shape", queryKey.asChar());

      AiNodeSetArray(query, "shapes", AiArrayConvert((unsigned int) shapes.size(), 1, AI_TYPE_NODE, &(shapes[0])));

      // Now get the switch_rgba shader
      MString switchKey = "switch";
      switchKey += (ind + 1);

      AtNode *switch_rgba = (previousSwitch == NULL) ? shader : GetArnoldNode(switchKey.asChar());
      if (switch_rgba == NULL)
         switch_rgba = AddArnoldNode("switch_rgba", switchKey.asChar());

      AiNodeLink(query, "index", switch_rgba);
      // if condition is verified, we'll have index one, zero otherwise
      AiNodeLink(shapeShader, "input1", switch_rgba);

      if (previousSwitch)
      {
         // link this new layer_rgba shader to previous one's input0
         // so that it's only called if the previous condition was false
         AiNodeLink(switch_rgba, "input0", previousSwitch);
      }
      
      previousSwitch = switch_rgba;
      ind++;
   }
   // We did all the shapes, now let's set the default value on the last switch input0
   if (previousSwitch)
   {
      MPlugArray connections;
      MPlug plug = FindMayaPlug("default");
      plug.connectedTo(connections, true, false);
      if (connections.length() > 0)
         ShaderComputeLink(this, connections[0], previousSwitch, "input0");
      else
      {
         std::vector<std::string> compInputs(3);
         compInputs[0] = "input0.r";
         compInputs[1] = "input0.g";
         compInputs[2] = "input0.b";
         switch(m_paramType)
         {
            {
            case AI_TYPE_FLOAT:
               float val = plug.asFloat();
               AiNodeSetRGBA(previousSwitch, "input0", val, val, val, 1.f);
               break;
            }
            {
            case AI_TYPE_VECTOR2:
               AiNodeSetRGBA(previousSwitch, "input0", plug.child(0).asFloat(), plug.child(1).asFloat(), 0.f, 1.f);
               for (int c = 0; c < 2; ++c)
               {
                  connections.clear();
                  plug.child(c).connectedTo(connections, true, false);
                  if (connections.length() > 0)
                     ShaderComputeLink(this, connections[0], previousSwitch, compInputs[c].c_str());

               }
            break;
            }
            {
            case AI_TYPE_RGB:
               AiNodeSetRGBA(previousSwitch, "input0", plug.child(0).asFloat(), plug.child(1).asFloat(), plug.child(2).asFloat(), 1.f);
               for (int c = 0; c < 3; ++c)
               {
                  connections.clear();
                  plug.child(c).connectedTo(connections, true, false);
                  if (connections.length() > 0)
                     ShaderComputeLink(this, connections[0], previousSwitch, compInputs[c].c_str());

               }
            break;
            }
            {
            default:
            case AI_TYPE_RGBA:
               ProcessParameter(previousSwitch, "input0", AI_TYPE_RGBA, "default");
               break;
            }
         }
      }      
   }
}

AtNode* CMayaShadingSwitchTranslator::CreateArnoldNodes()
{
   return AddArnoldNode("switch_rgba");
}

void* CreateSingleShadingSwitchTranslator()
{
   return new CMayaShadingSwitchTranslator("switch_rgba", AI_TYPE_FLOAT);
}

void* CreateDoubleShadingSwitchTranslator()
{
   return new CMayaShadingSwitchTranslator("switch_rgba", AI_TYPE_VECTOR2);
}

void* CreateTripleShadingSwitchTranslator()
{
   return new CMayaShadingSwitchTranslator("switch_rgba", AI_TYPE_RGB);
}

void* CreateQuadShadingSwitchTranslator()
{
   return new CMayaShadingSwitchTranslator("switch_rgba", AI_TYPE_RGBA);
}


// Toon
//

void CToonTranslator::Export(AtNode* shader)
{
   CShaderTranslator::Export(shader);

   // Now we need to do something special for attribute "rim_light".
   // It is a string in arnold, but it's actually an implicit connection to another node.
   // We need to ensure this connection is created in the translator by calling ExportConnectedNode,
   // and we need to use the real name of the light node, which might be different from the maya name.
   MPlug lightPlug = FindMayaPlug("rimLight");
   if (!lightPlug.isNull())
   {
      MString lightName = lightPlug.asString();
      if (lightName.length() > 0)
      {
         MSelectionList activeList;
         activeList.add(lightName);
         if (activeList.length() > 0)
         {
            MObject lightObj;
            activeList.getDependNode(0, lightObj);
            if (!lightObj.isNull())
            {
               MPlug dummyPlug = MFnDependencyNode(lightObj).findPlug("message", true);
               if (!dummyPlug.isNull())
               {
                  AtNode *lightArnoldNode = ExportConnectedNode(dummyPlug);
                  AtString lightArnoldName = (lightArnoldNode) ? AtString(AiNodeGetName(lightArnoldNode)) : AtString("");
                  AiNodeSetStr(shader, "rim_light", lightArnoldName); 
               }               
            }
         }
      }
   }
}

AtNode* CToonTranslator::CreateArnoldNodes()
{
   return AddArnoldNode("toon");
}

// MultiplyDivide shader
AtNode* CMultiplyDivideTranslator::CreateArnoldNodes()
{
   MPlug operationPlug = FindMayaPlug("operation");
   int op = 0;
   if (!operationPlug.isNull())
      op = operationPlug.asInt();

   switch(op)
   {
      default:
      case 0: // no op
      case 1:
         return AddArnoldNode("multiply");
      case 2:
         return AddArnoldNode("divide");
      case 3:
         return AddArnoldNode("pow");
   }
}

void CMultiplyDivideTranslator::Export(AtNode* shader)
{
   MPlug operationPlug = FindMayaPlug("operation");
   int op = 0;
   if (!operationPlug.isNull())
      op = operationPlug.asInt();

   switch(op)
   {
      default:
      case 0: // no op
         ProcessParameter(shader, "input1", AI_TYPE_RGB);
         break;
      case 1:
         ProcessParameter(shader, "input1", AI_TYPE_RGB);
         ProcessParameter(shader, "input2", AI_TYPE_RGB);
         break;
      case 2:
         ProcessParameter(shader, "input1", AI_TYPE_RGB);
         ProcessParameter(shader, "input2", AI_TYPE_RGB);
         break;         
      case 3:
         ProcessParameter(shader, "base", AI_TYPE_RGB, "input1");
         ProcessParameter(shader, "exponent", AI_TYPE_RGB, "input2");
         break;
   }   
}

// Luminance shader
AtNode* CLuminanceTranslator::CreateArnoldNodes()
{
   return AddArnoldNode("rgb_to_float");
}

void CLuminanceTranslator::Export(AtNode* shader)
{
   AiNodeSetStr(shader, "mode", "luminance");
   ProcessParameter(shader, "input", AI_TYPE_RGB, "value");
}
// Reverse shader
AtNode* CReverseTranslator::CreateArnoldNodes()
{
   return AddArnoldNode("complement");
}

void CReverseTranslator::Export(AtNode* shader)
{
   ProcessParameter(shader, "input", AI_TYPE_RGB, "input");
}

// Condition shader
AtNode* CConditionTranslator::CreateArnoldNodes()
{
   return AddArnoldNode("switch_rgba");
}

void CConditionTranslator::Export(AtNode* shader)
{
   AtNode *compare = GetArnoldNode("compare");
   if (compare == NULL)
      compare = AddArnoldNode("compare", "compare");

   MPlug opPlug = FindMayaPlug("operation");
   if (!opPlug.isNull())
   {
      switch (opPlug.asInt())
      {
         default:
         case 0: // "equal"
            AiNodeSetStr(compare, "test", "==");
         break;
         case 1: //"notequal"
            AiNodeSetStr(compare, "test", "!=");
         break;
         case 2: // "greaterthan"
            AiNodeSetStr(compare, "test", ">");
         break;
         case 3: //"greaterorequal"
            AiNodeSetStr(compare, "test", ">=");
         break;
         case 4: // "lessthan"
            AiNodeSetStr(compare, "test", "<");
         break;
         case 5: // "lessorequal"
            AiNodeSetStr(compare, "test", "<=");
         break;
      }
   }
   ProcessParameter(compare, "input1", AI_TYPE_FLOAT, "firstTerm");
   ProcessParameter(compare, "input2", AI_TYPE_FLOAT, "secondTerm");
   AiNodeLink(compare, "index", shader);
   
   ProcessParameter(shader, "input0", AI_TYPE_RGBA, "colorIfFalse");
   ProcessParameter(shader, "input1", AI_TYPE_RGBA, "colorIfTrue");

}

// Surface Luminance shader
AtNode* CSurfaceLuminanceTranslator::CreateArnoldNodes()
{
   return AddArnoldNode("rgb_to_float");
}

void CSurfaceLuminanceTranslator::Export(AtNode* shader)
{
   AtNode *utility = GetArnoldNode("utility");
   if (utility == NULL)
      utility = AddArnoldNode("utility", "utility");

   AiNodeSetStr(utility, "shade_mode", "lambert");
   // The lambert mode in utility forces a *0.7 factor.... I want this to be white diffuse
   // Also, there's a PI factor to be added to get the same result as surface luminance.
   // Note that we're now considering indirect illumination which wasn't the case before
   AiNodeSetRGB(utility, "color", 3.14f / 0.7f, 3.14f / 0.7f, 3.14f / 0.7f); 
   AiNodeLink(utility, "input", shader);
   AiNodeSetStr(shader, "mode", "luminance");
}

AtNode* CAiRoundCornersTranslator::CreateArnoldNodes()
{
   return AddArnoldNode("round_corners");
}

void CAiRoundCornersTranslator::NodeInitializer(CAbTranslator context)
{
   CExtensionAttrHelper helper(context.maya, "round_corners");
   
   CAttrData data;
   data.defaultValue.FLT() = 0.f;
   data.name = "outAlpha";
   data.shortName = "out_alpha";
   helper.MakeInputFloat(data);

}

void CAiOslShaderTranslator::NodeInitializer(CAbTranslator context)
{
}

AtNode* CAiOslShaderTranslator::CreateArnoldNodes()
{
   return AddArnoldNode("osl");
}
void CAiOslShaderTranslator::Export(AtNode* shader)
{
   // First, we need set the code string, so that all "param*" attributes are created properly on the arnold OSL node.
   ProcessParameter(shader, "code", AI_TYPE_STRING);
   // Now that all parameters were created, we can export all of them
   CNodeTranslator::Export(shader);
}
void CAiOslShaderTranslator::NodeChanged(MObject& node, MPlug& plug)
{
   MString plugName = plug.partialName(false, false, false, false, false, true);
   // If the code has changed, it's better to re-generate the arnold OSL node 
   // as the parameters and output type might become different
   if (plugName == MString("code"))
      SetUpdateMode(AI_RECREATE_NODE);

   CShaderTranslator::NodeChanged(node, plug);
}

AtNode* CGammaCorrectTranslator::CreateArnoldNodes()
{
   return AddArnoldNode("pow");
}

void CGammaCorrectTranslator::Export(AtNode* shader)
{
   ProcessParameter(shader, "base", AI_TYPE_RGB, "value");

   MPlug plug = FindMayaPlug("gamma");
   MPlugArray connections;
   plug.connectedTo(connections, true, false);
   if (connections.length() > 0)
   {
      AtNode* linkedNode = ExportConnectedNode(connections[0]);
      // there is a link on the gamma attribute. We need to insert an invert shader in the middle
      AtNode* invert = GetArnoldNode("invert");
      if (invert == NULL)
         invert = AddArnoldNode("reciprocal", "invert");

      AiNodeLink(linkedNode, "input", invert);   
      AiNodeLink(invert, "exponent", shader);
      
   } else
   {
      // no connection on the gamma attribute, just set the exponent as 1/gamma
      AiNodeSetRGB(shader, "exponent", 1.f / AiMax(AI_EPSILON, plug.child(0).asFloat()), 
                                       1.f / AiMax(AI_EPSILON, plug.child(1).asFloat()), 
                                       1.f / AiMax(AI_EPSILON, plug.child(2).asFloat()));
   }
   
}

// Maya's SurfaceShader
// If no matte is set (outMatteOpacity=white), we just want a standard_surface
// with emission + transparency. If there is matte, then we also need to insert
// a matte shader before the standard_surface, which will be evaluated for camera
// rays and behave like a passthrough for other ray types.
// We need to set the matte's color (same as standard_surface emission) and opacity.
// The matte color is converted to a scalar value here.
AtNode*  CSurfaceShaderTranslator::CreateArnoldNodes()
{
   AtNode *surface = AddArnoldNode("standard_surface");
   if (IsRGBAttrDefault(FindMayaPlug("outMatteOpacity"), 1.f, 1.f, 1.f))
      return surface;

   AtNode *matte = AddArnoldNode("matte", "matte");
   AiNodeLink(surface, "passthrough", matte);
   return matte;
}

void CSurfaceShaderTranslator::Export(AtNode* shader)
{
   shader = GetArnoldNode();
   AtNode *matte = GetArnoldNode("matte");
   AtNode* reverseNode = NULL;
   AtRGB opacity(1.f, 1.f, 1.f);

   ProcessParameter(shader, "emission_color", AI_TYPE_RGB, "outColor");
   AiNodeSetFlt(shader, "emission", 1.f);
   AiNodeSetFlt(shader, "base", 0.f);
   AiNodeSetFlt(shader, "specular", 0.f);
   
   // Transparency to opacity
   MPlug plug = FindMayaPlug("outTransparency");
   if (!plug.isNull())
   {
      MPlugArray connections;
      // For IPR unlink first
      if (AiNodeIsLinked(shader, "opacity")) AiNodeUnlink(shader, "opacity");
      plug.connectedTo(connections, true, false);
      if (connections.length() > 0)
      {
         AtNode* inNode = ExportConnectedNode(connections[0]);
         // Need to reverse it
         if (inNode != NULL)
         {
            MString tag = "outTransparency";
            reverseNode = GetArnoldNode(tag.asChar());
            if (reverseNode == NULL)
               reverseNode = AddArnoldNode("complement", tag.asChar());
            AiNodeLink(inNode, "input", reverseNode);
            AiNodeLink(reverseNode, "opacity", shader);
         }
      } 
      else
      {
         // Need also to support component linking. 
         // FIXME we have the same issue with all the other translations here where we just 
         // ask for the connection on the whole attribute plug
         bool comp_linking = false;
         for (int i = 0; i < 3; ++i)
         {
            plug.child(i).connectedTo(connections, true, false);
            comp_linking |= (connections.length() > 0);
         }
         if (comp_linking)
         {
            MString tag = "outTransparency";
            reverseNode = GetArnoldNode(tag.asChar());
            if (reverseNode == NULL)
               reverseNode = AddArnoldNode("complement", tag.asChar());
            AiNodeLink(reverseNode, "opacity", shader);
            ProcessParameter(reverseNode, "input", AI_TYPE_RGB, "outTransparency" );
         } else
         {
            opacity = AtRGB(1.0f - plug.child(0).asFloat(),
                           1.0f - plug.child(1).asFloat(),
                           1.0f - plug.child(2).asFloat());

            AiNodeSetRGB(shader, "opacity", opacity.r, opacity.g, opacity.b);
         }
      }
   }

   if (matte == NULL)
      return;

   // Need to deal with matte
   AtRGB color = AiNodeGetRGB(shader, "emission_color");
   if (AiNodeIsLinked(shader, "emission_color"))
   {
      // outColor attribute is linked, we need to link each R, G, B component separately
      // because the alpha must be connected to the outMatteOpacity
      AtNode *colorLink = AiNodeGetLink(shader, "emission_color");
      if (colorLink)
      {
         AiNodeLinkOutput(colorLink, "r", matte, "color.r");
         AiNodeLinkOutput(colorLink, "g", matte, "color.g");
         AiNodeLinkOutput(colorLink, "b", matte, "color.b");
      }
   } else // outColor isn't linked, let's just set the parameters (don't worry about color.a, we'll set it below)
      AiNodeSetRGBA(matte, "color", color.r, color.g, color.b, 1.f);

   // matte :  Note, we're not trying to support colored matte opacity, just a scalar matte value (in color.a)
   plug = FindMayaPlug("outMatteOpacity");
   if (!plug.isNull())
   {
      MPlugArray connections;
      plug.connectedTo(connections, true, false);
      if (connections.length() > 0)
      {
         // outMatteOpacity is connected. 
         // To avoid inserting yet another node in this mess for such a rare use case, 
         // we're just linking with the "r" output
         AtNode* inNode = ExportConnectedNode(connections[0]);
         AiNodeLinkOutput(inNode, "r", matte, "color.a");   
      } else
      {
         // outMatteOpacity is not connected, let's set color.a to the value of the color
         float matteVal = AiMax(plug.child(0).asFloat(), AiMax(plug.child(1).asFloat(), plug.child(2).asFloat()));
         AiNodeSetRGBA(matte, "color", color.r, color.g, color.b, matteVal);
      }

      // set matte's opacity to the same value as the standard_surface
      AiNodeSetRGB(matte, "opacity", opacity.r, opacity.g, opacity.b);
      // if we connected something to standard_surface's opacity, let's connect it here too
      if (reverseNode)
         AiNodeLink(reverseNode, "opacity", matte);

   }   
}

void CSurfaceShaderTranslator::NodeChanged(MObject& node, MPlug& plug)
{
   // If I didn't have created the matte before and if outMatteOpacity is being changed, 
   // it means we're likely going to have to create it and insert it at the root of this translator nodes.
   // So let's recreate the node in that case. If the matte shader already exists, we can leave it where it is,
   // even if outMatteOpacity is now set to white during IPR.
   MString plugName = plug.partialName(false, false, false, false, false, true);
   if (plugName == "outMatteOpacity" && (GetArnoldNode("matte") == NULL))
      SetUpdateMode(AI_RECREATE_NODE);

   CShaderTranslator::NodeChanged(node, plug);
}


AtNode* CRgbToHsvTranslator::CreateArnoldNodes()
{
   return AddArnoldNode("color_convert");
}

void CRgbToHsvTranslator::Export(AtNode* shader)
{
   ProcessParameter(shader, "input", AI_TYPE_RGB, "inRgb");
   AiNodeSetStr(shader, "from", "RGB");
   AiNodeSetStr(shader, "to", "HSV");
}

AtNode* CHsvToRgbTranslator::CreateArnoldNodes()
{
   return AddArnoldNode("color_convert");
}

void CHsvToRgbTranslator::Export(AtNode* shader)
{
   ProcessParameter(shader, "input", AI_TYPE_RGB, "inHsv");
   AiNodeSetStr(shader, "from", "HSV");
   AiNodeSetStr(shader, "to", "RGB");  
}

AtNode* CUserDataVec2Translator::CreateArnoldNodes()
{
   return AddArnoldNode("user_data_rgb");
}

void CUserDataVec2Translator::Export(AtNode* shader)
{

   ProcessParameter(shader, "attribute", AI_TYPE_STRING, "vec2AttrName");
   float defX = FindMayaPlug("defaultValueX").asFloat();
   float defY = FindMayaPlug("defaultValueY").asFloat();
   AiNodeSetRGB(shader,"default", defX, defY, 0.0f);

}

AtNode* CUserDataVectorTranslator::CreateArnoldNodes()
{
   return AddArnoldNode("user_data_rgb");
}

void CUserDataVectorTranslator::Export(AtNode* shader)
{

   ProcessParameter(shader, "attribute", AI_TYPE_STRING, "vectorAttrName");
   float defX = FindMayaPlug("defaultValueX").asFloat();
   float defY = FindMayaPlug("defaultValueY").asFloat();
   float defZ = FindMayaPlug("defaultValueZ").asFloat();
   AiNodeSetRGB(shader,"default", defX, defY, defZ);

}
AtNode* CUserDataBoolTranslator::CreateArnoldNodes()
{
   return AddArnoldNode("user_data_int");
}

void CUserDataBoolTranslator::Export(AtNode* shader)
{

   ProcessParameter(shader, "attribute", AI_TYPE_STRING, "boolAttrName");
   AiNodeSetInt(shader,"default", FindMayaPlug("defaultValue").asBool() ? 1 : 0);
}

AtNode* CContrastTranslator::CreateArnoldNodes()
{
   MPlug contrastPlug = FindMayaPlug("contrast");
   MPlug biasPlug = FindMayaPlug("bias");

   AtRGB contrast(contrastPlug.child(0).asFloat(), contrastPlug.child(1).asFloat(), contrastPlug.child(2).asFloat());
   AtRGB bias(biasPlug.child(0).asFloat(), biasPlug.child(1).asFloat(), biasPlug.child(2).asFloat());
   
   if (std::abs(contrast.r - contrast.g) < AI_EPSILON && std::abs(contrast.r - contrast.b) < AI_EPSILON &&
      std::abs(bias.r - bias.g) < AI_EPSILON && std::abs(bias.r - bias.b) < AI_EPSILON)
      m_isRgb = false;
   else
      m_isRgb = true;

   if (m_isRgb)
   {
      AddArnoldNode("range", "range_r");
      AddArnoldNode("range", "range_g");
      AddArnoldNode("range", "range_b");
      AddArnoldNode("flat", "flat");
      return AddArnoldNode("float_to_rgb");
   }

   return AddArnoldNode("range");
}

void CContrastTranslator::Export(AtNode* shader)
{
   if (!m_isRgb)
   {
      ProcessParameter(shader, "input", AI_TYPE_RGB, "value");
      float contrast = FindMayaPlug("contrast").child(0).asFloat();
      AiNodeSetFlt(shader, "gain", 1.f - powf(2.f, -contrast));
      float bias = FindMayaPlug("bias").child(0).asFloat();
      AiNodeSetFlt(shader, "bias", 1.f - bias);
      return;
   }
   AtNode *flat = GetArnoldNode("flat");
   AtNode *range_r = GetArnoldNode("range_r");
   AtNode *range_g = GetArnoldNode("range_g");
   AtNode *range_b = GetArnoldNode("range_b");
   AiNodeLink(range_r, "r", shader);
   AiNodeLink(range_g, "g", shader);
   AiNodeLink(range_b, "b", shader);
   AiNodeLinkOutput(flat, "r", range_r, "input");
   AiNodeLinkOutput(flat, "g", range_g, "input");
   AiNodeLinkOutput(flat, "b", range_b, "input");
   ProcessParameter(flat, "color", AI_TYPE_RGB, "value");
   MPlug contrastPlug = FindMayaPlug("contrast");
   AtRGB contrast(contrastPlug.child(0).asFloat(), contrastPlug.child(1).asFloat(), contrastPlug.child(2).asFloat());
   MPlug biasPlug = FindMayaPlug("bias");
   AtRGB bias(biasPlug.child(0).asFloat(), biasPlug.child(1).asFloat(), biasPlug.child(2).asFloat());
   AiNodeSetFlt(range_r, "gain", 1.f - powf(2.f, -contrast.r));
   AiNodeSetFlt(range_g, "gain", 1.f - powf(2.f, -contrast.g));
   AiNodeSetFlt(range_b, "gain", 1.f - powf(2.f, -contrast.b));
   AiNodeSetFlt(range_r, "bias", 1.f - bias.r);
   AiNodeSetFlt(range_g, "bias", 1.f - bias.g);
   AiNodeSetFlt(range_b, "bias", 1.f - bias.b);
}


void CContrastTranslator::NodeChanged(MObject& node, MPlug& plug)
{
   MString plugName = plug.partialName(false, false, false, false, false, true);
   if (!m_isRgb && (plugName == "contrast" || plugName == "bias"))
      SetUpdateMode(AI_RECREATE_NODE);
   
   CShaderTranslator::NodeChanged(node, plug);

}

AtNode* CSetRangeTranslator::CreateArnoldNodes()
{
   AtNode* X_Range = AddArnoldNode("range", "X_Range");
   AtNode* Y_Range = AddArnoldNode("range", "Y_Range");
   AtNode* Z_Range = AddArnoldNode("range", "Z_Range");
   AtNode* outFlat = AddArnoldNode("flat");
   AtNode* inFlat = AddArnoldNode("flat", "inFlat");

   AiNodeLink(X_Range, "color.r", outFlat);
   AiNodeLink(Y_Range, "color.g", outFlat);
   AiNodeLink(Z_Range, "color.b", outFlat);

   AiNodeLinkOutput(inFlat, "r", X_Range, "input");
   AiNodeLinkOutput(inFlat, "g", Y_Range, "input");
   AiNodeLinkOutput(inFlat, "b", Z_Range, "input");
   

   return outFlat;
}
void CSetRangeTranslator::Export(AtNode* shader)
{
   AtNode* X_Range = GetArnoldNode("X_Range");
   AtNode* Y_Range = GetArnoldNode("Y_Range");
   AtNode* Z_Range = GetArnoldNode("Z_Range");
   AtNode* inFlat  = GetArnoldNode("inFlat");
   //AtNode* outFlat = shader;
   
   ProcessParameter(inFlat, "color", AI_TYPE_RGB, "value");
   
   ProcessParameter(X_Range, "input_min", AI_TYPE_FLOAT, "oldMinX");
   ProcessParameter(Y_Range, "input_min", AI_TYPE_FLOAT, "oldMinY");
   ProcessParameter(Z_Range, "input_min", AI_TYPE_FLOAT, "oldMinZ");
   ProcessParameter(X_Range, "input_max", AI_TYPE_FLOAT, "oldMaxX");
   ProcessParameter(Y_Range, "input_max", AI_TYPE_FLOAT, "oldMaxY");
   ProcessParameter(Z_Range, "input_max", AI_TYPE_FLOAT, "oldMaxZ");

   ProcessParameter(X_Range, "output_min", AI_TYPE_FLOAT, "minX");
   ProcessParameter(Y_Range, "output_min", AI_TYPE_FLOAT, "minY");
   ProcessParameter(Z_Range, "output_min", AI_TYPE_FLOAT, "minZ");
   ProcessParameter(X_Range, "output_max", AI_TYPE_FLOAT, "maxX");
   ProcessParameter(Y_Range, "output_max", AI_TYPE_FLOAT, "maxY");
   ProcessParameter(Z_Range, "output_max", AI_TYPE_FLOAT, "maxZ");
}

void CRampRgbTranslator::NodeInitializer(CAbTranslator context)
{
   CExtensionAttrHelper helper("aiRampRgb");
   CAttrData data;
   data.name = "ramp";
   data.shortName = "aiRamp";
   helper.MakeInputColorRamp(data);
}

AtNode* CRampRgbTranslator::CreateArnoldNodes()
{
   return AddArnoldNode("ramp_rgb");
}
void CRampRgbTranslator::Export(AtNode* shader)
{
   ProcessParameter(shader, "type", AI_TYPE_INT);
   ProcessParameter(shader, "input", AI_TYPE_FLOAT);
   ProcessParameter(shader, "uvset", AI_TYPE_STRING);
   ProcessParameter(shader, "use_implicit_uvs", AI_TYPE_INT);
     
   MFnDependencyNode fnNode(GetMayaObject());
   MPlug plug;
   plug = FindMayaPlug("ramp");
   MObject opos = fnNode.attribute("ramp_Position");
   ProcessArrayParameter(shader, "position", plug, AI_TYPE_FLOAT, &opos);
   MObject ocol = fnNode.attribute("ramp_Color");
   ProcessArrayParameter(shader, "color", plug, AI_TYPE_RGB, &ocol);
   MObject oint = fnNode.attribute("ramp_Interp");
   ProcessArrayParameter(shader, "interpolation", plug, AI_TYPE_INT, &oint);

   AtArray *interpArray = AiNodeGetArray(shader, "interpolation");
   unsigned int numElems = (interpArray) ? AiArrayGetNumElements(interpArray) : 0;
   for (unsigned int i = 0; i < numElems; ++i)
   {
      switch (AiArrayGetInt(interpArray, i))
      {
         default: // 0: None and 1: Linear are already ok
            break;
         case 2: // smooth
            AiArraySetInt(interpArray, i, 6); // Smooth is 6 in ramp_rgb
            break;
         case 3: // spline
            AiArraySetInt(interpArray, i, 2); // Converting "Spline" as "Catmull-Rom" (2)
            break;
      }
   }
}

void CRampFloatTranslator::NodeInitializer(CAbTranslator context)
{
   CExtensionAttrHelper helper("aiRampFloat");
   
   CAttrData data;
   data.name = "ramp";
   data.shortName = "aiRamp";
   helper.MakeInputCurveRamp(data);
}

AtNode* CRampFloatTranslator::CreateArnoldNodes()
{
   return AddArnoldNode("ramp_float");
}
void CRampFloatTranslator::Export(AtNode* shader)
{
   ProcessParameter(shader, "type", AI_TYPE_INT);
   ProcessParameter(shader, "input", AI_TYPE_FLOAT);
   ProcessParameter(shader, "uvset", AI_TYPE_STRING);
   MFnDependencyNode fnNode(GetMayaObject());
   MPlug plug;
   plug = FindMayaPlug("ramp");
   MObject opos = fnNode.attribute("ramp_Position");
   ProcessArrayParameter(shader, "position", plug, AI_TYPE_FLOAT, &opos);
   MObject ocol = fnNode.attribute("ramp_FloatValue");
   ProcessArrayParameter(shader, "value", plug, AI_TYPE_FLOAT, &ocol);
   MObject oint = fnNode.attribute("ramp_Interp");
   ProcessArrayParameter(shader, "interpolation", plug, AI_TYPE_INT, &oint);

   AtArray *interpArray = AiNodeGetArray(shader, "interpolation");
   unsigned int numElems = (interpArray) ? AiArrayGetNumElements(interpArray) : 0;
   for (unsigned int i = 0; i < numElems; ++i)
   {
      switch (AiArrayGetInt(interpArray, i))
      {
         default: // 0: None and 1: Linear are already ok
            break;
         case 2: // smooth
            AiArraySetInt(interpArray, i, 6); // Smooth is 6 in ramp_rgb
            break;
         case 3: // spline
            AiArraySetInt(interpArray, i, 2); // Converting "Spline" as "Catmull-Rom" (2)
            break;
      }
   }  
}

void CStandardSurfaceTranslator::Export(AtNode* shader)
{
   // Base
   ProcessParameter(shader, "base", AI_TYPE_FLOAT);
   ProcessParameter(shader, "base_color", AI_TYPE_RGB);
   ProcessParameter(shader, "diffuse_roughness", AI_TYPE_FLOAT);
   ProcessParameter(shader, "metalness", AI_TYPE_FLOAT);

   // Specular
   ProcessParameter(shader, "specular", AI_TYPE_FLOAT);
   ProcessParameter(shader, "specular_color", AI_TYPE_RGB);
   ProcessParameter(shader, "specular_roughness", AI_TYPE_FLOAT);
   ProcessParameter(shader, "specular_IOR", AI_TYPE_FLOAT);
   ProcessParameter(shader, "specular_anisotropy", AI_TYPE_FLOAT);
   ProcessParameter(shader, "specular_rotation", AI_TYPE_FLOAT);

   // Transmission
   ProcessParameter(shader, "transmission", AI_TYPE_FLOAT);
   ProcessParameter(shader, "transmission_color", AI_TYPE_RGB);
   ProcessParameter(shader, "transmission_depth", AI_TYPE_FLOAT);
   ProcessParameter(shader, "transmission_scatter", AI_TYPE_RGB);
   ProcessParameter(shader, "transmission_scatter_anisotropy", AI_TYPE_FLOAT);
   ProcessParameter(shader, "transmission_dispersion", AI_TYPE_FLOAT);
   ProcessParameter(shader, "transmission_extra_roughness", AI_TYPE_FLOAT);
   ProcessParameter(shader, "transmit_aovs", AI_TYPE_BOOLEAN);

   // Subsurface
   ProcessParameter(shader, "subsurface", AI_TYPE_FLOAT);
   ProcessParameter(shader, "subsurface_color", AI_TYPE_RGB);
   ProcessParameter(shader, "subsurface_radius", AI_TYPE_RGB);
   ProcessParameter(shader, "subsurface_scale", AI_TYPE_FLOAT);
   ProcessParameter(shader, "subsurface_anisotropy", AI_TYPE_FLOAT);
   ProcessParameter(shader, "subsurface_type", AI_TYPE_ENUM);
   // Anrold specific: ENUM          subsurface_type                   randomwalk

   // Coat
   ProcessParameter(shader, "coat", AI_TYPE_FLOAT);
   ProcessParameter(shader, "coat_color", AI_TYPE_RGB);
   ProcessParameter(shader, "coat_roughness", AI_TYPE_FLOAT);
   ProcessParameter(shader, "coat_IOR", AI_TYPE_FLOAT);
   ProcessParameter(shader, "coat_anisotropy", AI_TYPE_FLOAT);
   ProcessParameter(shader, "coat_rotation", AI_TYPE_FLOAT);
   ProcessParameter(shader, "coat_normal", AI_TYPE_VECTOR);
   ProcessParameter(shader, "coat_affect_color", AI_TYPE_FLOAT);
   ProcessParameter(shader, "coat_affect_roughness", AI_TYPE_FLOAT);

   // Sheen
   ProcessParameter(shader, "sheen", AI_TYPE_FLOAT);
   ProcessParameter(shader, "sheen_color", AI_TYPE_RGB);
   ProcessParameter(shader, "sheen_roughness", AI_TYPE_FLOAT);

   // Emission
   ProcessParameter(shader, "emission", AI_TYPE_FLOAT);
   ProcessParameter(shader, "emission_color", AI_TYPE_RGB);

   // Thin Film
   ProcessParameter(shader, "thin_film_thickness", AI_TYPE_FLOAT);
   ProcessParameter(shader, "thin_film_IOR", AI_TYPE_FLOAT);

   // Geometry
   ProcessParameter(shader, "thin_walled", AI_TYPE_BOOLEAN);
   ProcessParameter(shader, "opacity", AI_TYPE_RGB);
   ProcessParameter(shader, "normal", AI_TYPE_VECTOR, "normalCamera");
   ProcessParameter(shader, "tangent", AI_TYPE_VECTOR, "tangentUCamera");


   // AOV's 
   ProcessParameter(shader, "aov_id1", AI_TYPE_STRING);
   ProcessParameter(shader, "id1", AI_TYPE_RGB);

   ProcessParameter(shader, "aov_id2", AI_TYPE_STRING);
   ProcessParameter(shader, "id2", AI_TYPE_RGB);

   ProcessParameter(shader, "aov_id3", AI_TYPE_STRING);
   ProcessParameter(shader, "id3", AI_TYPE_RGB);

   ProcessParameter(shader, "aov_id4", AI_TYPE_STRING);
   ProcessParameter(shader, "id4", AI_TYPE_RGB);

   ProcessParameter(shader, "aov_id5", AI_TYPE_STRING);
   ProcessParameter(shader, "id5", AI_TYPE_RGB);

   ProcessParameter(shader, "aov_id6", AI_TYPE_STRING);
   ProcessParameter(shader, "id6", AI_TYPE_RGB);

   ProcessParameter(shader, "aov_id7", AI_TYPE_STRING);
   ProcessParameter(shader, "id7", AI_TYPE_RGB);
   
   ProcessParameter(shader, "aov_id8", AI_TYPE_STRING);
   ProcessParameter(shader, "id8", AI_TYPE_RGB);

   // Advanced Attributes
   ProcessParameter(shader, "caustics", AI_TYPE_BOOLEAN);
   ProcessParameter(shader, "exit_to_background", AI_TYPE_BOOLEAN);
   ProcessParameter(shader, "internal_reflections", AI_TYPE_BOOLEAN);
   ProcessParameter(shader, "indirect_diffuse", AI_TYPE_FLOAT);
   ProcessParameter(shader, "indirect_specular", AI_TYPE_FLOAT);
}

AtNode* CStandardSurfaceTranslator::CreateArnoldNodes()
{
   return ProcessAOVOutput(AddArnoldNode("standard_surface"));
}

void CStandardSurfaceTranslator::NodeInitializer(CAbTranslator context)
{
   CExtensionAttrHelper helper("standardSurface");
   CAttrData data;

   // SSS Attributes 
      data.name = "subsurfaceType";
      data.shortName = "subsurface_type";
      MStringArray strArr;
      strArr.append("diffusion");
      strArr.append("randomwalk");
      strArr.append("randomwalk_v2");
      data.enums = strArr;
      data.defaultValue.INT() = 1;
      helper.MakeInputEnum(data);
      
   // Transmission Attributes 
      data.name = "transmitAovs";
      data.shortName = "transmit_aovs";
      helper.MakeInputBoolean(data);


   // Matte Attributes 
      data.name = "aiEnableMatte";
      data.shortName = "ai_enable_matte";
      helper.MakeInputBoolean(data);
      data.defaultValue.BOOL() = false;

      data.name = "aiMatteColor";
      data.shortName = "ai_matte_color";
      helper.MakeInputRGB(data);

      data.name = "aiMatteColorA";
      data.shortName = "ai_matte_color_a";
      data.hasMin = true;
      data.min.FLT() = 0.f;
      data.hasMax = true;
      data.max.FLT() = 1.0;
      data.defaultValue.FLT() = 0.0f;
      helper.MakeInputFloat(data);

   // AOV Attributes 
      data.name = "aovId1";
      data.shortName = "aov_id1";
      helper.MakeInputString(data);

      data.name = "id1";
      data.shortName = "id1";
      helper.MakeInputRGB(data);

      data.name = "aovId2";
      data.shortName = "aov_id2";
      helper.MakeInputString(data);

      data.name = "id2";
      data.shortName = "id2";
      helper.MakeInputRGB(data);

      data.name = "aovId3";
      data.shortName = "aov_id3";
      helper.MakeInputString(data);

      data.name = "id3";
      data.shortName = "id3";
      helper.MakeInputRGB(data);

      data.name = "aovId4";
      data.shortName = "aov_id4";
      helper.MakeInputString(data);

      data.name = "id4";
      data.shortName = "id4";
      helper.MakeInputRGB(data);

      data.name = "aovId5";
      data.shortName = "aov_id5";
      helper.MakeInputString(data);

      data.name = "id5";
      data.shortName = "id5";
      helper.MakeInputRGB(data);

      data.name = "aovId6";
      data.shortName = "aov_id6";
      helper.MakeInputString(data);

      data.name = "id6";
      data.shortName = "id6";
      helper.MakeInputRGB(data);

      data.name = "aovId7";
      data.shortName = "aov_id7";
      helper.MakeInputString(data);

      data.name = "id7";
      data.shortName = "id7";
      helper.MakeInputRGB(data);

      data.name = "aovId8";
      data.shortName = "aov_id8";
      helper.MakeInputString(data);

      data.name = "id8";
      data.shortName = "id8";
      helper.MakeInputRGB(data);

   // Advanced Attributes 
      
      data.name = "caustics";
      data.shortName = "caustics";
      helper.MakeInputBoolean(data);

      data.name = "exitToBackground";
      data.shortName = "exit_to_background";
      helper.MakeInputBoolean(data);

      data.name = "internalReflections";
      data.shortName = "internal_reflections";
      data.defaultValue.BOOL() = true;
      helper.MakeInputBoolean(data);

      data.name = "indirectDiffuse";
      data.shortName = "indirect_diffuse";
      helper.MakeInputFloat(data);

      data.name = "indirectSpecular";
      data.shortName = "indirect_specular";
      helper.MakeInputFloat(data);

}
void CStandardSurfaceTranslator::NodeChanged(MObject& node, MPlug& plug)
{
   MString plugName = plug.partialName(false, false, false, false, false, true);
   if (plugName == "aiEnableMatte" || plugName == "aiMatteColor" || plugName == "aiMatteColorA" )
         SetUpdateMode(AI_RECREATE_NODE); // I need to re-generate the shaders, so that they include the matte at the root of the shading tree

   CShaderTranslator::NodeChanged(node, plug);
}