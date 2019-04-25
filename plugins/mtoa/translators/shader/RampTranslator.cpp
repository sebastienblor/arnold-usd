#include "ShaderTranslators.h"
#include "platform/Platform.h"

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

#include <utils/MayaUtils.h>



void CRampTranslator::NodeInitializer(CAbTranslator context)
{
   CExtensionAttrHelper helper("ramp");
   
   CAttrData data;
   
   data.defaultValue.BOOL() = true;
   data.name = "aiCurveImplicitUvs";
   data.shortName = "ai_curve_implicit_uvs";
   data.channelBox = false;
   data.keyable = false;       
   helper.MakeInputBoolean(data);
}

// Ramp
//
AtNode*  CRampTranslator::CreateArnoldNodes()
{
   m_type = FindMayaPlug("type").asInt();
   m_custom_uvs = NULL;
   AtNode *shader = (m_type == RT_4CORNER) ? AddArnoldNode("mix_rgba") : AddArnoldNode("ramp_rgb");
   m_hasColorCorrect = RequiresColorCorrect();
   AtNode *cc = (m_hasColorCorrect) ? AddArnoldNode("color_correct", "cc") : NULL;
   
   AtNode *comp_uv = (m_type == RT_UV || m_type == RT_TARTAN) ? AddArnoldNode("composite", "comp_uv") : NULL;

   if (cc)
      return cc;

   if (comp_uv)
      return comp_uv;
   
   return shader;
}

void CRampTranslator::Export(AtNode* shader)
{              
   GetUvSet();
   AtNode *uvTransformNode = ExportUvTransform();

   if (m_type != RT_4CORNER)
   {
      // if I have a uv_transform, I need to be in "custom" type
      // and link to its input
      if (uvTransformNode)
      {
         AiNodeLinkOutput(uvTransformNode, "r", shader, "input");
         AiNodeSetStr(shader, "type", "custom");
      }
      else if (m_custom_uvs)
      {
         int outputType = AiNodeEntryGetOutputType(AiNodeGetNodeEntry(m_custom_uvs));
         if (outputType == AI_TYPE_VECTOR2 || outputType == AI_TYPE_VECTOR || 
            outputType == AI_TYPE_RGB ||outputType == AI_TYPE_RGBA)
         {
            if (m_type == RT_U)
               AiNodeLinkOutput(m_custom_uvs, (outputType == AI_TYPE_VECTOR2 || outputType == AI_TYPE_VECTOR) ? "x" : "r",
                  shader, "input");
            else if (m_type == RT_V)
               AiNodeLinkOutput(m_custom_uvs, (outputType == AI_TYPE_VECTOR2 || outputType == AI_TYPE_VECTOR) ? "y" : "g",
                  shader, "input");
            else
               AiNodeLink(m_custom_uvs, "input", shader);   
         } else
            AiNodeLink(m_custom_uvs, "input", shader);
      }
      else
         ExportRampType(shader, (RampType)m_type); // set the proper ramp type
      
      ExportRampKeys(shader); // export colors, positions, interpolations

      // Set UVset on the ramp and the eventual second ramp
      AiNodeSetStr(shader, "uvset", m_uvSet.asChar());
      // FIXME also need to set it eventually to ramp_float shaders....
      if (FindMayaPlug("aiCurveImplicitUvs").asBool())
         AiNodeSetStr(shader, "use_implicit_uvs", "curves_only");
      else
         AiNodeSetStr(shader, "use_implicit_uvs", "off");

      if (m_type == RT_UV || m_type == RT_TARTAN)
      {
         AtNode *comp_uv = GetArnoldNode("comp_uv");
         if (comp_uv == NULL)
            comp_uv = AddArnoldNode("composite", "comp_uv"); // shouldn't happen

         AtNode *ramp2 = GetArnoldNode("ramp2");
         if (ramp2 == NULL)
            ramp2 = AddArnoldNode("ramp_rgb", "ramp2");
         
         AiNodeLinkOutput(uvTransformNode, "g", ramp2, "input");
         AiNodeSetStr(ramp2, "type", "custom");
         ExportRampKeys(ramp2); // export colors, positions, interpolations
         AiNodeSetStr(ramp2, "uvset", AiNodeGetStr(shader, "uvset"));
         AiNodeSetStr(ramp2, "use_implicit_uvs", AiNodeGetStr(shader, "use_implicit_uvs"));

         if (uvTransformNode)
         {
            AiNodeSetBool(uvTransformNode, "mirror_u", true);
            AiNodeSetBool(uvTransformNode, "mirror_v", true);
            AtVector2 repeat = AiNodeGetVec2(uvTransformNode, "repeat");
            AiNodeSetVec2(uvTransformNode, "repeat", repeat.x * 2.f, repeat.y * 2.f);
            AtVector2 translate = AiNodeGetVec2(uvTransformNode, "translate_frame");
            AiNodeSetVec2(uvTransformNode, "translate_frame", translate.x  + 0.5f, translate.y + 0.5f);
         }

         AiNodeLink(shader, "A", comp_uv);
         AiNodeLink(ramp2, "B", comp_uv);
         AiNodeSetStr(comp_uv, "operation", (m_type == RT_UV) ? "multiply" : "average");
         shader = comp_uv; // this is the root shader now, in case we have a color correct node
      }
   } else
   {
      // 4 corners, our "shader" is a mix node
      AtNode *mixLeft = GetArnoldNode("mix_left");
      if (mixLeft == NULL)
         mixLeft = AddArnoldNode("mix_rgba", "mix_left");

      AtNode *mixRight = GetArnoldNode("mix_right");
      if (mixRight == NULL)
         mixRight = AddArnoldNode("mix_rgba", "mix_right");

      AiNodeLink(mixLeft, "input1", shader);
      AiNodeLink(mixRight, "input2", shader);
         
      std::vector<AtRGB> colors(4, AI_RGB_BLACK);

      MFnDependencyNode fnNode(GetMayaObject());
      MPlug plug, elem, col, elemPlug;
      MPlugArray conn;
      plug = FindMayaPlug("colorEntryList");
      MObject ocol = fnNode.attribute("color");
      MObject opos = fnNode.attribute("position");

      unsigned int size = AiMin(plug.numElements(), 4u); // we're only interested in the first 4 colors

      std::vector<AtNode *> mixNodes(4);
      mixNodes[0] = mixNodes[1] = mixLeft;
      mixNodes[2] = mixNodes[3] = mixRight;
      std::vector<std::string > paramNames(4);
      paramNames[0] = paramNames[3] = "input1";
      paramNames[1] = paramNames[2] = "input2";
      
      std::vector<AtVector2> posShuffle;
      posShuffle.reserve(size);
      for (unsigned int i = 0; i < size; ++i)
      {
         float pos = plug[i].child(opos).asFloat();
         AtVector2 elem(float(i), pos);
         bool inserted = false;
         for (unsigned int j = 0; j < posShuffle.size(); ++j)
         {
            if (pos < posShuffle[j].y)
            {
               inserted = true;
               // this position should be placed before an existing element
               posShuffle.insert(posShuffle.begin() + j, elem);
               break;
            }
         }
         if (!inserted)
            posShuffle.push_back(elem);
      }
      
      for (unsigned int i = 0; i < size; ++i)
      {
         MPlug childPlug = plug[int(posShuffle[i].x)].child(ocol);
         ProcessParameter(mixNodes[i], paramNames[i].c_str(), AI_TYPE_RGBA, childPlug);
         
         /*
         AiNodeSetRGBA(mixNodes[i], paramNames[i].c_str(), childPlug.child(0).asFloat(),
                                                          childPlug.child(1).asFloat(),
                                                          childPlug.child(2).asFloat(), 1.f);
         */
      }
      
      if (uvTransformNode)   
      {
         AiNodeLinkOutput(uvTransformNode, "g", mixLeft, "mix");
         AiNodeLinkOutput(uvTransformNode, "g", mixRight, "mix");
         AiNodeLinkOutput(uvTransformNode, "r", shader, "mix");           
      } else
      {
         AtNode *rampFloat = GetArnoldNode("ramp_float");
         if (rampFloat == NULL)
            rampFloat = AddArnoldNode("ramp_float", "ramp_float");
         AtNode *rampFloat2 = GetArnoldNode("ramp_float2");
         if (rampFloat2 == NULL)
            rampFloat2 = AddArnoldNode("ramp_float", "ramp_float2");

         if (FindMayaPlug("aiCurveImplicitUvs").asBool())
         {
            AiNodeSetStr(rampFloat, "use_implicit_uvs", "curves_only");
            AiNodeSetStr(rampFloat2, "use_implicit_uvs", "curves_only");
         } else
         {
            AiNodeSetStr(rampFloat, "use_implicit_uvs", "off");
            AiNodeSetStr(rampFloat2, "use_implicit_uvs", "off");
         }

         ExportRampType(rampFloat, RT_U);
         ExportRampType(rampFloat2, RT_V);
         AiNodeSetStr(rampFloat, "uvset", m_uvSet.asChar());
         AiNodeSetStr(rampFloat2, "uvset", m_uvSet.asChar());
         AiNodeLink(rampFloat2, "mix", mixLeft);
         AiNodeLink(rampFloat2, "mix", mixRight);
         AiNodeLink(rampFloat, "mix", shader);
      }

   }

   if (m_hasColorCorrect)
      ExportColorCorrect(shader);
}


AtNode *CRampTranslator::ExportColorCorrect(AtNode *target)
{
   AtNode *colorCorrectNode = GetArnoldNode("cc");
   if (!colorCorrectNode)
      return target;
   ProcessParameter(colorCorrectNode, "alpha_is_luminance", AI_TYPE_BOOLEAN, "alphaIsLuminance");
   // when "invert" is enabled, we want it to invert both the RGB and the alpha
   // this is done through 2 different attributes in color_correct
   ProcessParameter(colorCorrectNode, "invert", AI_TYPE_BOOLEAN, "invert");
   ProcessParameter(colorCorrectNode, "invert_alpha", AI_TYPE_BOOLEAN, "invert");
   ProcessParameter(colorCorrectNode, "alpha_multiply", AI_TYPE_FLOAT, "alphaGain");
   ProcessParameter(colorCorrectNode, "alpha_add", AI_TYPE_FLOAT, "alphaOffset");
   ProcessParameter(colorCorrectNode, "multiply", AI_TYPE_RGB, "colorGain");
   ProcessParameter(colorCorrectNode, "add", AI_TYPE_RGB, "colorOffset");
   AiNodeLink(target, "input", colorCorrectNode);

   if (!IsFloatAttrDefault(FindMayaPlug("hueNoise"), 0.f))
   {
      AtNode *noise = GetArnoldNode("hue_noise");
      if (noise == NULL)
         noise = AddArnoldNode("noise", "hue_noise");
      AiNodeLink(noise, "hue_shift", colorCorrectNode);
      AiNodeSetStr(noise, "coord_space", "uv");
      AiNodeSetStr(noise, "mode", "scalar");
      float noiseFreq = FindMayaPlug("hueNoiseFreq").asFloat();
      AiNodeSetVec(noise, "scale", 16*noiseFreq, 16*noiseFreq, 16*noiseFreq);
      AiNodeSetVec(noise, "offset", 0.75, 0.75, 0.75);
      AiNodeSetFlt(noise, "amplitude", 1.f);
      float hueNoise = FindMayaPlug("hueNoise").asFloat();
      AiNodeSetRGB(noise, "color1", 1.f - hueNoise, 1.f - hueNoise, 1.f - hueNoise);
      AiNodeSetRGB(noise, "color2", 1.f + hueNoise, 1.f + hueNoise, 1.f + hueNoise);
   } else
      AiNodeUnlink(colorCorrectNode, "hue_shift");

   if (!IsFloatAttrDefault(FindMayaPlug("satNoise"), 0.f))
   {
      AtNode *noise = GetArnoldNode("sat_noise");
      if (noise == NULL)
         noise = AddArnoldNode("noise", "sat_noise");
      AiNodeLink(noise, "saturation", colorCorrectNode);
      AiNodeSetStr(noise, "coord_space", "uv");
      AiNodeSetStr(noise, "mode", "scalar");
      float noiseFreq = FindMayaPlug("satNoiseFreq").asFloat();
      AiNodeSetVec(noise, "scale", 16*noiseFreq, 16*noiseFreq, 16*noiseFreq);
      AiNodeSetVec(noise, "offset", 0.75, 0.75, 0.75);
      AiNodeSetFlt(noise, "amplitude", 1.f);
      float satNoise = FindMayaPlug("satNoise").asFloat();
      AiNodeSetRGB(noise, "color1", 1.f - satNoise, 1.f - satNoise, 1.f - satNoise);
      AiNodeSetRGB(noise, "color2", 1.f + satNoise, 1.f + satNoise, 1.f + satNoise);
   }  else
      AiNodeUnlink(colorCorrectNode, "saturation");

   if (!IsFloatAttrDefault(FindMayaPlug("valNoise"), 0.f))
   {
      // FIXME we don't support value noise + colorGain linked to another shader
      AtNode *noise = GetArnoldNode("val_noise");
      if (noise == NULL)
         noise = AddArnoldNode("noise", "val_noise");
      AtRGB mult = AiNodeGetRGB(colorCorrectNode, "multiply");
      
      AiNodeSetRGB(noise, "color2", mult.r, mult.g, mult.b);
      
      AiNodeLink(noise, "multiply", colorCorrectNode);
      AiNodeSetStr(noise, "coord_space", "uv");
      AiNodeSetStr(noise, "mode", "scalar");
      float noiseFreq = FindMayaPlug("valNoiseFreq").asFloat();
      AiNodeSetVec(noise, "scale", 16*noiseFreq, 16*noiseFreq, 16*noiseFreq);
      AiNodeSetVec(noise, "offset", 0.75, 0.75, 0.75);
      AiNodeSetFlt(noise, "amplitude", 1.f);
      float valNoise = FindMayaPlug("valNoise").asFloat();
      AiNodeSetRGB(noise, "color1", mult.r * (1.f - valNoise), mult.g*(1.f - valNoise),mult.b*(1.f - valNoise));
      AiNodeSetRGB(noise, "color2", mult.r*(1.f + valNoise),mult.g*(1.f + valNoise), mult.g*(1.f + valNoise));
      
   } 
   
   return colorCorrectNode;
}

// Set the color / position / interpolation arrays
void CRampTranslator::ExportRampKeys(AtNode *shader)
{
   MFnDependencyNode fnNode(GetMayaObject());
   MPlug plug;
   plug = FindMayaPlug("colorEntryList");
   MObject opos = fnNode.attribute("position");
   ProcessArrayParameter(shader, "position", plug, AI_TYPE_FLOAT, &opos);
   MObject ocol = fnNode.attribute("color");
   ProcessArrayParameter(shader, "color", plug, AI_TYPE_RGB, &ocol);   
   AtArray *posArray = AiNodeGetArray(shader, "position");
   int numPositions = (posArray) ? AiArrayGetNumElements(posArray) : 1;

   // In ramp_rgb the interpolation is an array, but we should be able to set a single value
   RampInterpolationType interpolation = (RampInterpolationType) FindMayaPlug("interpolation").asInt();
   std::vector<int> interpVector;
   switch (interpolation)
   {
      case RIT_NONE:
         interpVector.assign(numPositions, 0);
      break;
      default:
      case RIT_LINEAR:
         interpVector.assign(numPositions, 1);
      break;
      case RIT_EXP_UP:
         interpVector.assign(numPositions, 4);
      break;
      case RIT_EXP_DOWN:
         interpVector.assign(numPositions, 5);
      break;
      case RIT_SMOOTH:
         interpVector.assign(numPositions, 6);
      break;
      case RIT_BUMP:
         interpVector.assign(numPositions, 7);
      break;
      case RIT_SPIKE:
         interpVector.assign(numPositions, 8);
      break;
   };
   AiNodeSetArray(shader, "interpolation", AiArrayConvert((unsigned int)interpVector.size(), 1, AI_TYPE_INT, &interpVector[0]));

}

// Set the correct type in the ramp
void CRampTranslator::ExportRampType(AtNode *rampShader, RampType type)
{
   if (m_custom_uvs)
   {
      AiNodeSetStr(rampShader, "type", "custom");
      return;
   }
   switch(type)
   {
      case RT_V:
         AiNodeSetStr(rampShader, "type", "v");
      break;
      case RT_U:
         AiNodeSetStr(rampShader, "type", "u");
      break;
      case RT_DIAGONAL:
         AiNodeSetStr(rampShader, "type", "diagonal");
      break;
      case RT_RADIAL:
         AiNodeSetStr(rampShader, "type", "radial");
      break;
      case RT_CIRCULAR:
         AiNodeSetStr(rampShader, "type", "circular");
      break;
      case RT_BOX:
         AiNodeSetStr(rampShader, "type", "box");
      break;
      default:
      break;
   }
}

void CRampTranslator::GetUvSet()
{
   m_uvSet = MString("");
   MFnDependencyNode fnNode(GetMayaObject());   
   MPlugArray connections;
   MPlug uvCoordPlug = FindMayaPlug("uvCoord");
   uvCoordPlug.connectedTo(connections, true, false);

   if (connections.length() != 0)
   {
      MObject srcObj = connections[0].node();
      MFnDependencyNode srcNodeFn(srcObj);

      if (srcNodeFn.typeName() == "place2dTexture")
      {
         srcNodeFn.findPlug("uvCoord", true).connectedTo(connections, true, false);
         if (connections.length() > 0)
         {
            MFnDependencyNode uvcNodeFn(connections[0].node());
            if (uvcNodeFn.typeName() == "uvChooser")
               m_uvSet = uvcNodeFn.findPlug("uvSets", true).elementByPhysicalIndex(0).asString();
               return;
         }
      } else
      {
         m_custom_uvs = ExportConnectedNode(connections[0]);
      }
   } else
   {
      // Nothing linked to uvCoord, let's see if we connected shaders to uCoord, vCoord
      MPlugArray conn;
      if (m_type == RT_U)
      {
         uvCoordPlug.child(0).connectedTo(conn, true, false);
         if (conn.length() != 0)
            m_custom_uvs = ExportConnectedNode(conn[0]);
      } else if (m_type == RT_V)
      {
         uvCoordPlug.child(1).connectedTo(conn, true, false);
         if (conn.length() != 0)
            m_custom_uvs = ExportConnectedNode(conn[0]);
      }
   }
}

AtNode* CRampTranslator::ExportUvTransform()
{
   if (!RequiresUvTransform())
      return NULL;

   AtNode *uvTransformNode = GetArnoldNode("uv");
   if (uvTransformNode == NULL)
      uvTransformNode = AddArnoldNode("uv_transform", "uv");

   if (uvTransformNode == NULL)
      return NULL;

   // First, convert all the parameters from the place2dTexture into the uv_transform
   MFnDependencyNode fnNode(GetMayaObject());   
   MPlugArray connections;
   fnNode.findPlug("uvCoord", true).connectedTo(connections, true, false);
   if (connections.length() != 0)
   {
      MObject srcObj = connections[0].node();
      MFnDependencyNode srcNodeFn(srcObj);
      if (srcNodeFn.typeName() == "place2dTexture")
      {

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
      }
   }

   AiNodeSetRGBA(uvTransformNode, "passthrough", 0.f, 0.f, 0.f, 1.f);
   AiNodeSetStr(uvTransformNode, "uvset", m_uvSet.asChar());
   // if we set the uvset on the uv_transform node, it should not be set on the linked shaders

   AtNode *uvTransformWave = NULL;

   bool uWave = !IsFloatAttrDefault(FindMayaPlug("uWave"), 0.f);
   bool vWave = !IsFloatAttrDefault(FindMayaPlug("vWave"), 0.f);

   bool hasNoise = !IsFloatAttrDefault(FindMayaPlug("noise"), 0.f);
   AtNode *noise = NULL;

   // Do we have noise on the uv coordinates
   if (hasNoise)
   {
      noise = GetArnoldNode("noise");
      if (noise == NULL)
         noise = AddArnoldNode("noise", "noise");

      AiNodeSetStr(noise, "coord_space", "uv");
      AiNodeSetStr(noise, "mode", "scalar");
      float noiseFreq = FindMayaPlug("noiseFreq").asFloat();
      AiNodeSetVec(noise, "scale", 16*noiseFreq, 16*noiseFreq, 16*noiseFreq);
      AiNodeSetVec(noise, "offset", 0.75, 0.75, 0.75);
      AiNodeLink(noise, "offset", uvTransformNode);
      ProcessParameter(noise, "amplitude", AI_TYPE_FLOAT, "noise");

      // In case we have noise + uWave/vWave, then we need 2 uv_transform nodes (bwah)
      if (uWave || vWave)
      {
         uvTransformWave = GetArnoldNode("uv_wave");
         if (uvTransformWave == NULL)
            uvTransformWave = AddArnoldNode("uv_transform", "uv_wave");

         AiNodeSetRGBA(uvTransformWave, "passthrough", 0.f, 0.f, 0.f, 1.f);
         AiNodeLink(uvTransformWave, "passthrough", uvTransformNode);
      } 
   } 


   // Do we have uv waves
   if (vWave || uWave)
   {
      AtNode *mult = GetArnoldNode("wave_mult");
      if (mult == NULL)
         mult = AddArnoldNode("multiply", "wave_mult");
      // we link multiply to the offset of uvTransform that is used for the wave
      AiNodeLink(mult, "offset", uvTransformWave ? uvTransformWave : uvTransformNode);
      // multiply's input1 has the uWave, vWave values, they will be multiplied by the trigo's result
      AiNodeSetRGB(mult, "input1", -FindMayaPlug("uWave").asFloat(), FindMayaPlug("vWave").asFloat(), 0.f);

      AtNode *state_u = NULL;
      AtNode *state_v = NULL;
      // create the required state_float nodes for u, v
      // note that u_wave is a variation on v, and vice-versa
      if (uWave)
      {
         state_v = GetArnoldNode("state_v");
         if (state_v == NULL)
            state_v = AddArnoldNode("state_float", "state_v");
         AiNodeSetStr(state_v, "variable", "v");
      }
      if (vWave)
      {
         state_u = GetArnoldNode("state_u");
         if (state_u == NULL)
            state_u = AddArnoldNode("state_float", "state_u");
         AiNodeSetStr(state_u, "variable", "u");
      }

      AtNode *trigo = GetArnoldNode("trigo");
      if (trigo == NULL)
         trigo = AddArnoldNode("trigo", "trigo");

      // trigo's input is linked to the UV values that are needed
      AiNodeUnlink(trigo, "input");
      AiNodeSetRGB(trigo, "input", 0.f, 0.f, 0.f);
      if (state_v)
         AiNodeLink(state_v, "input.r", trigo);
      if (state_u)
         AiNodeLink(state_u, "input.g", trigo);
      AiNodeSetFlt(trigo, "frequency", AI_PITIMES2); // this is what MayaRamp's code was doing....
      AiNodeSetStr(trigo, "function", "sin");

      // trigo is linked to the 2nd input of multiply
      AiNodeLink(trigo, "input2", mult);
   } 

   // All this ends with a ramp_float where I'm copying the ramp type from the main one
   // FIXME: for UV & Tartan types, we actually need 2 ramp_float
   AtNode *rampFloat = GetArnoldNode("ramp_float");
   if (rampFloat == NULL)
   {
      rampFloat = AddArnoldNode("ramp_float", "ramp_float");
      AiNodeSetArray(rampFloat, "interpolation", AiArray(2, 1, AI_TYPE_INT, 1, 1));
   }
   // if we're in UV / Tartan, it should be ramps with U & V types
   AiNodeSetStr(rampFloat, "uvset", ""); // the uvset was already set on the uv_transform node
   if (FindMayaPlug("aiCurveImplicitUvs").asBool())
      AiNodeSetStr(rampFloat, "use_implicit_uvs", "curves_only");
   else
      AiNodeSetStr(rampFloat, "use_implicit_uvs", "off");
   
   AtNode *rampFloat2 = NULL;

   if (m_type == RT_UV || m_type == RT_TARTAN || m_type == RT_4CORNER)
   {
      // Mamma mia, we need a second ramp_float
      rampFloat2 = GetArnoldNode("ramp_float2");
      if (rampFloat2 == NULL)
         rampFloat2 = AddArnoldNode("ramp_float", "ramp_float2");

      ExportRampType(rampFloat, RT_U);
      ExportRampType(rampFloat2, RT_V);
      AiNodeSetStr(rampFloat2, "uvset", ""); // the uvset was already set on the uv_transform node
      AiNodeLink(rampFloat, "passthrough.r", uvTransformWave ? uvTransformWave : uvTransformNode);
      AiNodeLink(rampFloat2, "passthrough.g", uvTransformWave ? uvTransformWave : uvTransformNode);
   } else
   {
      ExportRampType(rampFloat, (RampType)m_type);
      // the float ramp is linked to the 2nd uv_transform  if it exists. Otherwise it goes on the first one.
      AiNodeLink(rampFloat, "passthrough", uvTransformWave ? uvTransformWave : uvTransformNode);   
   }

   if (m_custom_uvs)
   {
      if (rampFloat)
         AiNodeLink(m_custom_uvs, "input", rampFloat);
      if (rampFloat2)
         AiNodeLink(m_custom_uvs, "input", rampFloat2);
      else
      {
         int outputType = AiNodeEntryGetOutputType(AiNodeGetNodeEntry(m_custom_uvs));
         if (m_type == RT_V && (outputType == AI_TYPE_RGBA || outputType == AI_TYPE_RGB || outputType == AI_TYPE_VECTOR2 || outputType == AI_TYPE_VECTOR))
            AiNodeLinkOutput(m_custom_uvs, (outputType == AI_TYPE_VECTOR || outputType == AI_TYPE_VECTOR2) ? "y" : "g", uvTransformNode, "passthrough");
         else
            AiNodeLink(m_custom_uvs, "passthrough", uvTransformNode);
      }

   }
   
   return uvTransformNode;
}

bool CRampTranslator::RequiresUvTransform() const
{
   if (m_type == RT_UV || m_type == RT_TARTAN) 
      return true;

   if (! (IsFloatAttrDefault(FindMayaPlug("uWave"), 0.f) &&
          IsFloatAttrDefault(FindMayaPlug("vWave"), 0.f) &&
          IsFloatAttrDefault(FindMayaPlug("noise"), 0.f)))
      return true;             

   MPlugArray connections;
   MPlug plug = FindMayaPlug("uvCoord");
   plug.connectedTo(connections, true, false);

   if (connections.length() == 0)
      return false;

   MObject srcObj = connections[0].node();
   MFnDependencyNode srcNodeFn(srcObj);

   if (srcNodeFn.typeName() != "place2dTexture")
      return false;

   if (IsBoolAttrDefault(srcNodeFn.findPlug("wrapU", true), true) &&
       IsBoolAttrDefault(srcNodeFn.findPlug("wrapV", true), true))
      return true;

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
bool CRampTranslator::RequiresColorCorrect() const
{
   return ! (IsFloatAttrDefault(FindMayaPlug("alphaGain"), 1.f) &&
             IsFloatAttrDefault(FindMayaPlug("alphaOffset"), 0.f) &&
             IsBoolAttrDefault(FindMayaPlug("alphaIsLuminance"), false) &&
             IsBoolAttrDefault(FindMayaPlug("invert"), false) && 
             IsRGBAttrDefault(FindMayaPlug("colorGain"), 1.f, 1.f, 1.f) &&
             IsRGBAttrDefault(FindMayaPlug("colorOffset"), 0.f, 0.f, 0.f)  &&
             IsFloatAttrDefault(FindMayaPlug("hueNoise"), 0.f) &&
             IsFloatAttrDefault(FindMayaPlug("valNoise"), 0.f) &&
             IsFloatAttrDefault(FindMayaPlug("satNoise"), 0.f));
}

void CRampTranslator::NodeChanged(MObject& node, MPlug& plug)
{
   MString plugName = plug.partialName(false, false, false, false, false, true);
   if (plugName == "type")
      SetUpdateMode(AI_RECREATE_NODE);

   if ((plugName == "alphaGain" || plugName == "alphaOffset" || plugName == "alphaIsLuminance" || plugName == "invert" || 
      plugName == "colorGain" || plugName == "colorOffset" || plugName == "hueNoise" ||
      plugName == "valNoise" || plugName == "satNoise") &&
      !RequiresColorCorrect())
      SetUpdateMode(AI_RECREATE_NODE);
/*
if ((plugName == "uvCoord" || plugName == "uWave" || plugName == "vWave" || plugName == "noise") &&
      !RequiresUvTransform())
      SetUpdateMode(AI_RECREATE_NODE);*/
   CShaderTranslator::NodeChanged(node, plug);
   
}
