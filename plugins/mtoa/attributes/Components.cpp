#include "Components.h"
#include "attributes/AttrHelper.h"

const MStringArray& GetComponentNames(int arnoldParamType)
{
   MStringArray componentNames;
   switch (arnoldParamType)
   {
   case AI_TYPE_RGB:
      return RGB_COMPONENTS;
   case AI_TYPE_RGBA:
      return RGBA_COMPONENTS;
   case AI_TYPE_VECTOR:
      return VECTOR_COMPONENTS;
   case AI_TYPE_VECTOR2:
      return VECTOR2_COMPONENTS;
   // FIXME Arnold5 make sure we don't want any components with closures
   default:
      return INVALID_COMPONENTS;
   }
}

MString GetComponentName(int arnoldParamType, const MPlug &plug)
{
   const MStringArray names = GetComponentNames(arnoldParamType);
   int index = GetFloatComponentIndex(plug);

   if (index >= 0 && (unsigned int)index < names.length())
      return names[index];
   return "";
}

// returns 0-3 or -1 if failed
int GetFloatComponentIndex(const MPlug &plug)
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


ComponentType ResolveFloatComponent(const MPlug &plug, MPlug &attrResult)
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
