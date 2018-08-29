#include "SynColorTranslator.h"

#include <maya/MGlobal.h>
#include <maya/MSelectionList.h>

#include "utils/MakeTx.h"
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

      MString renderingSpace = defaultColorSettings.findPlug("workingSpaceName").asString();

      const bool cmOCIOEnabled = defaultColorSettings.findPlug("configFileEnabled").asBool();
      if(cmOCIOEnabled)
      {
         AiNodeSetStr (node, "config",             defaultColorSettings.findPlug("configFilePath").asString().asChar());
         AiNodeSetStr (node, "color_space_linear", renderingSpace.asChar());
      }
      else
      {
         // Take values from the defaultColorMgtGlobals node

         AiNodeSetStr (node, "rendering_color_space",   renderingSpace.asChar());

         // Find the native catalog location
         // 
         //   PH 2017-02-27: For Maya 2018, it could be far more robust to have a mel command
         //   
         //   However it's important to not take for granted that the transform path 
         //   is the default one (by only using MAYA_LOCATION). A user could manually change 
         //   the preferences to use a central repository so that all its facility or 
         //   all its render farm nodes have 'granted' access to the synColor catalog.
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

         if (nativeCatalogDir.length() == 0)
         {
            // Old versions of the synColor preferences were not including the catalog path
            MGlobal::executeCommand("getenv MAYA_LOCATION", nativeCatalogDir);
#ifdef _DARWIN
            nativeCatalogDir += "/../..";
#endif
            nativeCatalogDir += "/synColor";
         }

         AiNodeSetStr(node, "native_catalog_path", nativeCatalogDir.asChar());

         // Find the shared catalog location
         MString customCatalogDir;
         MGlobal::executeCommand("colorManagementCatalog -queryUserTransformPath", customCatalogDir);
         AiNodeSetStr(node, "custom_catalog_path", customCatalogDir.asChar());
      }
   }

   if (m_renderingSpaceChanged)
   {
      MGlobal::displayWarning("Updating All TX after rendering space change....");
      MGlobal::executeCommand("arnoldRenderView -status \"Updating All TX after rendering space change....\"");
      
      updateAllTx();
      m_renderingSpaceChanged = false;
   }
}

void CSynColorTranslator::NodeInitializer(CAbTranslator context)
{
}

void CSynColorTranslator::NodeChanged(MObject& node, MPlug& plug)
{
   if (plug.name() == MString("defaultColorMgtGlobals.workingSpaceName"))
      m_renderingSpaceChanged = true;

   CNodeTranslator::NodeChanged(node, plug);
}

AtNode* CSynColorTranslator::CreateArnoldNodes()
{
   MFnDependencyNode defaultColorSettings(GetMayaObject());
   m_renderingSpaceChanged = false;
   const bool cmEnabled = defaultColorSettings.findPlug("cmEnabled").asBool();
   if(cmEnabled)
   {
      const bool cmOCIOEnabled = defaultColorSettings.findPlug("configFileEnabled").asBool();
      if(cmOCIOEnabled)
      {
         return AddArnoldNode("color_manager_ocio");
      }
      else
      {
         return AddArnoldNode("color_manager_syncolor");
      }
   }
   else
   {
      return 0x0;
   }
}

