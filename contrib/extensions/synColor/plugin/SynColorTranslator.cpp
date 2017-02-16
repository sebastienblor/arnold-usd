#include "SynColorTranslator.h"

#include <maya/MGlobal.h>
#include <maya/MSelectionList.h>

#include "utils/time.h"


void CSynColorTranslator::Export(AtNode* node)
{
   AtNode *options = AiUniverseGetOptions();
   AiNodeSetPtr(options, "color_manager", (void*)node);

   // Take values from the defaultColorMgtGlobals node

   MFnDependencyNode defaultColorSettings(GetMayaObject());
   AiNodeSetBool(node, "enabled",                 defaultColorSettings.findPlug("cmEnabled").asBool());
   AiNodeSetBool(node, "ocioconfig_enabled",      defaultColorSettings.findPlug("configFileEnabled").asBool());
   AiNodeSetStr (node, "ocioconfig_path",         defaultColorSettings.findPlug("configFilePath").asString().asChar());
   AiNodeSetStr (node, "rendering_color_space",   defaultColorSettings.findPlug("workingSpaceName").asString().asChar());


   // I should know if my output driver needs the output transform or the view transform

   MSelectionList activeList;
   activeList.add(MString("defaultArnoldDriver"));

   int mode = 0; // 0 = no transform, 1 = view transform, 2 = output transform

   if (activeList.length() > 0)
   {
      MObject depNode;
      activeList.getDependNode(0, depNode);

      MPlug colorManagementPlug = MFnDependencyNode(depNode).findPlug("colorManagement");
      if (!colorManagementPlug.isNull())
         mode = colorManagementPlug.asInt();
   } 


   switch(mode)
   {
      default:
      case 0:
         AiNodeSetBool(node, "output_enabled", false);
         AiNodeSetBool(node, "output_color_conversion", false);    
      break;
      case 1:
         AiNodeSetBool(node, "output_enabled", true);
         AiNodeSetBool(node, "output_color_conversion", false);    
         AiNodeSetStr (node, "output_color_space",      defaultColorSettings.findPlug("viewTransformName").asString().asChar());   
      break;
      case 2:
         AiNodeSetBool(node, "output_enabled",          defaultColorSettings.findPlug("outputTransformEnabled").asBool());
#if MAYA_API_VERSION >= 201800    
         AiNodeSetBool(node, "output_color_conversion", defaultColorSettings.findPlug("outputTransformUseColorConversion").asBool());
#else
         AiNodeSetBool(node, "output_color_conversion", false);
#endif
         AiNodeSetStr (node, "output_color_space",      defaultColorSettings.findPlug("outputTransformName").asString().asChar());
      break;

   }

   // Find the catalog location

   MString userPrefsDir;
   MGlobal::executeCommand("internalVar -userPrefDir", userPrefsDir);
   userPrefsDir += "/synColorConfig.xml";
   AiNodeSetStr(node, "configuration_path", userPrefsDir.asChar());
}

void CSynColorTranslator::NodeInitializer(CAbTranslator context)
{
}

AtNode* CSynColorTranslator::CreateArnoldNodes()
{
   return AddArnoldNode("syncolor_color_manager");
}

