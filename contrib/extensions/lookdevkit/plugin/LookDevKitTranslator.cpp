#include "LookDevKitTranslator.h"
#include "../shaders/utils.h"
#include "extension/Extension.h"
#include <cassert>

#include <maya/MFnNumericData.h>

AtNode* CLookDevKitTranslator::CreateArnoldNodes()
{
   MString nodeType = MFnDependencyNode(GetMayaObject()).typeName();
   if (nodeType == MString("channels"))
      nodeType = MString("shuffle");
   else if (nodeType == MString("colorComposite"))
      nodeType = MString("layer_rgba");
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
      ProcessParameter(shader, "input1", AI_TYPE_RGBA, "colorA");
      ProcessParameter(shader, "input2", AI_TYPE_RGBA, "colorB");
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

      // Need to deal with alpha channel
      MPlug alphaAPlug = FindMayaPlug("alphaA");
      MPlugArray connections;
      alphaAPlug.connectedTo(connections, true, false);
      if (connections.length() > 0)
      {
         AtNode* srcArnoldNode = ExportConnectedNode(connections[0]);
         AiNodeLink(srcArnoldNode, "input1.a", shader);
      } else 
      {
         float alphaA = alphaAPlug.asFloat();
         if (alphaA < 1.f)
         {
            if (!linkedA)
            {
               AtRGB col = AiNodeGetRGB(shader, "input1");
               AiNodeSetRGBA(shader, "input1", col.r, col.g, col.b, alphaA);
            } else
            {
               // FIXME: color is connected and a transparent alpha is used, we need to insert a node
            }
         }
      }

      MPlug alphaBPlug = FindMayaPlug("alphaB");
      connections.clear();
      alphaBPlug.connectedTo(connections, true, false);
      if (connections.length() > 0)
      {
         AtNode* srcArnoldNode = ExportConnectedNode(connections[0]);
         AiNodeLink(srcArnoldNode, "input2.a", shader);
      } else 
      {
         float alphaB = alphaBPlug.asFloat();
         if (alphaB < 1.f)
         {
            if (!linkedB)
            {
               AtRGB col = AiNodeGetRGB(shader, "input2");
               AiNodeSetRGBA(shader, "input2", col.r, col.g, col.b, alphaB);
            } else
            {
               // FIXME: color is connected and a transparent alpha is used, we need to insert a node
            }
         }
      }
      
   } else
      CNodeTranslator::Export(shader);
}


