#include "LookDevKitTranslator.h"
#include "../shaders/utils.h"
#include "extension/Extension.h"
#include <cassert>

#include <maya/MFnNumericData.h>

void CLookDevKitTranslator::ExportRGBAChannels(AtNode *shader, const char *arnoldParam, const char *rgbParam, const char *alphaParam)
{ 
   ProcessParameter(shader, arnoldParam, AI_TYPE_RGBA, rgbParam); 
      
   MPlug alphaPlug = FindMayaPlug(MString(alphaParam));
   MPlugArray connections;
   alphaPlug.connectedTo(connections, true, false);
   MString arnoldAlphaChannel = MString(arnoldParam) + MString(".a");

   bool isLinked = AiNodeIsLinked(shader, arnoldParam);

   if (connections.length() > 0)
   {
      AtNode* srcArnoldNode = ExportConnectedNode(connections[0]);
      // don't do the link if we're already connected to the same shader
      if ((!isLinked) || (srcArnoldNode != AiNodeGetLink(shader, arnoldParam)))
         AiNodeLink(srcArnoldNode, arnoldAlphaChannel.asChar(), shader);
      
   } else 
   {
      float alpha = alphaPlug.asFloat();
      if (alpha < 1.f)
      {
         if (!isLinked)
         {
            AtRGB col = AiNodeGetRGB(shader, arnoldParam);
            AiNodeSetRGBA(shader, arnoldParam, col.r, col.g, col.b, alpha);
         } else
         {
            // FIXME: color is connected and a transparent alpha is used, we need to insert a node
         }
      }
   }
}


AtNode* CLookDevKitTranslator::CreateArnoldNodes()
{
   MString nodeType = MFnDependencyNode(GetMayaObject()).typeName();
   if (nodeType == MString("channels"))
      nodeType = MString("shuffle");
   else if (nodeType == MString("colorComposite"))
      nodeType = MString("layer_rgba");
   else if (nodeType == MString("colorCondition"))
      nodeType = MString("switch_rgba");
   else if (nodeType == MString("colorLogic"))
      nodeType = MString("compare");
   else if (nodeType == MString("colorConstant"))
      nodeType = MString("shuffle");
   else if (nodeType == MString("floatLogic"))
      nodeType = MString("compare");
   else if (nodeType == MString("floatConstant"))
      nodeType = MString("layer_float");
   else
   {
	   MString prefix = nodeType.substringW(0, 0);
	   nodeType = nodeType.substringW(1, nodeType.length() - 1);
	   nodeType = "ldk" + prefix.toUpperCase() + nodeType;
   } 
   AtNode *node = AddArnoldNode(nodeType.asChar());
   return node;
}

void CLookDevKitTranslator::Export(AtNode* shader)
{
   MString nodeType = MFnDependencyNode(GetMayaObject()).typeName();
   if (nodeType == MString("channels"))
   {
      ProcessParameter(shader, "color", AI_TYPE_RGB, "inColor");
      ProcessParameter(shader, "alpha", AI_TYPE_FLOAT, "inAlpha");
      ProcessParameter(shader, "channel_r", AI_TYPE_INT, "channelR");
      ProcessParameter(shader, "channel_g", AI_TYPE_INT, "channelG");
      ProcessParameter(shader, "channel_b", AI_TYPE_INT, "channelB");
      ProcessParameter(shader, "channel_a", AI_TYPE_INT, "channelA");
      return;
   } else if (nodeType == MString("colorComposite"))
   {
      ExportRGBAChannels(shader, "input1", "colorA", "alphaA");
      ExportRGBAChannels(shader, "input2", "colorB", "alphaB");

      bool linkedA = AiNodeIsLinked(shader, "input1");
      bool linkedB = AiNodeIsLinked(shader, "input2");
      
      ProcessParameter(shader, "mix2", AI_TYPE_FLOAT, "factor");
      AiNodeSetBool(shader, "enable3", false);
      AiNodeSetBool(shader, "enable4", false);
      AiNodeSetBool(shader, "enable5", false);
      AiNodeSetBool(shader, "enable6", false);
      AiNodeSetBool(shader, "enable7", false);
      AiNodeSetBool(shader, "enable8", false);

      MPlug operationPlug = FindMayaPlug("operation");
      if (!operationPlug.isNull())
      {
         switch(operationPlug.asInt())
         {
            default:
            case COP_ADD:
               AiNodeSetStr(shader, "operation2", "plus");
            break;
            case COP_SUBTRACT:
               AiNodeSetStr(shader, "operation2", "subtract");
            break;
            case COP_MIX:
               AiNodeSetStr(shader, "operation2", "overwrite");
            break;
            case COP_MULTIPLY:
               AiNodeSetStr(shader, "operation2", "multiply");            
            break;
            case COP_SCREEN:
               // Note: in the past we were actually doing "add" here
               AiNodeSetStr(shader, "operation2", "screen");            
            break;
            case COP_OVERLAY:
               // overlay formula seemed to be wrong before
               AiNodeSetStr(shader, "operation2", "overlay");
            break;
            case COP_DIFFERENCE:
               // FIXME: should this be "minus" or "difference" ? it used to be "minus"...
               AiNodeSetStr(shader, "operation2", "difference");
            break;
            case COP_DODGE:
               AiNodeSetStr(shader, "operation2", "color_dodge");
            break;
            case COP_BURN:
               // The previous result seemed to be wrong
               AiNodeSetStr(shader, "operation2", "color_burn");
            break;
         }
      }

      
   } else if (nodeType == MString("colorCondition"))
   {
      ProcessParameter(shader, "index", AI_TYPE_INT, "condition");
      ExportRGBAChannels(shader, "input1", "colorA", "alphaA");
      ExportRGBAChannels(shader, "input0", "colorB", "alphaB");

   } else if (nodeType == MString("colorLogic"))
   {
      AtNode *rgb2floatA = GetArnoldNode("rgb2floatA");
      if (rgb2floatA == NULL)
         rgb2floatA = AddArnoldNode("rgb_to_float", "rgb2floatA");

      AtNode *rgb2floatB = GetArnoldNode("rgb2floatB");
      if (rgb2floatB == NULL)
         rgb2floatB = AddArnoldNode("rgb_to_float", "rgb2floatB");

      AiNodeSetStr(rgb2floatA, "mode", "sum");
      AiNodeSetStr(rgb2floatB, "mode", "sum");

      ProcessParameter(rgb2floatA, "input", AI_TYPE_RGB, "colorA");
      ProcessParameter(rgb2floatB, "input", AI_TYPE_RGB, "colorB");

      AiNodeLink(rgb2floatA, "input1", shader);
      AiNodeLink(rgb2floatB, "input2", shader);


      MPlug operationPlug = FindMayaPlug("operation");
      if (!operationPlug.isNull())
      {
         MString operation = operationPlug.asString();
         AiNodeSetStr(shader, "test", operation.asChar());
      }
   
   } else if (nodeType == MString("colorConstant"))
   {
      ProcessParameter(shader, "color", AI_TYPE_RGB, "inColor");
      ProcessParameter(shader, "alpha", AI_TYPE_FLOAT, "inAlpha");
   }  else if (nodeType == MString("floatConstant"))
   {
      ProcessParameter(shader, "input1", AI_TYPE_FLOAT, "inFloat");
   } else if (nodeType == MString("floatLogic"))
   {
      ProcessParameter(shader, "input1", AI_TYPE_FLOAT, "floatA");
      ProcessParameter(shader, "input2", AI_TYPE_FLOAT, "floatB");

      MPlug operationPlug = FindMayaPlug("operation");
      if (!operationPlug.isNull())
      {
         MString operation = operationPlug.asString();
         AiNodeSetStr(shader, "test", operation.asChar());
      }
   } else // default behaviour
      CNodeTranslator::Export(shader);
}


