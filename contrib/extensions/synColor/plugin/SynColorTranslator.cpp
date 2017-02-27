#include "SynColorTranslator.h"

#include <maya/MGlobal.h>
#include <maya/MSelectionList.h>

#include "utils/time.h"

#include <fstream>


void CSynColorTranslator::Export(AtNode* node)
{
   MFnDependencyNode defaultColorSettings(GetMayaObject());
   const bool cmEnabled = defaultColorSettings.findPlug("cmEnabled").asBool();
   if(cmEnabled)
   {
      AtNode *options = AiUniverseGetOptions();
      AiNodeSetPtr(options, "color_manager", (void*)node);

      // Take values from the defaultColorMgtGlobals node

      const bool cmOCIOEnabled = defaultColorSettings.findPlug("configFileEnabled").asBool();
      if(cmOCIOEnabled)
      {
         AiNodeSetStr (node, "ocioconfig_path", defaultColorSettings.findPlug("configFilePath").asString().asChar());
      }
      AiNodeSetStr (node, "rendering_color_space",   defaultColorSettings.findPlug("workingSpaceName").asString().asChar());

      // Find the native catalog location
      //   PH 2017-02-27: For Maya 2018, it could be far more robust to have a mel command
      //
      MString userPrefsDir;
      MGlobal::executeCommand("internalVar -userPrefDir", userPrefsDir);
      userPrefsDir += "/synColorConfig.xml";

      MString nativeCatalogDir;
      std::ifstream ifs(userPrefsDir.asChar(), std::ifstream::in);
      char line[2048];
      while(ifs.good())
      {
         ifs.getline(line, sizeof(line));
         const std::string str(line);
         static const char *tag = "<TransformsHome dir=";
         const size_t found = str.find(tag);
         if(found!=std::string::npos)
         {
            const std::size_t end = str.find("/>", found);
            if(end!=std::string::npos)
            {
               const size_t pos = found + strlen(tag) + 1;
               const std::string path = str.substr(pos, end - pos - 2);

               nativeCatalogDir = path.c_str();
            }
            break;
         }
      }
      ifs.close();

      AiNodeSetStr(node, "native_catalog_path", nativeCatalogDir.asChar());

      // Find the shared catalog location
      MString customCatalogDir;
      MGlobal::executeCommand("colorManagementCatalog -queryUserTransformPath", customCatalogDir);
      AiNodeSetStr(node, "custom_catalog_path", customCatalogDir.asChar());
   }

   // I should know if my output driver needs the output transform or the view transform

   MSelectionList activeList;
   activeList.add(MString("defaultArnoldDriver"));

   int mode = 0; // 0 = no transform, 1 = view transform, 2 = output transform

   if (activeList.length() > 0 && cmEnabled)
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
         // Nothing to do.
      break;

      case 1:
         AiNodeSetStr (node, "output_color_space",      defaultColorSettings.findPlug("viewTransformName").asString().asChar());   
      break;

      case 2:
#if MAYA_API_VERSION >= 201800
         AiNodeSetBool(node, "output_color_conversion", defaultColorSettings.findPlug("outputTransformUseColorConversion").asBool());
#endif
         AiNodeSetStr (node, "output_color_space",      defaultColorSettings.findPlug("outputTransformName").asString().asChar());
      break;

   }
}

void CSynColorTranslator::NodeInitializer(CAbTranslator context)
{
}

AtNode* CSynColorTranslator::CreateArnoldNodes()
{
   MFnDependencyNode defaultColorSettings(GetMayaObject());
   const bool cmEnabled = defaultColorSettings.findPlug("cmEnabled").asBool();
   if(cmEnabled)
   {
      return AddArnoldNode("color_manager_syncolor");
   }
   else
   {
      return 0x0;
   }
}

