#include "Components.h"
#include "attributes/AttrHelper.h"


ComponentType IsFloatComponent(const MPlug &plug, MPlug &attrResult)
{
   MStatus stat;
   MFnNumericAttribute nAttr(plug.attribute(), &stat);
   if (stat == MS::kSuccess && nAttr.unitType() == MFnNumericData::kFloat)
   {
      if (plug.isChild())
      {
         // don't use the component name: translators expect to compare against the parent name
         attrResult = plug.parent();
         return COMPONENT_TYPE_NORMAL;
      }
      else if (plug.partialName(false, false, false, false, false, true) == OUT_ALPHA_NAME)
      {

         MFnDependencyNode fnNode(plug.node());
         attrResult = fnNode.findPlug(OUT_COLOR_NAME, stat);
         if (stat != MS::kSuccess)
         {
            AiMsgError("[mtoa] [maya %s] Error exporting components: \"%s\" is connected, but node has no \"%s\" attribute",
                       fnNode.name().asChar(), OUT_ALPHA_NAME.asChar(), OUT_COLOR_NAME.asChar());
            return COMPONENT_TYPE_INVALID;
         }
         return COMPONENT_TYPE_OUTALPHA;
      }
   }
   attrResult = plug;
   return COMPONENT_TYPE_NONE;
}

/// Insert conversion nodes between component outputs and their inputs
///
AtNode* InsertConversionNodes(const MPlug &shaderOutputPlug, ComponentType compMode, AtNode* shader)
{
   if (compMode == COMPONENT_TYPE_NONE)
      return NULL;
   // shaderOutputPlug type (float if we've gotten this far) should correspond to the output type of the arnold node
   AtInt connectedOutType = AiNodeEntryGetOutputType(shader->base_node);
   if (connectedOutType != AI_TYPE_FLOAT)
   {
      // If the connected shader's output type is anything other than float, the incoming
      // connection is likely a component (as maya will not allow a color to connect to a float, for example).
      // Insert a conversion node
      // find the child index:
      MPlug parentPlug;
      int childIndex = -1;

      if (compMode == COMPONENT_TYPE_NORMAL)
      {
         parentPlug = shaderOutputPlug.parent();
         for (unsigned int i=0; i < parentPlug.numChildren(); ++i)
         {
            if (parentPlug.child(i) == shaderOutputPlug)
            {
               childIndex = i;
               break;
            }
         }
      }
      else if (COMPONENT_TYPE_OUTALPHA)
      {
         assert(connectedOutType == AI_TYPE_RGBA);
         MStatus stat;
         parentPlug = MFnDependencyNode(shaderOutputPlug.node()).findPlug(OUT_COLOR_NAME, &stat);
         if (stat != MS::kSuccess)
            return NULL;
         childIndex = 3;
      }
      else
         return NULL;
      AtNode* conversionNode = NULL;
      // TODO: handle error case
      switch (connectedOutType)
      {
      case AI_TYPE_RGB:
      case AI_TYPE_RGBA:
         {
            conversionNode = AiNode("colorToFloat");
         }
         break;
      case AI_TYPE_VECTOR:
         {
            conversionNode = AiNode("vectorToFloat");
         }
         break;
      case AI_TYPE_POINT:
         {
            conversionNode = AiNode("pointToFloat");
         }
         break;
      case AI_TYPE_POINT2:
         {
            conversionNode = AiNode("point2ToFloat");
         }
         break;
      default:
         {
//                  AiMsgWarning("[mtoa] Attribute %s is not of a data type that supports component connections",
//                               plug.partialName(true, false, false, false, false, true).asChar());
//                  return false;
         }
      }
      if (conversionNode != NULL)
      {
         AiNodeSetInt(conversionNode, "component", childIndex);
         AiNodeLink(shader, "input", conversionNode);
         return conversionNode;
      }
   }
   return shader;
}

