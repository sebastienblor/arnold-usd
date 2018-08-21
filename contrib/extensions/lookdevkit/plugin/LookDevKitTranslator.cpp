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
   else if (nodeType == MString("colorMask"))
      nodeType = MString("color_correct");
   else if (nodeType == MString("colorMath"))
      nodeType = MString("layer_rgba");
   else if (nodeType == MString("floatLogic"))
      nodeType = MString("compare");
   else if (nodeType == MString("floatCondition"))
      nodeType = MString("rgb_to_float");
   else if (nodeType == MString("floatConstant"))
      nodeType = MString("layer_float");
   else if (nodeType == MString("floatComposite"))
      nodeType = MString("rgb_to_float");
   else if (nodeType == MString("floatMask"))
      nodeType = MString("rgb_to_float");
   else if (nodeType == MString("floatMath"))
      nodeType = MString("rgb_to_float");
   else
   {
	   MString prefix = nodeType.substringW(0, 0);
	   nodeType = nodeType.substringW(1, nodeType.length() - 1);
	   nodeType = "ldk" + prefix.toUpperCase() + nodeType;
   } 
   AtNode *node = AddArnoldNode(nodeType.asChar());
   return node;
}


const char* OperationStrings[] =
{
   "==",
   "!=",
   "<",
   ">",
   "<=",
   ">=",
   NULL
};

enum operation
{
   OP_ADD = 0,
   OP_SUBTRACT,
   OP_MULTIPLY,
   OP_DIVIDE,
   OP_MIN,
   OP_MAX,
   OP_POW
};

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
   } else if (nodeType == MString("colorMask"))
   {
      ExportRGBAChannels(shader, "input", "inColor", "inAlpha");
      MPlug alphaIsLuminancePlug = FindMayaPlug("maskAlphaIsLuminance");
      AtNode *rgb_to_float = NULL;
      if (!alphaIsLuminancePlug.isNull() && alphaIsLuminancePlug.asBool())
      {
         rgb_to_float = GetArnoldNode("rgb_to_float");
         if (rgb_to_float == NULL)
            rgb_to_float = AddArnoldNode("rgb_to_float", "rgb_to_float");
         AiNodeSetStr(rgb_to_float, "mode", "luminance");
         ProcessParameter(rgb_to_float, "input", AI_TYPE_RGB, "mask");
      }
      AtNode *float_to_rgb = GetArnoldNode("float_to_rgb");
      if (float_to_rgb == NULL)
         float_to_rgb = AddArnoldNode("float_to_rgb", "float_to_rgb");

      if (rgb_to_float)
      {
         AiNodeLink(rgb_to_float, "r", float_to_rgb);
         AiNodeLink(rgb_to_float, "g", float_to_rgb);
         AiNodeLink(rgb_to_float, "b", float_to_rgb);
      } else
      {
         ProcessParameter(float_to_rgb, "r", AI_TYPE_FLOAT, "maskAlpha");
         ProcessParameter(float_to_rgb, "g", AI_TYPE_FLOAT, "maskAlpha");
         ProcessParameter(float_to_rgb, "b", AI_TYPE_FLOAT, "maskAlpha");
      } 

      AtNode *negate = GetArnoldNode("negate");
      if (negate == NULL)
         negate = AddArnoldNode("negate", "negate");

      AiNodeLink(float_to_rgb, "input", negate);

      AtNode *rgb_to_float2 = GetArnoldNode("rgb_to_float2");
      if (rgb_to_float2 == NULL)
         rgb_to_float2 = AddArnoldNode("rgb_to_float", "rgb_to_float2");
      AiNodeLink(negate, "input", rgb_to_float2);

      AiNodeLink(rgb_to_float2, "alpha_add", shader);

      MPlug preserveColorPlug = FindMayaPlug("preserveColor");
      if (!preserveColorPlug.isNull() && !preserveColorPlug.asBool())
      {
         AtNode *complement = GetArnoldNode("complement");
         if (complement == NULL)
            complement = AddArnoldNode("complement", "complement");

         AiNodeLink(float_to_rgb, "input", complement);
         AiNodeLink(complement, "multiply", shader);
      } else
         AiNodeResetParameter(shader, "multiply");
      
   } else if (nodeType == MString("floatConstant"))
   {
      ProcessParameter(shader, "input1", AI_TYPE_FLOAT, "inFloat");
   } else if (nodeType == MString("floatLogic"))
   {
      ProcessParameter(shader, "input1", AI_TYPE_FLOAT, "floatA");
      ProcessParameter(shader, "input2", AI_TYPE_FLOAT, "floatB");

      MPlug operationPlug = FindMayaPlug("operation");
      if (!operationPlug.isNull())
      {         
         int op = operationPlug.asInt();
         if (op >= 0 && op <= 5)
            AiNodeSetStr(shader, "test", OperationStrings[op]);
      }
   } else if (nodeType == MString("colorMath"))
   {
      ExportRGBAChannels(shader, "input1", "colorA", "alphaA");
      ExportRGBAChannels(shader, "input2", "colorB", "alphaB");

      MPlug opPlug = FindMayaPlug("operation");
      if (!opPlug.isNull())
      {
         int operation = opPlug.asInt();
         switch (operation)
         {
            default:
            case OP_ADD: // add
               AiNodeSetStr(shader, "operation2", "plus");
               break;
            case OP_SUBTRACT:  // subtract
               AiNodeSetStr(shader, "operation2", "minus");
               break;
            case OP_MULTIPLY: // multiply
               AiNodeSetStr(shader, "operation2", "multiply");
               break;
            case OP_DIVIDE: // divide
               AiNodeSetStr(shader, "operation2", "divide");
               break;
            case OP_MIN:  // Min
               AiNodeSetStr(shader, "operation2", "min");
               break;
            case OP_MAX: // Max
               AiNodeSetStr(shader, "operation2", "max");
               break;
         }
      }
      AiNodeSetBool(shader, "enable3", false);
      AiNodeSetBool(shader, "enable4", false);
      AiNodeSetBool(shader, "enable5", false);
      AiNodeSetBool(shader, "enable6", false);
      AiNodeSetBool(shader, "enable7", false);
      AiNodeSetBool(shader, "enable8", false);
   } else if (nodeType == MString("floatMath"))
   {     
      MPlug opPlug = FindMayaPlug("operation");
      std::string opName;
      if (!opPlug.isNull())
      {
         int operation = opPlug.asInt();
         switch (operation)
         {
            default:
            case OP_ADD: // add
               opName = "add";
               break;
            case OP_SUBTRACT:  // subtract
               opName = "subtract";
               break;
            case OP_MULTIPLY: // multiply
               opName = "multiply";
               break;
            case OP_DIVIDE: // divide
               opName = "divide";
               break;
            case OP_MIN:  // Min
               opName = "min";
               break;
            case OP_MAX: // Max
               opName = "max";
            case OP_POW: // Pow
               opName = "pow";
               break;
         }
      }
      AtNode *mathNode = GetArnoldNode(opName.c_str());
      if (mathNode == NULL)
         mathNode = AddArnoldNode(opName.c_str(), opName.c_str());

      AiNodeLink(mathNode, "input", shader);
      AiNodeSetStr(shader, "mode", "r");

      if (opName == "pow")
      {
         ProcessParameter(mathNode, "base.r", AI_TYPE_FLOAT, "floatA");
         ProcessParameter(mathNode, "exponent.r", AI_TYPE_FLOAT, "floatB");
      } else
      {
         ProcessParameter(mathNode, "input1.r", AI_TYPE_FLOAT, "floatA");
         ProcessParameter(mathNode, "input2.r", AI_TYPE_FLOAT, "floatB");
      }
   }  else if (nodeType == MString("floatCondition"))
   {
      AtNode *switchShader = GetArnoldNode("switch_rgba");
      if (switchShader == NULL)
         switchShader = AddArnoldNode("switch_rgba", "switch_rgba");

      ProcessParameter(switchShader, "input0.r", AI_TYPE_FLOAT, "floatB");
      ProcessParameter(switchShader, "input1.r", AI_TYPE_FLOAT, "floatA");
      ProcessParameter(switchShader, "index", AI_TYPE_INT, "condition");
      AiNodeLink(switchShader, "input", shader);
      AiNodeSetStr(shader, "mode", "r");
   } else if (nodeType == MString("floatComposite"))
   {
      AtNode *layerRgba = GetArnoldNode("layer_rgba");
      if (layerRgba == NULL)
         layerRgba = AddArnoldNode("layer_rgba", "layer_rgba");

      ProcessParameter(layerRgba, "input1.r", AI_TYPE_FLOAT, "floatA");
      ProcessParameter(layerRgba, "input2.r", AI_TYPE_FLOAT, "floatB");
      ProcessParameter(layerRgba, "mix2", AI_TYPE_FLOAT, "factor");
      AiNodeSetBool(layerRgba, "enable3", false);
      AiNodeSetBool(layerRgba, "enable4", false);
      AiNodeSetBool(layerRgba, "enable5", false);
      AiNodeSetBool(layerRgba, "enable6", false);
      AiNodeSetBool(layerRgba, "enable7", false);
      AiNodeSetBool(layerRgba, "enable8", false);

      MPlug operationPlug = FindMayaPlug("operation");
      if (!operationPlug.isNull())
      {
         switch(operationPlug.asInt())
         {
            default:
            case COP_ADD:
               AiNodeSetStr(layerRgba, "operation2", "plus");
            break;
            case COP_SUBTRACT:
               AiNodeSetStr(layerRgba, "operation2", "subtract");
            break;
            case COP_MIX:
               AiNodeSetStr(layerRgba, "operation2", "overwrite");
            break;
            case COP_MULTIPLY:
               AiNodeSetStr(layerRgba, "operation2", "multiply");            
            break;
            case COP_SCREEN:
               // Note: in the past we were actually doing "add" here
               AiNodeSetStr(layerRgba, "operation2", "screen");            
            break;
            case COP_OVERLAY:
               // overlay formula seemed to be wrong before
               AiNodeSetStr(layerRgba, "operation2", "overlay");
            break;
            case COP_DIFFERENCE:
               // FIXME: should this be "minus" or "difference" ? it used to be "minus"...
               AiNodeSetStr(layerRgba, "operation2", "difference");
            break;
            case COP_DODGE:
               AiNodeSetStr(layerRgba, "operation2", "color_dodge");
            break;
            case COP_BURN:
               // The previous result seemed to be wrong
               AiNodeSetStr(layerRgba, "operation2", "color_burn");
            break;
         }
      }

      AiNodeLink(layerRgba, "input", shader);
      AiNodeSetStr(shader, "mode", "r");
   } else if (nodeType == MString("floatMask"))
   {
      AtNode *subtract = GetArnoldNode("subtract");
      if (subtract == NULL)
         subtract = AddArnoldNode("subtract", "subtract");

      ProcessParameter(subtract, "input1.r", AI_TYPE_FLOAT, "inFloat");
      ProcessParameter(subtract, "input2.r", AI_TYPE_FLOAT, "mask");
      AiNodeLink(subtract, "input", shader);
      AiNodeSetStr(shader, "mode", "r");
   } else // default behaviour
      CNodeTranslator::Export(shader);
}


