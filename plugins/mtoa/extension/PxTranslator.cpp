#include "PxTranslator.h"

#include "nodes/ShaderUtils.h"
#include "nodes/shader/ArnoldShaderNode.h"
#include "translators/shader/ShaderTranslator.h"

// A Maya node proxy
CPxTranslator::CPxTranslator(const MString &translatorName,
                             const MString &arnoldNodeName,
                             const MString &providerName,
                             const MString &providerFile,
                             TCreatorFunction creatorFunction,
                             TNodeInitFunction nodeInitFunction)
{
   name = translatorName;
   arnold = arnoldNodeName;
   provider = providerName;
   // if (name.numChars() == 0) name = provider;
   file = providerFile;
   creator = creatorFunction;
   initialize = nodeInitFunction;
}

CPxTranslator::CPxTranslator(const MString &translatorName,
                             const AtNodeEntry* arnoldNodeEntry,
                             const MString &providerName,
                             const MString &providerFile,
                             TCreatorFunction creatorFunction,
                             TNodeInitFunction nodeInitFunction)
{
   name = translatorName;
   arnold = AiNodeEntryGetName(arnoldNodeEntry);
   provider = providerName;
   // if (name.numChars() == 0) name = provider;
   file = providerFile;
   creator = creatorFunction;
   initialize = nodeInitFunction;
}

MStatus CPxTranslator::ReadMetaData()
{
   const AtNodeEntry* arnoldNodeEntry = NULL;
   arnoldNodeEntry = AiNodeEntryLookUp(arnold.asChar());
   if (NULL == arnoldNodeEntry)
   {
      AiMsgError("[mtoa] [%s] Arnold node %s does not exist", provider.asChar(), arnold.asChar());
      return MStatus::kInvalidParameter;
   }

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
      const char* arnoldNodeTypeName;
      arnoldNodeTypeName = AiNodeEntryGetTypeName(arnoldNodeEntry);
      if (strcmp(arnoldNodeTypeName, "camera") == 0)
      {
         // TODO : define a non virtual generic CCameraTranslator
         // creator = CCameraTranslator::creator;
         // initialize = CCameraTranslator::NodeInitializer;
      }
      else if (strcmp(arnoldNodeTypeName,"light") == 0)
      {
         // TODO : define a non virtual generic CLightTranslator
         // creator = CLightTranslator::creator;
         // initialize = CLightTranslator::NodeInitializer;
      }
      else if (strcmp(arnoldNodeTypeName,"shader") == 0)
      {
         creator = CShaderTranslator::creator;
         // initialize = CShaderTranslator::NodeInitializer;
      }
      else if (strcmp(arnoldNodeTypeName,"shape") == 0)
      {
         // TODO : define a non virtual generic CShapeTranslator or Geo
         // creator = CShapeTranslator::creator;
         // initialize = CShapeTranslator::NodeInitializer;
      }
      // No default strategy to create the rest
   }

   return MStatus::kSuccess;
}
