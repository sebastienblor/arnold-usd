#include "SynColorTranslator.h"

#include <maya/MGlobal.h>

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
   AiNodeSetBool(node, "output_enabled",          defaultColorSettings.findPlug("outputTransformEnabled").asBool());
#if MAYA_API_VERSION >= 201800    
   AiNodeSetBool(node, "output_color_conversion", defaultColorSettings.findPlug("outputTransformUseColorConversion").asBool());
#else
   AiNodeSetBool(node, "output_color_conversion", false);
#endif
   AiNodeSetStr (node, "output_color_space",      defaultColorSettings.findPlug("outputTransformName").asString().asChar());

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

