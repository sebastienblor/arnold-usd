#include "PxTranslator.h"

#include "nodes/ShaderUtils.h"
#include "nodes/shader/ArnoldShaderNode.h"
#include "translators/AutoDagTranslator.h"
#include "translators/camera/AutoCameraTranslator.h"
#include "translators/shader/ShaderTranslator.h"
#include "translators/driver/DriverTranslator.h"
#include "translators/filter/FilterTranslator.h"

// A translator proxy
CPxTranslator::CPxTranslator(const MString &translatorName,
                             const MString &providerName,
                             const MString &providerFile,
                             TCreatorFunction creatorFunction,
                             TNodeInitFunction nodeInitFunction)
{
   SetName(translatorName);
   arnold = "";
   provider = providerName;
   // if (name.numChars() == 0) name = provider;
   file = providerFile;
   creator = creatorFunction;
   initialize = nodeInitFunction;
}

MStatus CPxTranslator::ReadMetaData(const AtNodeEntry* arnoldNodeEntry, bool mappedMayaNode)
{
   arnold = AiNodeEntryGetName(arnoldNodeEntry);
   // If no name was specified, use metadata, or by default the extension name
   if (name.numChars() == 0)
   {
      const char* translatorName;
      if (AiMetaDataGetStr(arnoldNodeEntry, NULL, "maya.translator", &translatorName))
      {
         SetName(MString(translatorName));
      }
      else
      {
         SetName(provider);
      }
   }
   // If no explicit translator was specified, choose a default one using Arnold node type
   // TODO : use metadata for a finer choice of base translator classes ?
   if (NULL == creator)
   {
      MString arnoldNodeTypeName = AiNodeEntryGetTypeName(arnoldNodeEntry);
      if (arnoldNodeTypeName ==  "camera")
      {
         // TODO : define a non virtual generic CCameraTranslator
         creator = CAutoCameraTranslator::creator;
         if (mappedMayaNode)
            initialize = CNodeTranslator::NodeInitializer;
      }
      else if (arnoldNodeTypeName == "light")
      {
         // TODO : define a non virtual generic CLightTranslator
         // creator = CLightTranslator::creator;
         // initialize = CLightTranslator::NodeInitializer;
      }
      else if (arnoldNodeTypeName == "shader")
      {
         creator = CShaderTranslator::creator;
         if (mappedMayaNode)
            initialize = CNodeTranslator::NodeInitializer;
      }
      else if (arnoldNodeTypeName == "shape")
      {
         // TODO : define a non virtual generic CShapeTranslator or Geo
         creator = CAutoDagTranslator::creator;
         if (mappedMayaNode)
            initialize = CNodeTranslator::NodeInitializer;
      }
      else if (arnoldNodeTypeName == "driver")
      {
         creator = CDriverTranslator::creator;
         initialize = CDriverTranslator::NodeInitializer;
      }
      else if (arnoldNodeTypeName == "filter")
      {
         creator = CFilterTranslator::creator;
         initialize = CFilterTranslator::NodeInitializer;
      }
      // No default strategy to create the rest
   }
   
   return MStatus::kSuccess;
}
