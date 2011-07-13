#include "PxTranslator.h"

#include "nodes/ShaderUtils.h"
#include "nodes/shader/ArnoldShaderNode.h"
#include "translators/shader/ShaderTranslator.h"
#include "translators/driver/DriverTranslator.h"

// A Maya node proxy
CPxTranslator::CPxTranslator(const MString &translatorName,
                             const MString &providerName,
                             const MString &providerFile,
                             TCreatorFunction creatorFunction,
                             TNodeInitFunction nodeInitFunction)
{
   name = translatorName;
   arnold = "";
   provider = providerName;
   // if (name.numChars() == 0) name = provider;
   file = providerFile;
   creator = creatorFunction;
   initialize = nodeInitFunction;
}

MStatus CPxTranslator::ReadMetaData(const AtNodeEntry* arnoldNodeEntry)
{
   arnold = AiNodeEntryGetName(arnoldNodeEntry);
   // If no name was specified, use metadata, or by default the extension name
   if (name.numChars() == 0)
   {
      const char* translatorName;
      if (AiMetaDataGetStr(arnoldNodeEntry, NULL, "maya.translator", &translatorName))
      {
         name = MString(translatorName);
      }
      else
      {
         name = provider;
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
         // creator = CCameraTranslator::creator;
         // initialize = CCameraTranslator::NodeInitializer;
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
         // initialize = CShaderTranslator::NodeInitializer;
      }
      else if (arnoldNodeTypeName == "shape")
      {
         // TODO : define a non virtual generic CShapeTranslator or Geo
         // creator = CShapeTranslator::creator;
         // initialize = CShapeTranslator::NodeInitializer;
      }
      else if (arnoldNodeTypeName == "driver")
      {
         creator = CDriverTranslator::creator;
         initialize = CDriverTranslator::NodeInitializer;
      }
      else if (arnoldNodeTypeName == "filter")
      {
         creator = CDriverTranslator::creator;
         initialize = CDriverTranslator::NodeInitializer;
      }
      // No default strategy to create the rest
   }

   return MStatus::kSuccess;
}
